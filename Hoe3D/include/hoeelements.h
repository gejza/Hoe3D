
#ifndef _HOE_ELEMENTS_INTERFACE_H_
#define _HOE_ELEMENTS_INTERFACE_H_

class IHoeElement : public IHoeInterface
{
};

class IHoeCamera : public IHoeElement
{
public:
	virtual void HOEAPI Set(const HoeMath::Vector3 & pos,
		const HoeMath::Vector3 & look) = 0;
	virtual void HOEAPI Pick(const float x, const float y, 
		HoeMath::Vector3 * vPickRayDir, HoeMath::Vector3 * vPickRayOrig) = 0;
	virtual void HOEAPI GetSize(int *w, int *h) const = 0;
};


/**
 * @brief Kurzor
 * @code
 * "cursor [resource]"
 * @endcode
 */
class iHoeCursor : public IHoeElement
{
public:

	/**
	 * Zobraz� kurzor 
	 */
	virtual void Show() = 0;

	/**
	 * Schov� kurzor 
	 */
	virtual void Hide() = 0;
	/**
	 * Nastav� pozici kurzoru 
	 */
	virtual void SetPos(int x,int y) = 0;
	/** 
	 * Nahraje kurzor ze souboru 
	 */
	virtual bool LoadCursor(int px,int py,const char * lpCursorName) = 0;

	/**
	 * Propoj� kurzor se vstupem my�i.
	 */
	virtual bool AcquireInput() = 0;

	/**
	 * Odpoj� kurzor od my�i.
	 */
	virtual void UnacquireDevice() = 0;

	/**
	 * Nastavi funkci pro p��jem kurzorov�ch zpr�v.
	 */
	//virtual void SetCallback(HoeInput::MsgCallback) = 0;
};

class IHoeParticleEmitor : public IHoeResource
{
public:
	virtual void Start() = 0;
	virtual void Stop() = 0;
};

class IHoeLight : public IHoeElement
{
public:
	virtual void HOEAPI SetPosition(const HoeMath::Vector3 & pos) = 0;
	virtual void HOEAPI SetColor(const float r, const float g, const float b) = 0;
};

class IHoeSoundPlayer : public IHoeElement
{
public:
	virtual void SetSound(IHoeSound * sound) = 0;
	virtual void Play() = 0;
	virtual void Stop() = 0;
};

#endif // _HOE_ELEMENTS_INTERFACE_H_

