#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"


class VertexArray
{
public:
	VertexArray();
	
	~VertexArray();

private:
	unsigned int m_RendererID;


public:
	void addBuffer(const VertexBuffer vb, const VertexBufferLayout& layout);
	void Bind()const;
	void unBind()const;
};






#endif // !VERTEX_ARRAY_H

