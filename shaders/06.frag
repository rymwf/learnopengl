#version 450

layout(location = 0) out vec4 outColor;

layout(location = 0) in VS_OUT { 
	vec3 color;
	vec2 texCoord;
}fs_in;

layout(binding=1) uniform sampler2D tex;
layout(binding=2) uniform sampler2DArray texarr;

void main() {
   //outColor = vec4(fs_in.color, 1);
   //outColor = vec4(fs_in.texCoord,0,h1);
   //outColor = texture(tex,fs_in.texCoord);
    if(gl_FragCoord.x>400)
		outColor=textureLod(texarr,vec3(fs_in.texCoord,0),5);
	else
		outColor=textureLod(tex,fs_in.texCoord,3);


 //  outColor =  (gl_FragCoord.x>300)?textureLod(texarr,vec3(fs_in.texCoord,0),3):textureLod(tex,fs_in.texCoord,3);
}
