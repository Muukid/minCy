// textBuffer.c
// Tests if the textBuffer is working properly

// This demo works best if FILE_CHUNK_CODEPOINTS is manually decreased

#include "core/gfx.h"
#include "editor/textBuffer.h"
#include "core/string.h"
#include "core/log.h"

#include <inttypes.h>

CyChunkedFile file;

void printFileInfo(void) {
	CyLog("\nInfo:\n");

	// Loop through each chunk
	CyFileChunk* chunks = file.chunks;
	while (MU_TRUE) {
		// Print chunk data
		for (uint32_m i = 0; i < FILE_CHUNK_CODEPOINTS; ++i) {
			if (chunks == file.cursorChunk && file.cursorIndex == i) {
				CyLog("%c[4;42m", 27);
			}

			switch (chunks->data[i]) {
				default: CyLog("%c ", (char)chunks->data[i]); break;

				case 0: {
					if (chunks == file.cursorChunk && file.cursorIndex == i) {
						CyLog("%c[4;43m", 27);
						CyLog("_ ");
					} else {
						CyLog("%c[4;41m", 27);
						CyLog("_ ");
					}
				} break;

				case 13: CyLog("\\n"); break;
				case 9: CyLog("\\t"); break;
			}

			CyLog("%c[0m", 27);
		}
		CyLog("\n");

		// Exit loop if no more chunks found
		chunks = chunks->next;
		if (!chunks) {
			break;
		}
	}

	CyLog("########\n");

	// Loop through each chunk
	chunks = file.chunks;
	while (MU_TRUE) {
		// Print chunk data
		for (uint32_m i = 0; i < FILE_CHUNK_CODEPOINTS; ++i) {
			if (chunks == file.cursorChunk && file.cursorIndex == i) {
				CyLog("%c[4;42m", 27);
			}

			switch (chunks->data[i]) {
				default: CyLog("%c", (char)chunks->data[i]); break;

				case 0: {
					if (chunks == file.cursorChunk && file.cursorIndex == i) {
						CyLog(" ");
					} else {
						CyLog("");
					}
				} break;

				case 13: {
					CyLog(" ");
					CyLog("%c[0m", 27);
					CyLog("\n");
				} break;

				case 9: {
					// Note: this code assumes that a tab is 5 spaces visually
					CyLog("     ");
				} break;
			}

			CyLog("%c[0m", 27);
		}

		// Exit loop if no more chunks found
		chunks = chunks->next;
		if (!chunks) {
			break;
		}
	}

	CyLog("\n\n");
}

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
			CyBackspaceCodepointInChunkedFile(&file);
			printFileInfo();
			return;
		} break;

		case 13: case 9: break;
	}

	CyLog("Writing codepoint to file...\n");
	CyInsertCodepointInChunkedFile(&file, codepoint);
	printFileInfo();
}

void keyInputCallback(muWindow win, muKeyboardKey key, muBool status) {
	if (status == MU_FALSE) {
		return;
	}
	
	if (key == MU_KEYBOARD_LEFT) {
		CyLog("Leftward movement detected; moving cursor.\n");
		CyMoveLeftInChunkedFile(&file, 1);
		printFileInfo();
	}
	else if (key == MU_KEYBOARD_RIGHT) {
		CyLog("Rightward movement detected; moving cursor.\n");
		CyMoveRightInChunkedFile(&file, 1);
		printFileInfo();
	}
}

int main(void) {
	CyLog("\n== minCy v1.0.0 (textBuffer.c) ==\n\n");
	CyGfxInit();

	CyLog("Creating empty chunked file...\n");
	if (!CyCreateEmptyChunkedFile(&file)) {
		CyLog("Failed to allocate for chunked file; exiting\n");
		return 0;
	}
	printFileInfo();

	mu_window_get_text_input(win, 400, 300, textInputCallback);

	void* funPtr = (void*)keyInputCallback;
	mu_window_set(win, MU_WINDOW_KEYBOARD_CALLBACK, &funPtr);

	while (CyGfxExists()) {
		CyGfxClear();

		CyGfxUpdate();
	}

	CyLog("Destroying empty chunked file...\n");
	CyDestroyChunkedFile(&file);

	CyGfxTerm();
	CyLog("Successful\n");
	return 0;
}

