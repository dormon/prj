#pragma once

#include <iostream>

#include <imguiOpenGLDormon/GetProcAddress.h>
#include <imguiOpenGLDormon/BackupOpenGLState.h>
#include <imguiOpenGLDormon/MinimalOpenGL.h>
#include <imguiDormon/imgui.h>

namespace imguiOpenGLDormon{

class RendererImpl{
  public:
    RendererImpl(size_t version);
    ~RendererImpl();
    void render(ImDrawData*data);
    GLuint createShader(GLenum type,std::string const&src)const;
    GLuint createVAO();
    void initFontTexture();
    void initBuffers();
    void initProgram(size_t glsl_version);
    void setMatrixUniform(ImDrawData const*data);
    void setupProgram(ImDrawData const*data);
    void uploadBuffers(ImDrawList const*cmds);
    void drawDefaultCommand(ImDrawList const* cmds,ImDrawCmd const*pcmd,ImVec2 pos,int width,int height,ImDrawIdx const*idx_buffer_offset);
    void drawCommand(ImDrawList const* cmds,ImDrawCmd const*pcmd,ImVec2 pos,int width,int height,ImDrawIdx const*idx_buffer_offset);
    void drawCommandList(ImDrawList const* cmd_list,ImVec2 pos,int width,int height);
    OpenGLTable gl;
    GLuint fontTexture = 0;
    GLint  program = 0;
    GLint  textureUniform = 0, matrixUniform = 0;
    GLint  positionAttribute = 0, coordAttribute = 0, colorAttribute = 0;
    GLuint vbo = 0, ebo = 0;
};

}
