
#ifndef _HOE_STATES_SYSTEM_
#define _HOE_STATES_SYSTEM_

class HoeStates
{
	float fFogStart;
	float fFogEnd;
	bool texture;
	bool wireframe;
	bool alphatest;
public:
	HoeStates();
	~HoeStates();

	void Reset();
	void Setup2D();
	void Setup2DAlpha();
	void Setup2DAlphaTest();

	void SetupModel();

	void SetupMap();

	void SetupCursor();
	// states func
	void SetupFont();
	void SetupFontBack();

	void SetupSkybox();
	//void ApplyState(HOESTATESBLOCK state);
	//void GetDisplaySize(SIZE * s);
	//BOOL Check();
	//BOOL GetDisplay(HOEDISPLAY &);
	//void RestoreStates(void);
	//void SetValue(HOEVALUETYPE,DWORD);
	//DWORD GetValue(HOEVALUETYPE);

	//void SetRenderState(dword State,dword Value);
	void EnableTexture();
	void DisableTexture();

	void StartWireframe();
	void EndWireframe();

	static int c_setwireframe(int argc, const char * argv[], void * param);
};

#endif // _HOE_STATES_SYSTEM_
