// visualBuffer.c
// Used to visually represent a chunked file

#include "editor/visualBuffer.h"
#include "core/log.h"
#include "core/gfx.h"

#include <math.h>

/* Inner */

	// Sets a codepoint given index
	void CySetCodepoint(CyEditorBox* box, uint32_m i, uint32_m codepoint) {
		// Get texture and layer for codepoint
		uint16_m texture, layer;
		CyFontGetTexture(box->font, codepoint, &texture, &layer);

		// Change texture info
		box->textRects[i].tex_pos[2] = layer;
		box->slots[i].texture = texture;
		box->slots[i].layer = layer;
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
		box->textDim[0] = floor(max_width / font->uAdvanceWidth);
		box->textDim[1] = floor(max_height / font->uHeight);
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
				uint32_m i = (r * box->textDim[0]) + c;

				// Set positions
				// - X
				box->textRects[i].center.pos[0] = (float)(c * font->uAdvanceWidth) + (font->layerDim[0] * 0.5f);
				box->colRects[i].center.pos[0] = (float)(c * font->uAdvanceWidth) + (((float)font->uAdvanceWidth) * 0.5f);
				// - Y
				box->textRects[i].center.pos[1] = (float)(r * font->uHeight) + (font->layerDim[1] * 0.5f) - font->uHeight;
				box->colRects[i].center.pos[1] = (float)(r * font->uHeight) + (((float)font->uHeight) * 0.5f);
				// - Z
				box->textRects[i].center.pos[2] = ((((float)i) / ((float)box->numRects)) * 0.5f) + 0.5f;
				box->colRects[i].center.pos[2] = ((((float)i) / ((float)box->numRects)) * 0.5f);

				// Set color
				// - Texture RGBA
				box->textRects[i].center.col[0] = box->textRects[i].center.col[1] =
				box->textRects[i].center.col[2] = box->textRects[i].center.col[3] = 1.f;
				// - Color RGBA
				box->colRects[i].center.col[0] = box->colRects[i].center.col[1] = box->colRects[i].center.col[2] = 0.f;
				box->colRects[i].center.col[3] = 1.f;

				// Set dimensions
				// - Texture
				box->textRects[i].dim[0] = font->layerDim[0];
				box->textRects[i].dim[1] = font->layerDim[1];
				// - Color
				box->colRects[i].dim[0] = font->uAdvanceWidth;
				box->colRects[i].dim[1] = font->uHeight;

				// Set rotation
				box->textRects[i].rot = 
				box->colRects[i].rot = 0.f;

				// Set texture cut-out
				box->textRects[i].tex_pos[0] = box->textRects[i].tex_pos[1] = 0.f;
				box->textRects[i].tex_pos[2] = space_layer;
				box->textRects[i].tex_dim[0] = box->textRects[i].tex_dim[1] = 1.f;

				// Set slot to space
				box->slots[i].texture = space_texture;
				box->slots[i].layer = space_layer;
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

		for (uint32_m i = 0; i < box->numRects; ++i) {
			if (!moreCodepoints) {
				CySetCodepoint(box, i, 0x20);
				continue;
			}

			// @TODO Newline and tab stuff here
			CySetCodepoint(box, i, chunkSlot.codepoint);

			moreCodepoints = CyGetNextSlotInChunkedFile(&chunkSlot);
		}

		mu_gobjects_fill(gfx, box->textRectBuf, box->textRects);
	}

