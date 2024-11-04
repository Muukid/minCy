// string.h
// Used for the logic of Unicode strings

#include "libs/libs.h"

// Converts a single UTF-8 portion to a codepoint
// Returns how much to increment data by; 0 means failed
// len must be at least 1, and len must exclude null-terminating byte
// codepoint should be initialized to 0
uint8_m CyUTF8CodepointDecode(uint32_m* codepoint, muByte* data, uint32_m len);


// Converts a single codepoint to UTF-8
// Returns how much of the 4-byte buffer it filled up (4 at max); 0 means failed
// If data is 0, simply returns how many bytes it will take up; 0 means failed
// Does not print a log
uint8_m CyCodepointUTF8Encode(uint32_m codepoint, muByte* data);

