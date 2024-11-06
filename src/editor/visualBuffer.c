// visualBuffer.c
// Used to visually represent a chunked file

#include "editor/visualBuffer.h"
#include "core/log.h"
#include "core/gfx.h"

#include <math.h>

/* Inner */

	// Sets proper offsets for a given codepoint slot
	void CyOffsetCodepoint(CyEditorBox* box, mug2DTextureArrayRect* rect, uint32_m codepoint, float mult, uint32_m i) {
		// Get glyph header
		uint16_m glyphID = CyFontGetGlyphID(box->font, codepoint);
		muttGlyphHeader header;
		if (mutt_result_is_fatal(mutt_glyph_header(&box->font->font, glyphID, &header))) {
			return;
		}

		// Calculate column and row
		int32_m r = (i / box->textDim[0]);
		int32_m c = i - (r * box->textDim[0]);

		// Get rglyph metrics
		muttRGlyph rglyph;
		mutt_rglyph_metrics(&box->font->font, &header, glyphID, &rglyph, box->font->pointSize, PPI);
		mutt_funits_punits_min_max(&box->font->font, &header, &rglyph, box->font->pointSize, PPI);

		// Offset x-value by lsb
		rect->center.pos[0] = (((float)c) * box->font->pAdvanceWidth) + (box->font->layerDim[0] * 0.5f);
		rect->center.pos[0] += rglyph.lsb * mult;
		rect->center.pos[0] = roundf(rect->center.pos[0]) + .5f;
		// Offset y-value by this magic thing that works
		rect->center.pos[1] = (((float)r) * box->font->pHeight) - (box->font->layerDim[1] * 0.5f);
		rect->center.pos[1] += ((rglyph.ascender * 2.f) - rglyph.descender) * mult;
		rect->center.pos[1] = roundf(rect->center.pos[1]);
	}

	// Sets a codepoint given index
	void CySetCodepoint(CyEditorBox* box, uint32_m i, uint32_m codepoint) {
		// De-offset codepoint offsets
		CyOffsetCodepoint(box, &box->textRects[i], box->slots[i].codepoint, -1.f, i);

		// Get texture and layer for codepoint
		uint16_m texture, layer;
		CyFontGetTexture(box->font, codepoint, &texture, &layer);

		// Change texture info
		box->textRects[i].tex_pos[2] = layer;
		box->slots[i].texture = texture;
		box->slots[i].layer = layer;
		box->slots[i].codepoint = codepoint;

		// Offset codepoint
		CyOffsetCodepoint(box, &box->textRects[i], codepoint, 1.f, i);
	}

	// Calculates the default info of a texture rect
	void CyDefaultTextureCodepoint(CyEditorBox* box, uint32_m c, uint32_m r, uint32_m i, uint32_m layer) {
		mug2DTextureArrayRect* rect = &box->textRects[i];

		// Z-value:
		rect->center.pos[2] =
			// Percentage of way through numRects
			((((float)i) / ((float)box->numRects))
			// Half it and add half to make it range 0.5 to 1.0
			* 0.5f) + 0.5f
		;

		// Offset value for space
		CyOffsetCodepoint(box, rect, 0x20, 1.f, i);

		// Color
		rect->center.col[0] = rect->center.col[1] =
		rect->center.col[2] = rect->center.col[3] = 1.f;
		// Dimensions
		rect->dim[0] = box->font->layerDim[0];
		rect->dim[1] = box->font->layerDim[1];

		// Rotation
		rect->rot = 0.f;

		// Cut-out
		rect->tex_pos[0] = rect->tex_pos[1] = 0.f;
		rect->tex_pos[2] = layer;
		rect->tex_dim[0] = rect->tex_dim[1] = 1.f;
	}

	// Calculates the default info of a color rect
	void CyDefaultColorCodepoint(CyEditorBox* box, uint32_m c, uint32_m r, uint32_m i) {
		mugRect* rect = &box->colRects[i];

		// X-value:
		rect->center.pos[0] =
			// Offset center to generally where slot is on the X
			(((float)c) * box->font->pAdvanceWidth)
			// Offset by half dimensions to account for position being center of rect
			+ (box->font->pAdvanceWidth * 0.5f)
		;

		// Y-value:
		rect->center.pos[1] =
			// Offset center to generally where slot is on the Y
			(((float)r) * box->font->pHeight)
			// Offset by half dimensions to account for position being center of rect
			+ (box->font->pHeight * 0.5f)
		;

		// Z-value:
		rect->center.pos[2] =
			// Percentage of way through numRects
			(((float)i) / ((float)box->numRects))
			// Half it to make it range 0.0 to 0.5
			* 0.5f
		;

		// Color:
		rect->center.col[0] = rect->center.col[1] =
		rect->center.col[2] = 0.f;
		rect->center.col[3] = 1.f;

		// Dimensions:
		rect->dim[0] = box->font->pAdvanceWidth;
		rect->dim[1] = box->font->pHeight;

		// Rotation:
		rect->rot = 0.f;
	}

	// Sets default codepoint information
	void CySetDefaultCodepoint(CyEditorBox* box, uint32_m c, uint32_m r, uint16_m texture, uint16_m layer) {
		uint32_m i = (r * box->textDim[0]) + c;
		CyEditorBoxSlot* slot = &box->slots[i];

		// Set default info
		CyDefaultTextureCodepoint(box, c, r, i, layer);
		CyDefaultColorCodepoint(box, c, r, i);
		slot->texture = texture;
		slot->layer = layer;
		slot->codepoint = 0x20;
	}

/* Outer */

	// Initializes a text box
	// Point size of font should be set beforehand
	// Logs messages
	muBool CyInitEditorBox(CyEditorBox* box, CyFont* font, float max_width, float max_height) {
		CyLog("Creating empty chunked file...\n");
		// Set font and file
		box->font = font;
		if (!CyCreateEmptyChunkedFile(&box->file)) {
			CyLog("Failed to allocate chunked file\n");
			return MU_FALSE;
		}

		// Set dimensions of text box
		box->textDim[0] = floor(max_width / font->pAdvanceWidth);
		box->textDim[1] = floor(max_height / font->pHeight);
		box->numRects = box->textDim[0] * box->textDim[1];

		// Allocate textRects, colRects, and slots
		CyLog("Allocating visual and informational slot information...\n");

		box->textRects = (mug2DTextureArrayRect*)malloc(sizeof(mug2DTextureArrayRect) * box->numRects);
		if (!box->textRects) {
			CyLog("Failed to allocate texture rects\n");
			CyDestroyChunkedFile(&box->file);
			return MU_FALSE;
		}

		box->colRects = (mugRect*)malloc(sizeof(mugRect) * box->numRects);
		if (!box->colRects) {
			CyLog("Failed to allocate color rects\n");
			free(box->textRects);
			CyDestroyChunkedFile(&box->file);
			return MU_FALSE;
		}

		box->slots = (CyEditorBoxSlot*)malloc(sizeof(CyEditorBoxSlot) * box->numRects);
		if (!box->slots) {
			CyLog("Failed to allocate per-slot information\n");
			free(box->colRects);
			free(box->textRects);
			CyDestroyChunkedFile(&box->file);
			return MU_FALSE;
		}

		// Fill in default info for arrays
		CyLog("Setting default slot information...\n");

		uint16_m space_texture, space_layer;
		CyFontGetTexture(font, 0x20, &space_texture, &space_layer);

		// Loop through each column and row
		for (uint32_m c = 0; c < box->textDim[0]; ++c) {
			for (uint32_m r = 0; r < box->textDim[1]; ++r) {
				CySetDefaultCodepoint(box, c, r, space_texture, space_layer);
			}
		}

		// Create and fill buffers
		CyLog("Creating and filling graphical buffers...\n");

		box->textRectBuf = mu_gobjects_create(gfx, MUG_OBJECT_TEXTURE_2D_ARRAY, box->numRects, box->textRects);
		if (!box->textRectBuf) {
			CyLog("Failed to create texture object buffer\n");
			free(box->slots);
			free(box->colRects);
			free(box->textRects);
			CyDestroyChunkedFile(&box->file);
		}

		box->colRectBuf = mu_gobjects_create(gfx, MUG_OBJECT_RECT, box->numRects, box->colRects);
		if (!box->colRectBuf) {
			CyLog("Failed to create color object buffer\n");
			mu_gobjects_destroy(gfx, box->textRectBuf);
			free(box->slots);
			free(box->colRects);
			free(box->textRects);
			CyDestroyChunkedFile(&box->file);
		}

		CyLog("\n");
		return MU_TRUE;
	}

	// Destroys a text box
	// Logs messages
	void CyDestroyEditorBox(CyEditorBox* box) {
		CyLog("Destroying graphical buffers...\n");
		mu_gobjects_destroy(gfx, box->colRectBuf);
		mu_gobjects_destroy(gfx, box->textRectBuf);

		CyLog("Deallocating visual and informational slot information...\n");
		free(box->slots);
		free(box->colRects);
		free(box->textRects);

		CyLog("Destroying chunked file...\n");
		CyDestroyChunkedFile(&box->file);
	}

	// Renders text box
	void CyRenderEditorBox(CyEditorBox* box) {
		// Render background rects
		mu_gobjects_render(gfx, box->colRectBuf);

		// Render spans of slots with the same texture
		uint32_m prev_i = 0;
		for (uint32_m i = 1; i < box->numRects; ++i) {
			if (box->slots[i].texture != box->slots[i-1].texture) {
				mu_gobjects_texture(gfx, box->textRectBuf, box->font->textures[box->slots[i-1].texture]);
				mu_gobjects_subrender(gfx, box->textRectBuf, prev_i, i-prev_i);
				prev_i = i;
			}
			if (i+1 == box->numRects) {
				mu_gobjects_texture(gfx, box->textRectBuf, box->font->textures[box->slots[i-1].texture]);
				mu_gobjects_subrender(gfx, box->textRectBuf, prev_i, box->numRects - prev_i);
			}
		}
	}

	// Refreshes the editor box to represent the chunked file
	void CyRefreshEditorBox(CyEditorBox* box) {
		CyChunkSlot chunkSlot;
		muBool moreCodepoints = CyGetFirstSlotInChunkedFile(&box->file, &chunkSlot);

		// Loop through each slot
		for (uint32_m i = 0; i < box->numRects; ++i) {
			// Set to just space if no more codepoints exist in the chunked file
			if (!moreCodepoints) {
				CySetCodepoint(box, i, 0x20);
				continue;
			}

			// Set corresponding chunked file codepoint
			// @TODO Newline and tab stuff here
			CySetCodepoint(box, i, chunkSlot.codepoint);

			// Get next chunked file codepoint
			moreCodepoints = CyGetNextSlotInChunkedFile(&chunkSlot);
		}

		// Refill texture buffer
		mu_gobjects_fill(gfx, box->textRectBuf, box->textRects);
	}

