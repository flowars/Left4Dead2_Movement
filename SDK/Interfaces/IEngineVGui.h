#pragma once
#include "IPanel.h"

enum VGuiPanel_t
{
	PANEL_ROOT = 0,
	PANEL_GAMEUIDLL,  // the console, game menu
	PANEL_CLIENTDLL,
	PANEL_TOOLS,
	PANEL_INGAMESCREENS,
	PANEL_GAMEDLL,
	PANEL_CLIENTDLL_TOOLS,
	PANEL_GAMEUIBACKGROUND, // the console background, shows under all other stuff in 3d engine view
};

// In-game panels are cropped to the current engine viewport size
enum PaintMode_t
{
	PAINT_UIPANELS = (1 << 0),
	PAINT_INGAMEPANELS = (1 << 1),
};

class IEngineVGui
{
public:
	virtual					~IEngineVGui(void) {}

	virtual VPANEL	GetPanel(VGuiPanel_t type) = 0;

	virtual bool			IsGameUIVisible() = 0;

	virtual void			ActivateGameUI() = 0;
};