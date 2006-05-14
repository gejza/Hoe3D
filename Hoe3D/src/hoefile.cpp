

#include "system.h"
#include "shared.h"
#include "utils.h"
//#undef _WIN32
#include "filesystem.h"
#include "shared.h"



/////////////////////////////// log

HoeLogFile::HoeLogFile(const char * fname)
{
	m_file = GetFileSystem()->Open(fname, hftWrite);
	//m_file.Open(fname,HoeFile::hfWrite);		
}

void HoeLogFile::Log(const char * szFormat,...)
{
	static char szBuff[1024];
	va_list args;

	va_start(args, szFormat);
	vsnprintf( szBuff, 1024, szFormat, args );
	va_end(args);

	m_file->Write(szBuff,strlen(szBuff));
	m_file->Write("\n",1);

}



