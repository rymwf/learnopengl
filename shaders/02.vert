#version 450
const vec2 inPos[3] = {{-0.5, -0.5}, {0.5, -0.5}, {-0.5, 0.5}};
void main() { gl_Position = vec4(inPos[gl_VertexID], 0, 1); }
