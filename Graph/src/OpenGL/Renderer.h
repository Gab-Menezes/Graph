#pragma once
#include <pch.h>
#include "ShaderProgram.h"
#include "VertexArray.h"
#include "IndexBuffer.h"

class Renderer
{
public:
	void DrawElements(const ShaderProgram& ShaderProgram, const VertexArray& VAO, const IndexBuffer& EBO) const
	{
		ShaderProgram.Bind();
		VAO.Bind();
		EBO.Bind();

		glDrawElements(GL_TRIANGLES, EBO.GetCount(), GL_UNSIGNED_INT, 0);
		//glDrawElements(GL_LINES, EBO.GetCount(), GL_UNSIGNED_INT, 0);

		ShaderProgram.Unbind();
		VAO.Unbind();
		EBO.Unbind();
	}

	/*void DrawArrays(const ShaderProgram& ShaderProgram, const VertexArray& VAO, unsigned int Offset, unsigned int Count) const
	{
		ShaderProgram.Bind();
		VAO.Bind();

		glDrawArrays(GL_LINE_STRIP, Offset, Count);

		ShaderProgram.Unbind();
		VAO.Unbind();
	}*/

private:
};