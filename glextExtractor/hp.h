#pragma once

#include<GL/gl.h>
#include<GL/glext.h>

namespace ge{
  namespace gl{
    namespace hp{
      extern PFNGLIMAGETRANSFORMPARAMETERIVHPPROC    glImageTransformParameterivHP   ;
      extern PFNGLGETIMAGETRANSFORMPARAMETERFVHPPROC glGetImageTransformParameterfvHP;
      extern PFNGLGETIMAGETRANSFORMPARAMETERIVHPPROC glGetImageTransformParameterivHP;
      extern PFNGLIMAGETRANSFORMPARAMETERIHPPROC     glImageTransformParameteriHP    ;
      extern PFNGLIMAGETRANSFORMPARAMETERFVHPPROC    glImageTransformParameterfvHP   ;
      extern PFNGLIMAGETRANSFORMPARAMETERFHPPROC     glImageTransformParameterfHP    ;
    }
  }
}
