#pragma once

#include<GL/gl.h>
#include<GL/glext.h>

namespace ge{
  namespace gl{
    namespace amd{
      extern PFNGLVERTEXATTRIBPARAMETERIAMDPROC       glVertexAttribParameteriAMD      ;
      extern PFNGLGETPERFMONITORCOUNTERINFOAMDPROC    glGetPerfMonitorCounterInfoAMD   ;
      extern PFNGLDEBUGMESSAGEENABLEAMDPROC           glDebugMessageEnableAMD          ;
      extern PFNGLENDPERFMONITORAMDPROC               glEndPerfMonitorAMD              ;
      extern PFNGLSETMULTISAMPLEFVAMDPROC             glSetMultisamplefvAMD            ;
      extern PFNGLBLENDFUNCINDEXEDAMDPROC             glBlendFuncIndexedAMD            ;
      extern PFNGLDELETENAMESAMDPROC                  glDeleteNamesAMD                 ;
      extern PFNGLBLENDEQUATIONSEPARATEINDEXEDAMDPROC glBlendEquationSeparateIndexedAMD;
      extern PFNGLISNAMEAMDPROC                       glIsNameAMD                      ;
      extern PFNGLSELECTPERFMONITORCOUNTERSAMDPROC    glSelectPerfMonitorCountersAMD   ;
      extern PFNGLGETPERFMONITORCOUNTERDATAAMDPROC    glGetPerfMonitorCounterDataAMD   ;
      extern PFNGLGETPERFMONITORCOUNTERSAMDPROC       glGetPerfMonitorCountersAMD      ;
      extern PFNGLBLENDEQUATIONINDEXEDAMDPROC         glBlendEquationIndexedAMD        ;
      extern PFNGLGETPERFMONITORGROUPSAMDPROC         glGetPerfMonitorGroupsAMD        ;
      extern PFNGLMULTIDRAWELEMENTSINDIRECTAMDPROC    glMultiDrawElementsIndirectAMD   ;
      extern PFNGLDEBUGMESSAGECALLBACKAMDPROC         glDebugMessageCallbackAMD        ;
      extern PFNGLTESSELLATIONFACTORAMDPROC           glTessellationFactorAMD          ;
      extern PFNGLGETDEBUGMESSAGELOGAMDPROC           glGetDebugMessageLogAMD          ;
      extern PFNGLTEXTURESTORAGESPARSEAMDPROC         glTextureStorageSparseAMD        ;
      extern PFNGLBLENDFUNCSEPARATEINDEXEDAMDPROC     glBlendFuncSeparateIndexedAMD    ;
      extern PFNGLTEXSTORAGESPARSEAMDPROC             glTexStorageSparseAMD            ;
      extern PFNGLBEGINPERFMONITORAMDPROC             glBeginPerfMonitorAMD            ;
      extern PFNGLTESSELLATIONMODEAMDPROC             glTessellationModeAMD            ;
      extern PFNGLDEBUGMESSAGEINSERTAMDPROC           glDebugMessageInsertAMD          ;
      extern PFNGLDELETEPERFMONITORSAMDPROC           glDeletePerfMonitorsAMD          ;
      extern PFNGLGENPERFMONITORSAMDPROC              glGenPerfMonitorsAMD             ;
      extern PFNGLGETPERFMONITORGROUPSTRINGAMDPROC    glGetPerfMonitorGroupStringAMD   ;
      extern PFNGLGETPERFMONITORCOUNTERSTRINGAMDPROC  glGetPerfMonitorCounterStringAMD ;
      extern PFNGLSTENCILOPVALUEAMDPROC               glStencilOpValueAMD              ;
      extern PFNGLQUERYOBJECTPARAMETERUIAMDPROC       glQueryObjectParameteruiAMD      ;
      extern PFNGLGENNAMESAMDPROC                     glGenNamesAMD                    ;
      extern PFNGLMULTIDRAWARRAYSINDIRECTAMDPROC      glMultiDrawArraysIndirectAMD     ;
    }
  }
}
