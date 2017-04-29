#pragma once

#include<GL/gl.h>
#include<GL/glext.h>
#include<geGL/Export.h>

namespace ge{
  namespace gl{
    class GEGL_EXPORT OpenGLFunctionTable{
      public:
#include<geGL/GeneratedOpenGLPFN.h>
#include<geGL/GeneratedOpenGLFunctions.h>
    };
  }
}


