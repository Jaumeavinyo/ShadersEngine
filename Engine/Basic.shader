#shader vertex 
#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoord;

out vTexCoord;

void main() {
	vTexCoord = aTexCoord;
	gl_position = vec4(aPosition, 1.0);

}


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 vTexCoord;

uniform sampler2D uTexture;

layout(location = 0) out vec4 oColor;

void main() {
	oColor = texture(uTexture, vTexCoord);
}