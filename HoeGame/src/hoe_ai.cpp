
#include "StdAfx.h"
#include "../include/HoeGame/hoe_ai.h"

BEGIN_HOEGAME
namespace AI {

/*void Path::Find(const HoeMath::Vector2 & s, const HoeMath::Vector2 & f, HoeMath::Polygon2 * p)
{
	// najit nejlevejsi bod
	points.Delete();
	points.Push(PathPoint(f,true));
	// ted zjistovat jak obejit polygon
	HoeMath::Vector2 t = f;
	HoeMath::Vector2 tt = s;
	while (p->RightPoint(t,tt))
	{
		points.Push(PathPoint(tt,true));
		t = tt;
		tt = s;
	}
	points.Push(PathPoint(s,true));
}*/

/////////////////////////////////////////////////////////
HoeMath::PolygonEx<HoeMath::Vector2v> * CreatePolygon(HoeCore::Set<HoeMath::SegmentLine2i> &lines)
{
	// vytvorit polygon
	// vzit prvni lajnu
	HoeMath::PolygonEx<HoeMath::Vector2v> * p = new HoeMath::PolygonEx<HoeMath::Vector2v>;
	hoe_assert(lines.Count() > 0);
	HoeMath::Vector2i first,act,next,last;
	last = first = act = lines[0].a;
	next = lines[0].b;
	lines.RemoveIndex(0);
	// musi skoncit kde to zacalo
	// polygon add first
	//FILE * f = fopen("lines.txt","wt");
	bool bsave = true;
	while (1)
	{
		// polygon add  act
		// 
		if (bsave)
			//p->points.Add(HoeMath::Vector2((act.x * m_distX)-m_sizeX*0.5f,(act.y * m_distY)-m_sizeY*0.5f));
			p->points.Add(HoeMath::Vector2v((vfloat)act.x,(vfloat)act.y));
		act = next;

		if (lines.Count() > 0)
		{
			uint i=0;
			for (;i < lines.Count();i++)
				if (lines[i].IfCanConnect(next))
					break;
			if (i == lines.Count())
				break;
			lines.RemoveIndex(i);
			if ((act.x == next.x && act.x == last.x)
				|| (act.y == next.y && act.y == last.y))
				bsave = false;
			else
			{
				bsave = true;
				last = act;
			}
		}
		else break;
	}
	//fclose(f);
	hoe_assert(first == act);
	// add to polygon
	p->ComputeMinMax();
	return p;
}

}
END_HOEGAME



