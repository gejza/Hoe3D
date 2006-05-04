
#include "StdAfx.h"
#include "../include/he/utils.h"

wxString HoeEditor::FullPath(const wxString &dir, const wxString &relpath)
{
	wxString fp;
	fp = HoeUtils::fullpath(dir.c_str(), relpath.c_str()).c_str();
	return fp;
}

