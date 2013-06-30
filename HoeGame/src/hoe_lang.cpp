
#include "StdAfx.h"
#include "../include/HoeGame/hoe_lang.h"
#include "../include/HoeGame/hoe_console.h"

//extern "C" {
int lang_load(FILE * f);
int lang_parse(char *buffer);
//}

namespace HoeGame {

Lang::Lang()
{
	num_i = 0;
	strings = NULL;
	dim = num_chars = 0;
}

Lang::~Lang()
{
	if (strings)
		free(strings);
}

void Lang::Reset()
{

}

bool Lang::Load(const char * filename)
{
	FILE * f = fopen(filename,"rt");
	if (!f)
	{
		BaseConsole::Printf("Failed open lang file: %s",filename);
		return false;
	}
	if (!lang_load(f))
	{
		fclose(f);
		BaseConsole::Printf("Failed read lang file: %s",filename);
		return false;
	}

	// zjistit zda je to utf-8
	unsigned char utf[3];
	bool code_utf = true;
	fread(utf, 1, 3, f);
	/** @todo Dodelat kodovani */
	if (utf[0] != 0xef) /*!!!*/
	{
		code_utf = false;
		fseek(f,0,0);
	}

	int nadd = 0;

	// ted by to melo cist dokud muze..
	while (1)
	{
		char buff[1024];
		int id = lang_parse(buff);
		if (id == 0 || id < 0)
			break;

		Add(id,buff);
		nadd++;
	}

	fclose(f);

	BaseConsole::Printf("Add %d strings from file %s",nadd,filename);
	return true;
}

void Lang::Add(int id, const char * str)
{
	Add(id,str,strlen(str));	
}

void Lang::Add(int id, const char * s1, int n1)
{
	int i;
	// convert from utf-8
	while ((num_chars + n1 + 1) > dim)
	{
		dim += 3000;
		strings = (char*)realloc(strings,dim);
		if (!strings)
		{
			//BechApp::GetConsole()->Con_Print("Out of memory !!!");
			return; // vyhodit vyjimku
		}
	}

	memcpy(strings + num_chars,s1,n1);
	*(strings + num_chars + n1) = '\0';

	// kontrola id
	i = GetPosByID(id);
	if (i == -1)
		i = num_i++;

	iters[i].id = id;
	iters[i].string = num_chars;

	num_chars += (n1 + 1);
	num_i++;
}

int Lang::GetPosByID(int id)
{
	for (int i=0;i < num_i;i++)
		if (iters[i].id == id)
			return i;
	return -1;
}

const char * Lang::Get(int id)
{
	int i = GetPosByID(id);
	if (i == -1)
		return NULL;

	return (strings + iters[i].string);
}

const char * Lang::GetString(int id)
{
	static char errmsg[256];
	const char * ptr = Get(id);
	if (ptr)
		return ptr;

	sprintf(errmsg,"< %d >",id);
	return errmsg;
}

} // namespace HoeGame


