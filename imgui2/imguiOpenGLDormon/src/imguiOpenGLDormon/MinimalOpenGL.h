#pragma once

#include<cstddef>

namespace imguiOpenGLDormon{

using GLuint     = unsigned int ;
using GLenum     = unsigned int ;
using GLint      = int          ;
using GLboolean  = unsigned char;
using GLsizei    = int          ;
using GLfloat    = float        ;
using GLvoid     = void         ;
using GLsizeiptr = ptrdiff_t    ;
using GLchar     = char         ;

#define GL_ACTIVE_TEXTURE 0x84E0
#define GL_TEXTURE0 0x84C0
#define GL_CURRENT_PROGRAM 0x8B8D
#define GL_TEXTURE_BINDING_2D 0x8069
#define GL_SAMPLER_BINDING 0x8919
#define GL_ARRAY_BUFFER_BINDING 0x8894
#define GL_VERTEX_ARRAY_BINDING 0x85B5
#define GL_POLYGON_MODE 0x0B40
#define GL_VIEWPORT 0x0BA2
#define GL_SCISSOR_BOX 0x0C10
#define GL_BLEND_SRC_RGB 0x80C9
#define GL_BLEND_DST_RGB 0x80C8
#define GL_BLEND_SRC_ALPHA 0x80CB
#define GL_BLEND_DST_ALPHA 0x80CA
#define GL_BLEND_EQUATION_RGB 0x8009
#define GL_BLEND_EQUATION_ALPHA 0x883D
#define GL_BLEND 0x0BE2
#define GL_CULL_FACE 0x0B44
#define GL_DEPTH_TEST 0x0B71
#define GL_SCISSOR_TEST 0x0C11
#define GL_FUNC_ADD 0x8006
#define GL_SRC_ALPHA 0x0302
#define GL_ONE_MINUS_SRC_ALPHA 0x0303
#define GL_FRONT_AND_BACK 0x0408
#define GL_FILL 0x1B02
#define GL_FALSE 0
#define GL_ARRAY_BUFFER 0x8892
#define GL_FLOAT 0x1406
#define GL_UNSIGNED_BYTE 0x1401
#define GL_TRUE 1
#define GL_STREAM_DRAW 0x88E0
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_TEXTURE_2D 0x0DE1
#define GL_TRIANGLES 0x0004
#define GL_UNSIGNED_INT 0x1405
#define GL_UNSIGNED_SHORT 0x1403
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_LINEAR 0x2601
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_UNPACK_ROW_LENGTH 0x0CF2
#define GL_RGBA 0x1908
#define GL_VERTEX_SHADER 0x8B31
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_COMPILE_STATUS 0x8B81
#define GL_INFO_LOG_LENGTH 0x8B84

using PFNGLGETINTEGERVPROC             = void     (*)(GLenum,GLint*);
using PFNGLACTIVETEXTUREPROC           = void     (*)(GLenum);
using PFNGLISENABLEDPROC               = GLboolean(*)(GLenum);
using PFNGLENABLEPROC                  = void     (*)(GLenum);
using PFNGLBLENDEQUATIONPROC           = void     (*)(GLenum);
using PFNGLBLENDFUNCPROC               = void     (*)(GLenum,GLenum);
using PFNGLDISABLEPROC                 = void     (*)(GLenum);
using PFNGLPOLYGONMODEPROC             = void     (*)(GLenum,GLenum);
using PFNGLVIEWPORTPROC                = void     (*)(GLint,GLint,GLsizei,GLsizei);
using PFNGLUSEPROGRAMPROC              = void     (*)(GLuint);
using PFNGLUNIFORM1IPROC               = void     (*)(GLint,GLint);
using PFNGLUNIFORMMATRIX4FVPROC        = void     (*)(GLint,GLsizei,GLboolean,const GLfloat*);
using PFNGLBINDSAMPLERPROC             = void     (*)(GLuint,GLuint);
using PFNGLGENVERTEXARRAYSPROC         = void     (*)(GLsizei,GLuint*);
using PFNGLBINDVERTEXARRAYPROC         = void     (*)(GLuint);
using PFNGLBINDBUFFERPROC              = void     (*)(GLenum,GLuint);
using PFNGLENABLEVERTEXATTRIBARRAYPROC = void     (*)(GLuint);
using PFNGLVERTEXATTRIBPOINTERPROC     = void     (*)(GLuint,GLint,GLenum,GLboolean,GLsizei,const void*);
using PFNGLBUFFERDATAPROC              = void     (*)(GLenum,GLsizeiptr,const void*,GLenum);
using PFNGLSCISSORPROC                 = void     (*)(GLint,GLint,GLsizei,GLsizei);
using PFNGLBINDTEXTUREPROC             = void     (*)(GLenum,GLuint);
using PFNGLDRAWELEMENTSPROC            = void     (*)(GLenum,GLsizei,GLenum,const GLvoid*);
using PFNGLDELETEVERTEXARRAYSPROC      = void     (*)(GLsizei,const GLuint*);
using PFNGLBLENDEQUATIONSEPARATEPROC   = void     (*)(GLenum,GLenum);
using PFNGLBLENDFUNCSEPARATEPROC       = void     (*)(GLenum,GLenum,GLenum,GLenum);
using PFNGLGENTEXTURESPROC             = void     (*)(GLsizei,GLuint*);
using PFNGLTEXPARAMETERIPROC           = void     (*)(GLenum,GLenum,GLint);
using PFNGLPIXELSTOREIPROC             = void     (*)(GLenum,GLint);
using PFNGLTEXIMAGE2DPROC              = void     (*)(GLenum,GLint,GLint,GLsizei,GLsizei,GLint,GLenum,GLenum,const GLvoid*);
using PFNGLDELETETEXTURESPROC          = void     (*)(GLsizei,const GLuint*);
using PFNGLCREATEPROGRAMPROC           = GLuint   (*)();
using PFNGLCREATESHADERPROC            = GLuint   (*)(GLenum);
using PFNGLSHADERSOURCEPROC            = void     (*)(GLuint,GLsizei,const GLchar*const*,const GLint*);
using PFNGLCOMPILESHADERPROC           = void     (*)(GLuint);
using PFNGLATTACHSHADERPROC            = void     (*)(GLuint,GLuint);
using PFNGLLINKPROGRAMPROC             = void     (*)(GLuint);
using PFNGLGETUNIFORMLOCATIONPROC      = GLint    (*)(GLuint,const GLchar*);
using PFNGLGETATTRIBLOCATIONPROC       = GLint    (*)(GLuint,const GLchar*);
using PFNGLGENBUFFERSPROC              = void     (*)(GLsizei,GLuint*);
using PFNGLDELETEBUFFERSPROC           = void     (*)(GLsizei,const GLuint*);
using PFNGLDETACHSHADERPROC            = void     (*)(GLuint,GLuint);
using PFNGLDELETESHADERPROC            = void     (*)(GLuint);
using PFNGLDELETEPROGRAMPROC           = void     (*)(GLuint);
using PFNGLGETSHADERIVPROC             = void     (*)(GLuint,GLenum,GLint*);
using PFNGLGETSHADERINFOLOGPROC        = void     (*)(GLuint,GLsizei,GLsizei*,GLchar*);

}
