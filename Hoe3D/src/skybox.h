
/**
   @file    skybox.h
   @version $Revision: 1.5 $
   @brief   Skybox a dalsi veci z okolniho sveta
*/
/*
	$Id: skybox.h,v 1.5 2005/03/02 14:19:12 gejza Exp $
	$Author: gejza $
	$Date: 2005/03/02 14:19:12 $
	$Revision: 1.5 $
*/ 

#ifndef _HOE_SKYBOX_H_
#define _HOE_SKYBOX_H_

class HoeCamera;
class HoeTexture;

/**
 * @brief Skybox
 */
class SkyBox
{
	/** Strany kostky skyboxu */
	enum Sides
	{
		Left = 0,
		Right,
		Front,
		Back,
		Top,
		Bottom
	};
	HoeTexture * m_sides[6]; /**< Textury */

	/**
	 * Pomocna funkce pro nahravani jednotlivych stran
	 */
	static HoeTexture * LoadSide(HoeFile *file, int size); 
public:
	/**
	 * Vyrenderuje skybox
	 * @param cam Kamera
	 * @todo Dodelat verzi pro D3D9 a dodelat nastaveni kamery
	 */
	void Render(HoeCamera * cam);

	/**
	 * Nacte skybox ze souboru mapy
	 * @param file Soubor s mapou nastavenej na misto skyboxu.
	 */
	bool Load( HoeFile *file);
};

#endif // _HOE_SKYBOX_H_

