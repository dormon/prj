#ifndef _LANG_H_
#define _LANG_H_

#include<iostream>
#include<vector>
#include<glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_access.hpp>

//zaridit dedicnost symbolu
//zaridit evoluci pravidel pravidlo se aplikuje a zmeni i pravidla (treba jejich pravdepodobnost)

namespace lang{
  class Random{
    private:
      unsigned value;
    public:
      Random(unsigned seed);
      unsigned getValue();
      float    getUniform();
      float    getValue(float minValue,float maxValue);
      unsigned getValue(unsigned range);
  };

  class Symbol{
    private:
      std::string name;
    public:
      Symbol(std::string name);
      Symbol*copy();
      std::string getName();
      virtual void copyData(Symbol*newSymbol);
      virtual void draw(glm::mat4 mvp);
      virtual ~Symbol();
  };
  class String
  {
    public:
      std::vector<Symbol*>string;
      std::string getString();
      ~String();
  };

  class Generator;

  class Rule
  {
    private:
      std::string _name;
      void _split(String&string,std::string str);
    public:
      std::string getName();
      String leftSide;
      virtual bool condition(Generator*generator);
      virtual String*body(Generator*generator)=0;
      Rule(std::string name,std::string left);
      std::string getString();
      virtual ~Rule();
  };
  class RuleList
  {
    public:
      std::vector<unsigned>positions;
      std::vector<Rule*>rules;
      ~RuleList();
  };
  class Generator
  {
    public:
      Random *_random;
      Generator(unsigned seed,String*startString);
      ~Generator();
      String   *currentString;
      unsigned  currentPosition;
      String   *newString;
      std::vector<Symbol*>symbols;
      std::vector<Rule*>rules;
      std::vector<String*>strings;
      std::vector<RuleList*>appliedRules;
      void next();
      std::string getCurrentString();
  };
}

#endif//_LANG_H_
