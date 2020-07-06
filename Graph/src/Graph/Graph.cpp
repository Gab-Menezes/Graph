#include <pch.h>
#include <Graph.h>

//Constructor
Graph::Graph(const std::shared_ptr<Expression>& exp, const glm::vec3& color, const glm::mat4& vp, const std::pair<double, double>& range, const double step, const glm::mat4& model) :
	m_Expression(exp), //m_Expression = exp
	m_Color(color), //m_Color = color
	m_Model(model), //m_Model = model
	m_VP(vp), //m_VP = vp
	m_Show(true), //m_Show = true (when created the show state it's always true)
	m_Range(range), //m_Range = range
	m_Step(step) //m_Step = step
{
	m_Vertices = m_Expression->Calculate(m_Range, step); //Calculates the vertices of the graph

	//Opengl
	m_Shader = std::make_shared<ShaderProgram>("res/graph.glsl"); //Creates the Shader
	m_Shader->Bind(); //Binds the Shader
	m_Shader->setUniformMat4f("u_Model", m_Model); //Sets the uniform "u_Model" to m_Model
	m_Shader->setUniformMat4f("u_VP", m_VP); //Sets the uniform "u_VP" to m_VP
	m_Shader->setUniform3f("u_Color", m_Color); //Sets the uniform "u_Color" to m_Color

	m_VBO = std::make_shared<VertexBuffer>(&m_Vertices[0], m_Vertices.size() * sizeof(double)); //Creates the Vertex Buffer
	m_VBO->Bind(); //Binds the Vertex Buffer

	m_VAO = std::make_shared<VertexArray>(); //Creates the Vertex Array
	m_VAO->Bind(); //Binds the Vertex Array
	VertexBufferAttLayout attLayout; //Creates a Attribute Layout
	attLayout.Push<double>(2); //Pushs 2 doubles as read mode
	m_VAO->AddAtt(*m_VBO, attLayout); //Adds the Attribute Layout (attLayout) to the Vertex Array (m_VAO)
}

//Destructor
Graph::~Graph()
{
}

void Graph::Draw() const
{
	m_Shader->Bind(); //Binds the shader
	m_VBO->Bind(); //Binds the vertex buffer
	m_VAO->Bind(); //Binds the vertex array

	glDrawArrays(GL_LINE_STRIP, 0, m_Vertices.size() / 2); //Draw the graph (OpenGL call)

	m_Shader->Unbind(); //Unbinds the shader
	m_VBO->Unbind(); //Unbinds the vertex buffer
	m_VAO->Unbind(); //Unbinds the vertex array
}

void Graph::SetExpression(const Expression & exp)
{
	//WARNING!!

	m_Expression = std::make_shared<Expression>(exp); //Sets the m_Expression to the new expression (exp)
	m_Vertices = m_Expression->Calculate(m_Range, m_Step); //Re-calculates the vertice of the graph

	//Re-does the vertex buffer and vertex array
	m_VBO = std::make_shared<VertexBuffer>(&m_Vertices[0], m_Vertices.size() * sizeof(double));
	m_VBO->Bind();

	m_VAO = std::make_shared<VertexArray>();
	m_VAO->Bind();
	VertexBufferAttLayout attLayout;
	attLayout.Push<double>(2);
	m_VAO->AddAtt(*m_VBO, attLayout);
}

void Graph::SetExpression(const std::string & strExp)
{
	m_Expression->SetExpression(strExp); //Sets the string expression of m_Expression to strExp
	if (m_Expression->IsCompiled()) //Checks if the new expression compiled
	{
		m_Vertices = m_Expression->Calculate(m_Range, m_Step); //Re-calculates the vertice of the graph

		//Re - does the vertex buffer and vertex array
		m_VBO = std::make_shared<VertexBuffer>(&m_Vertices[0], m_Vertices.size() * sizeof(double));
		m_VBO->Bind();

		m_VAO = std::make_shared<VertexArray>();
		m_VAO->Bind();
		VertexBufferAttLayout attLayout;
		attLayout.Push<double>(2);
		m_VAO->AddAtt(*m_VBO, attLayout);
	}
}

void Graph::SetRange(const std::pair<double, double>& range)
{
	m_Range = range; //Sets the graph range to the new range (range)
	unsigned int size = round(log10(abs(m_Range.first) + abs(m_Range.second)));
	if (size == 0)
	{
		std::cout << "oi" << std::endl;
	}
	m_Step = 0.0001 * pow(10, size);
	std::cout << size << std::endl;
	m_Vertices = m_Expression->Calculate(m_Range, m_Step);//Re-calculates the vertice of the graph

	//Re - does the vertex buffer and vertex array
	m_VBO = std::make_shared<VertexBuffer>(&m_Vertices[0], m_Vertices.size() * sizeof(double));
	m_VBO->Bind();

	m_VAO = std::make_shared<VertexArray>();
	m_VAO->Bind();
	VertexBufferAttLayout attLayout;
	attLayout.Push<double>(2);
	m_VAO->AddAtt(*m_VBO, attLayout);
}

void Graph::SetModel(const glm::mat4 & model)
{
	m_Model = model; //Sets the m_Model to the new model matrix (model)
	m_Shader->Bind(); //Binds the Shader
	m_Shader->setUniformMat4f("u_Model", m_Model); //Sets the uniform "u_Model" to the new m_Model
	m_Shader->Unbind(); //Unbinds the Shader
}

void Graph::SetVP(const glm::mat4& vp)
{
	m_VP = vp;
	m_Shader->Bind(); //Binds the Shader
	m_Shader->setUniformMat4f("u_VP", m_VP); //Sets the uniform "u_VP" to m_VP
	m_Shader->Unbind(); //Unbinds the Shader
}

void Graph::SetColor(const glm::vec3 & color)
{
	m_Color = color; //Sets the m_Color to the new color vector (color)
	m_Shader->Bind(); //Binds the Shader
	m_Shader->setUniform3f("u_Color", m_Color); //Sets the uniform "u_Color" to the new m_Color
	m_Shader->Unbind(); //Unbinds the Shader
}

void Graph::SetShow(bool show)
{
	m_Show = show; //Sets m_Show to the new state (show) 
}

const std::shared_ptr<Expression>& Graph::GetExpression() const
{
	return m_Expression; //Retuns a m_Expression Shared Pointer
}

glm::vec3 Graph::GetColor() const
{
	return m_Color; //Returns the current color of the graph
}

bool Graph::GetShow() const
{
	return m_Show; //Returns the currente state of the graph
}

Graph & Graph::operator=(const Graph & rhs)
{
	//Warning!!

	//Make all member variables equals to the rhs member variables
	m_Show = rhs.m_Show;
	m_Expression = rhs.m_Expression;
	m_Vertices = rhs.m_Vertices;
	m_Range = rhs.m_Range;
	m_Step = rhs.m_Step;
	m_Shader = rhs.m_Shader;
	m_VBO = rhs.m_VBO;
	m_VAO = rhs.m_VAO;
	m_Color = rhs.m_Color;
	m_Model = rhs.m_Model;
	m_VP = rhs.m_VP;

	return *this; //Return a dereferenced pointer
}
