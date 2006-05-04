
#include "StdAfx.h"
#include "hoe_settings.h"

extern "C" {
#include "iniparser.h"
}

BEGIN_HOEGAME

Settings::Settings(const char * config_file)
{
	ini = iniparser_load((char*)config_file);

}

Settings::Settings(const char * cmdline, const char * config_file)
{
	const char * config = config_file;
	while (*cmdline == ' ') cmdline++;
	if (*cmdline != '\0')
		config = cmdline;
	ini = iniparser_load((char*)config);

}

Settings::Settings(int argc,char * argv[], const char * config_file)
{
	const char * config = config_file;
	for (int i=1; i < argc; i++)
	{
		if (*argv[i] != '-')
		{
			config = argv[i];
			continue;
		}
	}
	ini = iniparser_load((char*)config);
}

Settings::~Settings()
{
	iniparser_freedict(reinterpret_cast<dictionary *>(ini));
}

const char * Settings::GetString(const char * name,const char * def)
{
	const char * ret = iniparser_getstr(reinterpret_cast<dictionary *>(ini),(char*)name);
	if (ret)
		return ret;
	else
		return def;
}

int Settings::GetInt(const char * name,int def)
{
	return iniparser_getint(reinterpret_cast<dictionary *>(ini),(char*) name, def);
}

double Settings::GetDouble(const char * name,double def)
{
	return iniparser_getdouble(reinterpret_cast<dictionary *>(ini),(char*)name,def);
}

bool Settings::GetBool(const char * name,bool def)
{
	return iniparser_getboolean(reinterpret_cast<dictionary *>(ini), (char*)name, def) != 0;
}

END_HOEGAME

