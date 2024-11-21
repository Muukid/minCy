// textBuffer.h
// Handles taking raw data and splitting it up into chunks representing a screen buffer, and the management thereof.

#include "libs/libs.h"

// Amount of codepoints per chunk
#define FILE_CHUNK_CODEPOINTS 8

// Struct representing a chunk
typedef struct CyFileChunk CyFileChunk;
struct CyFileChunk {
	// Codepoint data within this chunk
	uint32_m data[FILE_CHUNK_CODEPOINTS];
	// Next chunk; 0 if this is the last chunk
	CyFileChunk* next;
	// Previous chunk; 0 if this is the first
	CyFileChunk* prev;
};

// Struct representing an individual slot in a chunk
struct CyChunkSlot {
	// Codepoint of the slot
	uint32_m codepoint;
	// Chunk
	CyFileChunk* chunk;
	// Index
	uint32_m index;
};
typedef struct CyChunkSlot CyChunkSlot;

// Struct representing a chunked file
struct CyChunkedFile {
	// Chunks
	CyFileChunk* chunks;
	// Cursor location in file chunk;
	// codepoints can only be added or removed
	// relative to the cursor.
	// Initialized to 0
	CyFileChunk* cursorChunk;
	uint32_m cursorIndex;
};
typedef struct CyChunkedFile CyChunkedFile;

// Initializes an empty chunked file
// Returns false if failed to allocate chunks
muBool CyCreateEmptyChunkedFile(CyChunkedFile* file);
// Destroys a chunked file
void CyDestroyChunkedFile(CyChunkedFile* file);

// Moves cursor left n times
// Simply stops if it can't go any further left
void CyMoveLeftInChunkedFile(CyChunkedFile* file, uint32_m n);
// Moves cursor right n times
// Simply stops if it can't go any further right
void CyMoveRightInChunkedFile(CyChunkedFile* file, uint32_m n);

// Inserts codepoint
muBool CyInsertCodepointInChunkedFile(CyChunkedFile* file, uint32_m codepoint);
// Backspace a codepoint
void CyBackspaceCodepointInChunkedFile(CyChunkedFile* file);
// Writes codepoint
muBool CyWriteCodepointInChunkedFile(CyChunkedFile* file, uint32_m codepoint);

// Gets the first slot
// Returns false if no slot
muBool CyGetFirstSlotInChunkedFile(CyChunkedFile* file, CyChunkSlot* slot);
// Gets the next slot
// Returns false if no slot
muBool CyGetNextSlotInChunkedFile(CyChunkSlot* slot);

// Returns whether or not a given slot is at the cursor
muBool CyIsSlotAtCursor(CyChunkedFile* file, CyChunkSlot* slot);

