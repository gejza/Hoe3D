
#include "StdAfx.h"
#include "ref.h"
#include "ref_utils.h"

HoeFVF::HoeFVF()
{
	m_fvf[0] = '\0';
	m_dwfvf = 0;
}

bool HoeFVF::operator == (const char * f) const
{
	return strcmp(m_fvf, f) == 0;
}

bool HoeFVF::operator == (const dword f) const
{
	return m_dwfvf == f;
}

void HoeFVF::Set(const char * f)
{
	assert(strlen(f) < sizeof(m_fvf));
	strcpy(m_fvf, f);
	m_dwfvf = GetFVF(f);
}

dword HoeFVF::GetFVF(const char * f)
{
	dword fvf = 0;
	if (*f == 'p')
	{
		f++;
		fvf |= FVF_XYZ;
	}

	if (*f == 'r')
	{
		f++;
		fvf |= FVF_XYZRHW;
	}

	if (*f == 'n')
	{
		f++;
		fvf |= FVF_NORMAL;
	}

	if (*f == 'd')
	{
		f++;
		fvf |= FVF_DIFFUSE;
	}

	if (*f == 's')
	{
		f++;
		fvf |= FVF_SPECULAR;
	}

	if (*f == '2')
	{
		f++;
		fvf |= FVF_TEX1;
	}

	if (*f == 't')
	{
		f++;
		fvf |= FVF_TEX1;
	}

	if (*f == 't')
	{
		f++;
		fvf |= FVF_TEX2;
	}

	if (*f == '\0')
		return fvf;

	assert(!"bad fvf");
	return 0;
}

