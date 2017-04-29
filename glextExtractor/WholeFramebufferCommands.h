#pragma once

#include<geCore/Command.h>
#include<geGL/Export.h>
#include<GL/glew.h>
#include<geGL/AllAttribs.h>

namespace ge{
  namespace gl{
    template<
      typename FRAMEBUFFER_TYPE = GLuint        ,
      typename BUFFER_TYPE      = GLenum        ,
      typename DRAWBUFFER_TYPE  = GLint         ,
      typename VALUE_TYPE       = const GLfloat*>
    class ClearNamedFramebufferfv:
        public ge::core::Command{
      protected:
        void*_getAttribAddress(std::string name){
          if(name == "framebuffer")return(void*)&this->framebuffer;
          if(name == "buffer"     )return(void*)&this->buffer     ;
          if(name == "drawbuffer" )return(void*)&this->drawbuffer ;
          if(name == "value"      )return(void*)&this->value      ;
          return NULL;
        }
      public:
        FRAMEBUFFER_TYPE framebuffer;
        BUFFER_TYPE      buffer     ;
        DRAWBUFFER_TYPE  drawbuffer ;
        VALUE_TYPE       value      ;
        ClearNamedFramebufferfv(
            FRAMEBUFFER_TYPE const&framebuffer,
            BUFFER_TYPE      const&buffer     ,
            DRAWBUFFER_TYPE  const&drawbuffer ,
            VALUE_TYPE       const&value      ){
          this->framebuffer = framebuffer;
          this->buffer      = buffer     ;
          this->drawbuffer  = drawbuffer ;
          this->value       = value      ;
        }
        virtual~ClearNamedFramebufferfv(){}
        virtual void operator()(){
          glClearNamedFramebufferfv(
            ge::core::convertTo<GLuint        >(this->framebuffer),
            ge::core::convertTo<GLenum        >(this->buffer     ),
            ge::core::convertTo<GLint         >(this->drawbuffer ),
            ge::core::convertTo<const GLfloat*>(this->value      )
          );
        }
    };

    template<
      typename FRAMEBUFFER_TYPE = GLuint      ,
      typename BUFFER_TYPE      = GLenum      ,
      typename DRAWBUFFER_TYPE  = GLint       ,
      typename VALUE_TYPE       = const GLint*>
    class ClearNamedFramebufferiv:
        public ge::core::Command{
      protected:
        void*_getAttribAddress(std::string name){
          if(name == "framebuffer")return(void*)&this->framebuffer;
          if(name == "buffer"     )return(void*)&this->buffer     ;
          if(name == "drawbuffer" )return(void*)&this->drawbuffer ;
          if(name == "value"      )return(void*)&this->value      ;
          return NULL;
        }
      public:
        FRAMEBUFFER_TYPE framebuffer;
        BUFFER_TYPE      buffer     ;
        DRAWBUFFER_TYPE  drawbuffer ;
        VALUE_TYPE       value      ;
        ClearNamedFramebufferiv(
            FRAMEBUFFER_TYPE const&framebuffer,
            BUFFER_TYPE      const&buffer     ,
            DRAWBUFFER_TYPE  const&drawbuffer ,
            VALUE_TYPE       const&value      ){
          this->framebuffer = framebuffer;
          this->buffer      = buffer     ;
          this->drawbuffer  = drawbuffer ;
          this->value       = value      ;
        }
        virtual~ClearNamedFramebufferiv(){}
        virtual void operator()(){
          glClearNamedFramebufferiv(
            ge::core::convertTo<GLuint      >(this->framebuffer),
            ge::core::convertTo<GLenum      >(this->buffer     ),
            ge::core::convertTo<GLint       >(this->drawbuffer ),
            ge::core::convertTo<const GLint*>(this->value      )
          );
        }
    };

    template<
      typename MODE_TYPE = GLenum>
    class DrawBuffer:
        public ge::core::Command{
      protected:
        void*_getAttribAddress(std::string name){
          if(name == "mode")return(void*)&this->mode;
          return NULL;
        }
      public:
        MODE_TYPE mode;
        DrawBuffer(
            MODE_TYPE const&mode){
          this->mode = mode;
        }
        virtual~DrawBuffer(){}
        virtual void operator()(){
          glDrawBuffer(
            ge::core::convertTo<GLenum>(this->mode)
          );
        }
    };

    template<
      typename FRAMEBUFFER_TYPE = GLuint       ,
      typename BUFFER_TYPE      = GLenum       ,
      typename DEPTH_TYPE       = const GLfloat,
      typename STENCIL_TYPE     = GLint        >
    class ClearNamedFramebufferfi:
        public ge::core::Command{
      protected:
        void*_getAttribAddress(std::string name){
          if(name == "framebuffer")return(void*)&this->framebuffer;
          if(name == "buffer"     )return(void*)&this->buffer     ;
          if(name == "depth"      )return(void*)&this->depth      ;
          if(name == "stencil"    )return(void*)&this->stencil    ;
          return NULL;
        }
      public:
        FRAMEBUFFER_TYPE framebuffer;
        BUFFER_TYPE      buffer     ;
        DEPTH_TYPE       depth      ;
        STENCIL_TYPE     stencil    ;
        ClearNamedFramebufferfi(
            FRAMEBUFFER_TYPE const&framebuffer,
            BUFFER_TYPE      const&buffer     ,
            DEPTH_TYPE       const&depth      ,
            STENCIL_TYPE     const&stencil    ){
          this->framebuffer = framebuffer;
          this->buffer      = buffer     ;
          this->depth       = depth      ;
          this->stencil     = stencil    ;
        }
        virtual~ClearNamedFramebufferfi(){}
        virtual void operator()(){
          glClearNamedFramebufferfi(
            ge::core::convertTo<GLuint       >(this->framebuffer),
            ge::core::convertTo<GLenum       >(this->buffer     ),
            ge::core::convertTo<const GLfloat>(this->depth      ),
            ge::core::convertTo<GLint        >(this->stencil    )
          );
        }
    };

    template<
      typename BUFFER_TYPE     = GLenum       ,
      typename DRAWBUFFER_TYPE = GLint        ,
      typename VALUE_TYPE      = const GLuint*>
    class ClearBufferuiv:
        public ge::core::Command{
      protected:
        void*_getAttribAddress(std::string name){
          if(name == "buffer"    )return(void*)&this->buffer    ;
          if(name == "drawbuffer")return(void*)&this->drawbuffer;
          if(name == "value"     )return(void*)&this->value     ;
          return NULL;
        }
      public:
        BUFFER_TYPE     buffer    ;
        DRAWBUFFER_TYPE drawbuffer;
        VALUE_TYPE      value     ;
        ClearBufferuiv(
            BUFFER_TYPE     const&buffer    ,
            DRAWBUFFER_TYPE const&drawbuffer,
            VALUE_TYPE      const&value     ){
          this->buffer     = buffer    ;
          this->drawbuffer = drawbuffer;
          this->value      = value     ;
        }
        virtual~ClearBufferuiv(){}
        virtual void operator()(){
          glClearBufferuiv(
            ge::core::convertTo<GLenum       >(this->buffer    ),
            ge::core::convertTo<GLint        >(this->drawbuffer),
            ge::core::convertTo<const GLuint*>(this->value     )
          );
        }
    };

    template<
      typename BUFFER_TYPE     = GLenum        ,
      typename DRAWBUFFER_TYPE = GLint         ,
      typename VALUE_TYPE      = const GLfloat*>
    class ClearBufferfv:
        public ge::core::Command{
      protected:
        void*_getAttribAddress(std::string name){
          if(name == "buffer"    )return(void*)&this->buffer    ;
          if(name == "drawbuffer")return(void*)&this->drawbuffer;
          if(name == "value"     )return(void*)&this->value     ;
          return NULL;
        }
      public:
        BUFFER_TYPE     buffer    ;
        DRAWBUFFER_TYPE drawbuffer;
        VALUE_TYPE      value     ;
        ClearBufferfv(
            BUFFER_TYPE     const&buffer    ,
            DRAWBUFFER_TYPE const&drawbuffer,
            VALUE_TYPE      const&value     ){
          this->buffer     = buffer    ;
          this->drawbuffer = drawbuffer;
          this->value      = value     ;
        }
        virtual~ClearBufferfv(){}
        virtual void operator()(){
          glClearBufferfv(
            ge::core::convertTo<GLenum        >(this->buffer    ),
            ge::core::convertTo<GLint         >(this->drawbuffer),
            ge::core::convertTo<const GLfloat*>(this->value     )
          );
        }
    };

    template<
      typename FRAMEBUFFER_TYPE    = GLuint       ,
      typename NUMATTACHMENTS_TYPE = GLsizei      ,
      typename ATTACHMENTS_TYPE    = const GLenum*>
    class InvalidateNamedFramebufferData:
        public ge::core::Command{
      protected:
        void*_getAttribAddress(std::string name){
          if(name == "framebuffer"   )return(void*)&this->framebuffer   ;
          if(name == "numAttachments")return(void*)&this->numAttachments;
          if(name == "attachments"   )return(void*)&this->attachments   ;
          return NULL;
        }
      public:
        FRAMEBUFFER_TYPE    framebuffer   ;
        NUMATTACHMENTS_TYPE numAttachments;
        ATTACHMENTS_TYPE    attachments   ;
        InvalidateNamedFramebufferData(
            FRAMEBUFFER_TYPE    const&framebuffer   ,
            NUMATTACHMENTS_TYPE const&numAttachments,
            ATTACHMENTS_TYPE    const&attachments   ){
          this->framebuffer    = framebuffer   ;
          this->numAttachments = numAttachments;
          this->attachments    = attachments   ;
        }
        virtual~InvalidateNamedFramebufferData(){}
        virtual void operator()(){
          glInvalidateNamedFramebufferData(
            ge::core::convertTo<GLuint       >(this->framebuffer   ),
            ge::core::convertTo<GLsizei      >(this->numAttachments),
            ge::core::convertTo<const GLenum*>(this->attachments   )
          );
        }
    };

    template<
      typename MASK_TYPE = GLbitfield>
    class Clear:
        public ge::core::Command{
      protected:
        void*_getAttribAddress(std::string name){
          if(name == "mask")return(void*)&this->mask;
          return NULL;
        }
      public:
        MASK_TYPE mask;
        Clear(
            MASK_TYPE const&mask){
          this->mask = mask;
        }
        virtual~Clear(){}
        virtual void operator()(){
          glClear(
            ge::core::convertTo<GLbitfield>(this->mask)
          );
        }
    };

    template<
      typename N_TYPE    = GLsizei      ,
      typename BUFS_TYPE = const GLenum*>
    class DrawBuffers:
        public ge::core::Command{
      protected:
        void*_getAttribAddress(std::string name){
          if(name == "n"   )return(void*)&this->n   ;
          if(name == "bufs")return(void*)&this->bufs;
          return NULL;
        }
      public:
        N_TYPE    n   ;
        BUFS_TYPE bufs;
        DrawBuffers(
            N_TYPE    const&n   ,
            BUFS_TYPE const&bufs){
          this->n    = n   ;
          this->bufs = bufs;
        }
        virtual~DrawBuffers(){}
        virtual void operator()(){
          glDrawBuffers(
            ge::core::convertTo<GLsizei      >(this->n   ),
            ge::core::convertTo<const GLenum*>(this->bufs)
          );
        }
    };

    template<
      typename FRAMEBUFFER_TYPE = GLuint       ,
      typename BUFFER_TYPE      = GLenum       ,
      typename DRAWBUFFER_TYPE  = GLint        ,
      typename VALUE_TYPE       = const GLuint*>
    class ClearNamedFramebufferuiv:
        public ge::core::Command{
      protected:
        void*_getAttribAddress(std::string name){
          if(name == "framebuffer")return(void*)&this->framebuffer;
          if(name == "buffer"     )return(void*)&this->buffer     ;
          if(name == "drawbuffer" )return(void*)&this->drawbuffer ;
          if(name == "value"      )return(void*)&this->value      ;
          return NULL;
        }
      public:
        FRAMEBUFFER_TYPE framebuffer;
        BUFFER_TYPE      buffer     ;
        DRAWBUFFER_TYPE  drawbuffer ;
        VALUE_TYPE       value      ;
        ClearNamedFramebufferuiv(
            FRAMEBUFFER_TYPE const&framebuffer,
            BUFFER_TYPE      const&buffer     ,
            DRAWBUFFER_TYPE  const&drawbuffer ,
            VALUE_TYPE       const&value      ){
          this->framebuffer = framebuffer;
          this->buffer      = buffer     ;
          this->drawbuffer  = drawbuffer ;
          this->value       = value      ;
        }
        virtual~ClearNamedFramebufferuiv(){}
        virtual void operator()(){
          glClearNamedFramebufferuiv(
            ge::core::convertTo<GLuint       >(this->framebuffer),
            ge::core::convertTo<GLenum       >(this->buffer     ),
            ge::core::convertTo<GLint        >(this->drawbuffer ),
            ge::core::convertTo<const GLuint*>(this->value      )
          );
        }
    };

    template<
      typename MASK_TYPE = GLuint>
    class StencilMask:
        public ge::core::Command{
      protected:
        void*_getAttribAddress(std::string name){
          if(name == "mask")return(void*)&this->mask;
          return NULL;
        }
      public:
        MASK_TYPE mask;
        StencilMask(
            MASK_TYPE const&mask){
          this->mask = mask;
        }
        virtual~StencilMask(){}
        virtual void operator()(){
          glStencilMask(
            ge::core::convertTo<GLuint>(this->mask)
          );
        }
    };

    template<
      typename FACE_TYPE = GLenum,
      typename MASK_TYPE = GLuint>
    class StencilMaskSeparate:
        public ge::core::Command{
      protected:
        void*_getAttribAddress(std::string name){
          if(name == "face")return(void*)&this->face;
          if(name == "mask")return(void*)&this->mask;
          return NULL;
        }
      public:
        FACE_TYPE face;
        MASK_TYPE mask;
        StencilMaskSeparate(
            FACE_TYPE const&face,
            MASK_TYPE const&mask){
          this->face = face;
          this->mask = mask;
        }
        virtual~StencilMaskSeparate(){}
        virtual void operator()(){
          glStencilMaskSeparate(
            ge::core::convertTo<GLenum>(this->face),
            ge::core::convertTo<GLuint>(this->mask)
          );
        }
    };

    template<
      typename INDEX_TYPE = GLuint   ,
      typename R_TYPE     = GLboolean,
      typename G_TYPE     = GLboolean,
      typename B_TYPE     = GLboolean,
      typename A_TYPE     = GLboolean>
    class ColorMaski:
        public ge::core::Command{
      protected:
        void*_getAttribAddress(std::string name){
          if(name == "index")return(void*)&this->index;
          if(name == "r"    )return(void*)&this->r    ;
          if(name == "g"    )return(void*)&this->g    ;
          if(name == "b"    )return(void*)&this->b    ;
          if(name == "a"    )return(void*)&this->a    ;
          return NULL;
        }
      public:
        INDEX_TYPE index;
        R_TYPE     r    ;
        G_TYPE     g    ;
        B_TYPE     b    ;
        A_TYPE     a    ;
        ColorMaski(
            INDEX_TYPE const&index,
            R_TYPE     const&r    ,
            G_TYPE     const&g    ,
            B_TYPE     const&b    ,
            A_TYPE     const&a    ){
          this->index = index;
          this->r     = r    ;
          this->g     = g    ;
          this->b     = b    ;
          this->a     = a    ;
        }
        virtual~ColorMaski(){}
        virtual void operator()(){
          glColorMaski(
            ge::core::convertTo<GLuint   >(this->index),
            ge::core::convertTo<GLboolean>(this->r    ),
            ge::core::convertTo<GLboolean>(this->g    ),
            ge::core::convertTo<GLboolean>(this->b    ),
            ge::core::convertTo<GLboolean>(this->a    )
          );
        }
    };

    template<
      typename FRAMEBUFFER_TYPE = GLuint,
      typename BUF_TYPE         = GLenum>
    class NamedFramebufferDrawBuffer:
        public ge::core::Command{
      protected:
        void*_getAttribAddress(std::string name){
          if(name == "framebuffer")return(void*)&this->framebuffer;
          if(name == "buf"        )return(void*)&this->buf        ;
          return NULL;
        }
      public:
        FRAMEBUFFER_TYPE framebuffer;
        BUF_TYPE         buf        ;
        NamedFramebufferDrawBuffer(
            FRAMEBUFFER_TYPE const&framebuffer,
            BUF_TYPE         const&buf        ){
          this->framebuffer = framebuffer;
          this->buf         = buf        ;
        }
        virtual~NamedFramebufferDrawBuffer(){}
        virtual void operator()(){
          glNamedFramebufferDrawBuffer(
            ge::core::convertTo<GLuint>(this->framebuffer),
            ge::core::convertTo<GLenum>(this->buf        )
          );
        }
    };

    template<
      typename BUFFER_TYPE     = GLenum ,
      typename DRAWBUFFER_TYPE = GLint  ,
      typename DEPTH_TYPE      = GLfloat,
      typename STENCIL_TYPE    = GLint  >
    class ClearBufferfi:
        public ge::core::Command{
      protected:
        void*_getAttribAddress(std::string name){
          if(name == "buffer"    )return(void*)&this->buffer    ;
          if(name == "drawbuffer")return(void*)&this->drawbuffer;
          if(name == "depth"     )return(void*)&this->depth     ;
          if(name == "stencil"   )return(void*)&this->stencil   ;
          return NULL;
        }
      public:
        BUFFER_TYPE     buffer    ;
        DRAWBUFFER_TYPE drawbuffer;
        DEPTH_TYPE      depth     ;
        STENCIL_TYPE    stencil   ;
        ClearBufferfi(
            BUFFER_TYPE     const&buffer    ,
            DRAWBUFFER_TYPE const&drawbuffer,
            DEPTH_TYPE      const&depth     ,
            STENCIL_TYPE    const&stencil   ){
          this->buffer     = buffer    ;
          this->drawbuffer = drawbuffer;
          this->depth      = depth     ;
          this->stencil    = stencil   ;
        }
        virtual~ClearBufferfi(){}
        virtual void operator()(){
          glClearBufferfi(
            ge::core::convertTo<GLenum >(this->buffer    ),
            ge::core::convertTo<GLint  >(this->drawbuffer),
            ge::core::convertTo<GLfloat>(this->depth     ),
            ge::core::convertTo<GLint  >(this->stencil   )
          );
        }
    };

    template<
      typename S_TYPE = GLint>
    class ClearStencil:
        public ge::core::Command{
      protected:
        void*_getAttribAddress(std::string name){
          if(name == "s")return(void*)&this->s;
          return NULL;
        }
      public:
        S_TYPE s;
        ClearStencil(
            S_TYPE const&s){
          this->s = s;
        }
        virtual~ClearStencil(){}
        virtual void operator()(){
          glClearStencil(
            ge::core::convertTo<GLint>(this->s)
          );
        }
    };

    template<
      typename FRAMEBUFFER_TYPE = GLuint       ,
      typename N_TYPE           = GLsizei      ,
      typename BUFS_TYPE        = const GLenum*>
    class NamedFramebufferDrawBuffers:
        public ge::core::Command{
      protected:
        void*_getAttribAddress(std::string name){
          if(name == "framebuffer")return(void*)&this->framebuffer;
          if(name == "n"          )return(void*)&this->n          ;
          if(name == "bufs"       )return(void*)&this->bufs       ;
          return NULL;
        }
      public:
        FRAMEBUFFER_TYPE framebuffer;
        N_TYPE           n          ;
        BUFS_TYPE        bufs       ;
        NamedFramebufferDrawBuffers(
            FRAMEBUFFER_TYPE const&framebuffer,
            N_TYPE           const&n          ,
            BUFS_TYPE        const&bufs       ){
          this->framebuffer = framebuffer;
          this->n           = n          ;
          this->bufs        = bufs       ;
        }
        virtual~NamedFramebufferDrawBuffers(){}
        virtual void operator()(){
          glNamedFramebufferDrawBuffers(
            ge::core::convertTo<GLuint       >(this->framebuffer),
            ge::core::convertTo<GLsizei      >(this->n          ),
            ge::core::convertTo<const GLenum*>(this->bufs       )
          );
        }
    };

    template<
      typename D_TYPE = GLfloat>
    class ClearDepthf:
        public ge::core::Command{
      protected:
        void*_getAttribAddress(std::string name){
          if(name == "d")return(void*)&this->d;
          return NULL;
        }
      public:
        D_TYPE d;
        ClearDepthf(
            D_TYPE const&d){
          this->d = d;
        }
        virtual~ClearDepthf(){}
        virtual void operator()(){
          glClearDepthf(
            ge::core::convertTo<GLfloat>(this->d)
          );
        }
    };

    template<
      typename FLAG_TYPE = GLboolean>
    class DepthMask:
        public ge::core::Command{
      protected:
        void*_getAttribAddress(std::string name){
          if(name == "flag")return(void*)&this->flag;
          return NULL;
        }
      public:
        FLAG_TYPE flag;
        DepthMask(
            FLAG_TYPE const&flag){
          this->flag = flag;
        }
        virtual~DepthMask(){}
        virtual void operator()(){
          glDepthMask(
            ge::core::convertTo<GLboolean>(this->flag)
          );
        }
    };

    template<
      typename RED_TYPE   = GLboolean,
      typename GREEN_TYPE = GLboolean,
      typename BLUE_TYPE  = GLboolean,
      typename ALPHA_TYPE = GLboolean>
    class ColorMask:
        public ge::core::Command{
      protected:
        void*_getAttribAddress(std::string name){
          if(name == "red"  )return(void*)&this->red  ;
          if(name == "green")return(void*)&this->green;
          if(name == "blue" )return(void*)&this->blue ;
          if(name == "alpha")return(void*)&this->alpha;
          return NULL;
        }
      public:
        RED_TYPE   red  ;
        GREEN_TYPE green;
        BLUE_TYPE  blue ;
        ALPHA_TYPE alpha;
        ColorMask(
            RED_TYPE   const&red  ,
            GREEN_TYPE const&green,
            BLUE_TYPE  const&blue ,
            ALPHA_TYPE const&alpha){
          this->red   = red  ;
          this->green = green;
          this->blue  = blue ;
          this->alpha = alpha;
        }
        virtual~ColorMask(){}
        virtual void operator()(){
          glColorMask(
            ge::core::convertTo<GLboolean>(this->red  ),
            ge::core::convertTo<GLboolean>(this->green),
            ge::core::convertTo<GLboolean>(this->blue ),
            ge::core::convertTo<GLboolean>(this->alpha)
          );
        }
    };

    template<
      typename RED_TYPE   = GLclampf,
      typename GREEN_TYPE = GLclampf,
      typename BLUE_TYPE  = GLclampf,
      typename ALPHA_TYPE = GLclampf>
    class ClearColor:
        public ge::core::Command{
      protected:
        void*_getAttribAddress(std::string name){
          if(name == "red"  )return(void*)&this->red  ;
          if(name == "green")return(void*)&this->green;
          if(name == "blue" )return(void*)&this->blue ;
          if(name == "alpha")return(void*)&this->alpha;
          return NULL;
        }
      public:
        RED_TYPE   red  ;
        GREEN_TYPE green;
        BLUE_TYPE  blue ;
        ALPHA_TYPE alpha;
        ClearColor(
            RED_TYPE   const&red  ,
            GREEN_TYPE const&green,
            BLUE_TYPE  const&blue ,
            ALPHA_TYPE const&alpha){
          this->red   = red  ;
          this->green = green;
          this->blue  = blue ;
          this->alpha = alpha;
        }
        virtual~ClearColor(){}
        virtual void operator()(){
          glClearColor(
            ge::core::convertTo<GLclampf>(this->red  ),
            ge::core::convertTo<GLclampf>(this->green),
            ge::core::convertTo<GLclampf>(this->blue ),
            ge::core::convertTo<GLclampf>(this->alpha)
          );
        }
    };

    template<
      typename BUFFER_TYPE     = GLenum      ,
      typename DRAWBUFFER_TYPE = GLint       ,
      typename VALUE_TYPE      = const GLint*>
    class ClearBufferiv:
        public ge::core::Command{
      protected:
        void*_getAttribAddress(std::string name){
          if(name == "buffer"    )return(void*)&this->buffer    ;
          if(name == "drawbuffer")return(void*)&this->drawbuffer;
          if(name == "value"     )return(void*)&this->value     ;
          return NULL;
        }
      public:
        BUFFER_TYPE     buffer    ;
        DRAWBUFFER_TYPE drawbuffer;
        VALUE_TYPE      value     ;
        ClearBufferiv(
            BUFFER_TYPE     const&buffer    ,
            DRAWBUFFER_TYPE const&drawbuffer,
            VALUE_TYPE      const&value     ){
          this->buffer     = buffer    ;
          this->drawbuffer = drawbuffer;
          this->value      = value     ;
        }
        virtual~ClearBufferiv(){}
        virtual void operator()(){
          glClearBufferiv(
            ge::core::convertTo<GLenum      >(this->buffer    ),
            ge::core::convertTo<GLint       >(this->drawbuffer),
            ge::core::convertTo<const GLint*>(this->value     )
          );
        }
    };

    template<
      typename TARGET_TYPE         = GLenum       ,
      typename NUMATTACHMENTS_TYPE = GLsizei      ,
      typename ATTACHMENTS_TYPE    = const GLenum*>
    class InvalidateFramebuffer:
        public ge::core::Command{
      protected:
        void*_getAttribAddress(std::string name){
          if(name == "target"        )return(void*)&this->target        ;
          if(name == "numAttachments")return(void*)&this->numAttachments;
          if(name == "attachments"   )return(void*)&this->attachments   ;
          return NULL;
        }
      public:
        TARGET_TYPE         target        ;
        NUMATTACHMENTS_TYPE numAttachments;
        ATTACHMENTS_TYPE    attachments   ;
        InvalidateFramebuffer(
            TARGET_TYPE         const&target        ,
            NUMATTACHMENTS_TYPE const&numAttachments,
            ATTACHMENTS_TYPE    const&attachments   ){
          this->target         = target        ;
          this->numAttachments = numAttachments;
          this->attachments    = attachments   ;
        }
        virtual~InvalidateFramebuffer(){}
        virtual void operator()(){
          glInvalidateFramebuffer(
            ge::core::convertTo<GLenum       >(this->target        ),
            ge::core::convertTo<GLsizei      >(this->numAttachments),
            ge::core::convertTo<const GLenum*>(this->attachments   )
          );
        }
    };

    template<
      typename FRAMEBUFFER_TYPE    = GLuint       ,
      typename NUMATTACHMENTS_TYPE = GLsizei      ,
      typename ATTACHMENTS_TYPE    = const GLenum*,
      typename X_TYPE              = GLint        ,
      typename Y_TYPE              = GLint        ,
      typename WIDTH_TYPE          = GLsizei      ,
      typename HEIGHT_TYPE         = GLsizei      >
    class InvalidateNamedFramebufferSubData:
        public ge::core::Command{
      protected:
        void*_getAttribAddress(std::string name){
          if(name == "framebuffer"   )return(void*)&this->framebuffer   ;
          if(name == "numAttachments")return(void*)&this->numAttachments;
          if(name == "attachments"   )return(void*)&this->attachments   ;
          if(name == "x"             )return(void*)&this->x             ;
          if(name == "y"             )return(void*)&this->y             ;
          if(name == "width"         )return(void*)&this->width         ;
          if(name == "height"        )return(void*)&this->height        ;
          return NULL;
        }
      public:
        FRAMEBUFFER_TYPE    framebuffer   ;
        NUMATTACHMENTS_TYPE numAttachments;
        ATTACHMENTS_TYPE    attachments   ;
        X_TYPE              x             ;
        Y_TYPE              y             ;
        WIDTH_TYPE          width         ;
        HEIGHT_TYPE         height        ;
        InvalidateNamedFramebufferSubData(
            FRAMEBUFFER_TYPE    const&framebuffer   ,
            NUMATTACHMENTS_TYPE const&numAttachments,
            ATTACHMENTS_TYPE    const&attachments   ,
            X_TYPE              const&x             ,
            Y_TYPE              const&y             ,
            WIDTH_TYPE          const&width         ,
            HEIGHT_TYPE         const&height        ){
          this->framebuffer    = framebuffer   ;
          this->numAttachments = numAttachments;
          this->attachments    = attachments   ;
          this->x              = x             ;
          this->y              = y             ;
          this->width          = width         ;
          this->height         = height        ;
        }
        virtual~InvalidateNamedFramebufferSubData(){}
        virtual void operator()(){
          glInvalidateNamedFramebufferSubData(
            ge::core::convertTo<GLuint       >(this->framebuffer   ),
            ge::core::convertTo<GLsizei      >(this->numAttachments),
            ge::core::convertTo<const GLenum*>(this->attachments   ),
            ge::core::convertTo<GLint        >(this->x             ),
            ge::core::convertTo<GLint        >(this->y             ),
            ge::core::convertTo<GLsizei      >(this->width         ),
            ge::core::convertTo<GLsizei      >(this->height        )
          );
        }
    };

    template<
      typename TARGET_TYPE         = GLenum       ,
      typename NUMATTACHMENTS_TYPE = GLsizei      ,
      typename ATTACHMENTS_TYPE    = const GLenum*,
      typename X_TYPE              = GLint        ,
      typename Y_TYPE              = GLint        ,
      typename WIDTH_TYPE          = GLsizei      ,
      typename HEIGHT_TYPE         = GLsizei      >
    class InvalidateSubFramebuffer:
        public ge::core::Command{
      protected:
        void*_getAttribAddress(std::string name){
          if(name == "target"        )return(void*)&this->target        ;
          if(name == "numAttachments")return(void*)&this->numAttachments;
          if(name == "attachments"   )return(void*)&this->attachments   ;
          if(name == "x"             )return(void*)&this->x             ;
          if(name == "y"             )return(void*)&this->y             ;
          if(name == "width"         )return(void*)&this->width         ;
          if(name == "height"        )return(void*)&this->height        ;
          return NULL;
        }
      public:
        TARGET_TYPE         target        ;
        NUMATTACHMENTS_TYPE numAttachments;
        ATTACHMENTS_TYPE    attachments   ;
        X_TYPE              x             ;
        Y_TYPE              y             ;
        WIDTH_TYPE          width         ;
        HEIGHT_TYPE         height        ;
        InvalidateSubFramebuffer(
            TARGET_TYPE         const&target        ,
            NUMATTACHMENTS_TYPE const&numAttachments,
            ATTACHMENTS_TYPE    const&attachments   ,
            X_TYPE              const&x             ,
            Y_TYPE              const&y             ,
            WIDTH_TYPE          const&width         ,
            HEIGHT_TYPE         const&height        ){
          this->target         = target        ;
          this->numAttachments = numAttachments;
          this->attachments    = attachments   ;
          this->x              = x             ;
          this->y              = y             ;
          this->width          = width         ;
          this->height         = height        ;
        }
        virtual~InvalidateSubFramebuffer(){}
        virtual void operator()(){
          glInvalidateSubFramebuffer(
            ge::core::convertTo<GLenum       >(this->target        ),
            ge::core::convertTo<GLsizei      >(this->numAttachments),
            ge::core::convertTo<const GLenum*>(this->attachments   ),
            ge::core::convertTo<GLint        >(this->x             ),
            ge::core::convertTo<GLint        >(this->y             ),
            ge::core::convertTo<GLsizei      >(this->width         ),
            ge::core::convertTo<GLsizei      >(this->height        )
          );
        }
    };

    template<
      typename DEPTH_TYPE = GLclampd>
    class ClearDepth:
        public ge::core::Command{
      protected:
        void*_getAttribAddress(std::string name){
          if(name == "depth")return(void*)&this->depth;
          return NULL;
        }
      public:
        DEPTH_TYPE depth;
        ClearDepth(
            DEPTH_TYPE const&depth){
          this->depth = depth;
        }
        virtual~ClearDepth(){}
        virtual void operator()(){
          glClearDepth(
            ge::core::convertTo<GLclampd>(this->depth)
          );
        }
    };

    template<
      typename FRAMEBUFFER_TYPE = GLuint        ,
      typename BUFFER_TYPE      = GLenum        ,
      typename DRAWBUFFER_TYPE  = GLint         ,
      typename VALUE_TYPE       = const GLfloat*>
    inline ClearNamedFramebufferfv<
      FRAMEBUFFER_TYPE,
      BUFFER_TYPE     ,
      DRAWBUFFER_TYPE ,
      VALUE_TYPE      >* newClearNamedFramebufferfv(
        FRAMEBUFFER_TYPE const&framebuffer,
        BUFFER_TYPE      const&buffer     ,
        DRAWBUFFER_TYPE  const&drawbuffer ,
        VALUE_TYPE       const&value      ){
        return new ClearNamedFramebufferfv<
          FRAMEBUFFER_TYPE,
          BUFFER_TYPE     ,
          DRAWBUFFER_TYPE ,
          VALUE_TYPE      >(
              framebuffer,
              buffer     ,
              drawbuffer ,
              value      );
    }
    template<
      typename FRAMEBUFFER_TYPE = GLuint      ,
      typename BUFFER_TYPE      = GLenum      ,
      typename DRAWBUFFER_TYPE  = GLint       ,
      typename VALUE_TYPE       = const GLint*>
    inline ClearNamedFramebufferiv<
      FRAMEBUFFER_TYPE,
      BUFFER_TYPE     ,
      DRAWBUFFER_TYPE ,
      VALUE_TYPE      >* newClearNamedFramebufferiv(
        FRAMEBUFFER_TYPE const&framebuffer,
        BUFFER_TYPE      const&buffer     ,
        DRAWBUFFER_TYPE  const&drawbuffer ,
        VALUE_TYPE       const&value      ){
        return new ClearNamedFramebufferiv<
          FRAMEBUFFER_TYPE,
          BUFFER_TYPE     ,
          DRAWBUFFER_TYPE ,
          VALUE_TYPE      >(
              framebuffer,
              buffer     ,
              drawbuffer ,
              value      );
    }
    template<
      typename MODE_TYPE = GLenum>
    inline DrawBuffer<
      MODE_TYPE>* newDrawBuffer(
        MODE_TYPE const&mode){
        return new DrawBuffer<
          MODE_TYPE>(
              mode);
    }
    template<
      typename FRAMEBUFFER_TYPE = GLuint       ,
      typename BUFFER_TYPE      = GLenum       ,
      typename DEPTH_TYPE       = const GLfloat,
      typename STENCIL_TYPE     = GLint        >
    inline ClearNamedFramebufferfi<
      FRAMEBUFFER_TYPE,
      BUFFER_TYPE     ,
      DEPTH_TYPE      ,
      STENCIL_TYPE    >* newClearNamedFramebufferfi(
        FRAMEBUFFER_TYPE const&framebuffer,
        BUFFER_TYPE      const&buffer     ,
        DEPTH_TYPE       const&depth      ,
        STENCIL_TYPE     const&stencil    ){
        return new ClearNamedFramebufferfi<
          FRAMEBUFFER_TYPE,
          BUFFER_TYPE     ,
          DEPTH_TYPE      ,
          STENCIL_TYPE    >(
              framebuffer,
              buffer     ,
              depth      ,
              stencil    );
    }
    template<
      typename BUFFER_TYPE     = GLenum       ,
      typename DRAWBUFFER_TYPE = GLint        ,
      typename VALUE_TYPE      = const GLuint*>
    inline ClearBufferuiv<
      BUFFER_TYPE    ,
      DRAWBUFFER_TYPE,
      VALUE_TYPE     >* newClearBufferuiv(
        BUFFER_TYPE     const&buffer    ,
        DRAWBUFFER_TYPE const&drawbuffer,
        VALUE_TYPE      const&value     ){
        return new ClearBufferuiv<
          BUFFER_TYPE    ,
          DRAWBUFFER_TYPE,
          VALUE_TYPE     >(
              buffer    ,
              drawbuffer,
              value     );
    }
    template<
      typename BUFFER_TYPE     = GLenum        ,
      typename DRAWBUFFER_TYPE = GLint         ,
      typename VALUE_TYPE      = const GLfloat*>
    inline ClearBufferfv<
      BUFFER_TYPE    ,
      DRAWBUFFER_TYPE,
      VALUE_TYPE     >* newClearBufferfv(
        BUFFER_TYPE     const&buffer    ,
        DRAWBUFFER_TYPE const&drawbuffer,
        VALUE_TYPE      const&value     ){
        return new ClearBufferfv<
          BUFFER_TYPE    ,
          DRAWBUFFER_TYPE,
          VALUE_TYPE     >(
              buffer    ,
              drawbuffer,
              value     );
    }
    template<
      typename FRAMEBUFFER_TYPE    = GLuint       ,
      typename NUMATTACHMENTS_TYPE = GLsizei      ,
      typename ATTACHMENTS_TYPE    = const GLenum*>
    inline InvalidateNamedFramebufferData<
      FRAMEBUFFER_TYPE   ,
      NUMATTACHMENTS_TYPE,
      ATTACHMENTS_TYPE   >* newInvalidateNamedFramebufferData(
        FRAMEBUFFER_TYPE    const&framebuffer   ,
        NUMATTACHMENTS_TYPE const&numAttachments,
        ATTACHMENTS_TYPE    const&attachments   ){
        return new InvalidateNamedFramebufferData<
          FRAMEBUFFER_TYPE   ,
          NUMATTACHMENTS_TYPE,
          ATTACHMENTS_TYPE   >(
              framebuffer   ,
              numAttachments,
              attachments   );
    }
    template<
      typename MASK_TYPE = GLbitfield>
    inline Clear<
      MASK_TYPE>* newClear(
        MASK_TYPE const&mask){
        return new Clear<
          MASK_TYPE>(
              mask);
    }
    template<
      typename N_TYPE    = GLsizei      ,
      typename BUFS_TYPE = const GLenum*>
    inline DrawBuffers<
      N_TYPE   ,
      BUFS_TYPE>* newDrawBuffers(
        N_TYPE    const&n   ,
        BUFS_TYPE const&bufs){
        return new DrawBuffers<
          N_TYPE   ,
          BUFS_TYPE>(
              n   ,
              bufs);
    }
    template<
      typename FRAMEBUFFER_TYPE = GLuint       ,
      typename BUFFER_TYPE      = GLenum       ,
      typename DRAWBUFFER_TYPE  = GLint        ,
      typename VALUE_TYPE       = const GLuint*>
    inline ClearNamedFramebufferuiv<
      FRAMEBUFFER_TYPE,
      BUFFER_TYPE     ,
      DRAWBUFFER_TYPE ,
      VALUE_TYPE      >* newClearNamedFramebufferuiv(
        FRAMEBUFFER_TYPE const&framebuffer,
        BUFFER_TYPE      const&buffer     ,
        DRAWBUFFER_TYPE  const&drawbuffer ,
        VALUE_TYPE       const&value      ){
        return new ClearNamedFramebufferuiv<
          FRAMEBUFFER_TYPE,
          BUFFER_TYPE     ,
          DRAWBUFFER_TYPE ,
          VALUE_TYPE      >(
              framebuffer,
              buffer     ,
              drawbuffer ,
              value      );
    }
    template<
      typename MASK_TYPE = GLuint>
    inline StencilMask<
      MASK_TYPE>* newStencilMask(
        MASK_TYPE const&mask){
        return new StencilMask<
          MASK_TYPE>(
              mask);
    }
    template<
      typename FACE_TYPE = GLenum,
      typename MASK_TYPE = GLuint>
    inline StencilMaskSeparate<
      FACE_TYPE,
      MASK_TYPE>* newStencilMaskSeparate(
        FACE_TYPE const&face,
        MASK_TYPE const&mask){
        return new StencilMaskSeparate<
          FACE_TYPE,
          MASK_TYPE>(
              face,
              mask);
    }
    template<
      typename INDEX_TYPE = GLuint   ,
      typename R_TYPE     = GLboolean,
      typename G_TYPE     = GLboolean,
      typename B_TYPE     = GLboolean,
      typename A_TYPE     = GLboolean>
    inline ColorMaski<
      INDEX_TYPE,
      R_TYPE    ,
      G_TYPE    ,
      B_TYPE    ,
      A_TYPE    >* newColorMaski(
        INDEX_TYPE const&index,
        R_TYPE     const&r    ,
        G_TYPE     const&g    ,
        B_TYPE     const&b    ,
        A_TYPE     const&a    ){
        return new ColorMaski<
          INDEX_TYPE,
          R_TYPE    ,
          G_TYPE    ,
          B_TYPE    ,
          A_TYPE    >(
              index,
              r    ,
              g    ,
              b    ,
              a    );
    }
    template<
      typename FRAMEBUFFER_TYPE = GLuint,
      typename BUF_TYPE         = GLenum>
    inline NamedFramebufferDrawBuffer<
      FRAMEBUFFER_TYPE,
      BUF_TYPE        >* newNamedFramebufferDrawBuffer(
        FRAMEBUFFER_TYPE const&framebuffer,
        BUF_TYPE         const&buf        ){
        return new NamedFramebufferDrawBuffer<
          FRAMEBUFFER_TYPE,
          BUF_TYPE        >(
              framebuffer,
              buf        );
    }
    template<
      typename BUFFER_TYPE     = GLenum ,
      typename DRAWBUFFER_TYPE = GLint  ,
      typename DEPTH_TYPE      = GLfloat,
      typename STENCIL_TYPE    = GLint  >
    inline ClearBufferfi<
      BUFFER_TYPE    ,
      DRAWBUFFER_TYPE,
      DEPTH_TYPE     ,
      STENCIL_TYPE   >* newClearBufferfi(
        BUFFER_TYPE     const&buffer    ,
        DRAWBUFFER_TYPE const&drawbuffer,
        DEPTH_TYPE      const&depth     ,
        STENCIL_TYPE    const&stencil   ){
        return new ClearBufferfi<
          BUFFER_TYPE    ,
          DRAWBUFFER_TYPE,
          DEPTH_TYPE     ,
          STENCIL_TYPE   >(
              buffer    ,
              drawbuffer,
              depth     ,
              stencil   );
    }
    template<
      typename S_TYPE = GLint>
    inline ClearStencil<
      S_TYPE>* newClearStencil(
        S_TYPE const&s){
        return new ClearStencil<
          S_TYPE>(
              s);
    }
    template<
      typename FRAMEBUFFER_TYPE = GLuint       ,
      typename N_TYPE           = GLsizei      ,
      typename BUFS_TYPE        = const GLenum*>
    inline NamedFramebufferDrawBuffers<
      FRAMEBUFFER_TYPE,
      N_TYPE          ,
      BUFS_TYPE       >* newNamedFramebufferDrawBuffers(
        FRAMEBUFFER_TYPE const&framebuffer,
        N_TYPE           const&n          ,
        BUFS_TYPE        const&bufs       ){
        return new NamedFramebufferDrawBuffers<
          FRAMEBUFFER_TYPE,
          N_TYPE          ,
          BUFS_TYPE       >(
              framebuffer,
              n          ,
              bufs       );
    }
    template<
      typename D_TYPE = GLfloat>
    inline ClearDepthf<
      D_TYPE>* newClearDepthf(
        D_TYPE const&d){
        return new ClearDepthf<
          D_TYPE>(
              d);
    }
    template<
      typename FLAG_TYPE = GLboolean>
    inline DepthMask<
      FLAG_TYPE>* newDepthMask(
        FLAG_TYPE const&flag){
        return new DepthMask<
          FLAG_TYPE>(
              flag);
    }
    template<
      typename RED_TYPE   = GLboolean,
      typename GREEN_TYPE = GLboolean,
      typename BLUE_TYPE  = GLboolean,
      typename ALPHA_TYPE = GLboolean>
    inline ColorMask<
      RED_TYPE  ,
      GREEN_TYPE,
      BLUE_TYPE ,
      ALPHA_TYPE>* newColorMask(
        RED_TYPE   const&red  ,
        GREEN_TYPE const&green,
        BLUE_TYPE  const&blue ,
        ALPHA_TYPE const&alpha){
        return new ColorMask<
          RED_TYPE  ,
          GREEN_TYPE,
          BLUE_TYPE ,
          ALPHA_TYPE>(
              red  ,
              green,
              blue ,
              alpha);
    }
    template<
      typename RED_TYPE   = GLclampf,
      typename GREEN_TYPE = GLclampf,
      typename BLUE_TYPE  = GLclampf,
      typename ALPHA_TYPE = GLclampf>
    inline ClearColor<
      RED_TYPE  ,
      GREEN_TYPE,
      BLUE_TYPE ,
      ALPHA_TYPE>* newClearColor(
        RED_TYPE   const&red  ,
        GREEN_TYPE const&green,
        BLUE_TYPE  const&blue ,
        ALPHA_TYPE const&alpha){
        return new ClearColor<
          RED_TYPE  ,
          GREEN_TYPE,
          BLUE_TYPE ,
          ALPHA_TYPE>(
              red  ,
              green,
              blue ,
              alpha);
    }
    template<
      typename BUFFER_TYPE     = GLenum      ,
      typename DRAWBUFFER_TYPE = GLint       ,
      typename VALUE_TYPE      = const GLint*>
    inline ClearBufferiv<
      BUFFER_TYPE    ,
      DRAWBUFFER_TYPE,
      VALUE_TYPE     >* newClearBufferiv(
        BUFFER_TYPE     const&buffer    ,
        DRAWBUFFER_TYPE const&drawbuffer,
        VALUE_TYPE      const&value     ){
        return new ClearBufferiv<
          BUFFER_TYPE    ,
          DRAWBUFFER_TYPE,
          VALUE_TYPE     >(
              buffer    ,
              drawbuffer,
              value     );
    }
    template<
      typename TARGET_TYPE         = GLenum       ,
      typename NUMATTACHMENTS_TYPE = GLsizei      ,
      typename ATTACHMENTS_TYPE    = const GLenum*>
    inline InvalidateFramebuffer<
      TARGET_TYPE        ,
      NUMATTACHMENTS_TYPE,
      ATTACHMENTS_TYPE   >* newInvalidateFramebuffer(
        TARGET_TYPE         const&target        ,
        NUMATTACHMENTS_TYPE const&numAttachments,
        ATTACHMENTS_TYPE    const&attachments   ){
        return new InvalidateFramebuffer<
          TARGET_TYPE        ,
          NUMATTACHMENTS_TYPE,
          ATTACHMENTS_TYPE   >(
              target        ,
              numAttachments,
              attachments   );
    }
    template<
      typename FRAMEBUFFER_TYPE    = GLuint       ,
      typename NUMATTACHMENTS_TYPE = GLsizei      ,
      typename ATTACHMENTS_TYPE    = const GLenum*,
      typename X_TYPE              = GLint        ,
      typename Y_TYPE              = GLint        ,
      typename WIDTH_TYPE          = GLsizei      ,
      typename HEIGHT_TYPE         = GLsizei      >
    inline InvalidateNamedFramebufferSubData<
      FRAMEBUFFER_TYPE   ,
      NUMATTACHMENTS_TYPE,
      ATTACHMENTS_TYPE   ,
      X_TYPE             ,
      Y_TYPE             ,
      WIDTH_TYPE         ,
      HEIGHT_TYPE        >* newInvalidateNamedFramebufferSubData(
        FRAMEBUFFER_TYPE    const&framebuffer   ,
        NUMATTACHMENTS_TYPE const&numAttachments,
        ATTACHMENTS_TYPE    const&attachments   ,
        X_TYPE              const&x             ,
        Y_TYPE              const&y             ,
        WIDTH_TYPE          const&width         ,
        HEIGHT_TYPE         const&height        ){
        return new InvalidateNamedFramebufferSubData<
          FRAMEBUFFER_TYPE   ,
          NUMATTACHMENTS_TYPE,
          ATTACHMENTS_TYPE   ,
          X_TYPE             ,
          Y_TYPE             ,
          WIDTH_TYPE         ,
          HEIGHT_TYPE        >(
              framebuffer   ,
              numAttachments,
              attachments   ,
              x             ,
              y             ,
              width         ,
              height        );
    }
    template<
      typename TARGET_TYPE         = GLenum       ,
      typename NUMATTACHMENTS_TYPE = GLsizei      ,
      typename ATTACHMENTS_TYPE    = const GLenum*,
      typename X_TYPE              = GLint        ,
      typename Y_TYPE              = GLint        ,
      typename WIDTH_TYPE          = GLsizei      ,
      typename HEIGHT_TYPE         = GLsizei      >
    inline InvalidateSubFramebuffer<
      TARGET_TYPE        ,
      NUMATTACHMENTS_TYPE,
      ATTACHMENTS_TYPE   ,
      X_TYPE             ,
      Y_TYPE             ,
      WIDTH_TYPE         ,
      HEIGHT_TYPE        >* newInvalidateSubFramebuffer(
        TARGET_TYPE         const&target        ,
        NUMATTACHMENTS_TYPE const&numAttachments,
        ATTACHMENTS_TYPE    const&attachments   ,
        X_TYPE              const&x             ,
        Y_TYPE              const&y             ,
        WIDTH_TYPE          const&width         ,
        HEIGHT_TYPE         const&height        ){
        return new InvalidateSubFramebuffer<
          TARGET_TYPE        ,
          NUMATTACHMENTS_TYPE,
          ATTACHMENTS_TYPE   ,
          X_TYPE             ,
          Y_TYPE             ,
          WIDTH_TYPE         ,
          HEIGHT_TYPE        >(
              target        ,
              numAttachments,
              attachments   ,
              x             ,
              y             ,
              width         ,
              height        );
    }
    template<
      typename DEPTH_TYPE = GLclampd>
    inline ClearDepth<
      DEPTH_TYPE>* newClearDepth(
        DEPTH_TYPE const&depth){
        return new ClearDepth<
          DEPTH_TYPE>(
              depth);
    }
  }
}
