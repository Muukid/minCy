// visualBuffer.h
// Used to visually represent a chunked file

// An individual slot within the text box has a width of advance width
// and a height of abs((ascender - descender) + abs(line gap))

#include "libs/libs.h"
#include "rendering/fonts.h"
#include "editor/textBuffer.h"

// Struct representing an individual slot in the editor
struct CyEditorBoxSlot {
	// Texture index into CyEditorBox.font->textures[]
	uint16_m texture;
	// Layer index for the texture
	uint16_m layer;
	// Codepoint
	uint32_m codepoint;
};
typedef struct CyEditorBoxSlot CyEditorBoxSlot;

// Struct representing an editor box
struct CyEditorBox {
	// Respective font; should at least be monospace
	CyFont* font;
	// Respective chunked file
	CyChunkedFile file;
	// Width and height of entire box, in columns and rows
	uint32_m textDim[2];

	// Amount of rects; textDim[0] * textDim[1]
	uint32_m numRects;
	// The text texture rects + buffer
	mug2DTextureArrayRect* textRects;
	mugObjects textRectBuf;
	// Background color rects
	mugRect* colRects;
	mugObjects colRectBuf;

	// Cursor info
	uint32_m cursorWidth;
	mugRect cursorRect;
	mugObjects cursorRectBuf;

	// Individual slot info
	CyEditorBoxSlot* slots;
};
typedef struct CyEditorBox CyEditorBox;

// Initializes a text box
// Point size of font should be set beforehand
// Logs messages
muBool CyInitEditorBox(CyEditorBox* box, CyFont* font, float max_width, float max_height);

// Destroys a text box
// Logs messages
void CyDestroyEditorBox(CyEditorBox* box);

// Renders text box
void CyRenderEditorBox(CyEditorBox* box);

// Refreshes the editor box to represent the chunked file
void CyRefreshEditorBox(CyEditorBox* box);

