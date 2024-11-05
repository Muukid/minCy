// textBuffer.c
// Handles taking raw data and splitting it up into chunks representing a screen buffer, and the management thereof.

// Rule #1: Cursor assumes leftmost when empty:
// A___>_B
// The cursor there is at A

// Rule #2: Cursor can never be on a zero codepoint
// after calling a function, unless there are no codepoints
// after it.

// @TODO Clean up code.

#include "editor/textBuffer.h"
#include <string.h>
#include <stdlib.h>

// Remove after debug:
#include <stdio.h>

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
	// Pushes cursor to farthest left non-empty slot
	// Similar to CyShiftLeft
	void CyShiftLeftNonEmpty(CyChunkedFile* file) {
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

			// If this codepoint is not 0, we're good
			if (file->cursorChunk->data[file->cursorIndex] != 0) {
				return;
			}
		}
	}

	// Pushes cursor to farthest right non-empty slot
	void CyShiftRightNonEmpty(CyChunkedFile* file) {
		while (MU_TRUE) {
			// If we've gone as far right as possible, just exit
			if (!file->cursorChunk->next && file->cursorIndex == FILE_CHUNK_CODEPOINTS-1) {
				return;
			}

			// If we've gone as far right as possible in this chunk, wrap
			if (file->cursorIndex == FILE_CHUNK_CODEPOINTS-1) {
				file->cursorChunk = file->cursorChunk->next;
				file->cursorIndex = 0;
			}

			// If not, simply increment cursorIndex
			else {
				++file->cursorIndex;
			}

			// If this codepoint is not 0, we're good
			if (file->cursorChunk->data[file->cursorIndex] != 0) {
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
		memcpy(&newChunk->data[file->cursorIndex], &file->cursorChunk->data[file->cursorIndex], 4 * (FILE_CHUNK_CODEPOINTS - file->cursorIndex));
		// And then zero-out that data
		memset(&file->cursorChunk->data[file->cursorIndex], 0, 4 * (FILE_CHUNK_CODEPOINTS - file->cursorIndex));

		return MU_TRUE;
	}

	// Scans each chunk in a file for empty chunks, and removes them
	void CyShakeFile(CyChunkedFile* file) {
		// Start at first chunk
		CyFileChunk* chunk = file->chunks;

		while (MU_TRUE) {
			// Go to next chunk
			if (!chunk->next) {
				return;
			}
			chunk = chunk->next;

			// If the cursor is in this chunk, don't consider
			if (file->cursorChunk == chunk) {
				continue;
			}

			// Scan through for non-empty codepoints
			muBool empty = MU_TRUE;
			for (uint32_m c = 0; c < FILE_CHUNK_CODEPOINTS; ++c) {
				if (chunk->data[c] != 0) {
					empty = MU_FALSE;
					break;
				}
			}

			// If empty, remove
			if (empty) {
				chunk->prev->next = chunk->next;
				if (chunk->next) {
					chunk->next->prev = chunk->prev;
				}
				free(chunk);
			}
		}
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
			// It's just inserted.
			return CyWriteCodepointInChunkedFile(file, codepoint);
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

	// Backspace a codepoint
	void CyBackspaceCodepointInChunkedFile(CyChunkedFile* file) {
		// 1. Save where we currently are.
		CyFileChunk* prevChunk = file->cursorChunk;
		uint32_m prevIndex = file->cursorIndex;

		// 2. Move left until we're rather farthest left possible
		//    or we're on a non-zero codepoint.
		while (MU_TRUE) {
			if (!file->cursorChunk->prev && file->cursorIndex == 0) {
				break;
			}

			if (file->cursorIndex == 0) {
				file->cursorChunk = file->cursorChunk->prev;
				file->cursorIndex = FILE_CHUNK_CODEPOINTS-1;
			}
			else {
				--file->cursorIndex;
			}

			if (file->cursorChunk->data[file->cursorIndex] != 0) {
				break;
			}
		}

		// 3. Set that codepoint to 0.
		file->cursorChunk->data[file->cursorIndex] = 0;

		// 4. Go back.
		if (prevChunk->data[prevIndex] != 0) {
			file->cursorChunk = prevChunk;
			file->cursorIndex = prevIndex;
		}
		
		// 5. Shake file (optimize later)
		CyShakeFile(file);
	}

	// Writes codepoint
	muBool CyWriteCodepointInChunkedFile(CyChunkedFile* file, uint32_m codepoint) {
		// Is the current slot empty?
		if (file->cursorChunk->data[file->cursorIndex] == 0) {
			// If so, set the slot.
			file->cursorChunk->data[file->cursorIndex] = codepoint;
			// Move right.
			if (!file->cursorChunk->next && file->cursorIndex == FILE_CHUNK_CODEPOINTS-1) {
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

				// Set cursor to new chunk
				file->cursorChunk = newChunk;
				file->cursorIndex = 0;
			}
			else {
				CyMoveRightInChunkedFile(file, 1);
			}
			// And we're done.
			return MU_TRUE;
		}

		// If there is no previous spot, there is no free space.
		if (!file->cursorChunk->prev && file->cursorIndex == 0) {
			// In such case, data must be pushed.
			if (!CyPushRight(file)) {
				return MU_FALSE;
			}
			// And codepoint must be set
			file->cursorChunk->data[file->cursorIndex] = codepoint;
			return MU_TRUE;
		}

		// There is a previous spot; is it zero?
		CyFileChunk* testChunk = file->cursorChunk;
		uint32_m testIndex = file->cursorIndex;
		if (testIndex == 0) {
			testChunk = testChunk->prev;
			testIndex = FILE_CHUNK_CODEPOINTS-1;
		} else {
			--testIndex;
		}

		if (testChunk->data[testIndex] == 0) {
			// If it is, try moving back as far as possible
			// before it isn't zero.
			CyFileChunk* prevChunk = testChunk;
			uint32_m prevIndex = testIndex;
			while (MU_TRUE) {
				if (!prevChunk->prev && prevIndex == 0) {
					break;
				}

				if (prevIndex == 0) {
					prevChunk = prevChunk->prev;
					prevIndex = FILE_CHUNK_CODEPOINTS-1;
				} else {
					--prevIndex;
				}

				if (prevChunk->data[prevIndex] == 0) {
					testChunk = prevChunk;
					testIndex = prevIndex;
				} else {
					break;
				}
			}

			// Then set it.
			testChunk->data[testIndex] = codepoint;
			return MU_TRUE;
		}

		// There is no previous spot.
		// So, a few things need to happen:

		// 1. Push right.
		if (!CyPushRight(file)) {
			return MU_FALSE;
		}

		// 2. Set codepoint.
		file->cursorChunk->data[file->cursorIndex] = codepoint;
		// 3. Move cursor right.
		CyMoveRightInChunkedFile(file, 1);

		return MU_TRUE;
	}

	// Gets the first slot
	// Returns false if no slot
	muBool CyGetFirstSlotInChunkedFile(CyChunkedFile* file, CyChunkSlot* slot) {
		slot->index = 0;
		slot->chunk = file->chunks;

		while (MU_TRUE) {
			if (slot->chunk->data[slot->index] != 0) {
				slot->codepoint = slot->chunk->data[slot->index];
				return MU_TRUE;
			}

			if (slot->index == FILE_CHUNK_CODEPOINTS-1 && !slot->chunk->next) {
				return MU_FALSE;
			}
			else if (slot->index == FILE_CHUNK_CODEPOINTS-1) {
				slot->chunk = slot->chunk->next;
				slot->index = 0;
			}
			else {
				++slot->index;
			}
		}

		return MU_TRUE;
	}

	// Gets the next slot
	// Returns false if no slot
	muBool CyGetNextSlotInChunkedFile(CyChunkSlot* slot) {
		while (MU_TRUE) {
			if (slot->index == FILE_CHUNK_CODEPOINTS-1 && !slot->chunk->next) {
				return MU_FALSE;
			}
			else if (slot->index == FILE_CHUNK_CODEPOINTS-1) {
				slot->chunk = slot->chunk->next;
				slot->index = 0;
			}
			else {
				++slot->index;
			}

			if (slot->chunk->data[slot->index] != 0) {
				slot->codepoint = slot->chunk->data[slot->index];
				return MU_TRUE;
			}
		}

		return MU_FALSE;
	}

