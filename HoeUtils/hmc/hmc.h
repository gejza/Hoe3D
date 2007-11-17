
#ifndef _HMC_H_
#define _HMC_H_

#include "classes.h"

class HMC : public PrintParser 
{
protected:
	struct Object
	{
		std::string name;
		CObject * pointer;
	};
	std::vector<Object> m_obj;

public:
	int Compile(const char * filein);
	int Link(const char * fileout);
	
	Object * GetObject(const char * name);

	// compile stream
	virtual HoeSL::Parser::StreamParser * BeginStream(const char * name, const char * fvf, bool define);
	virtual HoeSL::Parser::IndexParser * BeginIndex(const char * name, bool define);
	virtual HoeSL::Parser::MaterialParser * BeginMaterial(const char * name, bool define);
	virtual HoeSL::Parser::ModelParser * BeginModel(const char * name, bool define);

};

#endif // _HMC_H_


