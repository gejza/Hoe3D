
#include "StdAfx.h"
#include "../include/hoe_structfile.h"
#include <locale.h>

void yyobjectrestart ( FILE *input_file );
int yyobjectlex();
extern unsigned char *yyobjecttext;
extern int yyobjectleng;
extern int yyobjectlineno;

BEGIN_HOEGAME

static int lex()
{
	int type = yyobjectlex();
	while (type == '\n')
		type = yyobjectlex();
	return type;
}

static int lexn()
{
	return yyobjectlex();
}

ObjectFileParser::ObjectFileParser()
{
	m_f = NULL;
}

ObjectFileParser::~ObjectFileParser()
{
	Close();
}

bool ObjectFileParser::Open(const char *fname)
{
	setlocale(LC_NUMERIC, "C");

	FILE * m_f = fopen(fname, "rt");
	if (!m_f)
		return false;
	yyobjectrestart(m_f);
	return true;
}

void ObjectFileParser::Close()
{
	if (m_f)
		fclose(m_f);
	m_f = NULL;
}

bool ObjectFileParser::ParseObject()
{
	int type = 0;
	if ((type=lex()) == 257)
	{
		// parse jmena
		m_objtype = (char*)yyobjecttext;
		// '{'
		type=lex();
		if (type != '{')
		{
			m_error = "syntax error, `{' requied";
			return false;
		}
		return true;
	}
	if (type != 0)
	{
		m_error = "syntax error on end of file";
	}
	return false;
}

const Property * ObjectFileParser::GetNextProperty()
{
	int type = 0;
	// jmeno
	if ((type=lex()) == 257)
	{
		m_property.SetName((char*)yyobjecttext);
		if (lexn() != '=')
		{
			m_error = "syntax error, `=' requied";
			return NULL;
		}
		// value
		switch (type = lexn())
		{
		case 257:
		case 258:
			m_property.Set((char*)yyobjecttext);
			break;
		case '(': // vector
			if (!ScanVector())
				return NULL;
			break;
		case '\n':
			m_error = "syntax error, empty value";
			return NULL;
		default:
			m_error.printf("syntax error, unknown character %c", type);
			return NULL;
		};
		if (lexn() != '\n')
		{
			m_error = "syntax error, new line requied";
			return NULL;
		}
		// 
		return &m_property;
	}
	if (type != '}')
	{
		m_error = "syntax error, `}' requied";
	}
	return NULL;
}


int ObjectFileParser::GetLineNum()
{
	return yyobjectlineno;
}

bool ObjectFileParser::ScanVector()
{
	int type;
	float vec[100];
	int s = 0;
	while (1)
	{
		if (s >= 100)
		{
			m_error = "vector is too big";
			return false;
		}
		// cislo
		type = lexn();
		switch (type)
		{
		case 259:
		case 260:
			vec[s++] = (float)atof((char*)yyobjecttext);
			break;
		default:
			m_error = "vector is only for numbers";
			return false;
		}
		type = lexn();
		if (type == ')')
		{
			m_property.Set(vec, s);
			return true;
		}
		if (type != ',')
		{
			m_error = "syntax error, `,' or `)' requied";
			return false;
		}
	}
}

END_HOEGAME


