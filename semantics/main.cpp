#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<list>
#include<cstring>
#include<cassert>
#include<functional>
#include<typeinfo>
#include<typeindex>

using SymbolId = size_t;

//properties describe compex thing by its simpler parts
//properties can also have properties
//there are atomic properties that cannot be splitted into simpler properties
//things are instances of one property that completely describes it
//
//there are a lot of words for describing basic properties
//
//attribute = property = value = number
//all masses are real numbers, but not all real numbers are masses
//mass is semantic
//all numbers are vectors, but not all vectors are numbers
//the most abstract property is: infinite tensor of number (that is infinite aleph)
//
//is vector more abstract than number?
//all numbers can be vector, but not all vectors can be numbers
//vector has properties: number, length so vector is extension of number
//on the other hand number is restriction of vector
//inheritance can extend properties of object
//inheritance can also reduce/specialise properties of object
//what should inheritance do?
//If you create storage for base type it should be able to store children type values
//
//what should be on top of class hierarchy? the most abstract thing
//number vs value
//vector can be viewed as scalar or shorter vector there for scalar is more basic
//Matrix can be viewed as vector or smaller matrix therefor vector is more basic
//
//templates describes the most children thing
//abstract class describes the most top thing
//templates are restricted/specialised
//basic classes are extended
//
//
//property: length, width are atomic, they cannot be splitted into simpler properties
//property: velocity (vector) is not atomic, it can be splitted 
//into simpler properties: x-velocity, y-velocity, z-velocity or velocity (scalar) and direction
//property: direction in not atomic, it can be splitted into x,y,z
//AABB: is not atomic, origin, size
//tensor->vector->scalar->numeric->aleph
//vector3D
//difference:
//vector3D and velocity3D
//velocity3D is vector3D
//vector3D is not velocity3D
//vector3D + semantic(velocity) = velocity3D
//
//velocity = delta distance / delta time
//
//time is atomic property
//distance is atomic property, distance is delta position
//mass is atomic property, maybe it is no longer atomic due to quantum physics
//energy is atomic
//charge is atomic
//acceleration = delta velocity / delta time
//force = acceleration * mass
//momentum = velocity * mass
//
//dimension
//real number

/*
class Value{};

class Number: public Value{};//all possible numbers, aleph infinity
class RealNumber: public Number{};//all possible real numbers, aleph 1
class IntegerNumber: public RealNumber{};//all possible whole numbers, aleph 0
class NaturalNumber: public IntegerNumber{};//all non negative whole numbers
*/

class Value{};

template<typename TYPE>
class Scalar: public Value{
  public:
    TYPE data;
};

template<typename TYPE,size_t DIMENSION>
class Vector: public Value{
  public:
    std::array<Scalar<TYPE>,DIMENSION>data;
};

template<typename TYPE,size_t X,size_t Y>
class Matrix: public Value{
  public:
    Vector<Vector<TYPE,Y>,X>data;
};

using SemanticId = size_t;

class Semantic{
  public:
    SemanticId id;
    Semantic(SemanticId identifier):id(identifier){}
    virtual ~Semantic(){}
};

class AtomicSemantic: public Semantic{
  public:
    std::string name;

};
//Atomic sementics:
//position
//color
//
//array := 
//
//array[float,3] := TYPE<float> + scalar<size_t>(3)
//
//vector3D := array[type,3] + vector
//
//position1D := array[type,1] + position
//position2D := array[type,2] + position
//position3D := array[type,3] + position
//position4D := array[type,4] + position
//
//vector := endPoint - startPoint
//
//
//
//
//point := position
//vector := point - point
//
//
//
//


class Numeric{
};

class Property{
  public:
};


int main(){
  return 0;
}
