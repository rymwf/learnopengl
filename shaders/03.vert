#version 450
layout(location = 0) in vec2 inPos;
layout(location = 1) in vec3 inColor;
out VS_OUT { vec3 color; }
vs_out;
void main() {
  gl_Position = vec4(inPos, 0, 1);
  vs_out.color = inColor;
}
