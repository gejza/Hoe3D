
#ifndef _HOE_UTILS_ERROR_H_
#define _HOE_UTILS_ERROR_H_
#pragma once

namespace HoeUtils {


class Error
{
protected:
	HoeCore::String m_str;
public:
	Error(void);
	Error(const tchar* format, ...);
	Error(HoeCore::String str) { m_str = str; }
	virtual ~Error(void);
	const HoeCore::String& GetStr() const { return m_str; }
};

class UnknownError : public Error
{
public:
	UnknownError(const HoeCore::CString name, const HoeCore::CString type);
};

class ConvertError : public Error
{
public:
	ConvertError(const HoeCore::CString name, 
				 const HoeCore::CString from,
				 const HoeCore::CString to);
};

class InternalError : public Error
{
public:
	InternalError(const HoeCore::CString func);
};

} // namespace HoeUtils

#endif // _HOE_UTILS_ERROR_H_


