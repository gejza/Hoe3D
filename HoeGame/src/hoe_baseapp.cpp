
#include "StdAfx.h"
#include "../include/HoeGame/hoe_baseapp.h"

BEGIN_HOEGAME

HoeBaseApp * HoeBaseApp::this_ = NULL;

HoeBaseApp::HoeBaseApp()
{
	assert(this_ == NULL);
	this_ = this;
}

END_HOEGAME

