#include"lang.h"
#include<limits>

using namespace lang;

Random::Random(unsigned seed){
  this->value=seed;
}

unsigned Random::getValue(){
	unsigned next = this->value;
	unsigned result;

	next *= 1103515245;
	next += 12345;
	result = (unsigned int) (next / 65536) % 2048;

	next *= 1103515245;
	next += 12345;
	result <<= 10;
	result ^= (unsigned int) (next / 65536) % 1024;

	next *= 1103515245;
	next += 12345;
	result <<= 10;
	result ^= (unsigned int) (next / 65536) % 1024;

	this->value = next;
	return result;
}

float Random::getUniform(){
  float result=this->getValue();
  return result/std::numeric_limits<unsigned>::max()*2;
}

unsigned Random::getValue(unsigned range){
  return (unsigned)(this->getUniform()*range);
}

float Random::getValue(float minValue,float maxValue){
  return this->getUniform()*(maxValue-minValue)+minValue;
}

Symbol::Symbol(std::string name){
  this->name = name;
}

Symbol*Symbol::copy(){
  Symbol*newSymbol = new Symbol(this->name);
  this->copyData(newSymbol);
  return newSymbol;
}


void Symbol::copyData(Symbol*){
  //empty
}

std::string Symbol::getName(){
  return this->name;
}

Symbol::~Symbol(){

}

void Symbol::draw(glm::mat4){

}

std::string String::getString(){
  std::string result="";
  if(!this->string.size())return result;
  for(unsigned i=0;i<this->string.size()-1;++i)
    result+=this->string[i]->getName()+" ";
  result+=this->string[this->string.size()-1]->getName();
  return result;
}

String::~String(){
  for(unsigned s=0;s<this->string.size();++s)
    delete this->string[s];
  this->string.clear();
}

void Rule::_split(String&string,std::string str){
  int pos=0;
  while((pos=str.find(" "))!=-1){
    string.string.push_back(new Symbol(str.substr(0,pos)));
    while(str[pos]==' ')++pos;
    str=str.substr(pos);
  }
  if(str!="")
    string.string.push_back(new Symbol(str));
}

std::string Rule::getName(){
  return this->_name;
}

bool Rule::condition(Generator*){
  return true;
}


Rule::Rule(std::string name,std::string left){
  this->_name=name;
  this->_split(this->leftSide ,left );
}

std::string Rule::getString(){
  std::string result;
  result+=this->leftSide.getString();
  result+=" -> ";
  return result;
}

Rule::~Rule(){
  for(unsigned s=0;s<this->leftSide.string.size();++s)
    delete this->leftSide.string[s];
  this->leftSide.string.clear();
}

RuleList::~RuleList(){

}


Generator::Generator(unsigned seed,String*startString){
  this->_random=new Random(seed);
  this->appliedRules.push_back(new RuleList);
  this->strings.push_back(startString);
  this->currentPosition=0;
  this->currentString=startString;
}

Generator::~Generator(){
  delete this->_random;
  for(unsigned r=0;r<this->rules.size();++r)
    delete this->rules[r];
  this->rules.clear();
  for(unsigned ar=0;ar<this->appliedRules.size();++ar)
    delete this->appliedRules[ar];
  this->appliedRules.clear();
  /*
  for(unsigned s=0;s<this->strings.size();++s)
    delete this->strings[s];
    */
  for(unsigned s=0;s<this->symbols.size();++s)
    delete this->symbols[s];
  this->appliedRules.clear();
}

void Generator::next(){
  unsigned currentStringSize=this->currentString->string.size();
  this->newString=new String();
  RuleList*newRules=new RuleList();
  bool evolving=false;
  while(this->currentPosition<currentStringSize){
    unsigned r=0;
    std::vector<Rule*>compatibleRules;
    for(;r<this->rules.size();++r){
      unsigned leftSideSize=this->rules[r]->leftSide.string.size();
      if(this->currentPosition+leftSideSize>currentStringSize)continue;
      unsigned l=0;
      for(;l<leftSideSize;++l){
        if(currentString->string[currentPosition+l]->getName()!=this->rules[r]->leftSide.string[l]->getName())break;
      }
      if(l==leftSideSize)//rule is compatible
        if(this->rules[r]->condition(this))
          compatibleRules.push_back(this->rules[r]);
    }
    if(!compatibleRules.size()){//no rule can be applied
      //std::cerr<<"no rule"<<std::endl;
      this->newString->string.push_back(currentString->string[currentPosition]);
      currentPosition++;
    }else{
      Rule*selectedRule=compatibleRules[this->_random->getValue(compatibleRules.size())];
      newRules->positions.push_back(this->currentPosition);
      newRules->rules.push_back(selectedRule);
      String*rightSide=selectedRule->body(this);
      for(unsigned i=0;i<rightSide->string.size();++i){
        this->newString->string.push_back(rightSide->string[i]);
        this->symbols.push_back(rightSide->string[i]);
      }
      rightSide->string.clear();
      delete rightSide;
      currentPosition+=selectedRule->leftSide.string.size();
      evolving=true;
    }
    compatibleRules.clear();
  }
  if(evolving){
    this->strings.push_back(this->newString);
    this->appliedRules.push_back(newRules);
    this->currentString   = this->newString;
  }else{
    newString->string.clear();
    delete newString;
    delete newRules;
  }
  this->currentPosition = 0;
  this->newString       = NULL;
}

std::string Generator::getCurrentString(){
  std::string result;
  for(unsigned i=0;i<this->currentString->string.size();++i)
    result+=this->currentString->string[i]->getName();
  return result;
}
