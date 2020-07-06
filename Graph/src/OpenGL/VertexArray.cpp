#include <pch.h>
#include "VertexArray.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_VAO);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_VAO);
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_VAO);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}

void VertexArray::AddAtt(const VertexBuffer& vb, const VertexBufferAttLayout& layout)
{
	vb.Bind();
	const auto& elements = layout.GetElements();
	//unsigned int offset = 0;
	unsigned int offset = layout.GetOffset();
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		Bind();
		const auto& element = elements[i];
		glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset);
		//glVertexAttribLPointer(i, element.count, GL_FLOAT, layout.GetStride(), (const void*)offset);
		glEnableVertexAttribArray(i);
		offset += element.count * AttElements::GetSizeOfType(element.type);
	}
}
