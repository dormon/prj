#pragma once

#include<imguiOpenGLDormon/OpenGLTable.h>

namespace imguiOpenGLDormon{

class BackupOpenGLState{
  public:
    BackupOpenGLState(OpenGLTable const& gl);
    ~BackupOpenGLState();
  private:
    OpenGLTable const& gl;
    GLenum last_active_texture;
    GLint last_program;
    GLint last_texture;
    GLint last_sampler;
    GLint last_array_buffer;
    GLint last_vertex_array;
    GLint last_polygon_mode[2];
    GLint last_viewport[4];
    GLint last_scissor_box[4];
    GLenum last_blend_src_rgb;
    GLenum last_blend_dst_rgb;
    GLenum last_blend_src_alpha;
    GLenum last_blend_dst_alpha;
    GLenum last_blend_equation_rgb;
    GLenum last_blend_equation_alpha;
    GLboolean last_enable_blend;
    GLboolean last_enable_cull_face;
    GLboolean last_enable_depth_test;
    GLboolean last_enable_scissor_test;
};

}
