//Includes
#include <pch.h>
#include <CartPlane.h>

//Global Varialbes
unsigned int WIDTH = 1000; //Window Witdh
unsigned int HEIGHT = 1000; //Window Height

exprtk::parser<double> Parser; //Mathematical expression parser

double SmallestLimit = 1.54624e-38; //Smallets limit of Camera posision in the Z-axis
double BiggestLimit = 1.5e+07; //Biggest limit of Camera posision in the Z-axis / Point of X and Y axis

static float zNear = pow(10.f, -38.f); //Near of the Projection Matrix
static float zFar = pow(10.f, 38.f); //Far of the Projection Matrix
glm::mat4 ProjMatrix = glm::perspective(glm::radians(45.f), (float)WIDTH / HEIGHT, zNear, zFar); //Projection Matrix
bool ChangeInProjMatrix = false; //If the Projection Matrix change this variable is set to true

double CamPosZ = 24.1421f; //Initial Camera position in the Z-Axis
bool ChangeInPosZ = false; //If the CamPosZ change this variable is set to true

int main()
{
	srand((unsigned int)time(NULL));
		
	//GLFW Setup
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Graph", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);//0 -> Vsync Off | 1 -> Vsync On

	//GLAD Setup
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, WIDTH, HEIGHT); //Position coordinates -> Screen coordinates

	//ImGui Setup
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	{
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 450");
	}

	//Events

	 //Scroll Event
	glfwSetScrollCallback(window, [](GLFWwindow* win, double xoff, double yoff)
	{
			CamPosZ -= yoff * pow(10.f, floor(log10(CamPosZ))) / 2.f; //The further the Camera position is, bigger is the increment

			//HACK THING
			if (CamPosZ >= BiggestLimit) //Checks if the Camera position is bigger than the BiggestLimit
			{
				CamPosZ = BiggestLimit;
			}
			else if (CamPosZ <= SmallestLimit) //Checks if the Camera position is smaller than the SmallestLimit
			{
				CamPosZ = SmallestLimit;
			}

			ChangeInPosZ = true; //Set the ChangeInPosZ to true
	});

	 //Resize Event
	glfwSetWindowSizeCallback(window, [](GLFWwindow* win, int width, int height)
	{
			//width == 0 && height == 0 -> minimized window
			if (width != 0 && height != 0) //Checks if the new width and height are different than 0. Due to the aspect ratio of the projection matrix
			{

				WIDTH = width; //Change the Window Width to the new Width
				HEIGHT = height;//Change the Window Height to the new Height

				glViewport(0, 0, WIDTH, HEIGHT); //Alters the viewport (Position coordinates -> Screen coordinates)

				ProjMatrix = glm::perspective(glm::radians(45.f), (float)WIDTH / HEIGHT, zNear, zFar); //New projection matrix based on the new Width and Height
				ChangeInProjMatrix = true; //Set the ChangeInProjMatrix to true
			}
			
	});

	//Line configuration
	glEnable(GL_LINE_SMOOTH); //Makes the line smoother
	glEnable(GL_BLEND); //Enables the Blend Function
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Blend Function config
	glLineWidth(2.f); //Line Width

	CartPlane Plane; //Creates the Cartesian Plane

	//Draw Loop
	while (!glfwWindowShouldClose(window))
	{
		//Background color
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		//ImGui Draw
		ImGui::Begin("Graph Control");
		Plane.ImGui();
		ImGui::End();

		//Plane Draw
		Plane.Draw();

		//Plane Update
		Plane.Update();

		//ImGui Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		//ImGui window backup
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}

		//Buffer
		glfwSwapBuffers(window);

		//Events
		glfwPollEvents();
	}

	//ImGui finalization
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	//GLFW finalization
	glfwTerminate();

	return 0;
}