#version 440

layout(location=0)out vec4 fColor;

in vec2 gCoord;
in vec3 gWorldPosition;
in vec3 gA;
in vec3 gB;

uniform bool alphaOn=true;

float plasmaDistance(vec3 A,vec3 B,vec3 P,float ra,float rb){
  //X(t)=A+t*(B-A)
  //(X(t)-P)^2
  //(A-P)^2+(B-A)*(A-P)*2*t+(B-A)^2*t^2=0
  //2*(B-A)*(A-P)+2*t*(B-A)^2=0
  //t = -(B-A)*(A-P)/(B-A)^2
  float t=-dot(B-A,A-P)/dot(B-A,B-A);
  if(t<0)return length(A-P)/ra;
  if(t>1)return length(B-P)/rb;
  return length(A+t*(B-A)-P)/(ra+t*(rb-ra));
}

void main(){
  float alpha=pow(1-abs(gCoord.x-.5),10);
  if(alphaOn)
    fColor=vec4(alpha);
  else{
    vec3 A=vec3(0);
    vec3 B=vec3(0,1,0);
    float d=float(clamp(1-plasmaDistance(gA,gB,gWorldPosition,1,1),0,1));

    fColor=vec4(pow(d,40));
    //fColor=vec4(1);
  }
}
