#pragma once

#include<GL/gl.h>
#include<GL/glext.h>

namespace ge{
  namespace gl{
    namespace apple{
      extern PFNGLFINISHFENCEAPPLEPROC                glFinishFenceAPPLE               ;
      extern PFNGLELEMENTPOINTERAPPLEPROC             glElementPointerAPPLE            ;
      extern PFNGLDELETEFENCESAPPLEPROC               glDeleteFencesAPPLE              ;
      extern PFNGLTESTFENCEAPPLEPROC                  glTestFenceAPPLE                 ;
      extern PFNGLTESTOBJECTAPPLEPROC                 glTestObjectAPPLE                ;
      extern PFNGLOBJECTUNPURGEABLEAPPLEPROC          glObjectUnpurgeableAPPLE         ;
      extern PFNGLFLUSHMAPPEDBUFFERRANGEAPPLEPROC     glFlushMappedBufferRangeAPPLE    ;
      extern PFNGLBINDVERTEXARRAYAPPLEPROC            glBindVertexArrayAPPLE           ;
      extern PFNGLTEXTURERANGEAPPLEPROC               glTextureRangeAPPLE              ;
      extern PFNGLDRAWRANGEELEMENTARRAYAPPLEPROC      glDrawRangeElementArrayAPPLE     ;
      extern PFNGLMULTIDRAWRANGEELEMENTARRAYAPPLEPROC glMultiDrawRangeElementArrayAPPLE;
      extern PFNGLGETTEXPARAMETERPOINTERVAPPLEPROC    glGetTexParameterPointervAPPLE   ;
      extern PFNGLMAPVERTEXATTRIB1FAPPLEPROC          glMapVertexAttrib1fAPPLE         ;
      extern PFNGLDELETEVERTEXARRAYSAPPLEPROC         glDeleteVertexArraysAPPLE        ;
      extern PFNGLSETFENCEAPPLEPROC                   glSetFenceAPPLE                  ;
      extern PFNGLMAPVERTEXATTRIB2FAPPLEPROC          glMapVertexAttrib2fAPPLE         ;
      extern PFNGLDRAWELEMENTARRAYAPPLEPROC           glDrawElementArrayAPPLE          ;
      extern PFNGLGENFENCESAPPLEPROC                  glGenFencesAPPLE                 ;
      extern PFNGLISVERTEXARRAYAPPLEPROC              glIsVertexArrayAPPLE             ;
      extern PFNGLVERTEXARRAYRANGEAPPLEPROC           glVertexArrayRangeAPPLE          ;
      extern PFNGLGETOBJECTPARAMETERIVAPPLEPROC       glGetObjectParameterivAPPLE      ;
      extern PFNGLMULTIDRAWELEMENTARRAYAPPLEPROC      glMultiDrawElementArrayAPPLE     ;
      extern PFNGLDISABLEVERTEXATTRIBAPPLEPROC        glDisableVertexAttribAPPLE       ;
      extern PFNGLGENVERTEXARRAYSAPPLEPROC            glGenVertexArraysAPPLE           ;
      extern PFNGLISVERTEXATTRIBENABLEDAPPLEPROC      glIsVertexAttribEnabledAPPLE     ;
      extern PFNGLISFENCEAPPLEPROC                    glIsFenceAPPLE                   ;
      extern PFNGLMAPVERTEXATTRIB1DAPPLEPROC          glMapVertexAttrib1dAPPLE         ;
      extern PFNGLBUFFERPARAMETERIAPPLEPROC           glBufferParameteriAPPLE          ;
      extern PFNGLENABLEVERTEXATTRIBAPPLEPROC         glEnableVertexAttribAPPLE        ;
      extern PFNGLMAPVERTEXATTRIB2DAPPLEPROC          glMapVertexAttrib2dAPPLE         ;
      extern PFNGLOBJECTPURGEABLEAPPLEPROC            glObjectPurgeableAPPLE           ;
      extern PFNGLFLUSHVERTEXARRAYRANGEAPPLEPROC      glFlushVertexArrayRangeAPPLE     ;
      extern PFNGLVERTEXARRAYPARAMETERIAPPLEPROC      glVertexArrayParameteriAPPLE     ;
      extern PFNGLFINISHOBJECTAPPLEPROC               glFinishObjectAPPLE              ;
    }
  }
}
