// textBuffer.c
// Handles taking raw data and splitting it up into chunks representing a screen buffer, and the management thereof.

// Rule #1: Cursor assumes leftmost when empty:
// A___>_B
// The cursor there is at A

#include "editor/textBuffer.h"
#include <string.h>
#include <stdlib.h>

/* Inner functions */

	// Destroys all chunks recursively
	void CyDestroyEachChunk(CyFileChunk* chunk) {
		// If another chunk still exists, fall into that
		if (chunk->next) {
			CyDestroyEachChunk(chunk->next);
		}
		// Deallocate this chunk
		free(chunk);
	}

	// Pushes cursor to farthest left empty slot
	void CyShiftLeft(CyChunkedFile* file) {
		while (MU_TRUE) {
			// If we've gone as far left as possible, just exit
			if (!file->cursorChunk->prev && file->cursorIndex == 0) {
				return;
			}

			// If we've gone as far left as possible in this chunk, wrap
			if (file->cursorIndex == 0) {
				file->cursorChunk = file->cursorChunk->prev;
				file->cursorIndex = FILE_CHUNK_CODEPOINTS-1;
			}

			// If not, simply decrement cursorIndex
			else {
				--file->cursorIndex;
			}

			// If this codepoint is not 0, move right once and we're good
			if (file->cursorChunk->data[file->cursorIndex] != 0) {
				if (file->cursorIndex == FILE_CHUNK_CODEPOINTS-1) {
					file->cursorChunk = file->cursorChunk->next;
					file->cursorIndex = 0;
				}
				else {
					++file->cursorIndex;
				}
				return;
			}
		}
	}

	// Pushes data starting from the cursor and past to a new chunk rightwards
	muBool CyPushRight(CyChunkedFile* file) {
		// Allocate new chunk
		CyFileChunk* newChunk = (CyFileChunk*)malloc(sizeof(CyFileChunk));
		if (!newChunk) {
			return MU_FALSE;
		}

		// Initialize variables
		memset(newChunk->data, 0, 4 * FILE_CHUNK_CODEPOINTS);

		// cursorChunk -> newChunk -> cursorChunk->next
		// - cursorChunk->next <- newChunk
		if (file->cursorChunk->next) {
			file->cursorChunk->next->prev = newChunk;
		}
		// - newChunk -> cursorChunk->next
		newChunk->next = file->cursorChunk->next;
		// - newChunk <- cursorChunk
		newChunk->prev = file->cursorChunk;
		// - cursorChunk -> newChunk
		file->cursorChunk->next = newChunk;

		// Move data over from cursor chunk to new
		memcpy(newChunk->data, &file->cursorChunk->data[file->cursorIndex], 4 * (FILE_CHUNK_CODEPOINTS - file->cursorIndex));
		// And then zero-out that data
		memset(&file->cursorChunk->data[file->cursorIndex], 0, 4 * (FILE_CHUNK_CODEPOINTS - file->cursorIndex));

		return MU_TRUE;
	}

/* Outer functions */

	// Initializes an empty chunked file
	// Returns false if failed to allocate chunks
	muBool CyCreateEmptyChunkedFile(CyChunkedFile* file) {
		// Allocate one chunk
		file->chunks = (CyFileChunk*)malloc(sizeof(CyFileChunk));
		if (!file->chunks) {
			return MU_FALSE;
		}
		// Set all memory within the chunk to 0
		memset(file->chunks, 0, sizeof(CyFileChunk));

		// Set cursor to 0
		file->cursorChunk = file->chunks;
		file->cursorIndex = 0;
		return MU_TRUE;
	}

	// Destroys a chunked file
	void CyDestroyChunkedFile(CyChunkedFile* file) {
		// Destroy all chunks
		CyDestroyEachChunk(file->chunks);
	}

	// Moves cursor left n times
	// Simply stops if it can't go any further left
	void CyMoveLeftInChunkedFile(CyChunkedFile* file, uint32_m n) {
		// Loop n amount of times
		while (n) {
			// If this is the last index in the chunk:
			if (file->cursorIndex == 0) {
				// If there is a previous chunk, go to it
				if (file->cursorChunk->prev) {
					file->cursorChunk = file->cursorChunk->prev;
					file->cursorIndex = FILE_CHUNK_CODEPOINTS - 1;
				}
				// If there is no previous chunk, farthest we can go; exit
				else {
					return;
				}
			}

			// If this isn't the last index in the chunk, simply
			// decrement by 1
			else {
				--file->cursorIndex;
			}

			// If we landed on a valid codepoint, count the left movement
			if (file->cursorChunk->data[file->cursorIndex] != 0) {
				--n;
			}
		}
	}

	// Moves cursor right n times
	// Simply stops if it can't go any further right
	// This code is very similar to CyMoveLeftInChunkedFile
	void CyMoveRightInChunkedFile(CyChunkedFile* file, uint32_m n) {
		// Loop n amount of times
		while (n) {
			// If this is the last index in the chunk:
			if (file->cursorIndex == FILE_CHUNK_CODEPOINTS-1) {
				// If there is a next chunk, go to it
				if (file->cursorChunk->next) {
					file->cursorChunk = file->cursorChunk->next;
					file->cursorIndex = 0;
				}
				// If there is no next chunk, farthest we can go; exit
				else {
					break;
				}
			}

			// If this isn't the last index in the chunk, simply
			// increment by 1
			else {
				++file->cursorIndex;
			}

			// If we're on on a valid codepoint, count the right movement
			if (file->cursorChunk->data[file->cursorIndex] != 0) {
				--n;
			}
		}

		// If we landed on an empty codepoint:
		if (file->cursorChunk->data[file->cursorIndex] == 0) {
			// That means that we reached the end of all the data;
			// go back to nearest empty slot to ensure optimal space usage
			CyShiftLeft(file);
		}
	}

	// Inserts codepoint
	muBool CyInsertCodepointInChunkedFile(CyChunkedFile* file, uint32_m codepoint) {
		// If cursor is at the end of this chunk and there is no next chunk:
		if (file->cursorIndex == FILE_CHUNK_CODEPOINTS-1 && !file->cursorChunk->next) {
			// Allocate new chunk
			CyFileChunk* newChunk = (CyFileChunk*)malloc(sizeof(CyFileChunk));
			if (!newChunk) {
				return MU_FALSE;
			}

			// Initialize variables
			memset(newChunk->data, 0, 4 * FILE_CHUNK_CODEPOINTS);

			// cursorChunk -> newChunk -> cursorChunk->next
			// - newChunk -> cursorChunk->next
			newChunk->next = file->cursorChunk->next;
			// - newChunk <- cursorChunk
			newChunk->prev = file->cursorChunk;
			// - cursorChunk -> newChunk
			file->cursorChunk->next = newChunk;
		}

		// If the given codepoint is a newline or tab and we're overwriting a non-zero codepoint:
		if ((codepoint == 13 || codepoint == 9)
			&& (file->cursorChunk->data[file->cursorIndex] != 0)
		) {
			// Data must be pushed right
			if (!CyPushRight(file)) {
				return MU_FALSE;
			}
			// Value must be written
			file->cursorChunk->data[file->cursorIndex] = codepoint;
			// Move right
			CyMoveRightInChunkedFile(file, 1);
			return MU_TRUE;
		}

		// If the current codepoint is newline:
		if (file->cursorChunk->data[file->cursorIndex] == 13) {

			// If we could theoretically move left by one:
			if ((file->cursorIndex == 0 && file->cursorChunk->prev) || (file->cursorIndex != 0)) {
				CyFileChunk* prevCursorChunk = file->cursorChunk;
				uint32_m prevCursorIndex = file->cursorIndex;
				// Move left by one
				if (file->cursorIndex == 0) {
					file->cursorChunk = file->cursorChunk->prev;
					file->cursorIndex = FILE_CHUNK_CODEPOINTS-1;
				} else {
					--file->cursorIndex;
				}

				// If we're now on an empty, there's empty space that can be
				// used; move as far left as possible for this empty space.
				if (file->cursorChunk->data[file->cursorIndex] == 0) {
					CyShiftLeft(file);
				}
				// If we're not on an empty, move right and push data
				else {
					file->cursorChunk = prevCursorChunk;
					file->cursorIndex = prevCursorIndex;
					if (!CyPushRight(file)) {
						return MU_FALSE;
					}
				}
			}

			// If we can't move left, push data to the right
			else {
				if (!CyPushRight(file)) {
					return MU_FALSE;
				}
			}
		}
		// Set codepoint of current slot
		file->cursorChunk->data[file->cursorIndex] = codepoint;
		// Move right
		CyMoveRightInChunkedFile(file, 1);

		return MU_TRUE;
	}

	// @TODO
	void CyBackspaceCodepointInChunkedFile(CyChunkedFile* file) {
		if (file) {}
	}

