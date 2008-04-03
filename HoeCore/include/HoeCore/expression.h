
#ifndef _HOE_CORE_EXPRESSION_H_
#define _HOE_CORE_EXPRESSION_H_

#include "hoe_structures.h"

namespace HoeCore {

class Universal;

class ExpMath
{
protected:
	double Compute(double a, double b, tchar op);
	long Compute(long a, long b, tchar op);
public:
	static int OpPrior(tchar o);
	bool IsOper(tchar o);
};

class ExpPostfix : public ExpMath
{
	union Num
	{
		double real;
		long num;
	};
	bool m_real;
	HoeCore::Stack<Num> m_stack;
	void ConvToReal();
public:
	ExpPostfix() : m_real(false) {}
	bool IsReal()
	{
		return m_real;
	}
	bool Add(double d)
	{
		if (!m_real) ConvToReal();
		m_stack.Push().real = d;
		return true;
	}
	bool Add(long num)
	{
		if (m_real)
			m_stack.Push().real = double(num);
		else
			m_stack.Push().num = num;
		return true;
	}
	bool AddOperator(tchar o)
	{
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
	bool GetRes(Universal& u);
};

class ExpInfix : public ExpPostfix
{
	HoeCore::Stack<tchar> m_op;
public:
	bool Add(double n)
	{
		return ExpPostfix::Add(n);
	}
	bool Add(long n)
	{
		return ExpPostfix::Add(n);
	}
	bool AddOperator(tchar o)
	{
		while (m_op.Count() && 
			ExpPostfix::OpPrior(o) <= ExpPostfix::OpPrior(m_op.Top()))
		{
			if (!ExpPostfix::AddOperator(m_op.Pop()))
				return false;
		}
		m_op.Push(o);
		return true;
	}
	void Flush()
	{
		while (m_op.Count())
			ExpPostfix::AddOperator(m_op.Pop());
	}
	bool GetRes(Universal& u)
	{
		Flush();
		return ExpPostfix::GetRes(u);
	}
};

} // namespace HoeCore

#endif // _HOE_CORE_EXPRESSION_H_
