#pragma once

#include<GL/gl.h>
#include<GL/glext.h>

namespace ge{
  namespace gl{
    namespace ibm{
      extern PFNGLFLUSHSTATICDATAIBMPROC           glFlushStaticDataIBM          ;
      extern PFNGLFOGCOORDPOINTERLISTIBMPROC       glFogCoordPointerListIBM      ;
      extern PFNGLVERTEXPOINTERLISTIBMPROC         glVertexPointerListIBM        ;
      extern PFNGLMULTIMODEDRAWELEMENTSIBMPROC     glMultiModeDrawElementsIBM    ;
      extern PFNGLINDEXPOINTERLISTIBMPROC          glIndexPointerListIBM         ;
      extern PFNGLNORMALPOINTERLISTIBMPROC         glNormalPointerListIBM        ;
      extern PFNGLSECONDARYCOLORPOINTERLISTIBMPROC glSecondaryColorPointerListIBM;
      extern PFNGLTEXCOORDPOINTERLISTIBMPROC       glTexCoordPointerListIBM      ;
      extern PFNGLMULTIMODEDRAWARRAYSIBMPROC       glMultiModeDrawArraysIBM      ;
      extern PFNGLEDGEFLAGPOINTERLISTIBMPROC       glEdgeFlagPointerListIBM      ;
      extern PFNGLCOLORPOINTERLISTIBMPROC          glColorPointerListIBM         ;
    }
  }
}
