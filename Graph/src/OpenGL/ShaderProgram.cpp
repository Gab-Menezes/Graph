#include <pch.h>
#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(const std::string & filepath)
{
	ShaderProgramSource source = ParseShader(filepath);
	m_ShaderProgram = CreateProgram(source.VertexSource, source.FragmentSource, source.GeometrySource);
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(m_ShaderProgram);
}

void ShaderProgram::Bind() const
{
	glUseProgram(m_ShaderProgram);
}

void ShaderProgram::Unbind() const
{
	glUseProgram(0);
}

//-------------------UNIFORMS---------------------------
int ShaderProgram::GetUniformLocation(const std::string & name)
{
	int location = glGetUniformLocation(m_ShaderProgram, name.c_str());
	if (location == -1)
	{
		std::cout << "Failed to get the Uniforme Location: " << name << " doesn't exist" << std::endl;
	}
	return location;
}

//Vectors
//INT
void ShaderProgram::setUniform1i(const std::string & name, int value)
{
	glUniform1i(GetUniformLocation(name), value);
}
void ShaderProgram::setUniform2i(const std::string& name, int v0, int v1)
{
	glUniform2i(GetUniformLocation(name), v0, v1);
}
void ShaderProgram::setUniform3i(const std::string& name, int v0, int v1, int v2)
{
	glUniform3i(GetUniformLocation(name), v0, v1, v2);
}
void ShaderProgram::setUniform4i(const std::string& name, int v0, int v1, int v2, int v3)
{
	glUniform4i(GetUniformLocation(name), v0, v1, v2, v3);
}

//FLOAT
void ShaderProgram::setUniform1f(const std::string & name, float value)
{
	glUniform1f(GetUniformLocation(name), value);
}
void ShaderProgram::setUniform2f(const std::string & name, float v0, float v1)
{
	glUniform2f(GetUniformLocation(name), v0, v1);
}

void ShaderProgram::setUniform3f(const std::string & name, float v0, float v1, float v2)
{
	glUniform3f(GetUniformLocation(name), v0, v1, v2);
}
void ShaderProgram::setUniform3f(const std::string & name, float v[3])
{
	glUniform3f(GetUniformLocation(name), v[0], v[1], v[2]);
}
void ShaderProgram::setUniform3f(const std::string & name, const glm::vec3 & vec3)
{
	glUniform3fv(GetUniformLocation(name), 1, glm::value_ptr(vec3));
}

void ShaderProgram::setUniform4f(const std::string & name, float v0, float v1, float v2, float v3)
{
	glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}
void ShaderProgram::setUniform4f(const std::string & name, float v[4])
{
	glUniform4f(GetUniformLocation(name), v[0], v[1], v[2], v[3]);
}
void ShaderProgram::setUniform4f(const std::string & name, const glm::vec4 & vec4)
{
	glUniform4fv(GetUniformLocation(name), 1, glm::value_ptr(vec4));
}

//Matrix
//MAT4
void ShaderProgram::setUniformMat4f(const std::string& name, const glm::mat4 & matrix)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
}

//----------------------------------------------------

//ShaderProgramSource ShaderProgram::ParseShader(const std::string & filepath)
//{
//	enum class ShaderType
//	{
//		NONE = -1, VERTEX = 0, FRAGMENT = 1
//	};
//	std::ifstream stream(filepath);
//
//	std::string line;
//	std::stringstream ss[2];
//	ShaderType type = ShaderType::NONE;
//
//	while (getline(stream, line))
//	{
//		if (line.find("#shader") != std::string::npos)
//		{
//			if (line.find("vertex") != std::string::npos)
//			{
//				type = ShaderType::VERTEX;
//			}
//			if (line.find("fragment") != std::string::npos)
//			{
//				type = ShaderType::FRAGMENT;
//			}
//		}
//		else
//		{
//			ss[(int)type] << line << '\n';
//		}
//	}
//	return { ss[0].str(), ss[1].str() };
//}

ShaderProgramSource ShaderProgram::ParseShader(const std::string& filepath)
{
	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1, GEOMETRY = 2
	};
	std::ifstream stream(filepath);

	std::string line;
	std::stringstream ss[3];
	ss[2] << "";//geometry
	ShaderType type = ShaderType::NONE;

	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
			if (line.find("geometry") != std::string::npos)
			{
				type = ShaderType::GEOMETRY;
			}
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}
	return { ss[0].str(), ss[1].str(), ss[2].str() };
}

//unsigned int ShaderProgram::CreateProgram(const std::string & vertexShader, const std::string & fragmentShader)
//{
//	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
//	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
//
//	unsigned int shaderProgram = glCreateProgram();
//	glAttachShader(shaderProgram, vs);
//	glAttachShader(shaderProgram, fs);
//	glLinkProgram(shaderProgram);
//	glValidateProgram(shaderProgram);
//
//	glDeleteShader(vs);
//	glDeleteShader(fs);
//	return shaderProgram;
//}

unsigned int ShaderProgram::CreateProgram(const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryShader)
{
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
	unsigned int gs;

	unsigned int shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vs);
	if (!geometryShader.empty())
	{
		gs = CompileShader(GL_GEOMETRY_SHADER, geometryShader);
		glAttachShader(shaderProgram, gs);
	}
	glAttachShader(shaderProgram, fs);

	glLinkProgram(shaderProgram);
	glValidateProgram(shaderProgram);

	glDeleteShader(vs);
	if (!geometryShader.empty())
	{
		glDeleteShader(gs);
	}
	glDeleteShader(fs);
	return shaderProgram;
}

unsigned int ShaderProgram::CompileShader(unsigned int type, const std::string & source)
{
	unsigned int shader = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);

	//checks if the compilation failed
	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		std::cout << "Failed to compile the shader:" << std::endl << infoLog << std::endl;
		return 0;
	}

	return shader;
}