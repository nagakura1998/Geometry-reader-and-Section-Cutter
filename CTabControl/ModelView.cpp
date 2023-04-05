#include "stdafx.h"
#include "ModelView.h"
#include <glad/glad.c>
#include <GLFW/glfw3.h>

#include<filesystem>
namespace fs = std::filesystem;

CModelView* CModelView::m_View = nullptr;
CModelView::CModelView()
{

}

CModelView* CModelView::CreateModelView() {
	if (!m_View) {
		m_View = new CModelView();
	}
	return m_View;
}

void CModelView::OnDraw()
{
	const unsigned int width = 1500;
	const unsigned int height = 1200;


	//// Vertices coordinates
	//Vertex vertices[] =
	//{ //               COORDINATES           /            COLORS          /           NORMALS         /       TEXTURE COORDINATES    //
	//	Vertex{glm::vec3(-1000.0f, 700.0f,  100.0f)},
	//	Vertex{glm::vec3(-1000.0f, 700.0f, -100.0f)},
	//	Vertex{glm::vec3(1000.0f, 700.0f, -100.0f)},
	//	Vertex{glm::vec3(1000.0f, 700.0f,  100.0f)}
	//};

	//// Indices for vertices order
	//GLuint indices[] =
	//{
	//	0, 1, 2,
	//	0, 2, 3
	//};


	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "Model View", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	glViewport(0, 0, width, height);

	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("light.vert", "light.frag");
	// Store mesh data in vectors for the mesh
	//std::vector <Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	//std::vector <GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
	//// Create section mesh
	//Mesh section(verts, ind);
	Mesh section(m_sectionVertices, m_sectionIndexes);

	// Shader for mesh cube
	Shader meshShader("light.vert", "light.frag");
	// Crate mesh mesh
	Mesh mesh(m_vertices, m_indexes);

	glm::vec4 meshColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 meshPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 meshModel = glm::mat4(1.0f);
	meshModel = glm::translate(meshModel, meshPos);

	glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 objectModel = glm::mat4(1.0f);
	objectModel = glm::translate(objectModel, objectPos);

	meshShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(meshShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(meshModel));
	glUniform4f(glGetUniformLocation(meshShader.ID, "lightColor"), meshColor.x, meshColor.y, meshColor.z, meshColor.w);
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), 1, 0, 0, meshColor.w);
	//glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), meshPos.x, meshPos.y, meshPos.z);

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Creates camera object
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 5.0f));

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Handles camera inputs
		camera.Inputs(window);
		// Updates and exports the camera matrix to the Vertex Shader
		camera.updateMatrix(45.0f, 1.0f, 100.0f);

		// Draws different meshes
		mesh.Draw(meshShader, camera);
		section.Draw(shaderProgram, camera, true);

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}

	// Delete all the objects we've created
	shaderProgram.Delete();
	meshShader.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
}