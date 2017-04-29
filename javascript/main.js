function asd(a){
  if(a)return {
    gl: function(){
      console.log("asdf");
    },
      vk: 42            
  };
  else return {
    gg:88
  }
}

b =asd();

var canvas;

canvas = document.getElementById("glcanvas");
var gl;
gl = canvas.getContext("experimental-webgl");
