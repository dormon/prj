#include<iostream>
#include<cstdint>
#include<cstdlib>
#include<cassert>
#include<vector>
#include<map>
#include"model.h"
// Syntax:
//
// Formula         : TermFormula
// Formula         : CompositeFormula
// TermFormula     : identifier
// TermFormula     : ~Formula
// TermFormula     : ( CompositeFormula )
// CompositeFormula: Formula -> Formula
//
// I0,I1,I2,Ii,... are shortcuts for identifiers
// F0,F1,F2,Fi,... are shortcuts for formulas
// _ is any Ii,Fi
//
//
// Axioms:
// F0 -> (F1 -> F0)
// (~F1 -> ~ F0) -> (F0 -> F1)
// (F0 -> (F1 -> F2)) -> ((F0 -> F1) -> (F0 -> F2))
//
// Deriving rule:
// O(F0,F0->F1) = F1
// 
// Substitution rule:
// S(old,where,new)
// S(I0,I0    ,F0) == F0
// S(I0,F0->F1,F2) == S(I0,F0,F2) -> S(I0,F1,F2)
// S(I0,(F0)  ,F1) == (S(I0,F0,F1))
// S(I0,~F0   ,F1) == ~S(I0,F0,F1)
// S(F0,F0    ,F1) == F1
// S(F0,F1->F2,F3) == S(F0,F1,F3) -> S(F0,F2,F3)
// S(F0,(F0)  ,F1) == (F1)
// S(F0,~F0   ,F1) == ~F1
// S(_ ,F0    ,_ ) == F0
//
// Ii == Ii
// Fi == Fi
//
//

// Main goal are:
// 1.) evolve commands according to inputs and outputs.
// min{ distance(f(input),output) } && min{ time(f(input)) } && min{ size(f(input)) }
// What is good command?
// Fast, small memory footprint, percise
//
// What is precision?
// Distance between command output and groundtruth and correlation.
// For some inputs, a command has to be very precise.
// For some inputs, a command does not have to be very precise.
// Distances:
// 1.) bit difference: number of bits that are different [0,sizeof(size_t)*8]
// 2.) unsigned integer distance: |A-B| [0,2^(sizeof(size_t)*8)-1]
// Correlation:
// sort input values
// do correlation on outputs
//
//
// 2.) Acquire new knowledge about universe in human readable format
//
// Crucial points:
// True formulas.
// Model of something.
//
// Model should behave as close as possible to entity it models.
//
// f is funcion, m(f) is model of funcion
// min{ distance(f(input),m(f)(input)) } && min{ time(m(f)(input)) } &
//
// Hard knoledge:
//
// Fundamental terms:
// integer number
// Set
//
// What is abstraction?
// A thing A is described by set of sentences.
// A thing B is described by set of sentences.
// An abstraction of these two things is decribed by set of sentences that is product of intersection of sentences of things A and B.
// Is this sufficient definition?
// 
// What is generalisation?
// A generalisation is in fact abscraction.
// Is this sufficient definition?
//
// What is model?
// A model is simplification. - not true model can be as complex as thing it models.
// A model M of thing A is projection of A from A's domain onto M in M's domain.
// When A behaves in one way, M behaves in another. These two behaviours are tied together.
// A language is a tool of making models in human brain of things in the reality.
// The mathematics is more precise model of reality. - Math can model things that are not real.
// A program is model of thing in reality or imagination.
// A function is a model of a behaviour of a thing in a program.
// The term model is probably the most important.
//
// A generalisation is process of generation more abstract models from more specialized models.
//
// About class hierarchy and what does inheritance do:
// 1.) A inherited child extends abilities of base class. (Child can add print function).
// 2.) A inherited child restricts abilities of base class. (Child can restric value ranges.
// So does inheritance extends or restricts base class?
// This conflict is fundamental.
// But there is simple solution:
// A inherited child always restrics base class.
// A base class does not tell anything about members it does not have.
// It does not restrics its possible values or methods.
// A inherited class expicitly restrics its members.
// A base class can contain all imaginable properties a does not restrics them as long as they are not listed in class declaration.
// When a child class inherite from base class and "add" new property, it restrics this class to always have this property of certain type.
//
// A class is model.
// 
// A model is composition of sub models - properties.
//
// Model: atomic term
// Model: {m1,m2,...,mN}, mi is Model
//
// What is gravity?
// Gravity is force between two objects.
// What is object?
// Object is model.
//
// Gravity is force between two models that have properties: mass and position.
// Force = gravity(Model{Mass,Position},Model{Mass,Position})
//
// What is force?
// A force is a value.
//
// What is value?
// Value is atomic term.
// Is there something else that is atomic and is different than value?
// - Set
// - Projection/Function
//
// Model is set of models.
// Model is a value.
// Model is projection.
//
// Whas is a function?
// A function is relation of several models in particular state.
//
// What is a state of model?
// A state of atomic term is its value.
// A state of composed model is state of its components.
//
// Value
// Real number
// Integer number
//
// What is semantic?
// Semantic: atomic semantic
// Semantic: {s1,s2,...sN}, si in Semantic
//
// What is the difference between a semantic and a model?
// 
// This is knowledge about universe:
// namespace Reality{
//   class Value                    {};
//   class Real            : Value  {};
//   class Integer         : Real   {};
//   class Natural         : Integer{};
//   class PositiveInteger : Natural{};
//
//   class Set       : Value {};
//   class FiniteSet : Set   {};
//   
//   powerSet   : Set       -> Set    ;
//   cardinality: Set       -> Integer;
//   setSize    : FiniteSet -> Integer;
//
//   class RealConstant    : Real   {};
//   class IntegerConstant : Integer{};
//
//   class GravitationalConstant : RealConstant   {};
//   class SpeedOfLight          : RealConstant   {};
//   class DaysPerWeek           : IntegerConstant{};
//
//   trunc: Real -> Integer;
//   add  : Real x Real -> Real;
//   div  : Real x Real -> Real;
//   mul  : Real x Real -> Real;
//
//   class Mass     : Real {};
//   class Distance : Real {};
//   class Force    : Real {};
//   class Time     : Real {};
//
//   gravity: Mass x Mass x Distance -> Force;
//
//   Force gravity(Mass a,Mass b,Distance r){
//     return mul(div(mul(a,b),mul(r,r)),GravitationalConstant);
//   }
// }
//
// This is what can actually be computed on computer:
// namespace CPU{
//   class Float {} isModelOf(Reality::Real   );
//   class SizeT {} isModelOf(Reality::Integer);
//
//   trunc: Float -> SizeT isModelOf(Reality::trunc);
// }
//
// Every integer is real.
// Not all reals are integers.
// Every float is real.
// Not all reals are floats.
// Every size_t is integer.
// Not all integer are size_t.
//
// Is Float in computes model of Real in mathematics?
// Yes.
//
// System has to be robust against new knoledge that can change models.
// 1) atomic term can be splitted into more atomic terms:
//   Atom into protons neutrons and electrons.
// 2) models can be described differently:
//   A time is discrete not real.
//
// New model should always create new instance and transformation function from old model.
//
// Atomic Model   : ID 0
// Composite Model: ID N ID1 ID2 ... IDN
//
// TODO:
// Integer setCardinality(Set)
// A set can have bigger cardinality that can be express by Integer.
//
// Is class property a function?
// How Are related properties/funtions to models.
//
//
//
// Time
//
// Value
// Real is(Value)
// Integer is(Real)
// Bool is(Integer)
//
// RealTime is(Real),isModelOf(Time).
//
// Real distance(Position,Position)
// Bool lessThanEqual(Real,Real)
//
// Sphere
// Position center(Sphere)
// Real     radius(Sphere)
// Bool     contains(Sphere,Position){
//   return lessThanEqual(distance(center(Sphere),Position),radius(Sphere));
// }
//
// Bool     isOnSurface(Sphere,Position){
//   return equal(distance(center(Sphere),Position),radius(Sphere));
// }
//
// SphereSurface is(Set)
// SphereSurface getSurface(Sphere){
//   return {Position in Space|isOnSurface(Sphere,Position};
// }
//
//
// Float isModelOf(Real)
// FloatTime is(Float),isModelOf(RealTime)
//
// Model: Base{
//   {Model property}*
// };
//
// 
// Model: atomic term
// Model: {m1,m2,...,mN}, mi is Model
//
// class ObjectList: public std::vector<Object>{
//   public:
// };
//
// class Inventory: public Object{
//   public:
//     ObjectList elements;
// };
//
//
// value
// set<WHAT,CARDINALITY>
// set<real,0>
// tuple
//
// set<,N>: set<,N+1>;
//
// value<CARDINALITY>
//
// value<N>: set<N+1>;
//
// Set can be finite.
// Set can be infinite.
//
// Finite set has N € Countable, N != inf<0>.
// Infinite set can be countable   aleph = 0.
// Infinite set can be uncountable aleph = 1.
// 
// A set has arbitrary size;
// A finite se is 
//
// class ArbitrarySet<N>: ArbitrarySet<N+1>{};
// class FiniteSet: ArbitrarySet<0>{};
//
// Digits: 0,1
//
// setSize(FiniteSet      ) = N     , N € ArbitrarySet<0>;
// setSize(ArbitrarySet<N>) = inf<N>, N & ArbitrarySet<0>;
//
//
// 
// model set<Real> = set
// using setOfReals = set<Real>;
// using countableSet = set<,1>;
//
// set x set
//
// model Real   : value;
// model Integer: Real ;
//
// model Tensor;
// model Vector: Matrix
// mode
//
// 
// 0. exists y: forall x: !(x in y)
// 1. forall x: forall y: exists z: forall w: w in z <=> w in x or w in y
// 2. forall x: forall y: isSet(x) && isSet(y) <=> isProposition(x in y)
// 3: forall x: exists y: forall z: exists w: z in y <=> z in w && w in x
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

class CommandInterfaceInfo{
  public:
    enum Type{
      COMMAND_READS  = 1lu << 0lu,
      COMMAND_WRITES = 1lu << 1lu,
    };
    std::vector<Type>entries;
};

using AtomicCommand = void(*)(size_t*const&);
class AtomicCommandInfo{
  public:
    AtomicCommand        ptr      ;
    std::string          name     ;
    CommandInterfaceInfo interface;
    AtomicCommandInfo(
        AtomicCommand        const&f = nullptr,
        std::string          const&n = ""     ,
        CommandInterfaceInfo const&i = CommandInterfaceInfo()):ptr(f),name(n),interface(i){}
};



class Value                    {};
class Real    : public Value   {};
class Integer : public Real    {};
class Float   : public Real    {};
class SizeT   : public Integer {};

Integer trunc(Real  const&);
SizeT   trunc(Float const&);

// CompositeCommand: compositeCommandId nofLayers Layers*
// Layer           : nofCommands Command*
// Command         : commandPointer operandOffset*

class Jak;

class Jak{
  public:
    enum StatementType{
      JUMP              = 0,
      ATOMIC_COMMAND    = 1,
      COMPOSITE_COMMAND = 2,
    };
    size_t*programCounter = nullptr;
    //memory contains data for currently evolving program
    size_t*memory         = nullptr;
    size_t*memoryEnd      = nullptr;
    //program contains currently evolving program
    size_t*program        = nullptr;
    size_t*programEnd     = nullptr;
    //knowledge contains discovered functions
    size_t*knowledge      = nullptr;
    size_t*knowledgeEnd   = nullptr;
    std::map<AtomicCommand,AtomicCommandInfo>atomicCommands;
    std::map<size_t       ,size_t const*    >compositeCommands;

    bool checkProgramCounter()const{
      assert(this != nullptr);
      return this->programCounter < this->memoryEnd;
    }
    bool incrementProgramCounter(){
      assert(this != nullptr);
      this->programCounter++;
      return this->checkProgramCounter();
    }
    bool checkProgramCounter(size_t const&offset){
      assert(this != nullptr);
      return this->programCounter + offset < this->programEnd;
    }
    bool operator()(){
      assert(this != nullptr);
      this->programCounter = this->program;
      for(;;){

        if(!this->checkProgramCounter())return false;

        auto const statementType = static_cast<StatementType>(*this->programCounter);

        if(statementType == JUMP){
          if(!this->incrementProgramCounter())return false;
          this->programCounter = this->program + (*programCounter);
          continue;
        }

        if(statementType == ATOMIC_COMMAND){
          if(!this->incrementProgramCounter())return false;
          auto const command = reinterpret_cast<AtomicCommand>(*programCounter);
          if(!this->incrementProgramCounter())return false;
          auto const it = this->atomicCommands.find(command);
          if(it == this->atomicCommands.end())return false;
          auto const info = it->second;
          auto const nofEntries = info.interface.entries.size();
          if(!this->checkProgramCounter(nofEntries))return false;
          command(programCounter);
          this->programCounter += nofEntries;
          continue;
        }

        if(statementType == COMPOSITE_COMMAND){
          if(!this->incrementProgramCounter())return false;
          auto const nofLayers = *this->programCounter;
          if(!this->incrementProgramCounter())return false;
          for(size_t l = 0; l < nofLayers; ++l){

          }
          
          static_cast<void>(nofLayers);
        }

      }
    }

};

int main(){
  Domain realWorld;
  auto const valueId = realWorld.addAtomicModel("Value");
  realWorld.addVectorModel("vector",valueId,valueId);


  return 0;
}
