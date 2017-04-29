#pragma once

#include<Node.h>
#include<AllContainer.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_access.hpp>

class Node2d: public Node{
  public:
    glm::mat3 mat;
    AllContainer data;
    Node2d(glm::mat3 const&m = glm::mat3(1.f),Node2d*parent = nullptr);
    Node2d(Node2d*parent,glm::mat3 const&m = glm::mat3(1.f));
    virtual ~Node2d();
};

inline Node2d::Node2d(glm::mat3 const&m,Node2d*parent):Node(parent),mat(m){}

inline Node2d::Node2d(Node2d*parent,glm::mat3 const&m):Node(parent),mat(m){}

inline Node2d::~Node2d(){}

