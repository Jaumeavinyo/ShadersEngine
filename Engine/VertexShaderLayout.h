
#ifndef VERTEX_SHADER_LAYOUT_H
#define VERTEX_SHADER_LAYOUT_H
#include "errorHandler.h"
#include <vector>
#include <glad/glad.h>// i can also uncomend next lines for the use of this api in non opengl suporting systems
/*#define GL_BYTE 0x1400
#define GL_UNSIGNED_BYTE 0x1401
#define GL_SHORT 0x1402
#define GL_UNSIGNED_SHORT 0x1403
#define GL_INT 0x1404
#define GL_UNSIGNED_INT 0x1405
#define GL_FLOAT 0x1406*/

struct VertexShaderElement {
	//location is element location in elements array

	GLsizei length;//int: number of characters insid name
	GLint size;//int: size of attrib variable
	GLenum type;//unsigned int: constant values include GL_FLOAT, GL_FLOAT_VEC2, GL_FLOAT_VEC3, GL_FLOAT_VEC4, GL_INT, GL_INT_VEC2, GL_INT_VEC3, GL_INT_VEC4
    GLchar name;


	void getSizeOfType(GLenum type) {
        switch (type) {
        case GL_FLOAT:
            // Handle float attribute variable
            break;
        case GL_FLOAT_VEC2:
            // Handle vec2 attribute variable
            break;
        case GL_FLOAT_VEC3:
            // Handle vec3 attribute variable
            break;
        case GL_FLOAT_VEC4:
            // Handle vec4 attribute variable
            break;
        case GL_INT:
            // Handle int attribute variable
            break;
        case GL_INT_VEC2:
            // Handle ivec2 attribute variable
            break;
        case GL_INT_VEC3:
            // Handle ivec3 attribute variable
            break;
        case GL_INT_VEC4:
            // Handle ivec4 attribute variable
            break;
        case GL_BOOL:
            // Handle bool attribute variable
            break;
        case GL_BOOL_VEC2:
            // Handle bvec2 attribute variable
            break;
        case GL_BOOL_VEC3:
            // Handle bvec3 attribute variable
            break;
        case GL_BOOL_VEC4:
            // Handle bvec4 attribute variable
            break;
        case GL_FLOAT_MAT2:
            // Handle mat2 attribute variable
            break;
        case GL_FLOAT_MAT3:
            // Handle mat3 attribute variable
            break;
        case GL_FLOAT_MAT4:
            // Handle mat4 attribute variable
            break;
            // Handle other attribute variable types
        default:
            // Handle unknown attribute variable type
            break;
        }
	}
};



class VertexShaderLayout
{
public:
	VertexShaderLayout() {}
	~VertexShaderLayout() {};

private:

	std::vector<VertexShaderElement> Elements;
	

public:

	
	void Push(GLsizei length, GLint size,GLenum type, GLchar name) {
		Elements.push_back({ length, size, type, name });
	}


	inline const std::vector<VertexShaderElement> getElements()const { return Elements; }

};




#endif // !VERTEX_SHADER_LAYOUT_H

