#pragma once

#include<GL/gl.h>
#include<GL/glext.h>

namespace ge{
  namespace gl{
    namespace ati{
      extern PFNGLUPDATEOBJECTBUFFERATIPROC           glUpdateObjectBufferATI          ;
      extern PFNGLNORMALSTREAM3BATIPROC               glNormalStream3bATI              ;
      extern PFNGLVERTEXSTREAM3SVATIPROC              glVertexStream3svATI             ;
      extern PFNGLVERTEXSTREAM4FATIPROC               glVertexStream4fATI              ;
      extern PFNGLVERTEXSTREAM3DATIPROC               glVertexStream3dATI              ;
      extern PFNGLCOLORFRAGMENTOP1ATIPROC             glColorFragmentOp1ATI            ;
      extern PFNGLVERTEXSTREAM1IVATIPROC              glVertexStream1ivATI             ;
      extern PFNGLVERTEXBLENDENVIATIPROC              glVertexBlendEnviATI             ;
      extern PFNGLSETFRAGMENTSHADERCONSTANTATIPROC    glSetFragmentShaderConstantATI   ;
      extern PFNGLCLIENTACTIVEVERTEXSTREAMATIPROC     glClientActiveVertexStreamATI    ;
      extern PFNGLGETVERTEXATTRIBARRAYOBJECTFVATIPROC glGetVertexAttribArrayObjectfvATI;
      extern PFNGLVERTEXATTRIBARRAYOBJECTATIPROC      glVertexAttribArrayObjectATI     ;
      extern PFNGLGETVARIANTARRAYOBJECTIVATIPROC      glGetVariantArrayObjectivATI     ;
      extern PFNGLGETTEXBUMPPARAMETERIVATIPROC        glGetTexBumpParameterivATI       ;
      extern PFNGLVERTEXSTREAM1FATIPROC               glVertexStream1fATI              ;
      extern PFNGLDRAWBUFFERSATIPROC                  glDrawBuffersATI                 ;
      extern PFNGLVERTEXSTREAM3DVATIPROC              glVertexStream3dvATI             ;
      extern PFNGLARRAYOBJECTATIPROC                  glArrayObjectATI                 ;
      extern PFNGLVERTEXSTREAM1IATIPROC               glVertexStream1iATI              ;
      extern PFNGLPASSTEXCOORDATIPROC                 glPassTexCoordATI                ;
      extern PFNGLSTENCILOPSEPARATEATIPROC            glStencilOpSeparateATI           ;
      extern PFNGLTEXBUMPPARAMETERFVATIPROC           glTexBumpParameterfvATI          ;
      extern PFNGLVERTEXSTREAM1SVATIPROC              glVertexStream1svATI             ;
      extern PFNGLUNMAPOBJECTBUFFERATIPROC            glUnmapObjectBufferATI           ;
      extern PFNGLFREEOBJECTBUFFERATIPROC             glFreeObjectBufferATI            ;
      extern PFNGLNORMALSTREAM3FATIPROC               glNormalStream3fATI              ;
      extern PFNGLVERTEXBLENDENVFATIPROC              glVertexBlendEnvfATI             ;
      extern PFNGLVERTEXSTREAM4IATIPROC               glVertexStream4iATI              ;
      extern PFNGLBINDFRAGMENTSHADERATIPROC           glBindFragmentShaderATI          ;
      extern PFNGLVERTEXSTREAM3SATIPROC               glVertexStream3sATI              ;
      extern PFNGLNORMALSTREAM3FVATIPROC              glNormalStream3fvATI             ;
      extern PFNGLVERTEXSTREAM4SVATIPROC              glVertexStream4svATI             ;
      extern PFNGLISOBJECTBUFFERATIPROC               glIsObjectBufferATI              ;
      extern PFNGLVERTEXSTREAM3FVATIPROC              glVertexStream3fvATI             ;
      extern PFNGLVERTEXSTREAM1DVATIPROC              glVertexStream1dvATI             ;
      extern PFNGLVERTEXSTREAM2FATIPROC               glVertexStream2fATI              ;
      extern PFNGLNORMALSTREAM3SVATIPROC              glNormalStream3svATI             ;
      extern PFNGLNEWOBJECTBUFFERATIPROC              glNewObjectBufferATI             ;
      extern PFNGLGETTEXBUMPPARAMETERFVATIPROC        glGetTexBumpParameterfvATI       ;
      extern PFNGLVERTEXSTREAM4IVATIPROC              glVertexStream4ivATI             ;
      extern PFNGLVERTEXSTREAM3IVATIPROC              glVertexStream3ivATI             ;
      extern PFNGLMAPOBJECTBUFFERATIPROC              glMapObjectBufferATI             ;
      extern PFNGLCOLORFRAGMENTOP2ATIPROC             glColorFragmentOp2ATI            ;
      extern PFNGLGETARRAYOBJECTIVATIPROC             glGetArrayObjectivATI            ;
      extern PFNGLVERTEXSTREAM4DVATIPROC              glVertexStream4dvATI             ;
      extern PFNGLVERTEXSTREAM1FVATIPROC              glVertexStream1fvATI             ;
      extern PFNGLGENFRAGMENTSHADERSATIPROC           glGenFragmentShadersATI          ;
      extern PFNGLVERTEXSTREAM3FATIPROC               glVertexStream3fATI              ;
      extern PFNGLVERTEXSTREAM1SATIPROC               glVertexStream1sATI              ;
      extern PFNGLNORMALSTREAM3DVATIPROC              glNormalStream3dvATI             ;
      extern PFNGLALPHAFRAGMENTOP1ATIPROC             glAlphaFragmentOp1ATI            ;
      extern PFNGLVERTEXSTREAM2FVATIPROC              glVertexStream2fvATI             ;
      extern PFNGLVERTEXSTREAM3IATIPROC               glVertexStream3iATI              ;
      extern PFNGLSAMPLEMAPATIPROC                    glSampleMapATI                   ;
      extern PFNGLVERTEXSTREAM2SVATIPROC              glVertexStream2svATI             ;
      extern PFNGLNORMALSTREAM3IVATIPROC              glNormalStream3ivATI             ;
      extern PFNGLVERTEXSTREAM2DATIPROC               glVertexStream2dATI              ;
      extern PFNGLTEXBUMPPARAMETERIVATIPROC           glTexBumpParameterivATI          ;
      extern PFNGLGETVARIANTARRAYOBJECTFVATIPROC      glGetVariantArrayObjectfvATI     ;
      extern PFNGLNORMALSTREAM3SATIPROC               glNormalStream3sATI              ;
      extern PFNGLENDFRAGMENTSHADERATIPROC            glEndFragmentShaderATI           ;
      extern PFNGLGETARRAYOBJECTFVATIPROC             glGetArrayObjectfvATI            ;
      extern PFNGLGETOBJECTBUFFERFVATIPROC            glGetObjectBufferfvATI           ;
      extern PFNGLVERTEXSTREAM2IATIPROC               glVertexStream2iATI              ;
      extern PFNGLDRAWRANGEELEMENTARRAYATIPROC        glDrawRangeElementArrayATI       ;
      extern PFNGLALPHAFRAGMENTOP3ATIPROC             glAlphaFragmentOp3ATI            ;
      extern PFNGLSTENCILFUNCSEPARATEATIPROC          glStencilFuncSeparateATI         ;
      extern PFNGLDRAWELEMENTARRAYATIPROC             glDrawElementArrayATI            ;
      extern PFNGLVERTEXSTREAM4DATIPROC               glVertexStream4dATI              ;
      extern PFNGLVERTEXSTREAM2SATIPROC               glVertexStream2sATI              ;
      extern PFNGLGETOBJECTBUFFERIVATIPROC            glGetObjectBufferivATI           ;
      extern PFNGLNORMALSTREAM3IATIPROC               glNormalStream3iATI              ;
      extern PFNGLVARIANTARRAYOBJECTATIPROC           glVariantArrayObjectATI          ;
      extern PFNGLPNTRIANGLESIATIPROC                 glPNTrianglesiATI                ;
      extern PFNGLALPHAFRAGMENTOP2ATIPROC             glAlphaFragmentOp2ATI            ;
      extern PFNGLNORMALSTREAM3BVATIPROC              glNormalStream3bvATI             ;
      extern PFNGLVERTEXSTREAM4SATIPROC               glVertexStream4sATI              ;
      extern PFNGLVERTEXSTREAM4FVATIPROC              glVertexStream4fvATI             ;
      extern PFNGLELEMENTPOINTERATIPROC               glElementPointerATI              ;
      extern PFNGLDELETEFRAGMENTSHADERATIPROC         glDeleteFragmentShaderATI        ;
      extern PFNGLVERTEXSTREAM1DATIPROC               glVertexStream1dATI              ;
      extern PFNGLVERTEXSTREAM2IVATIPROC              glVertexStream2ivATI             ;
      extern PFNGLGETVERTEXATTRIBARRAYOBJECTIVATIPROC glGetVertexAttribArrayObjectivATI;
      extern PFNGLCOLORFRAGMENTOP3ATIPROC             glColorFragmentOp3ATI            ;
      extern PFNGLNORMALSTREAM3DATIPROC               glNormalStream3dATI              ;
      extern PFNGLVERTEXSTREAM2DVATIPROC              glVertexStream2dvATI             ;
      extern PFNGLBEGINFRAGMENTSHADERATIPROC          glBeginFragmentShaderATI         ;
      extern PFNGLPNTRIANGLESFATIPROC                 glPNTrianglesfATI                ;
    }
  }
}
