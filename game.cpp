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
class Game {
public:
	int scrWidth=800, scrHeight=600;
	GLFWwindow* window;
//	Camera mainCam = Camera(0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	Player player = Player(glm::vec3(0, 64, 0),0.6f, 1.8f,0.6f);
	Game(GLFWwindow* window) {
		this->window = window;
	}
	Shader curShader;
	Mesh cubeMesh;
		//Chunk c;
	ChunkManager cm;
	FastNoiseLite noiseGenerator=FastNoiseLite();
	void Init() {
		LoadShader("D:/learnopengl/shaders/vshader.txt", "D:/learnopengl/shaders/fshader.txt", "simpleshader");
		LoadTexture("D:/learnopengl/shaders/wall.png", "wall");
		LoadTexture("D:/learnopengl/shaders/awesomeface.png", "face");
		LoadTexture("D:/learnopengl/shaders/terrain.png", "terrain");
		curShader = GetShader("simpleshader");
		
		//c = Chunk(0, 0, GetTexture("terrain"));
		cm = ChunkManager(64, glm::vec3(0.0f), GetTexture("terrain"), noiseGenerator);
		glEnable(GL_DEPTH_TEST);
		//std::thread updateWorldThread = std::thread([&]() {while (1) { cm.UpdateWorld(); } });
		//updateWorldThread.detach();
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
		player.ProcessKeyboard(moveDir,dt);

	//	std::cout << mainCam.Position.s << std::endl;
	}
	
	void Render() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		
		glUseProgram(curShader.ID);
		
		//glDrawElements(GL_TRIANGLES,3, GL_UNSIGNED_INT, 0);
		glm::mat4 view = player.cam.GetViewMatrix(); // make sure to initialize matrix to identity matrix first
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), (float)scrWidth / (float)scrHeight, 0.1f, 50000.0f);
	
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
			std::cout << "update" << std::endl;
			player.isNeededUpdatingWorld = false;
		}
		cm.RenderAllChunks(player.cam.Position, player.cam.Front,view, projection, curShader);
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