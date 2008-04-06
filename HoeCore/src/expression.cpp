
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

template <typename A> A UnaryT(A a, tchar op)
{
	switch (op)
	{
	case '-':
		return -a;
	case '+':
		return a;	
	default:
		return a;
	};
}

double HoeCore::ExpMath::Unary(double a, tchar op)
{
	return UnaryT<double>(a, op);
}

long HoeCore::ExpMath::Unary(long a, tchar op)
{
	return UnaryT<long>(a, op);
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

bool HoeCore::ExpPostfix::Add(double d)
{
	if (!m_real) ConvToReal();
	m_stack.Push().real = d;
	return true;
}

bool HoeCore::ExpPostfix::Add(long num)
{
	if (m_real)
		m_stack.Push().real = double(num);
	else
		m_stack.Push().num = num;
	return true;
}

bool HoeCore::ExpPostfix::AddOperator(tchar o)
{
	if (!m_stack.Count())
		return false;
	if (m_stack.Count() == 1)
	{
		if (m_real)
			m_stack.Push().real = this->Unary(m_stack.Pop().real,o);
		else
			m_stack.Push().num = this->Unary(m_stack.Pop().num,o);
		return true;
	}
	// pocitat operator
	if (m_real || (o == T('/') && 
		m_stack.Top(1).num%m_stack.Top().num))
	{
		if (!m_real) ConvToReal();
		double b=m_stack.Pop().real;
		double a=m_stack.Pop().real;
		m_stack.Push().real=this->Compute(a, b, o);
	}
	else
	{
		long b=m_stack.Pop().num;
		long a=m_stack.Pop().num;
		m_stack.Push().num=this->Compute(a, b, o);
	}
	return true;
}

//////// infix ///////////////////////
bool HoeCore::ExpInfix::Add(double n)
{
	return ExpPostfix::Add(n);
}

bool HoeCore::ExpInfix::Add(long n)
{
	return ExpPostfix::Add(n);
}

bool HoeCore::ExpInfix::AddOperator(tchar o)
{
	if (o == T('('))
	{
		m_bracks++;
		m_op.Push(o);
		return true;
	}
	if (o == T(')'))
	{
		if (!m_bracks)
			return false;
		// done az do 
		while (!m_op.IsEmpty() && m_op.Top() != T('('))
			if (!ExpPostfix::AddOperator(m_op.Pop()))
				return false;
		if (m_op.IsEmpty())
			return false;
		// odebrat zacatek zavorky
		m_op.Pop(); 
		m_bracks--;
		return true;
	}

	// process operator
	while (!m_op.IsEmpty() && m_op.Top() != T('(')
		&& ExpPostfix::OpPrior(o) <= ExpPostfix::OpPrior(m_op.Top()))
	{
		if (!ExpPostfix::AddOperator(m_op.Pop()))
			return false;
	}
	m_op.Push(o);
	return true;
}

void HoeCore::ExpInfix::Flush()
{
	while (!m_op.IsEmpty())
		ExpPostfix::AddOperator(m_op.Pop());
}

bool HoeCore::ExpInfix::GetRes(Universal& u)
{
	Flush();
	return ExpPostfix::GetRes(u);
}

bool HoeCore::ExpInfix::IsOper(tchar o)
{
	switch (o)
	{
	case T(')'):
		if (!m_bracks)
			return false;
	case T('('):
		return true;
	default:
		return ExpPostfix::IsOper(o);
	};
}


