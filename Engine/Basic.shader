#shader vertex 
#version 430 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;


out vec3 vPosition;
out vec3 vNormal;
out vec2 vTexCoord;


uniform mat4 worldMat;
uniform mat4 worldViewProjection;

void main() {
	vTexCoord = aTexCoord;
	vPosition = vec3(worldMat*vec4(aPosition,1.0));
	vNormal = vec3(worldMat * vec4(aNormal, 0.0));
	gl_Position = worldViewProjection * vec4(aPosition, 1.0);
}


#shader fragment
#version 430 core

out vec4 color;

in vec2 vTexCoord;
in vec3 vNormal;

uniform sampler2D uTexture;



void main() {
	color = texture(uTexture, vTexCoord);//vec4(1.0,1.0,0.5,1.0);//
}