
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

class Hoe2DFigure : public XHoe2DCallback
{
protected:
	// seznam vykresleni
public:
	/** Kontruktor */
	Hoe2DFigure();
	/** Destruktor */
	~Hoe2DFigure();

	virtual void HOEAPI _Paint(IHoe2D *);
	virtual bool Load(const char * fname);
	virtual BaseGui * CreateGUI(const char * type) = 0;
};

END_HOEGAME

#endif // _HOE_FIGURE_2D_H_

