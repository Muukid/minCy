// gfx.c
// Handles the boilerplate of a mug context

#include "core/gfx.h"
#include "core/log.h"
#include "core/useful.h"
#include "libs/libs.h"

/* Boilerplate variables */

	// Global mug context
	mugContext mug;
	// Graphic handle
	muGraphic gfx;
	// Graphic system
	muGraphicSystem gfx_system = MU_GRAPHIC_OPENGL;
	// Name of graphic system
	const char* gfx_system_name = "OpenGL";
	// The window system
	muWindowSystem window_system = MU_WINDOW_NULL; // (Auto)

	// Pixel format of window
	muPixelFormat winformat = {
		// RGBA bits
		8, 8, 8, 8,
		// Depth bits
		24,
		// Stencil bits
		0,
		// Samples
		1
	};

	// Window information
	muWindowInfo wininfo = {
		// Title
		(char*)"minCy",
		// Resolution
		800, 600,
		// Min/Max res. (none)
		0, 0, 0, 0,
		// Coordinates
		50, 50,
		// Pixel format
		&winformat,
		// Callbacks (default)
		0
	};

	// Window handle
	muWindow win;
	// Window keyboard map
	muBool* keyboard;

/* Functions */

	// Initializes gfx
	void CyGfxInit(void) {
		// Init mug
		CyLog("Initializing mug...\n");
		mug_context_create(&mug, window_system, MU_TRUE);

		// Log currently running window system
		CyLog("Running window system \"%s\"\n", mu_window_system_get_nice_name(muCOSA_context_get_window_system(&mug.cosa)));

		// Create graphic + window using graphic system
		CyLog("Creating graphic for %s...\n", gfx_system_name);
		gfx = mu_graphic_create_window(gfx_system, &wininfo);
		// Get window handle
		win = mu_graphic_get_window(gfx);

		// Get keyboard key map
		mu_window_get(win, MU_WINDOW_KEYBOARD_MAP, &keyboard);

		CyLog("\n");
	}

	// Terminates gfx
	void CyGfxTerm(void) {
		CyLog("Destroying graphic...\n");
		gfx = mu_graphic_destroy(gfx);

		CyLog("Terminating mug...\n");
		mug_context_destroy(&mug);

		// Print possible error
		if (mug.result != MUG_SUCCESS) {
			CyLog("Something went wrong with mug during the program's life; result: %s\n", mug_result_get_name(mug.result));
		} else {
			CyLog("No issues reported from mug\n");
		}

		CyLog("\n");
	}

	// Returns whether or not the gfx is still going
	// AKA whether or not the program should still be running
	muBool CyGfxExists(void) {
		return mu_graphic_exists(gfx);
	}

	// Clears the screen of the gfx
	void CyGfxClear(void) {
		mu_graphic_clear(gfx, 0.f, 0.f, 0.f);
	}

	// Calls the general update functions for the gfx
	void CyGfxUpdate(void) {
		// Swap buffers (to present image)
		mu_graphic_swap_buffers(gfx);
		// Update graphic at ~100 FPS
		mu_graphic_update(gfx, 100.f);
	}

