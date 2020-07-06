#include <pch.h>
#include <CartPlane.h>

//Global variable (defined ate main.cpp)
extern double CamPosZ;
extern unsigned int WIDTH;
extern unsigned int HEIGHT;
extern glm::mat4 ProjMatrix;
extern bool ChangeInPosZ;
extern bool ChangeInProjMatrix;

//Constructor
CartPlane::CartPlane(const std::pair<double, double>& range) : 
	m_AxisShader("res/plane.glsl"), //m_AxisShader = Shader("res/plane.glsl")
	m_GridShader("res/grid.glsl"), //m_GridShader = Shader("res/grid.glsl")
	m_AxisVBO(&m_AxisVertices, sizeof(m_AxisVertices)), //m_AxisVBO = VertexBuffer(&m_AxisVertices, sizeof(m_AxisVertices))
	m_Range(range), //m_Range = range (Usually { -10.f, 10.f })
	m_InitRange(range), //m_InitRange = range (Usually { -10.f, 10.f })
	m_Camera(glm::vec3(0.0f, .0f, 24.1421f), glm::vec3(.0f, .0f, -1.0f), glm::vec3(.0f, 1.0f, .0f)), //Creates a camera object (Initial position = glm::vec3(0.0f, .0f, 24.1421f), front = glm::vec3(.0f, .0f, -1.0f), up = glm::vec3(.0f, 1.0f, .0f))
	m_Proj(ProjMatrix) //m_Proj = ProjMatrix (alias)
{
	m_VP = m_Proj * m_Camera.GetViewMatrix(); //Sets the View Projection matrix

	m_GridOffset = 2.f; //Sets the grid offset to 2.f
	GenGrid(); //Generates the grid

	//Grid OpenGL Setup
	m_GridShader.Bind(); //Binds the grid shader
	m_GridShader.setUniformMat4f("u_VP", m_VP); //Sets the uniform "u_VP" to m_VP
	m_GridShader.setUniform1f("u_OffSet", m_GridOffset); //Sets the uniform "u_OffSet" to m_GridOffset
	m_GridShader.setUniform3f("u_Color", glm::vec3(0.3f)); //Sets the uniform "u_Color" to glm::vec3(0.3f)
	m_GridShader.Unbind(); //Unbinds the grid shader

	m_GridVBO = std::make_unique<VertexBuffer>(&m_GridVertices[0], m_GridVertices.size() * sizeof(float)); //Creates the grid Vertex Buffer
	m_GridVBO->Bind(); //Binds the grid Vertex Buffer
	m_GridVAO.Bind(); //Binds the grid Vertex Array
	VertexBufferAttLayout GridAttLayout; //Grid Attribute Layout
	GridAttLayout.Push<float>(2); //Pushs 2 doubles as read mode
	m_GridVAO.AddAtt(*m_GridVBO, GridAttLayout); //Adds the Attribute Layout (m_GridAttLayout) to the Vertex Array (m_GridVAO)

	//Axis OpenGL Setup
	m_AxisShader.Bind(); //Binds the axis shader
	m_AxisShader.setUniformMat4f("u_VP", m_VP); //Sets the uniform "u_VP" to m_VP
	m_AxisShader.setUniform3f("u_Color", glm::vec3(0.f)); //Sets the uniform "u_Color" to glm::vec3(0.f)

	m_AxisVBO.Bind(); //Binds the axis Vertex Buffer
	m_AxisVAO.Bind(); //Binds the axis Vertex Array
	VertexBufferAttLayout AxisAttLayout; //Axis Attribute Layout
	AxisAttLayout.Push<double>(2); //Pushs 2 doubles as read mode
	m_AxisVAO.AddAtt(m_AxisVBO, AxisAttLayout); //Adds the Attribute Layout (m_AttLayout) to the Vertex Array (m_VAO)

	//ImGui
	m_ExpressionInputs.emplace_back(""); //m_ExpressionInputs already start with empty string for the user use as input (n° of graphs + 1)
	m_NewGraphColor = GenColor(); //Generates a random color to the new graph
}

//Destructor
CartPlane::~CartPlane()
{
}

void CartPlane::Draw()
{
	//Draw Grid
	m_GridShader.Bind(); //Binds the grid shader
	m_GridVAO.Bind(); //Binds the grid Vertex Array
	glDrawArrays(GL_POINTS, 0, m_GridVertices.size() / 2); //Draws the grid (OpenGL call)
	m_GridVAO.Unbind(); //Unbinds the grid Vertex Array
	m_GridShader.Unbind(); //Unbinds the grid shader

	//Draw Plane
	m_AxisShader.Bind(); //Binds the graoh shader
	m_AxisVAO.Bind(); //Binds the graph Vertex Array
	glDrawArrays(GL_LINES, 0, 4); //Draws the axis (OpenGL call)
	m_AxisVAO.Unbind(); //Unbinds the axis Vertex Array
	m_AxisShader.Unbind(); //Unbinds the axis shader

	//Draw Graph
	for (const Graph& graph : m_Graphs) //Loops through all the graphs
	{
		//Checks if the show state is true
		if (graph.GetShow())
		{
			graph.Draw();
		}
	}
}

void CartPlane::ImGui()
{
	if (ImGui::Button("Hide Graphs")) //ImGui Button
	{
		for (Graph& graph : m_Graphs) //Loops through all the graphs
		{
			graph.SetShow(false); //Sets the show state to false
		}
	}

	ImGui::Spacing(); //Larger space between the lines

	for (unsigned int i = 0; i < m_ExpressionInputs.size(); i++) //Loops through all the m_ExpressionInputs
	{
		//Color
		std::string sLabelColor = "Color##" + std::to_string(i); //Color label string
		const char* cLabelColor = sLabelColor.c_str(); //Color label const char*
		auto ColorFlags = ImGuiColorEditFlags_NoInputs + ImGuiColorEditFlags_NoLabel; //Color Flags
		if (i == m_ExpressionInputs.size() - 1) //Checks if i is equals to the last index of m_ExpressionInputs
		{
			ImGui::ColorEdit3(cLabelColor, &m_NewGraphColor[0], ColorFlags); //ImGui Color Edit
		}
		else
		{
			//All other graphs

			//Show function
			bool toggle = m_Graphs[i].GetShow(); //Gets the current graph show state
			std::string sLabelToggle = "Show##" + std::to_string(i); //Show label strig
			const char* cLabelToggle = sLabelToggle.c_str(); //Show label const char*
			ImGui::PushID(cLabelToggle); //Pushs an ID for not enter a name in the ImGui::Checkbox()
			if (ImGui::Checkbox("", &toggle)) //ImGui Checkbox
			{
				m_Graphs[i].SetShow(toggle); //Sets the current graph show state
			}
			ImGui::PopID(); //ImGui PopID

			ImGui::SameLine(); //Puts others ImGui objects in the same line

			//Color picker
			auto color = m_Graphs[i].GetColor(); //Gets the color of the current graph
			if (ImGui::ColorEdit3(cLabelColor, &color[0], ColorFlags)) //ImGui Color Edit
			{
				m_Graphs[i].SetColor(color); //Sets the current graph color to the new color
			}
		}

		ImGui::SameLine(); //Puts others ImGui objects in the same line

		//Expression input
		std::string sLabelTextInput = "Expression##" + std::to_string(i); //Expression input label string
		const char* cLabelTextInput = sLabelTextInput.c_str(); //Expression input const char*
		ImGui::PushID(cLabelTextInput); //Pushs an ID for not enter a name in the ImGui::InputText()
		if (ImGui::InputText("", &m_ExpressionInputs[i], ImGuiInputTextFlags_EnterReturnsTrue)) //ImGui Input Text
		{
			GraphLogic(i); //Calls GraphLogic()
		}
		ImGui::PopID(); //ImGui PopID

		ImGui::SameLine(); //Puts others ImGui objects in the same line

		//Enter the expression
		std::string sLabelCompileButton = "Enter##" + std::to_string(i); //Enter button label string
		const char* cLabelCompileButton = sLabelCompileButton.c_str(); //Enter button label const char*
		if (ImGui::Button(cLabelCompileButton)) //ImGui Button
		{
			GraphLogic(i); //Calls GraphLogic()
		}
	}

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);//Frame Time and FPS
}

void CartPlane::Update()
{
	//Zoom In/Out
	if (ChangeInPosZ) //Checks if the position of the camera changed (ChangeInPosZ -> global variable defined at main.cpp)
	{
		auto pos = m_Camera.GetCamPos(); //Gets the current camera position
		pos.z = (float)CamPosZ; //Set the camera z position to the new posision (CamPosZ -> global variable defined at main.cpp)

		m_Camera.SetCamPos(pos); //Sets the new camera position

		SetVP(); //Calls SetVP()

		for (Graph& graph : m_Graphs) //Loops through all the graphs
		{
			std::cout << m_Range.first << " | " << m_Range.second << " | " << round(log10(abs(m_Range.first) + m_Range.second)) << "\n";
			graph.SetRange(m_Range); //Sets each graph View Projection matrix
		}

		ChangeInPosZ = false; //Set the ChangeInPosZ to false
	}

	//Drag the plane
	if (m_Camera.MouseMove()) //Checks if the mouse was dragged
	{
		SetVP(); //Calls SetVP()
	}	

	//Resize
	if (ChangeInProjMatrix) //Checks if the projection matrix changed (ChangeInProjMatrix -> global variable defined at main.cpp)
	{
		m_Proj = ProjMatrix; //Sets m_Proj to the new ProjMatrix (alias) (ProjMatrix -> global variable defined at main.cpp)

		SetVP(); //Calls SetVP()

		ChangeInProjMatrix = false; //Sets ChangeInProjMatrix to false (ChangeInProjMatrix -> global variable defined at main.cpp)
	}
}

void CartPlane::GenGrid()
{
	unsigned int size = (unsigned int)(abs(m_Range.first) + abs(m_Range.second)); //Absolute value of the m_Range difference
	m_GridVertices.reserve((size + 1) * (size + 1)); //Reserve size^2 memory


	//int degree = 0;
	//int subDegree;
	//float step = 1;
	////int input = 4 * 2;
	//if (size > 2 * (2 * pow(10, m_Degree)) && size <= 2 * (2 * pow(10, m_Degree + 1)))
	//{
	//	step = 0.5 * pow(10, m_Degree);
	//	if (size > 2 * (2 * pow(10, m_Degree)) && size <= 2 * (4 * pow(10, m_Degree)))
	//	{
	//		//std::cout << "]2, 4 ]" << std::endl;
	//		subDegree = 0;
	//		step *= pow(2, subDegree);
	//	}
	//	else if (size > 2 * (4 * pow(10, m_Degree)) && size <= 2 * (8 * pow(10, m_Degree)))
	//	{
	//		//std::cout << "]4, 8 ]" << std::endl;
	//		subDegree = 1;
	//		step *= pow(2, subDegree);
	//	}
	//	else if (size > 2 * (8 * pow(10, m_Degree)) && size <= 2 * (2 * pow(10, m_Degree + 1)))
	//	{
	//		//std::cout << "]8, 20 ]" << std::endl;
	//		subDegree = 2;
	//		step *= pow(2, subDegree);
	//	}
	//}
	//else if (size > 2 * (2 * pow(10, m_Degree + 1)))
	//{
	//	m_Degree += 1;
	//	subDegree = 0;
	//	step = 0.5 * pow(10, m_Degree) * pow(2, subDegree);
	//}
	//else if (size <= 2 * (2 * pow(10, m_Degree)))
	//{
	//	m_Degree -= 1;
	//	subDegree = 2;
	//	step = 0.5 * pow(10, m_Degree) * pow(2, subDegree);
	//}


	for (int j = (int)m_Range.first; j <= (int)m_Range.second; j++) //Goes from -Y (m_Range.first) to +Y (m_Range.second)
	{
		for (int i = (int)m_Range.first; i <= (int)m_Range.second; i++) //Goes from -X (m_Range.first) to +X (m_Range.second)
		{
			float x = m_GridOffset * i; //X point
			float y = m_GridOffset * j; //Y point

			m_GridVertices.push_back(x); //Pushes X to m_GridVertices
			m_GridVertices.push_back(y); //Pushes Y to m_GridVertices
		}
	}

	

}

bool CartPlane::AddGraph(const std::string & strExp, int index, const glm::vec3 & color)
{
	std::shared_ptr<Expression> exp = std::make_shared<Expression>(strExp); //Makes a new expression
	if (exp->IsCompiled()) //Checks if the new expression compiled
	{
		m_Graphs.emplace_back(exp, color, m_VP, m_Range); //Emplace back the new graph
		return true;
	}
	else
	{
		return false;
	}
}

void CartPlane::GraphLogic(int i)
{
	if (m_ExpressionInputs[i] != "") //Checks if the input isn't empty
	{
		if (i == m_ExpressionInputs.size() - 1) //Check if input is from the last index of m_ExpressionInputs (completely new graph)
		{
			if (AddGraph(m_ExpressionInputs[i], m_Graphs.size(), m_NewGraphColor)) //Checks if the graph was sucssefully added
			{
				m_ExpressionInputs.push_back(""); //Adds a empty string to m_ExpressionInputs (n° of graphs + 1)
				m_NewGraphColor = GenColor(); //Generates a random color to the new graph
			}
		}
		else //Already existing graph
		{
			std::string graphBackup = m_Graphs[i].GetExpression()->GetStrExpression(); //Makes a backup string for the graph string
			graphBackup.erase(std::remove_if(graphBackup.begin(), graphBackup.end(), std::isspace), graphBackup.end()); //Removes the spaces of graphBackup string

			std::string inputBackup = m_ExpressionInputs[i]; //Makes a backup string for the input string
			inputBackup.erase(std::remove_if(inputBackup.begin(), inputBackup.end(), std::isspace), inputBackup.end()); //Removes the spaces of inputBackup string

			if (graphBackup != inputBackup) //Checks if the input is different to the current mathematical expression of the graph
			{
				m_BackupColor = m_Graphs[i].GetColor(); //Backup the color of the graph
				m_Graphs[i].SetExpression(m_ExpressionInputs[i]); //Sets the current graph expression to the new expression
			}
		}
	}
}

glm::vec3 CartPlane::GenColor()
{
	float r; //Red
	float g; //Green
	float b; //Blue
	do
	{
		r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
		g = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
		b = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	} while (r < 0.3f || g < 0.3f || b < 0.3f);

	return glm::vec3(r, g, b);
}

void CartPlane::SetRange()
{
	double xl = (-m_VP[3].w - m_VP[3].x) / 2.414213f;
	double xr = (m_VP[3].w - m_VP[3].x) / 2.414213f;

	m_Range = { xl, xr };

	//unsigned int size = (unsigned int)(abs(m_Range.first) + abs(m_Range.second)); //Absolute value of the m_Range
	//std::cout << "Left X: " << xl  << " | Right X: " << xr << " | Size: " << size << "\n";
}

void CartPlane::SetVP()
{
	m_VP = m_Proj * m_Camera.GetViewMatrix(); //Sets the View Projection matrix

	SetRange();

	m_AxisShader.Bind(); //Binds the axis shader
	m_AxisShader.setUniformMat4f("u_VP", m_VP); //Sets the uniform "u_VP" to m_VP
	m_AxisShader.Unbind(); //Unbinds the axis shader

	m_GridShader.Bind(); //Binds the grid shader
	m_GridShader.setUniformMat4f("u_VP", m_VP); //Sets the uniform "u_VP" to m_VP
	m_GridShader.Unbind(); //Unbinds the axis shader

	for (Graph& graph : m_Graphs) //Loops through all the graphs
	{
		graph.SetVP(m_VP); //Sets each graph View Projection matrix
	}
}
