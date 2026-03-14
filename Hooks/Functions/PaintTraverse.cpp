#include "../Hooks.h"
#include "../../Utils/Render/Draw.h"

void __stdcall Hooks::PaintTraverseHook(unsigned int panel, bool force_repaint, bool allow_force)
{
	if (panel == (interfaces::engineVGUI->GetPanel(PANEL_TOOLS)))
	{

	/*	Render.AddToRender([]() {

			});*/

	}

	Hooks::PaintTraverseOriginal(interfaces::panel, panel, force_repaint, allow_force);
}