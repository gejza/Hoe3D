
#ifndef _SCENE_GRAPH_H_
#define _SCENE_GRAPH_H_

// vrcholy okynka a vse pro optimalizaci sceny
// normalni vrchol stromu
// urcuje obsahuje velikost (kam az zasahuje) (vsech part i podobjektu)

struct TSceneGroup
{
	TSceneGroup * next;
	enum Type {
		ePolygon,
		eTerrain,
	} type;
	union {
		class HoePolygon * p;
		class HoeQuadTerrain * ter;
	} value;
};

#endif // _SCENE_GRAPH_H_

