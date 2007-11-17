
#ifndef _SUBMAP_H_
#define _SUBMAP_H_

class Unit
{
public:
	virtual void Destroy() = 0;
	virtual bool SaveToFile(HoeUtils::File *) = 0;
};

class Part
{
private:
	std::vector<Unit*> units;
	std::string id;
public:
	Part();
	virtual bool Process(HoeUtils::XMLNode *);
	virtual bool SaveToFile(HoeUtils::File *);
};

#endif // _SUBMAP_H_

