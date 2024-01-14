#pragma once
#include "camera.cpp"
#include "AABB.h"
#include <vector>
#include "chunkmanager.h"
#include <iostream>
#include "mesh.h"
class Player
{
public:
	Camera cam;
	AxisAlignedBB playerBound;
	std::vector<AxisAlignedBB> blocksAround;
	void GetBlocksAround(AxisAlignedBB aabb);
	void Move(float dx, float dy, float dz);
	void ApplyGravity();
	glm::vec3 playerPos;
	bool isGrounded;
	Player(glm::vec3 pos, float sizeX, float sizeY, float sizeZ);
	void ProcessKeyboard(glm::vec2 moveDir, float dt);
	float moveSpeed = 25.0f;
	Mesh playerMesh;
};

