#pragma once
#include <stdlib.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
struct ChunkPosition {
public:
	int x;
	int y;


	ChunkPosition() = default;


	bool operator < (const ChunkPosition a) const
	{
		if (x  < a.x ) {
			return true;
		}
		else {
		return false;
		}
			
		
	}
	bool operator>(const ChunkPosition a) const
	{
		if (y < + a.y) {
			return false;
		}
		else {
			return true;
		}


	}
	ChunkPosition(int x, int y) {
		this->x = x;
		this->y = y;
	}

	bool operator==(const ChunkPosition& other) const
	{
		return x == other.x && y == other.y;
	}
};
const int CHUNKWIDTH = 16;
const int CHUNKHEIGHT = 256;
enum BlockID {
	Air = 0,
	Dirt = 1,
	Stone = 2
};