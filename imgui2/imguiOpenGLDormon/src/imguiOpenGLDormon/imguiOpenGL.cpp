#include <imguiOpenGLDormon/imguiOpenGL.h>
#include <imguiOpenGLDormon/GetProcAddress.h>
#include <imguiOpenGLDormon/MinimalOpenGL.h>
#include <imguiOpenGLDormon/BackupOpenGLState.h>
#include <imguiDormon/imgui.h>


IMGUI_IMPL_API bool     ImGui_ImplOpenGL3_Init(const char* glsl_version = "#version 150");
IMGUI_IMPL_API void     ImGui_ImplOpenGL3_NewFrame();

// Called by Init/NewFrame/Shutdown
IMGUI_IMPL_API void     ImGui_ImplOpenGL3_DestroyFontsTexture();
IMGUI_IMPL_API bool     ImGui_ImplOpenGL3_CreateDeviceObjects();
IMGUI_IMPL_API void     ImGui_ImplOpenGL3_DestroyDeviceObjects();

class ImguiOpenGLImpl{
  public:
    ImguiOpenGLImpl(char const*version);
    ~ImguiOpenGLImpl();
    void render(ImDrawData*data);
    OpenGLTable gl;
    // OpenGL Data
    char         g_GlslVersion[32] = "#version 150";
    GLuint       g_FontTexture = 0;
    GLint          g_ShaderHandle = 0, g_VertHandle = 0, g_FragHandle = 0;
    GLint          g_AttribLocationTex = 0, g_AttribLocationProjMtx = 0;
    GLint          g_AttribLocationPosition = 0, g_AttribLocationUV = 0, g_AttribLocationColor = 0;
    GLuint g_VboHandle = 0, g_ElementsHandle = 0;
};

void ImguiOpenGLImpl::render(ImDrawData*draw_data){
  // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
  ImGuiIO& io = ImGui::GetIO();

  int fb_width = (int)(draw_data->DisplaySize.x * io.DisplayFramebufferScale.x);
  int fb_height = (int)(draw_data->DisplaySize.y * io.DisplayFramebufferScale.y);
  if (fb_width <= 0 || fb_height <= 0)
      return;

  draw_data->ScaleClipRects(io.DisplayFramebufferScale);

  BackupOpenGLState states{gl};

  // Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled, polygon fill
  gl.glEnable(GL_BLEND);
  gl.glBlendEquation(GL_FUNC_ADD);
  gl.glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  gl.glDisable(GL_CULL_FACE);
  gl.glDisable(GL_DEPTH_TEST);
  gl.glEnable(GL_SCISSOR_TEST);
  gl.glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  // Setup viewport, orthographic projection matrix
  // Our visible imgui space lies from draw_data->DisplayPps (top left) to draw_data->DisplayPos+data_data->DisplaySize (bottom right). DisplayMin is typically (0,0) for single viewport apps.
  gl.glViewport(0, 0, (GLsizei)fb_width, (GLsizei)fb_height);
  float L = draw_data->DisplayPos.x;
  float R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
  float T = draw_data->DisplayPos.y;
  float B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;
  const float ortho_projection[4][4] =
  {
      { 2.0f/(R-L),   0.0f,         0.0f,   0.0f },
      { 0.0f,         2.0f/(T-B),   0.0f,   0.0f },
      { 0.0f,         0.0f,        -1.0f,   0.0f },
      { (R+L)/(L-R),  (T+B)/(B-T),  0.0f,   1.0f },
  };
  gl.glUseProgram(g_ShaderHandle);
  gl.glUniform1i(g_AttribLocationTex, 0);
  gl.glUniformMatrix4fv(g_AttribLocationProjMtx, 1, GL_FALSE, &ortho_projection[0][0]);
  if (gl.glBindSampler) gl.glBindSampler(0, 0); // We use combined texture/sampler state. Applications using GL 3.3 may set that otherwise.

  // Recreate the VAO every time 
  // (This is to easily allow multiple GL contexts. VAO are not shared among GL contexts, and we don't track creation/deletion of windows so we don't have an obvious key to use to cache them.)
  GLuint vao_handle = 0;
  gl.glGenVertexArrays(1, &vao_handle);
  gl.glBindVertexArray(vao_handle);
  gl.glBindBuffer(GL_ARRAY_BUFFER, g_VboHandle);
  gl.glEnableVertexAttribArray(g_AttribLocationPosition);
  gl.glEnableVertexAttribArray(g_AttribLocationUV);
  gl.glEnableVertexAttribArray(g_AttribLocationColor);
  gl.glVertexAttribPointer(g_AttribLocationPosition, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, pos));
  gl.glVertexAttribPointer(g_AttribLocationUV, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, uv));
  gl.glVertexAttribPointer(g_AttribLocationColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, col));

  // Draw
  ImVec2 pos = draw_data->DisplayPos;
  for (int n = 0; n < draw_data->CmdListsCount; n++)
  {
      const ImDrawList* cmd_list = draw_data->CmdLists[n];
      const ImDrawIdx* idx_buffer_offset = 0;

      gl.glBindBuffer(GL_ARRAY_BUFFER, g_VboHandle);
      gl.glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)cmd_list->VtxBuffer.Size * sizeof(ImDrawVert), (const GLvoid*)cmd_list->VtxBuffer.Data, GL_STREAM_DRAW);

      gl.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ElementsHandle);
      gl.glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx), (const GLvoid*)cmd_list->IdxBuffer.Data, GL_STREAM_DRAW);

      for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
      {
          const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
          if (pcmd->UserCallback)
          {
              // User callback (registered via ImDrawList::AddCallback)
              pcmd->UserCallback(cmd_list, pcmd);
          }
          else
          {
              ImVec4 clip_rect = ImVec4(pcmd->ClipRect.x - pos.x, pcmd->ClipRect.y - pos.y, pcmd->ClipRect.z - pos.x, pcmd->ClipRect.w - pos.y);
              if (clip_rect.x < fb_width && clip_rect.y < fb_height && clip_rect.z >= 0.0f && clip_rect.w >= 0.0f)
              {
                  // Apply scissor/clipping rectangle
                  gl.glScissor((int)clip_rect.x, (int)(fb_height - clip_rect.w), (int)(clip_rect.z - clip_rect.x), (int)(clip_rect.w - clip_rect.y));

                  // Bind texture, Draw
                  gl.glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
                  gl.glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idx_buffer_offset);
              }
          }
          idx_buffer_offset += pcmd->ElemCount;
      }
  }
  gl.glDeleteVertexArrays(1, &vao_handle);


}


ImguiOpenGLImpl::ImguiOpenGLImpl(char const*glsl_version){
  // Store GLSL version string so we can refer to it later in case we recreate shaders. Note: GLSL version is NOT the same as GL version. Leave this to NULL if unsure.
  if (glsl_version == NULL)
      glsl_version = "#version 150";
  IM_ASSERT((int)strlen(glsl_version) + 2 < IM_ARRAYSIZE(g_GlslVersion));
  strcpy(g_GlslVersion, glsl_version);
  strcat(g_GlslVersion, "\n");


  if (!g_FontTexture)
    ImGui_ImplOpenGL3_CreateDeviceObjects();

  // Backup GL state
  GLint last_texture, last_array_buffer, last_vertex_array;
  gl.glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
  gl.glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
  gl.glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);

  // Create shaders
  const GLchar *vertex_shader =
      "uniform mat4 ProjMtx;\n"
      "in vec2 Position;\n"
      "in vec2 UV;\n"
      "in vec4 Color;\n"
      "out vec2 Frag_UV;\n"
      "out vec4 Frag_Color;\n"
      "void main()\n"
      "{\n"
      "	Frag_UV = UV;\n"
      "	Frag_Color = Color;\n"
      "	gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
      "}\n";

  const GLchar* fragment_shader =
      "uniform sampler2D Texture;\n"
      "in vec2 Frag_UV;\n"
      "in vec4 Frag_Color;\n"
      "out vec4 Out_Color;\n"
      "void main()\n"
      "{\n"
      "	Out_Color = Frag_Color * texture( Texture, Frag_UV.st);\n"
      "}\n";

  const GLchar* vertex_shader_with_version[2] = { g_GlslVersion, vertex_shader };
  const GLchar* fragment_shader_with_version[2] = { g_GlslVersion, fragment_shader };

  g_ShaderHandle = gl.glCreateProgram();
  g_VertHandle = gl.glCreateShader(GL_VERTEX_SHADER);
  g_FragHandle = gl.glCreateShader(GL_FRAGMENT_SHADER);
  gl.glShaderSource(g_VertHandle, 2, vertex_shader_with_version, NULL);
  gl.glShaderSource(g_FragHandle, 2, fragment_shader_with_version, NULL);
  gl.glCompileShader(g_VertHandle);
  gl.glCompileShader(g_FragHandle);
  gl.glAttachShader(g_ShaderHandle, g_VertHandle);
  gl.glAttachShader(g_ShaderHandle, g_FragHandle);
  gl.glLinkProgram(g_ShaderHandle);

  g_AttribLocationTex = gl.glGetUniformLocation(g_ShaderHandle, "Texture");
  g_AttribLocationProjMtx = gl.glGetUniformLocation(g_ShaderHandle, "ProjMtx");
  g_AttribLocationPosition = gl.glGetAttribLocation(g_ShaderHandle, "Position");
  g_AttribLocationUV = gl.glGetAttribLocation(g_ShaderHandle, "UV");
  g_AttribLocationColor = gl.glGetAttribLocation(g_ShaderHandle, "Color");

  gl.glGenBuffers(1, &g_VboHandle);
  gl.glGenBuffers(1, &g_ElementsHandle);

  // Build texture atlas
  ImGuiIO& io = ImGui::GetIO();
  unsigned char* pixels;
  int width, height;
  io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);   // Load as RGBA 32-bits (75% of the memory is wasted, but default font is so small) because it is more likely to be compatible with user's existing shaders. If your ImTextureId represent a higher-level concept than just a GL texture id, consider calling GetTexDataAsAlpha8() instead to save on GPU memory.

  // Upload texture to graphics system
  gl.glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
  gl.glGenTextures(1, &g_FontTexture);
  gl.glBindTexture(GL_TEXTURE_2D, g_FontTexture);
  gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  gl.glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  gl.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

  // Store our identifier
  io.Fonts->TexID = (void *)(intptr_t)g_FontTexture;

  // Restore state
  gl.glBindTexture(GL_TEXTURE_2D, last_texture);

  // Restore modified GL state
  gl.glBindTexture(GL_TEXTURE_2D, last_texture);
  gl.glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
  gl.glBindVertexArray(last_vertex_array);

}

ImguiOpenGLImpl::~ImguiOpenGLImpl(){
    if (g_VboHandle) gl.glDeleteBuffers(1, &g_VboHandle);
    if (g_ElementsHandle) gl.glDeleteBuffers(1, &g_ElementsHandle);
    g_VboHandle = g_ElementsHandle = 0;

    if (g_ShaderHandle && g_VertHandle) gl.glDetachShader(g_ShaderHandle, g_VertHandle);
    if (g_VertHandle) gl.glDeleteShader(g_VertHandle);
    g_VertHandle = 0;

    if (g_ShaderHandle && g_FragHandle) gl.glDetachShader(g_ShaderHandle, g_FragHandle);
    if (g_FragHandle) gl.glDeleteShader(g_FragHandle);
    g_FragHandle = 0;

    if (g_ShaderHandle) gl.glDeleteProgram(g_ShaderHandle);
    g_ShaderHandle = 0;

    if (g_FontTexture)
    {
        ImGuiIO& io = ImGui::GetIO();
        gl.glDeleteTextures(1, &g_FontTexture);
        io.Fonts->TexID = 0;
        g_FontTexture = 0;
    }
}

ImguiOpenGL::ImguiOpenGL(char const*glsl_version){
  impl = std::make_unique<ImguiOpenGLImpl>(glsl_version);
}

ImguiOpenGL::~ImguiOpenGL(){}


void ImguiOpenGL::render(ImDrawData*data){
  impl->render(data);
}

