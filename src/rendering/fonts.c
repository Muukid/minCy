// fonts.c
// Organizes the common fonts used in minCy

#include "rendering/fonts.h"
#include "core/useful.h"
#include "core/log.h"

/* Fundamental required font(s) */
	
	// Monospace font used for rendering text from a file
	CyFont textFont;
	const char* textFontFilename = "resources/fonts/font.ttf";

/* Functions */

	// Loads all required fonts
	void CyLoadRequiredFonts(void) {
		CyLog("Loading required fonts...\n");

		// Load text font (+ ' ' and missing)
		CyLog("Loading text font...\n");
		if (!CyLoadFont(&textFont, textFontFilename)) {
			CyLog("Failed to load text font; exiting\n");
			CyExit();
		} else if (!textFont.monospace) {
			CyLog("Text font isn't monospace; exiting\n");
			CyExit();
		}/* else if (!CyFontLoadCodepoint(&textFont, 0x20)) {
			CyLog("Failed to load space character; exiting\n");
			CyExit();
		} else if (!CyFontIsCodepointLoaded(&textFont, 0x25A1, 0, 0) && !CyFontLoadCodepoint(&textFont, 0x25A1)) {
			CyLog("Failed to load missing character; exiting\n");
			CyExit();
		}*/ else {
			CyLog("Loaded text font\n");
		}

		CyLog("\n");
	}

	// Deloads all required fonts
	void CyDeloadRequiredFonts(void) {
		CyLog("Deloading text font...\n");
		CyDestroyFont(&textFont);

		CyLog("\n");
	}

