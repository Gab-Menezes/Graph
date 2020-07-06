#pragma once
//Includes
#include <pch.h>
#include <Expression.h>

#include <VertexBuffer.h>
#include <VertexArray.h>
#include <VertexBufferAttLayout.h>
#include <ShaderProgram.h>

class Graph
{
public:
	Graph(const std::shared_ptr<Expression>& exp, const glm::vec3& color, const glm::mat4& vp, const std::pair<double, double>& range = { -10.f, 10.f }, const double step = 0.001f, const glm::mat4 & model = glm::mat4(1.f)); //Graph Constructor

	~Graph(); //Graph Destructor

	void Draw() const; //Draws the graph

	//Setters
	void SetExpression(const Expression& exp); //Sets the expression (m_Expression) to a new expression
	void SetExpression(const std::string& strExp); //Sets the string expression of m_Expression to a new string expression

	void SetRange(const std::pair<double, double>& range); //Sets the range (m_Range) of draw and evaluation of the graph to a new range

	void SetModel(const glm::mat4& model); //Sets the model matirx (m_Model) to a new matrix
	void SetVP(const glm::mat4& vp); //Sets the View Projection matrix (m_VP) to a new matrix
	void SetColor(const glm::vec3& color); //Sets the color vector (m_Color) to a new vector
	void SetShow(bool show); //Sets the show state (m_Show) of the graph to a new state

	//Getters
	const std::shared_ptr<Expression>& GetExpression() const; //Returns a shared pointer of the expression (m_Expression)
	glm::vec3 GetColor() const; //Returns the color vector (m_Color)

	bool GetShow() const; //Returns the show state (m_Show) of the graph

	Graph& operator=(const Graph& rhs); //= Operator Overload

private:
	//Member variables
	bool m_Show; //Show state of the graph (false = not showing) (true = showing)

	std::shared_ptr<Expression> m_Expression; //Expression Shared Pointer

	std::vector<double> m_Vertices; //Vertices of the graph (xy vector)
	std::pair<double, double> m_Range; //Range of draw and evaluation of the graph
	double m_Step; //Incrementation step

	std::shared_ptr<ShaderProgram> m_Shader; //Shader Shared Pointer
	std::shared_ptr<VertexBuffer> m_VBO; //Vertex Buffer Shared Pointer
	std::shared_ptr<VertexArray> m_VAO; //Vertex Array Shared Pointer

	glm::vec3 m_Color; //Color vector
	glm::mat4 m_Model; //Model matrix
	glm::mat4 m_VP; //View Projection matrix
};