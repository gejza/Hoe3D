
#ifndef _XMLPARSER_H_
#define _XMLPARSER_H_

#include <libxml/parser.h>

#include "utils.h"

namespace HoeUtils {

class XMLNode
{
	xmlNodePtr cur;
	xmlDocPtr doc;
public:
	XMLNode(xmlNodePtr c, xmlDocPtr d);
	const char * GetName();
	bool IsName(const char *);
	bool Next();
	XMLNode * GetNext();
	XMLNode * GetChild();
	std::string GetText();
	std::string GetAttr(const char * attr);
};

class XMLParser
{
protected:
	xmlDocPtr doc;
public:
	XMLNode * ParseFile(const char * name);
};

} // namespace HoeUtils

#endif // _XMLPARSER_H_
