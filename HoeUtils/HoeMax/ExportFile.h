
#ifndef _HOE_EXPORT_FILE_
#define _HOE_EXPORT_FILE_ 

class ModelExportFile
{
	// seznam radek
	FILE * m_file;
public:
	bool Open(const char * filename);
	void Printf(const char * szFormat,...);
	void Close();
};

#endif // _HOE_EXPORT_FILE_

