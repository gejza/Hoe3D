
#include "StdAfx.h"
#include "../include/HoeCore/const_file.h"
#include "../include/HoeCore/expression.h"

enum Tokens {
	TName = 256,
	TReal,
	TNumber,
	TPixel,
	TString,
};

#define ERR(str) do { parser.ParseError(T("Error: ") T(str)); return false; } while (0)


bool HoeCore::ConstParser::ParseValue(HoeCore::ConstParserI& parser)
{
	int st = 0;
	bool typed = false;
	if (m_value.Count() >= 2 && m_value[0].token == TName && m_value[1].token == T('('))
	{
		st = 2;
		typed = true;
	}
	HoeCore::List<HoeCore::Universal> vv;
	int np = 0;
next_par:
	if (!GetValue(parser, typed ? m_value[0].str:NULL, st, vv.Add(), np++)) // 
		return false;
	if (typed && st < m_value.Count() && m_value[st].token == T(','))
	{
		st++;
		goto next_par;
	}

	if (typed)
	{
		if (st == m_value.Count() || m_value[st].token != T(')'))
			ERR("End `)' missing.");
		else
			st++;
	}

	// run value
	if (typed)
		parser.SetConst(m_name, m_value[0].str, vv);
	else
		parser.SetConst(m_name, vv[0]);

	if (st != m_value.Count())
		ERR("Unknown error");

	return true;
}

bool HoeCore::ConstParser::GetValue(ConstParserI& parser, const tchar* type, int& st, 
		HoeCore::Universal& value, int numv)
{
	// st + while
	if (st >= m_value.Count())
		return false;

	double p = 0;
	HoeCore::ExpInfix exp;
	HoeCore::ConstParserI::ValueName tmp;
	while (st < m_value.Count())
	{
		switch (m_value[st].token)
		{
		case TPixel:
			exp.Add(parser.PixelFunc(m_name, type, numv, 
				m_value[st].number)); break;
		case TNumber:
			exp.Add(m_value[st].number); break;
		case TReal:
			exp.Add(m_value[st].real); break;
		case TName:
			tmp.Set(m_value[st++].str);
			while (st < m_value.Count() && m_value[st].token == T('.'))
			{
				st++;
				if (m_value[st].token != TName) ERR("Name requied.");
				tmp.Add(m_value[st++].str);
			}
			st--;
			{
				HoeCore::Universal u;
				if (!parser.GetConst(tmp, u))
				{
					ERR("not exist");
				}
				if (u.GetType() == Universal::TypeFloat)
					exp.Add(double(u.GetFloat()));
				else
					exp.Add(long(u.GetDecimal()));
			}
			break;
		default:
			if (!exp.IsOper(m_value[st].token))
				goto endval;
			else
				exp.AddOperator(m_value[st].token);
			break;
		}
		st++;
	}
			
endval:
	if (!exp.GetRes(value))
		ERR("Syntax error");
	// neco, operator, neco operator, neco
	return true;
}

// sax

void HoeCore::ConstParserSAX::SetConst(const ValueName& name,
	const Universal& value) // run from sax parser
{
	hoe_assert(name.Count());
	HoeCore::String_s<2048> n;
	n.Join(name, T("."));
	this->SetConst(n, value);
}

void HoeCore::ConstParserSAX::SetConst(const tchar* name,
		const Universal& value)
{
}

void HoeCore::ConstParserSAX::SetConst(const ValueName& name, const tchar* type, const Values& params)
{
}

bool HoeCore::ConstParserSAX::GetConst(const ValueName& name, Universal& value) // run from sax parser
{
	hoe_assert(name.Count());
	HoeCore::String_s<2048> n;
	n.Join(name, T("."));
	return this->GetConst(n, value);
}

bool HoeCore::ConstParserSAX::GetConst(const tchar* name, Universal& value)
{
	return false;
}



