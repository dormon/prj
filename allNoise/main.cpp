#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<sstream>

struct BitStream{
  std::vector<size_t>data       = std::vector<size_t>(1,0);
  size_t             bits       = 1                       ;
  size_t const       baseBits   = sizeof(size_t)*8        ;
  size_t             nofNumbers = 0                       ;

  BitStream(){}
  BitStream(size_t d):bits(d){
    nofNumbers       = 1<<bits;
    data.resize(nofNumbers);
    //auto nofBits     = nofNumbers * bits;
    //auto nofElements = nofBits/baseBits + (size_t)(nofBits%baseBits!=0);
    //data.resize(nofElements,0);
  }
  size_t mask(size_t bits)const{
    return (1<<bits)-1;
  }
  size_t get(size_t i)const{
    return data.at(i);
    size_t r = 0;
    auto start  = i*bits;
    auto elem   = start/baseBits;
    auto offset = start%baseBits;
    r |= (data[elem] >> offset) & mask(bits);
    if(baseBits-offset<bits)
      r |= (data[elem+1] << (baseBits-offset));
    r &= mask(bits);
    return r;
  }
  void set(size_t i,size_t v){
    data.at(i) = v;
    return;
    auto start  = i*bits;
    auto elem   = start/baseBits;
    auto offset = start%baseBits;
    data[elem] &= ~(mask(bits)<<offset);
    data[elem] |= v << offset;
    if(baseBits-offset<bits){
      data[elem+1] &= ~mask(bits-(baseBits-offset));
      data[elem+1] |= v >> offset;
    }
  }
  std::string toBinaryStr()const{
    std::stringstream ss;
    for(size_t i=0;i<data.size();++i){
      auto d = data.at(data.size()-i-1);
      for(size_t j=0;j<baseBits;++j)
        ss << (((d>>(baseBits-j-1))&1)?"1":"0");
    }
    return ss.str();
  }
  std::string toNumbersStr()const{
    std::stringstream ss;
    bool first = true;
    for(size_t i=0;i<nofNumbers;++i){
      if(first)first = false;
      else ss << " ";
      ss << get(i);
    }
    return ss.str();
  }
  bool operator==(BitStream const&o){
    if(bits != o.bits)return false;
    return data == o.data;
  }

  bool hasRedundancy()const{
    std::set<size_t>uniques;
    for(size_t i=0;i<nofNumbers;++i){
      auto n = get(i);
      if(uniques.find(n)!=std::end(uniques))return true;
      uniques.insert(n);
    }
    return false;
  }

  BitStream getInversion()const{
    auto res = BitStream(bits);
    for(size_t i=0;i<nofNumbers;++i)
      res.set(get(i),i);
    return res;
  }
};



template<size_t n>
size_t power(size_t x){
  if constexpr (n==0) return 1;
  else return power<n-1>(x)*x;
}

template<typename H,typename...T>
size_t head(H h,T...t){
  return h;
}

template<typename H,typename...T>
size_t noise(size_t bits,size_t x,H h,T... a){
  if constexpr (sizeof...(a) == 0)return h % (1<<bits);
  else return (h * power<sizeof...(a)>(x) + noise(bits,x,a...)) % (1<<bits);
}

size_t noise(size_t x,size_t bits,std::vector<size_t>const&pp){
  size_t r=0;
  for(size_t i=0;i<pp.size();++i){
    r+=pp[i];
    if(i<pp.size()-1)r*=x;
  }
  return r % (1<<bits);
}

struct PolynomNumbers{
  PolynomNumbers(size_t bits):numbers(bits){}
  BitStream numbers;
  std::vector<size_t>polynom;
  std::string polynomToStr()const{
    std::stringstream ss;
    char const*expstr[] = {"⁰","¹","²","³","⁴","⁵","⁶","⁷","⁸","⁹"};

    std::cerr << "f(x) = ";
    size_t exp = polynom.size()-1;
    bool first = true;
    for(auto const&x:polynom){
      if(first){
        if(x!=0){
          first = false;
        }
        else{
          if(exp!=0)
            std::cerr << "   ";
        }
      }else{
        if(x==0){
          std::cerr << "   ";
        }else{
          std::cerr << " + ";
        }
      }

      if(x==0){
        std::cerr << "   ";
      }else{
        if(x==1){
          if(exp==0)std::cerr << "1  ";
          else{
            std::cerr << " ";
          }
        }
        else{
          std::cerr << x  ;
        }
        if(exp>0)std::cerr << "x";
        else     std::cerr << " ";
        if(exp<=1)std::cerr << " ";
        else      std::cerr << expstr[exp];
      }
      


      exp--;
    }
    return ss.str();
  }
};

void compute(BitStream&stream,std::vector<size_t>const&p){
  size_t nofNumbers = 1<<stream.bits;
  for(size_t x=0;x<nofNumbers;++x){
    auto v = noise(x,stream.bits,p);
    stream.set(x,v);
  }
}

void init_p(std::vector<size_t>&p,size_t np){
  p.resize(np,0);
}

bool incrementAndReturnFalseOnLast(std::vector<size_t>&p,size_t bits){
  uint8_t ii=0;
  while(ii<p.size()){
    if(p[ii]+1 == (1ull<<bits)){
      p[ii] = 0;
      ii++;
    }else{
      p[ii]++;
      return true;
    }
  }
  return false;
}

struct RESs{
  std::vector<PolynomNumbers>data;
  void print(){
    for(auto const&p:data)
      std::cerr << p.polynomToStr() << std::endl;
  }
  ssize_t find(PolynomNumbers const&what){
    for(size_t i=0;i<data.size();++i)
      if(data.at(i).numbers == what.numbers)return i;
    return -1;
  }
};

RESs getNonRedundant(RESs const&r){
  RESs res;
  for(auto const&d:r.data){
    if(!d.numbers.hasRedundancy())
      res.data.push_back(d);
  }
  return res;
}

void compute(RESs&ress,size_t bits,size_t np){
  std::vector<size_t>p;
  init_p(p,np);
  do{
    ress.data.push_back(bits);
    auto&last = ress.data.back();
    last.polynom = p;
    compute(last.numbers,p);
  }while(incrementAndReturnFalseOnLast(p,bits));
}

int main(){
  RESs ress;
  compute(ress,3,3);
  ress.print();

  auto nonRedundant = getNonRedundant(ress);
  nonRedundant.print();
  std::cerr << "______________" << std::endl;
  for(size_t i = 0;i<nonRedundant.data.size();++i){
    auto noise     = nonRedundant.data.at(i);
    auto noise_inv = noise.numbers.getInversion();
    noise.print();
    std::cerr << "noise_inv:               ";
    noise_inv.print();
    bool found = false;
    size_t j;
    for(j=i+1;j<nonRedundant.data.size();++j){
      if(nonRedundant.data.at(i).numbers == noise_inv){
        nonRedundant.data.at(j).numbers.print();
        noise_inv.print();
        found = true;
        break;
      }
    }
    if(found){
      noise.print();
      noise.numbers.print();
      std::cerr << "INVERSION!!!!:" << std::endl;
      nonRedundant.data.at(j).print();
      nonRedundant.data.at(j).numbers.print();
      noise_inv.print();
    }
    std::cerr << std::endl;
  }

  auto a = std::vector<size_t>({1,2,3});
  auto b = std::vector<size_t>({1,2,3});
  if(a==b)std::cerr << "a == b" << std::endl;
  else std::cerr << "a != b" << std::endl;

  return 0;
}
