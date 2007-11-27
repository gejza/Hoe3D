#ifndef _HOE_REF_UTILS_
#define _HOE_REF_UTILS_

/*
FVF:
   typ, pocet, pouziti
   f - float
   x - fixed
   p - pozice
   c - barva
   n - normala
   1-9 - pocet

	

*/

/** 
* Pomocna trida pro fvf
*/
class HoeFVF
{
	char m_fvf[8]; ///< textove fvf
	dword m_dwfvf; ///< zakodovane fvf
public:
	/** Konstruktor */
	HoeFVF();
	/** Preklad textoveho fvf na ciselny */
	static dword GetFVF(const char *); 
	/** Vrati ciselne fvf */
	inline dword GetFVF() const { return m_dwfvf; }
	/** Vrati stringove fvf */
	inline const char * GetStringFVF() const { return m_fvf; }
	/** Porovnani se stringovym fvf */
	bool operator == (const char * f) const;
	/** Porovnani s ciselnym fvf */
	bool operator == (const dword f) const ;
	/** Nastaveni fvf */
	void Set(const char * f);
};

#endif // _HOE_REF_UTILS_
