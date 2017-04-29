#include<iostream>
#include<sstream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>
#include<utility>
#include<cassert>
#include<limits>

#include<geCore/fsa/MealyMachine.h>
#include<geCore/fsa/transitionChooser.h>
#include<geCore/fsa/mapTransitionChooser.h>

void printPP(MealyMachine*,void* const&){std::cout<<"++"<<std::endl;}
void printP (MealyMachine*,void* const&){std::cout<<"+" <<std::endl;}
void printMM(MealyMachine*,void* const&){std::cout<<"--"<<std::endl;}
void printM (MealyMachine*,void* const&){std::cout<<"-" <<std::endl;}

int main(){
  MealyMachine mm(1);
  auto S = mm.addState(std::make_shared<MapTransitionChooser<1>>());
  auto P = mm.addState(std::make_shared<MapTransitionChooser<1>>());
  auto M = mm.addState(std::make_shared<MapTransitionChooser<1>>());
  auto E = mm.addState(std::make_shared<MapTransitionChooser<2>>());

  mm.addTransition(S,std::vector<MealyMachine::BasicUnit>({'+'}).data(),P);
  mm.addTransition(S,std::vector<MealyMachine::BasicUnit>({'-'}).data(),M);
  mm.addElseTransitions(S,E);
  mm.addEOFTransition(S);

  mm.addTransition(P,std::vector<MealyMachine::BasicUnit>({'+'}).data(),S,printPP);
  mm.addTransition(P,std::vector<MealyMachine::BasicUnit>({'-'}).data(),M,printP );
  mm.addElseTransitions(P,E,printP);
  mm.addEOFTransition(P,printP);

  mm.addTransition(M,std::vector<MealyMachine::BasicUnit>({'-'}).data(),S,printMM);
  mm.addTransition(M,std::vector<MealyMachine::BasicUnit>({'+'}).data(),P,printM );
  mm.addElseTransitions(M,E,printM);
  mm.addEOFTransition(M,printM);

  mm.addElseTransitions(E,E);
  mm.addEOFTransition(E);

  auto str = "++--+-+-++---";

  mm.begin();
  mm.parse((MealyMachine::BasicUnit const*)str,std::strlen(str));
  mm.end();

  return 0;
}
