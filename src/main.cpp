#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "glm/gtc/type_ptr.hpp"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

#define TINYOBJLOADER_IMPLEMENTATION

#include "Mesh.h"
#include "Shader.h"
#include "Dragon.h"
#include "Camera.h"
#include <vector>
#include "Classes/Obj.h"

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float deltaTime = 0.0f;    // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

Camera cam;

unsigned int loadCubemap(std::vector<std::string> faces);


void processInput(GLFWwindow *window, Camera *cam) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cam->moveForward(cam->getSpeed() * deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cam->moveBackward(deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cam->moveLeftward(deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cam->moveRightward(deltaTime);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
	cam.mouse_callback(window, xpos, ypos);
}

//Rezise
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}

void messageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message,
                     const void *userParam) {
	switch (severity) {
		case GL_DEBUG_SEVERITY_HIGH:
			std::cout << "ERROR " << id << ": " << message << std::endl;

		case GL_DEBUG_SEVERITY_MEDIUM:
			std::cout << "WARNING " << id << ": " << message << std::endl;
			break;
		case GL_DEBUG_SEVERITY_LOW:
			std::cout << "INFO " << id << ": " << message << std::endl;
			break;
	}
}

unsigned int loadTexture(const std::string &path) {
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
	if (data) {
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	} else {
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

unsigned int loadCubemap(std::vector<std::string> faces) {
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++) {
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
			             data);
			stbi_image_free(data);
		} else {
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

int main() {
	Obj cube("resources/models", "resources/models/barrel.obj");
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "cpp-base", nullptr, nullptr);

	if (window == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glViewport(0, 0, 800, 600); // Zone de rendu

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //Rezise

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glDebugMessageCallback(messageCallback, nullptr);

	std::cout << "Driver: " << glGetString(GL_VERSION) << "\n";
	std::cout << "GPU: " << glGetString(GL_RENDERER) << "\n";
	std::vector<std::string> faces
			{
					"resources/textures/skybox/right.jpg",
					"resources/textures/skybox/left.jpg",
					"resources/textures/skybox/top.jpg",
					"resources/textures/skybox/bottom.jpg",
					"resources/textures/skybox/front.jpg",
					"resources/textures/skybox/back.jpg"
			};

	float skyboxVertices[] = {
			// positions
			-1.0f, 1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, 1.0f, -1.0f,
			-1.0f, 1.0f, -1.0f,

			-1.0f, -1.0f, 1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f, 1.0f, -1.0f,
			-1.0f, 1.0f, -1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f, -1.0f, 1.0f,

			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 1.0f,
			-1.0f, -1.0f, 1.0f,

			-1.0f, 1.0f, -1.0f,
			1.0f, 1.0f, -1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f, 1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f, 1.0f,
			1.0f, -1.0f, 1.0f
	};

	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);

	std::vector<unsigned int> objTextureIds;
	for (auto &texName : cube.getTexturesName()) {
		objTextureIds.push_back(loadTexture(texName));
	}
	unsigned int cubemapTexture = loadCubemap(faces);

	Shader skyboxShader("skybox");
	Shader myShader("myShader");

//	Mesh dragon;
//	dragon.setVertices(DragonVertices, sizeof(DragonVertices) / sizeof(float));
//	dragon.setIndices(DragonIndices, sizeof(DragonIndices) / sizeof(uint16_t));

	Mesh mesh;
	mesh.setVertices(&cube.getVertices().front(), cube.getVertices().size());
	mesh.setIndices(&cube.getIndices().front(), cube.getIndices().size());

	myShader.bind();
	glUniform1i(glGetUniformLocation(myShader.getId(), "texture1"), 0);
	skyboxShader.bind();
	glUniform1i(glGetUniformLocation(skyboxShader.getId(), "skybox"), 0);

	cam.init();

	// Configuration du framebuffer
	unsigned int fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	// On crée le color attachment texture
	unsigned int textureColorbuffer;
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
	// Maintenant on crée le renderbuffer object pour le depth et le stencil attachment
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH,
	                      SCR_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER,
	                          rbo); // now actually attach it
	// Enfin on vérifie si le framebuffer est complet
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete! : " << glCheckFramebufferStatus(GL_FRAMEBUFFER)
		          << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// Envents
		glfwPollEvents();

		// Inputs
		processInput(window, &cam);
		glfwSetCursorPosCallback(window, mouse_callback);

		glBindFramebuffer(GL_FRAMEBUFFER, fbo); // switch framebuffer
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Mettre en place le shader dans le framebuffer

		glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		myShader.bind();

		//Displays Position
		glm::vec3 pos(0.0f, -5.0f, 0.0f);


		glm::mat4 model = glm::translate(pos)// Position in word space
		                  * glm::eulerAngleXYZ(0.0f, 2.0f, 0.0f) // Model angle
		                  * glm::scale(glm::vec3(1.0f, 1.0f, 1.0f)); //scale

		//Update for move cam
		glm::mat4 view = cam.LookAtFront();
		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
		GLfloat ratio = (float) viewport[2] / (float) viewport[3];
		glm::mat4 projection = glm::perspective(glm::radians(60.0f), ratio, 0.01f, 100.0f);

		glm::mat4 mvp = projection * view * model;
		//Model pos
		glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(model));
		//CamPos
		glUniform3fv(2, 1, glm::value_ptr(cam.getPos()));
		//Object color
		glUniform3fv(3, 1, glm::value_ptr(mesh.getColor()));
		//lightColor
		glUniform3fv(4, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
		//Light pos
		glUniform3fv(5, 1, glm::value_ptr(glm::vec3(10.0f, 5.0f, 10.0f)));
		//view
		glUniformMatrix4fv(6, 1, GL_FALSE, glm::value_ptr(view));
		//projection
		glUniformMatrix4fv(7, 1, GL_FALSE, glm::value_ptr(projection));
		//ambientStrength
		glUniform1f(8, 0.05f);
		//specularStrength
		glUniform1f(9, 1.0f);
		//Use mesh
		mesh.bind();

		for (int i = 0; i < objTextureIds.size(); ++i) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, objTextureIds[i]);
		}
		glDrawElements(GL_TRIANGLES, cube.getIndices().size(), GL_UNSIGNED_SHORT, nullptr);
		mesh.unbind();

		// draw skybox as last
		glDepthFunc(
				GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		skyboxShader.bind();
		view = glm::mat4(glm::mat3(cam.LookAtFront())); // remove translation from the view matrix
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.getId(), "view"), 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.getId(), "projection"), 1, GL_FALSE, &projection[0][0]);

		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // set depth function back to default
		//Draw mesh

		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVAO);
	glfwTerminate();
	return 0;
}