#pragma once
#include <pch.h>
#include "VertexBuffer.h"
#include "VertexBufferAttLayout.h"

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void Bind() const;
	void Unbind() const;
	void AddAtt(const VertexBuffer& VBO, const VertexBufferAttLayout& layout);
private:
	unsigned int m_VAO;
};