#pragma once

#include<GL/gl.h>
#include<GL/glext.h>

namespace ge{
  namespace gl{
    namespace sgi{
      extern PFNGLGETSHARPENTEXFUNCSGISPROC          glGetSharpenTexFuncSGIS         ;
      extern PFNGLFLUSHRASTERSGIXPROC                glFlushRasterSGIX               ;
      extern PFNGLTEXTURECOLORMASKSGISPROC           glTextureColorMaskSGIS          ;
      extern PFNGLFRAGMENTMATERIALFVSGIXPROC         glFragmentMaterialfvSGIX        ;
      extern PFNGLDEFORMATIONMAP3DSGIXPROC           glDeformationMap3dSGIX          ;
      extern PFNGLGETDETAILTEXFUNCSGISPROC           glGetDetailTexFuncSGIS          ;
      extern PFNGLSHARPENTEXFUNCSGISPROC             glSharpenTexFuncSGIS            ;
      extern PFNGLLISTPARAMETERFVSGIXPROC            glListParameterfvSGIX           ;
      extern PFNGLGETFOGFUNCSGISPROC                 glGetFogFuncSGIS                ;
      extern PFNGLFRAGMENTLIGHTIVSGIXPROC            glFragmentLightivSGIX           ;
      extern PFNGLSPRITEPARAMETERISGIXPROC           glSpriteParameteriSGIX          ;
      extern PFNGLCOLORTABLEPARAMETERFVSGIPROC       glColorTableParameterfvSGI      ;
      extern PFNGLDEFORMSGIXPROC                     glDeformSGIX                    ;
      extern PFNGLSTOPINSTRUMENTSSGIXPROC            glStopInstrumentsSGIX           ;
      extern PFNGLFINISHASYNCSGIXPROC                glFinishAsyncSGIX               ;
      extern PFNGLFRAGMENTCOLORMATERIALSGIXPROC      glFragmentColorMaterialSGIX     ;
      extern PFNGLTEXIMAGE4DSGISPROC                 glTexImage4DSGIS                ;
      extern PFNGLFRAGMENTLIGHTFSGIXPROC             glFragmentLightfSGIX            ;
      extern PFNGLTEXSUBIMAGE4DSGISPROC              glTexSubImage4DSGIS             ;
      extern PFNGLFRAGMENTLIGHTMODELFSGIXPROC        glFragmentLightModelfSGIX       ;
      extern PFNGLPOLLINSTRUMENTSSGIXPROC            glPollInstrumentsSGIX           ;
      extern PFNGLDELETEASYNCMARKERSSGIXPROC         glDeleteAsyncMarkersSGIX        ;
      extern PFNGLSAMPLEMASKSGISPROC                 glSampleMaskSGIS                ;
      extern PFNGLFOGFUNCSGISPROC                    glFogFuncSGIS                   ;
      extern PFNGLREADINSTRUMENTSSGIXPROC            glReadInstrumentsSGIX           ;
      extern PFNGLPIXELTEXGENPARAMETERFSGISPROC      glPixelTexGenParameterfSGIS     ;
      extern PFNGLPIXELTEXGENPARAMETERISGISPROC      glPixelTexGenParameteriSGIS     ;
      extern PFNGLLOADIDENTITYDEFORMATIONMAPSGIXPROC glLoadIdentityDeformationMapSGIX;
      extern PFNGLSPRITEPARAMETERFVSGIXPROC          glSpriteParameterfvSGIX         ;
      extern PFNGLDETAILTEXFUNCSGISPROC              glDetailTexFuncSGIS             ;
      extern PFNGLLISTPARAMETERISGIXPROC             glListParameteriSGIX            ;
      extern PFNGLGETLISTPARAMETERFVSGIXPROC         glGetListParameterfvSGIX        ;
      extern PFNGLLIGHTENVISGIXPROC                  glLightEnviSGIX                 ;
      extern PFNGLGETCOLORTABLEPARAMETERIVSGIPROC    glGetColorTableParameterivSGI   ;
      extern PFNGLINSTRUMENTSBUFFERSGIXPROC          glInstrumentsBufferSGIX         ;
      extern PFNGLLISTPARAMETERIVSGIXPROC            glListParameterivSGIX           ;
      extern PFNGLASYNCMARKERSGIXPROC                glAsyncMarkerSGIX               ;
      extern PFNGLDEFORMATIONMAP3FSGIXPROC           glDeformationMap3fSGIX          ;
      extern PFNGLPOINTPARAMETERFVSGISPROC           glPointParameterfvSGIS          ;
      extern PFNGLGENASYNCMARKERSSGIXPROC            glGenAsyncMarkersSGIX           ;
      extern PFNGLIGLOOINTERFACESGIXPROC             glIglooInterfaceSGIX            ;
      extern PFNGLFRAGMENTMATERIALFSGIXPROC          glFragmentMaterialfSGIX         ;
      extern PFNGLGETLISTPARAMETERIVSGIXPROC         glGetListParameterivSGIX        ;
      extern PFNGLTEXFILTERFUNCSGISPROC              glTexFilterFuncSGIS             ;
      extern PFNGLCOLORTABLEPARAMETERIVSGIPROC       glColorTableParameterivSGI      ;
      extern PFNGLPIXELTEXGENSGIXPROC                glPixelTexGenSGIX               ;
      extern PFNGLSPRITEPARAMETERFSGIXPROC           glSpriteParameterfSGIX          ;
      extern PFNGLFRAGMENTMATERIALISGIXPROC          glFragmentMaterialiSGIX         ;
      extern PFNGLGETFRAGMENTMATERIALIVSGIXPROC      glGetFragmentMaterialivSGIX     ;
      extern PFNGLPIXELTEXGENPARAMETERFVSGISPROC     glPixelTexGenParameterfvSGIS    ;
      extern PFNGLFRAMEZOOMSGIXPROC                  glFrameZoomSGIX                 ;
      extern PFNGLFRAGMENTLIGHTMODELISGIXPROC        glFragmentLightModeliSGIX       ;
      extern PFNGLFRAGMENTLIGHTISGIXPROC             glFragmentLightiSGIX            ;
      extern PFNGLTAGSAMPLEBUFFERSGIXPROC            glTagSampleBufferSGIX           ;
      extern PFNGLISASYNCMARKERSGIXPROC              glIsAsyncMarkerSGIX             ;
      extern PFNGLSPRITEPARAMETERIVSGIXPROC          glSpriteParameterivSGIX         ;
      extern PFNGLSAMPLEPATTERNSGISPROC              glSamplePatternSGIS             ;
      extern PFNGLLISTPARAMETERFSGIXPROC             glListParameterfSGIX            ;
      extern PFNGLCOPYCOLORTABLESGIPROC              glCopyColorTableSGI             ;
      extern PFNGLREFERENCEPLANESGIXPROC             glReferencePlaneSGIX            ;
      extern PFNGLGETFRAGMENTLIGHTIVSGIXPROC         glGetFragmentLightivSGIX        ;
      extern PFNGLSTARTINSTRUMENTSSGIXPROC           glStartInstrumentsSGIX          ;
      extern PFNGLFRAGMENTLIGHTFVSGIXPROC            glFragmentLightfvSGIX           ;
      extern PFNGLGETPIXELTEXGENPARAMETERIVSGISPROC  glGetPixelTexGenParameterivSGIS ;
      extern PFNGLPOLLASYNCSGIXPROC                  glPollAsyncSGIX                 ;
      extern PFNGLGETINSTRUMENTSSGIXPROC             glGetInstrumentsSGIX            ;
      extern PFNGLFRAGMENTLIGHTMODELFVSGIXPROC       glFragmentLightModelfvSGIX      ;
      extern PFNGLFRAGMENTMATERIALIVSGIXPROC         glFragmentMaterialivSGIX        ;
      extern PFNGLGETTEXFILTERFUNCSGISPROC           glGetTexFilterFuncSGIS          ;
      extern PFNGLCOLORTABLESGIPROC                  glColorTableSGI                 ;
      extern PFNGLGETFRAGMENTLIGHTFVSGIXPROC         glGetFragmentLightfvSGIX        ;
      extern PFNGLGETPIXELTEXGENPARAMETERFVSGISPROC  glGetPixelTexGenParameterfvSGIS ;
      extern PFNGLFRAGMENTLIGHTMODELIVSGIXPROC       glFragmentLightModelivSGIX      ;
      extern PFNGLGETFRAGMENTMATERIALFVSGIXPROC      glGetFragmentMaterialfvSGIX     ;
      extern PFNGLGETCOLORTABLESGIPROC               glGetColorTableSGI              ;
      extern PFNGLPIXELTEXGENPARAMETERIVSGISPROC     glPixelTexGenParameterivSGIS    ;
      extern PFNGLGETCOLORTABLEPARAMETERFVSGIPROC    glGetColorTableParameterfvSGI   ;
      extern PFNGLPOINTPARAMETERFSGISPROC            glPointParameterfSGIS           ;
    }
  }
}
