
/**
   @file    hoeinterfaces.h
   @version $Revision: 1.3 $
   @brief   Seznam Interface pro komunikaci s Hoe3D
   @see		IHoe3DEngine::Create
*/


#ifndef _HOE_INTERFACES_
#define _HOE_INTERFACES_
#pragma once

#define HOE_INTERFACE_VER  97

/**
 * @brief Zakladni interface
 */
class IHoeInterface
{
public:
	/** Metoda pro ziskani jmena objektu. */
	virtual const tchar * GetName() { return T("unknown");}
	virtual void Release() {};
};

// console

class XHoeConsole
{
public:
	virtual void HOEAPI Con_Print(const tchar *) = 0;

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
