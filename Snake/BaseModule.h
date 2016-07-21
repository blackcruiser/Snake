#pragma once
#include "stdafx.h"

class BaseModule
{

public:
	BaseModule();
	virtual ~BaseModule();

	virtual void WillRender();
	virtual void Render();
	virtual void DidRender();

	virtual void onKeyDown(int key);
	virtual void onTimeout();
};

