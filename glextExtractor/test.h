void,glTextureParameterfv,GLuint,texture,GLenum,pname,const GLfloat*,param
void,glObjectLabel,GLenum,identifier,GLuint,name,GLsizei,length,const GLchar*,label
void,glCreateQueries,GLenum,target,GLsizei,n,GLuint*,ids
void,glUniform2d,GLint,location,GLdouble,x,GLdouble,y
void,glScaled,GLdouble,x,GLdouble,y,GLdouble,z
void,glBlendFuncSeparatei,GLuint,buf,GLenum,srcRGB,GLenum,dstRGB,GLenum,srcAlpha,GLenum,dstAlpha
void,glNormal3sv,const GLshort*,v
void,glMultiDrawElementsBaseVertex,GLenum,mode,const GLsizei*,count,GLenum,type,const void*const*,indices,GLsizei,drawcount,const GLint*,basevertex
void,glInvalidateBufferSubData,GLuint,buffer,GLintptr,offset,GLsizeiptr,length
void,glTextureStorage3DMultisample,GLuint,texture,GLsizei,samples,GLenum,internalformat,GLsizei,width,GLsizei,height,GLsizei,depth,GLboolean,fixedsamplelocations
void,glTexImage2DMultisample,GLenum,target,GLsizei,samples,GLenum,internalformat,GLsizei,width,GLsizei,height,GLboolean,fixedsamplelocations
void,glGetnMapdv,GLenum,target,GLenum,query,GLsizei,bufSize,GLdouble*,v
void,glColor3us,GLushort,red,GLushort,green,GLushort,blue
void,glClearNamedFramebufferfv,GLuint,framebuffer,GLenum,buffer,GLint,drawbuffer,const GLfloat*,value
void,glDeleteSync,GLsync,sync
const GLubyte*,glGetString,GLenum,name
void,glVertexAttrib1d,GLuint,index,GLdouble,x
void,glIndexdv,const GLdouble*,c
void,glVertex4fv,const GLfloat*,v
void,glPointParameteriv,GLenum,pname,const GLint*,params
void,glPopClientAttrib
void,glGetnUniformfv,GLuint,program,GLint,location,GLsizei,bufSize,GLfloat*,params
void,glFrustum,GLdouble,left,GLdouble,right,GLdouble,bottom,GLdouble,top,GLdouble,near_val,GLdouble,far_val
void,glSelectBuffer,GLsizei,size,GLuint*,buffer
void,glBindSampler,GLuint,unit,GLuint,sampler
void,glGetTexParameteriv,GLenum,target,GLenum,pname,GLint*,params
void,glCopyImageSubData,GLuint,srcName,GLenum,srcTarget,GLint,srcLevel,GLint,srcX,GLint,srcY,GLint,srcZ,GLuint,dstName,GLenum,dstTarget,GLint,dstLevel,GLint,dstX,GLint,dstY,GLint,dstZ,GLsizei,srcWidth,GLsizei,srcHeight,GLsizei,srcDepth
void,glVertexAttribI4ui,GLuint,index,GLuint,x,GLuint,y,GLuint,z,GLuint,w
void,glPushDebugGroup,GLenum,source,GLuint,id,GLsizei,length,const GLchar*,message
void,glGetActiveAttrib,GLuint,program,GLuint,index,GLsizei,bufSize,GLsizei*,length,GLint*,size,GLenum*,type,GLchar*,name
void,glEvalCoord2d,GLdouble,u,GLdouble,v
void,glMultMatrixd,const GLdouble*,m
void,glGetTextureLevelParameterfv,GLuint,texture,GLint,level,GLenum,pname,GLfloat*,params
void,glVertexP2ui,GLenum,type,GLuint,value
void,glIndexfv,const GLfloat*,c
void,glGetQueryBufferObjectiv,GLuint,id,GLuint,buffer,GLenum,pname,GLintptr,offset
void,glClearNamedFramebufferiv,GLuint,framebuffer,GLenum,buffer,GLint,drawbuffer,const GLint*,value
void,glMultiTexCoord2d,GLenum,target,GLdouble,s,GLdouble,t
void,glPixelMapusv,GLenum,map,GLsizei,mapsize,const GLushort*,values
void,glMultiTexCoord3iv,GLenum,target,const GLint*,v
void,glMultTransposeMatrixf,const GLfloat,m[16]
void,glFogiv,GLenum,pname,const GLint*,params
void,glColor4f,GLfloat,red,GLfloat,green,GLfloat,blue,GLfloat,alpha
void,glGetMultisamplefv,GLenum,pname,GLuint,index,GLfloat*,val
void,glFogCoordfv,const GLfloat*,coord
void,glGetSeparableFilter,GLenum,target,GLenum,format,GLenum,type,GLvoid*,row,GLvoid*,column,GLvoid*,span
void,glClearNamedBufferData,GLuint,buffer,GLenum,internalformat,GLenum,format,GLenum,type,const void*,data
void,glVertexAttrib4Nuiv,GLuint,index,const GLuint*,v
GLboolean,glIsShader,GLuint,shader
void,glVertexAttribP1ui,GLuint,index,GLenum,type,GLboolean,normalized,GLuint,value
void,glLoadMatrixd,const GLdouble*,m
void,glMultiDrawArrays,GLenum,mode,const GLint*,first,const GLsizei*,count,GLsizei,drawcount
void,glIndexsv,const GLshort*,c
void,glTexCoord1dv,const GLdouble*,v
void,glUseProgram,GLuint,program
void,glMultiTexCoord1dv,GLenum,target,const GLdouble*,v
void,glNormal3iv,const GLint*,v
void,glVertexAttribP2uiv,GLuint,index,GLenum,type,GLboolean,normalized,const GLuint*,value
void,glTexParameterf,GLenum,target,GLenum,pname,GLfloat,param
void,glGetProgramResourceiv,GLuint,program,GLenum,programInterface,GLuint,index,GLsizei,propCount,const GLenum*,props,GLsizei,bufSize,GLsizei*,length,GLint*,params
void,glMultiTexCoordP3uiv,GLenum,texture,GLenum,type,const GLuint*,coords
void,glVertexArrayAttribIFormat,GLuint,vaobj,GLuint,attribindex,GLint,size,GLenum,type,GLuint,relativeoffset
void,glGetTexEnvfv,GLenum,target,GLenum,pname,GLfloat*,params
const GLubyte*,glGetStringi,GLenum,name,GLuint,index
void,glClearTexSubImage,GLuint,texture,GLint,level,GLint,xoffset,GLint,yoffset,GLint,zoffset,GLsizei,width,GLsizei,height,GLsizei,depth,GLenum,format,GLenum,type,const void*,data
void,glIndexs,GLshort,c
void,glGetMapfv,GLenum,target,GLenum,query,GLfloat*,v
void,glProgramUniformMatrix4x3dv,GLuint,program,GLint,location,GLsizei,count,GLboolean,transpose,const GLdouble*,value
void,glUniform3dv,GLint,location,GLsizei,count,const GLdouble*,value
void,glTexCoordP2ui,GLenum,type,GLuint,coords
void,glColor3usv,const GLushort*,v
void,glSecondaryColor3uiv,const GLuint*,v
void,glGetQueryIndexediv,GLenum,target,GLuint,index,GLenum,pname,GLint*,params
void,glGetActiveAtomicCounterBufferiv,GLuint,program,GLuint,bufferIndex,GLenum,pname,GLint*,params
void,glVertexAttrib4bv,GLuint,index,const GLbyte*,v
void,glMultiTexCoord3sv,GLenum,target,const GLshort*,v
void,glUniform1fv,GLint,location,GLsizei,count,const GLfloat*,value
void,glGetProgramBinary,GLuint,program,GLsizei,bufSize,GLsizei*,length,GLenum*,binaryFormat,void*,binary
void,glGetQueryObjecti64v,GLuint,id,GLenum,pname,GLint64*,params
void,glInitNames
void,glShaderStorageBlockBinding,GLuint,program,GLuint,storageBlockIndex,GLuint,storageBlockBinding
void,glUniformMatrix2x3dv,GLint,location,GLsizei,count,GLboolean,transpose,const GLdouble*,value
void,glObjectPtrLabel,const void*,ptr,GLsizei,length,const GLchar*,label
void,glTexCoord4d,GLdouble,s,GLdouble,t,GLdouble,r,GLdouble,q
void,glGetConvolutionFilter,GLenum,target,GLenum,format,GLenum,type,GLvoid*,image
void,glGenFramebuffers,GLsizei,n,GLuint*,framebuffers
void,glDrawBuffer,GLenum,mode
void,glStencilFuncSeparate,GLenum,face,GLenum,func,GLint,ref,GLuint,mask
void,glNamedRenderbufferStorageMultisample,GLuint,renderbuffer,GLsizei,samples,GLenum,internalformat,GLsizei,width,GLsizei,height
void,glColorP3uiv,GLenum,type,const GLuint*,color
void,glVertexAttribP3ui,GLuint,index,GLenum,type,GLboolean,normalized,GLuint,value
void,glWindowPos2d,GLdouble,x,GLdouble,y
void,glUniform1dv,GLint,location,GLsizei,count,const GLdouble*,value
void,glPointSize,GLfloat,size
void,glMultiTexCoord2iv,GLenum,target,const GLint*,v
void,glColor4uiv,const GLuint*,v
void,glBindFramebuffer,GLenum,target,GLuint,framebuffer
void,glVertexAttribIPointer,GLuint,index,GLint,size,GLenum,type,GLsizei,stride,const void*,pointer
void,glWindowPos2i,GLint,x,GLint,y
void,glBindTextureUnit,GLuint,unit,GLuint,texture
void,glClearNamedFramebufferfi,GLuint,framebuffer,GLenum,buffer,const GLfloat,depth,GLint,stencil
void,glTexGend,GLenum,coord,GLenum,pname,GLdouble,param
void,glGetQueryBufferObjectuiv,GLuint,id,GLuint,buffer,GLenum,pname,GLintptr,offset
GLenum,glGetError
void,glProgramUniformMatrix2x4dv,GLuint,program,GLint,location,GLsizei,count,GLboolean,transpose,const GLdouble*,value
void,glNormalPointer,GLenum,type,GLsizei,stride,const GLvoid*,ptr
void,glGetTexParameterfv,GLenum,target,GLenum,pname,GLfloat*,params
void,glNamedBufferStorage,GLuint,buffer,GLsizeiptr,size,const void*,data,GLbitfield,flags
void,glGetMinmax,GLenum,target,GLboolean,reset,GLenum,format,GLenum,types,GLvoid*,values
void,glWindowPos3iv,const GLint*,v
void,glVertexAttrib3fv,GLuint,index,const GLfloat*,v
void,glColor4ui,GLuint,red,GLuint,green,GLuint,blue,GLuint,alpha
void,glBindImageTextures,GLuint,first,GLsizei,count,const GLuint*,textures
void,glGetActiveSubroutineUniformiv,GLuint,program,GLenum,shadertype,GLuint,index,GLenum,pname,GLint*,values
void,glDrawElementsInstancedBaseInstance,GLenum,mode,GLsizei,count,GLenum,type,const void*,indices,GLsizei,instancecount,GLuint,baseinstance
void,glMultiDrawElements,GLenum,mode,const GLsizei*,count,GLenum,type,const void*const*,indices,GLsizei,drawcount
void,glProgramUniformMatrix4x2dv,GLuint,program,GLint,location,GLsizei,count,GLboolean,transpose,const GLdouble*,value
void,glVertexP3ui,GLenum,type,GLuint,value
void,glVertex3fv,const GLfloat*,v
GLint,glGetFragDataIndex,GLuint,program,const GLchar*,name
void,glMultiTexCoord1i,GLenum,target,GLint,s
void,glProgramUniform1ui,GLuint,program,GLint,location,GLuint,v0
void,glBeginConditionalRender,GLuint,id,GLenum,mode
void,glDepthRange,GLclampd,near_val,GLclampd,far_val
void,glVertexAttrib3f,GLuint,index,GLfloat,x,GLfloat,y,GLfloat,z
void,glNamedFramebufferRenderbuffer,GLuint,framebuffer,GLenum,attachment,GLenum,renderbuffertarget,GLuint,renderbuffer
void,glWindowPos3d,GLdouble,x,GLdouble,y,GLdouble,z
void,glSecondaryColor3i,GLint,red,GLint,green,GLint,blue
void,glEvalCoord1dv,const GLdouble*,u
void,glGenSamplers,GLsizei,count,GLuint*,samplers
void,glVertexAttrib2f,GLuint,index,GLfloat,x,GLfloat,y
void,glGetCompressedTextureSubImage,GLuint,texture,GLint,level,GLint,xoffset,GLint,yoffset,GLint,zoffset,GLsizei,width,GLsizei,height,GLsizei,depth,GLsizei,bufSize,void*,pixels
void,glGetVertexAttribdv,GLuint,index,GLenum,pname,GLdouble*,params
void,glViewportIndexedfv,GLuint,index,const GLfloat*,v
GLuint,glCreateShader,GLenum,type
void,glMultiTexCoord4f,GLenum,target,GLfloat,s,GLfloat,t,GLfloat,r,GLfloat,q
void,glVertexAttribP2ui,GLuint,index,GLenum,type,GLboolean,normalized,GLuint,value
void,glGetMapiv,GLenum,target,GLenum,query,GLint*,v
void,glGenVertexArrays,GLsizei,n,GLuint*,arrays
void,glUniformBlockBinding,GLuint,program,GLuint,uniformBlockIndex,GLuint,uniformBlockBinding
void,glTexCoord4sv,const GLshort*,v
void,glShadeModel,GLenum,mode
void,glVertexAttrib4Nsv,GLuint,index,const GLshort*,v
void,glWindowPos3f,GLfloat,x,GLfloat,y,GLfloat,z
void,glGetnUniformdv,GLuint,program,GLint,location,GLsizei,bufSize,GLdouble*,params
void,glGetnPixelMapusv,GLenum,map,GLsizei,bufSize,GLushort*,values
void,glMap1d,GLenum,target,GLdouble,u1,GLdouble,u2,GLint,stride,GLint,order,const GLdouble*,points
void,glProgramUniform4ui,GLuint,program,GLint,location,GLuint,v0,GLuint,v1,GLuint,v2,GLuint,v3
void,glProgramUniformMatrix2x4fv,GLuint,program,GLint,location,GLsizei,count,GLboolean,transpose,const GLfloat*,value
void,glSecondaryColor3ubv,const GLubyte*,v
void,glVertex2sv,const GLshort*,v
void,glCompressedTextureSubImage3D,GLuint,texture,GLint,level,GLint,xoffset,GLint,yoffset,GLint,zoffset,GLsizei,width,GLsizei,height,GLsizei,depth,GLenum,format,GLsizei,imageSize,const void*,data
void,glLoadIdentity
void,glIndexMask,GLuint,mask
void,glPauseTransformFeedback
void,glMultiTexCoord3d,GLenum,target,GLdouble,s,GLdouble,t,GLdouble,r
void,glFogi,GLenum,pname,GLint,param
void,glGetObjectLabel,GLenum,identifier,GLuint,name,GLsizei,bufSize,GLsizei*,length,GLchar*,label
void,glReadnPixels,GLint,x,GLint,y,GLsizei,width,GLsizei,height,GLenum,format,GLenum,type,GLsizei,bufSize,void*,data
void,glClearBufferuiv,GLenum,buffer,GLint,drawbuffer,const GLuint*,value
void,glGetObjectPtrLabel,const void*,ptr,GLsizei,bufSize,GLsizei*,length,GLchar*,label
void,glProgramUniform1iv,GLuint,program,GLint,location,GLsizei,count,const GLint*,value
void,glUniform2uiv,GLint,location,GLsizei,count,const GLuint*,value
void,glRasterPos2s,GLshort,x,GLshort,y
void,glTextureStorage2DMultisample,GLuint,texture,GLsizei,samples,GLenum,internalformat,GLsizei,width,GLsizei,height,GLboolean,fixedsamplelocations
void,glEvalCoord2fv,const GLfloat*,u
void,glMultiTexCoordP1uiv,GLenum,texture,GLenum,type,const GLuint*,coords
void,glClearBufferfv,GLenum,buffer,GLint,drawbuffer,const GLfloat*,value
void,glProgramUniform3fv,GLuint,program,GLint,location,GLsizei,count,const GLfloat*,value
void,glCopyTextureSubImage3D,GLuint,texture,GLint,level,GLint,xoffset,GLint,yoffset,GLint,zoffset,GLint,x,GLint,y,GLsizei,width,GLsizei,height
void,glFramebufferTexture3D,GLenum,target,GLenum,attachment,GLenum,textarget,GLuint,texture,GLint,level,GLint,zoffset
GLboolean,glUnmapNamedBuffer,GLuint,buffer
void,glCallLists,GLsizei,n,GLenum,type,const GLvoid*,lists
void,glProgramUniform1fv,GLuint,program,GLint,location,GLsizei,count,const GLfloat*,value
void,glMapGrid2f,GLint,un,GLfloat,u1,GLfloat,u2,GLint,vn,GLfloat,v1,GLfloat,v2
void,glProgramUniform2ui,GLuint,program,GLint,location,GLuint,v0,GLuint,v1
void,glGetQueryObjectiv,GLuint,id,GLenum,pname,GLint*,params
void,glWaitSync,GLsync,sync,GLbitfield,flags,GLuint64,timeout
void,glMap2f,GLenum,target,GLfloat,u1,GLfloat,u2,GLint,ustride,GLint,uorder,GLfloat,v1,GLfloat,v2,GLint,vstride,GLint,vorder,const GLfloat*,points
void,glUniform3ui,GLint,location,GLuint,v0,GLuint,v1,GLuint,v2
void,glFinish
GLuint,glCreateProgram
void,glTransformFeedbackBufferRange,GLuint,xfb,GLuint,index,GLuint,buffer,GLintptr,offset,GLsizeiptr,size
void,glColor3s,GLshort,red,GLshort,green,GLshort,blue
void,glGetShaderiv,GLuint,shader,GLenum,pname,GLint*,params
void,glBlendEquation,GLenum,mode
void,glGenQueries,GLsizei,n,GLuint*,ids
void,glVertex4i,GLint,x,GLint,y,GLint,z,GLint,w
void,glConvolutionParameterf,GLenum,target,GLenum,pname,GLfloat,params
void,glUniform3iv,GLint,location,GLsizei,count,const GLint*,value
void,glMultiTexCoordP2ui,GLenum,texture,GLenum,type,GLuint,coords
void,glGetNamedRenderbufferParameteriv,GLuint,renderbuffer,GLenum,pname,GLint*,params
void,glInvalidateNamedFramebufferData,GLuint,framebuffer,GLsizei,numAttachments,const GLenum*,attachments
void,glDeleteTextures,GLsizei,n,const GLuint*,textures
void,glPixelTransferf,GLenum,pname,GLfloat,param
void,glVertexAttrib4Nub,GLuint,index,GLubyte,x,GLubyte,y,GLubyte,z,GLubyte,w
void,glWindowPos3fv,const GLfloat*,v
void,glMultiTexCoordP4ui,GLenum,texture,GLenum,type,GLuint,coords
void,glVertexAttribL3d,GLuint,index,GLdouble,x,GLdouble,y,GLdouble,z
void,glGetActiveUniformBlockName,GLuint,program,GLuint,uniformBlockIndex,GLsizei,bufSize,GLsizei*,length,GLchar*,uniformBlockName
void,glVertex4sv,const GLshort*,v
void,glGetConvolutionParameterfv,GLenum,target,GLenum,pname,GLfloat*,params
void,glTexCoord2sv,const GLshort*,v
void,glGenRenderbuffers,GLsizei,n,GLuint*,renderbuffers
void,glLightfv,GLenum,light,GLenum,pname,const GLfloat*,params
void,glMap2d,GLenum,target,GLdouble,u1,GLdouble,u2,GLint,ustride,GLint,uorder,GLdouble,v1,GLdouble,v2,GLint,vstride,GLint,vorder,const GLdouble*,points
void,glMultiTexCoord1s,GLenum,target,GLshort,s
void,glEvalPoint1,GLint,i
void,glVertexAttribP1uiv,GLuint,index,GLenum,type,GLboolean,normalized,const GLuint*,value
void,glClear,GLbitfield,mask
void,glRasterPos2f,GLfloat,x,GLfloat,y
void,glVertex4dv,const GLdouble*,v
void,glMateriali,GLenum,face,GLenum,pname,GLint,param
void,glVertexAttribL4dv,GLuint,index,const GLdouble*,v
void,glGetNamedBufferPointerv,GLuint,buffer,GLenum,pname,void**,params
void,glGetBufferSubData,GLenum,target,GLintptr,offset,GLsizeiptr,size,void*,data
void,glGetInternalformativ,GLenum,target,GLenum,internalformat,GLenum,pname,GLsizei,bufSize,GLint*,params
void,glDrawArraysInstancedBaseInstance,GLenum,mode,GLint,first,GLsizei,count,GLsizei,instancecount,GLuint,baseinstance
void,glViewportArrayv,GLuint,first,GLsizei,count,const GLfloat*,v
void,glDeleteFramebuffers,GLsizei,n,const GLuint*,framebuffers
void,glGetMaterialfv,GLenum,face,GLenum,pname,GLfloat*,params
void,glGetnColorTable,GLenum,target,GLenum,format,GLenum,type,GLsizei,bufSize,void*,table
void,glDrawBuffers,GLsizei,n,const GLenum*,bufs
void,glPopAttrib
void,glWindowPos2iv,const GLint*,v
void,glSamplerParameterf,GLuint,sampler,GLenum,pname,GLfloat,param
void,glTexCoordP2uiv,GLenum,type,const GLuint*,coords
void,glVertexAttribI1ui,GLuint,index,GLuint,x
void,glVertexAttribI4iv,GLuint,index,const GLint*,v
void,glRasterPos3s,GLshort,x,GLshort,y,GLshort,z
void,glGetInternalformati64v,GLenum,target,GLenum,internalformat,GLenum,pname,GLsizei,bufSize,GLint64*,params
void,glGetMinmaxParameteriv,GLenum,target,GLenum,pname,GLint*,params
void,glWindowPos2dv,const GLdouble*,v
void,glRasterPos3d,GLdouble,x,GLdouble,y,GLdouble,z
void,glGetUniformIndices,GLuint,program,GLsizei,uniformCount,const GLchar*const*,uniformNames,GLuint*,uniformIndices
void,glDeleteLists,GLuint,list,GLsizei,range
void,glDeleteTransformFeedbacks,GLsizei,n,const GLuint*,ids
void,glTexCoordP4ui,GLenum,type,GLuint,coords
void,glScissorIndexedv,GLuint,index,const GLint*,v
void,glBindTexture,GLenum,target,GLuint,texture
void,glVertexAttribI1iv,GLuint,index,const GLint*,v
void,glFramebufferTexture2D,GLenum,target,GLenum,attachment,GLenum,textarget,GLuint,texture,GLint,level
void,glIndexiv,const GLint*,c
void,glPushMatrix
void,glDebugMessageCallback,GLDEBUGPROC,callback,const void*,userParam
void,glMultTransposeMatrixd,const GLdouble,m[16]
void,glColorTable,GLenum,target,GLenum,internalformat,GLsizei,width,GLenum,format,GLenum,type,const GLvoid*,table
void,glVertex3sv,const GLshort*,v
GLint,glGetProgramResourceLocation,GLuint,program,GLenum,programInterface,const GLchar*,name
void,glTexStorage3D,GLenum,target,GLsizei,levels,GLenum,internalformat,GLsizei,width,GLsizei,height,GLsizei,depth
void,glGetUniformiv,GLuint,program,GLint,location,GLint*,params
void,glLineWidth,GLfloat,width
void,glLineStipple,GLint,factor,GLushort,pattern
void,glTextureBarrier
void,glValidateProgramPipeline,GLuint,pipeline
void,glIndexf,GLfloat,c
void,glGetLightiv,GLenum,light,GLenum,pname,GLint*,params
void,glArrayElement,GLint,i
void,glTextureParameterIiv,GLuint,texture,GLenum,pname,const GLint*,params
void,glFramebufferRenderbuffer,GLenum,target,GLenum,attachment,GLenum,renderbuffertarget,GLuint,renderbuffer
void,glMultiTexCoord1iv,GLenum,target,const GLint*,v
void,glTextureSubImage2D,GLuint,texture,GLint,level,GLint,xoffset,GLint,yoffset,GLsizei,width,GLsizei,height,GLenum,format,GLenum,type,const void*,pixels
void,glActiveTexture,GLenum,texture
void,glRasterPos3dv,const GLdouble*,v
void,glRasterPos4d,GLdouble,x,GLdouble,y,GLdouble,z,GLdouble,w
void,glGenerateTextureMipmap,GLuint,texture
GLboolean,glAreTexturesResident,GLsizei,n,const GLuint*,textures,GLboolean*,residences
void,glTransformFeedbackVaryings,GLuint,program,GLsizei,count,const GLchar*const*,varyings,GLenum,bufferMode
void,glWindowPos2sv,const GLshort*,v
void,glTexCoord2iv,const GLint*,v
void,glGenBuffers,GLsizei,n,GLuint*,buffers
void,glEnableClientState,GLenum,cap
void,glVertexAttrib4Nusv,GLuint,index,const GLushort*,v
void,glGetnMapiv,GLenum,target,GLenum,query,GLsizei,bufSize,GLint*,v
void,glEnableVertexArrayAttrib,GLuint,vaobj,GLuint,index
void,glDispatchComputeIndirect,GLintptr,indirect
void,glGetRenderbufferParameteriv,GLenum,target,GLenum,pname,GLint*,params
void,glColor3uiv,const GLuint*,v
void,glIndexd,GLdouble,c
void,glCullFace,GLenum,mode
void,glMaterialiv,GLenum,face,GLenum,pname,const GLint*,params
void,glTexCoord1sv,const GLshort*,v
void,glConvolutionFilter2D,GLenum,target,GLenum,internalformat,GLsizei,width,GLsizei,height,GLenum,format,GLenum,type,const GLvoid*,image
void,glTexCoord1s,GLshort,s
void,glTexCoord4s,GLshort,s,GLshort,t,GLshort,r,GLshort,q
void,glTexGeniv,GLenum,coord,GLenum,pname,const GLint*,params
void,glStencilOpSeparate,GLenum,face,GLenum,sfail,GLenum,dpfail,GLenum,dppass
void,glCopyTexImage1D,GLenum,target,GLint,level,GLenum,internalformat,GLint,x,GLint,y,GLsizei,width,GLint,border
void,glReleaseShaderCompiler
void,glUniformMatrix4dv,GLint,location,GLsizei,count,GLboolean,transpose,const GLdouble*,value
void,glBindTransformFeedback,GLenum,target,GLuint,id
void,glListBase,GLuint,base
void,glMultiTexCoord2fv,GLenum,target,const GLfloat*,v
void,glVertexAttrib4ubv,GLuint,index,const GLubyte*,v
void,glVertexAttrib4Niv,GLuint,index,const GLint*,v
void,glScissor,GLint,x,GLint,y,GLsizei,width,GLsizei,height
void,glColor4us,GLushort,red,GLushort,green,GLushort,blue,GLushort,alpha
void,glRasterPos2i,GLint,x,GLint,y
void,glMultiTexCoord2i,GLenum,target,GLint,s,GLint,t
void,glBindImageTexture,GLuint,unit,GLuint,texture,GLint,level,GLboolean,layered,GLint,layer,GLenum,access,GLenum,format
void,glGetTexGenfv,GLenum,coord,GLenum,pname,GLfloat*,params
void,glClientActiveTexture,GLenum,texture
void,glUniform1ui,GLint,location,GLuint,v0
void*,glMapBufferRange,GLenum,target,GLintptr,offset,GLsizeiptr,length,GLbitfield,access
void,glBindRenderbuffer,GLenum,target,GLuint,renderbuffer
void,glFeedbackBuffer,GLsizei,size,GLenum,type,GLfloat*,buffer
void,glRasterPos2sv,const GLshort*,v
void,glBlendFuncSeparate,GLenum,sfactorRGB,GLenum,dfactorRGB,GLenum,sfactorAlpha,GLenum,dfactorAlpha
void,glUniformMatrix3x4dv,GLint,location,GLsizei,count,GLboolean,transpose,const GLdouble*,value
void,glColor4d,GLdouble,red,GLdouble,green,GLdouble,blue,GLdouble,alpha
GLboolean,glIsList,GLuint,list
void,glTextureSubImage3D,GLuint,texture,GLint,level,GLint,xoffset,GLint,yoffset,GLint,zoffset,GLsizei,width,GLsizei,height,GLsizei,depth,GLenum,format,GLenum,type,const void*,pixels
void,glPixelStorei,GLenum,pname,GLint,param
void,glTexCoordP1ui,GLenum,type,GLuint,coords
void,glMultiTexCoord3dv,GLenum,target,const GLdouble*,v
void,glVertexAttribLPointer,GLuint,index,GLint,size,GLenum,type,GLsizei,stride,const void*,pointer
void,glDrawArraysInstanced,GLenum,mode,GLint,first,GLsizei,count,GLsizei,instancecount
void,glTexStorage2D,GLenum,target,GLsizei,levels,GLenum,internalformat,GLsizei,width,GLsizei,height
void,glEvalCoord1d,GLdouble,u
GLenum,glClientWaitSync,GLsync,sync,GLbitfield,flags,GLuint64,timeout
void,glDrawPixels,GLsizei,width,GLsizei,height,GLenum,format,GLenum,type,const GLvoid*,pixels
void,glGetTextureImage,GLuint,texture,GLint,level,GLenum,format,GLenum,type,GLsizei,bufSize,void*,pixels
void,glRotatef,GLfloat,angle,GLfloat,x,GLfloat,y,GLfloat,z
void,glGetProgramInfoLog,GLuint,program,GLsizei,bufSize,GLsizei*,length,GLchar*,infoLog
void,glValidateProgram,GLuint,program
void,glGetTextureParameteriv,GLuint,texture,GLenum,pname,GLint*,params
void,glMultiTexCoord4sv,GLenum,target,const GLshort*,v
void,glMultMatrixf,const GLfloat*,m
void,glGetQueryObjectuiv,GLuint,id,GLenum,pname,GLuint*,params
void,glProgramUniform1i,GLuint,program,GLint,location,GLint,v0
void,glGetActiveUniformsiv,GLuint,program,GLsizei,uniformCount,const GLuint*,uniformIndices,GLenum,pname,GLint*,params
void,glVertexAttribI2ui,GLuint,index,GLuint,x,GLuint,y
void,glMultiTexCoord4i,GLenum,target,GLint,s,GLint,t,GLint,r,GLint,q
void,glDetachShader,GLuint,program,GLuint,shader
void,glProgramUniformMatrix2x3dv,GLuint,program,GLint,location,GLsizei,count,GLboolean,transpose,const GLdouble*,value
void,glInterleavedArrays,GLenum,format,GLsizei,stride,const GLvoid*,pointer
void,glColorP4uiv,GLenum,type,const GLuint*,color
void,glVertexArrayAttribLFormat,GLuint,vaobj,GLuint,attribindex,GLint,size,GLenum,type,GLuint,relativeoffset
void,glPixelMapx,GLenum,map,GLint,size,const GLfixed*,values
void,glColorPointer,GLint,size,GLenum,type,GLsizei,stride,const GLvoid*,ptr
void,glClearNamedFramebufferuiv,GLuint,framebuffer,GLenum,buffer,GLint,drawbuffer,const GLuint*,value
void,glProgramUniform3uiv,GLuint,program,GLint,location,GLsizei,count,const GLuint*,value
void,glBindTextures,GLuint,first,GLsizei,count,const GLuint*,textures
void,glMapGrid2d,GLint,un,GLdouble,u1,GLdouble,u2,GLint,vn,GLdouble,v1,GLdouble,v2
void,glTexCoord1d,GLdouble,s
void,glGetFloati_v,GLenum,target,GLuint,index,GLfloat*,data
void,glPushAttrib,GLbitfield,mask
GLint,glRenderMode,GLenum,mode
void,glGetTextureParameterfv,GLuint,texture,GLenum,pname,GLfloat*,params
void,glNamedFramebufferParameteri,GLuint,framebuffer,GLenum,pname,GLint,param
void,glResetMinmax,GLenum,target
void,glDebugMessageControl,GLenum,source,GLenum,type,GLenum,severity,GLsizei,count,const GLuint*,ids,GLboolean,enabled
void,glFlushMappedBufferRange,GLenum,target,GLintptr,offset,GLsizeiptr,length
void,glStencilMask,GLuint,mask
void,glColorP4ui,GLenum,type,GLuint,color
void,glTexGenf,GLenum,coord,GLenum,pname,GLfloat,param
void,glDrawElementsInstancedBaseVertexBaseInstance,GLenum,mode,GLsizei,count,GLenum,type,const void*,indices,GLsizei,instancecount,GLint,basevertex,GLuint,baseinstance
void,glMultiTexCoordP2uiv,GLenum,texture,GLenum,type,const GLuint*,coords
void,glScalef,GLfloat,x,GLfloat,y,GLfloat,z
void,glCopyTexSubImage1D,GLenum,target,GLint,level,GLint,xoffset,GLint,x,GLint,y,GLsizei,width
void,glVertexAttribI2uiv,GLuint,index,const GLuint*,v
void,glEnable,GLenum,cap
void,glSamplerParameterIuiv,GLuint,sampler,GLenum,pname,const GLuint*,param
void,glUniform2iv,GLint,location,GLsizei,count,const GLint*,value
void,glProgramUniform2uiv,GLuint,program,GLint,location,GLsizei,count,const GLuint*,value
void,glGetShaderPrecisionFormat,GLenum,shadertype,GLenum,precisiontype,GLint*,range,GLint*,precision
void,glDisablei,GLenum,target,GLuint,index
void,glVertexAttribI4usv,GLuint,index,const GLushort*,v
void,glVertexAttribI4sv,GLuint,index,const GLshort*,v
void,glProgramUniform3i,GLuint,program,GLint,location,GLint,v0,GLint,v1,GLint,v2
void,glRotated,GLdouble,angle,GLdouble,x,GLdouble,y,GLdouble,z
GLuint,glGetSubroutineIndex,GLuint,program,GLenum,shadertype,const GLchar*,name
void,glGetClipPlane,GLenum,plane,GLdouble*,equation
void,glVertexAttribI1i,GLuint,index,GLint,x
void,glGetConvolutionParameteriv,GLenum,target,GLenum,pname,GLint*,params
void,glProgramUniform3d,GLuint,program,GLint,location,GLdouble,v0,GLdouble,v1,GLdouble,v2
void,glProgramUniformMatrix3dv,GLuint,program,GLint,location,GLsizei,count,GLboolean,transpose,const GLdouble*,value
void,glPixelMapfv,GLenum,map,GLsizei,mapsize,const GLfloat*,values
void,glClearBufferSubData,GLenum,target,GLenum,internalformat,GLintptr,offset,GLsizeiptr,size,GLenum,format,GLenum,type,const void*,data
void,glGetPixelMapfv,GLenum,map,GLfloat*,values
void,glTextureParameteriv,GLuint,texture,GLenum,pname,const GLint*,param
void,glGetBufferPointerv,GLenum,target,GLenum,pname,void**,params
void,glClipControl,GLenum,origin,GLenum,depth
void,glTextureView,GLuint,texture,GLenum,target,GLuint,origtexture,GLenum,internalformat,GLuint,minlevel,GLuint,numlevels,GLuint,minlayer,GLuint,numlayers
void,glNewList,GLuint,list,GLenum,mode
void,glVertexAttrib3dv,GLuint,index,const GLdouble*,v
void,glGetBufferParameteri64v,GLenum,target,GLenum,pname,GLint64*,params
void,glPopName
void,glGetDoublev,GLenum,pname,GLdouble*,params
void,glGetInteger64i_v,GLenum,target,GLuint,index,GLint64*,data
void,glGetTexLevelParameteriv,GLenum,target,GLint,level,GLenum,pname,GLint*,params
void,glGetVertexArrayiv,GLuint,vaobj,GLenum,pname,GLint*,param
void,glRasterPos4f,GLfloat,x,GLfloat,y,GLfloat,z,GLfloat,w
void,glLighti,GLenum,light,GLenum,pname,GLint,param
void,glTexSubImage3D,GLenum,target,GLint,level,GLint,xoffset,GLint,yoffset,GLint,zoffset,GLsizei,width,GLsizei,height,GLsizei,depth,GLenum,format,GLenum,type,const GLvoid*,pixels
void,glDrawElementsBaseVertex,GLenum,mode,GLsizei,count,GLenum,type,const void*,indices,GLint,basevertex
void,glBindBuffersRange,GLenum,target,GLuint,first,GLsizei,count,const GLuint*,buffers,const GLintptr*,offsets,const GLsizeiptr*,sizes
void,glLightiv,GLenum,light,GLenum,pname,const GLint*,params
void,glDepthRangeIndexed,GLuint,index,GLdouble,n,GLdouble,f
void,glDepthRangef,GLfloat,n,GLfloat,f
void,glDrawRangeElements,GLenum,mode,GLuint,start,GLuint,end,GLsizei,count,GLenum,type,const GLvoid*,indices
void,glGetShaderSource,GLuint,shader,GLsizei,bufSize,GLsizei*,length,GLchar*,source
void,glPatchParameterfv,GLenum,pname,const GLfloat*,values
void,glStencilMaskSeparate,GLenum,face,GLuint,mask
GLboolean,glIsRenderbuffer,GLuint,renderbuffer
void,glGetnMapfv,GLenum,target,GLenum,query,GLsizei,bufSize,GLfloat*,v
void,glUniform2f,GLint,location,GLfloat,v0,GLfloat,v1
void,glUniform1iv,GLint,location,GLsizei,count,const GLint*,value
GLenum,glGetGraphicsResetStatus
void,glGetVertexAttribPointerv,GLuint,index,GLenum,pname,void**,pointer
void,glUniformMatrix2x4dv,GLint,location,GLsizei,count,GLboolean,transpose,const GLdouble*,value
void,glVertexArrayVertexBuffers,GLuint,vaobj,GLuint,first,GLsizei,count,const GLuint*,buffers,const GLintptr*,offsets,const GLsizei*,strides
void,glRectf,GLfloat,x1,GLfloat,y1,GLfloat,x2,GLfloat,y2
void,glTexParameteriv,GLenum,target,GLenum,pname,const GLint*,params
void,glColor3b,GLbyte,red,GLbyte,green,GLbyte,blue
void,glTexSubImage1D,GLenum,target,GLint,level,GLint,xoffset,GLsizei,width,GLenum,format,GLenum,type,const GLvoid*,pixels
void,glColorMaski,GLuint,index,GLboolean,r,GLboolean,g,GLboolean,b,GLboolean,a
void,glFramebufferTexture,GLenum,target,GLenum,attachment,GLuint,texture,GLint,level
void,glVertexAttrib4Nbv,GLuint,index,const GLbyte*,v
void,glBufferSubData,GLenum,target,GLintptr,offset,GLsizeiptr,size,const void*,data
void,glClearTexImage,GLuint,texture,GLint,level,GLenum,format,GLenum,type,const void*,data
void,glVertexArrayElementBuffer,GLuint,vaobj,GLuint,buffer
void,glGetIntegeri_v,GLenum,target,GLuint,index,GLint*,data
void,glEdgeFlag,GLboolean,flag
void,glVertexAttrib2dv,GLuint,index,const GLdouble*,v
void,glCreateBuffers,GLsizei,n,GLuint*,buffers
void,glPixelStoref,GLenum,pname,GLfloat,param
void,glClearIndex,GLfloat,c
void,glGetSynciv,GLsync,sync,GLenum,pname,GLsizei,bufSize,GLsizei*,length,GLint*,values
void,glVertexAttrib1dv,GLuint,index,const GLdouble*,v
void,glTexCoord4f,GLfloat,s,GLfloat,t,GLfloat,r,GLfloat,q
void,glSecondaryColor3dv,const GLdouble*,v
void,glSeparableFilter2D,GLenum,target,GLenum,internalformat,GLsizei,width,GLsizei,height,GLenum,format,GLenum,type,const GLvoid*,row,const GLvoid*,column
void,glColor3fv,const GLfloat*,v
void,glVertexArrayVertexBuffer,GLuint,vaobj,GLuint,bindingindex,GLuint,buffer,GLintptr,offset,GLsizei,stride
void,glGetTextureParameterIiv,GLuint,texture,GLenum,pname,GLint*,params
void,glConvolutionParameteriv,GLenum,target,GLenum,pname,const GLint*,params
void,glSamplerParameterfv,GLuint,sampler,GLenum,pname,const GLfloat*,param
void,glTexStorage1D,GLenum,target,GLsizei,levels,GLenum,internalformat,GLsizei,width
void,glPatchParameteri,GLenum,pname,GLint,value
void,glUniform4i,GLint,location,GLint,v0,GLint,v1,GLint,v2,GLint,v3
void,glEvalPoint2,GLint,i,GLint,j
void,glVertexAttribLFormat,GLuint,attribindex,GLint,size,GLenum,type,GLuint,relativeoffset
void,glGetColorTableParameterfv,GLenum,target,GLenum,pname,GLfloat*,params
void,glDrawTransformFeedback,GLenum,mode,GLuint,id
void,glPixelMapuiv,GLenum,map,GLsizei,mapsize,const GLuint*,values
void,glVertexAttribI2iv,GLuint,index,const GLint*,v
void,glVertex3iv,const GLint*,v
void,glSecondaryColor3d,GLdouble,red,GLdouble,green,GLdouble,blue
void,glVertexAttribP4ui,GLuint,index,GLenum,type,GLboolean,normalized,GLuint,value
void,glHistogram,GLenum,target,GLsizei,width,GLenum,internalformat,GLboolean,sink
void,glAccum,GLenum,op,GLfloat,value
void,glVertexAttrib1f,GLuint,index,GLfloat,x
void,glUniform2i,GLint,location,GLint,v0,GLint,v1
void,glVertexAttribI3i,GLuint,index,GLint,x,GLint,y,GLint,z
GLenum,glCheckNamedFramebufferStatus,GLuint,framebuffer,GLenum,target
void,glGetProgramPipelineInfoLog,GLuint,pipeline,GLsizei,bufSize,GLsizei*,length,GLchar*,infoLog
void,glLoadTransposeMatrixf,const GLfloat,m[16]
void,glGetnCompressedTexImage,GLenum,target,GLint,lod,GLsizei,bufSize,void*,pixels
void,glColor4fv,const GLfloat*,v
void,glGetProgramStageiv,GLuint,program,GLenum,shadertype,GLenum,pname,GLint*,values
void,glBlendFunc,GLenum,sfactor,GLenum,dfactor
void,glTexCoordPointer,GLint,size,GLenum,type,GLsizei,stride,const GLvoid*,ptr
void,glViewportIndexedf,GLuint,index,GLfloat,x,GLfloat,y,GLfloat,w,GLfloat,h
void,glProgramUniform2dv,GLuint,program,GLint,location,GLsizei,count,const GLdouble*,value
void,glBindBuffersBase,GLenum,target,GLuint,first,GLsizei,count,const GLuint*,buffers
void,glTexImage3D,GLenum,target,GLint,level,GLint,internalFormat,GLsizei,width,GLsizei,height,GLsizei,depth,GLint,border,GLenum,format,GLenum,type,const GLvoid*,pixels
void,glRectsv,const GLshort*,v1,const GLshort*,v2
void,glVertexAttrib4uiv,GLuint,index,const GLuint*,v
void,glTexCoord4dv,const GLdouble*,v
void,glColorP3ui,GLenum,type,GLuint,color
void,glGetShaderInfoLog,GLuint,shader,GLsizei,bufSize,GLsizei*,length,GLchar*,infoLog
void,glWindowPos2fv,const GLfloat*,v
void,glPixelZoom,GLfloat,xfactor,GLfloat,yfactor
void,glColor3d,GLdouble,red,GLdouble,green,GLdouble,blue
void,glVertexAttrib4sv,GLuint,index,const GLshort*,v
void,glTexCoord2dv,const GLdouble*,v
void,glPixelStorex,GLenum,pname,GLfixed,param
void,glGetSamplerParameterIiv,GLuint,sampler,GLenum,pname,GLint*,params
void,glFramebufferParameteri,GLenum,target,GLenum,pname,GLint,param
void,glDeleteBuffers,GLsizei,n,const GLuint*,buffers
void,glProgramUniform3iv,GLuint,program,GLint,location,GLsizei,count,const GLint*,value
void,glDebugMessageInsert,GLenum,source,GLenum,type,GLuint,id,GLenum,severity,GLsizei,length,const GLchar*,buf
void,glGetNamedBufferParameteriv,GLuint,buffer,GLenum,pname,GLint*,params
void,glTextureSubImage1D,GLuint,texture,GLint,level,GLint,xoffset,GLsizei,width,GLenum,format,GLenum,type,const void*,pixels
void,glScissorIndexed,GLuint,index,GLint,left,GLint,bottom,GLsizei,width,GLsizei,height
void,glVertex4iv,const GLint*,v
void,glBindFragDataLocationIndexed,GLuint,program,GLuint,colorNumber,GLuint,index,const GLchar*,name
void,glFogCoorddv,const GLdouble*,coord
void,glGetPixelMapxv,GLenum,map,GLint,size,GLfixed*,values
void,glMultiTexCoord3fv,GLenum,target,const GLfloat*,v
void,glGetnUniformuiv,GLuint,program,GLint,location,GLsizei,bufSize,GLuint*,params
void,glCreateVertexArrays,GLsizei,n,GLuint*,arrays
void,glNamedFramebufferDrawBuffer,GLuint,framebuffer,GLenum,buf
void,glReadBuffer,GLenum,mode
void,glClampColor,GLenum,target,GLenum,clamp
void,glNormal3s,GLshort,nx,GLshort,ny,GLshort,nz
void,glDisable,GLenum,cap
void,glSampleMaski,GLuint,maskNumber,GLbitfield,mask
void,glProgramUniformMatrix4fv,GLuint,program,GLint,location,GLsizei,count,GLboolean,transpose,const GLfloat*,value
void,glMultiTexCoord1f,GLenum,target,GLfloat,s
void,glDeleteSamplers,GLsizei,count,const GLuint*,samplers
void,glDrawTransformFeedbackStream,GLenum,mode,GLuint,id,GLuint,stream
void,glDeleteVertexArrays,GLsizei,n,const GLuint*,arrays
GLboolean,glIsEnabled,GLenum,cap
void,glBindFragDataLocation,GLuint,program,GLuint,color,const GLchar*,name
GLboolean,glIsTransformFeedback,GLuint,id
void,glLightModelf,GLenum,pname,GLfloat,param
void,glNamedFramebufferReadBuffer,GLuint,framebuffer,GLenum,src
void,glProgramUniformMatrix4dv,GLuint,program,GLint,location,GLsizei,count,GLboolean,transpose,const GLdouble*,value
void,glVertexAttribIFormat,GLuint,attribindex,GLint,size,GLenum,type,GLuint,relativeoffset
void,glEvalCoord1f,GLfloat,u
void,glRecti,GLint,x1,GLint,y1,GLint,x2,GLint,y2
void,glMemoryBarrierByRegion,GLbitfield,barriers
void,glMaterialf,GLenum,face,GLenum,pname,GLfloat,param
void,glUniformMatrix4x2fv,GLint,location,GLsizei,count,GLboolean,transpose,const GLfloat*,value
void,glGetnPixelMapuiv,GLenum,map,GLsizei,bufSize,GLuint*,values
void,glBeginTransformFeedback,GLenum,primitiveMode
void,glVertex4d,GLdouble,x,GLdouble,y,GLdouble,z,GLdouble,w
void,glUniformMatrix2x3fv,GLint,location,GLsizei,count,GLboolean,transpose,const GLfloat*,value
void,glDrawTransformFeedbackStreamInstanced,GLenum,mode,GLuint,id,GLuint,stream,GLsizei,instancecount
void,glGetTransformFeedbacki_v,GLuint,xfb,GLenum,pname,GLuint,index,GLint*,param
void,glUniform4dv,GLint,location,GLsizei,count,const GLdouble*,value
void,glDrawArraysIndirect,GLenum,mode,const void*,indirect
void,glBeginQuery,GLenum,target,GLuint,id
void,glGetSamplerParameterIuiv,GLuint,sampler,GLenum,pname,GLuint*,params
void,glTexGenfv,GLenum,coord,GLenum,pname,const GLfloat*,params
void,glFlushMappedNamedBufferRange,GLuint,buffer,GLintptr,offset,GLsizeiptr,length
void,glCopyColorSubTable,GLenum,target,GLsizei,start,GLint,x,GLint,y,GLsizei,width
void,glNormal3i,GLint,nx,GLint,ny,GLint,nz
void,glProgramUniformMatrix3x4fv,GLuint,program,GLint,location,GLsizei,count,GLboolean,transpose,const GLfloat*,value
void,glGetTextureSubImage,GLuint,texture,GLint,level,GLint,xoffset,GLint,yoffset,GLint,zoffset,GLsizei,width,GLsizei,height,GLsizei,depth,GLenum,format,GLenum,type,GLsizei,bufSize,void*,pixels
void,glBindAttribLocation,GLuint,program,GLuint,index,const GLchar*,name
void,glEndQuery,GLenum,target
void,glTexEnvf,GLenum,target,GLenum,pname,GLfloat,param
void,glBlitNamedFramebuffer,GLuint,readFramebuffer,GLuint,drawFramebuffer,GLint,srcX0,GLint,srcY0,GLint,srcX1,GLint,srcY1,GLint,dstX0,GLint,dstY0,GLint,dstX1,GLint,dstY1,GLbitfield,mask,GLenum,filter
void,glShaderBinary,GLsizei,count,const GLuint*,shaders,GLenum,binaryformat,const void*,binary,GLsizei,length
void,glColor4ub,GLubyte,red,GLubyte,green,GLubyte,blue,GLubyte,alpha
GLboolean,glIsProgram,GLuint,program
void,glGetPointerv,GLenum,pname,GLvoid**,params
void,glVertexAttribL1dv,GLuint,index,const GLdouble*,v
void,glColor4dv,const GLdouble*,v
void,glGetTransformFeedbackVarying,GLuint,program,GLuint,index,GLsizei,bufSize,GLsizei*,length,GLsizei*,size,GLenum*,type,GLchar*,name
void,glProgramUniform1uiv,GLuint,program,GLint,location,GLsizei,count,const GLuint*,value
void,glGetFramebufferAttachmentParameteriv,GLenum,target,GLenum,attachment,GLenum,pname,GLint*,params
void,glClearBufferfi,GLenum,buffer,GLint,drawbuffer,GLfloat,depth,GLint,stencil
void,glCompressedTexImage2D,GLenum,target,GLint,level,GLenum,internalformat,GLsizei,width,GLsizei,height,GLint,border,GLsizei,imageSize,const GLvoid*,data
void,glAttachShader,GLuint,program,GLuint,shader
void,glGetProgramInterfaceiv,GLuint,program,GLenum,programInterface,GLenum,pname,GLint*,params
void,glVertexPointer,GLint,size,GLenum,type,GLsizei,stride,const GLvoid*,ptr
void,glGetUniformuiv,GLuint,program,GLint,location,GLuint*,params
void,glGetColorTable,GLenum,target,GLenum,format,GLenum,type,GLvoid*,table
void,glGetQueryObjectui64v,GLuint,id,GLenum,pname,GLuint64*,params
void,glGetProgramPipelineiv,GLuint,pipeline,GLenum,pname,GLint*,params
void,glColor4bv,const GLbyte*,v
void,glCompressedTexImage3D,GLenum,target,GLint,level,GLenum,internalformat,GLsizei,width,GLsizei,height,GLsizei,depth,GLint,border,GLsizei,imageSize,const GLvoid*,data
void,glFogf,GLenum,pname,GLfloat,param
void,glVertex4s,GLshort,x,GLshort,y,GLshort,z,GLshort,w
void,glMap1f,GLenum,target,GLfloat,u1,GLfloat,u2,GLint,stride,GLint,order,const GLfloat*,points
void,glTexCoord3sv,const GLshort*,v
void,glPushClientAttrib,GLbitfield,mask
void,glVertex2s,GLshort,x,GLshort,y
void,glDeleteShader,GLuint,shader
void,glIndexPointer,GLenum,type,GLsizei,stride,const GLvoid*,ptr
void,glMinSampleShading,GLfloat,value
void,glGetTextureParameterIuiv,GLuint,texture,GLenum,pname,GLuint*,params
void,glUniformMatrix4fv,GLint,location,GLsizei,count,GLboolean,transpose,const GLfloat*,value
void,glPointParameteri,GLenum,pname,GLint,param
void,glMultiTexCoord4dv,GLenum,target,const GLdouble*,v
void,glGetProgramiv,GLuint,program,GLenum,pname,GLint*,params
GLuint,glGetUniformBlockIndex,GLuint,program,const GLchar*,uniformBlockName
void,glWindowPos3sv,const GLshort*,v
void,glGetVertexArrayIndexediv,GLuint,vaobj,GLuint,index,GLenum,pname,GLint*,param
void,glVertexAttribL1d,GLuint,index,GLdouble,x
void,glTextureBuffer,GLuint,texture,GLenum,internalformat,GLuint,buffer
void,glGetTexParameterIiv,GLenum,target,GLenum,pname,GLint*,params
void,glGetVertexArrayIndexed64iv,GLuint,vaobj,GLuint,index,GLenum,pname,GLint64*,param
void,glGetQueryBufferObjecti64v,GLuint,id,GLuint,buffer,GLenum,pname,GLintptr,offset
void,glColor3iv,const GLint*,v
void,glGetnSeparableFilter,GLenum,target,GLenum,format,GLenum,type,GLsizei,rowBufSize,void*,row,GLsizei,columnBufSize,void*,column,void*,span
void,glUniformMatrix2x4fv,GLint,location,GLsizei,count,GLboolean,transpose,const GLfloat*,value
void,glGetTexLevelParameterfv,GLenum,target,GLint,level,GLenum,pname,GLfloat*,params
void,glNormal3f,GLfloat,nx,GLfloat,ny,GLfloat,nz
void,glCompressedTextureSubImage2D,GLuint,texture,GLint,level,GLint,xoffset,GLint,yoffset,GLsizei,width,GLsizei,height,GLenum,format,GLsizei,imageSize,const void*,data
void,glDispatchCompute,GLuint,num_groups_x,GLuint,num_groups_y,GLuint,num_groups_z
void,glActiveShaderProgram,GLuint,pipeline,GLuint,program
void,glLightModelfv,GLenum,pname,const GLfloat*,params
void,glGetPolygonStipple,GLubyte*,mask
void,glGetnMinmax,GLenum,target,GLboolean,reset,GLenum,format,GLenum,type,GLsizei,bufSize,void*,values
void,glVertexArrayAttribFormat,GLuint,vaobj,GLuint,attribindex,GLint,size,GLenum,type,GLboolean,normalized,GLuint,relativeoffset
void,glFogCoordf,GLfloat,coord
void,glProgramUniformMatrix2fv,GLuint,program,GLint,location,GLsizei,count,GLboolean,transpose,const GLfloat*,value
void,glProgramUniform4uiv,GLuint,program,GLint,location,GLsizei,count,const GLuint*,value
void,glGetVertexAttribfv,GLuint,index,GLenum,pname,GLfloat*,params
void,glLoadMatrixf,const GLfloat*,m
void,glEvalMesh2,GLenum,mode,GLint,i1,GLint,i2,GLint,j1,GLint,j2
void,glColor3ubv,const GLubyte*,v
void,glGetVertexAttribIiv,GLuint,index,GLenum,pname,GLint*,params
void,glGetUniformfv,GLuint,program,GLint,location,GLfloat*,params
void,glSecondaryColor3b,GLbyte,red,GLbyte,green,GLbyte,blue
void,glDisableVertexAttribArray,GLuint,index
void,glTextureStorage2D,GLuint,texture,GLsizei,levels,GLenum,internalformat,GLsizei,width,GLsizei,height
void,glStencilFunc,GLenum,func,GLint,ref,GLuint,mask
void,glVertex2fv,const GLfloat*,v
void,glBindSamplers,GLuint,first,GLsizei,count,const GLuint*,samplers
void,glMultiTexCoord4fv,GLenum,target,const GLfloat*,v
void,glVertexAttrib4d,GLuint,index,GLdouble,x,GLdouble,y,GLdouble,z,GLdouble,w
void,glMultiTexCoord4d,GLenum,target,GLdouble,s,GLdouble,t,GLdouble,r,GLdouble,q
void,glLogicOp,GLenum,opcode
void,glMatrixMode,GLenum,mode
void,glBlendEquationi,GLuint,buf,GLenum,mode
void,glWindowPos2s,GLshort,x,GLshort,y
void,glUniform3d,GLint,location,GLdouble,x,GLdouble,y,GLdouble,z
void,glMultiTexCoordP1ui,GLenum,texture,GLenum,type,GLuint,coords
void,glColor4b,GLbyte,red,GLbyte,green,GLbyte,blue,GLbyte,alpha
void,glRasterPos4s,GLshort,x,GLshort,y,GLshort,z,GLshort,w
void,glSecondaryColor3fv,const GLfloat*,v
void,glRectdv,const GLdouble*,v1,const GLdouble*,v2
void,glGetSamplerParameterfv,GLuint,sampler,GLenum,pname,GLfloat*,params
void,glGetTexImage,GLenum,target,GLint,level,GLenum,format,GLenum,type,GLvoid*,pixels
void,glProgramUniform4f,GLuint,program,GLint,location,GLfloat,v0,GLfloat,v1,GLfloat,v2,GLfloat,v3
void,glClearStencil,GLint,s
void,glDisableVertexArrayAttrib,GLuint,vaobj,GLuint,index
void,glFramebufferTexture1D,GLenum,target,GLenum,attachment,GLenum,textarget,GLuint,texture,GLint,level
void,glRasterPos4sv,const GLshort*,v
void,glNormal3bv,const GLbyte*,v
void,glProgramUniform3dv,GLuint,program,GLint,location,GLsizei,count,const GLdouble*,value
void,glVertexAttrib2fv,GLuint,index,const GLfloat*,v
void,glUniformMatrix4x3dv,GLint,location,GLsizei,count,GLboolean,transpose,const GLdouble*,value
GLuint,glGetProgramResourceIndex,GLuint,program,GLenum,programInterface,const GLchar*,name
void,glResumeTransformFeedback
void,glGetQueryiv,GLenum,target,GLenum,pname,GLint*,params
void,glCompressedTexImage1D,GLenum,target,GLint,level,GLenum,internalformat,GLsizei,width,GLint,border,GLsizei,imageSize,const GLvoid*,data
void,glNormal3dv,const GLdouble*,v
void,glMemoryBarrier,GLbitfield,barriers
void,glCopyConvolutionFilter2D,GLenum,target,GLenum,internalformat,GLint,x,GLint,y,GLsizei,width,GLsizei,height
void,glProgramUniformMatrix3x2fv,GLuint,program,GLint,location,GLsizei,count,GLboolean,transpose,const GLfloat*,value
void,glColor4usv,const GLushort*,v
void,glEvalCoord2f,GLfloat,u,GLfloat,v
void,glColor4s,GLshort,red,GLshort,green,GLshort,blue,GLshort,alpha
void,glGetAttachedShaders,GLuint,program,GLsizei,maxCount,GLsizei*,count,GLuint*,shaders
void,glProgramUniformMatrix3x4dv,GLuint,program,GLint,location,GLsizei,count,GLboolean,transpose,const GLdouble*,value
void,glTexCoordP4uiv,GLenum,type,const GLuint*,coords
void,glMinmax,GLenum,target,GLenum,internalformat,GLboolean,sink
void,glTexParameterIiv,GLenum,target,GLenum,pname,const GLint*,params
void,glGetQueryBufferObjectui64v,GLuint,id,GLuint,buffer,GLenum,pname,GLintptr,offset
void,glTexEnvfv,GLenum,target,GLenum,pname,const GLfloat*,params
void,glGetPixelMapusv,GLenum,map,GLushort*,values
void,glVertexAttribPointer,GLuint,index,GLint,size,GLenum,type,GLboolean,normalized,GLsizei,stride,const void*,pointer
void,glProgramUniform3ui,GLuint,program,GLint,location,GLuint,v0,GLuint,v1,GLuint,v2
void,glPixelTransferi,GLenum,pname,GLint,param
void,glRects,GLshort,x1,GLshort,y1,GLshort,x2,GLshort,y2
void,glConvolutionParameteri,GLenum,target,GLenum,pname,GLint,params
void,glTexCoord3d,GLdouble,s,GLdouble,t,GLdouble,r
void,glUniform3i,GLint,location,GLint,v0,GLint,v1,GLint,v2
void,glSecondaryColor3ui,GLuint,red,GLuint,green,GLuint,blue
void,glLinkProgram,GLuint,program
void,glNamedFramebufferDrawBuffers,GLuint,framebuffer,GLsizei,n,const GLenum*,bufs
void,glCompileShader,GLuint,shader
void,glColor3i,GLint,red,GLint,green,GLint,blue
void,glPrioritizeTextures,GLsizei,n,const GLuint*,textures,const GLclampf*,priorities
void,glFogfv,GLenum,pname,const GLfloat*,params
void,glClearBufferData,GLenum,target,GLenum,internalformat,GLenum,format,GLenum,type,const void*,data
void,glReadPixels,GLint,x,GLint,y,GLsizei,width,GLsizei,height,GLenum,format,GLenum,type,GLvoid*,pixels
GLboolean,glIsTexture,GLuint,texture
void*,glMapNamedBuffer,GLuint,buffer,GLenum,access
void,glIndexi,GLint,c
void,glGetnUniformiv,GLuint,program,GLint,location,GLsizei,bufSize,GLint*,params
void,glProgramUniform4i,GLuint,program,GLint,location,GLint,v0,GLint,v1,GLint,v2,GLint,v3
void,glCompressedTexSubImage2D,GLenum,target,GLint,level,GLint,xoffset,GLint,yoffset,GLsizei,width,GLsizei,height,GLenum,format,GLsizei,imageSize,const GLvoid*,data
void,glShaderSource,GLuint,shader,GLsizei,count,const GLchar*const*,string,const GLint*,length
void,glBegin,GLenum,mode
void,glProgramUniform4fv,GLuint,program,GLint,location,GLsizei,count,const GLfloat*,value
void,glEndTransformFeedback
void,glGetTexGeniv,GLenum,coord,GLenum,pname,GLint*,params
void,glBindVertexBuffer,GLuint,bindingindex,GLuint,buffer,GLintptr,offset,GLsizei,stride
void,glBindBufferBase,GLenum,target,GLuint,index,GLuint,buffer
void,glTextureParameteri,GLuint,texture,GLenum,pname,GLint,param
void,glGenProgramPipelines,GLsizei,n,GLuint*,pipelines
void,glProgramUniform3f,GLuint,program,GLint,location,GLfloat,v0,GLfloat,v1,GLfloat,v2
void,glDrawElementsIndirect,GLenum,mode,GLenum,type,const void*,indirect
void,glWindowPos3i,GLint,x,GLint,y,GLint,z
void,glGetFramebufferParameteriv,GLenum,target,GLenum,pname,GLint*,params
void,glGetNamedFramebufferParameteriv,GLuint,framebuffer,GLenum,pname,GLint*,param
void,glTexCoord1fv,const GLfloat*,v
void,glGetnTexImage,GLenum,target,GLint,level,GLenum,format,GLenum,type,GLsizei,bufSize,void*,pixels
void,glNormal3d,GLdouble,nx,GLdouble,ny,GLdouble,nz
void,glTexCoord1i,GLint,s
void,glVertexAttribL2dv,GLuint,index,const GLdouble*,v
void,glVertexAttribL4d,GLuint,index,GLdouble,x,GLdouble,y,GLdouble,z,GLdouble,w
void,glClearDepthf,GLfloat,d
void,glDisableClientState,GLenum,cap
void,glVertex2iv,const GLint*,v
void,glUniform2fv,GLint,location,GLsizei,count,const GLfloat*,value
void,glUniformMatrix3dv,GLint,location,GLsizei,count,GLboolean,transpose,const GLdouble*,value
void,glRasterPos4fv,const GLfloat*,v
void,glEnableVertexAttribArray,GLuint,index
void,glRectfv,const GLfloat*,v1,const GLfloat*,v2
void,glSamplerParameteriv,GLuint,sampler,GLenum,pname,const GLint*,param
void,glFrontFace,GLenum,mode
void,glTexCoordP3ui,GLenum,type,GLuint,coords
void,glDrawArrays,GLenum,mode,GLint,first,GLsizei,count
void,glSamplerParameteri,GLuint,sampler,GLenum,pname,GLint,param
void,glWindowPos2f,GLfloat,x,GLfloat,y
void,glBufferData,GLenum,target,GLsizeiptr,size,const void*,data,GLenum,usage
void,glVertexArrayBindingDivisor,GLuint,vaobj,GLuint,bindingindex,GLuint,divisor
void,glVertexP2uiv,GLenum,type,const GLuint*,value
void,glProgramUniformMatrix2dv,GLuint,program,GLint,location,GLsizei,count,GLboolean,transpose,const GLdouble*,value
void,glVertex3d,GLdouble,x,GLdouble,y,GLdouble,z
void,glVertexAttrib4iv,GLuint,index,const GLint*,v
void,glProgramUniform2i,GLuint,program,GLint,location,GLint,v0,GLint,v1
void,glVertexAttribI1uiv,GLuint,index,const GLuint*,v
GLuint,glGenLists,GLsizei,range
void,glProgramUniform2fv,GLuint,program,GLint,location,GLsizei,count,const GLfloat*,value
void,glMultiTexCoord2sv,GLenum,target,const GLshort*,v
void,glUniform4fv,GLint,location,GLsizei,count,const GLfloat*,value
void,glColor3bv,const GLbyte*,v
void,glGetTexEnviv,GLenum,target,GLenum,pname,GLint*,params
void,glFramebufferTextureLayer,GLenum,target,GLenum,attachment,GLuint,texture,GLint,level,GLint,layer
void,glRasterPos4i,GLint,x,GLint,y,GLint,z,GLint,w
void,glSecondaryColorPointer,GLint,size,GLenum,type,GLsizei,stride,const void*,pointer
void,glBufferStorage,GLenum,target,GLsizeiptr,size,const void*,data,GLbitfield,flags
void,glSecondaryColor3bv,const GLbyte*,v
void,glTexGendv,GLenum,coord,GLenum,pname,const GLdouble*,params
void,glPushName,GLuint,name
void,glGetMinmaxParameterfv,GLenum,target,GLenum,pname,GLfloat*,params
void,glUniformMatrix2fv,GLint,location,GLsizei,count,GLboolean,transpose,const GLfloat*,value
GLint,glGetSubroutineUniformLocation,GLuint,program,GLenum,shadertype,const GLchar*,name
void,glVertexAttribI4bv,GLuint,index,const GLbyte*,v
void,glVertexAttrib1sv,GLuint,index,const GLshort*,v
void,glNamedBufferData,GLuint,buffer,GLsizeiptr,size,const void*,data,GLenum,usage
void,glTexCoord2f,GLfloat,s,GLfloat,t
void,glMultiTexCoord1fv,GLenum,target,const GLfloat*,v
void,glTexCoord4iv,const GLint*,v
void,glUniformSubroutinesuiv,GLenum,shadertype,GLsizei,count,const GLuint*,indices
GLint,glGetProgramResourceLocationIndex,GLuint,program,GLenum,programInterface,const GLchar*,name
void,glGenerateMipmap,GLenum,target
void,glVertexArrayAttribBinding,GLuint,vaobj,GLuint,attribindex,GLuint,bindingindex
void,glCopyTexSubImage3D,GLenum,target,GLint,level,GLint,xoffset,GLint,yoffset,GLint,zoffset,GLint,x,GLint,y,GLsizei,width,GLsizei,height
GLboolean,glIsVertexArray,GLuint,array
void,glLightModeli,GLenum,pname,GLint,param
void,glSecondaryColor3us,GLushort,red,GLushort,green,GLushort,blue
void,glIndexubv,const GLubyte*,c
void,glVertexAttrib2s,GLuint,index,GLshort,x,GLshort,y
void,glHint,GLenum,target,GLenum,mode
void,glUniformMatrix3x2dv,GLint,location,GLsizei,count,GLboolean,transpose,const GLdouble*,value
void,glBlendColor,GLclampf,red,GLclampf,green,GLclampf,blue,GLclampf,alpha
void,glRasterPos4dv,const GLdouble*,v
GLboolean,glIsSampler,GLuint,sampler
void,glTextureStorage1D,GLuint,texture,GLsizei,levels,GLenum,internalformat,GLsizei,width
void,glVertexAttrib3d,GLuint,index,GLdouble,x,GLdouble,y,GLdouble,z
void,glSecondaryColor3ub,GLubyte,red,GLubyte,green,GLubyte,blue
void,glInvalidateTexImage,GLuint,texture,GLint,level
void,glTexEnviv,GLenum,target,GLenum,pname,const GLint*,params
void,glVertexAttrib2sv,GLuint,index,const GLshort*,v
void,glVertexP4ui,GLenum,type,GLuint,value
void,glRasterPos2dv,const GLdouble*,v
void,glVertexAttrib4f,GLuint,index,GLfloat,x,GLfloat,y,GLfloat,z,GLfloat,w
void,glProvokingVertex,GLenum,mode
void,glTexBuffer,GLenum,target,GLenum,internalformat,GLuint,buffer
void,glSampleCoverage,GLclampf,value,GLboolean,invert
void,glVertexP4uiv,GLenum,type,const GLuint*,value
void,glUniform1i,GLint,location,GLint,v0
void,glColor3f,GLfloat,red,GLfloat,green,GLfloat,blue
void,glTexCoord4fv,const GLfloat*,v
void,glPolygonMode,GLenum,face,GLenum,mode
void,glGetActiveSubroutineName,GLuint,program,GLenum,shadertype,GLuint,index,GLsizei,bufsize,GLsizei*,length,GLchar*,name
void,glCompressedTextureSubImage1D,GLuint,texture,GLint,level,GLint,xoffset,GLsizei,width,GLenum,format,GLsizei,imageSize,const void*,data
void,glTexCoord3iv,const GLint*,v
void,glCopyBufferSubData,GLenum,readTarget,GLenum,writeTarget,GLintptr,readOffset,GLintptr,writeOffset,GLsizeiptr,size
void,glColor4iv,const GLint*,v
void,glGenTransformFeedbacks,GLsizei,n,GLuint*,ids
void,glVertex3s,GLshort,x,GLshort,y,GLshort,z
void,glInvalidateBufferData,GLuint,buffer
void,glRectiv,const GLint*,v1,const GLint*,v2
void,glUniform4ui,GLint,location,GLuint,v0,GLuint,v1,GLuint,v2,GLuint,v3
void,glUniformMatrix3x4fv,GLint,location,GLsizei,count,GLboolean,transpose,const GLfloat*,value
void,glDrawElementsInstanced,GLenum,mode,GLsizei,count,GLenum,type,const void*,indices,GLsizei,instancecount
void,glBindBufferRange,GLenum,target,GLuint,index,GLuint,buffer,GLintptr,offset,GLsizeiptr,size
void,glMultiTexCoord2f,GLenum,target,GLfloat,s,GLfloat,t
void,glUniformMatrix4x3fv,GLint,location,GLsizei,count,GLboolean,transpose,const GLfloat*,value
void,glBlendEquationSeparate,GLenum,modeRGB,GLenum,modeAlpha
void,glCopyTextureSubImage1D,GLuint,texture,GLint,level,GLint,xoffset,GLint,x,GLint,y,GLsizei,width
void,glDepthMask,GLboolean,flag
void,glColor3sv,const GLshort*,v
void,glTransformFeedbackBufferBase,GLuint,xfb,GLuint,index,GLuint,buffer
void,glEnd
void,glGetActiveUniform,GLuint,program,GLuint,index,GLsizei,bufSize,GLsizei*,length,GLint*,size,GLenum*,type,GLchar*,name
void,glGetTexGendv,GLenum,coord,GLenum,pname,GLdouble*,params
void,glMultiTexCoordP4uiv,GLenum,texture,GLenum,type,const GLuint*,coords
void,glQueryCounter,GLuint,id,GLenum,target
void,glMultiTexCoord1d,GLenum,target,GLdouble,s
void,glSecondaryColor3s,GLshort,red,GLshort,green,GLshort,blue
void,glCreateTextures,GLenum,target,GLsizei,n,GLuint*,textures
void,glLightModeliv,GLenum,pname,const GLint*,params
void,glUniform2dv,GLint,location,GLsizei,count,const GLdouble*,value
void,glTexParameterIuiv,GLenum,target,GLenum,pname,const GLuint*,params
void,glRasterPos3i,GLint,x,GLint,y,GLint,z
void,glPopDebugGroup
void,glTexCoordP3uiv,GLenum,type,const GLuint*,coords
void,glGetnHistogram,GLenum,target,GLboolean,reset,GLenum,format,GLenum,type,GLsizei,bufSize,void*,values
void,glCreateRenderbuffers,GLsizei,n,GLuint*,renderbuffers
GLenum,glCheckFramebufferStatus,GLenum,target
void,glRasterPos2fv,const GLfloat*,v
void,glTexCoord2i,GLint,s,GLint,t
void,glProgramUniform2iv,GLuint,program,GLint,location,GLsizei,count,const GLint*,value
void,glUniform4iv,GLint,location,GLsizei,count,const GLint*,value
void,glVertexAttribI3ui,GLuint,index,GLuint,x,GLuint,y,GLuint,z
void,glClearNamedBufferSubData,GLuint,buffer,GLenum,internalformat,GLintptr,offset,GLsizeiptr,size,GLenum,format,GLenum,type,const void*,data
void*,glMapNamedBufferRange,GLuint,buffer,GLintptr,offset,GLsizeiptr,length,GLbitfield,access
void,glColor4sv,const GLshort*,v
void,glProgramUniform2d,GLuint,program,GLint,location,GLdouble,v0,GLdouble,v1
void,glTexSubImage2D,GLenum,target,GLint,level,GLint,xoffset,GLint,yoffset,GLsizei,width,GLsizei,height,GLenum,format,GLenum,type,const GLvoid*,pixels
void,glCopyPixels,GLint,x,GLint,y,GLsizei,width,GLsizei,height,GLenum,type
void,glTextureBufferRange,GLuint,texture,GLenum,internalformat,GLuint,buffer,GLintptr,offset,GLsizeiptr,size
void,glTexCoord3f,GLfloat,s,GLfloat,t,GLfloat,r
void,glCopyTexImage2D,GLenum,target,GLint,level,GLenum,internalformat,GLint,x,GLint,y,GLsizei,width,GLsizei,height,GLint,border
void,glIndexub,GLubyte,c
void,glRasterPos2d,GLdouble,x,GLdouble,y
void,glMultiTexCoord3s,GLenum,target,GLshort,s,GLshort,t,GLshort,r
void,glTexGeni,GLenum,coord,GLenum,pname,GLint,param
void,glVertexAttribI2i,GLuint,index,GLint,x,GLint,y
void,glUniform3fv,GLint,location,GLsizei,count,const GLfloat*,value
void,glVertex3dv,const GLdouble*,v
void,glFlush
void,glUniform4d,GLint,location,GLdouble,x,GLdouble,y,GLdouble,z,GLdouble,w
void,glVertexAttribI4ubv,GLuint,index,const GLubyte*,v
void,glRasterPos3fv,const GLfloat*,v
void,glTexCoord3fv,const GLfloat*,v
void,glMultiTexCoordP3ui,GLenum,texture,GLenum,type,GLuint,coords
void,glProgramUniform4iv,GLuint,program,GLint,location,GLsizei,count,const GLint*,value
void,glVertexAttribDivisor,GLuint,index,GLuint,divisor
void,glVertexAttrib4Nubv,GLuint,index,const GLubyte*,v
void,glBlendEquationSeparatei,GLuint,buf,GLenum,modeRGB,GLenum,modeAlpha
void,glMultiTexCoord4s,GLenum,target,GLshort,s,GLshort,t,GLshort,r,GLshort,q
void,glBindVertexBuffers,GLuint,first,GLsizei,count,const GLuint*,buffers,const GLintptr*,offsets,const GLsizei*,strides
void,glGetColorTableParameteriv,GLenum,target,GLenum,pname,GLint*,params
void,glSecondaryColorP3uiv,GLenum,type,const GLuint*,color
void,glGetMapdv,GLenum,target,GLenum,query,GLdouble*,v
void,glRasterPos3sv,const GLshort*,v
void,glColor3dv,const GLdouble*,v
void,glCallList,GLuint,list
void,glVertexAttrib4usv,GLuint,index,const GLushort*,v
void,glMaterialfv,GLenum,face,GLenum,pname,const GLfloat*,params
void,glAlphaFunc,GLenum,func,GLclampf,ref
GLboolean,glIsFramebuffer,GLuint,framebuffer
void,glUniform1uiv,GLint,location,GLsizei,count,const GLuint*,value
GLuint,glGetDebugMessageLog,GLuint,count,GLsizei,bufSize,GLenum*,sources,GLenum*,types,GLuint*,ids,GLenum*,severities,GLsizei*,lengths,GLchar*,messageLog
void,glVertexAttribL2d,GLuint,index,GLdouble,x,GLdouble,y
void,glProgramUniformMatrix2x3fv,GLuint,program,GLint,location,GLsizei,count,GLboolean,transpose,const GLfloat*,value
void,glVertex2dv,const GLdouble*,v
void,glGetHistogram,GLenum,target,GLboolean,reset,GLenum,format,GLenum,type,GLvoid*,values
void,glMultiDrawArraysIndirect,GLenum,mode,const void*,indirect,GLsizei,drawcount,GLsizei,stride
void,glVertexAttribBinding,GLuint,attribindex,GLuint,bindingindex
void,glDepthFunc,GLenum,func
void,glUniformMatrix3fv,GLint,location,GLsizei,count,GLboolean,transpose,const GLfloat*,value
void,glGetBufferParameteriv,GLenum,target,GLenum,pname,GLint*,params
void,glProgramUniform1f,GLuint,program,GLint,location,GLfloat,v0
void,glNamedRenderbufferStorage,GLuint,renderbuffer,GLenum,internalformat,GLsizei,width,GLsizei,height
void,glColorMask,GLboolean,red,GLboolean,green,GLboolean,blue,GLboolean,alpha
void,glRenderbufferStorage,GLenum,target,GLenum,internalformat,GLsizei,width,GLsizei,height
void,glPopMatrix
void,glVertex3f,GLfloat,x,GLfloat,y,GLfloat,z
void,glTexBufferRange,GLenum,target,GLenum,internalformat,GLuint,buffer,GLintptr,offset,GLsizeiptr,size
void,glUniform4f,GLint,location,GLfloat,v0,GLfloat,v1,GLfloat,v2,GLfloat,v3
GLboolean,glUnmapBuffer,GLenum,target
void*,glMapBuffer,GLenum,target,GLenum,access
void,glEnablei,GLenum,target,GLuint,index
void,glGetVertexAttribiv,GLuint,index,GLenum,pname,GLint*,params
void,glVertexAttrib4fv,GLuint,index,const GLfloat*,v
void,glRasterPos2iv,const GLint*,v
void,glDeleteQueries,GLsizei,n,const GLuint*,ids
void,glRenderbufferStorageMultisample,GLenum,target,GLsizei,samples,GLenum,internalformat,GLsizei,width,GLsizei,height
void,glTexCoord2s,GLshort,s,GLshort,t
void,glCopyNamedBufferSubData,GLuint,readBuffer,GLuint,writeBuffer,GLintptr,readOffset,GLintptr,writeOffset,GLsizeiptr,size
void,glGetTextureLevelParameteriv,GLuint,texture,GLint,level,GLenum,pname,GLint*,params
void,glRectd,GLdouble,x1,GLdouble,y1,GLdouble,x2,GLdouble,y2
void,glColorTableParameteriv,GLenum,target,GLenum,pname,const GLint*,params
void,glCopyTexSubImage2D,GLenum,target,GLint,level,GLint,xoffset,GLint,yoffset,GLint,x,GLint,y,GLsizei,width,GLsizei,height
void,glCreateSamplers,GLsizei,n,GLuint*,samplers
void,glGetVertexAttribLdv,GLuint,index,GLenum,pname,GLdouble*,params
void,glCompressedTexSubImage3D,GLenum,target,GLint,level,GLint,xoffset,GLint,yoffset,GLint,zoffset,GLsizei,width,GLsizei,height,GLsizei,depth,GLenum,format,GLsizei,imageSize,const GLvoid*,data
void,glGetSamplerParameteriv,GLuint,sampler,GLenum,pname,GLint*,params
void,glGetNamedFramebufferAttachmentParameteriv,GLuint,framebuffer,GLenum,attachment,GLenum,pname,GLint*,params
GLsync,glFenceSync,GLenum,condition,GLbitfield,flags
void,glBindProgramPipeline,GLuint,pipeline
void,glColorTableParameterfv,GLenum,target,GLenum,pname,const GLfloat*,params
void,glPointParameterfv,GLenum,pname,const GLfloat*,params
void,glGetHistogramParameterfv,GLenum,target,GLenum,pname,GLfloat*,params
void,glCopyColorTable,GLenum,target,GLenum,internalformat,GLint,x,GLint,y,GLsizei,width
void,glCreateTransformFeedbacks,GLsizei,n,GLuint*,ids
void,glUniformMatrix4x2dv,GLint,location,GLsizei,count,GLboolean,transpose,const GLdouble*,value
void,glTexImage2D,GLenum,target,GLint,level,GLint,internalFormat,GLsizei,width,GLsizei,height,GLint,border,GLenum,format,GLenum,type,const GLvoid*,pixels
void,glTexCoord3dv,const GLdouble*,v
void,glClipPlane,GLenum,plane,const GLdouble*,equation
void,glVertexAttrib3s,GLuint,index,GLshort,x,GLshort,y,GLshort,z
void,glPassThrough,GLfloat,token
void,glClearColor,GLclampf,red,GLclampf,green,GLclampf,blue,GLclampf,alpha
void,glClearBufferiv,GLenum,buffer,GLint,drawbuffer,const GLint*,value
void,glEdgeFlagv,const GLboolean*,flag
void,glUniform4uiv,GLint,location,GLsizei,count,const GLuint*,value
GLuint,glCreateShaderProgramv,GLenum,type,GLsizei,count,const GLchar*const*,strings
void,glGetPixelMapuiv,GLenum,map,GLuint*,values
void,glTextureParameterIuiv,GLuint,texture,GLenum,pname,const GLuint*,params
void,glUniform2ui,GLint,location,GLuint,v0,GLuint,v1
void,glVertex3i,GLint,x,GLint,y,GLint,z
void,glMultiTexCoord2s,GLenum,target,GLshort,s,GLshort,t
void,glPolygonStipple,const GLubyte*,mask
void,glProgramUniformMatrix3x2dv,GLuint,program,GLint,location,GLsizei,count,GLboolean,transpose,const GLdouble*,value
void,glLightf,GLenum,light,GLenum,pname,GLfloat,param
void,glTexCoord3s,GLshort,s,GLshort,t,GLshort,r
void,glGetUniformSubroutineuiv,GLenum,shadertype,GLint,location,GLuint*,params
GLboolean,glIsBuffer,GLuint,buffer
void,glDeleteRenderbuffers,GLsizei,n,const GLuint*,renderbuffers
void,glGetMaterialiv,GLenum,face,GLenum,pname,GLint*,params
void,glMultiDrawElementsIndirect,GLenum,mode,GLenum,type,const void*,indirect,GLsizei,drawcount,GLsizei,stride
void,glConvolutionFilter1D,GLenum,target,GLenum,internalformat,GLsizei,width,GLenum,format,GLenum,type,const GLvoid*,image
void,glVertexAttrib1s,GLuint,index,GLshort,x
void,glProgramUniform4d,GLuint,program,GLint,location,GLdouble,v0,GLdouble,v1,GLdouble,v2,GLdouble,v3
void,glInvalidateFramebuffer,GLenum,target,GLsizei,numAttachments,const GLenum*,attachments
void,glGetActiveUniformBlockiv,GLuint,program,GLuint,uniformBlockIndex,GLenum,pname,GLint*,params
void,glLoadName,GLuint,name
void,glNormal3fv,const GLfloat*,v
void,glGetInteger64v,GLenum,pname,GLint64*,data
void,glProgramUniformMatrix4x2fv,GLuint,program,GLint,location,GLsizei,count,GLboolean,transpose,const GLfloat*,value
void,glViewport,GLint,x,GLint,y,GLsizei,width,GLsizei,height
void,glCopyConvolutionFilter1D,GLenum,target,GLenum,internalformat,GLint,x,GLint,y,GLsizei,width
void,glNormalP3uiv,GLenum,type,const GLuint*,coords
void,glVertexAttribL3dv,GLuint,index,const GLdouble*,v
void,glProgramUniformMatrix3fv,GLuint,program,GLint,location,GLsizei,count,GLboolean,transpose,const GLfloat*,value
void,glPolygonOffset,GLfloat,factor,GLfloat,units
void,glTexImage1D,GLenum,target,GLint,level,GLint,internalFormat,GLsizei,width,GLint,border,GLenum,format,GLenum,type,const GLvoid*,pixels
void,glUniformMatrix3x2fv,GLint,location,GLsizei,count,GLboolean,transpose,const GLfloat*,value
void,glNamedFramebufferTextureLayer,GLuint,framebuffer,GLenum,attachment,GLuint,texture,GLint,level,GLint,layer
void,glVertexAttrib3sv,GLuint,index,const GLshort*,v
void,glTexCoord1iv,const GLint*,v
void,glRasterPos4iv,const GLint*,v
void,glGetUniformdv,GLuint,program,GLint,location,GLdouble*,params
void,glGetCompressedTextureImage,GLuint,texture,GLint,level,GLsizei,bufSize,void*,pixels
void,glTexImage3DMultisample,GLenum,target,GLsizei,samples,GLenum,internalformat,GLsizei,width,GLsizei,height,GLsizei,depth,GLboolean,fixedsamplelocations
void,glTranslatef,GLfloat,x,GLfloat,y,GLfloat,z
void,glBlendFunci,GLuint,buf,GLenum,src,GLenum,dst
void,glGetVertexAttribIuiv,GLuint,index,GLenum,pname,GLuint*,params
void,glMultiTexCoord4iv,GLenum,target,const GLint*,v
void,glGetNamedBufferParameteri64v,GLuint,buffer,GLenum,pname,GLint64*,params
void,glMultiTexCoord3i,GLenum,target,GLint,s,GLint,t,GLint,r
void,glTextureStorage3D,GLuint,texture,GLsizei,levels,GLenum,internalformat,GLsizei,width,GLsizei,height,GLsizei,depth
void,glGetnPixelMapfv,GLenum,map,GLsizei,bufSize,GLfloat*,values
void,glGetProgramResourceName,GLuint,program,GLenum,programInterface,GLuint,index,GLsizei,bufSize,GLsizei*,length,GLchar*,name
void,glInvalidateNamedFramebufferSubData,GLuint,framebuffer,GLsizei,numAttachments,const GLenum*,attachments,GLint,x,GLint,y,GLsizei,width,GLsizei,height
void,glColorMaterial,GLenum,face,GLenum,mode
void,glGetNamedBufferSubData,GLuint,buffer,GLintptr,offset,GLsizeiptr,size,void*,data
void,glTexCoord4i,GLint,s,GLint,t,GLint,r,GLint,q
GLboolean,glIsEnabledi,GLenum,target,GLuint,index
void,glVertexBindingDivisor,GLuint,bindingindex,GLuint,divisor
void,glEdgeFlagPointer,GLsizei,stride,const GLvoid*,ptr
void,glCreateProgramPipelines,GLsizei,n,GLuint*,pipelines
void,glVertexAttrib4dv,GLuint,index,const GLdouble*,v
void,glVertexP3uiv,GLenum,type,const GLuint*,value
void,glNormal3b,GLbyte,nx,GLbyte,ny,GLbyte,nz
void,glGetIntegerv,GLenum,pname,GLint*,params
void,glUniform3f,GLint,location,GLfloat,v0,GLfloat,v1,GLfloat,v2
void,glBitmap,GLsizei,width,GLsizei,height,GLfloat,xorig,GLfloat,yorig,GLfloat,xmove,GLfloat,ymove,const GLubyte*,bitmap
void,glVertex2i,GLint,x,GLint,y
void,glVertexAttrib4s,GLuint,index,GLshort,x,GLshort,y,GLshort,z,GLshort,w
void,glVertex2d,GLdouble,x,GLdouble,y
void,glConvolutionParameterfv,GLenum,target,GLenum,pname,const GLfloat*,params
GLboolean,glIsProgramPipeline,GLuint,pipeline
void,glVertexAttribP3uiv,GLuint,index,GLenum,type,GLboolean,normalized,const GLuint*,value
void,glGetnPolygonStipple,GLsizei,bufSize,GLubyte*,pattern
void,glBindVertexArray,GLuint,array
void,glTexCoord1f,GLfloat,s
void,glInvalidateSubFramebuffer,GLenum,target,GLsizei,numAttachments,const GLenum*,attachments,GLint,x,GLint,y,GLsizei,width,GLsizei,height
void,glRasterPos3f,GLfloat,x,GLfloat,y,GLfloat,z
void,glMultiTexCoord1sv,GLenum,target,const GLshort*,v
void,glEndQueryIndexed,GLenum,target,GLuint,index
void,glVertexAttribI3iv,GLuint,index,const GLint*,v
void,glNormalP3ui,GLenum,type,GLuint,coords
void,glTexStorage3DMultisample,GLenum,target,GLsizei,samples,GLenum,internalformat,GLsizei,width,GLsizei,height,GLsizei,depth,GLboolean,fixedsamplelocations
void,glGetTransformFeedbackiv,GLuint,xfb,GLenum,pname,GLint*,param
void,glTexCoordP1uiv,GLenum,type,const GLuint*,coords
void,glEndList
void,glGetnConvolutionFilter,GLenum,target,GLenum,format,GLenum,type,GLsizei,bufSize,void*,image
void,glTexCoord2fv,const GLfloat*,v
void,glSecondaryColor3sv,const GLshort*,v
void,glPointParameterf,GLenum,pname,GLfloat,param
void,glWindowPos3s,GLshort,x,GLshort,y,GLshort,z
void,glMapGrid1d,GLint,un,GLdouble,u1,GLdouble,u2
void,glVertex2f,GLfloat,x,GLfloat,y
void,glProgramUniform2f,GLuint,program,GLint,location,GLfloat,v0,GLfloat,v1
void,glProgramUniform1dv,GLuint,program,GLint,location,GLsizei,count,const GLdouble*,value
void,glCreateFramebuffers,GLsizei,n,GLuint*,framebuffers
void,glTextureParameterf,GLuint,texture,GLenum,pname,GLfloat,param
void,glGetTransformFeedbacki64_v,GLuint,xfb,GLenum,pname,GLuint,index,GLint64*,param
void,glProgramBinary,GLuint,program,GLenum,binaryFormat,const void*,binary,GLsizei,length
void,glVertexAttribFormat,GLuint,attribindex,GLint,size,GLenum,type,GLboolean,normalized,GLuint,relativeoffset
GLboolean,glIsQuery,GLuint,id
void,glDrawElementsInstancedBaseVertex,GLenum,mode,GLsizei,count,GLenum,type,const void*,indices,GLsizei,instancecount,GLint,basevertex
void,glOrtho,GLdouble,left,GLdouble,right,GLdouble,bottom,GLdouble,top,GLdouble,near_val,GLdouble,far_val
void,glUniformMatrix2dv,GLint,location,GLsizei,count,GLboolean,transpose,const GLdouble*,value
void,glTranslated,GLdouble,x,GLdouble,y,GLdouble,z
void,glDrawElements,GLenum,mode,GLsizei,count,GLenum,type,const GLvoid*,indices
void,glCopyTextureSubImage2D,GLuint,texture,GLint,level,GLint,xoffset,GLint,yoffset,GLint,x,GLint,y,GLsizei,width,GLsizei,height
void,glGetLightfv,GLenum,light,GLenum,pname,GLfloat*,params
void,glGetTexParameterIuiv,GLenum,target,GLenum,pname,GLuint*,params
void,glUseProgramStages,GLuint,pipeline,GLbitfield,stages,GLuint,program
void,glVertexAttrib2d,GLuint,index,GLdouble,x,GLdouble,y
void,glFogCoordPointer,GLenum,type,GLsizei,stride,const void*,pointer
void,glGenTextures,GLsizei,n,GLuint*,textures
void,glGetHistogramParameteriv,GLenum,target,GLenum,pname,GLint*,params
void,glDepthRangeArrayv,GLuint,first,GLsizei,count,const GLdouble*,v
void,glColor4i,GLint,red,GLint,green,GLint,blue,GLint,alpha
void,glGetFloatv,GLenum,pname,GLfloat*,params
void,glTexStorage2DMultisample,GLenum,target,GLsizei,samples,GLenum,internalformat,GLsizei,width,GLsizei,height,GLboolean,fixedsamplelocations
GLint,glGetAttribLocation,GLuint,program,const GLchar*,name
void,glMapGrid1f,GLint,un,GLfloat,u1,GLfloat,u2
void,glColor3ui,GLuint,red,GLuint,green,GLuint,blue
void,glBlendColor,GLfloat,red,GLfloat,green,GLfloat,blue,GLfloat,alpha
GLint,glGetUniformLocation,GLuint,program,const GLchar*,name
void,glUniform1f,GLint,location,GLfloat,v0
GLint,glGetFragDataLocation,GLuint,program,const GLchar*,name
void,glLoadTransposeMatrixd,const GLdouble,m[16]
void,glSecondaryColorP3ui,GLenum,type,GLuint,color
void,glGetDoublei_v,GLenum,target,GLuint,index,GLdouble*,data
void,glVertex4f,GLfloat,x,GLfloat,y,GLfloat,z,GLfloat,w
void,glPrimitiveRestartIndex,GLuint,index
void,glUniform3uiv,GLint,location,GLsizei,count,const GLuint*,value
GLboolean,glIsSync,GLsync,sync
void,glRasterPos3iv,const GLint*,v
void,glSamplerParameterIiv,GLuint,sampler,GLenum,pname,const GLint*,param
void,glSecondaryColor3f,GLfloat,red,GLfloat,green,GLfloat,blue
void,glProgramUniformMatrix4x3fv,GLuint,program,GLint,location,GLsizei,count,GLboolean,transpose,const GLfloat*,value
void,glDeleteProgramPipelines,GLsizei,n,const GLuint*,pipelines
void,glEndConditionalRender
void,glVertexAttribI3uiv,GLuint,index,const GLuint*,v
void,glVertexAttribI4uiv,GLuint,index,const GLuint*,v
void,glTexEnvi,GLenum,target,GLenum,pname,GLint,param
void,glTexCoord3i,GLint,s,GLint,t,GLint,r
void,glSecondaryColor3iv,const GLint*,v
void,glUniform1d,GLint,location,GLdouble,x
void,glEvalCoord1fv,const GLfloat*,u
void,glInvalidateTexSubImage,GLuint,texture,GLint,level,GLint,xoffset,GLint,yoffset,GLint,zoffset,GLsizei,width,GLsizei,height,GLsizei,depth
void,glNamedBufferSubData,GLuint,buffer,GLintptr,offset,GLsizeiptr,size,const void*,data
void,glGetActiveUniformName,GLuint,program,GLuint,uniformIndex,GLsizei,bufSize,GLsizei*,length,GLchar*,uniformName
void,glWindowPos3dv,const GLdouble*,v
void,glTexParameteri,GLenum,target,GLenum,pname,GLint,param
void,glCompressedTexSubImage1D,GLenum,target,GLint,level,GLint,xoffset,GLsizei,width,GLenum,format,GLsizei,imageSize,const GLvoid*,data
void,glTexCoord2d,GLdouble,s,GLdouble,t
void,glProgramUniform1d,GLuint,program,GLint,location,GLdouble,v0
void,glBlitFramebuffer,GLint,srcX0,GLint,srcY0,GLint,srcX1,GLint,srcY1,GLint,dstX0,GLint,dstY0,GLint,dstX1,GLint,dstY1,GLbitfield,mask,GLenum,filter
void,glBeginQueryIndexed,GLenum,target,GLuint,index,GLuint,id
void,glStencilOp,GLenum,fail,GLenum,zfail,GLenum,zpass
void,glColor3ub,GLubyte,red,GLubyte,green,GLubyte,blue
void,glColorSubTable,GLenum,target,GLsizei,start,GLsizei,count,GLenum,format,GLenum,type,const GLvoid*,data
void,glClearAccum,GLfloat,red,GLfloat,green,GLfloat,blue,GLfloat,alpha
void,glMultiTexCoord3f,GLenum,target,GLfloat,s,GLfloat,t,GLfloat,r
void,glDrawTransformFeedbackInstanced,GLenum,mode,GLuint,id,GLsizei,instancecount
void,glVertexAttrib1fv,GLuint,index,const GLfloat*,v
void,glVertexAttribP4uiv,GLuint,index,GLenum,type,GLboolean,normalized,const GLuint*,value
void,glDeleteProgram,GLuint,program
void,glProgramParameteri,GLuint,program,GLenum,pname,GLint,value
void,glDrawRangeElementsBaseVertex,GLenum,mode,GLuint,start,GLuint,end,GLsizei,count,GLenum,type,const void*,indices,GLint,basevertex
void,glMultiTexCoord2dv,GLenum,target,const GLdouble*,v
void,glGetBooleanv,GLenum,pname,GLboolean*,params
void,glBindBuffer,GLenum,target,GLuint,buffer
void,glEvalCoord2dv,const GLdouble*,u
void,glGetBooleani_v,GLenum,target,GLuint,index,GLboolean*,data
void,glEvalMesh1,GLenum,mode,GLint,i1,GLint,i2
void,glColor4ubv,const GLubyte*,v
void,glTexParameterfv,GLenum,target,GLenum,pname,const GLfloat*,params
void,glNamedFramebufferTexture,GLuint,framebuffer,GLenum,attachment,GLuint,texture,GLint,level
void,glSecondaryColor3usv,const GLushort*,v
void,glScissorArrayv,GLuint,first,GLsizei,count,const GLint*,v
void,glVertexAttribI4i,GLuint,index,GLint,x,GLint,y,GLint,z,GLint,w
void,glGetActiveSubroutineUniformName,GLuint,program,GLenum,shadertype,GLuint,index,GLsizei,bufsize,GLsizei*,length,GLchar*,name
void,glProgramUniform4dv,GLuint,program,GLint,location,GLsizei,count,const GLdouble*,value
void,glGetCompressedTexImage,GLenum,target,GLint,lod,GLvoid*,img
void,glResetHistogram,GLenum,target
void,glFogCoordd,GLdouble,coord
void,glClearDepth,GLclampd,depth
