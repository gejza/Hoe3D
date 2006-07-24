
#include <he/app.h>
#include <he/editor.h>
//#include <vector>
//#include <string>
#include <hoe3d.h>
//#include "modelview.h"

class HoeViewApp : public HoeEditor::App
{
public:
	virtual HoeEditor::BaseEditor * CreateEditor()
	{
		return NULL;
	}
};

IMPLEMENT_APP(HoeViewApp)
const char * EditorName = "HoeViewer";



