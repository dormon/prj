#include<iostream>
#include<cstdint>
#include<cstdlib>
#include<cassert>
#include<vector>
#include<limits>
#include<functional>
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

// f<p>(x)
//
// (f<p>(x(0)) - y(0))^2
//
// p[n+1] = p[n] - s*diff((f<p>(x[e])-y[e])^2,p)(p[n])
// p[n+1] = p[n] - s*2*((f<p>(x[e])-y[e])*diff(f<p>(x[e]),p))(p[n])
// p[n+1] = p[n] - s*2*(f<p[n]>(x[e])-y[e]) * (diff(f<p>(x[e]),p))(p[n])
//
// f[layer][neuronInLayer]<parameters>(inputs) = g(z[layer][neuronInlayer]<parameters>(inputs))
//
// f[l][k]<p>(x) = g(z[l][k]<p>(x))
//
// diff(f[l][k]<p>(x),p) = diff(g[l][k]<p>(x),p) = h(z[l][k]<p>(x)) * diff(z[l][k]<p>(x),p)
//
// z[l+1][k]<p>(x) = sum(f[l][i]<q[i]>(x)*a[l][k][i];i=0;i<L[l])-w[l+1][k] , p = union(q[i];i=0;i<L[l]) + {w[l+1][k]} + {a[l][k][i];i=0;i<L[l]}
//
// diff(z[l+1][k]<p>(x),p) = { 
//   diff(z[l+1][k]<p>(x),w[k]                  )
//   diff(z[l+1][k]<p>(x),{a[i];i=0;i<L[l]}     )
//   diff(z[l+1][k]<p>(x),union(q[i];i=0;i<L[l]))
// }
//
// diff(z[l+1][k]<p>(x),w[k]) = -1
// 
// diff(z[l+1][k]<p>(x),{a[i];i=0;i<L[l]}) = 
// diff(sum(f[l][i]<q[i]>(x)*a[i];i=0;i<L[l])-w[k],{a[i];i=0;i<L[l]}) = 
// diff(sum(f[l][i]<q[i]>(x)*a[i];i=0;i<L[l]),{a[i];i=0;i<L[l]}) = 
// {f[l][i]<q[i]>(x);i=0;i<L[l]}
//
// diff(z[l+1][k]<p>(x),union(q[i];i=0;i<L[l])) = 
// diff(sum(f[l][i]<q[i]>(x)*a[i];i=0;i<L[l])-w[k],{a[i];i=0;i<L[l]},union(q[i];i=0;i<L[l])) = 
// diff(sum(f[l][i]<q[i]>(x)*a[i];i=0;i<L[l]),{a[i];i=0;i<L[l]},union(q[i];i=0;i<L[l])) = 
// {diff(f[l][i]<q[i]>(x),q[i])*a[i];i=0;i<L[l]}
//
//
// f[l+1][k]<p>(x) = g(sum(f[l][i]<q[i]>(x)*a[i],i)-w[k]), p = union(q[i]) + {w[k]} + {a[i]}
// f[l+1][k]<p>(x) = g(z<p>(x))
//
// diff(f[l+1][k]<p>(x),p) = {
//   g(sum(f[l][i]<q[i]>(x)*a[i],i)-w[k])
//   union(g(sum(f[l][i]<q[i]>(x)*a[i],i)-w[k])*f[l][j]<q[i]>(x),j)
// }
//
// f[0][k]<p>(x) = x[k]
//
// for x i get y but expect r:
// y = f(x), r
// error is:
// e = y-r
// what is error on x?
// x-g(e), f(g(x))=x
// x-g(y-r)
//
//
// min(g,x,N,s){
//   let x[0] = 0
//   let x[n+1] = x[n] - s*diff(g,x)(x[n])
//   return x[N]
// }
//
// C(y2,y)
// 
// y[0][k] = x[k]
// y[l+1][k](p[l+1][k]) = f<p[l][k]>[l][k](y[l]()
// y[L-1][k]
//
//
// C(W,B) = (f(W⋅x-B)-y) ⋅ (f(W⋅x-B)-y)^T
//
// ∂C(W,B)
// ─────── = {(f(W⋅x-B)-y) * g(W⋅x-B)} × x^T
//   ∂W
//
// ∂f(W⋅x)
// ─────── = g(W.x) × x^T
//   ∂W    
//
// ∂f(w*x)
// ─────── = g(w*x) * x
//   ∂w   
//
// ∂f(w×x^T)
// ───────── = g(w×x^T)
//    ∂w   
//
//  (|w0|        )   |f(w0*x0) f(w0*x1)|   |(g(w0*x0)*x0,0          ,0          ) (g(w0*x1)*x1,0          ,0          )|
//  (|w1|        )   |f(w1*x0) f(w1*x1)|   |(0          ,g(w1*x0)*x0,0          ) (0          ,g(w1*x1)*x1,0          )|
// f(|w2|×|x0 x1|) = |f(w2*x0) f(w2*x1)| = |(0          ,0          ,g(w2*x0)*x0) (0          ,0          ,g(w2*x0)*x0)|
//
//
// ∂C(W,B)   ∂(f(W⋅x-B)-y) ⋅ (f(W⋅x-B)-y)^T   ∂(f(W⋅x-B)-y)                                 ∂(f(W⋅x-B)-y)^T
// ─────── = ────────────────────────────── = ─────────────⋅(f(W⋅x-B)-y)^T  +  (f(W⋅x-B)-y)⋅───────────────
//   ∂W                   ∂W                       ∂W                                             ∂W      
//
//          ∂W⋅x-B
// g(W⋅x-B)⋅──────
//            ∂W  
//
//
// ∂(f(x0 x1) g(x0 x1) h(x0 x1))   (∂(f(x0 x1) g(x0 x1) h(x0 x1)) ∂(f(x0 x1) g(x0 x1) h(x0 x1)))
// ───────────────────────────── = (───────────────────────────── ─────────────────────────────) =
//          ∂(x0 x1)               (              ∂x0                           ∂x1            )
//
// ((F<x0>(x0 x1) G<x0>(x0 x1) H<x0>(x0 x1)) (F<x1>(x0 x1) G<x1>(x0 x1) H<x1>(x0 x1)))
//
// ∂(f(x0 x1) g(x0 x1) h(x0 x1))   |(F<x0>(x0 x1) G<x0>(x0 x1) H<x0>(x0 x1))|   |F<x0>(x0 x1) G<x0>(x0 x1) H<x0>(x0 x1)|
// ───────────────────────────── = |(F<x1>(x0 x1) G<x1>(x0 x1) H<x1>(x0 x1))| = |F<x1>(x0 x1) G<x1>(x0 x1) H<x1>(x0 x1)|
//          ∂(x0 x1)^T           
//
//
//
//
//
//
// ∂(x0 x1 x2) ⋅ (y0 y1 y2)^T
// ────────────────────────── = (y0 y1 y2)
//        ∂(x0 x1 x2)
//
//
// ∂(x0 x1 x2) ⋅ (y0 y1 y2)^T
// ────────────────────────── = (y0 y1 y2)^T
//       ∂(x0 x1 x2)^T
//
//
//
// C = y^T.f(B.f(A.x-u)-v)
//
// ∂C 
// ── = [y*g(z_{L-2})] * f^T(z_{L-3}^T)
// ∂B
//
// ∂C 
// ── = [(B^T.[y*g(z_{L-2})])*g(z_{L-3})] * x^T
// ∂A
//
//
//
// H[L-2] = y*g(z[L-2])
// H[l-1] = W[l]^T.H[l]
//
//    ∂C
// ─────── = H[L-2] * y^T[L-2]
// ∂W[L-2]
//
//    ∂C
// ─────── = (H[l]*g(z[l-1])) * y^T[l-1]
// ∂W[l-1]
//
//
//
// ∂C 
// ── = y.(-1) * g(z_{L-2})
// ∂u
//
// ∂C                                            (    ∂C)
// ── = [B^T.(y.(-1) * g(z_{L-2}))]*g(z_{L-3}) = (B^T.──)*g(z_{L-3})
// ∂v                                            (    ∂u)
//
//////////////////////////////////////////
//    ∂C                                //
// ─────── = y.(-1) * g(z[L-2])         //
// ∂B[L-2]                              //
//                                      //
//                                      //
//    ∂C     (           ∂C )           //
// ─────── = (W[l-1]^T.─────)*g(z[l-1]) //
// ∂B[l-1]   (         ∂B[l])           //
//////////////////////////////////////////
//
//########################################
//
// H[L-2] = y            *g(z[L-2])
// H[l-1] = (W[l]^T.H[l])*g(z[l-1])
//
//    ∂C                                ##
// ─────── = H[L-2].(-1)                ##
// ∂B[L-2]                              ##
//                                      ##
//    ∂C                                ##
// ─────── = H[l-1].(-1)                ##
// ∂B[l-1]                              ##
//                                      ##
//    ∂C                                ##
// ─────── = H[L-2] * y^T[L-2]          ##
// ∂W[L-2]                              ##
//
//    ∂C
// ─────── = H[l-1] * y^T[l-1]
// ∂W[l-1]
//
//    ∂C                                ##
// ─────── = y*g(z[L-2]).(-1)           ##
// ∂B[L-2]                              ##
//                                      ##
//   ∂C    (         ∂C   )             ##
// ───── = (W[l]^T.───────)*g(z[l])     ##
// ∂B[l]   (       ∂B[l+1])             ##
//                                      ##
//    ∂C          ∂C                    ##
// ─────── = - ─────── * y^T[L-2]       ##
// ∂W[L-2]     ∂B[L-2]                  ##
//                                      ##
//    ∂C          ∂C                       ##
// ─────── = - ─────── * y^T[l-1]          ##
// ∂W[l-1]     ∂B[L-2]                     ##
//
//
//
//########################################
//
//
//

template<typename TYPE>
class Function{
  public:
    using FceImpl = std::function<TYPE(std::vector<TYPE>const&,std::vector<TYPE>const&)>;
    Function(FceImpl const&fce,size_t nofParams):fce(fce){
      assert(this != nullptr);
      parameters.resize(nofParams,static_cast<TYPE>(0));
    }
    TYPE operator()(std::vector<TYPE>const&i)const{
      assert(this != nullptr);
      assert(fce != nullptr);
      return fce(parameters,i);
    }
    std::vector<TYPE>parameters;
    FceImpl fce;
};


template<typename TYPE>
class Matrix{
  public:
    Matrix(size_t rows,size_t cols);
    
  protected:
    std::vector<TYPE>data;
};

template<typename TYPE>
class Layer{
  public:
    Layer(size_t nofNeurons):nofNeurons(nofNeurons){}
    ~Layer(){
      assert(this != nullptr);
      if(isInputLayer())return;
      delete output;
    }
    void initAsInputLayer(){}
    void initAsHiddenLayer(){
      assert(this != nullptr);
      output = new std::vector<TYPE>;
      output->resize(getNofNeurons(),static_cast<TYPE>(0));
    }
    void initAsOutputLayer(){
      assert(this != nullptr);
      initAsHiddenLayer();
    }
    void setNeighbourLayers(Layer<TYPE>*const&previous,Layer<TYPE>*const&next){
      assert(this != nullptr);
      previousLayer = previous;
      nextLayer     = next    ;
    }
    void setFunctions(typename Function<TYPE>::FceImpl const&fce){
      assert(this != nullptr);
      if(isInputLayer())return;
      for(size_t k = 0; k < getNofNeurons(); ++k)
        functions.emplace_back(fce,getNofInputNeurons());
    }
    bool isInputLayer()const{
      assert(this != nullptr);
      return !previousLayer;
    }
    bool isOutputLayer()const{
      assert(this != nullptr);
      return !nextLayer;
    }
    size_t getNofNeurons()const{
      assert(this != nullptr);
      return nofNeurons;
    }
    size_t getNofInputNeurons()const{
      assert(this != nullptr);
      if(isInputLayer())return 0;
      return previousLayer->getNofNeurons();
    }
    void computeOutput(){
      assert(this != nullptr);
      if(isInputLayer())return;
      previousLayer->computeOutput();
      for(size_t k = 0; k < getNofNeurons(); ++k)
        output->at(k) = functions.at(k)(previousLayer->getOutput());
    }
    void setOutput(std::vector<TYPE>const*const&o){
      assert(this != nullptr);
      assert(!isInputLayer());
      output = o;
    }
    std::vector<TYPE>const& getOutput()const{
      assert(this != nullptr);
      assert(output != nullptr);
      return *output;
    }
  protected:
    size_t            nofNeurons    = 0      ;
    Layer<TYPE>      *previousLayer = nullptr;
    Layer<TYPE>      *nextLayer     = nullptr;
    std::vector<TYPE>*output        = nullptr;
    std::vector<Function<TYPE>>functions;
};

/*
template<typename TYPE>
class Layer{
  public:
    Layer(size_t nofNeurons){
      assert(this != nullptr);
      y.resize(nofNeurons,0);
      z.resize(nofNeurons,0);
      e.resize(nofNeurons,0);
    }
    void setOutput(std::vector<TYPE>const&x){
      assert(this != nullptr);
      assert(getNofNeurons() == x.size());
      y = x;
    }
    virtual ~Layer(){}
    size_t getNofNeurons()const{
      assert(this != nullptr);
      return y.size();
    }
    bool isInputLayer()const{
      assert(this != nullptr);
      return !previousLayer;
    }
    bool isOutputLayer()const{
      assert(this != nullptr);
      return !nextLayer;
    }
    size_t getNofInputNeurons()const{
      assert(this != nullptr);
      if(isInputLayer())return 0;
      return previousLayer->getNofNeurons();
    }
    TYPE& getY(size_t i){
      assert(this != nullptr);
      assert(i < y.size());
      return y.at(i);
    }
    TYPE& getZ(size_t i){
      assert(this != nullptr);
      assert(i < z.size());
      return z.at(i);
    }
    TYPE& getE(size_t i){
      assert(this != nullptr);
      assert(i < e.size());
      return e.at(i);
    }
    TYPE getW(size_t i,size_t k)const{
      assert(this != nullptr);
      assert(k < weights.size());
      assert(i < weights.at(k).size());
      return weights.at(k).at(i);
    }
    TYPE getB(size_t k)const{
      assert(this != nullptr);
      assert(k < biases.size());
      return biases.at(k);
    }
    void computeZ(size_t k){
      assert(this != nullptr);
      assert(k < getNofNeurons());
      getZ(k) = 0;
      for(size_t i = 0; i < getNofInputNeurons(); ++i)
        getZ(k) += previousLayer->getY(i) * getW(i,k);
      getZ(k) -= getB(k);
    }
    void computeZ(){
      assert(this != nullptr);
      if(isInputLayer())return;
      previousLayer->computeY();
      for(size_t k = 0; k < getNofNeurons(); ++k)
        computeZ(k);
    }
    void computeY(){
      assert(this != nullptr);
      if(isInputLayer())return;
      computeZ();
      for(size_t k = 0; k < getNofNeurons(); ++k)
        getY(k) = g(getZ(k));
    }
    void backPropagate(std::vector<TYPE>const&v){
      assert(this != nullptr);
      if(isOutputLayer()){
        for(size_t k = 0; k < getNofNeurons(); ++k)
          getE(k) = getY(k) - v.at(k);
      }else{

      }
    }
  protected:
    Layer<TYPE>*previousLayer = nullptr;
    Layer<TYPE>*nextLayer     = nullptr;
    std::vector<TYPE>biases;
    std::vector<std::vector<TYPE>>weights;
    std::vector<TYPE>y;
    std::vector<TYPE>z;
    std::vector<TYPE>e;
    std::function<TYPE(TYPE)>g = [](TYPE v){if(v>0)return v;};
    std::function<TYPE(TYPE)>h = [](TYPE v){if(v>0)return 1;return 0;};
};
*/
/*
template<typename TYPE>
class DNN{
  public:
    DNN(
        size_t             const&nofInputs   ,
        size_t             const&nofOutputs  ,
        std::vector<size_t>const&hiddenLayers){
      assert(this != nullptr);
      assert(hiddenLayers.size() > 0);

      this->_layers.emplace_back(new InputLayer<TYPE>(nofInputs));

      for(auto const&layerSize:hiddenLayers)
        this->_layers.emplace_back(new HiddenLayer<TYPE>(layerSize));

      this->_layers.emplace_back(new OutputLayer<TYPE>(nofOutputs));

      this->_connections.emplace_back(nofInputs,hiddenLayers.front());

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
    std::vector<Layer<TYPE>*>_layers;
    std::vector<Connection>_connections;
};
*/

int main(){
  return 0;
}
