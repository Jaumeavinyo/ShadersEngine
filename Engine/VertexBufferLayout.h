#ifndef VERTEX_BUFFER_LAYOUT_H
#define VERTEX_BUFFER_LAYOUT_H
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

struct VertexBufferElement {
	unsigned int type;
	unsigned int count;
	unsigned char normalized;
	unsigned int location;
	unsigned int elementOffset;
	static unsigned int getSizeOfType(unsigned int type) {
		switch (type)
		{
			case GL_FLOAT:
				return 4;	
			case GL_UNSIGNED_INT:
				return 4;
			case GL_UNSIGNED_BYTE:
				return 1;
		}
		_ASSERT(false);
		return 0;
	}
};



class VertexBufferLayout
{
public:
	VertexBufferLayout() :Stride(0) {}
	~VertexBufferLayout() {};

private:

	std::vector<VertexBufferElement> Elements;
	unsigned int Stride;

public:

	template<typename T>
	void Push(unsigned int count,unsigned int location) {
		static_assert(false);
	}

	template<>
	void Push<float>(unsigned int count, unsigned int location) {
		Elements.push_back({GL_FLOAT,count,GL_FALSE,location});
		Elements[Elements.size()-1u].elementOffset = Stride + count * VertexBufferElement::getSizeOfType(GL_FLOAT);
		Stride += count * VertexBufferElement::getSizeOfType(GL_FLOAT);
	}

	template<>
	void Push<unsigned int>(unsigned int count, unsigned int location) {
		Elements.push_back({ GL_UNSIGNED_INT,count,GL_FALSE,location });
		Elements[Elements.size() - 1u].elementOffset = Stride + count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);
		Stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void Push<unsigned char>(unsigned int count, unsigned int location) {
		Elements.push_back({ GL_UNSIGNED_BYTE,count,GL_TRUE,location });
		Elements[Elements.size() - 1u].elementOffset = Stride + count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE);
		Stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE) ;
	}


	inline const std::vector<VertexBufferElement> getElements()const { return Elements; }
	inline unsigned int getStride()const { return Stride; }

};




#endif // !VERTEX_BUFFER_LAYOUT_H

