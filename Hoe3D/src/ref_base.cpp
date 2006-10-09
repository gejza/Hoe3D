
/**
   @file    ref_base.cpp
   @date    Mar 2006
   @version 1.0
   @brief   Definice tridy RefBase.
*/

#include "StdAfx.h"
#include "shared.h"
#include "utils.h"
#include "ref_base.h"
#include "../include/hoe3d.h"
#include "config.h"

RefBase::RefBase()
{
	m_BackColor = 0xff000000;
}

void HOEAPI RefBase::SetBackgroundColor(dword color)
{
	m_BackColor = color;
}

