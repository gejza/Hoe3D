
#ifndef _HOE_LOG_H_
#define _HOE_LOG_H_

////////////////////////////////
// Log class

class HoeLog
{
public:
	virtual ~HoeLog() {}
	virtual void Log(const char *,...);
	virtual void Delete() = 0;
};

class HoeLogConsole : public HoeLog
{
	char m_name[256];
public:
	HoeLogConsole(const char * name);
	virtual void Log(const char *,...);
	virtual void Delete() { delete this; }
};

class HoeLogFile : public HoeLog
{
	FILE * m_file;
public:
	HoeLogFile(const char * name);
	virtual ~HoeLogFile();
	virtual void Log(const char *,...);
	virtual void Delete() { delete this; }

};

// use LOG(log)->Log("text");
#define LOG(x) if (x) (x)

#endif // _HOE_LOG_H_