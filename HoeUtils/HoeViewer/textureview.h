

#ifndef _TEXTURE_VIEW_H_
#define _TEXTURE_VIEW_H_

class TextureView : public HoeEditor::FileEditor
{
protected:
	HoeEditor::EngineView m_view;

	// fpath to open
	wxString m_fpath;

public:
	virtual HoeEditor::EngineView * GetEngineView() { return &m_view; }
	virtual IHoeFS * GetFS() { return NULL; }
};

class TexViewApp : public HoeEditor::App
{
public:
	virtual HoeEditor::BaseEditor * CreateEditor();
};

#endif // _TEXTURE_VIEW_H_

