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
#include "shader.cpp"
#include "mesh.h"
#include <FastNoiseLite.h>
class Chunk {
public:
	bool isChunkBuildCompleted = false;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	Texture texture;
	Mesh chunkMesh;
	ChunkPosition chunkPos;
	short map[CHUNKWIDTH][CHUNKHEIGHT][CHUNKWIDTH];
	FastNoiseLite* noiseGenerator;
	Chunk* leftChunk;
	Chunk* rightChunk;
	Chunk* frontChunk;
	Chunk* backChunk;
	float thisHeightMap[CHUNKWIDTH + 2][CHUNKWIDTH + 2];
	void GenerateHeightmap();
	Chunk() {}
	void BuildChunkTask();
	Chunk(int posX, int posY, Texture tex,FastNoiseLite* noiseGen);
	void InitMap();
	void BuildMesh();
	void BuildBlock(int x, int y, int z, std::vector<Vertex>* vertices, std::vector<unsigned int>* indices);
	void BuildFace(int blockID, glm::vec3 corner, glm::vec3 up, glm::vec3 right, std::vector<Vertex>* vertices, std::vector<unsigned int>* indices,int side);
	short GetBlockType(int x, int y, int z);
	bool CheckNeedBuildFace(int x, int y, int z);
	bool CheckisInView(glm::vec3 position, glm::vec3 normal);
	void RenderChunk(glm::vec3 camPosition, glm::vec3 cameraNormal, glm::mat4 view, glm::mat4 projection, Shader shader);
	short PredictBlockType(float noiseValue, int y);
	void ApplyMesh();
	bool isChunkDataBuilt = false;
};