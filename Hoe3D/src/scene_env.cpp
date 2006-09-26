
#include "system.h"
#include "shared.h"
#include "utils.h"
#include "ref.h"
#include "../include/hoefs.h"
#include "scene_env.h"
#include "hoe3d_math.h"
#include "texture_system.h"

/*

Rozdeleni objektu:
modely
 - celistvy objekt, pouze nastavit nejake vlastnosti a pres shade model modyfikovat
 - pres objekty
objekty mapy
 - neobjektove, co model to objekt, neni vazba model objekt
 - ovlivnuji fyziku a optimalizaci vykreslovani
 - teren, steny, hradby

 jak je poskladat??
 musi byt nejak provazane
 daji se borit, pridavat, modyfikovat
 zacnu stenami
 kazda stena sama za sebe?? 
 kazdy teren sam za sebe
 vsechno by se melo radit do skupin a logicky celku, podle kterych se vytvori optimalizace
 nejlepe stromova architektura
 na listech stromu jednotlive objekty
 strom musi byt optimalne navrhnut, udela navrhar levelu
 strom bude take pospojovan spojovym seznamem (z jedne casti stromu je videt jiny strom)
 kazdy vrchol stromu se jmenuje part
 strom bude moci byt optimalizovan za behu
 strom by mel umet sdilet listy
 nebude to vlastne strom ale graf s typem cest



*/
// jak navrhnout levela
// tak aby i s terenem
// teren, voda atd. vsechno se to jmenuje elementy
// building vnitrni a vnejsi

struct WallVec
{
	HoeMath::VECTOR3 pos;
	//dword color;
	HoeMath::VECTOR2 tex;
};

/////////////////////////////////////
void HoePolygon::Create(int n)
{
	nump = n;
	this->m_stream.Create(n, "p2", sizeof(WallVec)*n);
	mat = NULL;
}

void HoePolygon::Render()
{
	m_stream.Set(0);
	if (mat)
	{
		mat->Setup(0xffffffff);
		Ref::DrawFanObject(0, nump);
	}
}
/////////////////////////////////////

HoePolygonSet::HoePolygonSet(HoePolygon * p)
{ 
	poly = p; 
}
void HoePolygonSet::SetPos(float * vv, uint stride)
{
	WallVec * v = (WallVec*)poly->m_stream.Lock();
	for (int i=0;i < poly->nump;i++)
	{
		v->pos = HoeMath::VECTOR3(vv[0],vv[1],vv[2]);
		v++;
		vv = reinterpret_cast<float*>(reinterpret_cast<byte*>(vv)+stride);
	}
	poly->m_stream.Unlock();
}
void HoePolygonSet::SetTex(float * vv, uint stride)
{
	WallVec * v = (WallVec*)poly->m_stream.Lock();
	for (int i=0;i < poly->nump;i++)
	{
		v->tex = HoeMath::VECTOR2(vv[0],vv[1]);
		v++;
		vv = reinterpret_cast<float*>(reinterpret_cast<byte*>(vv)+stride);
	}
	poly->m_stream.Unlock();
}
void HoePolygonSet::SetMaterial(IHoeMaterial * m)
{
	poly->mat = (HoeMaterial*)m;
}




