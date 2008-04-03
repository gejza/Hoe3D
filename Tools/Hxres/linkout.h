
#ifndef _HXRES_LINKOUT_H_
#define _HXRES_LINKOUT_H_

// save to file
// save to rc, datas, cpp, h

class Namespace;

class LinkExport
{
	const HoeCore::CString m_name;

public:
	LinkExport(const HoeCore::CString name);
	void Export(Namespace& ns);
};

class LinkRes
{
	struct RF {
		HoeCore::String name;
		HoeCore::File file;
		int size;
		unsigned long adler; // todo 64bit
	};
	// const
	HoeCore::File m_fc;

	const HoeCore::CString m_name;
	int m_maxsize;
	int m_maxnum;
	HoeCore::List<RF> m_rc;

	void ExportFile(HoeCore::File &f, int* fo, size_t* pos);
	void ExportRes(HoeCore::String nsn, Namespace& ns,HoeCore::WriteStream& cpp);
	RF& AddFile();
public:
	LinkRes(const HoeCore::CString name);
	void Export(Namespace& ns);
};

#endif // _HXRES_LINKOUT_H_


