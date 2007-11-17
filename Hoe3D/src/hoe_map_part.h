
#ifndef _HOE_MAP_PART_H_
#define _HOE_MAP_PART_H_

class HoeTexture;
class HoeCamera;
class HoeIndex;
class HoeStream;
class SkyBox;

class HoeMapPart
{
	SkyBox * skybox;
public:
	HoeMapPart();

	void Render(HoeCamera *);

	float GetHeight(float x,float y);
	bool GetCamber(const float x1,const float x2,const float y1,const float y2, float & min,float &max);

};


#endif // _HOE_MAP_PART_H_

