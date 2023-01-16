#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>
#include<math.h>

#define ___ fprintf(stderr,"%s %i\n",__FILE__,__LINE__)

struct Matrix;
void copy(Matrix&res,Matrix const&a);

struct Matrix{
  Matrix(int r,int c):nofRows(r),nofColumns(c){
    data = new float[c*r];
    memset(data,0,sizeof(float)*c*r);
  }
  Matrix(){}
  ~Matrix(){
    delete[]data;
  }
  void check(int r,int c)const{
    if(r>=nofRows || c>=nofColumns){
      fprintf(stderr,"Matrix[%i,%i](%i,%i)\n",nofRows,nofColumns,r,c);
      assert(false);
    }
  }
  Matrix(Matrix const&a):Matrix(a.nofRows,a.nofColumns){
    copy(*this,a);
  }
  Matrix&operator=(Matrix const&a){
    delete data;
    nofRows = a.nofRows;
    nofColumns = a.nofColumns;
    data = new float[nofRows*nofColumns];
    copy(*this,a);
    return*this;
  }
  float operator[](int i)const{assert(i<nofRows*nofColumns);return data[i];}
  float&operator[](int i){assert(i<nofRows*nofColumns);return data[i];}
  float operator()(int r,int c)const{check(r,c);return data[r*nofColumns+c];}
  float&operator()(int r,int c){check(r,c);return data[r*nofColumns+c];}
  int   nofRows    = 0;
  int   nofColumns = 0;
  float*data       = nullptr;
};

void print(Matrix const&m){
  for(int r=0;r<m.nofRows;++r){
    for(int c=0;c<m.nofColumns;++c)
      fprintf(stderr,"%1.5f ",m(r,c));
    fprintf(stderr,"\n");
  }
}

void mult(Matrix&res,Matrix const&a,Matrix const&b){
  assert(a.nofColumns == b.nofRows);
  assert(res.nofRows == a.nofRows);
  assert(res.nofColumns == b.nofColumns);
  for(int r=0;r<res.nofRows;++r)
    for(int c=0;c<res.nofColumns;++c){
      res(r,c) = 0;
      for(int i=0;i<a.nofColumns;++i){
        res(r,c) += a(r,i) * b(i,c);
      }
    }
}

void copy(Matrix&res,Matrix const&a){
  assert(res.nofRows == a.nofRows);
  assert(res.nofColumns == a.nofColumns);
  for(int r=0;r<res.nofRows;++r)
    for(int c=0;c<res.nofColumns;++c)
      res(r,c) = a(r,c);
}

float uniform(){
  return (float)rand()/(float)RAND_MAX;
}

Matrix operator+(Matrix const&a,Matrix const&b){
  assert(a.nofColumns == b.nofColumns && a.nofRows == b.nofRows);
  auto res = Matrix(b.nofRows,a.nofRows);
  for(int r=0;r<res.nofRows;++r)
    for(int c=0;c<res.nofColumns;++c)
      res(r,c) = a(r,c) + b(r,c);
  return res;
}

Matrix zeros(Matrix const&m){
  return Matrix(m.nofRows,m.nofColumns);
}

Matrix random(int r,int c){
  auto res = Matrix(r,c);
  for(int r=0;r<res.nofRows;++r)
    for(int c=0;c<res.nofColumns;++c)
      res(r,c) = uniform();
  return res;
}

struct NNMF{
  Matrix W;
  Matrix H;
};

#define printSizes(m)\
  fprintf(stderr,"%s[%i,%i]\n",#m,m.nofRows,m.nofColumns)

NNMF nnmf(Matrix const&v,int K,int nofIterations){
  NNMF res;
  auto&W = res.W;
  auto&H = res.H;

  // init
  W = random(v.nofRows,K);
  H = random(K,v.nofColumns);

  auto newW = zeros(W);
  auto wh   = zeros(v);

  for(int i=0;i<nofIterations;++i){
    //W update
    mult(wh,W,H);
    for(int r=0;r<newW.nofRows;++r)
      for(int c=0;c<newW.nofColumns;++c){
        float a=0;
        for(int u=0;u<H.nofColumns;++u)
          a += v(r,u)/wh(r,u)*H(c,u);
        newW(r,c) = W(r,c)*a;
      }
    //normalize W
    for(int c=0;c<newW.nofColumns;++c){
      float s = 0;
      for(int r=0;r<newW.nofRows;++r)
        s+=newW(r,c);
      for(int r=0;r<newW.nofRows;++r)
        newW(r,c) /= s;
    }
    copy(W,newW);

    //H update
    //mult(wh,W,H);
    for(int r=0;r<H.nofRows;++r)
      for(int c=0;c<H.nofColumns;++c){
        float a=0;
        for(int i=0;i<newW.nofRows;++i)
          a+=newW(i,r)*v(i,c)/wh(i,c);
        H(r,c)*=a;
      }
    //copy(W,newW);
  }

  return res;
}

float distance(Matrix const&a,Matrix const&b){
  assert(a.nofRows == b.nofRows);
  assert(a.nofColumns == b.nofColumns);
  float err = 0;
  for(int r=0;r<a.nofRows;++r)
    for(int c=0;c<a.nofColumns;++c){
      float x = (a(r,c)-b(r,c));
      err = x*x;
    }
  return sqrt(err);
}

int main(){
  auto r = random(8,8);
  print(r);
  fprintf(stderr,"\n");

  auto nn = nnmf(r,3,1000);
  print(nn.W);
  fprintf(stderr,"\n");
  print(nn.H);
  fprintf(stderr,"\n");
  auto a = zeros(r);
  mult(a,nn.W,nn.H);

  print(a);
  fprintf(stderr,"error: %f\n",distance(r,a));
  return 0;
}


