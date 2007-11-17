
#ifndef _BASE_OBJ_H_
#define _BASE_OBJ_H_

enum TypeObj
{
	Main,
	Model,
	Stream,
	Index,
	Mat
}; 

class Object
{
public:
	Object * m_parent;
	std::string m_name;

	Object(Object * parent);
	virtual bool Parse();

	virtual Object * GetParent();
	virtual TypeObj GetType() = 0;
	virtual Object * Find(std::string name,bool rek);

	virtual int Export(HoeUtils::File *);
};

#endif // _BASE_OBJ_H_

