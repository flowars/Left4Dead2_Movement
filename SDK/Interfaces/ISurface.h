#pragma once
#include "../Classes/IAppSystem.h"
#include "../Classes/Color.h"

typedef unsigned int VPANEL;
typedef unsigned long HCursor;
typedef unsigned long HFont;
const HFont INVALID_FONT = 0; // the value of an invalid font handle

enum FontFeature_t
{
	FONT_FEATURE_ANTIALIASED_FONTS = 1,
	FONT_FEATURE_DROPSHADOW_FONTS = 2,
	FONT_FEATURE_OUTLINE_FONTS = 6,
};

// adds to the font
enum FontFlags_t
{
	FONTFLAG_NONE,
	FONTFLAG_ITALIC = 0x001,
	FONTFLAG_UNDERLINE = 0x002,
	FONTFLAG_STRIKEOUT = 0x004,
	FONTFLAG_SYMBOL = 0x008,
	FONTFLAG_ANTIALIAS = 0x010,
	FONTFLAG_GAUSSIANBLUR = 0x020,
	FONTFLAG_ROTARY = 0x040,
	FONTFLAG_DROPSHADOW = 0x080,
	FONTFLAG_ADDITIVE = 0x100,
	FONTFLAG_OUTLINE = 0x200,
	FONTFLAG_CUSTOM = 0x400,		// custom generated font - never fall back to asian compatibility mode
	FONTFLAG_BITMAP = 0x800,		// compiled bitmap font - no fallbacks
};

enum FontDrawType_t
{
	// Use the "additive" value from the scheme file
	FONT_DRAW_DEFAULT = 0,

	// Overrides
	FONT_DRAW_NONADDITIVE,
	FONT_DRAW_ADDITIVE,

	FONT_DRAW_TYPE_COUNT = 2,
};

struct IntRect
{
	int x0;
	int y0;
	int x1;
	int y1;
};

class ISurface : public IAppSystem
{
public:
	// call to Shutdown surface; surface can no longer be used after this is called
	virtual void Shutdown() = 0;

	// hierarchy root
	virtual VPANEL GetEmbeddedPanel() = 0;
	virtual void SetEmbeddedPanel(VPANEL pPanel) = 0;

	// drawing context
	virtual void PushMakeCurrent(VPANEL panel, bool useInsets) = 0;
	virtual void PopMakeCurrent(VPANEL panel) = 0;

	// rendering functions
	virtual void DrawSetColor(int r, int g, int b, int a) = 0;
	virtual void DrawSetColor(Color col) = 0;

	virtual void DrawFilledRect(int x0, int y0, int x1, int y1) = 0;
	virtual void DrawFilledRectArray(IntRect* pRects, int numRects) = 0;
	virtual void DrawOutlinedRect(int x0, int y0, int x1, int y1) = 0;

	virtual void DrawLine(int x0, int y0, int x1, int y1) = 0;
	virtual void DrawPolyLine(int* px, int* py, int numPoints) = 0;

	virtual void DrawSetTextFont(HFont font) = 0;
	virtual void DrawSetTextColor(int r, int g, int b, int a) = 0;
	virtual void DrawSetTextColor(Color col) = 0;
	virtual void DrawSetTextPos(int x, int y) = 0;
	virtual void DrawGetTextPos(int& x, int& y) = 0;
	virtual void DrawPrintText(const wchar_t* text, int textLen, FontDrawType_t drawType = FONT_DRAW_DEFAULT) = 0;
	virtual void DrawUnicodeChar(wchar_t wch, FontDrawType_t drawType = FONT_DRAW_DEFAULT) = 0;

	virtual void DrawFlushText() = 0;		// flushes any buffered text (for rendering optimizations)
	virtual void* CreateHTMLWindow(void* events, VPANEL context) = 0;
	virtual void PaintHTMLWindow(void* htmlwin) = 0;
	virtual void DeleteHTMLWindow(void* htmlwin) = 0;

	virtual int	 DrawGetTextureId(char const* filename) = 0;
	virtual bool DrawGetTextureFile(int id, char* filename, int maxlen) = 0;
	virtual void DrawSetTextureFile(int id, const char* filename, int hardwareFilter, bool forceReload) = 0;
	virtual void DrawSetTextureRGBA(int id, const unsigned char* rgba, int wide, int tall, int hardwareFilter, bool forceReload) = 0;
	virtual void DrawSetTexture(int id) = 0;
	virtual void DrawGetTextureSize(int id, int& wide, int& tall) = 0;
	virtual void DrawTexturedRect(int x0, int y0, int x1, int y1) = 0;
	virtual bool IsTextureIDValid(int id) = 0;

	virtual int CreateNewTextureID(bool procedural = false) = 0;

	virtual void GetScreenSize(int& wide, int& tall) = 0;
	virtual void SetAsTopMost(VPANEL panel, bool state) = 0;
	virtual void BringToFront(VPANEL panel) = 0;
	virtual void SetForegroundWindow(VPANEL panel) = 0;
	virtual void SetPanelVisible(VPANEL panel, bool state) = 0;
	virtual void SetMinimized(VPANEL panel, bool state) = 0;
	virtual bool IsMinimized(VPANEL panel) = 0;
	virtual void FlashWindow(VPANEL panel, bool state) = 0;
	virtual void SetTitle(VPANEL panel, const wchar_t* title) = 0;
	virtual void SetAsToolBar(VPANEL panel, bool state) = 0;		// removes the window's task bar entry (for context menu's, etc.)

	// windows stuff
	virtual void CreatePopup(VPANEL panel, bool minimised, bool showTaskbarIcon = true, bool disabled = false, bool mouseInput = true, bool kbInput = true) = 0;
	virtual void SwapBuffers(VPANEL panel) = 0;
	virtual void Invalidate(VPANEL panel) = 0;
	virtual void SetCursor(HCursor cursor) = 0;
	virtual bool IsCursorVisible() = 0;
	virtual void ApplyChanges() = 0;
	virtual bool IsWithin(int x, int y) = 0;
	virtual bool HasFocus() = 0;

	// returns true if the surface supports minimize & maximize capabilities
	enum SurfaceFeature_e
	{
		ANTIALIASED_FONTS = 1,
		DROPSHADOW_FONTS = 2,
		ESCAPE_KEY = 3,
		OPENING_NEW_HTML_WINDOWS = 4,
		FRAME_MINIMIZE_MAXIMIZE = 5,
		OUTLINE_FONTS = 6,
	};
	virtual bool SupportsFeature(SurfaceFeature_e feature) = 0;

	// restricts what gets drawn to one panel and it's children
	// currently only works in the game
	virtual void RestrictPaintToSinglePanel(VPANEL panel) = 0;

	// these two functions obselete, use IInput::SetAppModalSurface() instead
	virtual void SetModalPanel(VPANEL panel) = 0;
	virtual VPANEL GetModalPanel() = 0;

	virtual void UnlockCursor() = 0;
	virtual void LockCursor() = 0;
};