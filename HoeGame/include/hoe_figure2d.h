
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

BEGIN_HOEGAME

/**
 * @brief	Rozložení 2d scény.
 * No jasne.. to bude uprava tohoto, tohle bude obsahovat informace
 * o tom co a jak a v jakem poradi se ma kreslit.
 * pres nastavitelne parametry se budou moci pozmenovat infa
 * proste takovy 2d engine, s pojmenovanymi oblastmi
 */

class Hoe2DFigure : public XHoe2DCallback
{
private:
	IHoeScene * m_scene;
protected:
	IHoe2D * m_hoe2d;
public:
	/** Kontruktor */
	Hoe2DFigure();

	/** Destruktor */
	~Hoe2DFigure();

	void Show(IHoeScene *);
	void Hide();

	virtual void HOEAPI _Paint(IHoe2D *);

	virtual void OnPaint();

	inline IHoe2D * Get2D() { return m_hoe2d; }
	//inline IHoe3DEngine * Get3D() { return m_hoe3d; }
};

END_HOEGAME

#endif // _HOE_FIGURE_2D_H_

