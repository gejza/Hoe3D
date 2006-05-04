
#include <string>
#include "xmlparser.h"

namespace HoeUtils {

/*void
parseStory (xmlDocPtr doc, xmlNodePtr cur) {

	xmlChar *atr;
	xmlChar *key;
	 cur = cur->xmlChildrenNode;
	 while (cur != NULL) {
	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"keyword"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			atr = xmlGetProp(cur, (xmlChar*)"pc");
			if (atr)
			{
				printf("(%s)", atr);
				xmlFree(atr);
			}
		    printf("keyword: %s\n", key);
		    xmlFree(key);
 	    }
	cur = cur->next;
	}
    return;
}*/

XMLNode * XMLParser::ParseFile(const char * docname)
{
	xmlNodePtr cur;
	 doc = xmlParseFile(docname);
	
	 if (doc == NULL ) {
		printf("Document not parsed successfully. \n");
		return false;
	}

	 cur = xmlDocGetRootElement(doc);
	
	 if (cur == NULL) {
		printf("empty document\n");
		xmlFreeDoc(doc);
		return false;
	}
	
	 return new XMLNode(cur,doc);

}

XMLNode::XMLNode(xmlNodePtr c, xmlDocPtr d)
{
	this->cur = c;
	this->doc = d;
}

const char * XMLNode::GetName()
{
	return (const char*)cur->name;
}

std::string XMLNode::GetText()
{
	std::string str;
	xmlChar *key;
	key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
	str = (const char*)key;
	xmlFree(key);
	return str;
}

std::string XMLNode::GetAttr(const char * attr)
{
	std::string str;
	xmlChar *atr;
	atr = xmlGetProp(cur, (xmlChar*)attr);
	if (atr)
	{
		str = (const char*)atr;
		xmlFree(atr);
	}
	return str;
}

bool XMLNode::IsName(const char * name)
{
	return !xmlStrcmp(cur->name, (const xmlChar *) name);
}

bool XMLNode::Next()
{
	cur = cur->next;
	return cur != NULL;
}

XMLNode * XMLNode::GetNext()
{
	if (cur->next)
		return new XMLNode(cur->next,doc);
	else
		return NULL;
}

XMLNode * XMLNode::GetChild()
{
	if (cur->xmlChildrenNode)
		return new XMLNode(cur->xmlChildrenNode,doc);
	else
		return NULL;
}

} // namespace HoeUtils


