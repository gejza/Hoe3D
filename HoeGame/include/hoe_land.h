
#ifndef _HOE_GAME_LAND_H_
#define _HOE_GAME_LAND_H_

#include "hoe_game.h"

BEGIN_HOEGAME

class LandPath
{
public:
    struct Point
    {
        HoeMath::Vector2 pos;
        bool virt;
    };
//protected:
    HoeCore::Stack<Point> m_stack;
public:
    void Insert(float x, float y, bool virt);
};

class Land : private HoeCore::Algorythm::Dajkrs
{
    struct GP : public HoeCore::Algorythm::Dajkrs::TGraphPoint
    {
        dword pos;
    };
    GP *m_tile;
    int m_width;
    int m_height;
	HoeCore::List<HoeCore::Algorythm::Dajkrs::TGraphPoint*> m_lists;
	virtual float FCALL w(const TGraphPoint * from, const TGraphPoint * to);
	virtual float FCALL Heuristic(const TGraphPoint * from, const TGraphPoint * to);
public:
    Land();
    virtual ~Land();
    bool Create(int width, int height);
	bool Preprocess(HoeCore::WordTileMap & map);
	bool Find(uint fx,uint fy, uint tx, uint ty, LandPath * out);
};


END_HOEGAME

#endif // _HOE_GAME_LAND_H_

