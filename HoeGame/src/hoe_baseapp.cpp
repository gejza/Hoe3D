
#include "StdAfx.h"
#include "../include/HoeGame/hoe_baseapp.h"

namespace HoeGame {

HoeBaseApp * HoeBaseApp::this_ = NULL;

HoeBaseApp::HoeBaseApp()
{
	assert(this_ == NULL);
	this_ = this;
}

} // namespace HoeGame

