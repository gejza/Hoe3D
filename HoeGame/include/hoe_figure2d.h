
/**
   @file    hoe_figure2d.h
   @date    Nov 2004
   @version $Revision: 1.5 $
   @brief   Rozložení 2d scény.
*/

#ifndef _HOE_FIGURE_2D_H_
#define _HOE_FIGURE_2D_H_

#include <hoe3d.h>
#include "hoe_game.h"
#include "hoe_fs.h"
#include "hoe_utils.h"

BEGIN_HOEGAME

class FigureFile : public TextFile
{
};

/**
 * @brief	Rozložení 2d scény.
 * No jasne.. to bude uprava tohoto, tohle bude obsahovat informace
 * o tom co a jak a v jakem poradi se ma kreslit.
 * pres nastavitelne parametry se budou moci pozmenovat infa
 * proste takovy 2d engine, s pojmenovanymi oblastmi
 */
class BaseGui;
class GuiItem;

class Hoe2DFigureBase : public XHoe2DCallback
{
public:
	/** Kontruktor */
	Hoe2DFigureBase();
	/** Destruktor */
	~Hoe2DFigureBase();

	virtual void HOEAPI _Paint(IHoe2D *);
	bool Load(const char * fname);
	virtual BaseGui * CreateGUI(const char * type) = 0;
};

class Hoe2DFigure : public Hoe2DFigureBase
{
protected:
	// seznam vykresleni
	PtrList<GuiItem*> m_list;
public:
	virtual BaseGui * CreateGUI(const char * type);
	virtual void Draw(IHoe2D * hoe2d);
	GuiItem * GetItem(const char * name);
	GuiItem * ReqItem(const char * name);
};

END_HOEGAME

#endif // _HOE_FIGURE_2D_H_

