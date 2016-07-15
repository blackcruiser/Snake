#pragma once
#include "stdafx.h"

class BaseModule
{

public:
	BaseModule();
	virtual ~BaseModule();

	virtual void ProcessMessage(int message);
	virtual void Render();
	virtual void onKeyDown(int key);
	virtual void onTimeout();
};

