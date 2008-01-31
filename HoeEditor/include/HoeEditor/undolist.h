
#ifndef _HOE_EDITOR_UNDOLIST_H_
#define _HOE_EDITOR_UNDOLIST_H_

namespace HoeEditor {

class UndoAction
{
	UndoAction * m_prev;
	UndoAction * m_next;
public:
	UndoAction();
	virtual ~UndoAction();
	virtual void OnUndo() = 0;
	virtual void OnRedo() = 0;

	friend class UndoList;
};

class UndoList
{
protected:
	uint m_maxactions;
	uint m_numactions;
	UndoAction * m_list;
	void DeletePrevList(UndoAction * act);
public:
	UndoList();
	~UndoList();
	void Add(UndoAction * action);
	void DeleteList();
	bool CanUndo();
	bool CanRedo();
	void Undo();
	void Redo();
};

}; // namespace HoeEditor

#endif // _HOE_EDITOR_UNDOLIST_H_

