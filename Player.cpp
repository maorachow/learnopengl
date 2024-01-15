#include "Player.h"
#include <algorithm>
void Player::GetBlocksAround(AxisAlignedBB aabb) {
    int minX = AxisAlignedBB::floorFloat(aabb.getMinX() - 0.1f);
    int minY = AxisAlignedBB::floorFloat(aabb.getMinY() - 0.1f);
    int minZ = AxisAlignedBB::floorFloat(aabb.getMinZ() - 0.1f);
    int maxX = AxisAlignedBB::ceilFloat(aabb.getMaxX() + 0.1f);
    int maxY = AxisAlignedBB::ceilFloat(aabb.getMaxY() + 0.1f);
    int maxZ = AxisAlignedBB::ceilFloat(aabb.getMaxZ() + 0.1f);
    this->blocksAround = std::vector<AxisAlignedBB>{};
    for (int z = minZ - 1; z <= maxZ + 1; z++) {
        for (int x = minX - 1; x <= maxX + 1; x++) {
            for (int y = minY - 1; y <= maxY + 1; y++) {
            //    std::cout << x << " " << y << " " << z << " " << std::endl;
                int blockID =ChunkManager::GetBlock(x, y, z);
                if (blockID > 0 && blockID < 100) {
                    this->blocksAround.push_back(AxisAlignedBB(x, y, z, x + 1, y + 1, z+ 1));
                }
            }
        }
    }
}
void Player::Move(float dx, float dy, float dz) {
    //  this.ySize *= 0.4;
   // std::cout << blocksAround.size() << std::endl;
    
    float movX = dx;
    float movY = dy;
    float movZ = dz;
    if (blocksAround.size() == 0) {
        playerBound = *playerBound.offset(0, dy, 0);
        playerBound = *playerBound.offset(dx, 0, 0);
        playerBound = *playerBound.offset(0, 0, dz);
    }





    for(int i = 0; i < blocksAround.size();i++) {
        dy =blocksAround[i].calculateYOffset(playerBound, dy);
    }

    if (movY != dy) {
        isGrounded = true;

    }
    else {
        isGrounded = false;
    }
    playerBound = *playerBound.offset(0, dy, 0);

    //      bool fallingFlag = (this.onGround || (dy != movY && movY < 0));

    for (int i = 0; i < blocksAround.size(); i++) {
        dx = blocksAround[i].calculateXOffset(playerBound, dx);
    }

    playerBound = *playerBound.offset(dx, 0, 0);

    for (int i = 0; i < blocksAround.size(); i++) {
        dz = blocksAround[i].calculateZOffset(playerBound, dz);
    }

    playerBound = *playerBound.offset(0, 0, dz);
    playerPos = glm::vec3((playerBound.getMaxX() + playerBound.getMinX()) / 2, (playerBound.getMinY() + playerBound.getMaxY()) / 2+0.6f, (playerBound.getMinZ() + playerBound.getMaxZ()) / 2);
    cam.Position = playerPos;
}

void Player::ApplyGravity(float dt) {
    if (curChunk == NULL) {
        return;
    }
    curGravityValue += dt * -9.8f;
    Move(0.0f, curGravityValue*dt, 0.0f);
    curGravityValue = glm::clamp(curGravityValue, -10.0f, 10.0f);
}
void Player::Jump() {
    if (isGrounded == true) {
    curGravityValue =5.0f;
    }
    
}
Player::Player(glm::vec3 pos, float sizeX, float sizeY, float sizeZ) {
    this->playerBound = AxisAlignedBB(pos.x, pos.y, pos.z, pos.x + sizeX, pos.y + sizeY, pos.z + sizeZ);
    this->playerPos = glm::vec3((playerBound.getMaxX() + playerBound.getMinX()) / 2, (playerBound.getMinY() + playerBound.getMaxY()) / 2 , (playerBound.getMinZ() + playerBound.getMaxZ()) / 2);
    this->cam = Camera(0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
    this->cam.Position = playerPos;
}
void Player::ProcessKeyboard(glm::vec2 dir, float deltaTime)
{
     if (curChunk == NULL) {
        return;
    }
    playerCurIntPos = playerPos;
    if (playerCurIntPos != playerLastIntPos) {
        GetBlocksAround(playerBound);
       // std::cout << "get" << std::endl;
    }
   
    playerLastIntPos = playerCurIntPos;
    float velocity = moveSpeed * deltaTime;
    glm::vec2 finalMoveVec = dir * velocity;
    if (finalMoveVec.x != 0.0f)
         this-> Move(((cam.Front * finalMoveVec.x).x), 0,(cam.Front * finalMoveVec.x).z);


    if (finalMoveVec.y != 0.0f)
        this->Move((cam.Right * finalMoveVec.y).x,0, (cam.Right * finalMoveVec.y).z);
}
bool Player::CheckIsInChunk(Chunk* c) {
    if (c == NULL) {
        return false;
    }
    glm::vec3 playerSpacePos = playerPos - glm::vec3(c->chunkPos.x, 0, c->chunkPos.y);
    if (playerSpacePos.x<0 || playerSpacePos.z<0 || playerSpacePos.x>CHUNKWIDTH || playerSpacePos.z>CHUNKWIDTH) {
        return false;
    }
    else {
        return true;
    }
}
void Player::UpdatePlayerChunk() {
  
    if (!CheckIsInChunk(curChunk)) {
        curChunk = ChunkManager::GetChunk(ChunkManager::Vec3ToChunkPos(playerPos));
        isNeededUpdatingWorld = true;
    }
}