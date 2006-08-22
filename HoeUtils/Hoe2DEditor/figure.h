
/**
   @file    figure.h
   @date    Aug 2006
   @version 1.0
   @brief   Soubor s deklaraci tridy pro zobrazovani 2d.
*/

#ifndef _HOE_2D_EDITOR_FIGURE_H_
#define _HOE_2D_EDITOR_FIGURE_H_

class FigureEdit : public XHoe2DCallback
{
	IHoeScene * m_scene;
public:
	bool Create(IHoe3DEngine * eng);
	virtual void HOEAPI _Paint(IHoe2D * );
};

#endif // _HOE_2D_EDITOR_FIGURE_H_

