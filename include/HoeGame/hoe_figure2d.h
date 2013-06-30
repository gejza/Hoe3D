
/**
   @file    hoe_figure2d.h
   @date    Nov 2004
   @version $Revision: 1.5 $
   @brief   Rozložení 2d scény.
*/

#ifndef _HOE_FIGURE_2D_H_
#define _HOE_FIGURE_2D_H_

#include <HoeCore/hoe_structures.h>
#include <hoe3d.h>
#include "hoe_game.h"
#include "hoe_fs.h"
#include "hoe_utils.h"
#include "hoe_gui.h"

namespace HoeGame {

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
class Hoe2DFigureBase : public Hoe2DControl
{
public:
	/** Kontruktor */
	Hoe2DFigureBase();
	/** Destruktor */
	virtual ~Hoe2DFigureBase();
	/** Funkce ktera nacte ze souboru dalsi prvky */
	bool Load(const tchar * fname);
	virtual Gui::Base * CreateGUI(const tchar * type) = 0;
};

class Hoe2DFigure : public Hoe2DFigureBase
{
protected:
	// seznam vykresleni
	HoeCore::MemoryPool m_pool;
	typedef HoeCore::LList<Gui::Item> ItemList;
	ItemList m_list;
public:
	Hoe2DFigure() : m_list(m_pool) {}
	virtual ~Hoe2DFigure() { Clear(); }
	void Clear();
	virtual Gui::Base * CreateGUI(const char * type);
	virtual void Draw(IHoe2D * hoe2d);
	Gui::Item * GetItem(const char * name, Gui::EType type);
	Gui::Item * ReqItem(const char * name, Gui::EType type);
	void Move(const float x, const float y, bool & act);
	bool Click(const float x, const float y);
	const HoeCore::LList<Gui::Item> & GetItems() const { return m_list; }

	template<typename TYPE> TYPE * NewItem()
	{
		return new (m_list.AddForNew(sizeof(TYPE))) TYPE;
	}
};



} // namespace HoeGame

#endif // _HOE_FIGURE_2D_H_

