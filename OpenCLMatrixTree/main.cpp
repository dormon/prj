#include <CL/cl.hpp>
#include <cassert>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <vector>
#include <glm/glm.hpp>

#include <CLQuick.h>
#include <Timer.h>
#include <CommandLineArguments.h>
#include <DAG.h>

// ABDCEFG xDCEFG xDyG
// ABCEF   xCEF   xy
//
//
//
//
//
//
//
//
//



std::string static const kernel_code =
    R".(
kernel void oneThreadPerOutput(
  global float      *output    ,
  global float const*matrix    ,
  global float const*input     ,
         uint  const inputSize ,
         uint  const outputSize){
  uint outputIndex = get_local_id(0);
  float result = 0.f;
  for(uint inputIndex = 0; inputIndex < inputSize; ++inputIndex)
    result += input[inputIndex] * matrix[outputIndex * inputSize + inputIndex];
    
}
).";

void printPlatformNames(std::vector<cl::Platform> const& platforms) {
  for (auto const& platform : platforms)
    std::cout << platform.getInfo<CL_PLATFORM_NAME>() << std::endl;
}

void printDeviceNames(std::vector<cl::Device> const& devices) {
  for (auto const& device : devices)
    std::cout << device.getInfo<CL_DEVICE_NAME>() << std::endl;
}

/*
class Node{
  public:
    Node(Graph*graph);
    glm::mat4 getMatrix()const{return matrix;}
    void setMatrix(glm::mat4 const&m){matrix = m;}
    size_t nofChilds()const{return childs.size();}
    Node*getChild(size_t i)const{
      if(i>=nofChilds())throw std::invalid_argument("child id is too large");
      return childs.at(i);
    }
    size_t addChild(Node*const child){
      auto const it = childToId.find(child);
      if(it != childToId.end())return it->second;
      auto const id = nofChilds();
      childs.push_back(child);
      childToId[child] = id;
      child->addParent(this);
      return id;
    }
    void removeChild(Node*const child){
    }

    size_t nofParents()const{return parents.size();}
    Node*getParent(size_t i)const{
      if(i>=nofParents())throw std::invalid_argument("parent id is too large");
      return parents.at(i);
    }
    size_t addParent(Node*const parent){
      auto const it = parentToId.find(parent);
      if(it != parentToId.end())return it->second;
      auto const id = nofParents();
      parents.push_back(parent);
      parentToId[parent] = id;
      parent->addChild(this);
      return id;
    }
    void removeParent(size_t i){
      if(i>=nofParents())throw std::invalid_argument("parent id is too large");

    }
  protected:
    Graph*graph;
    glm::mat4 matrix;
    std::vector<Node*>childs;
    std::vector<Node*>parents;
    std::map<Node*,size_t>childToId;
    std::map<Node*,size_t>parentToId;
};

class Graph{
  public:
    size_t addRoot();
    void removeRoot(size_t id);
    std::map<size_t,std::shared_ptr<Node>>roots;
    std::map<size_t,Node*>id2Node;
    std::map<Node*,size_t>node2Id;
    void registerNode(Node*n){
      auto const id = id2Node.size();
      id2Node[id] = n;
      node2Id[n] = id;
    }
    friend class Graph;
};

Node::Node(Graph*graph):graph(graph){graph->registerNode(this);}

float randomf(float minVal = 0.f,float maxVal = 1.f){
  auto const uniform = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
  return uniform * (maxVal - minVal) + minVal;
}

glm::mat4 generateMatrix(){
  glm::mat4 result;
  for(int i=0;i<4;++i)
    for(int j=0;j<4;++j)
      result[i][j] = randomf();
  return result;
};

void generateNTreeNode(std::shared_ptr<Node>const&node,Node*parent,size_t depth,size_t branching){
  if(depth == 0)return;
  for(size_t i=0;i<branching;++i)
    node->childs.insert(std::make_shared<Node>());
  node->parents.insert(parent);
  node->matrix = generateMatrix();
  for(auto const&n:node->childs)
    generateNTreeNode(n,&*node,depth-1,branching);
}

void generateNTree(TreeGraph&graph,size_t depth,size_t branching){
  if(depth == 0)throw std::invalid_argument("depth has to be at least 1");
  if(branching == 0)throw std::invalid_argument("branching has to be at least 1");
  auto root = std::make_shared<Node>();
  root->matrix = generateMatrix();
  depth--;
  generateNTreeNode(root,nullptr,depth,branching);
  graph.roots.insert(root);
}
*/

struct CommandLineArguments{
  Arguments args;
  size_t    selectedPlatformId;
  size_t    selectedDeviceId;
  bool      printPlatforms;
  bool      printDevices;
  size_t    inputSize;
  size_t    outputSize;
  size_t    workGroupSizeX;
  size_t    workGroupSizeY;
  bool      printHelp;
  std::string kernelName;
  CommandLineArguments(int argc, char* argv[]){
    selectedPlatformId = getArgument<size_t>(args, argc, argv, "--platform", 0,"selected opencl platform id");
    selectedDeviceId   = getArgument<size_t>(args, argc, argv, "--device", 0,"selected opencl device id");
    printPlatforms     = isArgumentPresent(args, argc, argv, "--printPlatforms","print available platforms");
    printDevices       = isArgumentPresent(args, argc, argv, "--printDevices","print available devices");
    inputSize          = getArgument<size_t>(args, argc, argv, "--inputSize", 1024,"size of input vector");
    outputSize         = getArgument<size_t>(args, argc, argv, "--outputSize", 1024,"size of output vector");
    workGroupSizeX     = getArgument<size_t>(args, argc, argv, "--wgsX", 64,"work group size x");
    workGroupSizeY     = getArgument<size_t>(args, argc, argv, "--wgsY", 1,"work group size y");
    printHelp          = isArgumentPresent(args, argc, argv, "--help","print this help");
    kernelName         = getArgument<std::string>(args,argc,argv,"--kernelName","oneThreadPerOutput","kernel name: oneThreadPerOutput");

    if (printHelp) {
      for (auto const& x : args) std::cout << x.second << std::endl;
      exit(EXIT_SUCCESS);
    }
  }
};

struct OpenCLVars {
  cl::Platform     platform;
  cl::Device       device;
  cl::Context      context;
  cl::CommandQueue queue;
  cl::Program      program;
  cl::Kernel       kernel;
  OpenCLVars(size_t selectedPlatformId,size_t selectedDeviceId,bool printPlatforms,bool printDevices,std::string const&source,std::string const&kernelName,std::string const&options = "") {
    auto const platforms = clq::getPlatforms();

    if (printPlatforms) {
      printPlatformNames(platforms);
      exit(EXIT_SUCCESS);
    }

    platform           = platforms.at(selectedPlatformId);
    auto const devices = clq::getDevices(platform);

    if (printDevices) {
      printDeviceNames(devices);
      exit(EXIT_SUCCESS);
    }

    device  = devices.at(selectedDeviceId);
    context = clq::createContext({device});
    queue   = clq::createCommandQueue(context, device);
    program = clq::createProgram(context, {device}, {source},options);
    kernel = clq::createKernel(program, kernelName);
  }
};

int main(int argc, char* argv[]) {
  try {
    CommandLineArguments args(argc, argv);
    OpenCLVars           clv{args.selectedPlatformId,args.selectedDeviceId,args.printPlatforms,args.printDevices,kernel_code,args.kernelName};

    auto inputBuffer =
        clq::createBuffer(clv.context, sizeof(float) * args.inputSize);
    auto outputBuffer =
        clq::createBuffer(clv.context, sizeof(float) * args.outputSize);
    auto matrixBuffer = clq::createBuffer(
        clv.context, sizeof(float) * args.inputSize * args.outputSize);
    clq::setArg(clv.kernel, 0, outputBuffer);
    clq::setArg(clv.kernel, 1, matrixBuffer);
    clq::setArg(clv.kernel, 2, inputBuffer);
    clq::setArg(clv.kernel, 3, static_cast<cl_uint>(args.inputSize));
    clq::setArg(clv.kernel, 4, static_cast<cl_uint>(args.outputSize));
    Timer<float> timer{};
    clv.queue.enqueueNDRangeKernel(
        clv.kernel, cl::NullRange, cl::NDRange(args.inputSize),
        cl::NDRange(args.workGroupSizeX, args.workGroupSizeY));
    clv.queue.finish();
    std::cout << timer.elapsedFromStart() << std::endl;
  } catch (std::runtime_error& e) {
    std::cerr << e.what() << std::endl;
  }
  return EXIT_SUCCESS;
}
