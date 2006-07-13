
#ifndef _MODEL_LOADER_H_
#define _MODEL_LOADER_H_

#include "../include/hoefs.h"

class FileSystem;
class XHoeFile;
class MaterialSystem;
class HoeModel;
class HoeStream;
class HoeIndex;
struct hfm_chunk;
struct tChunk;

class ModelLoader
{
	HoeFileReader m_reader;

	HoeLog * m_log;
	int m_flags;

	bool m_soft;

	HoeModel * LoadModel();
public:
	ModelLoader(HoeLog * log = NULL, int flags = 0);
	HoeModel * LoadModel(const char * name, bool soft);

	HoeStream * GetStream(int id);
	HoeIndex * GetIndex(int id);
	HoeMaterial * GetMaterial(int id);
};


#endif // _MODEL_LOADER_H_

