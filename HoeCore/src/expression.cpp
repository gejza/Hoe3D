
#include "StdAfx.h"
#include "../include/HoeCore/expression.h"
#include "../include/HoeCore/hoe_universal.h"

//// base ////
template <typename A, typename B> A ComputeT(A a, B b, tchar op)
{
	switch (op)
	{
	case '*':
		return a * b;
	case '/':
		return a / b;
	case '-':
		return a - b;
	case '+':
		return a + b;	
	default:
		return a;
	};
}

double HoeCore::ExpMath::Compute(double a, double b, tchar op)
{
	return ComputeT<double, double>(a,b,op);
}

long HoeCore::ExpMath::Compute(long a, long b, tchar op)
{
	return ComputeT<long, long>(a,b,op);
}

int HoeCore::ExpMath::OpPrior(tchar o)
{
	switch (o)
	{
	case '*':
	case '/':
		return 3;
	case '-':
	case '+':
		return 1;
	default:
		return 0;
	}
}

bool HoeCore::ExpMath::IsOper(tchar o)
{
	return OpPrior(o) > 0;
}

/////////////////////////////////

void HoeCore::ExpPostfix::ConvToReal()
{
	if (m_real) return;
	for (uint i=0;i < m_stack.Count();i++)
	{
		m_stack[i].real = double(m_stack[i].num);
	}
	m_real = true;
}

bool HoeCore::ExpPostfix::GetRes(Universal& u)
{
	if (m_stack.Count() != 1)
		return false;
	if (m_real)
		u = Universal::TReal(m_stack.Top().real);
	else
		u = Universal::TDecimal(m_stack.Top().num);
	return true;
}


