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
	glm::ivec3 playerLastIntPos;
	glm::ivec3 playerCurIntPos;
	Camera cam;
	AxisAlignedBB playerBound;
	std::vector<AxisAlignedBB> blocksAround;
	void GetBlocksAround(AxisAlignedBB aabb);
	void Move(float dx, float dy, float dz);
	void ApplyGravity();
	glm::vec3 playerPos;
	bool isGrounded;
	Player(glm::vec3 pos, float sizeX, float sizeY, float sizeZ);
	void ProcessKeyboard(glm::vec2 moveDir, float dt, bool isLeftMousePressed,bool isRightMousePressed);
	float moveSpeed = 10.0f;
	Mesh playerMesh;
	Chunk* curChunk;
	bool CheckIsInChunk(Chunk* c);
	bool isNeededUpdatingWorld;
	void UpdatePlayerChunk();
	void ApplyGravity(float dt);
	float curGravityValue;
	void Jump();
	void BreakBlock();
	void PlaceBlock();
	float placeBlockCD = 0.0f;
};

