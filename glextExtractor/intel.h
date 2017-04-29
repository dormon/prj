#pragma once

#include<GL/gl.h>
#include<GL/glext.h>

namespace ge{
  namespace gl{
    namespace intel{
      extern PFNGLCREATEPERFQUERYINTELPROC      glCreatePerfQueryINTEL     ;
      extern PFNGLCOLORPOINTERVINTELPROC        glColorPointervINTEL       ;
      extern PFNGLENDPERFQUERYINTELPROC         glEndPerfQueryINTEL        ;
      extern PFNGLBEGINPERFQUERYINTELPROC       glBeginPerfQueryINTEL      ;
      extern PFNGLGETPERFQUERYDATAINTELPROC     glGetPerfQueryDataINTEL    ;
      extern PFNGLVERTEXPOINTERVINTELPROC       glVertexPointervINTEL      ;
      extern PFNGLGETNEXTPERFQUERYIDINTELPROC   glGetNextPerfQueryIdINTEL  ;
      extern PFNGLGETFIRSTPERFQUERYIDINTELPROC  glGetFirstPerfQueryIdINTEL ;
      extern PFNGLMAPTEXTURE2DINTELPROC         glMapTexture2DINTEL        ;
      extern PFNGLTEXCOORDPOINTERVINTELPROC     glTexCoordPointervINTEL    ;
      extern PFNGLUNMAPTEXTURE2DINTELPROC       glUnmapTexture2DINTEL      ;
      extern PFNGLGETPERFCOUNTERINFOINTELPROC   glGetPerfCounterInfoINTEL  ;
      extern PFNGLGETPERFQUERYIDBYNAMEINTELPROC glGetPerfQueryIdByNameINTEL;
      extern PFNGLGETPERFQUERYINFOINTELPROC     glGetPerfQueryInfoINTEL    ;
      extern PFNGLSYNCTEXTUREINTELPROC          glSyncTextureINTEL         ;
      extern PFNGLDELETEPERFQUERYINTELPROC      glDeletePerfQueryINTEL     ;
      extern PFNGLNORMALPOINTERVINTELPROC       glNormalPointervINTEL      ;
    }
  }
}
