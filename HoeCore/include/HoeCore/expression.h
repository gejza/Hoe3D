
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
	double Unary(double a, tchar op);
	long Unary(long a, tchar op);
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
	bool IsReal() { return m_real; }
	bool Add(double d);
	bool Add(long num);
	bool AddOperator(tchar o);
	bool GetRes(Universal& u);
};

class ExpInfix : public ExpPostfix
{
	HoeCore::Stack<tchar> m_op;
	int m_bracks;
	void Flush();
public:
	ExpInfix() : m_bracks(0) {}
	bool Add(double n);
	bool Add(long n);
	bool AddOperator(tchar o);
	bool GetRes(Universal& u);
	bool IsOper(tchar o);
};

} // namespace HoeCore

#endif // _HOE_CORE_EXPRESSION_H_
