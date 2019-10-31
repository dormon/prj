#pragma once

#include <iostream>
#include <memory>
#include <geGL/geGL.h>
#include <geGL/StaticCalls.h>
#include <Vars/Vars.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "util.h"

class QuiltImage{
  public:
    QuiltImage(std::string const&fileName,vars::Vars&vars):vars(vars){
      auto vs = std::make_shared<ge::gl::Shader>(GL_VERTEX_SHADER,
      R".(
      #version 310 es
      #line 19
      out highp vec2 texCoords;
      void main(){
        texCoords = vec2(gl_VertexID&1,gl_VertexID>>1);
        gl_Position = vec4(texCoords*2.f-1.f,0.f,1.f);
      }
      ).");
      auto fs = std::make_shared<ge::gl::Shader>(GL_FRAGMENT_SHADER,
      R".(
      #version 310 es
      #line 32

      in highp vec2 texCoords;
      
      out lowp vec4 fragColor;
      
      // HoloPlay values
      uniform lowp float pitch;
      uniform lowp float tilt;
      uniform lowp float center;
      uniform lowp float invView;
      uniform lowp float subp;
      uniform lowp vec4 tile;
      uniform lowp vec4 viewPortion;
      uniform lowp vec4 aspect;
      
      uniform sampler2D screenTex;
      
      uniform lowp float focus;
      uniform lowp float zoom;
      uniform lowp vec2 pan;

      vec2 texArr(highp vec3 uvz)
      {
          // decide which section to take from based on the z.
          
          lowp float z = floor(uvz.z * 45.f /*tile.z*/);
          lowp float focusMod = focus*(1.f-2.f*clamp(uvz.z,0.f,1.f));
          highp float x = (mod(z, tile.x) + clamp((uvz.x+focusMod)*zoom-zoom*.5f+.5f+pan.x,0.f,1.f) )  * (1.f/5.f) /* / tile.x*/;
          highp float y = (floor(z *(1.f/5.f) /* / tile.x*/) + clamp(uvz.y * zoom-zoom*.5f+.5f+pan.y,0.f,1.f)) * (1.f/9.f) /* / tile.y*/;
          return vec2(x, y) * viewPortion.xy;
      }
      #line 64
      void main()
      {
        lowp float z = (texCoords.x + texCoords.y * tilt) * pitch - center;
        fragColor[0] = texture(screenTex, texArr(vec3(texCoords.xy,1.f-fract(z))))[0];
        z = z + subp*pitch;
        fragColor[1] = texture(screenTex, texArr(vec3(texCoords.xy,1.f-fract(z))))[1];
        z = z + subp*pitch;
        fragColor[2] = texture(screenTex, texArr(vec3(texCoords.xy,1.f-fract(z))))[2];
      }
      ).");
      prg = std::make_shared<ge::gl::Program>(vs,fs);

      /*
      auto tex = std::make_shared<ge::gl::Texture>(GL_TEXTURE_2D,GL_RGB8,1,w,h);
      ge::gl::glBindTexture(GL_TEXTURE_2D,tex->getId());
      ge::gl::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
      */
      ge::gl::glGenTextures(1, &tex);
      ___;

      ge::gl::glBindTexture(GL_TEXTURE_2D, tex);
      ___;
      
      ge::gl::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      ___;
      ge::gl::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      ___;
      
      loadImage(fileName);
      ___;


    }

    void loadImage(std::string const&fileName){
      int w;
      int h;
      int comp;
      ___;
      unsigned char* image = stbi_load(fileName.c_str(), &w, &h, &comp, STBI_rgb);
      ___;

      for(int y=0;y<h/2;++y)
        for(int x=0;x<w;++x)
          for(int c=0;c<comp;++c){
            auto z = image[(y*w+x)*comp+c];
            image[(y*w+x)*comp+c] = image[((h-y-1)*w+x)*comp+c];
            image[((h-y-1)*w+x)*comp+c] = z;
          }
      ___;
      ge::gl::glPixelStorei(GL_UNPACK_ROW_LENGTH,w);
      ge::gl::glPixelStorei(GL_UNPACK_ALIGNMENT ,1);
      ___;
      if(comp == 3)
          ge::gl::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB565, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
      else if(comp == 4)
          ge::gl::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
      ___;
      stbi_image_free(image);
    }

    ~QuiltImage(){
      ___;
      ge::gl::glDeleteTextures(1,&tex);
      ___;
    }
    void draw(){
      ___;
      prg
         ->set1f ("pitch"           ,                vars.getFloat      ("quiltView.pitch"      ))
         ->set1f ("tilt"            ,                vars.getFloat      ("quiltView.tilt"       ))
         ->set1f ("center"          ,                vars.getFloat      ("quiltView.center"     ))
         ->set1f ("subp"            ,                vars.getFloat      ("quiltView.subp"       ))
         ->set4fv("tile"            ,glm::value_ptr(*vars.get<glm::vec4>("quiltView.tile"       )))
         ->set4fv("viewPortion"     ,glm::value_ptr(*vars.get<glm::vec4>("quiltView.viewPortion")))
         ->set1f ("focus"           ,focus                                                        )
         ->set1f ("zoom"            ,zoom                                                         )
         ->set2fv("pan"             ,glm::value_ptr(pan)                                          )
         ->use();
      ___;
      auto const w = 1920*2;
      auto const h = 1080*2;
      auto const ww = w*crop;
      auto const hh = h*crop;
      ge::gl::glEnable(GL_SCISSOR_TEST);
      ___;
      ge::gl::glScissor((w-ww)/2,(h-hh)/2,ww,hh);
      ___;
      ge::gl::glDrawArrays(GL_TRIANGLE_STRIP,0,4);
      ___;
    }

    static void plusInRange(float&v,glm::vec2 const&range,float s){
      v += s;
      if(v > range.y)v = range.y;
    }
    
    static void minusInRange(float&v,glm::vec2 const&range,float s){
      v -= s;
      if(v < range.x)v = range.x;
    }


    void incViewport(){
      plusInRange(crop,cropRange,cropStep);
    }
    void decViewport(){
      minusInRange(crop,cropRange,cropStep);
    }
    void focusPlaneNearer(){
      plusInRange(focus,focusRange,focusStep);
    }
    void focusPlaneFarrer(){
      minusInRange(focus,focusRange,focusStep);
    }
    void zoomIn(){
      minusInRange(zoom,zoomRange,zoomStep);
    }
    void zoomOut(){
      plusInRange(zoom,zoomRange,zoomStep);
    }

    void panLeft(){
      plusInRange(pan.x,xPanRange,panStep);
    }
    void panRight(){
      minusInRange(pan.x,xPanRange,panStep);
    }
    void panDown(){
      plusInRange(pan.y,yPanRange,panStep);
    }
    void panUp(){
      minusInRange(pan.y,yPanRange,panStep);
    }

    void reset(){
      crop = 1.f;
      focus = 0.f;
      zoom = 1.f;
      pan = glm::vec2(0.f);
    }

    float crop = 1.f;
    glm::vec2 const cropRange = glm::vec2(.3f,1.f);
    float const cropStep = 0.05f;

    float focus = 0.f;
    glm::vec2 const focusRange = glm::vec2(-.13f,.13f);
    float const focusStep = 0.004f;

    float zoom = 1.f;
    glm::vec2 const zoomRange = glm::vec2(0.4f,1.f);
    float const zoomStep = 0.01;

    glm::vec2 pan = glm::vec2(0.f);
    glm::vec2 const xPanRange = glm::vec2(-1.f,+1.f);
    glm::vec2 const yPanRange = glm::vec2(-1.f,+1.f);
    float const panStep = 0.01f;
  protected:

    vars::Vars&vars;
    std::shared_ptr<ge::gl::Program>prg;
    GLuint tex;
};

