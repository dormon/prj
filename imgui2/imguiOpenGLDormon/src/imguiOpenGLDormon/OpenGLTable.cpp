#include <imguiOpenGLDormon/OpenGLTable.h>
#include <imguiOpenGLDormon/GetProcAddress.h>

namespace imguiOpenGLDormon{

OpenGLTable::OpenGLTable(){
  glGetIntegerv             = (PFNGLGETINTEGERVPROC            )getProcAddress("glGetIntegerv"            );
  glActiveTexture           = (PFNGLACTIVETEXTUREPROC          )getProcAddress("glActiveTexture"          );
  glIsEnabled               = (PFNGLISENABLEDPROC              )getProcAddress("glIsEnabled"              );
  glEnable                  = (PFNGLENABLEPROC                 )getProcAddress("glEnable"                 );
  glBlendEquation           = (PFNGLBLENDEQUATIONPROC          )getProcAddress("glBlendEquation"          );
  glBlendFunc               = (PFNGLBLENDFUNCPROC              )getProcAddress("glBlendFunc"              );
  glDisable                 = (PFNGLDISABLEPROC                )getProcAddress("glDisable"                );
  glPolygonMode             = (PFNGLPOLYGONMODEPROC            )getProcAddress("glPolygonMode"            );
  glViewport                = (PFNGLVIEWPORTPROC               )getProcAddress("glViewport"               );
  glUseProgram              = (PFNGLUSEPROGRAMPROC             )getProcAddress("glUseProgram"             );
  glUniform1i               = (PFNGLUNIFORM1IPROC              )getProcAddress("glUniform1i"              );
  glUniformMatrix4fv        = (PFNGLUNIFORMMATRIX4FVPROC       )getProcAddress("glUniformMatrix4fv"       );
  glBindSampler             = (PFNGLBINDSAMPLERPROC            )getProcAddress("glBindSampler"            );
  glGenVertexArrays         = (PFNGLGENVERTEXARRAYSPROC        )getProcAddress("glGenVertexArrays"        );
  glBindVertexArray         = (PFNGLBINDVERTEXARRAYPROC        )getProcAddress("glBindVertexArray"        );
  glBindBuffer              = (PFNGLBINDBUFFERPROC             )getProcAddress("glBindBuffer"             );
  glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)getProcAddress("glEnableVertexAttribArray");
  glVertexAttribPointer     = (PFNGLVERTEXATTRIBPOINTERPROC    )getProcAddress("glVertexAttribPointer"    );
  glBufferData              = (PFNGLBUFFERDATAPROC             )getProcAddress("glBufferData"             );
  glScissor                 = (PFNGLSCISSORPROC                )getProcAddress("glScissor"                );
  glBindTexture             = (PFNGLBINDTEXTUREPROC            )getProcAddress("glBindTexture"            );
  glDrawElements            = (PFNGLDRAWELEMENTSPROC           )getProcAddress("glDrawElements"           );
  glDeleteVertexArrays      = (PFNGLDELETEVERTEXARRAYSPROC     )getProcAddress("glDeleteVertexArrays"     );
  glBlendEquationSeparate   = (PFNGLBLENDEQUATIONSEPARATEPROC  )getProcAddress("glBlendEquationSeparate"  );
  glBlendFuncSeparate       = (PFNGLBLENDFUNCSEPARATEPROC      )getProcAddress("glBlendFuncSeparate"      );
  glGenTextures             = (PFNGLGENTEXTURESPROC            )getProcAddress("glGenTextures"            );
  glTexParameteri           = (PFNGLTEXPARAMETERIPROC          )getProcAddress("glTexParameteri"          );
  glPixelStorei             = (PFNGLPIXELSTOREIPROC            )getProcAddress("glPixelStorei"            );
  glTexImage2D              = (PFNGLTEXIMAGE2DPROC             )getProcAddress("glTexImage2D"             );
  glDeleteTextures          = (PFNGLDELETETEXTURESPROC         )getProcAddress("glDeleteTextures"         );
  glCreateProgram           = (PFNGLCREATEPROGRAMPROC          )getProcAddress("glCreateProgram"          );
  glCreateShader            = (PFNGLCREATESHADERPROC           )getProcAddress("glCreateShader"           );
  glShaderSource            = (PFNGLSHADERSOURCEPROC           )getProcAddress("glShaderSource"           );
  glCompileShader           = (PFNGLCOMPILESHADERPROC          )getProcAddress("glCompileShader"          );
  glAttachShader            = (PFNGLATTACHSHADERPROC           )getProcAddress("glAttachShader"           );
  glLinkProgram             = (PFNGLLINKPROGRAMPROC            )getProcAddress("glLinkProgram"            );
  glGetUniformLocation      = (PFNGLGETUNIFORMLOCATIONPROC     )getProcAddress("glGetUniformLocation"     );
  glGetAttribLocation       = (PFNGLGETATTRIBLOCATIONPROC      )getProcAddress("glGetAttribLocation"      );
  glGenBuffers              = (PFNGLGENBUFFERSPROC             )getProcAddress("glGenBuffers"             );
  glDeleteBuffers           = (PFNGLDELETEBUFFERSPROC          )getProcAddress("glDeleteBuffers"          );
  glDetachShader            = (PFNGLDETACHSHADERPROC           )getProcAddress("glDetachShader"           );
  glDeleteShader            = (PFNGLDELETESHADERPROC           )getProcAddress("glDeleteShader"           );
  glDeleteProgram           = (PFNGLDELETEPROGRAMPROC          )getProcAddress("glDeleteProgram"          );
  glGetShaderiv             = (PFNGLGETSHADERIVPROC            )getProcAddress("glGetShaderiv"            );
  glGetShaderInfoLog        = (PFNGLGETSHADERINFOLOGPROC       )getProcAddress("glGetShaderInfoLog"       );

}

}
