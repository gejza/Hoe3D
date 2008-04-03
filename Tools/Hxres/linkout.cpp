
#include "StdAfx.h"
#include "linkout.h"
#include "linker.h"

using namespace HoeRes;

void ExportNS(Namespace& ns, HoeCore::WriteStream& str, 
			const HoeCore::Endianness& end, bool expheader = true);

LinkExport::LinkExport(const HoeCore::CString name)
	: m_name(name) 
{

}

void LinkExport::Export(Namespace& ns)
{
	HoeCore::File f;
	HoeCore::Stream& s = f;
	HoeCore::Endianness end(HoeCore::Endianness::Low);
	f.Open(m_name, HoeCore::File::hftRewrite);

	//m_obj.QSort();
	Res::MainNamespace head;
	memset(&head, 0, sizeof(head));
	head.id = le_uint32(Res::IDHRESHEADER);
	head.size_struct = be_uint16(sizeof(head));
	head.version_struct = be_uint16(Res::IDHRESVER);
	head.flags = be_uint32((uint32)end.Get());
	head.num_symbols = be_uint32(ns.GetObj().Count() + ns.GetNumNS());
	s.Write(&head, sizeof(head));
	ExportNS(ns, f, end, false);
}

bool operator < (const HoeRes::Res::Symbol& a, const HoeRes::Res::Symbol& b)
{
	return HoeCore::string::cmp(a.name,b.name) < 0;
}

bool operator > (const HoeRes::Res::Symbol& a, const HoeRes::Res::Symbol& b)
{
	return HoeCore::string::cmp(a.name,b.name) > 0;
}

void ExportNS(Namespace& ns, HoeCore::WriteStream& str, 
			const HoeCore::Endianness& end, bool expheader)
{
	// vypocitat poctet symbolu
	int numsymbols = ns.GetObj().Count() + ns.GetNumNS();
	if (expheader)
	{
		Res::Namespace head;
		memset(&head, 0, sizeof(head));
		head.id = HoeRes::Res::IDNamespace;
		head.num_symbols = end.num<uint32>(numsymbols);
		head.size_struct = end.num<uint16>(sizeof(head));
		head.version_struct = end.num<uint16>(1);
		str.Write(&head, sizeof(head));
	}
	if (numsymbols < 1)
		return;
	Res::Symbol * syms = (Res::Symbol *) str.CreateBuffer(sizeof(Res::Symbol)*numsymbols);
	memset(syms, 0, sizeof(Res::Symbol)*numsymbols);
	int nsym = 0;
	for (HoeCore::LList<Namespace>::Iterator it = ns.GetNS();it;it++)
	{
		Res::Symbol& sym = syms[nsym];
		memset(&sym,0, sizeof(sym));
		HoeCore::string::copy(sym.name, it->GetName(), sizeof(sym.name));
		sym.type = end.num<uint32>(HoeRes::Res::IDNamespace);
		sym.position = end.num<uint64>(str.Tell());
		ExportNS(*it, str, end);
		nsym++;
	}
	// Export objs
	for (HoeCore::List<Namespace::Obj>::Iterator it(ns.GetObj());it;it++)
	{
		// export const?

		Res::Symbol& sym = syms[nsym];
		memset(&sym,0, sizeof(sym));
		HoeCore::string::copy(sym.name, it->name, sizeof(sym.name));
		sym.type = it->type;
		sym.position = str.Tell();
		// export obj
		it->file.Flush();
		it->file.Seek(0);
		str.Write(it->file);
		nsym++;
	}
	hoe_assert(nsym == numsymbols);
	HoeCore::qsort(syms, numsymbols);
}

/*

{ "gallery:a:b", IDPicture, 1, 3445  }
{ "gallery:a:c", IDPicture, 1, 3457  }

*/
////////////////////////////////////////////////////////////
// resource
LinkRes::LinkRes(const HoeCore::CString name)
	: m_name(name) 
{
	m_maxsize = 0;
	m_maxnum = 0;

	HoeCore::String_s<200> oc;
	oc.printf("%s.const", name.GetPtr());
	if (!m_fc.Open(oc,HoeCore::File::hftRewrite))
		throw HoeUtils::Error(T("Failed open file '%s' for writing."), oc.GetPtr());

}

void LinkRes::ExportRes(HoeCore::String nsn, Namespace& ns,HoeCore::WriteStream& cpp)
{
	// operate obj
	for (HoeCore::List<Namespace::Obj>::Iterator i(ns.GetObj());i;i++)
	{
		HoeCore::String name = nsn;
		if (nsn.IsEmpty())
			name = i->name;
		else {
			name += ":";
			name += i->name;
		}
		int fout;
		size_t pos;
		ExportFile(i->file, &fout, &pos); 
		cpp.Print("{ ID%s, %d, 0x%x, T(\"%s\") },\n", 
			HoeRes::Res::GetTypeName(i->type),
			fout, pos, name.GetPtr());

		// export constanc
		const Compiler::ConstMap& cst = i->c->GetConsts();
		if (!cst.IsEmpty())
		{
			name.Replace(T(':'), T('.'));
			m_fc.Print("// %s\n", name.GetPtr());
			
			for (Compiler::ConstMap::Iterator iccm(cst);iccm;iccm++)
			{
				m_fc.Print("%s.%s = ", name.GetPtr(), iccm->name.GetPtr());
				tchar buff[120];
				iccm->value.Dump(buff, 100);
				m_fc.Print("%s\n", buff);
			}
			m_fc.Print("\n");
		}
	}
	for (HoeCore::LList<Namespace>::Iterator i(ns.GetNS());i;i++)
	{
		HoeCore::String name = nsn;
		if (nsn.IsEmpty())
			name = i->GetName();
		else {
			name += ":";
			name += i->GetName();
		}
		ExportRes(name, *i, cpp);
	}
}

class Adler : public HoeCore::ReadStream
{
	HoeCore::ReadStream& m_s;
public:
	unsigned long adler;
	Adler(HoeCore::ReadStream& s, unsigned long a) : m_s(s), adler(a) {}
	virtual size_t Read(void* ptr, size_t size)
	{
		size_t s = m_s.Read(ptr, size);
		adler = adler32(adler, (Bytef*)ptr, s);
		return s;
	}
	virtual uint Close(void) { return m_s.Close(); }
};

void LinkRes::ExportFile(HoeCore::File &f, int* fo, size_t* pos)
{
	// find file
	static int l = 0;
	//l = l % 8;
	if (l >= m_rc.Count())
	{
		AddFile();
	}
	*fo = l++;
	f.Flush();
	f.Seek(0);
	*pos = m_rc[*fo].file.Tell();
	HoeCore::WriteStream& out = m_rc[*fo].file;
	Adler a(f, m_rc[*fo].adler);
	m_rc[*fo].size += out.Write(a);
	m_rc[*fo].adler = a.adler;
	//out.CreateSpace(HoeCore::RandInt(10000, 1000000));
}

LinkRes::RF& LinkRes::AddFile()
{
	RF& rc = m_rc.Add();
	rc.name.printf("%sres%d.obj", m_name.GetPtr(), m_rc.Count());
	rc.file.Open(rc.name,HoeCore::File::hftRewrite);
	rc.size = 0;
	rc.adler = adler32(0L, Z_NULL, 0); 
	return rc;
}

void LinkRes::Export(Namespace& ns)
{
	HoeCore::File fcpp;
	HoeCore::String ncpp = m_name.GetPtr(); //todo simple
	ncpp += ".cpp";
	if (!fcpp.Open(ncpp, HoeCore::File::hftRewrite))
		return;

	fcpp.WriteString("// Auto generated file index\n");
	if (1)
		fcpp.WriteString("#include \"StdAfx.h\"\n");
	else
		fcpp.WriteString("#include <windows.h>\n"
			"#include <hoe_types.h>\n"
			"#include <hoe_resfile.h>\n");
	fcpp.WriteString("\n"
		"using namespace HoeRes::Res;\n"
		"\n"
		"HoeRes::SymbolLink g_link[] = {"
		);
	
	ExportRes("", ns, fcpp);
	fcpp.WriteString("{0,0,0,0}};\n\n");

	// export files
	HoeCore::File frc;
	HoeCore::String nrc = m_name.GetPtr(); //todo simple
	nrc += ".rc";
	if (!frc.Open(nrc, HoeCore::File::hftRewrite))
		return;

	frc.WriteString("// Auto generated file resources\n");
	fcpp.WriteString("\nHoeRes::SymbolFile g_link_files[] = { ");
	for (uint i=0;i < m_rc.Count();i++)
	{
		HoeCore::String n = HoeUtils::GetFileName(m_rc[i].name, false);
		fcpp.Print("{ T(\"%s\"), %d, 0x%x }, \n", n.GetPtr(), m_rc[i].size, m_rc[i].adler);
		frc.Print("%s HOERES %s\n", n.GetPtr(), m_rc[i].name.GetPtr());
	}
	fcpp.WriteString("{0,0,0} };\n");

}


