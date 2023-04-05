#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include"shaderClass.h"

class Camera
{
public:
	// Stores the main vectors of the camera
	static glm::vec3 Position;
	static glm::vec3 Orientation;
	static glm::vec3 Up;
	static glm::mat4 cameraMatrix;

	// Prevents the camera from jumping around when first clicking left click
	bool firstClick = true;

	// Stores the width and height of the window
	static int width;
	static int height;

	// Adjust the speed of the camera and it's sensitivity when looking around
	static float speed;
	float sensitivity = 100.0f;

	// Camera constructor to set up initial values
	Camera(int width, int height, glm::vec3 position);

	// Updates the camera matrix to the Vertex Shader
	static void updateMatrix(float FOVdeg, float nearPlane, float farPlane);
	// Exports the camera matrix to a shader
	void Matrix(Shader& shader, const char* uniform);
	static void SetZoom(double ratio);
	// Handles camera inputs
	void Inputs(GLFWwindow* window);
};
#endif