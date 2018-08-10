#pragma once

#include<imguiOpenGLDormon/MinimalOpenGL.h>

namespace imguiOpenGLDormon{

class OpenGLTable{
  public:
    OpenGLTable();
    PFNGLGETINTEGERVPROC             glGetIntegerv;
    PFNGLACTIVETEXTUREPROC           glActiveTexture;
    PFNGLISENABLEDPROC               glIsEnabled;
    PFNGLENABLEPROC                  glEnable;
    PFNGLBLENDEQUATIONPROC           glBlendEquation;
    PFNGLBLENDFUNCPROC               glBlendFunc;
    PFNGLDISABLEPROC                 glDisable;
    PFNGLPOLYGONMODEPROC             glPolygonMode;
    PFNGLVIEWPORTPROC                glViewport;
    PFNGLUSEPROGRAMPROC              glUseProgram;
    PFNGLUNIFORM1IPROC               glUniform1i;
    PFNGLUNIFORMMATRIX4FVPROC        glUniformMatrix4fv;
    PFNGLBINDSAMPLERPROC             glBindSampler;
    PFNGLGENVERTEXARRAYSPROC         glGenVertexArrays;
    PFNGLBINDVERTEXARRAYPROC         glBindVertexArray;
    PFNGLBINDBUFFERPROC              glBindBuffer;
    PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
    PFNGLVERTEXATTRIBPOINTERPROC     glVertexAttribPointer;
    PFNGLBUFFERDATAPROC              glBufferData;
    PFNGLSCISSORPROC                 glScissor;
    PFNGLBINDTEXTUREPROC             glBindTexture;
    PFNGLDRAWELEMENTSPROC            glDrawElements;
    PFNGLDELETEVERTEXARRAYSPROC      glDeleteVertexArrays;
    PFNGLBLENDEQUATIONSEPARATEPROC   glBlendEquationSeparate;
    PFNGLBLENDFUNCSEPARATEPROC       glBlendFuncSeparate;
    PFNGLGENTEXTURESPROC             glGenTextures;
    PFNGLTEXPARAMETERIPROC           glTexParameteri;
    PFNGLPIXELSTOREIPROC             glPixelStorei;
    PFNGLTEXIMAGE2DPROC              glTexImage2D;
    PFNGLDELETETEXTURESPROC          glDeleteTextures;
    PFNGLCREATEPROGRAMPROC           glCreateProgram;
    PFNGLCREATESHADERPROC            glCreateShader;
    PFNGLSHADERSOURCEPROC            glShaderSource;
    PFNGLCOMPILESHADERPROC           glCompileShader;
    PFNGLATTACHSHADERPROC            glAttachShader;
    PFNGLLINKPROGRAMPROC             glLinkProgram;
    PFNGLGETUNIFORMLOCATIONPROC      glGetUniformLocation;
    PFNGLGETATTRIBLOCATIONPROC       glGetAttribLocation;
    PFNGLGENBUFFERSPROC              glGenBuffers;
    PFNGLDELETEBUFFERSPROC           glDeleteBuffers;
    PFNGLDETACHSHADERPROC            glDetachShader;
    PFNGLDELETESHADERPROC            glDeleteShader;
    PFNGLDELETEPROGRAMPROC           glDeleteProgram;
    PFNGLGETSHADERIVPROC             glGetShaderiv;
    PFNGLGETSHADERINFOLOGPROC        glGetShaderInfoLog;

};

}
