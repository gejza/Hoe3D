
#include "StdAfx.h"
#include "../include/HoeCore/const_file.h"
#include "../include/HoeCore/expression.h"

enum Tokens {
	TName = 256,
	TReal,
	TNumber,
	TString,
};

#define ERR(str) do { parser.ParseError(T("Error: ") T(str)); return false; } while (0)


bool HoeCore::ConstParser::ParseValue(HoeCore::ConstParserSAX& parser)
{
	// single
	if (m_value.Count() == 1)
	{
		switch (m_value[0].token)
		{
		case TReal:
			parser.SetConst(m_name, float(m_value[0].real));
			return true;
		case TNumber:
			parser.SetConst(m_name, m_value[0].number);
			return true;
		case TString:
			parser.SetConst(m_name, m_value[0].str);
			return true;
		};
	}
	// scan vice hodnot
	int st = 0;
	bool typed = false;
	if (m_value.Count() >= 2)
	{
		if (m_value[0].token == TName && m_value[1].token == T('('))
			st = 2;
		typed = true;
	}
	HoeCore::List<Universal> vv;
par:
	if (!GetValue(parser, st, vv.Add()))
		return false;
	if (typed && st < m_value.Count() && m_value[st].token == T(','))
	{
		st++;
		goto par;
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

bool HoeCore::ConstParser::GetValue(ConstParserSAX& parser, int& st, HoeCore::Universal& value)
{
	// st + while
	if (st >= m_value.Count())
		return false;

	double p = 0;
	HoeCore::ExpInfix exp;
	HoeCore::ConstParserSAX::ValueName tmp;
	while (st < m_value.Count())
	{
		switch (m_value[st].token)
		{
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
			{

				exp.Flush();
				// konec
				if (!exp.GetRes(value))
					ERR("Syntax error");
				return true;
			}
			else
				exp.AddOperator(m_value[st].token);
			break;
		}
		st++;
	}
	// neco, operator, neco operator, neco
/*
operandy na vstupu posilej primo na vystup 
operator na vstupu, ktery ma vyssi prioritu nez operator na vrcholu zasobniku (nebo je-li zasobnik prazdny), posli do zasobniku 
je-li operator na vstupu nizsi nebo stejne priority jako v zasobniku, posli na vystup operator z vrcholu zasobniku; tento postup opakuj, dokud nenastane situace z predchoziho bodu (t.j. ze na vrcholu zasobniku je operator nizsi priority nez na vstupu nebo je zasobnik jiz prazdny) 
levou zavorku, ktera se objevi na vstupu, presuneme do zasobniku a budeme ji povazovat za docasne relativni dno zasobniku; tim jsme zalozili novou vrstvu zasobniku 
pravou zavorku, ktera se objevi na vstupu, povazujeme za docasny relativni ukoncujici symbol; vyvola odchod vsech symbolu az po nejblizsi relativni dno zasobniku a zrusi tak jeho vrchni vrstvu; obe zavorky (ktere se nyni priblizily na dosah) splnily svou funkci a mizi

	from, to

	a+b+c*d*e
	ab+cd*e*+
*/	

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



