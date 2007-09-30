
/**
   @file    hoe_structfile.h
   @date    Apr 2007
   @version $Revision: 1.5 $
   @brief   Textovy objektovy soubor.
*/

#ifndef _HOE_STRUCT_FILE_H_
#define _HOE_STRUCT_FILE_H_

#include <hoe_structures.h>
#include "hoe_game.h"
#include "hoe_fs.h"
#include "hoe_utils.h"

BEGIN_HOEGAME

class Property
{
public:
	const HoeCore::String_s<256>& GetName() const { return m_name; }
	const HoeCore::Universal & GetValue() const { return m_value; }
	void SetName(const tchar * name) { m_name = name; }
	void Set(const char * value) { m_value = value; }
	void Set(float value) { m_value = value; }
	void Set(unsigned long value) { m_value = value; }
	void Set(long value) { m_value = value; }
	void Set(int value) { m_value = value; }
	void Set(const float * v, int size) { m_value.Set(v, size); }
protected:
	HoeCore::String_s<256> m_name;
	HoeCore::Universal m_value;
};

class DItem : public HoeCore::Universal
{
public:
	const tchar * GetName() const { return m_name; }
	void SetName(const tchar * name) { m_name = name; }
protected:
	HoeCore::String_s<256> m_name;
};

class Dictionary
{
	// key list
public:
	//
	bool IfExist(const char * key) const;

};

/** Parsovani textoveho souboru */
class ObjectFileParser
{
	FILE * m_f;
	Dictionary m_prop;
	HoeCore::String_s<1024> m_error;
	HoeCore::String_s<1024> m_objtype;
	Property m_property;
	// functions
	bool ScanVector();
public:
	/** 
	* Konstruktor
	*/
	ObjectFileParser();
	/**
	* Destruktor
	*/
	virtual ~ObjectFileParser();
	/**
	* otevirani souboru
	*/
	bool Open(const char * fname);
	/**
	* zavreni souboru
	*/
	void Close();
	/**
	* Rozparsuje dalsi objekt
	* @return Vraci zda dalsi objekt existuje nebo je konec souboru
	*/
	bool ParseObject();
	/**
	* Typ objektu ( type {} )
	*/
	const tchar * GetTypeObject() { return m_objtype; }
	/** 
	* Vlastnosti objektu, zbyvajici vlastnosti vrati jako slovnik
	*/
	const Dictionary & GetProperties();
	/**
	* Vraci ukazatel na dalsi vlastnost. 
	* Stavajici je timto volanim zrusena.
	*/
	const Property * GetNextProperty();
	/**
	* Vraci cislo aktualniho radku v souboru
	*/
	int GetLineNum();
	/**
	* Chyba pri parsovani
	*/
	const tchar * GetLastError() { return m_error.IsEmpty() ? NULL:(const tchar *)m_error; }
};

/** Ukladani do textoveho objektoveho souboru
*/
class ObjectFileWriter
{
protected:
	FILE * m_file;
	int m_level;
	void WriteLevel();
public:
	/** 
	* Konstruktor
	*/
	ObjectFileWriter();
	/**
	* Destruktor
	*/
	virtual ~ObjectFileWriter();
	/**
	* otevirani souboru
	*/
	bool Open(const char * fname);
	/**
	* zavreni souboru
	*/
	void Close();
	/**
	* Novy objekt
	*/
	void BeginObject(const char * objname);
	/**
	* Ukonceni objektu
	*/
	void EndObject();
	/**
	* Ulozeni stringu do souboru
	*/
	void AddString(const char * name, const char * str);
	/**
	* Ulozeni floatu
	*/
	void AddFloat(const char * name, float f);
	/**
	* Ulozeni booleanu
	*/
	void AddBool(const char * name, bool b);
	/**
	* Ulozeni hodnoty ve formatu hex
	*/
	void AddHex(const char * name, unsigned long dw);
	/**
	* Ulozeni vektoru do souboru
	*/
	void AddVector(const char * name, float a1,float a2,float a3,float a4);
};

END_HOEGAME

#endif // _HOE_STRUCT_FILE_H_

