
#include "StdAfx.h"
#include "error.h"

using namespace HoeCore;

Error::Error(void)
{
}

Error::~Error(void)
{
}

UnknownError::UnknownError(const CString name, const CString type)
{
	m_str.printf("Unknown %s `%s'.", (const tchar*)type, (const tchar*) name);
}

ConvertError::ConvertError(const HoeCore::CString name, 
				 const HoeCore::CString from,
				 const HoeCore::CString to)
{
	m_str.printf("Cannot convert attribute `%s' from type `%s' to `%s'.", 
				(const tchar*)name,(const tchar*)from,(const tchar*)to); 
}

DefineError::DefineError(const HoeCore::CString name, Namespace::Obj* obj)
{
	if (obj->location.ident == "")
		m_str.printf("Symbol %s is already defined.", 
			name.GetPtr());
	else if (!obj->location.line)
		m_str.printf("Symbol %s is already defined in %s", 
			name.GetPtr(), 
			obj->location.ident.GetPtr());
	else
		m_str.printf("Symbol %s is already defined in %s(%d)", 
			name.GetPtr(), 
			obj->location.ident.GetPtr(),
			obj->location.line);
}

InternalError::InternalError(const HoeCore::CString func)
{
	m_str.printf("Internal error: Function %s has flag for never run.", func.GetPtr());
}

