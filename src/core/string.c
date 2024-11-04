// string.c
// Used for the logic of Unicode strings

#include "core/string.h"

// Converts a single UTF-8 portion to a codepoint
// Returns how much to increment data by; 0 means failed
// len must be at least 1, and len must exclude null-terminating byte
// codepoint should be initialized to 0
uint8_m CyUTF8CodepointDecode(uint32_m* codepoint, muByte* data, uint32_m len) {
	uint8_m byte = *data;
	// 1-byte (00000000 <= n <= 01111111)
	if (byte <= 0x7F) {
		*codepoint = byte;
		return 1;
	}
	// 2-byte (11000000 <= n < 11100000)
	else if (byte >= 0xC0 && byte < 0xE0) {
		if (len < 2) {
			return 0;
		}
		*codepoint =
			// 110xxxxx
			  (uint32_m)(data[0] & 0x1F) << 6
			// 10xxxxxx
			| (uint32_m)(data[1] & 0x3F)
		;
		return 2;
	}
	// 3-byte (11100000 <= n < 11110000)
	else if (byte >= 0xE0 && byte < 0xF0) {
		if (len < 3) {
			return 0;
		}
		*codepoint =
			// 1110xxxx
			  (uint32_m)(data[0] & 0xF) << 12
			// 10xxxxxx
			| (uint32_m)(data[1] & 0x3F) << 6
			// 10xxxxxx
			| (uint32_m)(data[2] & 0x3F)
		;
		return 3;
	}
	// 4-byte (11110000 <= n <= 11110111)
	else if (byte >= 0xF0 && byte <= 0xF7) {
		if (len < 4) {
			return 0;
		}
		*codepoint =
			// 1110xxxx
			  (uint32_m)(data[0] & 0x7) << 18
			// 10xxxxxx
			| (uint32_m)(data[1] & 0x3F) << 12
			// 10xxxxxx
			| (uint32_m)(data[2] & 0x3F) << 6
			// 10xxxxxx
			| (uint32_m)(data[3] & 0x3F)
		;
		return 4;
	}
	// NA (illegal encoding)
	return 0;
}


// Converts a single codepoint to UTF-8
// Returns how much of the 4-byte buffer it filled up (4 at max); 0 means failed
// If data is 0, simply returns how many bytes it will take up; 0 means failed
// Does not print a log
uint8_m CyCodepointUTF8Encode(uint32_m codepoint, muByte* data) {
	// Calculate length of codepoint
	uint8_m len = 0;
	// - Storable in 7 bits = 1 byte
	if (codepoint < 128) {
		len = 1;
	}
	// - Storable in 11 bits = 2 bytes
	else if (codepoint < 2048) {
		len = 2;
	}
	// - Storable in 16 bits = 3 bytes
	else if (codepoint < 65536) {
		len = 3;
	}
	// - Storable in 21 bits = 4 bytes
	else if (codepoint < 2097152) {
		len = 4;
	}
	// - Not storable :L
	else {
		return 0;
	}

	// If no data is given, just return the length
	if (!data) {
		return len;
	}

	// Encode data based on previously calculated length
	switch (len) {
		default: break;

		// 1-byte: just write
		case 1: {
			data[0] = (muByte)codepoint;
		} break;

		// 2-byte
		case 2: {
			// 110xxxxx
			data[0] = (muByte)(((codepoint >> 6)  & 31) | 192);
			// 10xxxxxx
			data[1] = (muByte)((codepoint         & 63) | 128);
		} break;

		// 3-byte
		case 3: {
			// 1110xxxx
			data[0] = (muByte)(((codepoint >> 12) & 15) | 124);
			// 10xxxxxx
			data[1] = (muByte)(((codepoint >> 6)  & 63) | 128);
			// 10xxxxxx
			data[2] = (muByte)((codepoint         & 63) | 128);
		} break;

		// 4-byte
		case 4: {
			// 11110xxx
			data[0] = (muByte)(((codepoint >> 18) & 7)  | 240);
			// 10xxxxxx
			data[1] = (muByte)(((codepoint >> 12) & 63) | 128);
			// 10xxxxxx
			data[2] = (muByte)(((codepoint >> 6)  & 63) | 128);
			// 10xxxxxx
			data[3] = (muByte)((codepoint         & 63) | 128);
		} break;
	}

	// Return length
	return len;
}

