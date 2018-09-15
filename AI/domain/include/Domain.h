#pragma once
#include<iostream>
#include<cstdlib>
#include<vector>
#include<cassert>
#include<map>
#include<set>
#include<limits>
#include<sstream>
#include<geCore/ValuePrinter.h>
#include<geCore/ErrorPrinter.h>

// Necessary 
// if
// while
// body
// DEC
// bool
//
// if, while depend on bool
//
// Model Value;
// Model Array: array<Value>;
// Model Real: Value;
// Model Integer: Real;
// Model Natural: Integer;
// Model PositiveNatural: Natural;
//
// Model RealArray = array<Real>;
// Model RealVector3D: RealArray;
// Model RealVector1D = Real;
// Model IntegerVector = array<Integer>;
// Model RealPosition = RealVector;
// Model RealPosition3D: Position;
// Model RealNormal3D: RealVector;
// Model RealNormal3D: RealVector3D;
// Model RealSize3D: RealVector3D;
// Model RealAABB3D = {RealPosition3D,RealSize3D};
//
// What is the difference between RealVector and RealVector3D?
// RealVector3D : RealVector;
//
// DECLARE(PositiveNatural,a);
// DECLARE(RealVector3D,b);
// getDimension(a,b);
// a == 3
// getDimension(out PositiveNatural a,in RealVector   b);
//
// getDimension(out PositiveNatural a,in RealVector3D b){
//   =(a,3);
// }
//
// getDimension(out PositiveNatural a,in RealVector2D b){
//   =(a,2);
// }
//
// forall b
//
//
//
// Position namespace{
// }
//
// 3D namespace{
// }
//
// Real namespace{
// }
//
//
// ######################
// Atomics:
// ######################
// 0. DEC(a)
// 1. 0(a)
// 2. ++(a)
// 3. <(r,a,b)
// 4. nand(r,a,b)
//
// ######################
// Composite:
// ######################
// !(r,a){
//   nand(r,a,a);
// }
//
// &&(r,a,b){
//   DEC(c);
//   nand(c,a,b);
//   !(r,c);
// }
//
// ||(r,a,b){
//   DEC(aa);
//   DEC(bb);
//   !(aa,a);
//   !(bb,b);
//   nand(r,aa,bb);
// }
//
// ==(r,a,b){
//   DEC(x);
//   DEC(y);
//   DEC(xx);
//   DEC(yy);
//   <(x,a,b);
//   <(y,b,a);
//   !(xx,x);
//   !(yy,y);
//   &&(r,xx,yy);
// }
//
// !=(r,a,b){
//   DEC(x);
//   ==(x,a,b);
//   !(r,x);
// }
// 
// =(r,a){
//   DEC(c);
//   0(r);
//   0(c);
//   <(c,r,a);
//   while(c){
//     ++(r);
//     <(c,r,a);
//   }
// }
//
// +(r,a,b){
//   =(r,a);
//   DEC(i);
//   DEC(c);
//   0(i);
//   0(c);
//   <(c,i,b);
//   while(c){
//     ++(r);
//     ++(i);
//     <(c,i,b);
//   }
// }
//
// *(r,a,b){
//   DEC(i);
//   DEC(c);
//   0(i);
//   0(c);
//   0(r);
//   <(c,i,b);
//   while(c){
//     +(r,r,a);
//     ++(i);
//     <(c,i,b);
//   }
// }
//
// while(a)while(b)cmd(); <=> while(b)while(a)cmd();
//
// cmd2(a,cmd,args...) == while(a)cmd(args...);
//
//
//
//
//
//

// Model: atomicModel
// Model: Model[]
// Model: {m1,...,mn} mi is Model
//
// First, there is image of object in consciousness, then the name for that object comes.
// Domain: 
// Real     domain,
// Brain    domain,
// Computer domain,
// Human Language,
// English
// Czech
// Mathematics
//
// A thought is object of brain domain and it is a model of object of real domain.
// Human Language is model of thoughts.
//
// A table is composed of a dest and legs.
//
// class Table{
//   Desk;
//   Leg[];
// };
//

//class Value                    {};
//class Vector      :Value       {};
//class Real        :Value       {};
//class Integer     :Real        {};
//class RealVector  :Vector      {};
//class RealVector3D:RealVector  {};
//class Position    :RealVector  {};
//class Position3D  :RealVector3D{};
//class Position3D  :Position    {};
//
//class Light{float intensity;};
//class OmniLight: Light{vec3 position;};
//
//class ColoredLight: Light{vec3 color;};
//class PhongLight: ColoredLight, OmniLight{};
//
//
//void vectorSize(Integer out,Vector in);
//
//Position3D is Position
//Position   is RealVector
//Position3D is Position
//
//Simplex0 is point
//Simplex1 is line segment
//Simplex2 is triangle
//Simplex3 is tetrahedron
//
//Simplex0 is Simplex
//Simplex1 is Simplex
//Simplex2 is Simplex
//Simplex3 is Simplex
//
//
//
//OrientedLineSegment is LineSegment
//
//All Positions are Vectors
//Not all Vectors are Positions
//
//All Velocities are Vectors
//Not all Vectors are Velocities
//
//
//
//
//
//
//
//
//

#include<ModelDescription.h>
#include<CommandDescription.h>

class Domain{
  public:
    std::map<ModelId  ,ModelDescription  *>models  ;
    std::map<CommandId,CommandDescription*>commands;

    std::map<ModelId    ,std::string>model2Name;
    std::map<std::string,ModelId    >name2Model;

    std::map<CommandId  ,std::string>command2Name;
    std::map<std::string,CommandId  >name2Command;


    ModelId   static const nonexistingModel  ;
    CommandId static const nonexistingCommand;

    ModelId addAtomicModel(
        std::string      const&name           ,
        std::set<ModelId>const&baseModels = {});

    ModelId addVectorModel(
        std::string      const&name      ,
        std::set<ModelId>const&baseModels,
        ModelId          const&innerModel);

    ModelId addCompositeModel(
        std::string      const&name       ,
        std::set<ModelId>const&baseModels ,
        std::set<ModelId>const&innerModels);

    ModelId addAtomicModel(
        std::string          const&name ,
        std::set<std::string>const&bases);

    ModelId addVectorModel(
        std::string          const&name      ,
        std::set<std::string>const&baseModels,
        std::string          const&innerModel);

    ModelId addCompositeModel(
        std::string          const&name       ,
        std::set<std::string>const&baseModels ,
        std::set<std::string>const&innerModels);



    ModelId getModelId(std::string const&name)const;

    CommandId addAtomicCommand(
        std::string              const&name  ,
        std::vector<CommandInput>const&inputs);

    CommandId addBodyCommand(
        std::string                     const&name   ,
        std::vector<CommandId>          const&cmds   ,
        std::vector<std::vector<size_t>>const&mapping);

    CommandId addAtomicCommand(
        std::string             const&name    ,
        std::vector<std::string>const&models  ,
        std::string             const&accesses);



    template<typename...ARGUMENTS>
      ModelId addAtomicModel(
          std::string const&   name ,
          ARGUMENTS   const&...bases);
        
  protected:
    bool _checkBaseModels(
        std::set<ModelId>const&bases)const;
    ModelId _addModel(
        std::string      const&name ,
        ModelDescription*const&model);
    CommandId _addCommand(
        std::string        const&name   ,
        CommandDescription*const&command);
};

template<typename...ARGUMENTS>
ModelId Domain::addAtomicModel(
    std::string const&   name ,
    ARGUMENTS   const&...bases){
  return this->addAtomicModel(name,{this->getModelId(bases)...});
}

