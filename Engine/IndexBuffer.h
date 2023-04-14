#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

class IndexBuffer
{
public:
	IndexBuffer(const unsigned int* data,unsigned int count);
	
	~IndexBuffer();

	void bind()const;
	void unbind()const;

private:
	unsigned int m_RendererID;
	unsigned int m_Count;//how many indices

public:
	unsigned int getRendererID() const { return m_RendererID; }
	inline unsigned int getCount() const { return m_Count; }
};



#endif // !INDEX_BUFFER_H
