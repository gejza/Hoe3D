
#ifndef _HOE_MAP_FILE_
#define _HOE_MAP_FILE_

#define IDMAPHEADER	('H'+('M'<<8)+('a'<<16)+('p'<<24))

#define IDMAPVER 113

#pragma pack(push, 4)

namespace FileMap {

struct HoeMapHeader
{
	unsigned long id;
	int ver;
	// leafs size
	// 
};

enum MapSectionType
{
	MapSectionTerrain = 1,
	MapSectionObject = 1010,
	MapSectionFileEnd = 0x8fffffff
};

struct MapSection
{
	unsigned long type;
	int id;
	unsigned int size;
};

struct MapTerrain
{
	float distX; // vzdalenost mezi pixlama v bitmape
	float distY; // vzdalenost mezi pixlama v bitmape
	int sizeX; // velikost vyskovy
	int sizeY;
	int rectX; // velikost ctverce
	int rectY; 
};

} // namespace FileMap

#pragma pack(pop)

#endif //_HOE_MAP_FILE_
