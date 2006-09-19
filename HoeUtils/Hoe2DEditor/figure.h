
/**
   @file    figure.h
   @date    Aug 2006
   @version 1.0
   @brief   Soubor s deklaraci tridy pro zobrazovani 2d.
*/

#ifndef _HOE_2D_EDITOR_FIGURE_H_
#define _HOE_2D_EDITOR_FIGURE_H_

class BaseItem;

/*

static
{ 
 name = ""
 rect = (10 10 100 20)

}
radio
{
}
button
check
text


*/

class FigureEdit : public HoeGame::Hoe2DFigureBase, public HoeEditor::EditorFileObject
{
protected:
	IHoeScene * m_scene;
	IHoe3DEngine * m_eng;
	wxTreeItemId m_root;
	wxTreeCtrl * m_tc;
	std::vector<BaseItem*> m_list; 
public:
	FigureEdit(wxTreeCtrl * tc);
	bool Create(IHoe3DEngine * eng);
	void Delete();
	inline wxTreeCtrl * GetTreeCtrl() const { return m_tc; }
	virtual void HOEAPI _Paint(IHoe2D * );
	void AddItem(BaseItem * item, const wxString name);
	virtual bool OnSave(const wxString fname);
	virtual HoeGame::Gui::Base * CreateGUI(const char * type);
};

#endif // _HOE_2D_EDITOR_FIGURE_H_

