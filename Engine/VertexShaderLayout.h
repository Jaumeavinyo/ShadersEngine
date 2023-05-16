
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
	unsigned int count;

};



class VertexShaderLayout
{
public:
	VertexShaderLayout() {}
	~VertexShaderLayout() {};

private:

	std::vector<VertexShaderElement> Elements;
	

public:

	
	void Push(unsigned int count) {
		Elements.push_back({ count });
	}


	inline const std::vector<VertexShaderElement> getElements()const { return Elements; }

};




#endif // !VERTEX_SHADER_LAYOUT_H

