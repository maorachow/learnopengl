#pragma once
#include <queue>
#include "chunkposition.h"
#include <thread>
#include <FastNoiseLite.h>
#include "vertex.h"
#include "chunk.h"
#include <vector>
#include "chunkmanager.h"
#include <algorithm>
class ChunkLoadingQueue
{
public:
	
	static std::vector<ChunkPosition> loadingChunks;
	static void LoadChunkThread(Texture tex, FastNoiseLite noiseGenerator);
};

