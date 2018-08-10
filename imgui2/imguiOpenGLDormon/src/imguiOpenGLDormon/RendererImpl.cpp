#include <imguiOpenGLDormon/RendererImpl.h>
#include <sstream>

namespace imguiOpenGLDormon{

void setRenderState(OpenGLTable const&gl,int width,int height){
  gl.glEnable(GL_BLEND);
  gl.glBlendEquation(GL_FUNC_ADD);
  gl.glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  gl.glDisable(GL_CULL_FACE);
  gl.glDisable(GL_DEPTH_TEST);
  gl.glEnable(GL_SCISSOR_TEST);
  gl.glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  gl.glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}

GLuint RendererImpl::createVAO(){
  GLuint vao;
  gl.glGenVertexArrays(1, &vao);
  gl.glBindVertexArray(vao);
  gl.glBindBuffer(GL_ARRAY_BUFFER, vbo);
  gl.glEnableVertexAttribArray(positionAttribute);
  gl.glEnableVertexAttribArray(coordAttribute   );
  gl.glEnableVertexAttribArray(colorAttribute   );
  gl.glVertexAttribPointer(positionAttribute,2,GL_FLOAT        ,GL_FALSE,sizeof(ImDrawVert),(GLvoid*)IM_OFFSETOF(ImDrawVert,pos));
  gl.glVertexAttribPointer(coordAttribute   ,2,GL_FLOAT        ,GL_FALSE,sizeof(ImDrawVert),(GLvoid*)IM_OFFSETOF(ImDrawVert,uv ));
  gl.glVertexAttribPointer(colorAttribute   ,4,GL_UNSIGNED_BYTE,GL_TRUE ,sizeof(ImDrawVert),(GLvoid*)IM_OFFSETOF(ImDrawVert,col));
  return vao;
}

void RendererImpl::setMatrixUniform(ImDrawData const*data){
  auto const& pos  = data->DisplayPos;
  auto const& size = data->DisplaySize;
  float L = pos.x         ;
  float R = pos.x + size.x;
  float T = pos.y         ;
  float B = pos.y + size.y;
  const float ortho[] = {
    2.f/(R-L)  ,0.f        , 0.f,0.f,
    0.f        ,2.f/(T-B)  , 0.f,0.f,
    0.f        ,0.f        ,-1.f,0.f,
    (R+L)/(L-R),(T+B)/(B-T), 0.f,1.f,
  };
  gl.glUniformMatrix4fv(matrixUniform, 1, GL_FALSE, ortho);
}

void uploadBuffer(OpenGLTable const&gl,GLuint buffer,GLenum binding,size_t size,void*data){
  gl.glBindBuffer(binding,buffer);
  gl.glBufferData(binding,size,data,GL_STREAM_DRAW);
}

void RendererImpl::uploadBuffers(ImDrawList const*cmd_list){
  uploadBuffer(gl,vbo,GL_ARRAY_BUFFER        ,cmd_list->VtxBuffer.Size * sizeof(ImDrawVert),cmd_list->VtxBuffer.Data);
  uploadBuffer(gl,ebo,GL_ELEMENT_ARRAY_BUFFER,cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx ),cmd_list->IdxBuffer.Data);
}

bool isClipRectangleNotVisible(ImVec4 rect,int width,int height){ 
  return !(rect.x < width && rect.y < height && rect.z >= 0.0f && rect.w >= 0.0f);
}

GLenum getElementType(){
  return sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;
}

void RendererImpl::drawDefaultCommand(ImDrawList const* cmd_list,ImDrawCmd const*pcmd,ImVec2 pos,int width,int height,ImDrawIdx const*idx_buffer_offset){
  auto const&clipRect = pcmd->ClipRect;
  ImVec4 rect = ImVec4(clipRect.x - pos.x, clipRect.y - pos.y, clipRect.z - pos.x, clipRect.w - pos.y);
  if (isClipRectangleNotVisible(rect,width,height))return;
  gl.glScissor((int)rect.x, (int)(height - rect.w), (int)(rect.z - rect.x), (int)(rect.w - rect.y));

  gl.glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
  gl.glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, getElementType(), idx_buffer_offset);
}

void RendererImpl::drawCommand(ImDrawList const* cmd_list,ImDrawCmd const*pcmd,ImVec2 pos,int fb_width,int fb_height,ImDrawIdx const*idx_buffer_offset){
  if (pcmd->UserCallback)
    pcmd->UserCallback(cmd_list, pcmd);
  else
    drawDefaultCommand(cmd_list,pcmd,pos,fb_width,fb_height,idx_buffer_offset);
}

void RendererImpl::drawCommandList(ImDrawList const* cmd_list,ImVec2 pos,int fb_width,int fb_height){
  const ImDrawIdx* idx_buffer_offset = 0;
  uploadBuffers(cmd_list);
  for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++){
    auto const pcmd = &cmd_list->CmdBuffer[cmd_i];
    drawCommand(cmd_list,pcmd,pos,fb_width,fb_height,idx_buffer_offset);
    idx_buffer_offset += pcmd->ElemCount;
  }
}

int getFramebufferWidth(ImDrawData const*data,ImGuiIO const&io){
  return (int)(data->DisplaySize.x * io.DisplayFramebufferScale.x);
}

int getFramebufferHeight(ImDrawData const*data,ImGuiIO const&io){
  return (int)(data->DisplaySize.y * io.DisplayFramebufferScale.y);
}

bool isViewportNotVisible(ImDrawData const*data,ImGuiIO const&io){
  int fb_width  = getFramebufferWidth (data,io);
  int fb_height = getFramebufferHeight(data,io);
  return fb_width <= 0 || fb_height <= 0;
}

void RendererImpl::setupProgram(ImDrawData const*draw_data){
  gl.glUseProgram(program);
  gl.glUniform1i(textureUniform, 0);
  setMatrixUniform(draw_data);
  if (gl.glBindSampler) gl.glBindSampler(0, 0);
}

void RendererImpl::render(ImDrawData*draw_data){
  ImGuiIO& io = ImGui::GetIO();

  if(isViewportNotVisible(draw_data,io))return;

  draw_data->ScaleClipRects(io.DisplayFramebufferScale);

  BackupOpenGLState states{gl};

  int fb_width  = getFramebufferWidth (draw_data,io);
  int fb_height = getFramebufferHeight(draw_data,io);
  setRenderState(gl,fb_width,fb_height);
  
  setupProgram(draw_data);

  GLuint vao = createVAO();

  // Draw
  ImVec2 pos = draw_data->DisplayPos;
  for (int n = 0; n < draw_data->CmdListsCount; n++)
    drawCommandList(draw_data->CmdLists[n],pos,fb_width,fb_height);

  gl.glDeleteVertexArrays(1, &vao);
}

GLint getShaderInfoLogLength(OpenGLTable const&gl,GLuint id){
  GLint length;
  gl.glGetShaderiv(id,GL_INFO_LOG_LENGTH,&length);
  return length;
}

std::string getShaderInfoLog(OpenGLTable const&gl,GLuint id){
  auto length = getShaderInfoLogLength(gl,id);
  if(!length)return"";
  std::string info(length,' ');
  gl.glGetShaderInfoLog(id,length,NULL,(GLchar*)info.c_str());
  return info;
}

bool didShaderCompile(OpenGLTable const&gl,GLuint id){
  GLint param;
  gl.glGetShaderiv(id,GL_COMPILE_STATUS,&param);
  return static_cast<bool>(param);
}

void checkCompileStatus(OpenGLTable const&gl,GLuint id){
  if(didShaderCompile(gl,id))return;
  throw std::runtime_error(getShaderInfoLog(gl,id));
}

GLuint createShader(OpenGLTable const&gl,GLenum type,std::string const&src){
  auto id = gl.glCreateShader(type);
  const GLchar* srcs[] = {src.c_str()};
  gl.glShaderSource (id, 1, srcs, nullptr);
  gl.glCompileShader(id);
  checkCompileStatus(gl,id);
  return id;
}

std::string getVertexShaderSource(size_t version){
  std::stringstream src;
  src << "#version " << version << std::endl;
  src << R".(
  uniform mat4 projection;
  in      vec2 position  ;
  in      vec2 coord     ;
  in      vec4 color     ;
  out     vec2 vCoord    ;
  out     vec4 vColor    ;

  void main(){
  	vCoord      = coord;
  	vColor      = color;
  	gl_Position = projection * vec4(position,0,1);
  }
  ).";
  return src.str();
}

std::string getFragmentShaderSource(size_t version){
  std::stringstream src;
  src << "#version " << version << std::endl;
  src << R".(
  uniform sampler2D tex   ;
  in      vec2      vCoord;
  in      vec4      vColor;
  out     vec4      fColor;

  void main(){
  	fColor = vColor * texture(tex,vCoord);
  }
  ).";
  return src.str();
}

GLuint createProgram(OpenGLTable const&gl,size_t version){
  auto vertexShaderSource   = getVertexShaderSource  (version);
  auto fragmentShaderSource = getFragmentShaderSource(version);

  auto program    = gl.glCreateProgram();
  auto vertexID   = createShader(gl,GL_VERTEX_SHADER  ,vertexShaderSource  );
  auto fragmentID = createShader(gl,GL_FRAGMENT_SHADER,fragmentShaderSource);

  gl.glAttachShader(program,vertexID  );
  gl.glAttachShader(program,fragmentID);
  gl.glLinkProgram (program);

  gl.glDeleteShader(vertexID  );
  gl.glDeleteShader(fragmentID);

  return program;
}

GLuint createFontTexture(OpenGLTable const&gl,int width,int height,unsigned char*pixels){
  GLuint id;
  gl.glGenTextures(1, &id);
  gl.glBindTexture(GL_TEXTURE_2D, id);
  gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  gl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  gl.glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  gl.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
  return id;
}

void RendererImpl::initFontTexture(){
  ImGuiIO& io = ImGui::GetIO();
  unsigned char* pixels;
  int width, height;
  io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

  fontTexture = createFontTexture(gl,width,height,pixels);

  io.Fonts->TexID = (void *)(intptr_t)fontTexture;
}

void RendererImpl::initProgram(size_t version){
  program = createProgram(gl,version);

  textureUniform = gl.glGetUniformLocation(program,"tex"       );
  matrixUniform  = gl.glGetUniformLocation(program,"projection");

  positionAttribute = gl.glGetAttribLocation(program,"position");
  coordAttribute    = gl.glGetAttribLocation(program,"coord"   );
  colorAttribute    = gl.glGetAttribLocation(program,"color"   );
}

void RendererImpl::initBuffers(){
  gl.glGenBuffers(1, &vbo);
  gl.glGenBuffers(1, &ebo);
}

RendererImpl::RendererImpl(size_t glsl_version){
  BackupOpenGLState states{gl};
  initProgram(glsl_version);
  initBuffers();
  initFontTexture();
}

RendererImpl::~RendererImpl(){
  if (vbo) gl.glDeleteBuffers(1, &vbo);
  if (ebo) gl.glDeleteBuffers(1, &ebo);
  vbo = ebo = 0;

  if (program) gl.glDeleteProgram(program);
  program = 0;

  if (fontTexture){
    ImGuiIO& io = ImGui::GetIO();
    gl.glDeleteTextures(1, &fontTexture);
    io.Fonts->TexID = 0;
    fontTexture = 0;
  }
}

}
