
#ifndef _HOE_STREAM_
#define _HOE_STREAM_

/**
* Stream - buffer pro vertexy
*/
class HoeStream
{
	char m_fvf[8]; ///< textove fvf
	dword m_dwfvf; ///< zakodovane fvf
	dword m_size; ///< velikost celeho bufferu
	dword m_numvert; ///< pocet vertexu
	bool m_dynamic; ///< dynamicky stream
	bool m_soft; ///< softwarovy stream
	SysVertexBuffer m_vb; ///< hardware vertex buffer
	byte * m_pVertices; ///< ukazatel na zamcene vertexy
	HoeMath::BoundingBox m_box; ///< bounding box
	static dword GetFVF(const char *); 
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
	dword GetFVF() { return m_dwfvf;}
	/**
	* Pocet vertexu
	*/
	inline dword GetNumVert() { return m_numvert; }
	/**
	* Bounding box
	*/
	const HoeMath::BoundingBox * GetBounding() { return &m_box;}
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
