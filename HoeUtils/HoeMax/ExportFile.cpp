
#include <Max.h>
#include <stdio.h>
#include "ExportFile.h"

bool ModelExportFile::Open(const char * name)
{
	// Open the stream
	m_file = _tfopen(name,_T("wt"));
	if (!m_file) 
	{
		return false;
	}
	return true;
}

void ModelExportFile::Printf(const char * szFormat,...)
{
	static char szBuff[1024];

	va_list args;

	va_start(args, szFormat);
	_vsntprintf( szBuff, 1024, szFormat, args );
	va_end(args);

	fputs(szBuff,m_file);
}
	
void ModelExportFile::Close()
{
	fclose(m_file);
}


