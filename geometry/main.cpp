#include<iostream>
#include<cstdint>
#include<cstdlib>
#include<cassert>
#include<vector>
#include<map>
#include<memory>

#include<glm/glm.hpp>

template<typename TYPE,size_t DIMENSION>
class Vector{
  public:
    TYPE data[DIMENSION];
    TYPE operator[](size_t const&i)const{
      assert(this != nullptr);
      assert(i < DIMENSION);
      return this->data[i];
    }
    TYPE&operator[](size_t const&i){
      assert(this != nullptr);
      assert(i < DIMENSION);
      return this->data[i];
    }
    int32_t compare(Vector<TYPE,DIMENSION>const&other)const{
      assert(this != nullptr);
      for(size_t i = 0; i < DIMENSION; ++i){
        if(this->data[i] < other.data[i])return -1;
        if(this->data[i] > other.data[i])return +1;
        return 0;
      }
    }
    bool operator==(Vector<TYPE,DIMENSION>const&other)const{assert(this != nullptr);return this->compare(other) == 0;}
    bool operator!=(Vector<TYPE,DIMENSION>const&other)const{assert(this != nullptr);return this->compare(other) != 0;}
    bool operator>=(Vector<TYPE,DIMENSION>const&other)const{assert(this != nullptr);return this->compare(other) >= 0;}
    bool operator<=(Vector<TYPE,DIMENSION>const&other)const{assert(this != nullptr);return this->compare(other) <= 0;}
    bool operator> (Vector<TYPE,DIMENSION>const&other)const{assert(this != nullptr);return this->compare(other) >  0;}
    bool operator< (Vector<TYPE,DIMENSION>const&other)const{assert(this != nullptr);return this->compare(other) <  0;}
    Vector<TYPE,DIMENSION> operator+(Vector<TYPE,DIMENSION>const&other){assert(this != nullptr);Vector<TYPE,DIMENSION>r;for(size_t i=0;i<DIMENSION;++i)r[i] = this->data[i] + other.data[i];return r;}
    Vector<TYPE,DIMENSION> operator-(Vector<TYPE,DIMENSION>const&other){assert(this != nullptr);Vector<TYPE,DIMENSION>r;for(size_t i=0;i<DIMENSION;++i)r[i] = this->data[i] - other.data[i];return r;}
    Vector<TYPE,DIMENSION> operator*(Vector<TYPE,DIMENSION>const&other){assert(this != nullptr);Vector<TYPE,DIMENSION>r;for(size_t i=0;i<DIMENSION;++i)r[i] = this->data[i] * other.data[i];return r;}
    Vector<TYPE,DIMENSION> operator/(Vector<TYPE,DIMENSION>const&other){assert(this != nullptr);Vector<TYPE,DIMENSION>r;for(size_t i=0;i<DIMENSION;++i)r[i] = this->data[i] / other.data[i];return r;}
};

class Point3D;
class Vector3D;
class Plane3D;

class Vector3DConstructor{
  public:
    virtual ~Vector3DConstructor(){}
};

class Point3D2Vector3DConstructor{
  public:
    Point3D2Vector3DConstructor(
        std::shared_ptr<Point3D>const&a,
        std::shared_ptr<Point3D>const&b){
      assert(this != nullptr);
      this->points[0] = a;
      this->points[1] = b;
    }
    std::shared_ptr<Point3D>points[2];
};

class Vector3D: public glm::vec3{
  public:
    std::unique_ptr<Vector3DConstructor>constructor;

};

class Point3DConstructor{
  public:
    virtual ~Point3DConstructor(){}
};

class Plane3D2Point3DConstructor: public Point3DConstructor{
  public:
    Plane3D2Point3DConstructor(
        std::shared_ptr<Plane3D>const&a,
        std::shared_ptr<Plane3D>const&b,
        std::shared_ptr<Plane3D>const&c){
      assert(this != nullptr);
      this->planes[0] = a;
      this->planes[1] = b;
      this->planes[2] = c;
    }
    std::shared_ptr<Plane3D>planes[3];
};

class Point3D: public glm::vec3{
  public:
    std::unique_ptr<Point3DConstructor>constructor;
    int  compare(Point3D const&other)const{
      assert(this != nullptr);
      for(size_t i = 0;i < 3; ++i){
        if((*this)[i] < other[i])return -1;
        if((*this)[i] > other[i])return +1;
      }
      return 0;
    }
    bool operator<(Point3D const&other)const{
      assert(this != nullptr);
      return this->compare(other) < 0;
    }
    bool operator>(Point3D const&other)const{
      assert(this != nullptr);
      return this->compare(other) > 0;
    }
    bool operator==(Point3D const&other)const{
      assert(this != nullptr);
      return this->compare(other) == 0;
    }
};

class Plane3DConstructor{
  public:
    virtual ~Plane3DConstructor(){}
};

class Point3D2Plane3DConstructor: public Plane3DConstructor{
  public:
    Point3D2Plane3DConstructor(
        std::shared_ptr<Point3D>const&a,
        std::shared_ptr<Point3D>const&b,
        std::shared_ptr<Point3D>const&c){
      assert(this != nullptr);
      this->points[0] = a;
      this->points[1] = b;
      this->points[2] = c;
    }
    std::shared_ptr<Point3D>points[3];
};

class Plane3D: public glm::vec4{
  public:
    std::unique_ptr<Plane3DConstructor>constructor;
};

int main(){

  return 0;
}
