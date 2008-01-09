#pragma once

class Error
{
	HoeCore::String m_str;
public:
	Error(void);
	Error(HoeCore::String str) { m_str = str; }
	virtual ~Error(void);
	const HoeCore::String& GetStr() const { return m_str; }
};
