#include "ChunkLoadingQueue.h"

std::mutex ChunkLoadingQueue::mtx = std::mutex();
std::vector<ChunkPosition> ChunkLoadingQueue::loadingChunks = std::vector<ChunkPosition>();
void ChunkLoadingQueue::LoadChunkThread(Texture tex,FastNoiseLite noiseGenerator) {
	while (1) {
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	if (loadingChunks.size() > 0) {
		//std::cout << loadingChunks.size() << std::endl;
		mtx.lock();
		ChunkPosition cPos = loadingChunks.back();
		Chunk* c = new Chunk(cPos.x, cPos.y, tex, &noiseGenerator);
		ChunkManager::Chunks.push_back(c);
		loadingChunks.pop_back();
		mtx.unlock();
	}
	}
	

}