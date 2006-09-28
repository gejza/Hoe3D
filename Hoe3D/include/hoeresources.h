
#ifndef _HOE_RESOURCES_INTERFACE_H_
#define _HOE_RESOURCES_INTERFACE_H_

/**
 * @brief Zakladni trida pro datove objekty.
 */
class IHoeResource : public IHoeInterface
{
public:
	virtual void HOEAPI Reload() {};
	virtual void HOEAPI Free() {};
	virtual void HOEAPI Delete() {};
};

/**
 * @brief Obrazek pro 2D grafiku 
 * @code
 * "picture <jmeno textury>"
 * @endcode
 */
class IHoePicture : public IHoeResource
{
public:
	virtual void CreateRegions(unsigned int x,unsigned int y) = 0;
	virtual void SetActiveRegion(unsigned int) = 0;
	virtual unsigned int GetNumRegions() const = 0;
	virtual unsigned int GetWidth() const = 0;
	virtual unsigned int GetHeight() const = 0;
};

struct THoeFontSize
{
	float width;
};
/**
 * @brief Pismo
 * @code
 * "font <jmeno pisma> [velikost pisma]"
 * @endcode
 */
class IHoeFont : public IHoeResource
{
public:
	
	virtual bool DrawText( float x, float y, dword dwColor, 
                      const char* strText, dword dwFlags=0L ) = 0;
	virtual void GetTextSize(const char *text,THoeFontSize * size) = 0;
	virtual float GetTextHeight() = 0;
};

/**
 * @brief 3D model
 * @code
 * "model <jmeno modelu> [con:<idtext>|file:<jmeno souboru>]"
 * @endcode
 */
class IHoeModel : public IHoeResource
{
public:
	virtual bool HOEAPI GetParameter(const char * name, THoeParameter * parameter) = 0;
};


class IHoeSound : public IHoeResource
{
public:
	virtual void HOEAPI Play(bool loop) = 0;
	virtual void HOEAPI Stop() = 0;
};

class IHoeParticle : public IHoeResource
{
public:
};

class IHoeMaterial : public IHoeResource
{
public:
};

#endif // _HOE_RESOURCES_INTERFACE_H_

