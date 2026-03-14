#pragma once
#include "../Classes/ClientClass.h"

//-----------------------------------------------------------------------------
// Purpose: The engine reports to the client DLL what stage it's entering so the DLL can latch events
//  and make sure that certain operations only happen during the right stages.
// The value for each stage goes up as you move through the frame so you can check ranges of values
//  and if new stages get added in-between, the range is still valid.
//-----------------------------------------------------------------------------
enum ClientFrameStage_t
{
	FRAME_UNDEFINED = -1,			// (haven't run any frames yet)
	FRAME_START,

	// A network packet is being recieved
	FRAME_NET_UPDATE_START,
	// Data has been received and we're going to start calling PostDataUpdate
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	// Data has been received and we've called PostDataUpdate on all data recipients
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	// We've received all packets, we can now do interpolation, prediction, etc..
	FRAME_NET_UPDATE_END,

	// We're about to start rendering the scene
	FRAME_RENDER_START,
	// We've finished rendering the scene.
	FRAME_RENDER_END
};


class IBaseClientDLL
{
	public:
		// Connect appsystem components, get global interfaces, don't run any other init code
		virtual int				Connect() = 0;

		// run other init code here
		virtual int				Init() = 0;

		virtual void			PostInit() = 0;

		// Called once when the client DLL is being unloaded
		virtual void			Shutdown(void) = 0;

		// Called at the start of each level change
		virtual void			LevelInitPreEntity(char const* pMapName) = 0;
		// Called at the start of a new level, after the entities have been received and created
		virtual void			LevelInitPostEntity() = 0;
		// Called at the end of a level
		virtual void			LevelShutdown(void) = 0;

		// Request a pointer to the list of client datatable classes
		virtual ClientClass* GetAllClasses(void) = 0;
};