// fonts.h
// Organizes the common fonts used in minCy

#include "rendering/font.h"

/* Fundamental required font(s) */
	
	// Monospace font used for rendering text from a file
	extern CyFont textFont;
	extern const char* textFontFilename;

/* Functions */

	// Loads all required fonts
	void CyLoadRequiredFonts(void);

	// Deloads all required fonts
	void CyDeloadRequiredFonts(void);


