#shader vertex 
#version 430 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out vec2 vTexCoord;
out vec3 vNormal;

uniform mat4 modelTransform;
uniform mat4 viewTransform;
uniform mat4 projectionTransform;

void main() {
	vTexCoord = aTexCoord;
	vNormal = aNormal;
	gl_Position = projectionTransform * viewTransform * modelTransform * vec4(aPosition, 1.0);
	
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