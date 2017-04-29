#include"tests.h"

void test_run(){
  if(!test_float        ())std::cerr<<"test_float         failed"<<std::endl;
  if(!test_floatList    ())std::cerr<<"test_floatList     failed"<<std::endl;
  if(!test_addTransition())std::cerr<<"test_addTransition failed"<<std::endl;
}

const char* exponent  = "eE" ;
const char* sufix     = "f"  ;
const char* delimiter = "."  ;

bool test_float(){
  ge::util::FSA*fsa=new ge::util::FSA("S",
      "S","+-"                ,"A",
      "S",delimiter           ,"B",
      "S",ge::util::FSA::digit,"C",
      "A",delimiter           ,"B",
      "A",ge::util::FSA::digit,"C",
      "B",ge::util::FSA::digit,"D",
      "C",ge::util::FSA::digit,"C",
      "C",delimiter           ,"D",
      "C",exponent            ,"E",
      "C",sufix               ,"H",
      "D",ge::util::FSA::digit,"D",
      "D",exponent            ,"E",
      "D",sufix               ,"H",
      "E","+-"                ,"F",
      "E",ge::util::FSA::digit,"G",
      "F",ge::util::FSA::digit,"G",
      "G",ge::util::FSA::digit,"G",
      "G",sufix               ,"H");

  std::string testString="+831.3232e32f";
  fsa->run(testString);
  std::string result=fsa->getAlreadyReadString();
  delete fsa;
  return result==testString;
}
void registerStart(ge::util::FSA*fsa,void*data){
  *(unsigned*)data=fsa->getCurrentPosition();
};
void printFloat(ge::util::FSA*fsa,void*data){
  unsigned pos=*(unsigned*)data;
  std::cerr<<"#"<<fsa->getAlreadyReadString().substr(pos)<<"#"<<std::endl;
};

bool test_floatList(){
  unsigned startPos;
  ge::util::FSA*fsa=new ge::util::FSA("S",
      "S","+-"                ,"A",registerStart,(void*)&startPos,
      "S",delimiter           ,"B",registerStart,(void*)&startPos,
      "S",ge::util::FSA::digit,"C",registerStart,(void*)&startPos,
      "S",ge::util::FSA::els  ,"S",
      "A",delimiter           ,"B",
      "A",ge::util::FSA::digit,"C",
      "B",ge::util::FSA::digit,"D",
      "C",ge::util::FSA::digit,"C",
      "C",delimiter           ,"D",
      "C",exponent            ,"E",
      "C",sufix               ,"H",
      "C",ge::util::FSA::els  ,"S",printFloat,(void*)&startPos,
      "C",ge::util::FSA::eof  ,"S",printFloat,(void*)&startPos,
      "D",ge::util::FSA::digit,"D",
      "D",exponent            ,"E",
      "D",sufix               ,"H",
      "D",ge::util::FSA::els  ,"S",printFloat,(void*)&startPos,
      "D",ge::util::FSA::eof  ,"S",printFloat,(void*)&startPos,
      "E","+-"                ,"F",
      "E",ge::util::FSA::digit,"G",
      "F",ge::util::FSA::digit,"G",
      "G",ge::util::FSA::digit,"G",
      "G",sufix               ,"H",
      "G",ge::util::FSA::els  ,"S",printFloat,(void*)&startPos,
      "G",ge::util::FSA::eof  ,"S",printFloat,(void*)&startPos,
      "H",ge::util::FSA::els  ,"S",printFloat,(void*)&startPos,
      "H",ge::util::FSA::eof  ,"S",printFloat,(void*)&startPos);

  std::string testString="asdasd+831.3232e32fasdasd-3232.323212e+32fasdasd32fsd32f";
  fsa->run(testString);
  std::string result=fsa->getAlreadyReadString();
  delete fsa;
  return result==testString;
}

bool test_addTransition(){
  unsigned pos;
  ge::util::FSA*fsa=new ge::util::FSA("S");
  fsa->addTransition    ("S",ge::util::FSA::space,"S");
  fsa->addElseTransition("S","A",registerStart,(void*)&pos);
  fsa->addTransition    ("A",ge::util::FSA::space,"S",printFloat,(void*)&pos);
  fsa->addElseTransition("A","A");
  fsa->addEOFTransition ("A","B",printFloat,(void*)&pos);
  std::string testString=" asdasd \t \r \n  asd qwe  we qeww ass    ";
  fsa->run(testString);
  std::string result=fsa->getAlreadyReadString();
  delete fsa;
  return result==testString;
}
