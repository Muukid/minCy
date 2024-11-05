// visualBuffer.c
// Tests if the visualBuffer is working properly

// Include files
#include "core/gfx.h"
#include "core/string.h"
#include "editor/visualBuffer.h"

#include <inttypes.h>

CyEditorBox box;
muBool shouldUpdate = MU_FALSE;
muBool shouldInsert = MU_FALSE;

void textInputCallback(muWindow win, uint8_m* data) {
	uint32_m codepoint = 0;
	CyUTF8CodepointDecode(&codepoint, data, 4);
	CyLog("Received codepoint '%" PRIu32 "'\n", codepoint);

	switch (codepoint) {
		default: {
			if (codepoint < 32 || codepoint == 127) {
				CyLog("Codepoint is invalid as a character; ignoring.\n");
				return;
			}
		} break;

		case 8: {
			CyLog("Backspace noted; writing a backspace...\n");
			CyBackspaceCodepointInChunkedFile(&box.file);
			shouldUpdate = MU_TRUE;
			return;
		} break;

		case 13: case 9: break;
	}

	if (shouldInsert) {
		CyLog("Inserting codepoint to file...\n");
		CyInsertCodepointInChunkedFile(&box.file, codepoint);
	} else {
		CyLog("Writing codepoint to file...\n");
		CyWriteCodepointInChunkedFile(&box.file, codepoint);
	}

	shouldUpdate = MU_TRUE;
}

void keyInputCallback(muWindow win, muKeyboardKey key, muBool status) {
	if (status == MU_FALSE) {
		return;
	}
	
	if (key == MU_KEYBOARD_LEFT) {
		CyLog("Leftward movement detected; moving cursor.\n");
		CyMoveLeftInChunkedFile(&box.file, 1);
	}
	else if (key == MU_KEYBOARD_RIGHT) {
		CyLog("Rightward movement detected; moving cursor.\n");
		CyMoveRightInChunkedFile(&box.file, 1);
	}
	else if (key == MU_KEYBOARD_INSERT) {
		shouldInsert = !shouldInsert;
		if (shouldInsert) {
			CyLog("Insert mode ON\n");
		} else {
			CyLog("Insert mode OFF\n");
		}
	}
}

int main(void) {
	CyLog("\n== minCy v1.0.0 (visualBuffer.c) ==\n\n");

	// Init. stuff
	CyGfxInit();
	CyLoadRequiredFonts();

	CyLog("Initializing point size to 16...\n");
	CySetFontPointSize(&textFont, 16.f);

	CyLog("Getting basic codepoints...\n");
	if (!CyFontLoadCodepoint(&textFont, 0x20)) {
		CyLog("Failed to get space codepoint; exiting\n");
		return -1;
	}
	if (!CyFontLoadCodepoint(&textFont, 0x25A1)) {
		CyLog("Failed to get missing codepoint; exiting\n");
		return -1;
	}
	CyLog("\n");

	// Create editor box
	CyLog("Creating editor box\n");
	if (!CyInitEditorBox(&box, &textFont, wininfo.width, wininfo.height)) {
		CyLog("Failed to create editor box; exiting\n");
		return -1;
	}

	mu_window_get_text_input(win, 400, 300, textInputCallback);

	void* funPtr = (void*)keyInputCallback;
	mu_window_set(win, MU_WINDOW_KEYBOARD_CALLBACK, &funPtr);

	// Main loop:
	while (CyGfxExists()) {
		CyGfxClear();

		if (shouldUpdate) {
			shouldUpdate = MU_FALSE;
			CyRefreshEditorBox(&box);
		}
		CyRenderEditorBox(&box);

		CyGfxUpdate();
	}

	// Destroy editor box
	CyLog("Destroying editor box...\n");
	CyDestroyEditorBox(&box);

	// Destroy stuff
	CyDeloadRequiredFonts();
	CyGfxTerm();

	CyLog("Successful\n");
	return 0;
}

