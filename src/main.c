// window.c
// Tests if just creating the window and the general
// boilerplate is okay

#include "core/gfx.h"
#include "rendering/fonts.h"

int main(void) {
	CyLog("\n== minCy v1.0.0 ==\n\n");

	CyGfxInit();
	CyLoadRequiredFonts();

	while (CyGfxExists()) {
		CyGfxClear();

		CyGfxUpdate();
	}

	CyDeloadRequiredFonts();
	CyGfxTerm();
	return 0;
}

