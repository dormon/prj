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
//
//
//
//
//
//
//
//


using ModelId = size_t;

class ModelDescription{
  public:
    enum Type{
      ATOMIC   ,
      VECTOR   ,
      COMPOSITE,
    }type;
    ModelId base;
    ModelDescription(Type const&t,ModelId const&base):type(t),base(base){}
};

class AtomicModelDescription: public ModelDescription{
  public:
    AtomicModelDescription(ModelId const&base);
};

class VectorModelDescription: public ModelDescription{
  public:
    ModelId inner;
    VectorModelDescription(
        ModelId const&base ,
        ModelId const&inner);
};

class CompositeModelDescription: public ModelDescription{
  public:
    std::set<ModelId>inners;
    CompositeModelDescription(
        ModelId          const&base  ,
        std::set<ModelId>const&inners);
};

class Domain{
  public:
    std::map<ModelId,ModelDescription*>models;
    std::map<ModelId,std::string>id2Name;
    std::map<std::string,ModelId>name2Id;
    ModelId static const nonexistingModel;

    ModelId addAtomicModel(
        std::string const&name                                ,
        ModelId     const&baseModel = Domain::nonexistingModel);

    ModelId addVectorModel(
        std::string const&name      ,
        ModelId     const&baseModel ,
        ModelId     const&innerModel);

    ModelId addCompositeModel(
        std::string      const&name       ,
        ModelId          const&baseModel  ,
        std::set<ModelId>const&innerModels);
};
