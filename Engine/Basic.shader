#shader vertex 
#version 430 core

layout(location = 0) in vec3 aPosition;
layout(location = 2) in vec2 aTexCoord;

out vec2 vTexCoord;

void main() {
	vTexCoord = aTexCoord;
	gl_Position = vec4(aPosition, 1.0);
	gl_Position.z = -gl_Position.z;
}


#shader fragment
#version 430 core

out vec4 color;

in vec2 vTexCoord;

uniform sampler2D uTexture;



void main() {
	color = texture(uTexture, vTexCoord);//vec4(1.0,1.0,0.5,1.0);//
}