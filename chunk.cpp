


#include "chunk.h"
#include "chunkmanager.h"
#include <FastNoiseLite.h>
#include "chunkmanager.cpp"
#include "ChunkBlocksDic.h"
#include <thread>
#include <mutex>
	//static std::vector < Chunk>Chunks;//¾²Ì¬map
	void Chunk::GenerateHeightmap() {
		for (int x = 0;x < CHUNKWIDTH + 2; x++) {
			for (int z = 0; z < CHUNKWIDTH + 2; z++) {
				float noiseValue= noiseGenerator->GetNoise(1+(float)chunkPos.x + (float)x,  1+(float)chunkPos.y + (float)z);
				thisHeightMap[x][z] = noiseValue;
			}
		}
	}

	Chunk::Chunk(int posX, int posY,Texture tex,FastNoiseLite* noiseGen) {
		chunkPos = ChunkPosition(posX,posY);
		this->noiseGenerator = noiseGen;
		this->texture = tex;
	//	std::thread t([&]() {InitMap(); });
		//
		InitMap();
	}

	void Chunk::InitMap() {
			GenerateHeightmap();
	
		for (int x = 0; x < CHUNKWIDTH; x++) {
			for (int y = 0; y < CHUNKHEIGHT; y++) {
				for (int z = 0; z < CHUNKWIDTH; z++) {
					map[x][y][z] = 0;
				}
			}
		}
		for (int x = 0; x < CHUNKWIDTH; x++) {
			for (int z = 0; z < CHUNKWIDTH; z++) {
				float noiseValue = thisHeightMap[x + 1][z + 1];
				//	std::cout << noiseValue << std::endl;W
				for (int y = CHUNKHEIGHT - 1; y >= 0; y--) {
					if (y < (float)CHUNKHEIGHT / 4.0f + 20.0f * noiseValue) {
						map[x][y][z] = 1;
					}

				}
			}
		}
		for (int x = 0; x < CHUNKWIDTH; x++) {
			for (int z = 0; z < CHUNKWIDTH; z++) {

				for (int y = CHUNKHEIGHT - 1; y >= 0; y--) {
					if (map[x][y][z] == 1 && map[x][y + 1][z] == 0) {
						map[x][y][z] = 4;
					}

				}
			}
		}
		
		//

		BuildMesh(); 
	
		
	//	t.join();
	} 

	 void Chunk::BuildMesh() {
		leftChunk = ChunkManager::GetChunk(ChunkPosition(chunkPos.x - CHUNKWIDTH, chunkPos.y));
		rightChunk = ChunkManager::GetChunk(ChunkPosition(chunkPos.x + CHUNKWIDTH, chunkPos.y));
		frontChunk = ChunkManager::GetChunk(ChunkPosition(chunkPos.x, chunkPos.y + CHUNKWIDTH));
		backChunk = ChunkManager::GetChunk(ChunkPosition(chunkPos.x, chunkPos.y - CHUNKWIDTH));
		vertices = std::vector<Vertex>();
		indices = std::vector<unsigned int>();
		for (int x = 0; x < CHUNKWIDTH; x++)
		{
			for (int y = 0; y < CHUNKHEIGHT; y++)
			{
				for (int z = 0; z < CHUNKWIDTH; z++)
				{
				//	std::cout << "build" << std::endl;
					BuildBlock(x, y, z,&vertices,&indices);
				}
			}
		}
		//std::cout << vertices.size() << std::endl;
	//	std::cout << indices.size() << std::endl;
		isChunkDataBuilt = true;
	//	if (std::find(ChunkManager::Chunks.begin(), ChunkManager::Chunks.end(), this) == ChunkManager::Chunks.end()) {
		
		
		
		
	}
	 void Chunk::ApplyMesh() {
		if (vertices.size() > 0 && indices.size() > 0) {
		this->chunkMesh = Mesh(vertices, indices, std::vector<Texture>{texture});
		isChunkBuildCompleted = true;

		}
		std::vector<unsigned int>().swap(indices);
		std::vector<Vertex>().swap(vertices);
		
		//delete(&vertices);
		
	 }
	void Chunk::BuildBlock(int x, int y, int z, std::vector<Vertex>* vertices, std::vector<unsigned int>* indices) {
		if (map[x] [y][ z] == 0) return;

		int blockID = map[x][y][z];

		//Left
		if (CheckNeedBuildFace(x - 1, y, z))
		//	std::cout << "buildFace" << std::endl;
			BuildFace(blockID, glm:: vec3(x, y, z), glm::vec3(0,1,0), glm::vec3(0,0,1), vertices,indices,0, false);
		//Right
		if (CheckNeedBuildFace(x + 1, y, z))
			BuildFace(blockID, glm::vec3(x + 1, y, z), glm::vec3(0, 1, 0), glm::vec3(0,0,1), vertices, indices,1, true);

		//Bottom
		if (CheckNeedBuildFace(x, y - 1, z))
			BuildFace(blockID, glm::vec3(x, y, z), glm::vec3(0, 0, 1), glm::vec3(1, 0, 0), vertices, indices,2, false);
		//Top
		if (CheckNeedBuildFace(x, y + 1, z))
			BuildFace(blockID, glm::vec3(x, y + 1, z), glm::vec3(0, 0, 1), glm::vec3(1, 0, 0), vertices, indices,3, true);

		//Back
		if (CheckNeedBuildFace(x, y, z - 1))
			BuildFace(blockID, glm::vec3(x, y, z), glm::vec3(0, 1, 0), glm::vec3(1, 0, 0), vertices, indices,4, true);
		//Front
		if (CheckNeedBuildFace(x, y, z + 1))
			BuildFace(blockID, glm::vec3(x, y, z + 1), glm::vec3(0, 1, 0), glm::vec3(1, 0, 0), vertices, indices,5, false);
	}
	void Chunk::BuildFace(int blockID, glm::vec3 corner, glm::vec3 up, glm::vec3 right, std::vector<Vertex>* vertices, std::vector<unsigned int>* indices, int side, bool reversed) {
		int index = vertices->size();
		Vertex vert00=Vertex();
		Vertex vert01 = Vertex();
		Vertex vert11 = Vertex();
		Vertex vert10 = Vertex();
		vert00.Position = corner;
		vert01.Position = corner + up;
		vert11.Position = corner + up + right;
		vert10.Position = corner + right;
		glm::vec3 normal = glm::cross(up, right);
		vert00.Normal = normal;
		vert01.Normal = normal;
		vert11.Normal = normal;
		vert10.Normal = normal;
		//verts.Add(corner);
		//verts.Add(corner + up);
		//verts.Add(corner + up + right);
		//verts.Add(corner + right);

		glm::vec2 uvWidth = glm::vec2(0.0625f, 0.0625f);
		glm::vec2 uvCorner = ChunkBlocksDic::uvCoords[blockID][side];

	//	uvCorner.x += (float)(blockID - 1) / 16.0f;
		vert00.TexCoords = uvCorner;
		vert01.TexCoords = glm::vec2(uvCorner.x, uvCorner.y + uvWidth.y);
		vert11.TexCoords = glm::vec2(uvCorner.x + uvWidth.x, uvCorner.y + uvWidth.y);
		vert10.TexCoords = glm::vec2(uvCorner.x + uvWidth.x, uvCorner.y);
	//	uvs.Add(uvCorner);
		//uvs.Add(new Vector2(uvCorner.x, uvCorner.y + uvWidth.y));
	//	uvs.Add(new Vector2(uvCorner.x + uvWidth.x, uvCorner.y + uvWidth.y));
	//	uvs.Add(new Vector2(uvCorner.x + uvWidth.x, uvCorner.y));
		vertices->push_back(vert00);
		vertices->push_back(vert01);
		vertices->push_back(vert11);
		vertices->push_back(vert10);
		if (reversed) {
			indices->push_back(index + 0);
			indices->push_back(index + 1);
			indices->push_back(index + 2);
			indices->push_back(index + 2);
			indices->push_back(index + 3);
			indices->push_back(index + 0);
		}
		else {
			indices->push_back(index + 1);
			indices->push_back(index + 0);
			indices->push_back(index + 2);
			indices->push_back(index + 3);
			indices->push_back(index + 2);
			indices->push_back(index + 0);
		}
		
	
	}
	short Chunk::GetBlockType(int x, int y, int z) {
		if (x >= CHUNKWIDTH || x < 0 || y >= CHUNKHEIGHT || y < 0 || z >= CHUNKWIDTH || z < 0) {
		//	std::cout << "height:" << thisHeightMap[0][0] << std::endl;
			if (x >= CHUNKWIDTH) {
				if (rightChunk != NULL) {
					return rightChunk->map[0][y][z];
				}
				else return PredictBlockType(thisHeightMap[CHUNKWIDTH+1][z],y);
			}
			if (z >= CHUNKWIDTH) {
				if (frontChunk != NULL) {
					return frontChunk->map[x][y][0];
				}
				else return PredictBlockType(thisHeightMap[x][CHUNKWIDTH+ 1], y);
			}
			if (z < 0) {
				if (backChunk != NULL) {
					return backChunk->map[x][y][CHUNKWIDTH-1];
				}
				else return PredictBlockType(thisHeightMap[x][0], y);
			}
			if (x < 0) {
				if (leftChunk != NULL) {
					return leftChunk->map[CHUNKWIDTH - 1][y][z];
				}

				else return PredictBlockType(thisHeightMap[0][z], y);
			}
		}
		else {
			return map[x][y][z];
		}
	}
	short Chunk::PredictBlockType(float noiseValue, int y) {
		float noiseValue1 = (float) CHUNKHEIGHT/4.0f+noiseValue*20.0f-2.0f;
		if (y < noiseValue1) {
			return 1;
		}
		else {
			return 0;
		}
	}
	bool Chunk::CheckNeedBuildFace(int x, int y, int z)
	{
		if (y < 0) return false;
		short type = GetBlockType(x, y, z);
		switch (type)
		{
		case 0:
			return true;
		default:
			return false;
		}
	}
	//AABB chunkBouundingBox = AABB(glm::vec3(chunkPos.x, 0, chunkPos.y), glm::vec3(chunkPos.x + CHUNKWIDTH, CHUNKHEIGHT, chunkPos.y + CHUNKWIDTH));

	bool Chunk::CheckisInView(glm::vec3 position,glm::vec3 normal) {//ÊÓ×¶ÌÞ³ý
	//	Plane plane = Plane{ normal,glm::length(position-normal)};
		std::vector<glm::vec3> deltas = std::vector<glm::vec3>{
		glm::vec3(chunkPos.x,0,chunkPos.y),
		glm::vec3(chunkPos.x, 0, chunkPos.y + CHUNKWIDTH),
		glm::vec3(chunkPos.x + CHUNKWIDTH, 0, chunkPos.y),
		glm::vec3(chunkPos.x + CHUNKWIDTH, 0, chunkPos.y + CHUNKWIDTH),
		glm::vec3(chunkPos.x,CHUNKHEIGHT,chunkPos.y),
		glm::vec3(chunkPos.x, CHUNKHEIGHT, chunkPos.y + CHUNKWIDTH),
		glm::vec3(chunkPos.x + CHUNKWIDTH, CHUNKHEIGHT, chunkPos.y),
		glm::vec3(chunkPos.x + CHUNKWIDTH, CHUNKHEIGHT, chunkPos.y + CHUNKWIDTH),
		};
		for (int i = 0; i < deltas.size(); i++) {
			deltas[i]= deltas[i] - position;
		}
		//glm::vec3 delta = glm::vec3(chunkPos.x+CHUNKWIDTH/2,0, chunkPos.y + CHUNKWIDTH/2) - position;
		for (int i = 0; i < deltas.size(); i++) {
		if (glm::dot(normal,deltas[i])>0) {//ÉãÏñ»úÇ°·½chunk²»ÌÞ³ý£¬ºó·½ÌÞ³ý
			return true;
		}
		}
		
		
		//	std::cout << "culled" << std::endl;
			return false;
		
	/*	glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(chunkPos.x, 0, chunkPos.y));
		std::vector<glm::vec4> boundingBoxPoints = std::vector<glm::vec4>{
		glm::vec4(chunkPos.x,0,chunkPos.y,1),
		glm::vec4(chunkPos.x, 0, chunkPos.y+CHUNKWIDTH,1),
		glm::vec4(chunkPos.x+CHUNKWIDTH, 0, chunkPos.y,1),
		glm::vec4(chunkPos.x + CHUNKWIDTH, 0, chunkPos.y + CHUNKWIDTH,1),
		glm::vec4(chunkPos.x,CHUNKHEIGHT,chunkPos.y,1),
		glm::vec4(chunkPos.x, CHUNKHEIGHT, chunkPos.y + CHUNKWIDTH,1),
		glm::vec4(chunkPos.x + CHUNKWIDTH, CHUNKHEIGHT, chunkPos.y,1),
		glm::vec4(chunkPos.x + CHUNKWIDTH, CHUNKHEIGHT, chunkPos.y + CHUNKWIDTH,1),
		};
		for (int i = 0; i < boundingBoxPoints.size(); i++) {
			boundingBoxPoints[i] = boundingBoxPoints[i] * model * view *projection;//¾ØÕó³ËÏòÁ¿
		}
		bool result = false;
		for (int i = 0; i < boundingBoxPoints.size(); i++) {
			glm::vec4 tmp = boundingBoxPoints[i];
			if (glm::abs(tmp.x) < 800 || glm::abs(tmp.y) < 600 || glm::abs(tmp.z) < 600) {
				result = true;
			}
			
		}
		return result;*/
	}
	void Chunk::RenderChunk(glm::vec3 camPosition, glm::vec3 cameraNormal,glm::mat4 view,glm::mat4 projection,Shader shader) {
		if (this == NULL) {
			return;
		}
		if (isChunkBuildCompleted == false) {
			return;
		}
		
		if (CheckisInView(camPosition,cameraNormal) == false) {
			return;
		}
		shader.use();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(chunkPos.x,0,chunkPos.y));
		shader.setVec3("lightPos", glm::vec3(chunkPos.x, 0, chunkPos.y)+ glm::vec3(CHUNKWIDTH/2, CHUNKHEIGHT, CHUNKWIDTH/2));
		shader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
		shader.setVec3("viewPos",camPosition);
		shader.setMat4("model", model);
		shader.setMat4("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
		shader.setMat4("view", view);
		chunkMesh.Draw(shader);
	}



