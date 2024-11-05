// font.h
// Handles loading a font and making textures for it

#include "libs/libs.h"

// Each font texture has 256 layers for 256 codepoints
#define FONT_LAYERS 256
// Maximum amount of loaded font ranges
// 256 ranges, 256 characters per range, 65536 max loaded codepoints
// Max amount of glyphs in a TrueType font is 65536 so this is usually fine
#define FONT_RANGE_BUF 256
// Assumed PPI of display
#define PPI 96.f
// Rendering method
#define FONT_RENDER_METHOD MUTTR_FULL_PIXEL_AA8X8

// Struct representing font to be rendered by mug
struct CyFont {
	// The TrueType font handle
	muttFont font;
	// Amount of textures
	uint16_m numTextures;
	// Texture handles
	mugTexture textures[FONT_RANGE_BUF];
	// Unicode beginning ranges for each texture
	uint32_m textureStartCodes[FONT_RANGE_BUF];
	// Point size that the textures are rendered at
	float pointSize;
	// Width and height of each texture layer
	uint32_m layerDim[2];
	// Rglyph data buffer
	muByte* rdata;
	// Whether or not the font is monospace
	muBool monospace;
	// Monospace advance width; undefined if !monospace
	uint16_m advanceWidth;

	// Pixel-unit info:
	float pAdvanceWidth;
	float pAscender;
	float pDescender;
	float pLineGap;
	float pHeight; // abs((ascender - descender) + abs(line gap))
	float pMaxWidth; // abs(x_max - x_min)
	float pMaxHeight; // abs(y_max - y_min)
	// Ceiling pixel-unit info
	uint32_m uAdvanceWidth;
	uint32_m uHeight;
};
typedef struct CyFont CyFont;

// Generates a CyFont based on filename
// Returns if it failed or succeeded
muBool CyLoadFont(CyFont* font, const char* filename);

// Destroys a font
void CyDestroyFont(CyFont* font);

// Sets the point size of a font
// Also deloads all font layers
void CySetFontPointSize(CyFont* font, float pointSize);

// Loads a codepoint into a font range
// Should be confirmed that given codepoint doesn't already
// have a range before calling this...
muBool CyFontLoadCodepoint(CyFont* font, uint32_m codepoint);

// Checks if the given codepoint has been loaded into a font
// Sets layer if found
muBool CyFontIsCodepointLoaded(CyFont* font, uint32_m codepoint, uint16_m* texture, uint16_m* layer);

// Gets the texture and layer of a codepoint
// Defaults on missing character
void CyFontGetTexture(CyFont* font, uint32_m codepoint, uint16_m* texture, uint16_m* layer);

