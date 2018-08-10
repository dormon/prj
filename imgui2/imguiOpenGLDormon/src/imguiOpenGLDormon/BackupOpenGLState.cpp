#include <imguiOpenGLDormon/BackupOpenGLState.h>

namespace imguiOpenGLDormon{

BackupOpenGLState::BackupOpenGLState(OpenGLTable const&gl):gl(gl){
  gl.glGetIntegerv(GL_ACTIVE_TEXTURE, (GLint*)&last_active_texture);
  gl.glActiveTexture(GL_TEXTURE0);
  gl.glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
  gl.glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
  gl.glGetIntegerv(GL_SAMPLER_BINDING, &last_sampler);
  gl.glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
  gl.glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);
  gl.glGetIntegerv(GL_POLYGON_MODE, last_polygon_mode);
  gl.glGetIntegerv(GL_VIEWPORT, last_viewport);
  gl.glGetIntegerv(GL_SCISSOR_BOX, last_scissor_box);
  gl.glGetIntegerv(GL_BLEND_SRC_RGB, (GLint*)&last_blend_src_rgb);
  gl.glGetIntegerv(GL_BLEND_DST_RGB, (GLint*)&last_blend_dst_rgb);
  gl.glGetIntegerv(GL_BLEND_SRC_ALPHA, (GLint*)&last_blend_src_alpha);
  gl.glGetIntegerv(GL_BLEND_DST_ALPHA, (GLint*)&last_blend_dst_alpha);
  gl.glGetIntegerv(GL_BLEND_EQUATION_RGB, (GLint*)&last_blend_equation_rgb);
  gl.glGetIntegerv(GL_BLEND_EQUATION_ALPHA, (GLint*)&last_blend_equation_alpha);
  gl.glIsEnabled(GL_BLEND);
  gl.glIsEnabled(GL_CULL_FACE);
  gl.glIsEnabled(GL_DEPTH_TEST);
  gl.glIsEnabled(GL_SCISSOR_TEST);
}

BackupOpenGLState::~BackupOpenGLState(){
  gl.glUseProgram(last_program);
  gl.glBindTexture(GL_TEXTURE_2D, last_texture);
  if (gl.glBindSampler) gl.glBindSampler(0, last_sampler);
  gl.glActiveTexture(last_active_texture);
  gl.glBindVertexArray(last_vertex_array);
  gl.glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
  gl.glBlendEquationSeparate(last_blend_equation_rgb, last_blend_equation_alpha);
  gl.glBlendFuncSeparate(last_blend_src_rgb, last_blend_dst_rgb,
                      last_blend_src_alpha, last_blend_dst_alpha);
  if (last_enable_blend)
    gl.glEnable(GL_BLEND);
  else
    gl.glDisable(GL_BLEND);
  if (last_enable_cull_face)
    gl.glEnable(GL_CULL_FACE);
  else
    gl.glDisable(GL_CULL_FACE);
  if (last_enable_depth_test)
    gl.glEnable(GL_DEPTH_TEST);
  else
    gl.glDisable(GL_DEPTH_TEST);
  if (last_enable_scissor_test)
    gl.glEnable(GL_SCISSOR_TEST);
  else
    gl.glDisable(GL_SCISSOR_TEST);
  gl.glPolygonMode(GL_FRONT_AND_BACK, (GLenum)last_polygon_mode[0]);
  gl.glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2],
             (GLsizei)last_viewport[3]);
  gl.glScissor(last_scissor_box[0], last_scissor_box[1],
            (GLsizei)last_scissor_box[2], (GLsizei)last_scissor_box[3]);
}

}
