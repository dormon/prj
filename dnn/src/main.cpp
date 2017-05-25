#include<iostream>
#include<cstdint>
#include<cstdlib>
#include<cassert>
#include<vector>
#include<map>

// Calculus:
// x - vector
// w - scalar
// x = {x[0],x[1],...,x[K-1]}^T
// G = diff(x,w)
// G = {diff(x[0],w),diff(x[1],w),...,diff(x[K-1],w)}
//
// w = {w[0],w[1],....,w[K-1]}
// x = vector of constants
//
// G = diff(w*x,w[0])
// G = diff(x[0]*w[0]+x[1]*w[1]+...+x[K-1]*w[K-1],w[0])
// G = x[0]
//
// G = diff(w*x,w) = x^T
//
// G = diff((f[k](x)-y[k])^2,x)
// G = 2 * (f[k](x)-y[k]) * diff(f[k](x)-y[k],x)
// G = 2 * (f[k](x)-y[k]) * diff(f[k](x),x)
//
// (f[0](x)-y[0])^2 + (f[1](x)-y[1])^2 + ...
// 2*(f[0](x)-y[0])*diff(f[0](x),x) + 2*(f[1](x)-y[1])*diff(f[1](x),x)
//
//
//      (|a b|          )^2  |a b|
// diff((|c d| * |x y|^T)  , |c d|)
//      (|e f|          )    |e f|
//
//     (|a b|          )        |a b|            |a b|
// 2 * (|c d| * |x y|^T) * diff(|c d| * |x y|^T, |c d|)
//     (|e f|          )        |e f|            |e f|
//
//     |ax + by|        |a b|            |a b|
// 2 * |cx + dy| * diff(|c d| * |x y|^T, |c d|)
//     |ex + fy|        |e f|            |e f|
//
//     |ax + by|        |ax + by|  |a b|
// 2 * |cx + dy| * diff(|cx + dy|, |c d|)
//     |ex + fy|        |ex + fy|  |e f|
//
//     |ax + by|   |x|
// 2 * |cx + dy| * |0|
//     |ex + fy|   |0|
//
//
// A*X = trace(A.X)
//
// diff((A*X)^2,X)
// 2*(A.X).diff(A.X,X)
// 2*(
//
//
// diff((Ax*Xx+Ay*Xy+Az*Xz)^2,(Xx,Xy,Xz))
// (2*(Ax*Xx+Ay*Xy+Az*Xz)*Ax,2*(Ax*Xx+Ay*Xy+Az*Xz)*Ay,2*(Ax*Xx+Ay*Xy+Az*Xz)*Az)
// 2*(Ax*Xx+Ay*Xy+Az*Xz)*(Ax,Ay,Az)
//  
//
//
// diff((W*A)^2,W)
// 2*(W*A)*diff(W*A,W)
// 2*(W*A)*A^T
//
//
// (ax+by)^2 + (cx+dy)^2 + (ex+fy)^2
//
// |2*(ax+by)x 2*(ax+by)y|
// |2*(cx+dy)x 2*(cx+dy)y|
// |2*(ex+fy)x 2*(ex+fy)y|
//
//     |(ax+by)x (ax+by)y|
// 2 * |(cx+dy)x (cx+dy)y|
//     |(ex+fy)x (ex+fy)y|
//
//
//     |ax + by|   
// 2 * |cx + dy| . |x y|
//     |ex + fy|   
//
//
//
//

// dimension of imput:
// n
//
// dimension of output:
// m
//
// number of layers:
// L
//
// number of datasets:
// K
// 
// k-th input vector:
// X[k] = {x[0],x[1],...,x[n-1]}^T
// k-th output vector:
// Y[k] = {y[1],y[2],...,y[m-1]}^T
//
// all input vectors:
// X = {X[0],X[1],...,X[K-1]}
//
// all output vectors:
// Y = {Y[0],Y[1],...,Y[K-1]}
//
// weight matrix to layer l:
// W[l]
//
// bias vector of layer l:
// B[l]
//
// weight tensor:
// W
//
// bias matrix:
// B
//
// sigmoid function:
// s(x) = 1/(1+exp(-x))
//
// derivate of sigmoid:
// d(x)=(-(-exp(-x)))/((1+exp(-x))^2)
// d(x)=(+exp(-x))/((1+exp(-x))^2)
// d(x)=exp(-x)/((1+exp(-x))^2)
// d(x)=exp(-x)/(1+exp(-2x)+2exp(-x))
// d(x)=((1+exp(-2x)+2exp(-x)) / exp(-x))^-1
// d(x)=(exp(x) + exp(-x) + 2)^-1
// d(x)=1/(exp(x)+exp(-1)+2)
//
// output of layer l:
// a[l]
// a[0] = x
// z[l] = W[l] * a[l-1] - B[l]
// a[l] = s(z[l])
//
// output of layer l for dataset k:
// a[l][k]
// a[0] = x[k]
// z[l][k] = W[l] * a[l-1][k] - B[l]
// a[l][k] = s(z[l][k])
//
//
// cost function:
// C = 1/K * ( (a[L-1][0] - y[0])^2 + (a[L-1][1] - y[1])^2 + ... + (a[L-1][K-1] - y[K-1])^2 )
//
// cost function for k-th dataset:
// C[k] = 1/K * (a[L-1][k] - y[k])^2
// C = C[0]+C[1]+...+C[K-1]
//
// gradient of cost function with respect to weight tensor:
// G = diff(C,W)
// G = diff(C[0]+C[1]+...+C[K-1],W)
// G = diff(C[0],W) + diff(C[1],W) + ... + diff(C[K-1],W)
//
// gradient of cost function for k-th dataset with respect to weight tensor:
//
// G[k] = diff(C[k],W)
// G[k] = diff(1/K * (a[L-1][k] - y[k])^2,W)
// G[k] = 1/K * diff((a[L-1][k] - y[k])^2,W)
// G[k] = 1/K * 2 * (a[L-1][k] - y[k]) * diff(a[L-1][k] - y[k],W)
// G[k] = 1/K * 2 * (a[L-1][k] - y[k]) * diff(a[L-1][k],W)
// G[k] = 1/K * 2 * (a[L-1][k] - y[k]) * diff(s(z[L-1][k]),W)
// G[k] = 1/K * 2 * (a[L-1][k] - y[k]) * d(z[L-1][k]) * diff(z[L-1][k],W)
// G[k] = 1/K * 2 * (a[L-1][k] - y[k]) * d(z[L-1][k]) * diff(W[L-1] * a[L-2][k] - B[L-1],W)
// G[k] = 1/K * 2 * (a[L-1][k] - y[k]) * d(z[L-1][k]) * diff(W[L-1] * a[L-2][k],W)
// G[k] = 1/K * 2 * (a[L-1][k] - y[k]) * d(z[L-1][k]) * diff(W[L-1] * a[L-2][k],W)
//
// gradient of cost function for k-th dataset with respect to wight matrix of L-1 layer:
// G[k] = 1/K * 2 * (a[L-1][k] - y[k]) * d(z[L-1][k]) * diff(W[L-1] * a[L-2][k],W[L-1])
//
// gradient of cost function for k-th dataset with respect to wight matrix of l layer:
// G[k] = 1/K * 2 * (a[L-1][k] - y[k]) * d(z[L-1][k]) * diff(W[L-1] * a[L-2][k],W[l])
// G[k] = 1/K * 2 * (a[L-1][k] - y[k]) * d(z[L-1][k]) * W[L-1] * diff(a[L-2][k],W[l])
// G[k] = 1/K * 2 * (a[L-1][k] - y[k]) * d(z[L-1][k]) * W[L-1] * d(z[L-2][k]) * diff(W[L-2] * a[L-3][k],W[l])
// G[k] = 1/K * 2 * (a[L-1][k] - y[k]) * d(z[L-1][k]) * W[L-1] * d(z[L-2][k]) * W[L-2] * diff(a[L-3][k],W[l])
//
//
//
//
//
//
//
//
//
//
//
//
//


class Layer{
  public:
    virtual ~Layer(){}
};

class Connection{
  public:
    Connection(
        size_t const&inputSize ,
        size_t const&outputSize){
      assert(this != nullptr);
      this->_weights.resize(outputSize);
      for(auto&row:this->_weights)
        row.resize(inputSize,0.f);
    }
  protected:
    std::vector<std::vector<float>>_weights;
};

class InputLayer: public Layer{
  public:
    InputLayer(size_t const&n):_nofInputs(n){}
    virtual ~InputLayer(){}
  protected:
    size_t _nofInputs;
};

class HiddenLayer: public Layer{
  public:
    HiddenLayer(size_t const&n){
      this->biases.resize(n,0.f);
    }
    virtual ~HiddenLayer(){}
  protected:
    std::vector<float>biases;
};

class OutputLayer: public Layer{
  public:
    OutputLayer(size_t const&n):_nofOutputs(n){}
    virtual ~OutputLayer(){}
  protected:
    size_t _nofOutputs;
};

class DNN{
  public:
    DNN(
        size_t             const&nofInputs   ,
        size_t             const&nofOutputs  ,
        std::vector<size_t>const&hiddenLayers){
      assert(this != nullptr);
      assert(hiddenLayers.size() > 0);
      this->_layers.emplace_back(new InputLayer(nofInputs));
      for(auto const&layerSize:hiddenLayers)
        this->_layers.emplace_back(new HiddenLayer(layerSize));
      this->_layers.emplace_back(new OutputLayer(nofOutputs));
      this->_connections.emplace_back(nofInputs,hiddenLayers.at(0));
      for(size_t l=0;l+1<hiddenLayers.size();++l)
        this->_connections.emplace_back(hiddenLayers[l],hiddenLayers[l+1]);
      this->_connections.emplace_back(hiddenLayers.back(),nofOutputs);
    }
    ~DNN(){
      assert(this != nullptr);
      for(auto const&l:this->_layers)
        delete l;
    }
  protected:
    std::vector<Layer*>_layers;
    std::vector<Connection>_connections;
};

int main(){
  return 0;
}
