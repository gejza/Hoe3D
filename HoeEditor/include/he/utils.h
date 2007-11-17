
#ifndef _HOE_EDITOR_UTILS_H_
#define _HOE_EDITOR_UTILS_H_

namespace HoeEditor {

#ifndef SAFE_DELETE
#define SAFE_DELETE(ptr) if (ptr) delete (ptr);ptr = NULL;
#endif

wxString FullPath(const wxString &dir, const wxString &relpath);

// PRESUNOUT
class EditorTool
{
public:
	EditorTool() {}
	virtual ~EditorTool() {}

	virtual void LeftDown(const int x, const int y, const wxMouseEvent & e) {};
	virtual void LeftUp(const int x, const int y, const wxMouseEvent & e) {}
	virtual void RightDown(const int x, const int y, const wxMouseEvent & e);
	virtual void RightUp(const int x, const int y, const wxMouseEvent & e) {}
	virtual void Wheel(const wxMouseEvent & e) {}
	virtual void Move(int relX, int relY, int absX, int absY, const wxMouseEvent & ev) {}
	virtual void Enter(int absX, int absY) {}
	virtual void Leave() {}

	/** Vykresleni nastroje na 2d grafiku */
	virtual void _Paint(IHoe2D * ) {}
	/**
	* Funkce ktera zaridi aby se nastroj "vypnul" a automaticky smaze objekt
	*/
	virtual void Exit() { delete this; }
};

class EditorDragTool : public EditorTool
{
protected:
	bool m_leftdragging;
	bool m_rightdragging;
	int m_mouseleftx;
	int m_mouselefty;
	int m_mouserightx;
	int m_mouserighty;
public:
	EditorDragTool();
	virtual ~EditorDragTool() {}

	virtual void LeftDown(const int x, const int y, const wxMouseEvent & e);
	virtual void LeftUp(const int x, const int y, const wxMouseEvent & e);
	virtual void RightDown(const int x, const int y, const wxMouseEvent & e);
	virtual void RightUp(const int x, const int y, const wxMouseEvent & e);
	virtual void Move(int relX, int relY, int absX, int absY, const wxMouseEvent & ev);
	virtual void Leave();
	/** Zacateg tahnuti, vyvolana az pohnuti drzenou mysi */
	virtual void OnLeftDrag(const int x, const int y) {}
	virtual void OnLeftDrop(const int x, const int y) {}
	virtual void OnRightDrag(const int x, const int y) {}
	virtual void OnRightDrop(const int x, const int y) {}

	inline bool IsLeftDrag() { return m_leftdragging; }
	inline bool IsRightDrag() { return m_rightdragging; }
};

class EditorFileObject
{
	wxString m_filepath;
public:
	const wxString & GetFilePath() { return m_filepath; }
	void SetFilePath(wxString str) { m_filepath = str; }
	bool SaveFile()
	{
		return OnSave(m_filepath);
	}
	virtual bool OnSave(const wxString fname) = 0;
};

}; // namespace HoeEditor


#endif // _HOE_EDITOR_UTILS_H_

