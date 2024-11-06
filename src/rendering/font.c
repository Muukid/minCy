// font.c
// Handles loading a font and making textures for it

// @TODO Clean up!!!

#include "rendering/font.h"
#include "libs/libs.h"
#include "core/log.h"
#include "core/gfx.h"

#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <math.h>

// Generates a CyFont based on filename
// Returns if it failed or succeeded
muBool CyLoadFont(CyFont* font, const char* filename) {
	// Initialize members within font
	font->numTextures = 0;

	// Open file in binary
	CyLog("Opening font file \"%s\"...\n", filename);
	FILE* fptr = fopen(filename, "rb");
	if (!fptr) {
		CyLog("Unable to find/open file\n");
		return MU_FALSE;
	}

	// Get size of file
	CyLog("Getting size of file... ");
	fseek(fptr, 0L, SEEK_END);
	size_m fptrSize = ftell(fptr);
	fseek(fptr, 0L, SEEK_SET);
	CyLog("size: %" PRIu32 " bytes\n", (uint32_m)fptrSize);

	// Load data into buffer
	CyLog("Loading data into buffer...\n");
	muByte* data = (muByte*)malloc(fptrSize);
	if (!data) {
		CyLog("Unable to allocate buffer\n");
		fclose(fptr);
		return MU_FALSE;
	}
	fread(data, fptrSize, 1, fptr);

	// Close file
	CyLog("Closing file...\n");
	fclose(fptr);

	// Load everything in the font
	CyLog("Loading font...\n");
	muttResult mutt_res = mutt_load(data, (uint64_m)fptrSize, &font->font, MUTT_LOAD_ALL);
	free(data);

	// Print result of loading
	if (mutt_res != MUTT_SUCCESS) {
		CyLog("Loading font wasn't fully successful (%s)\n", mutt_result_get_name(mutt_res));
		// Return bad value if fatal
		if (mutt_result_is_fatal(mutt_res)) {
			CyLog("Bad result value is fatal\n");
			return MU_FALSE;
		}
	}
	CyLog("Successfully loaded font file \"%s\"\n", filename);

	// Print if some tables failed to load
	if (font->font.fail_load_flags) {
		CyLog("Some tables failed to load\n");
	} else {
		CyLog("All tables loaded successfully\n");
	}

	// Allocate rglyph data buffer
	font->rdata = (muByte*)malloc(mutt_header_rglyph_max(&font->font));
	if (!font->rdata) {
		CyLog("Unable to allocate rglyph data buffer\n");
		mutt_deload(&font->font);
		return MU_FALSE;
	}

	// Tell if the font is monospace or not
	// This is performed by seeing if the codepoint 'i' has the
	// same width as 'm'; this is a bit rough, but I can't think
	// of a better way.
	font->monospace = MU_FALSE;
	{
		uint16_m iId = mutt_get_glyph(&font->font, 0x69);
		uint16_m mId = mutt_get_glyph(&font->font, 0x6D);
		if (iId != mId) {
			// Monospace if:
			// Both are over the numberOfHMetrics
			if (iId >= font->font.hhea->number_of_hmetrics && mId >= font->font.hhea->number_of_hmetrics) {
				font->monospace = MU_TRUE;
				font->advanceWidth = font->font.hmtx->hmetrics[font->font.hhea->number_of_hmetrics-1].advance_width;
			}
			// Both advanceWidths are the same
			else if (font->font.hmtx->hmetrics[iId].advance_width == font->font.hmtx->hmetrics[mId].advance_width) {
				font->monospace = MU_TRUE;
				font->advanceWidth = font->font.hmtx->hmetrics[iId].advance_width;
			}
		}
	}

	return MU_TRUE;
}

// Destroys a font
void CyDestroyFont(CyFont* font) {
	CyLog("Deloading font...\n");

	// Free rglyph buffer
	free(font->rdata);

	// Destroy all textures
	for (uint16_m i = 0; i < font->numTextures; ++i) {
		font->textures[i] = mu_gtexture_destroy(gfx, font->textures[i]);
	}
	font->numTextures = 0;

	// Deload font
	mutt_deload(&font->font);
}

// Sets the point size of a font
// Also deloads all font layers
void CySetFontPointSize(CyFont* font, float pointSize) {
	// Set point size and destroy all texture layers
	font->pointSize = pointSize;
	// Destroy all textures
	for (uint16_m i = 0; i < font->numTextures; ++i) {
		font->textures[i] = mu_gtexture_destroy(gfx, font->textures[i]);
	}
	font->numTextures = 0;

	// Determine layer dimensions based on point size
	font->layerDim[0] = (mutt_funits_to_punits(&font->font, font->font.head->x_max - font->font.head->x_min, pointSize, PPI)) + 2;
	font->layerDim[1] = (mutt_funits_to_punits(&font->font, font->font.head->y_max - font->font.head->y_min, pointSize, PPI)) + 2;

	// Pixel-unit info
	font->pAdvanceWidth = mutt_funits_to_punits(&font->font, font->advanceWidth, pointSize, PPI);
	font->pAscender = mutt_funits_to_punits(&font->font, font->font.hhea->ascender, pointSize, PPI);
	font->pDescender = mutt_funits_to_punits(&font->font, font->font.hhea->descender, pointSize, PPI);
	font->pLineGap = mutt_funits_to_punits(&font->font, font->font.hhea->line_gap, pointSize, PPI);
	font->pHeight = fabs((font->pAscender - font->pDescender) + fabs(font->pLineGap));
}

// Loads a codepoint into a font range
// Should be confirmed that given codepoint doesn't already
// have a range before calling this...
muBool CyFontLoadCodepoint(CyFont* font, uint32_m codepoint) {
	CyLog("Loading new texture for codepoint range...\n");

	// Increase number of textures
	if (font->numTextures == FONT_RANGE_BUF-1) {
		CyLog("Reached maximum amount of %i ranges per font\n", FONT_RANGE_BUF);
		return MU_FALSE;
	}
	uint32_m i = font->numTextures++;

	// Determine start code
	font->textureStartCodes[i] = floor(((float)codepoint) / ((float)FONT_LAYERS)) * FONT_LAYERS;
	CyLog("Range %" PRIu32 " - %" PRIu32 "\n", font->textureStartCodes[i], font->textureStartCodes[i]+(FONT_LAYERS-1));

	CyLog("Rasterizing codepoints within range...\n");

	// Allocate pixels
	muByte* pixels = (muByte*)malloc(font->layerDim[0] * font->layerDim[1] * FONT_LAYERS * 4);
	if (!pixels) {
		CyLog("Failed to allocate pixels\n");
		font->numTextures--;
		return MU_FALSE;
	}
	memset(pixels, 0, font->layerDim[0] * font->layerDim[1] * FONT_LAYERS * 4);
	// Describe bitmap
	muttRBitmap bitmap;
	bitmap.width = font->layerDim[0];
	bitmap.height = font->layerDim[1];
	bitmap.channels = MUTTR_RGBA;
	bitmap.stride = bitmap.width*4;
	bitmap.io_color = MUTTR_BW;
	bitmap.pixels = pixels;

	// Loop through each character in codepoint range
	for (uint32_m c = 0; c < FONT_LAYERS; ++c) {
		// Get glyph ID (default on missing codepoint)
		uint32_m thisCodepoint = font->textureStartCodes[i] + c;
		uint16_m glyph_id = CyFontGetGlyphID(font, thisCodepoint);

		// Load header
		muttGlyphHeader header;
		muttResult mutt_res = mutt_glyph_header(&font->font, glyph_id, &header);
		if ((mutt_result_is_fatal(mutt_res)) || (header.number_of_contours == 0)) {
			CyLog("Codepoint %" PRIu32 "'s header failed to load (may be empty, this is normal); skipping\n", thisCodepoint);
			bitmap.pixels += bitmap.stride * bitmap.height;
			continue;
		}

		// Load rglyph
		muttRGlyph rglyph;
		mutt_res = mutt_header_rglyph(&font->font, &header, &rglyph, font->pointSize, PPI, font->rdata, 0);
		if (mutt_result_is_fatal(mutt_res)) {
			CyLog("Codepoint %" PRIu32 "'s raster glyph failed to load; skipping\n", thisCodepoint);
			bitmap.pixels += bitmap.stride * bitmap.height;
			continue;
		}

		// Rasterize
		mutt_res = mutt_raster_glyph(&rglyph, &bitmap, FONT_RENDER_METHOD);
		if (mutt_result_is_fatal(mutt_res)) {
			CyLog("Codepoint %" PRIu32 " failed to rasterize\n", thisCodepoint);
		}
		bitmap.pixels += bitmap.stride * bitmap.height;
	}

	// Create texture
	CyLog("Finished rasterizing; creating texture...\n");
	mugTextureInfo texInfo;
	texInfo.type = MUG_TEXTURE_2D_ARRAY;
	texInfo.format = MUG_TEXTURE_U8_RGBA;
	texInfo.wrapping[0] = texInfo.wrapping[1] = MUG_TEXTURE_CLAMP;
	texInfo.filtering[0] = texInfo.filtering[1] = MUG_TEXTURE_NEAREST;
	uint32_m dim[3] = { font->layerDim[0], font->layerDim[1], FONT_LAYERS };
	font->textures[i] = mu_gtexture_create(gfx, &texInfo, dim, pixels);
	free(pixels);
	if (font->textures[i] == 0) {
		CyLog("Failed to create texture\n");
		font->numTextures--;
		return MU_FALSE;
	}

	return MU_TRUE;
}

// Checks if the given codepoint has been loaded into a font
// Sets layer if found
muBool CyFontIsCodepointLoaded(CyFont* font, uint32_m codepoint, uint16_m* texture, uint16_m* layer) {
	for (uint16_m i = 0; i < font->numTextures; ++i) {
		if (codepoint >= font->textureStartCodes[i] && codepoint <= (font->textureStartCodes[i] + FONT_LAYERS)) {
			if (texture) {
				*texture = i;
			}
			if (layer) {
				*layer = codepoint - font->textureStartCodes[i];
			}
			return MU_TRUE;
		}
	}
	return MU_FALSE;
}

// Gets the texture and layer of a codepoint
// Defaults on missing character
void CyFontGetTexture(CyFont* font, uint32_m codepoint, uint16_m* texture, uint16_m* layer) {
	for (uint16_m i = 0; i < font->numTextures; ++i) {
		if (codepoint >= font->textureStartCodes[i] && codepoint <= (font->textureStartCodes[i] + FONT_LAYERS)) {
			*texture = i;
			*layer = codepoint - font->textureStartCodes[i];
			return;
		}
	}
	CyFontGetTexture(font, 0x25A1, texture, layer);
}

// Gets a codepoint from a font
// Defaults on missing
uint16_m CyFontGetGlyphID(CyFont* font, uint32_m codepoint) {
	uint16_m glyph_id = mutt_get_glyph(&font->font, codepoint);
	if (glyph_id == 0) {
		glyph_id = mutt_get_glyph(&font->font, 0x25A1);
	}
	return glyph_id;
}

