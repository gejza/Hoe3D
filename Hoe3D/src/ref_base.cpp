
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
	//m_BackColor = 0xffff00ff;
}

void HOEAPI RefBase::SetBackgroundColor(unsigned long color)
{
	m_BackColor = color;
}

