
#ifndef _HOE_STREAM_
#define _HOE_STREAM_

#include "ref_utils.h"

/**
* Stream - buffer pro vertexy
*/
class HoeStream
{
	HoeFVF m_fvf;
	dword m_size; ///< velikost celeho bufferu
	dword m_numvert; ///< pocet vertexu
	bool m_dynamic; ///< dynamicky stream
	bool m_soft; ///< softwarovy stream
	SysVertexBuffer m_vb; ///< hardware vertex buffer
	byte * m_pVertices; ///< ukazatel na zamcene vertexy
	HoeMath::BoundingBox3 m_box; ///< bounding box
public:
	/**
	* Konstruktor
	* @param dynamic Dynamicky buffer
	* @param soft Softwarovy buffer
	*/
	HoeStream(bool dynamic = false, bool soft = false);
	/**
	* Vytvoreni bufferu. Pri zavolani smaze puvodni
	* @param numvert Pocet vertexu
	* @param fvf String fvf
	* @param size Velikost bufferu v bytech
	*/
	bool Create(dword numvert,const char * fvf,dword size);
	/**
	* Vytvoreni bufferu s daty. Pri zavolani smaze puvodni
	* @param numvert Pocet vertexu
	* @param fvf String fvf
	* @param size Velikost bufferu v bytech
	* @param data Data na vytvoreni
	*/
	bool Create(dword numvert,const char * fvf,dword size, byte * data);
	/**
	* Zamceni bufferu a ziskani ukazatele na zamcena data.
	*/
	byte * Lock();
	/**
	* Odemceni bufferu.
	*/
	void Unlock();
	/**
	* Nastaveni streamu pro rendering. Pouze u nesoftwaroveho bufferu.
	* @param n Slot pro rendering
	*/
	void Set(int n);
	/**
	* Ziskani zakodovaneho fvf
	*/
	dword GetFVF() { return m_fvf.GetFVF();}
	/**
	* Pocet vertexu
	*/
	inline dword GetNumVert() { return m_numvert; }
	/**
	* Bounding box
	*/
	const HoeMath::BoundingBox3 * GetBounding() { return &m_box;}
	/**
	* Geter zda je buffer dynamicky
	*/
	inline bool IsDynamic() { return m_dynamic; }
	/**
	* Dumpnuti streamu do logu
	*/
	void Dump(HoeLog * log);
};

#endif // _HOE_STREAM_
