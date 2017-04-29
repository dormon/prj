#ifndef _MAIN_HPP_
#define _MAIN_HPP_

#include<iostream>
#include<fstream>
#include<istream>
#include<sstream>
using namespace std;

#include<GL/glew.h>
#include<GL/glext.h>
#include<GL/gl.h>
#include<SDL2/SDL.h>

#include<glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_access.hpp>

#include<geGL/geGL.h>
#include<geUtil/WindowObject.h>
#include<geUtil/ArgumentObject.h>
#include<geUtil/CameraObject.h>


#include"shaderdir.hpp"
#include<AntTweakBar.h>
#include"lang.h"
#include"langstd.h"
#include"quadlang.h"
#include"boxlang.h"
#include"collision.h"


#endif//_MAIN_HPP_

extern ge::gl::ProgramObject*drawCube;
extern glm::mat4 mvp;

