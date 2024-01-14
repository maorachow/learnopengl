


#include "chunkmanager.h"



BS::thread_pool pool = BS::thread_pool();
ChunkManager::ChunkManager(int vRange, glm::vec3 worldCenterPos, Texture tex, FastNoiseLite noiseGenerator) {
	this->viewRange = vRange;
	this->worldCenterPos = worldCenterPos;
	this->tex = tex;
	this->noiseGenerator = noiseGenerator;

}


Chunk* ChunkManager::GetChunk(ChunkPosition cpos) {
	//std::cout << Chunks.count(cpos) << std::endl;
	for (int i = 0; i < Chunks.size(); i++) {
		if (Chunks[i]->chunkPos == cpos) {
			return Chunks[i];
		}
	}
	return NULL;
}
Chunk* ChunkManager::GetChunkUnloaded(ChunkPosition cpos) {
	//std::cout << Chunks.count(cpos) << std::endl;
	for (int i = 0; i < ChunksUnloaded.size(); i++) {
		if (ChunksUnloaded[i]->chunkPos == cpos) {
			return Chunks[i];
		}
	}
	return NULL;
}
 ChunkPosition ChunkManager::Vec3ToChunkPos(glm::vec3 pos) {
	glm::vec3 tmp = pos;
	tmp.x = glm::floor(tmp.x / (float)CHUNKWIDTH) * CHUNKWIDTH;
	tmp.z = glm::floor(tmp.z / (float)CHUNKWIDTH) * CHUNKWIDTH;
	ChunkPosition value = ChunkPosition((int)tmp.x, (int)tmp.z);
	return value;
}
 short ChunkManager::GetBlock(int x, int y, int z) {
	ChunkPosition chunkPos = ChunkManager::Vec3ToChunkPos(glm::vec3(x, y, z));
	Chunk* c = ChunkManager::GetChunk(chunkPos);
	if (c == NULL) {
		return 0;
	}
	glm::ivec3 chunkSpacePos = glm::ivec3(x, y, z) - glm::ivec3(chunkPos.x, 0, chunkPos.y);
	short value = c->map[chunkSpacePos.x][chunkSpacePos.y][chunkSpacePos.z];
	return value;
}
 short ChunkManager::GetBlock(int x, int y, int z,Chunk* c) {
	 ChunkPosition chunkPos = ChunkManager::Vec3ToChunkPos(glm::vec3(x, y, z));
	
	 if (c == NULL) {
		 return 0;
	 }
	 glm::ivec3 chunkSpacePos = glm::ivec3(x, y, z) - glm::ivec3(chunkPos.x, 0, chunkPos.y);
	 short value = c->map[chunkSpacePos.x][chunkSpacePos.y][chunkSpacePos.z];
	 return value;
 }
void ChunkManager::UpdateWorld() {
	for (int x = worldCenterPos.x - viewRange; x <= worldCenterPos.x + viewRange; x += CHUNKWIDTH) {
		for (int z = worldCenterPos.z - viewRange; z <= worldCenterPos.z + viewRange; z += CHUNKWIDTH) {

			glm::vec3 pos = glm::vec3(x, 0, z);
			ChunkPosition cPos = Vec3ToChunkPos(pos);

			Chunk* c = GetChunk(cPos);
			
			if (c == NULL) {
				
				c = new Chunk(cPos.x, cPos.y, tex, &noiseGenerator);
				Chunks.push_back(c);
		
			}


		}
	}
	for (int i = 0; i < Chunks.size(); i++) {
		Chunk* c = Chunks[i];
		if (c->chunkPos.x - worldCenterPos.x > viewRange+CHUNKWIDTH || c->chunkPos.y - worldCenterPos.z > viewRange + CHUNKWIDTH) {
			for (std::vector<Chunk*>::iterator it = Chunks.begin(); it != Chunks.end(); )
			{
				if (*it == c) {
				it = Chunks.erase(it);
				}
				else {
					it++;
				}
				
			}
			
		}
	}
}
void ChunkManager::RenderAllChunks(glm::vec3 cameraPosition, glm::vec3 cameraNormal, glm::mat4 view, glm::mat4 projection, Shader shader) {


	//std::cout << Chunks.size() << std::endl;
	//		cout << "key:" << iter->first << " value:" << iter->second << endl;
	for (int i = 0; i < Chunks.size(); i++) {
		Chunk* c = Chunks[i];
		//if(glm::frustum())
		c->RenderChunk(cameraPosition, cameraNormal, view, projection, shader);
	}



	//cout << it.first << " " << it.second << endl;


}
void ChunkManager::RebuildAllChunks() {
	for (int i = 0; i < Chunks.size(); i++) {
		Chunk* c = Chunks[i];
		c->BuildMesh();
	}
}