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
#include "ChunkLoadingQueue.h"
#include "Ray.h"
#include <algorithm>
#include <BS_thread_pool.hpp>
#include "AABB.h"
class ChunkManager {
public:
	ChunkManager() {}
	static short GetBlock(int x, int y, int, Chunk* c);
	ChunkManager(int vRange, glm::vec3 worldCenterPos, Texture tex,FastNoiseLite noiseGenerator);
	static Chunk* GetChunk(ChunkPosition cpos);
	static Chunk* GetChunkUnloaded(ChunkPosition cpos);
	glm::vec3 worldCenterPos;
	FastNoiseLite noiseGenerator;
	int viewRange = 256;
	Texture tex;
	static std::vector<Chunk*> Chunks;
	static std::vector<Chunk*> ChunksUnloaded;
	static ChunkPosition Vec3ToChunkPos(glm::vec3 pos);
	void RebuildAllChunks();
	void UpdateWorld();
	void RenderAllChunks(glm::vec3 cameraPosition, glm::vec3 cameraNormal, glm::mat4 view, glm::mat4 projection, Shader shader);
	static short GetBlock(int x, int y, int z);
	static int getChunkCalls;
	static void SetBlock(int x,int y,int z,short value);
	static glm::ivec3 HitFirstBlock(float length,Ray ray) {
		for (float i = 0; i < length; i += 0.05f) {
			glm::vec3 pos = ray.origin + ray.direction * i;
			glm::ivec3 intPos =glm::ivec3( AxisAlignedBB::floorFloat(pos.x), AxisAlignedBB::floorFloat(pos.y), AxisAlignedBB::floorFloat(pos.z));
			int blockID = ChunkManager::GetBlock(intPos.x, intPos.y, intPos.z);
			if (blockID != 0) {
				return intPos;
			}
		}
	}
	static glm::vec3 HitFirstPoint(float length, Ray ray) {
		for (float i = 0; i < length; i += 0.01f) {
			glm::vec3 pos = ray.origin + ray.direction * i;
			glm::ivec3 intPos = glm::ivec3(AxisAlignedBB::floorFloat(pos.x), AxisAlignedBB::floorFloat(pos.y), AxisAlignedBB::floorFloat(pos.z));
			int blockID = ChunkManager::GetBlock(intPos.x, intPos.y, intPos.z);
			if (blockID != 0) {
				return pos;
			}
		}
	}
};
