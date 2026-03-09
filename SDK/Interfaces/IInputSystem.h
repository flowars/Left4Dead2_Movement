#pragma once
#include "../Classes/IAppSystem.h"

class IInputSystem : public IAppSystem
{
public:
	// Inherited from IAppSystem
	virtual	InitReturnVal_t Init() = 0;
	virtual bool Connect(CreateInterfaceFn factory) = 0;

	virtual void Shutdown() = 0;

	// Inherited from IInputSystem
	virtual void AttachToWindow(void* hWnd) = 0;
	virtual void DetachFromWindow() = 0;
	virtual void EnableInput(bool bEnable) = 0;
};