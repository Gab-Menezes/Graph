#pragma once
#include <pch.h>

//struct ShaderProgramSource
//{
//	std::string VertexSource;
//	std::string FragmentSource;
//};


struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
	std::string GeometrySource = "";
};

class ShaderProgram
{
public:
	ShaderProgram(const std::string& filepath);
	~ShaderProgram();

	void Bind() const;
	void Unbind() const;

	//Vectors
	//INT
	void setUniform1i(const std::string& name, int value);
	void setUniform2i(const std::string& name, int v0, int v1);
	void setUniform3i(const std::string& name, int v0, int v1, int v2);
	void setUniform4i(const std::string& name, int v0, int v1, int v2, int v3);

	//FLOAT
	void setUniform1f(const std::string& name, float value);
	void setUniform2f(const std::string& name, float v0, float v1);

	void setUniform3f(const std::string& name, float v0, float v1, float v2);
	void setUniform3f(const std::string& name, float v[3]);
	void setUniform3f(const std::string& name, const glm::vec3& vec3);

	void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void setUniform4f(const std::string& name, const glm::vec4& vec4);
	void setUniform4f(const std::string& name, float v[4]);

	//Matrix
	//MAT4
	void setUniformMat4f(const std::string& name, const glm::mat4& matrix);

	//Materials
	//MATERIAL
private:
	std::string m_FilePath;
	unsigned int m_ShaderProgram;

	ShaderProgramSource ParseShader(const std::string& filepath);
	int GetUniformLocation(const std::string& name);

	//unsigned int CreateProgram(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int CreateProgram(const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryShader);
	unsigned int CompileShader(unsigned int type, const std::string& source);
};