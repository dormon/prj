#include<CL/cl.hpp>

#include<OpenCLInferenceEngine/Engine.h>
#include<OpenCLInferenceEngine/OpenCLHelper.h>
#include<OpenCLInferenceEngine/Kernels.h>
#include<OpenCLInferenceEngine/OpenCLLayers.h>

#include<TextUtils/Utils.h>

std::string getIndentation(size_t size){
  std::stringstream ss;
  for(size_t i=0;i<size;++i)
    ss << " ";
  return ss.str();
}

std::string addIndentation(std::string const&txt,size_t size = 0){
  auto const indentation = getIndentation(size);
  auto lines = splitString(txt,"\n");

  std::stringstream ss;
  for(auto const&l:lines)
    ss << indentation << l << std::endl;
  return ss.str();  
}

std::string getDeviceInfo(cl::Device const&d){
  std::stringstream ss;
  ss << d.getInfo<CL_DEVICE_NAME>() << std::endl;
  return ss.str();
}

std::string getContextInfo(cl::Context const&c){
  std::stringstream ss;

  std::vector<cl::Device>devs;
  auto err = c.getInfo(CL_CONTEXT_DEVICES,&devs);
  if(err != CL_SUCCESS){
    ss << "devices cannot be obtained" << std::endl;
  }else{
    for(auto const&d:devs)
      ss << addIndentation(getDeviceInfo(d),2);
  }
  return ss.str();
}

std::string getEventInfo(cl::Event const&e){
  std::stringstream ss;
  cl::Context ctx;
  auto err = e.getInfo(CL_EVENT_CONTEXT,&ctx);
  if(err != CL_SUCCESS){
    ss << "context cannot be obtained: " << clErrorCodes.at(err) << std::endl;
  }else{
    ss << addIndentation(getContextInfo(ctx),2);
  }
  return ss.str();
}

namespace occ{

  class Command{
    public:
      enum Type{
        INPUT  ,
        COMPUTE,
      };
      virtual ~Command(){}
      Command(Type t,std::string const&n):type(t),name(n){}
      Type        type  ;
      std::string name  ;
      cl::Buffer  output;
      virtual void enqueue(cl::CommandQueue&) = 0;
  };

  class Input: public Command{
    public:
      Input(std::string const&n):Command(INPUT,n){}
      virtual void enqueue(cl::CommandQueue&){}
  };

  class ComputeCommand: public Command{
    public:
      enum Type{
        CONV_2D              ,
        BIAS                 ,
        BATCH_NORMALIZATION  ,
        ACTIVATION           ,
        MAX_POOLING_2D       ,
        GLOBAL_MAX_POOLING_2D,
        DENSE                ,
        SOFTMAX_NORMALIZATION,
      };
      Type                                   type        ;
      std::vector<cl::Buffer>                inputBuffers;
      cl::Kernel                             kernel      ;
      cl::NDRange                            local       ;
      cl::NDRange                            global      ;
      std::shared_ptr<cl::Event>             outputEvent ;
      std::vector<std::shared_ptr<cl::Event>>inputEvents ;
      ComputeCommand(Type t,std::string const&n):Command(COMPUTE,n),type(t){}
      virtual void enqueue(cl::CommandQueue&queue)override{

        std::vector<cl::Event>inputEventsForEnqueue;
        for(auto const&x:inputEvents)
          inputEventsForEnqueue.push_back(*x);

        cl_int err = queue.enqueueNDRangeKernel(
            kernel,
            cl::NullRange,
            global,
            local,
            &inputEventsForEnqueue,
            &*outputEvent);

        if(err != CL_SUCCESS){
          std::stringstream ss;
          ss << "enqueueing: " << this->name << std::endl;
          ss << "global: " << global[0] << " " << global[1] << std::endl;
          ss << "local : " << local [0] << " " << local [1] << std::endl;
          ss << "enqueueNDRangeKernel failed: " << clErrorCodes.at(err) << std::endl;
          ss << "output event: " << std::endl;
          ss << addIndentation(getEventInfo(*outputEvent),2);
          ss << "nofInputEvents: " << inputEvents.size() << std::endl;
          size_t eventCounter = 0;
          for(auto const&e:inputEvents){
            ss << "input event " << eventCounter++ << ": " << std::endl;
            ss << addIndentation(getEventInfo(*e),2);
          }
          throw ss.str();
        }
      }
  };

  class Conv2D: public ComputeCommand{
    public:
      cl::Buffer              weights   ;
      cl::Buffer              biases    ;
      cl_uint                 nofLayers ;
      cl_uint                 nofFilters;
      std::array<cl_uint,2>   imageSize ;
      std::array<cl_uint,2>   kernelSize;
      Conv2D(std::string const&n):ComputeCommand(CONV_2D,n){}
  };

  class Bias: public ComputeCommand{
    public:
      cl::Buffer biases;
      std::array<cl_uint,2>imageSize;
      cl_uint              nofLayers;
      Bias(std::string const&n):ComputeCommand(BIAS,n){}
  };

  class BatchNormalization: public ComputeCommand{
    public:
      cl::Buffer           parameters;
      cl_uint              nofLayers ;
      std::array<cl_uint,2>imageSize ;
      cl_float             epsilon   ;
      BatchNormalization(std::string const&n):ComputeCommand(BATCH_NORMALIZATION,n){}
  };

  class Activation: public ComputeCommand{
    public:
      cl_uint                 nofLayers ;
      std::array<cl_uint,2>   imageSize ;
      cnnConf::ActivationType activation;
      Activation(std::string const&n):ComputeCommand(ACTIVATION,n){}
  };

  class MaxPooling: public ComputeCommand{
    public:
      cl_uint              nofLayers      ;
      std::array<cl_uint,2>inputImageSize ;
      std::array<cl_uint,2>outputImageSize;
      std::array<cl_uint,2>stride         ;
      MaxPooling(std::string const&n):ComputeCommand(MAX_POOLING_2D,n){}
  };

  class GlobalMaxPooling: public ComputeCommand{
    public:
      cl_uint              nofLayers;
      std::array<cl_uint,2>imageSize;
      GlobalMaxPooling(std::string const&n):ComputeCommand(GLOBAL_MAX_POOLING_2D,n){}
  };

  class Dense: public ComputeCommand{
    public:
      cl::Buffer              weights   ;
      cl::Buffer              biases    ;
      cl_uint                 inputSize ;
      cl_uint                 outputSize;
      cnnConf::ActivationType activation;
      Dense(std::string const&n):ComputeCommand(DENSE,n){}
  };

  class SoftmaxNormalization: public ComputeCommand{
    public:
      cl_uint size;
      SoftmaxNormalization(std::string const&n):ComputeCommand(SOFTMAX_NORMALIZATION,n){}
  };

  class CommandList{
    public:
      std::vector<std::shared_ptr<occ::Command>>commands;
      std::map<std::string,std::shared_ptr<occ::Command>>allCommands;
  };
}

size_t divRoundUp(size_t value,size_t divisor){
  return (value/divisor) + static_cast<size_t>((value%divisor) != 0);
}

size_t roundToClosestMultipleOf(size_t value,size_t multiplier){
  return divRoundUp(value,multiplier)*multiplier;
} 

template<typename TYPE>
TYPE multiplyElements(std::vector<TYPE>const&v){
  return std::accumulate(std::cbegin(v),std::cend(v),1,std::multiplies<TYPE>());
}

void createOutputBuffer(
    std::shared_ptr<occ::Command>const&cmd,
    std::shared_ptr<ocl::Layer>const&layer,
    cl::Context const&ctx){
  assert(cmd != nullptr);
  assert(layer != nullptr);

  auto const size = multiplyElements(layer->outputSize);

  auto const buffer = clh::createBuffer()
    .givenSize(size*sizeof(float))
    .givenContext(ctx)();

  cmd->output = buffer;
}

std::shared_ptr<occ::Command>createInputCommand(
    std::shared_ptr<ocl::Layer>const&layer,
    cl::Context const&ctx){
  assert(layer != nullptr);
  auto const typedLayer = std::dynamic_pointer_cast<ocl::Input>(layer);
  assert(typedLayer != nullptr);


  auto const command = std::make_shared<occ::Input>(layer->name);
  assert(command != nullptr);
  createOutputBuffer(command,layer,ctx);
  return command;
}

cl::Buffer createBufferFromTensor(cl::Context const&ctx,Tensor<float>const&t){
  auto const size = multiplyElements(t.size);
  return clh::createBuffer()
    .givenContext(ctx)
    .givenSize(size*sizeof(float))
    .givenFlags(CL_MEM_COPY_HOST_PTR)
    .givenData(const_cast<float*>(t.data.data()))();
}


std::shared_ptr<occ::Command>createCond2DCommand(
    std::shared_ptr<ocl::Layer>const&layer,
    cl::Context const&ctx){
  assert(layer != nullptr);
  auto const typedLayer = std::dynamic_pointer_cast<ocl::Conv2D>(layer);
  assert(typedLayer != nullptr);

  auto const command = std::make_shared<occ::Conv2D>(layer->name);
  assert(command != nullptr);
  createOutputBuffer(command,layer,ctx);
  command->outputEvent = std::make_shared<cl::Event>();

  auto const&weights = typedLayer->weights;
  command->weights = createBufferFromTensor(ctx,weights);
  assert(layer->inputs.size() == 1);
  auto const inputLayer = std::dynamic_pointer_cast<ocl::Layer>(layer->inputs.at(0));
  assert(inputLayer != nullptr);
  assert(inputLayer->outputSize.size() == 3);
  command->nofLayers     = static_cast<cl_uint>(inputLayer->outputSize.at(2));
  command->nofFilters    = static_cast<cl_uint>(weights.size.at(3));
  command->imageSize [0] = static_cast<cl_uint>(inputLayer->outputSize.at(0));
  command->imageSize [1] = static_cast<cl_uint>(inputLayer->outputSize.at(1));
  command->kernelSize[0] = static_cast<cl_uint>(weights.size.at(0));
  command->kernelSize[1] = static_cast<cl_uint>(weights.size.at(1));

  return command;
}

std::shared_ptr<occ::Command>createBiasCommand(
    std::shared_ptr<ocl::Layer>const&layer,
    cl::Context const&ctx){
  assert(layer != nullptr);
  auto const typedLayer = std::dynamic_pointer_cast<ocl::Bias>(layer);
  assert(typedLayer != nullptr);

  auto const command = std::make_shared<occ::Bias>(layer->name);
  assert(command != nullptr);
  createOutputBuffer(command,layer,ctx);
  command->outputEvent = std::make_shared<cl::Event>();

  auto const&biases = typedLayer->biases;
  command->biases = createBufferFromTensor(ctx,biases);

  assert(layer->inputs.size() == 1);
  auto const inputLayer = std::dynamic_pointer_cast<ocl::Layer>(layer->inputs.at(0));
  assert(inputLayer != nullptr);
  assert(inputLayer->outputSize.size() == 3);
  command->nofLayers     = static_cast<cl_uint>(inputLayer->outputSize.at(2));
  command->imageSize [0] = static_cast<cl_uint>(inputLayer->outputSize.at(0));
  command->imageSize [1] = static_cast<cl_uint>(inputLayer->outputSize.at(1));

  return command;
}

std::shared_ptr<occ::Command>createBatchNormalizationCommand(
    std::shared_ptr<ocl::Layer>const&layer,
    cl::Context const&ctx){
  assert(layer != nullptr);
  auto const typedLayer = std::dynamic_pointer_cast<ocl::BatchNormalization>(layer);
  assert(typedLayer != nullptr);

  auto const command = std::make_shared<occ::BatchNormalization>(layer->name);
  assert(command != nullptr);
  createOutputBuffer(command,layer,ctx);
  command->outputEvent = std::make_shared<cl::Event>();

  auto const&parameters = typedLayer->parameters;
  command->parameters = createBufferFromTensor(ctx,parameters);
  assert(layer->inputs.size() == 1);
  auto const inputLayer = std::dynamic_pointer_cast<ocl::Layer>(layer->inputs.at(0));
  assert(inputLayer != nullptr);
  assert(inputLayer->outputSize.size() == 3);
  command->nofLayers    = static_cast<cl_uint>(inputLayer->outputSize.at(2));
  command->imageSize[0] = static_cast<cl_uint>(inputLayer->outputSize.at(0));
  command->imageSize[1] = static_cast<cl_uint>(inputLayer->outputSize.at(1));
  command->epsilon      = typedLayer->epsilon;

  return command;
}

std::shared_ptr<occ::Command>createActivationCommand(
    std::shared_ptr<ocl::Layer>const&layer,
    cl::Context const&ctx){
  assert(layer != nullptr);
  auto const typedLayer = std::dynamic_pointer_cast<ocl::Activation>(layer);
  assert(typedLayer != nullptr);

  auto const command = std::make_shared<occ::Activation>(layer->name);
  assert(command != nullptr);
  createOutputBuffer(command,layer,ctx);
  command->outputEvent = std::make_shared<cl::Event>();

  assert(layer->inputs.size() == 1);
  auto const inputLayer = std::dynamic_pointer_cast<ocl::Layer>(layer->inputs.at(0));
  assert(inputLayer != nullptr);
  assert(inputLayer->outputSize.size() == 3);
  command->nofLayers    = static_cast<cl_uint>(inputLayer->outputSize.at(2));
  command->imageSize[0] = static_cast<cl_uint>(inputLayer->outputSize.at(0));
  command->imageSize[1] = static_cast<cl_uint>(inputLayer->outputSize.at(1));
  command->activation   = typedLayer->activation;

  return command;
}

std::shared_ptr<occ::Command>createMaxPoolingCommand(
    std::shared_ptr<ocl::Layer>const&layer,
    cl::Context const&ctx){
  assert(layer != nullptr);
  auto const typedLayer = std::dynamic_pointer_cast<ocl::MaxPooling>(layer);
  assert(typedLayer != nullptr);

  auto const command = std::make_shared<occ::MaxPooling>(layer->name);
  assert(command != nullptr);
  createOutputBuffer(command,layer,ctx);
  command->outputEvent = std::make_shared<cl::Event>();

  assert(layer->inputs.size() == 1);
  auto const inputLayer = std::dynamic_pointer_cast<ocl::Layer>(layer->inputs.at(0));
  assert(inputLayer != nullptr);
  assert(inputLayer->outputSize.size() == 3);
  assert(layer->outputSize.size() == 3);
  command->nofLayers          = static_cast<cl_uint>(inputLayer->outputSize.at(2));
  command->inputImageSize[0]  = static_cast<cl_uint>(inputLayer->outputSize.at(0));
  command->inputImageSize[1]  = static_cast<cl_uint>(inputLayer->outputSize.at(1));
  command->outputImageSize[0] = static_cast<cl_uint>(layer->outputSize.at(0)     );
  command->outputImageSize[1] = static_cast<cl_uint>(layer->outputSize.at(1)     );
  command->stride[0]          = static_cast<cl_uint>(typedLayer->strideX         );
  command->stride[1]          = static_cast<cl_uint>(typedLayer->strideY         );

  return command;
}

std::shared_ptr<occ::Command>createGlobalMaxPoolingCommand(
    std::shared_ptr<ocl::Layer>const&layer,
    cl::Context const&ctx){
  assert(layer != nullptr);
  auto const typedLayer = std::dynamic_pointer_cast<ocl::GlobalMaxPooling>(layer);
  assert(typedLayer != nullptr);

  auto const command = std::make_shared<occ::GlobalMaxPooling>(layer->name);
  assert(command != nullptr);
  createOutputBuffer(command,layer,ctx);
  command->outputEvent = std::make_shared<cl::Event>();

  assert(layer->inputs.size() == 1);
  auto const inputLayer = std::dynamic_pointer_cast<ocl::Layer>(layer->inputs.at(0));
  assert(inputLayer != nullptr);
  assert(inputLayer->outputSize.size() == 3);
  command->nofLayers    = static_cast<cl_uint>(inputLayer->outputSize.at(2));
  command->imageSize[0] = static_cast<cl_uint>(inputLayer->outputSize.at(0));
  command->imageSize[1] = static_cast<cl_uint>(inputLayer->outputSize.at(1));

  return command;
}

std::shared_ptr<occ::Command>createDenseCommand(
    std::shared_ptr<ocl::Layer>const&layer,
    cl::Context const&ctx){
  assert(layer != nullptr);
  auto const typedLayer = std::dynamic_pointer_cast<ocl::Dense>(layer);
  assert(typedLayer != nullptr);

  auto const command = std::make_shared<occ::Dense>(layer->name);
  assert(command != nullptr);
  createOutputBuffer(command,layer,ctx);
  command->outputEvent = std::make_shared<cl::Event>();

  command->weights = createBufferFromTensor(ctx,typedLayer->weights);
  command->biases  = createBufferFromTensor(ctx,typedLayer->biases );
  assert(layer->inputs.size() == 1);
  auto const inputLayer = std::dynamic_pointer_cast<ocl::Layer>(layer->inputs.at(0));
  assert(inputLayer != nullptr);
  assert(inputLayer->outputSize.size() == 1);
  assert(layer->outputSize.size() == 1);
  command->inputSize  = static_cast<cl_uint>(inputLayer->outputSize.at(0));
  command->outputSize = static_cast<cl_uint>(layer->outputSize.at(0)     );
  command->activation = typedLayer->activation;

  return command;
}

std::shared_ptr<occ::Command>createSoftmaxNormalizationCommand(
    std::shared_ptr<ocl::Layer>const&layer,
    cl::Context const&ctx){
  assert(layer != nullptr);
  auto const typedLayer = std::dynamic_pointer_cast<ocl::SoftmaxNormalization>(layer);
  assert(typedLayer != nullptr);

  auto const command = std::make_shared<occ::SoftmaxNormalization>(layer->name);
  assert(command != nullptr);
  createOutputBuffer(command,layer,ctx);
  command->outputEvent = std::make_shared<cl::Event>();

  assert(layer->inputs.size() == 1);
  auto const inputLayer = std::dynamic_pointer_cast<ocl::Layer>(layer->inputs.at(0));
  assert(inputLayer != nullptr);
  assert(inputLayer->outputSize.size() == 1);
  command->size = static_cast<cl_uint>(inputLayer->outputSize.at(0));

  return command;
}

std::shared_ptr<occ::Command>createComputeCommand(
    std::shared_ptr<ocl::Layer>const&layer,
    cl::Context const&ctx){
  switch(layer->type){
    case ocl::Layer::CONV_2D              :
      return createCond2DCommand(layer,ctx);
    case ocl::Layer::BIAS                 :
      return createBiasCommand(layer,ctx);
    case ocl::Layer::BATCH_NORMALIZATION  :
      return createBatchNormalizationCommand(layer,ctx);
    case ocl::Layer::ACTIVATION           :
      return createActivationCommand(layer,ctx);
    case ocl::Layer::MAX_POOLING_2D       :
      return createMaxPoolingCommand(layer,ctx);
    case ocl::Layer::GLOBAL_MAX_POOLING_2D:
      return createGlobalMaxPoolingCommand(layer,ctx);
    case ocl::Layer::DENSE                :
      return createDenseCommand(layer,ctx);
    case ocl::Layer::SOFTMAX_NORMALIZATION:
      return createSoftmaxNormalizationCommand(layer,ctx);
    case ocl::Layer::INPUT:
      assert(false);
      return nullptr;
  }
  return nullptr;
}

void addComputeCommandInputBuffers(
    std::shared_ptr<occ::CommandList>const&commands,
    std::shared_ptr<ocl::Layer>const&layer,
    std::shared_ptr<occ::Command>const&command){
  assert(command != nullptr);
  auto const computeCommand = std::dynamic_pointer_cast<occ::ComputeCommand>(command);

  for(auto const&x:layer->inputs){
    assert(x != nullptr);
    auto const inputLayer = std::dynamic_pointer_cast<ocl::Layer>(x);
    assert(inputLayer != nullptr);
    auto const inputCommand = commands->allCommands.at(inputLayer->name);
    assert(inputCommand != nullptr);
    computeCommand->inputBuffers.push_back(inputCommand->output);
  }
}

void addComputeCommandInputEvents(
    std::shared_ptr<occ::CommandList>const&commands,
    std::shared_ptr<ocl::Layer>const&layer,
    std::shared_ptr<occ::Command>const&command){
  assert(command != nullptr);
  auto const computeCommand = std::dynamic_pointer_cast<occ::ComputeCommand>(command);

  for(auto const&x:layer->inputs){
    assert(x != nullptr);
    auto const inputLayer = std::dynamic_pointer_cast<ocl::Layer>(x);
    assert(inputLayer != nullptr);

    //input layer does not have output events
    if(inputLayer->type == ocl::Layer::INPUT)continue;

    auto const inputCommand = commands->allCommands.at(inputLayer->name);
    assert(inputCommand != nullptr);
    auto const inputComputeCommand = std::dynamic_pointer_cast<occ::ComputeCommand>(inputCommand);
    assert(inputComputeCommand != nullptr);
    computeCommand->inputEvents.push_back(inputComputeCommand->outputEvent);
  }
}

void createCommand(
    std::shared_ptr<occ::CommandList>const&commands,
    std::shared_ptr<ocl::Layer>const&layer,
    cl::Context const&ctx);

void createInputsCommands(
    std::shared_ptr<occ::CommandList>const&commands,
    std::shared_ptr<ocl::Layer>const&layer,
    cl::Context const&ctx){
  assert(commands != nullptr);
  assert(layer != nullptr);

  for(auto const&x:layer->inputs){
    assert(x != nullptr);
    auto const inputLayer = std::dynamic_pointer_cast<ocl::Layer>(x);
    assert(inputLayer != nullptr);
    createCommand(commands,inputLayer,ctx);
  }
}

std::shared_ptr<occ::Command>createLayerCommand(
    std::shared_ptr<occ::CommandList>const&commands,
    std::shared_ptr<ocl::Layer>const&layer,
    cl::Context const&ctx){
  assert(commands != nullptr);
  assert(layer != nullptr);

  std::shared_ptr<occ::Command>command;
  if(layer->type == ocl::Layer::INPUT)
    command = createInputCommand(layer,ctx);
  else{
    command = createComputeCommand(layer,ctx);
    addComputeCommandInputBuffers(commands,layer,command);
    addComputeCommandInputEvents (commands,layer,command);
  }
  return command;
}

void createCommand(
    std::shared_ptr<occ::CommandList>const&commands,
    std::shared_ptr<ocl::Layer>const&layer,
    cl::Context const&ctx){
  assert(commands != nullptr);
  assert(layer != nullptr);

  bool alreadyCreated = commands->allCommands.count(layer->name) > 0;
  if(alreadyCreated)return;

  createInputsCommands(commands,layer,ctx);

  auto const command = createLayerCommand(commands,layer,ctx);

  commands->allCommands[layer->name] = command;
  commands->commands.push_back(command);

}

void createConv2DKernel(
    std::shared_ptr<occ::ComputeCommand>const&cmd,
    cl::Program const&prg,
    OpenCLInferenceEngine::Config const&cfg){
  assert(cmd != nullptr);
  auto const typedCmd = std::dynamic_pointer_cast<occ::Conv2D>(cmd);
  assert(typedCmd != nullptr);
  assert(typedCmd->inputBuffers.size() == 1);
  typedCmd->kernel = clh::createKernel().givenProgram(prg).givenName("conv2d")();
  clh::setArg(typedCmd->kernel,0,typedCmd->output);
  clh::setArg(typedCmd->kernel,1,typedCmd->inputBuffers.at(0));
  clh::setArg(typedCmd->kernel,2,typedCmd->weights);
  clh::setArg(typedCmd->kernel,3,typedCmd->biases);
  clh::setArg(typedCmd->kernel,4,typedCmd->nofLayers);
  clh::setArg(typedCmd->kernel,5,typedCmd->nofFilters);
  clh::setArg(typedCmd->kernel,6,typedCmd->imageSize);
  clh::setArg(typedCmd->kernel,7,typedCmd->kernelSize);

  typedCmd->local = cl::NDRange(
      cfg.conv2dWorkGroupSize.at(0),
      cfg.conv2dWorkGroupSize.at(1));
  typedCmd->global = cl::NDRange(
      roundToClosestMultipleOf(typedCmd->imageSize.at(0),typedCmd->local[0]),
      roundToClosestMultipleOf(typedCmd->imageSize.at(1),typedCmd->local[1]));
}

void createBiasKernel(
    std::shared_ptr<occ::ComputeCommand>const&cmd,
    cl::Program                         const&prg,
    OpenCLInferenceEngine::Config       const&cfg){
  assert(cmd != nullptr);
  auto const typedCmd = std::dynamic_pointer_cast<occ::Bias>(cmd);
  assert(typedCmd != nullptr);
  assert(typedCmd->inputBuffers.size() == 1);
  typedCmd->kernel = clh::createKernel().givenProgram(prg).givenName("bias")();
  clh::setArg(typedCmd->kernel,0,typedCmd->output);
  clh::setArg(typedCmd->kernel,1,typedCmd->inputBuffers.at(0));
  clh::setArg(typedCmd->kernel,2,typedCmd->biases);
  clh::setArg(typedCmd->kernel,3,typedCmd->nofLayers);
  clh::setArg(typedCmd->kernel,4,typedCmd->imageSize);

  typedCmd->local = cl::NDRange(
      cfg.biasWorkGroupSize.at(0),
      cfg.biasWorkGroupSize.at(1));
  typedCmd->global = cl::NDRange(
      roundToClosestMultipleOf(typedCmd->imageSize.at(0),typedCmd->local[0]),
      roundToClosestMultipleOf(typedCmd->imageSize.at(1),typedCmd->local[1]));
}



void createBatchNormalizationKernel(
    std::shared_ptr<occ::ComputeCommand>const&cmd,
    cl::Program const&prg,
    OpenCLInferenceEngine::Config const&cfg){
  assert(cmd != nullptr);
  auto const typedCmd = std::dynamic_pointer_cast<occ::BatchNormalization>(cmd);
  assert(typedCmd != nullptr);
  assert(typedCmd->inputBuffers.size() == 1);
  typedCmd->kernel = clh::createKernel().givenProgram(prg).givenName("batchNormalization")();
  clh::setArg(typedCmd->kernel,0,typedCmd->output);
  clh::setArg(typedCmd->kernel,1,typedCmd->inputBuffers.at(0));
  clh::setArg(typedCmd->kernel,2,typedCmd->parameters);
  clh::setArg(typedCmd->kernel,3,typedCmd->nofLayers);
  clh::setArg(typedCmd->kernel,4,typedCmd->imageSize);
  clh::setArg(typedCmd->kernel,5,typedCmd->epsilon);

  typedCmd->local = cl::NDRange(
      cfg.batchNormalizationWorkGroupSize.at(0),
      cfg.batchNormalizationWorkGroupSize.at(1));
  typedCmd->global = cl::NDRange(
      roundToClosestMultipleOf(typedCmd->imageSize.at(0),typedCmd->local[0]),
      roundToClosestMultipleOf(typedCmd->imageSize.at(1),typedCmd->local[1]));
}

void createActivationKernel(
    std::shared_ptr<occ::ComputeCommand>const&cmd,
    cl::Program const&prg,
    OpenCLInferenceEngine::Config const&cfg){
  assert(cmd != nullptr);
  auto const typedCmd = std::dynamic_pointer_cast<occ::Activation>(cmd);
  assert(typedCmd != nullptr);
  assert(typedCmd->inputBuffers.size() == 1);

  typedCmd->kernel = clh::createKernel().givenProgram(prg).givenName("activation")();
  clh::setArg(typedCmd->kernel,0,typedCmd->output);
  clh::setArg(typedCmd->kernel,1,typedCmd->inputBuffers.at(0));
  clh::setArg(typedCmd->kernel,2,typedCmd->nofLayers);
  clh::setArg(typedCmd->kernel,3,typedCmd->imageSize);
  clh::setArg(typedCmd->kernel,4,static_cast<cl_uint>(typedCmd->activation));

  typedCmd->local = cl::NDRange(
      cfg.activationWorkGroupSize.at(0),
      cfg.activationWorkGroupSize.at(1));
  typedCmd->global = cl::NDRange(
      roundToClosestMultipleOf(typedCmd->imageSize.at(0),typedCmd->local[0]),
      roundToClosestMultipleOf(typedCmd->imageSize.at(1),typedCmd->local[1]));
}

void createMaxPoolingKernel(
    std::shared_ptr<occ::ComputeCommand>const&cmd,
    cl::Program const&prg,
    OpenCLInferenceEngine::Config const&cfg){
  assert(cmd != nullptr);
  auto const typedCmd = std::dynamic_pointer_cast<occ::MaxPooling>(cmd);
  assert(typedCmd != nullptr);
  assert(typedCmd->inputBuffers.size() == 1);

  typedCmd->kernel = clh::createKernel().givenProgram(prg).givenName("maxPooling")();
  clh::setArg(typedCmd->kernel,0,typedCmd->output);
  clh::setArg(typedCmd->kernel,1,typedCmd->inputBuffers.at(0));
  clh::setArg(typedCmd->kernel,2,typedCmd->nofLayers);
  clh::setArg(typedCmd->kernel,3,typedCmd->inputImageSize);
  clh::setArg(typedCmd->kernel,4,typedCmd->outputImageSize);
  clh::setArg(typedCmd->kernel,5,typedCmd->stride);

  typedCmd->local = cl::NDRange(
      cfg.maxPoolingWorkGroupSize.at(0),
      cfg.maxPoolingWorkGroupSize.at(1));
  typedCmd->global = cl::NDRange(
      roundToClosestMultipleOf(typedCmd->outputImageSize.at(0),typedCmd->local[0]),
      roundToClosestMultipleOf(typedCmd->outputImageSize.at(1),typedCmd->local[1]));
}

void createGlobalMaxPoolingKernel(
    std::shared_ptr<occ::ComputeCommand>const&cmd,
    cl::Program const&prg,
    OpenCLInferenceEngine::Config const&cfg){

  assert(cmd != nullptr);
  auto const typedCmd = std::dynamic_pointer_cast<occ::GlobalMaxPooling>(cmd);
  assert(typedCmd != nullptr);
  assert(typedCmd->inputBuffers.size() == 1);

  typedCmd->kernel = clh::createKernel().givenProgram(prg).givenName("globalMaxPooling")();
  clh::setArg(typedCmd->kernel,0,typedCmd->output);
  clh::setArg(typedCmd->kernel,1,typedCmd->inputBuffers.at(0));
  clh::setArg(typedCmd->kernel,2,typedCmd->nofLayers);
  clh::setArg(typedCmd->kernel,3,typedCmd->imageSize);

  typedCmd->local = cl::NDRange(
      cfg.globalMaxPoolingWorkGroupSize);
  typedCmd->global = cl::NDRange(
      roundToClosestMultipleOf(typedCmd->nofLayers,typedCmd->local[0]));
}

void createDenseKernel(
    std::shared_ptr<occ::ComputeCommand>const&cmd,
    cl::Program const&prg,
    OpenCLInferenceEngine::Config const&cfg){
  assert(cmd != nullptr);
  auto const typedCmd = std::dynamic_pointer_cast<occ::Dense>(cmd);
  assert(typedCmd != nullptr);
  assert(typedCmd->inputBuffers.size() == 1);

  typedCmd->kernel = clh::createKernel().givenProgram(prg).givenName("dense")();
  clh::setArg(typedCmd->kernel,0,typedCmd->output);
  clh::setArg(typedCmd->kernel,1,typedCmd->inputBuffers.at(0));
  clh::setArg(typedCmd->kernel,2,typedCmd->weights);
  clh::setArg(typedCmd->kernel,3,typedCmd->biases);
  clh::setArg(typedCmd->kernel,4,typedCmd->inputSize);
  clh::setArg(typedCmd->kernel,5,typedCmd->outputSize);
  clh::setArg(typedCmd->kernel,6,static_cast<cl_uint>(typedCmd->activation));

  typedCmd->local = cl::NDRange(
      cfg.denseWorkGroupSize);
  typedCmd->global = cl::NDRange(
      roundToClosestMultipleOf(typedCmd->outputSize,typedCmd->local[0]));
}

void createSoftmaxNormalizationKernel(
    std::shared_ptr<occ::ComputeCommand>const&cmd,
    cl::Program const&prg,
    OpenCLInferenceEngine::Config const&cfg){
  assert(cmd != nullptr);
  auto const typedCmd = std::dynamic_pointer_cast<occ::SoftmaxNormalization>(cmd);
  assert(typedCmd != nullptr);
  assert(typedCmd->inputBuffers.size() == 1);

  typedCmd->kernel = clh::createKernel().givenProgram(prg).givenName("softmaxNormalization")();
  clh::setArg(typedCmd->kernel,0,typedCmd->output);
  clh::setArg(typedCmd->kernel,1,typedCmd->inputBuffers.at(0));
  clh::setArg(typedCmd->kernel,2,typedCmd->size);

  typedCmd->local = cl::NDRange(
      cfg.softmaxNormalizationWorkGroupSize);
  typedCmd->global = cl::NDRange(
      roundToClosestMultipleOf(typedCmd->size,typedCmd->local[0]));
}


void createKernelForComputeCommand(
    std::shared_ptr<occ::ComputeCommand>const&cmd,
    cl::Program const&prg,
    OpenCLInferenceEngine::Config const&cfg){
  assert(cmd != nullptr);
  switch(cmd->type){
    case occ::ComputeCommand::CONV_2D              :
      createConv2DKernel(cmd,prg,cfg);
      break;
    case occ::ComputeCommand::BIAS                 :
      createBiasKernel(cmd,prg,cfg);
      break;
    case occ::ComputeCommand::BATCH_NORMALIZATION  :
      createBatchNormalizationKernel(cmd,prg,cfg);
      break;
    case occ::ComputeCommand::ACTIVATION           :
      createActivationKernel(cmd,prg,cfg);
      break;
    case occ::ComputeCommand::MAX_POOLING_2D       :
      createMaxPoolingKernel(cmd,prg,cfg);
      break;
    case occ::ComputeCommand::GLOBAL_MAX_POOLING_2D:
      createGlobalMaxPoolingKernel(cmd,prg,cfg);
      break;
    case occ::ComputeCommand::DENSE                :
      createDenseKernel(cmd,prg,cfg);
      break;
    case occ::ComputeCommand::SOFTMAX_NORMALIZATION:
      createSoftmaxNormalizationKernel(cmd,prg,cfg);
      break;
  }
}

void createKernel(
    std::shared_ptr<occ::Command>const&cmd,
    cl::Program const&prg,
    OpenCLInferenceEngine::Config const&cfg){
  assert(cmd != nullptr);
  if(cmd->type == occ::Command::INPUT)return;

  auto const computeCommand = std::dynamic_pointer_cast<occ::ComputeCommand>(cmd);
  assert(computeCommand != nullptr);
  createKernelForComputeCommand(computeCommand,prg,cfg);
}

std::shared_ptr<occ::CommandList>createCommandList(
    std::shared_ptr<ocl::Network>const&net,
    cl::Context const&ctx,
    cl::Program const&prg,
    OpenCLInferenceEngine::Config const&cfg){
  assert(net != nullptr);

  auto const result = std::make_shared<occ::CommandList>();
  std::vector<std::shared_ptr<occ::Command>>commands;

  for(auto const&x:net->outputs)
    createCommand(result,x,ctx);

  for(auto const&x:result->allCommands)
    createKernel(x.second,prg,cfg);

  return result;
}

class OpenCLInferenceEngineImpl{
  public:
    OpenCLInferenceEngineImpl(
        std::shared_ptr<cnnConf::Network>const&network,
        OpenCLInferenceEngine::Config    const&cfg    );
    cl::Platform     platform;
    cl::Device       device  ;
    cl::Context      context ;
    cl::CommandQueue queue   ;
    cl::Program      program ;



    std::shared_ptr<cnnConf::Network>network;
    std::shared_ptr<ocl::Network>    oclNet;

    std::shared_ptr<occ::CommandList>commands;

};


OpenCLInferenceEngineImpl::OpenCLInferenceEngineImpl(
    std::shared_ptr<cnnConf::Network>const&network,
    OpenCLInferenceEngine::Config    const&cfg    ):network(network){

  static_cast<void>(cfg);

  platform = clh::getPlatform()();
  device   = clh::getDevice().givenPlatform(platform).givenDeviceId(cfg.deviceId)();
  context  = clh::getContext().givenDevices({device})();
  program  = clh::buildProgram().givenContext(context).givenDevices({device}).givenSources({kernelsSources})();
  queue    = clh::getQueue().givenDevice(device).givenContext(context)();


  oclNet = ocl::translateNetwork(network);
  commands = createCommandList(oclNet,context,program,cfg);
}

OpenCLInferenceEngine::OpenCLInferenceEngine(
    std::shared_ptr<cnnConf::Network>const&network,OpenCLInferenceEngine::Config const&cfg):InferenceEngine(network){
  pimpl = std::make_unique<OpenCLInferenceEngineImpl>(network,cfg);
}

OpenCLInferenceEngine::~OpenCLInferenceEngine(){}

void OpenCLInferenceEngine::operator()(
    std::map<std::string,Tensor<float>>     &output,
    Tensor<float>                      const&input ){
  static_cast<void>(output);
  static_cast<void>(input);

  assert(pimpl != nullptr);

  //write data to OpenCL
  {
    assert(pimpl != nullptr);
    assert(pimpl->commands != nullptr);
    assert(pimpl->oclNet != nullptr);
    assert(pimpl->oclNet->inputs.size() == 1);
    assert(pimpl->oclNet->inputs.cbegin() != pimpl->oclNet->inputs.cend());
    auto const inputLayer = *pimpl->oclNet->inputs.cbegin();
    assert(inputLayer != nullptr);
    auto const inputLayerName = inputLayer->name;
    assert(pimpl->commands->allCommands.count(inputLayerName) > 0);
    auto const inputCommand = pimpl->commands->allCommands.at(inputLayerName);

    auto const buffer = inputCommand->output;
    auto const size = inputLayer->outputSize;
    size_t const inputSize = multiplyElements(size)*sizeof(float);

    assert(size == input.size);
    assert(inputSize/sizeof(float) == input.data.size());

    pimpl->queue.enqueueWriteBuffer(
        buffer,
        CL_TRUE,
        0,
        inputSize,
        const_cast<float*>(input.data.data()),
        nullptr,
        nullptr);
  }

  assert(pimpl != nullptr);
  assert(pimpl->commands != nullptr);
  for(auto const&cmd : pimpl->commands->commands)
    cmd->enqueue(pimpl->queue);
  pimpl->queue.finish();

  //read data from OpenCL
  {
    output.clear();

    assert(pimpl != nullptr);
    assert(pimpl->oclNet != nullptr);
    for(auto const&x:pimpl->oclNet->outputs){
      assert(x != nullptr);
      auto const outputName = x->sourceName;
      assert(pimpl->commands != nullptr);
      assert(pimpl->commands->allCommands.count(x->name) > 0);
      auto const outputCommand = pimpl->commands->allCommands.at(x->name);
      auto const&size = x->outputSize;
      output[outputName] = Tensor<float>(size);
      auto const outputSize = multiplyElements(size)*sizeof(float);

      //TODO possible acceleration
      //engueue all reads and perform them at once
      //memory transferes could be merge that way...
      pimpl->queue.enqueueReadBuffer(
          outputCommand->output,
          CL_TRUE,
          0,
          outputSize,
          const_cast<float*>(output[outputName].data.data()),
          nullptr,
          nullptr);
    }
  }
}

Tensor<float>OpenCLInferenceEngine::getLayerOutput(std::string const&n)const{
  assert(this != nullptr);
  assert(pimpl != nullptr);

  auto const cmd = pimpl->commands->allCommands.at(n);
  auto const size = pimpl->oclNet->allLayers.at(n)->outputSize;
  auto const ssize = multiplyElements(size)*sizeof(float);
  Tensor<float>result(size);
  pimpl->queue.enqueueReadBuffer(
      cmd->output,
      CL_TRUE,
      0,
      ssize,
      const_cast<float*>(result.data.data()),
      nullptr,
      nullptr);
  return result;
}
