
#include "StdAfx.h"
#include "hoe_config.h"
#include "hoe_vars.h"
#include "hoe_console.h"

extern "C"
{
	int config_load(FILE * f);
	int config_parse(char ** text);
}

BEGIN_HOEGAME

Config::Config()
{
	str = NULL;
	size = 0;
}

int Config::Load(const char * filename)
{
	FILE * f = fopen(filename, "r");
	if (!f)
	{
		BaseConsole::Printf("Warning: config file %s not found.", filename);
		return 1;
	}
	config_load(f);
	// parse
	int i;
	char * str;
	int line = 0;
	char var[100];
	while (1)
	{
		i=config_parse(&str);
		if (i==0)
			break;
		if (i==1)
		{
			line++; continue;
		}
		// musi byt jmeno
		if (i!=2)
		{
			// chyba
			continue;
		}
		strncpy(var, str, sizeof(var)-1);
		if (config_parse(&str)!=3)
		{
			// req. =
			continue;
		}
		// value
		switch (config_parse(&str))
		{
		case 4:
			{
				char buffer[1000];
				while (str[0]!='\'' && str[0] != '"' && str[0] != 0) str++;
				strncpy(buffer, str+1,999);
				size_t l = strlen(buffer) - 1;
				assert(buffer[l] == '\'' || buffer[l] == '"');
				buffer[l] = '\0';
				this->Var(var, buffer);
			}
			break;
		case 5:
			this->Var(var, getenv(str+1));
			break;
		case 6: // cislo
			while (str[0]!='+' && str[0]!='-' && (str[0]<'0' || str[0]>'9') && str[0]!=0) str++;
			assert(str[0]!=0);
			this->Var(var, atoi(str));
			break;
		case 9: // float cislo
			while (str[0]!='+' && str[0]!='-' && (str[0]<'0' || str[0]>'9') && str[0]!=0) str++;
			assert(str[0]!=0);
			this->Var(var, (float)atof(str));
			break;
		case 7:
			this->Var(var, true);
			break;
		case 8:
			this->Var(var, false);
			break;
		default:
			//error
			break;
		}
	}
	fclose(f);
	return 0;
}

void Config::Var(const char * varname, bool value)
{
	Var(varname, value ? "true":"false");
}

void Config::Var(const char * varname, int value)
{
	char str[100];
	sprintf(str, "%d",value);
	Var(varname, str);
}

void Config::Var(const char * varname, float value)
{
	char str[100];
	sprintf(str, "%f",value);
	Var(varname, str);
}

bool Config::LoadToModify(const char * filename)
{
	if (str)
		delete [] str;
	str = NULL;
	FILE * f = fopen(filename, "rt");
	if (f)
	{
		fseek(f, 0, SEEK_END);
		size = ftell(f);
		fseek(f, 0, SEEK_SET);
		size += 100;
		// read
		str = (char*)malloc(size);
		size_t t = 0;
		while (fgets(str+t, size-t, f) != NULL)
		{
			while (str[t] != 0) t++;
		}
		fclose(f);
		return true;
	}
	return false;
}

void Config::Set(const char * varname, const char * value)
{
	// najit v bufferu hodnotu
	if (!str)
	{
		size = 100;
		str = (char*)malloc(size);
		str[0] = 0;
	}
	
	size_t line = 0;
	bool first = true;
	while (str[line] != 0)
	{
		if (first && str[line]==varname[0])
		{
			// musi byt na zacatku radky
			size_t t = 0;
			while(str[line+t]==varname[t]) t++;
			if (varname[t] !=0 || (str[line+t] != '=' && str[line+t] != ' ' && str[line+t] != '\t'))
			{
				line++;
				continue;
			}

			// naslo
			line += t;
			while (str[line] && str[line]!= '=' && str[line] != '\n') line++;
			// if (str[line]!= '=') error
			if (str[line]=='=')
				line++;
			// zachovat formatovani
			while (str[line]==' '||str[line]=='\t') line++;
			/*else
				error*/
			// nahradit celou radku az do konce radky
			t=0;
			while (str[line+t]&&str[line+t]!='\n') t++;
			// v t je pocet nahrazenych znaku
			size_t nt = strlen(value);
			if (nt < t) /* zmensit misto */
			{
				char * s = str + line;
				do
				{
					*s = s[t-nt];
				} while (*s++ != 0);
			}
			else if (nt > t) // zvetsit misto
			{
				size_t sstr = strlen(str);
				Realloc(sstr+nt-t);
				char * s = str+sstr;
				do
				{
					s[nt-t] = *s;
				} while (s-- >= str+line);
			}
			// nahrazeni
			while (nt-- > 0)
			{
				str[line] = *value;
				line++;value++;
			}
			return;
		}
		if (str[line] != ' ' && str[line] != '\t')
			first = str[line] == '\n';
		line++;
	}

	// nenalezeno
	size_t l = strlen(str);
	char buff[1000];
	if (l==0 || str[l-1]=='\n')
		sprintf(buff, "%s = %s\n", varname, value);
	else
		sprintf(buff, "\n%s = %s\n", varname, value);
	Realloc(l+strlen(buff));
	strcat(str, buff);
}

bool Config::Realloc(size_t max)
{
	if (max+3 > size)
	{
		size = max+50;
		str = (char*)realloc(str, size);
	}
	return str != NULL;
}

bool Config::Save(const char * filename)
{
	FILE *f = fopen(filename, "wt");
	if (!f)
		return false;
	fputs(str,f);
	fclose(f);
	return false;
}

#define ISWHITE(c) ((c) == ' ' || (c) == '\t')

void Config::ParseArgs(HOE_INSTANCE hInstance, const char *cmdline)
{
	char arg[1000] = {0};
	strncpy(arg, cmdline, sizeof(arg)-1);
	int argc = 0;
	char * argv[100];
#ifdef _WIN32
	char moduleName[ MAX_PATH ];
	if ( !GetModuleFileName( hInstance, moduleName, MAX_PATH ) )
	{
		return;
	}
	argv[argc++] = moduleName;
#endif
	char * a = arg;
	while (1)
	{
		bool uv = false;
		while (ISWHITE(*a)) a++;
		if (*a == 0)
			break;
		if (*a == '\"')
		{
			uv = true;
			a++;
		}
		argv[argc++] = a;
		if (uv)
		{
			while (*a && *a!='\"') a++;
		}
		else
		{
			while (*a && !ISWHITE(*a)) a++;
		}
		if (*a)
			*a++ = 0;
	}
	ParseArgs(argc, argv);
	//free(arg);
}

void Config::ParseArgs(int argc, char *argv[])
{
	for (int i=1;i < argc;)
	{
		int r = Arg(argc-i, &argv[i]);
		if (r < 1)
			return;
		i+=r;
	}
}

int Config::Arg(int argc, char *argv[])
{
	return argc;
}


////////////////////////////////////////////////////////////
void ConfigVars::Var(const char * varname, const char * value)
{
	CVar *v = CVar::GetVar(varname);
	if (v)
	{
		v->Set(value);
	}
	else
	{
		BaseConsole::Printf("Warning: Unknown option %s", varname);
	}
}

void ConfigVars::PushVar(CVar & var)
{
	Set(var.GetName(), var.GetStringValue());
}

void ConfigVars::PushVar(const char * varname)
{
	CVar * v = CVar::GetVar(varname);
	if (v)
		PushVar(*v);
	else
		BaseConsole::Printf("Warning: Unknown option %s", varname);
}

END_HOEGAME
