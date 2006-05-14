
#ifndef _HOE_FILE_H_
#define _HOE_FILE_H_

#include "../include/hoefs.h"

class HoeLogFile : public HoeLog
{
	XHoeFile *m_file;
public:
	HoeLogFile(const char * fname);
	virtual void Log(const char *,...);
	virtual void Delete() { delete this; }
};

#endif // _HOE_FILE_H_

