#version 410 core

in vec2 UV;

out vec4 color;

uniform sampler2D renderedTexture;
uniform float time;

void main(){
    color = vec4(texture( renderedTexture, UV ).rgb, 1.0) ;
    //color = vec4(1.0, 0.0, 0.0, 1.0);
}
