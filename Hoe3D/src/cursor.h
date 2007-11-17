
#ifndef _HOE_CURSOR_H_
#define _HOE_CURSOR_H_

class HoeCursor : public iHoeCursor/*, public XHoeMouse*/
{
private:
	static HoeCursor * m_act;

	uint XHotSpot;
	uint YHotSpot;
	HoeTexture * res;

	int x;
	int y;

	bool m_show;

	//HoeInput::MsgCallback m_msgcb;
public:
	/** Kontruktor */
	HoeCursor();

	static void Draw();

	/**
	 * Zobrazí kurzor 
	 */
	virtual void Show();

	/**
	 * Schová kurzor 
	 */
	virtual void Hide();
	/**
	 * Nastaví pozici kurzoru 
	 */
	virtual void SetPos(int x,int y);

	virtual void GetVirtualPos(float *x, float *y);
	/** 
	 * Nahraje kurzor ze souboru 
	 */
	virtual bool LoadCursor(int px,int py,const char * lpCursorName);

	/**
	 * Propojí kurzor se vstupem myši.
	 * @param type Typ kurzoru
	 */
	virtual bool AcquireInput();

	/**
	 * Odpojí kurzor od myši.
	 */
	virtual void UnacquireDevice();

	/**
	 * Nastavi funkci pro příjem kurzorových zpráv.
	 */
	//virtual void SetCallback(HoeInput::MsgCallback);

	static void AxisFunc(const int axis, const float time);
	static void ButtonFunc(const int butt, const int phase);
};

#endif // _HOE_CURSOR_H_

