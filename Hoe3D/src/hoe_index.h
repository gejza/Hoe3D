
#ifndef _HOE_INDEX_H_
#define _HOE_INDEX_H_

/**
* Index buffer
*/
class HoeIndex
{
	SysIndexBuffer m_ib; ///< hardware buffer
	word  *m_sw; ///< ukazatel na zamcene indexy
	dword m_num; ///< pocet indexu
	bool m_soft; ///< softwarovy stream
public:
	/**
	* Konstruktor
	* @param soft Pouzit softwarovy index
	*/
	HoeIndex(bool soft = false);
	/**
	* Vytvoreni bufferu. Pri zavolani smaze puvodni
	* @param num_indices Pocet indexu
	*/
	bool Create(dword num_indices);
	/**
	* Vytvoreni bufferu z predanych dat. Pri zavolani smaze puvodni
	* @param num_indices Pocet indexu
	* @param data Predavana data
	*/
	bool Create(dword num_indices, word * data);
	/**
	* Zamceni bufferu a ziskani ukazatele na zamcena data.
	*/
	word * Lock();
	/**
	* Odemceni bufferu.
	*/
	void Unlock();
	/**
	* Dumpnuti indexu do logu
	*/
	void Dump(HoeLog *log);
	/**
	* Handle na buffer
	*/
#ifdef _HOE_D3D_
	inline SysIndexBuffer GetIndexBuffer() { return m_ib; }
#else
	inline word * GetIndexBuffer() { return m_sw; }
#endif
	/**
	* Pocet vertexu
	*/
	inline dword GetNumIndices() { return m_num; }
};

#endif // _HOE_INDEX_H_
