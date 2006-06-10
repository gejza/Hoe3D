
/**
   @file    hoeinterfaces.h
   @version $Revision: 1.3 $
   @brief   Seznam Interface pro komunikaci s Hoe3D
   @see		IHoe3DEngine::Create
*/


#ifndef _HOE_INTERFACES_
#define _HOE_INTERFACES_

#include "hoetypes.h"

#define HOE_INTERFACE_VER  67

/**
 * @brief Zakladni interface
 */
class IHoeInterface
{
public:
	/** Metoda pro ziskani jmena objektu. */
	virtual const char * GetName() { return "none";}
};

// console

class XHoeConsole
{
public:
	virtual void HOEAPI Con_Print(const char *) = 0;

};

class IHoe2D;
class XHoe2DCallback
{
public:
	virtual void HOEAPI _Paint(IHoe2D * ) {};
}; 

#include "hoeobject.h"
#include "hoeresources.h"
#include "hoeelements.h"
#include "hoescene.h"
#include "hoesystems.h"
#include "hoeinput.h"

#endif //_HOE_INTERFACES_
