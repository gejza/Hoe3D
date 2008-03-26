
#include "StdAfx.h"
#include "../include/HoeUtils/error.h"

using namespace HoeCore;

HoeUtils::Error::Error(void)
{
}

HoeUtils::Error::Error(const tchar* format, ...)
{
	va_list va;
	va_start(va, format);
	m_str.vprintf(format, va);
	va_end(va);
}


HoeUtils::Error::~Error(void)
{
}

HoeUtils::UnknownError::UnknownError(const CString name, const CString type)
{
	m_str.printf(T("Unknown %s `%s'."), (const tchar*)type, (const tchar*) name);
}

HoeUtils::ConvertError::ConvertError(const HoeCore::CString name, 
				 const HoeCore::CString from,
				 const HoeCore::CString to)
{
	m_str.printf(T("Cannot convert attribute `%s' from type `%s' to `%s'."), 
				(const tchar*)name,(const tchar*)from,(const tchar*)to); 
}

HoeUtils::InternalError::InternalError(const HoeCore::CString func)
{
	m_str.printf(T("Internal error: Function %s has flag for never run."), func.GetPtr());
}



