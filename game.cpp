#pragma once
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include "mesh.h"
#include <iostream>
#include <map>
#include "shader.cpp"
#include <string>
#include "camera.cpp"
#include <stb_image.h>
#include "chunkmanager.h"
#include "chunk.h"
#include "Player.h"
#include "Skybox.h"


class Game {
public:
	unsigned int VBO, VAO, EBO;
	unsigned int shaderProgram ;
	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";
	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
		"}\n\0";

	void Test() {
		// build and compile our shader program
	  // ------------------------------------
	  // vertex shader
		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);
		// check for shader compile errors
		int success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		// fragment shader
		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);
		// check for shader compile errors
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		// link shaders
		 shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);
		// check for linking errors
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		// set up vertex data (and buffer(s)) and configure vertex attributes
		// ------------------------------------------------------------------
		float vertices[] = {
			 0.5f,  0.5f, 0.0f,  // top right
			 0.5f, -0.5f, 0.0f,  // bottom right
			-0.5f, -0.5f, 0.0f,  // bottom left
			-0.5f,  0.5f, 0.0f   // top left 
		};
		unsigned int indices[] = {  // note that we start from 0!
			0, 1, 3,  // first Triangle
			1, 2, 3   // second Triangle
		};
		unsigned int VBO, VAO, EBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
		// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
		glBindVertexArray(0);
	}








	int scrWidth=800, scrHeight=600;
	GLFWwindow* window;
//	Camera mainCam = Camera(0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	Player player = Player(glm::vec3(0, 128, 0),0.6f, 1.8f,0.6f);
	Game(GLFWwindow* window) {
		this->window = window;
	}
	Shader curShader;
	Shader skyboxShader;
	//Skybox skybox;
		//Chunk c;
	ChunkManager cm;
	FastNoiseLite noiseGenerator=FastNoiseLite();
	void Init() {
		Test();
		LoadShader("D:/learnopengl/shaders/vshader.txt", "D:/learnopengl/shaders/fshader.txt", "simpleshader");
		LoadShader("D:/learnopengl/shaders/vshaderskybox.txt", "D:/learnopengl/shaders/fshaderskybox.txt", "skyboxshader");
		LoadTexture("D:/learnopengl/shaders/wall.png", "wall");
		LoadTexture("D:/learnopengl/shaders/awesomeface.png", "face");
		LoadTexture("D:/learnopengl/shaders/terrain.png", "terrain");
		skyboxShader = GetShader("skyboxshader");
		curShader = GetShader("simpleshader");
		
		//c = Chunk(0, 0, GetTexture("terrain"));
		cm = ChunkManager(256, glm::vec3(0.0f), GetTexture("terrain"), noiseGenerator);
		glEnable(GL_DEPTH_TEST);
	/*	skybox = Skybox(std::vector<std::string>{
			"D:/learnopengl/shaders/skybox/right.jpg",
			"D:/learnopengl/shaders/skybox/left.jpg",
			"D:/learnopengl/shaders/skybox/top.jpg",
			"D:/learnopengl/shaders/skybox/bottom.jpg",
			"D:/learnopengl/shaders/skybox/front.jpg",
			"D:/learnopengl/shaders/skybox/back.jpg"
			}, skyboxShader, GetTexture("terrain"));*/
	//	skybox.SetupSkybox();
		std::thread updateWorldThread = std::thread([&]() {ChunkLoadingQueue::LoadChunkThread(GetTexture("terrain"), noiseGenerator); });
		updateWorldThread.detach();
		
	}
	void ProcessInput(GLFWwindow* window,float dt) {
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
		glm::vec2 moveDir(0.0f);
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			moveDir.x = 1.0f;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			moveDir.x = -1.0f;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			moveDir.y = -1.0f;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			moveDir.y = 1.0f;
		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
			cm.RebuildAllChunks();
		}
		player.ProcessKeyboard(moveDir,dt,glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT), glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT));
		player.ApplyGravity(dt);
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			player.Jump();
		}
	//	std::cout << mainCam.Position.s << std::endl;
	}
	void FixedUpdate() {
	//	
	
		//ChunkLoadingQueue::LoadChunkThread(GetTexture("terrain"), noiseGenerator);
	//	std::cout << ChunkManager::getChunkCalls << std::endl;
	//	ChunkManager::getChunkCalls = 0;
	}
	void Render() {
		
		
	//	glCheckError();
		//glUseProgram(shaderProgram);
		
		glm::mat4 view =player.cam.GetViewMatrix(); // make sure to initialize matrix to identity matrix first
		glm::mat4 projection = glm::mat4(1.0f);
		//view = glm::mat4(glm::mat3(player.cam.GetViewMatrix()));
		projection = glm::perspective(glm::radians(45.0f), (float)scrWidth / (float)scrHeight, 0.1f, 50000.0f);
		//skybox.SetupSkybox();
		curShader.use();
		//skybox.Draw(view, projection);
	//	std::cout << player.playerPos.x << " " << player.playerPos.y << " " << player.playerPos.z << std::endl;
	//	std::cout << player.cam.Position.x << " " << player.cam.Position.y << " " << player.cam.Position.z << std::endl;
	
		// pass transformation matrices to the shader
	/*	curShader.setVec3("lightPos", mainCam.Position);
		curShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
		curShader.setMat4("model", glm::mat4(1.0f));
		curShader.setMat4("projection", projection);*/ // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
		//curShader.setMat4("view", view);
		//cubeMesh.Draw(curShader);
		//c.RenderChunk(view, projection, curShader);
	//	std::cout << mainCam.Position.x <<" " << mainCam.Position.y << " " << mainCam.Position.z << std::endl;
		cm.worldCenterPos = player.cam.Position;
		//cm.UpdateWorld();
		

		player.UpdatePlayerChunk();
		if (player.isNeededUpdatingWorld == true) {
			cm.UpdateWorld();
	//		std::cout << "update" << std::endl;
			player.isNeededUpdatingWorld = false;
		}

		cm.RenderAllChunks(player.cam.Position, player.cam.Front,view, projection, curShader);
	//	
		glUseProgram(0);
		//view = glm::mat4(glm::mat3(player.cam.GetViewMatrix()));
		//skyboxShader.use();
		
		//skyboxShader.setInt("gCubemapTexture", 0);
	//	skybox.SetupSkybox();
		
		
	}
	void UpdateGame(float dt) {
		if (player.placeBlockCD >= 0.0f) {
			player.placeBlockCD -= dt;
			std::cout << player.placeBlockCD << std::endl;
		}
	}
	 std::map<std::string, Texture> allTextures;
	 std::map<std::string, Shader> allShaders;
	 void LoadShader(const char* vPath, const char* fPath,std::string name) {
		Shader shader = Shader(vPath, fPath);
		allShaders[name] = shader;
	}
	 Shader GetShader(std::string name) {
		if (&allShaders[name] != NULL) {
			return allShaders[name];

		}
		else {
			std::cout << "null" << std::endl;
			return  Shader();
		}
	}
	 Texture GetTexture(std::string name) {
		if (&allTextures[name] != NULL) {
			return allTextures[name];

		}
		else {
			std::cout << "null" << std::endl;
		//	return *NULL;
		}
	}
	 void LoadTexture(const char* path, std::string name) {
		stbi_set_flip_vertically_on_load(true);
		int width, height, nrChannels;
		unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


	
	//	std::cout << nrChannels << std::endl;
		glTexImage2D(GL_TEXTURE_2D, 0, (nrChannels==3)?GL_RGB:GL_RGBA, width, height, 0, (nrChannels == 3) ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		
		Texture t;
		t.id = texture;
		t.path = path;
		t.type = "texture_diffuse";
		allTextures[name] = t;
		stbi_image_free(data);
	}

};
