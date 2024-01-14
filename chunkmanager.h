#pragma once
#include "chunkposition.h"
#include "vertex.h"
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>
#include <map>
#include "chunk.h"
#include "shader.cpp"
#include <FastNoiseLite.h>
class ChunkManager {
public:
	ChunkManager() {}
	ChunkManager(int vRange, glm::vec3 worldCenterPos, Texture tex,FastNoiseLite noiseGenerator);
	static Chunk* GetChunk(ChunkPosition cpos);
	glm::vec3 worldCenterPos;
	FastNoiseLite noiseGenerator;
	int viewRange = 64;
	Texture tex;
	static std::vector<Chunk*> Chunks;
	static ChunkPosition Vec3ToChunkPos(glm::vec3 pos);
	void RebuildAllChunks();
	void UpdateWorld();
	void RenderAllChunks(glm::vec3 cameraPosition, glm::vec3 cameraNormal, glm::mat4 view, glm::mat4 projection, Shader shader);
	static short GetBlock(int x, int y, int z);
};
