// gfx.h
// Handles the boilerplate of a mug context

#include "core/log.h"
#include "core/useful.h"
#include "libs/libs.h"

/* Boilerplate variables */

	// Global mug context
	extern mugContext mug;
	// Graphic handle
	extern muGraphic gfx;
	// Graphic system
	extern muGraphicSystem gfx_system;
	// Name of graphic system
	extern const char* gfx_system_name;
	// The window system
	extern muWindowSystem window_system; // (Auto)

	// Pixel format of window
	extern muPixelFormat winformat;
	// Window information
	extern muWindowInfo wininfo;
	// Window handle
	extern muWindow win;
	// Window keyboard map
	extern muBool* keyboard;

/* Functions */

	// Initializes gfx
	void CyGfxInit(void);

	// Terminates gfx
	void CyGfxTerm(void);

	// Returns whether or not the gfx is still going
	// AKA whether or not the program should still be running
	muBool CyGfxExists(void);

	// Clears the screen of the gfx
	void CyGfxClear(void);

	// Calls the general update functions for the gfx
	void CyGfxUpdate(void);

