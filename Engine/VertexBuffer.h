#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H




class VertexBuffer
{
public:
	VertexBuffer(const void* data, unsigned int size);
	VertexBuffer() { m_RendererID = 0; };
	~VertexBuffer();

	void bind()const;
	void unbind()const;


private:
	unsigned int m_RendererID;

public: 

	unsigned int getRendererID() { return m_RendererID; }
	
};

#endif VERTEX_BUFFER_H