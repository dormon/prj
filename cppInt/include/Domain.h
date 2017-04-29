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
//
// Atomics:
// +(r,a,b)
// <(r,a,b)
// 
// +(r,a,b) <=> +(r,b,a)
// +(a,a,0)
//
// ==(<(0,a,b) && <(0,b,a),a,b)
//
// *(r,a,b){
//   =(r,0);
//   DEC(i,0);
//   DEC(c,0);
//   <(c,i,b);
//   while(c){
//     +(r,r,a);
//     +(i,i,1);
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

    //CommandId addAtomicCommand(
    //    std::string      const&name       ,

        
  protected:
    bool _checkBaseModels(
        std::set<ModelId>const&bases)const;
    ModelId _addModel(
        std::string      const&name ,
        ModelDescription*const&model);
};
