
#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <hoe3d.h>
#include "hoe_game.h"

BEGIN_HOEGAME

class Settings
{
protected:
	void * ini; // dictionary
public:
	Settings(const char * config_file);
	Settings(const char * cmdline, const char * config_file);
	Settings(int argc,char * argv[], const char * config_file);
	~Settings();
	virtual const char * HOEAPI GetString(const char * name,const char * def);
	virtual int HOEAPI GetInt(const char * name,int def);
	virtual double HOEAPI GetDouble(const char * name,double def);
	virtual bool HOEAPI GetBool(const char * name,bool def);

};

END_HOEGAME

#endif // _SETTINGS_H_

