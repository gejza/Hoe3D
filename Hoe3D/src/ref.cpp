
/**
   @file    ref.cpp
   @date    Mar 2006
   @version 1.0
   @brief   Definice tridy Ref.
*/

#include "StdAfx.h"
#include "shared.h"
#include "ref.h"

Ref::Ref()
{
	SET_SHARED_PTR(ref);
}

Ref::~Ref()
{
	UNSET_SHARED_PTR(ref);
}

