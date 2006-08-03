
#include "StdAfx.h"
#include "../include/he/undolist.h"
//#include "../include/he/app.h"
//#include "../include/he/.h"

namespace HoeEditor {

UndoList::UndoList()
{
	m_maxactions = 20;
}

UndoList::~UndoList()
{
}

void UndoList::Add(UndoAction * action)
{
}

void DeleteList()
{
}

} // namespace HoeEditor
