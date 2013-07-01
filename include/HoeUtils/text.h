
#ifndef _HOE_UTILS_TEXT_H_
#define _HOE_UTILS_TEXT_H_

// core/stream

namespace HoeUtils {

class TextProcessor : public HoeCore::WriteStream
{
protected:
	HoeCore::WriteStream& m_out;
	bool m_nl;
public:
	TextProcessor(HoeCore::WriteStream& out);
	virtual uint Close();
	virtual size_t Write(const void* ptr, size_t size);
	virtual size_t PreLine();
};

class IndentStream : public TextProcessor
{
	int m_level;
public:
	IndentStream(HoeCore::WriteStream& s);
	virtual size_t PreLine();
	void Indent(int l);
};

} // namespace HoeUtils

#endif // _HOE_UTILS_TEXT_H_

