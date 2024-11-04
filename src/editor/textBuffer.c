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
	}

	// Inserts codepoint
	muBool CyInsertCodepointInChunkedFile(CyChunkedFile* file, uint32_m codepoint) {
		// If cursor is at the end of this chunk and there is no next chunk:
		if (file->cursorIndex == FILE_CHUNK_CODEPOINTS-1 && !file->cursorChunk->next) {
			// Allocate a new chunk
			CyFileChunk* newChunk = (CyFileChunk*)malloc(sizeof(CyFileChunk));
			if (!newChunk) {
				return MU_FALSE;
			}

			// Set members to be correct
			memset(newChunk->data, 0, 4 * FILE_CHUNK_CODEPOINTS);
			newChunk->next = 0;
			newChunk->prev = file->cursorChunk;
			file->cursorChunk->next = newChunk;
		}

		// Set codepoint of current slot
		file->cursorChunk->data[file->cursorIndex] = codepoint;
		// Move right
		CyMoveRightInChunkedFile(file, 1);

		return MU_TRUE;
	}

