#include<iostream>
#include<geCore/fsa/fsa.h>

std::string parse(std::string data){
  std::string pData = "";
  auto createPP = [](ge::core::FSA*,void*data){
    *((std::string*)data)+="++ ";
  };
  auto createPgoBack = [](ge::core::FSA*fsa,void*data){
    *((std::string*)data)+="+ ";
    fsa->goBack();
  };
  auto createP = [](ge::core::FSA*,void*data){
    *((std::string*)data)+="+ ";
  };
  auto createMM = [](ge::core::FSA*,void*data){
    *((std::string*)data)+="-- ";
  };
  auto createMgoBack = [](ge::core::FSA*fsa,void*data){
    *((std::string*)data)+="- ";
    fsa->goBack();
  };
  auto createM = [](ge::core::FSA*,void*data){
    *((std::string*)data)+="- ";
  };

  static auto fsa=ge::core::FSA(
      "START",
      "START","+"               ,"PLUS" ,
      "START","-"               ,"MINUS",
      "START",ge::core::FSA::eof,"END"  ,
      "PLUS" ,"+"               ,"START",createPP     ,(void*)&pData,//create ++
      "PLUS" ,"-"               ,"START",createPgoBack,(void*)&pData,//create +, goBack
      "PLUS" ,ge::core::FSA::eof,"END"  ,createP      ,(void*)&pData,//create +
      "MINUS","-"               ,"START",createMM     ,(void*)&pData,//create -, goBack
      "MINUS","+"               ,"START",createMgoBack,(void*)&pData,//create --
      "MINUS",ge::core::FSA::eof,"END"  ,createM      ,(void*)&pData //create -
      );
  fsa.run(data);
  return pData;
}

int main(){
  std::size_t s=0;
  for(std::size_t i=0;i<10000000;++i)
    s+=parse("++--+-+-"+i%2?std::string("++"):"").size();
  std::cout<<s<<std::endl;
  return 0;
}
