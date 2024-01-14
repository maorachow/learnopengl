#include "ChunkBlocksDic.h"
std::map<int, std::vector<glm::vec2>> ChunkBlocksDic::uvCoords = std::map<int, std::vector<glm::vec2>>{
	{1,std::vector<glm::vec2>{glm::vec2(0.0f,0.0f),glm::vec2(0.0f,0.0f),glm::vec2(0.0f,0.0f),glm::vec2(0.0f,0.0f),glm::vec2(0.0f,0.0f),glm::vec2(0.0f,0.0f)}},
	{2,std::vector<glm::vec2>{glm::vec2(0.0625f,0.0f),glm::vec2(0.0625f,0.0f),glm::vec2(0.0625f,0.0f),glm::vec2(0.0625f,0.0f),glm::vec2(0.0625f,0.0f),glm::vec2(0.0625f,0.0f)}},
	{3,std::vector<glm::vec2>{glm::vec2(0.125f,0.0f),glm::vec2(0.125f,0.0f),glm::vec2(0.125f,0.0f),glm::vec2(0.125f,0.0f),glm::vec2(0.125f,0.0f),glm::vec2(0.125f,0.0f)}},
	{4,std::vector<glm::vec2>{glm::vec2(0.1875f,0.0f),glm::vec2(0.1875f,0.0f),glm::vec2(0.125f,0.0f),glm::vec2(0.0625f,0.0f),glm::vec2(0.1875f,0.0f),glm::vec2(0.1875f,0.0f)}},
};