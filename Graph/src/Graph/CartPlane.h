#pragma once
//Includes
#include <pch.h>
#include <Graph.h>

#include <VertexBuffer.h>
#include <VertexArray.h>
#include <VertexBufferAttLayout.h>
#include <ShaderProgram.h>
#include <IndexBuffer.h>
#include <Renderer.h>

#include <Camera.h>

extern double BiggestLimit;

class CartPlane
{
public:
	CartPlane(const std::pair<double, double>& range = { -10.f, 10.f }); //CartPlane Constructor
	~CartPlane(); //CartPlane Destructor

	void Draw(); //Draws the cartisian plane, grid and the graphs by calling the Graph::Draw() function
	void ImGui(); //ImGui draw function
	void Update(); //Events

private:
	void GenGrid(); //Generates the grid central points (assembled by the geometry shader)
	
	bool AddGraph(const std::string& strExp, int index, const glm::vec3& color); //Adds a new graph in the given index
	void GraphLogic(int i); //Does the input graph logic

	glm::vec3 GenColor(); //Generates a random color to the new graph

	void SetRange(); //Set the range to the new edges points
	void SetVP(); //Set the View Projection matrix (m_VP) -> calls SetRange()

	//Member Variables

	//Grid variables
	std::vector<float> m_GridVertices; //Central points of the grid
	float m_GridOffset; //Grid offset
	ShaderProgram m_GridShader; //Grid Shader
	std::unique_ptr<VertexBuffer> m_GridVBO; //Grid Vertex Buffer
	VertexArray m_GridVAO; //Grid Vertex Array

	//Axis variables
	double m_AxisVertices[8] = //Axis vertices
	{
		//Left X
		-BiggestLimit, 0.f,
		//Right X
		BiggestLimit, 0.f,
		//Bottom Y
		0.f, -BiggestLimit,
		//Top Y
		0.f, BiggestLimit
	}; 
	ShaderProgram m_AxisShader; //Axis shader
	VertexBuffer m_AxisVBO; //Axis Vertex Buffer
	VertexArray m_AxisVAO; //Axis Vertex Array

	//Graphs Variables
	std::pair<double, double> m_Range; //Current showing range
	std::pair<double, double> m_InitRange; //Initial range (Usually { -10f, 10.f })
	std::vector<Graph> m_Graphs; //Graphs vector

	//Uniforms
	glm::mat4& m_Proj; //Projectio Matrix (alisa -> defined at main.cpp)
	glm::mat4 m_VP; //View Projection Matrix
	Camera m_Camera; //Camera


	//ImGui variables
	std::vector<std::string> m_ExpressionInputs; //Vector of the string expressions entered by the user (n° of graphs + 1)
	glm::vec3 m_NewGraphColor; //Holds the color of the new graph
	glm::vec3 m_BackupColor; //Color backup for when a already existing graph is alter

	//int m_Degree = 0;
};