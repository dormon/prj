#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

class Point{
  public:
    float _x;
    float _y;
    float _z;
    Point(float x,float y,float z){
      this->_x = x;
      this->_y = y;
      this->_z = z;
    }
    Point(Point const&point){
    }
};

class Points{
  public:
    std::vector<Point>_points;
    void add(Point const&point){
      this->_points.push_back(point);
    }
};

int main(){
  Point a(10,11,12);
  Points pp;
  pp.add(a);
  a._x+=.2f;
  std::cout<<a._x<<std::endl;
  std::cout<<pp._points[0]._x<<std::endl;

  std::set<int>va;
  va.insert(12);
  va.insert(13);
  va.insert(14);

  std::set<int>vb;
  vb=va;
  va.insert(1000);

  for(auto x:va)
    std::cout<<x<<std::endl;
  for(auto x:vb)
    std::cout<<x<<std::endl;

  return 0;
}
