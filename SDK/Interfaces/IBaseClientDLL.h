#pragma once
#include "../Classes/ClientClass.h"

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