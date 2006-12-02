
#ifndef _HOE_CORE_H_
#define _HOE_CORE_H_

#include "hoe_types.h"
#include "hoe_math.h"
#include "hoe_structures.h"

float SysFloatTime();

namespace HoeCore {

class WordTileMap
{
	word * m_map;
	uint m_width;
	uint m_height;
public:
	WordTileMap();
	WordTileMap(uint width, uint height);
	WordTileMap(const WordTileMap & map)
	{
		m_map = NULL;
		Copy(&map);
	}
	virtual ~WordTileMap();
	const WordTileMap & operator = (const WordTileMap & map)
	{
		Copy(&map);
		return *this;
	}
	void Create(uint width, uint height);
	void Copy(const WordTileMap * map);
	HOE_INLINE word Get(uint x, uint y) 
	{ 
		hoe_assert(m_map);
		hoe_assert(x >= 0 && x < m_width && y >= 0 && y < m_height);
		return m_map[y*m_width+x];
	}
	HOE_INLINE void Set(uint x, uint y, word b) 
	{ 
		hoe_assert(m_map);
		hoe_assert(x >= 0 && x < m_width && y >= 0 && y < m_height);
		m_map[y*m_width+x] = b;
	}
	void Clear(word b=0);
	bool Find(word b, uint * x, uint * y);
	void FloodFill(uint x, uint y, word w);
	// vyplnuje vzalenosti od 0
	void FloodFillPotencial();
	int GetLines(word tile, HoeCore::Set<HoeMath::VLine2Int> &lines);
};

class TimeMeter
{
protected:
	float start;
	float max;
	float total;
	float avarage;
	float percent;
	float pubmax;
	float last;
	int num;
public:
	TimeMeter();
	void ComputeFrame(const float totaltime)
	{
		percent = 100.f * total / totaltime;
		avarage = (total / num) * 1000;
		pubmax = max * 1000;
		num = 0;
		max = 0;
		total = 0.f;
	}
	void Begin();
	void End();
};

dword HashString(const char * str);
HOE_INLINE int FCALL RandInt(int from, int to)
{
	return (rand() % (to-from)) + from;
}
HOE_INLINE int FCALL RandInt(int max)
{
	return (rand() % max);
}
HOE_INLINE float FCALL RandFloat(float from, float to)
{
	return (float)(((double)rand() / (double)RAND_MAX) * (to-from)) + from;
}
HOE_INLINE float FCALL RandFloat(float max)
{
	return (float)(((double)rand() / (double)RAND_MAX) * max);
}

// algoritmy
namespace Algorythm {

class Dajkrs
{
protected:
	struct TGraphPoint
	{
		float g;
		float f;
		bool inopen;
		bool inclosed;
		TGraphPoint * prev;
		TGraphPoint ** souseds;
	};
	class PGraphPoint
	{
	public:
		TGraphPoint * p;
		HOE_INLINE bool operator < (const PGraphPoint & n) const;
		HOE_INLINE bool operator > (const PGraphPoint & n) const;
	};
private:
    HoeCore::Heap<PGraphPoint> m_open;
public:
	bool Create(uint width, uint height);
	bool Process(TGraphPoint * from, TGraphPoint * to);
	virtual float FCALL w(const TGraphPoint * from, const TGraphPoint * to) = 0;
	virtual float FCALL Heuristic(const TGraphPoint * from, const TGraphPoint * to) { return 0.f; }
};

bool Dajkrs::PGraphPoint::operator <(const HoeCore::Algorythm::Dajkrs::PGraphPoint &n) const
{
	return p->f < n.p->f;
}

bool Dajkrs::PGraphPoint::operator >(const HoeCore::Algorythm::Dajkrs::PGraphPoint &n) const
{
	return p->f > n.p->f;
}


}

} // namespace HoeCore


#endif // _HOE_CORE_H_

