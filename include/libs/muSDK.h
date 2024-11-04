/*
muSDK.h - Muukid
Public domain single-file C library for general high-level software development.
https://github.com/Muukid/muSDK
No warranty implied; use at your own risk.

Licensed under (MIT License or public domain, whichever you prefer) AND Apache 2.0.
More explicit license information at the end of file.
*/

/* @DOCBEGIN

# muSDK v1.0.0

muSDK is a public domain single-file C library for general high-level software development. Its header is automatically defined upon inclusion if not already included (`MUSDK_H`), and the source code is defined if `MUSDK_IMPLEMENTATION` is defined, following the internal structure of:

```c
#ifndef MUSDK_H
	#define MUSDK_H
	// (Header code)
#endif

#ifdef MUSDK_IMPLEMENTATION
	// (Source code)
#endif
```

Therefore, a standard inclusion of the file to get all automatic functionality looks like:

```c
#define MUSDK_IMPLEMENTATION
#include "muSDK.h"
```

More information about the general structure of a mu library is provided at [the mu library information GitHub repository](https://github.com/Muukid/mu-library-information).

# Demos

Demos are designed for muSDK to both test its functionality and to allow users to get the basic idea of the structure of the library quickly without having to read the documentation in full. These demos are available in the `demos` folder.

## Demo resources

The demos use other files to operate correctly when running as a compiled executable. These other files can be found in the `resources` folder within `demos`, and this folder is expected to be in the same location that the program is executing from. For example, if a user compiles a demo into `main.exe`, and decides to run it, the `resources` folder from `demos` should be in the same directory as `main.exe`.

The resources' licenses may differ from the licensing of muSDK itself; see the [license section that covers demo resources](#licensing-of-demo-resources).

# Licensing

muSDK is licensed under public domain or MIT, whichever you prefer, as well as (technically) [Apache 2.0 due to OpenGL's licensing](https://github.com/KhronosGroup/OpenGL-Registry/issues/376#issuecomment-596187053).

## Licensing of demo resources

The resources used by the demos may differ from licensing of the demos themselves; in that context, their licenses apply, with licensing of each file available as a separate file with the same name, but with no filename extension.

# Libraries

Since muSDK is a general software development kit, it contains various libraries whose code needs to be toggled manually with macros, as otherwise, all libraries would be defined automatically, which can be inefficient in many circumstances. This section covers every library included in muSDK and how to define it, as well as macros that apply to each library.

Note that the macros that activate these libraries need to be defined BEFORE `muSDK.h` is included for the first time in order to function properly.

## Turn on all name macros

All mu libraries included in muSDK have name functions to convert some of their enumerators to `const char*` representations. To define all of the name functions for all of the libraries, define `MUSDK_ALL_NAMES` before `muSDK.h` is included for the first time.

@DOCEND */

// Debug; includes files instead of defining them:
// #define MUSDK_INCLUDE_LIBS

#ifndef MUSDK_H
	#define MUSDK_H

	// @DOCLINE ## muGraphics
	// @DOCLINE muSDK has support for [muGraphics v1.0.0](https://github.com/Muukid/muGraphics/releases/tag/v1.0.0). The macro to define it is `MUSDK_MUG`.
	// @IGNORE
	#ifdef MUSDK_MUG
		// Names
		#ifdef MUSDK_ALL_NAMES
			#define MUG_NAMES
			#define MUCOSA_NAMES
		#endif

		// Debug inclusion
		#ifdef MUSDK_INCLUDE_LIBS
			#include "muGraphics.h"
		#endif

		// Header
		#ifndef MUG_H
			#define MUG_H
			
			// @DOCLINE # Other library dependencies
			// @DOCLINE mug has a dependency on:

			// @DOCLINE * [muCOSA v2.0.1](https://github.com/Muukid/muCOSA/releases/tag/v2.0.1).
			// @IGNORE
				#if !defined(MU_CHECK_VERSION_MISMATCHING) && defined(MUCOSA_H) && (MUCOSA_VERSION_MAJOR != 2 || MUCOSA_VERSION_MINOR != 0 || MUCOSA_VERSION_PATCH != 1)
					#pragma message("[MUG] muCOSA's header has already been defined, but version doesn't match the version that this library is built for. This may lead to errors, warnings, or unexpected behavior. Define MU_CHECK_VERSION_MISMATCHING before this to turn off this message.")
				#endif
				#ifndef MUCOSA_H
					#define MUCOSA_H
					
					// @DOCLINE # Other library dependencies
					// @DOCLINE muCOSA has a dependency on:

					// @DOCLINE * [muUtility v2.0.1](https://github.com/Muukid/muUtility/releases/tag/v2.0.1).
					// @IGNORE
						#if !defined(MU_CHECK_VERSION_MISMATCHING) && defined(MUU_H) && \
							(MUU_VERSION_MAJOR != 2 || MUU_VERSION_MINOR != 0 || MUU_VERSION_PATCH != 1)

							#pragma message("[MUCOSA] muUtility's header has already been defined, but version doesn't match the version that this library is built for. This may lead to errors, warnings, or unexpected behavior. Define MU_CHECK_VERSION_MISMATCHING before this to turn off this message.")

						#endif

						#ifndef MUU_H
							#define MUU_H

							// @DOCLINE # Version
								// @DOCLINE The macros `MUU_VERSION_MAJOR`, `MUU_VERSION_MINOR`, and `MUU_VERSION_PATCH` are defined to match its respective release version, following the formatting of `MAJOR.MINOR.PATCH`.
								
								#define MUU_VERSION_MAJOR 2
								#define MUU_VERSION_MINOR 0
								#define MUU_VERSION_PATCH 1

							// @DOCLINE # `MUDEF`
								// @DOCLINE The `MUDEF` macro is used by virtually all mu libraries, and is generally added before a header-defined variable or function. Its default value is `extern`, but can be changed to `static` by defining `MU_STATIC` before the header section of muUtility is defined. Its value can also be overwritten entirely to anything else by directly defining `MUDEF`.
								
								#ifndef MUDEF
									#ifdef MU_STATIC
										#define MUDEF static
									#else
										#define MUDEF extern
									#endif
								#endif
							
							// @DOCLINE # Secure warnings
								// @DOCLINE mu libraries often use non-secure functions that will trigger warnings on certain compilers. These warnings are, to put it lightly, dumb, so muUtility defines `_CRT_SECURE_NO_WARNINGS`. However, it is not guaranteed that this definition will actually turn the warnings off, which, at that point, they have to be manually turned off by the user. This functionality can be turned off by defining `MU_SECURE_WARNINGS`.
								#if !defined(MU_SECURE_WARNINGS) && !defined(_CRT_SECURE_NO_WARNINGS)
									#define _CRT_SECURE_NO_WARNINGS
								#endif
							
							// @DOCLINE # C++ extern
								// @DOCLINE Every mu library's primary code externs "C" within the main chunks of their code; macros `MU_CPP_EXTERN_START` and `MU_CPP_EXTERN_END` are defined to make this process easier, and would read like this:
								/* @DOCBEGIN
								```
								MU_CPP_EXTERN_START

								// Library code goes here...

								MU_CPP_EXTERN_END
								```
								@DOCEND */
								#ifdef __cplusplus
									#define MU_CPP_EXTERN_START extern "C" {
									#define MU_CPP_EXTERN_END   }
								#else
									#define MU_CPP_EXTERN_START
									#define MU_CPP_EXTERN_END
								#endif
							
							MU_CPP_EXTERN_START

							// @DOCLINE # C standard library dependencies

								// @DOCLINE muUtility has several C standard library dependencies, all of which are overridable by defining them before the inclusion of the file. The following is a list of those dependencies.

								// @DOCLINE ## `stdint.h` dependencies
								#if !defined(int8_m) || \
									!defined(uint8_m) || \
									!defined(int16_m) || \
									!defined(uint16_m) || \
									!defined(int32_m) || \
									!defined(uint32_m) || \
									!defined(int64_m) || \
									!defined(uint64_m) || \
									!defined(MU_SIZE_MAX)

									#define __STDC_LIMIT_MACROS
									#define __STDC_CONSTANT_MACROS
									#include <stdint.h>
									
									// @DOCLINE * `int8_m` - equivalent to `int8_t` if `INT8_MAX` is defined; `char` if otherwise.
									#ifndef int8_m
										#ifdef INT8_MAX
											#define int8_m int8_t
										#else
											#define int8_m char
										#endif
									#endif

									// @DOCLINE * `uint8_m` - equivalent to `uint8_t` if `UINT8_MAX` is defined; `unsigned char` if otherwise.
									#ifndef uint8_m
										#ifdef UINT8_MAX
											#define uint8_m uint8_t
										#else
											#define uint8_m unsigned char
										#endif
									#endif

									// @DOCLINE * `int16_m` - equivalent to `int16_t` if `INT16_MAX` is defined; `short` if otherwise.
									#ifndef int16_m
										#ifdef INT16_MAX
											#define int16_m int16_t
										#else
											#define int16_m short
										#endif
									#endif

									// @DOCLINE * `uint16_m` - equivalent to `uint16_t` if `UINT16_MAX` is defined; `unsigned short` if otherwise.
									#ifndef uint16_m
										#ifdef UINT16_MAX
											#define uint16_m uint16_t
										#else
											#define uint16_m unsigned short
										#endif
									#endif

									// @DOCLINE * `int32_m` - equivalent to `int32_t` if `INT32_MAX` is defined; `long` if otherwise.
									#ifndef int32_m
										#ifdef INT32_MAX
											#define int32_m int32_t
										#else
											#define int32_m long
										#endif
									#endif

									// @DOCLINE * `uint32_m` - equivalent to `uint32_t` if `UINT32_MAX` is defined; `unsigned long` if otherwise.
									#ifndef uint32_m
										#ifdef UINT32_MAX
											#define uint32_m uint32_t
										#else
											#define uint32_m unsigned long
										#endif
									#endif

									// @DOCLINE * `int64_m` - equivalent to `int64_t` if `INT64_MAX` is defined; `long long` if otherwise.
									#ifndef int64_m
										#ifdef INT64_MAX
											#define int64_m int64_t
										#else
											#define int64_m long long
										#endif
									#endif

									// @DOCLINE * `uint64_m` - equivalent to `uint64_t` if `UINT64_MAX` is defined; `unsigned long long` if otherwise.
									#ifndef uint64_m
										#ifdef UINT64_MAX
											#define uint64_m uint64_t
										#else
											#define uint64_m unsigned long long
										#endif
									#endif

									// @DOCLINE * `MU_SIZE_MAX` - equivalent to `SIZE_MAX`.
									#ifndef MU_SIZE_MAX
										#define MU_SIZE_MAX SIZE_MAX
									#endif

								#endif /* stdint.h */

								// @DOCLINE ## `stddef.h` dependencies
								#if !defined(size_m)

									#include <stddef.h>

									// @DOCLINE * `size_m` - equivalent to `size_t`.
									#ifndef size_m
										#define size_m size_t
									#endif

								#endif /* stddef.h */

								// @DOCLINE ## `stdbool.h` dependencies
								#if !defined(muBool) || \
									!defined(MU_TRUE) || \
									!defined(MU_FALSE)

									#include <stdbool.h>

									// @DOCLINE * `muBool` - equivalent to `bool`.
									#ifndef muBool
										#define muBool bool
									#endif

									// @DOCLINE * `MU_TRUE` - equivalent to `true`.
									#ifndef MU_TRUE
										#define MU_TRUE true
									#endif

									// @DOCLINE * `MU_FALSE` - equivalent to `false`.
									#ifndef MU_FALSE
										#define MU_FALSE false
									#endif

								#endif /* stdbool.h */

							// @DOCLINE # Zero struct

								// @DOCLINE There are two macros, `MU_ZERO_STRUCT` and `MU_ZERO_STRUCT_CONST`, which are functions used to zero-out a struct's contents, with their only parameter being the struct type. The reason this needs to be defined is because the way C and C++ syntax handles an empty struct are different, and need to be adjusted for. These macros are overridable by defining them before `muUtility.h` is included.

								#ifndef MU_ZERO_STRUCT
									#ifdef __cplusplus
										#define MU_ZERO_STRUCT(s) {}
									#else
										#define MU_ZERO_STRUCT(s) (s){0}
									#endif
								#endif

								#ifndef MU_ZERO_STRUCT_CONST
									#ifdef __cplusplus
										#define MU_ZERO_STRUCT_CONST(s) {}
									#else
										#define MU_ZERO_STRUCT_CONST(s) {0}
									#endif
								#endif

							// @DOCLINE # Byte manipulation

								// @DOCLINE muUtility defines several inline functions that read a value from a given array of bytes. Internally, they're all defined with the prefix `muu_...`, and then a macro is defined for them as `MU_...` (with change in capitalization after the prefix as well). The macros for these functions can be overridden, and, in such case, the original function will go undefined. For example, the function `muu_rleu8` is primarily referenced via the macro `MU_RLEU8`, and if `MU_RLEU8` is overridden, `muu_rleu8` is never defined and is not referenceable.

								// @DOCLINE All reading functions take in a pointer of bytes as their only parameter and have a return type of the fixed-width size of bits in question; for example, `muu_rleu8` is defined as:

								/* @DOCBEGIN
								```
								MUDEF inline uint8_m muu_rleu8(muByte* b);
								```
								@DOCEND */

								// @DOCLINE All writing functions take in a pointer of bytes as their first parameter, the number to be written as the second parameter, and have a return type of void; for example, `muu_wleu8` is defined as:

								/* @DOCBEGIN
								```
								MUDEF inline void muu_wleu8(muByte* b, uint8_m n);
								```
								@DOCEND */

								// @DOCLINE The exception to this is 24-bit, in which case, the fixed-width types are 32-bit (`uint32_m` / `int32_m`).

								// @DOCLINE ## Byte type

									// @DOCLINE muUtility defines the type `muByte` to refer to a byte. It is defined as `uint8_m`, and is overridable.
									#ifndef muByte
										#define muByte uint8_m
									#endif

								// @DOCLINE ## 8-bit

									// @DOCLINE The following macros exist for byte manipulation regarding 8-bit integers:

									// @DOCLINE * `MU_RLEU8` - reads an unsigned 8-bit integer from little-endian byte data; overridable macro to `muu_rleu8`.
									#ifndef MU_RLEU8
										MUDEF inline uint8_m muu_rleu8(muByte* b) {
											return b[0];
										}
										#define MU_RLEU8 muu_rleu8
									#endif

									// @DOCLINE * `MU_WLEU8` - writes an unsigned 8-bit integer to little-endian byte data; overridable macro to `muu_wleu8`.
									#ifndef MU_WLEU8
										MUDEF inline void muu_wleu8(muByte* b, uint8_m n) {
											b[0] = n;
										}
										#define MU_WLEU8 muu_wleu8
									#endif

									// @DOCLINE * `MU_RLES8` - reads a signed 8-bit integer from little-endian byte data; overridable macro to `muu_rles8`.
									#ifndef MU_RLES8
										MUDEF inline int8_m muu_rles8(muByte* b) {
											// I'm PRETTY sure this is okay...
											return *(int8_m*)b;
										}
										#define MU_RLES8 muu_rles8
									#endif

									// @DOCLINE * `MU_WLES8` - writes a signed 8-bit integer to little-endian byte data; overridable macro to `muu_wles8`.
									#ifndef MU_WLES8
										MUDEF inline void muu_wles8(muByte* b, int8_m n) {
											((int8_m*)(b))[0] = n;
										}
										#define MU_WLES8 muu_wles8
									#endif

									// @DOCLINE * `MU_RBEU8` - reads an unsigned 8-bit integer from big-endian byte data; overridable macro to `muu_rbeu8`.
									#ifndef MU_RBEU8
										MUDEF inline uint8_m muu_rbeu8(muByte* b) {
											return b[0];
										}
										#define MU_RBEU8 muu_rbeu8
									#endif

									// @DOCLINE * `MU_WBEU8` - writes an unsigned 8-bit integer to big-endian byte data; overridable macro to `muu_wbeu8`.
									#ifndef MU_WBEU8
										MUDEF inline void muu_wbeu8(muByte* b, uint8_m n) {
											b[0] = n;
										}
										#define MU_WBEU8 muu_wbeu8
									#endif

									// @DOCLINE * `MU_RBES8` - reads a signed 8-bit integer from big-endian byte data; overridable macro to `muu_rbes8`.
									#ifndef MU_RBES8
										MUDEF inline int8_m muu_rbes8(muByte* b) {
											return *(int8_m*)b;
										}
										#define MU_RBES8 muu_rbes8
									#endif

									// @DOCLINE * `MU_WBES8` - writes a signed 8-bit integer to big-endian byte data; overridable macro to `muu_wbes8`.
									#ifndef MU_WBES8
										MUDEF inline void muu_wbes8(muByte* b, int8_m n) {
											((int8_m*)(b))[0] = n;
										}
										#define MU_WBES8 muu_wbes8
									#endif

								// @DOCLINE ## 16-bit

									// @DOCLINE The following macros exist for byte manipulation regarding 16-bit integers:

									// @DOCLINE * `MU_RLEU16` - reads an unsigned 16-bit integer from little-endian byte data; overridable macro to `muu_rleu16`.
									#ifndef MU_RLEU16
										MUDEF inline uint16_m muu_rleu16(muByte* b) {
											return (
												((uint16_m)(b[0]) << 0) |
												((uint16_m)(b[1]) << 8)
											);
										}
										#define MU_RLEU16 muu_rleu16
									#endif

									// @DOCLINE * `MU_WLEU16` - writes an unsigned 16-bit integer to little-endian byte data; overridable macro to `muu_wleu16`.
									#ifndef MU_WLEU16
										MUDEF inline void muu_wleu16(muByte* b, uint16_m n) {
											b[0] = (uint8_m)(n >> 0);
											b[1] = (uint8_m)(n >> 8);
										}
										#define MU_WLEU16 muu_wleu16
									#endif

									// @DOCLINE * `MU_RLES16` - reads a signed 16-bit integer from little-endian byte data; overridable macro to `muu_rles16`.
									#ifndef MU_RLES16
										MUDEF inline int16_m muu_rles16(muByte* b) {
											uint16_m u16 = muu_rleu16(b);
											return *(int16_m*)&u16;
										}
										#define MU_RLES16 muu_rles16
									#endif

									// @DOCLINE * `MU_WLES16` - writes a signed 16-bit integer to little-endian byte data; overridable macro to `muu_wles16`.
									#ifndef MU_WLES16
										MUDEF inline void muu_wles16(muByte* b, int16_m n) {
											uint16_m un = *(uint16_m*)&n;
											b[0] = (uint8_m)(un >> 0);
											b[1] = (uint8_m)(un >> 8);
										}
										#define MU_WLES16 muu_wles16
									#endif

									// @DOCLINE * `MU_RBEU16` - reads an unsigned 16-bit integer from big-endian byte data; overridable macro to `muu_rbeu16`.
									#ifndef MU_RBEU16
										MUDEF inline uint16_m muu_rbeu16(muByte* b) {
											return (
												((uint16_m)(b[1]) << 0) |
												((uint16_m)(b[0]) << 8)
											);
										}
										#define MU_RBEU16 muu_rbeu16
									#endif

									// @DOCLINE * `MU_WBEU16` - writes an unsigned 16-bit integer to big-endian byte data; overridable macro to `muu_wbeu16`.
									#ifndef MU_WBEU16
										MUDEF inline void muu_wbeu16(muByte* b, uint16_m n) {
											b[1] = (uint8_m)(n >> 0);
											b[0] = (uint8_m)(n >> 8);
										}
										#define MU_WBEU16 muu_wbeu16
									#endif

									// @DOCLINE * `MU_RBES16` - reads a signed 16-bit integer from big-endian byte data; overridable macro to `muu_rbes16`.
									#ifndef MU_RBES16
										MUDEF inline int16_m muu_rbes16(muByte* b) {
											uint16_m u16 = muu_rbeu16(b);
											return *(int16_m*)&u16;
										}
										#define MU_RBES16 muu_rbes16
									#endif

									// @DOCLINE * `MU_WBES16` - writes a signed 16-bit integer to big-endian byte data; overridable macro to `muu_wbes16`.
									#ifndef MU_WBES16
										MUDEF inline void muu_wbes16(muByte* b, int16_m n) {
											uint16_m un = *(uint16_m*)&n;
											b[1] = (uint8_m)(un >> 0);
											b[0] = (uint8_m)(un >> 8);
										}
										#define MU_WBES16 muu_wbes16
									#endif

								// @DOCLINE ## 24-bit

									// @DOCLINE The following macros exist for byte manipulation regarding 24-bit integers:

									// @DOCLINE * `MU_RLEU24` - reads an unsigned 24-bit integer from little-endian byte data; overridable macro to `muu_rleu24`.
									#ifndef MU_RLEU24
										MUDEF inline uint32_m muu_rleu24(muByte* b) {
											return (
												((uint32_m)(b[0]) << 0) |
												((uint32_m)(b[1]) << 8) |
												((uint32_m)(b[2]) << 16)
											);
										}
										#define MU_RLEU24 muu_rleu24
									#endif

									// @DOCLINE * `MU_WLEU24` - writes an unsigned 24-bit integer to little-endian byte data; overridable macro to `muu_wleu24`.
									#ifndef MU_WLEU24
										MUDEF inline void muu_wleu24(muByte* b, uint32_m n) {
											b[0] = (uint8_m)(n >> 0);
											b[1] = (uint8_m)(n >> 8);
											b[2] = (uint8_m)(n >> 16);
										}
										#define MU_WLEU24 muu_wleu24
									#endif

									// @DOCLINE * `MU_RLES24` - reads a signed 24-bit integer from little-endian byte data; overridable macro to `muu_rles24`.
									#ifndef MU_RLES24
										MUDEF inline int32_m muu_rles24(muByte* b) {
											uint32_m u24 = muu_rleu24(b);
											return *(int32_m*)&u24;
										}
										#define MU_RLES24 muu_rles24
									#endif

									// @DOCLINE * `MU_WLES24` - writes a signed 24-bit integer to little-endian byte data; overridable macro to `muu_wles24`.
									#ifndef MU_WLES24
										MUDEF inline void muu_wles24(muByte* b, int32_m n) {
											// Probably definitely doesn't work with signed integers; fix later
											uint32_m un = *(uint32_m*)&n;
											b[0] = (uint8_m)(un >> 0);
											b[1] = (uint8_m)(un >> 8);
											b[2] = (uint8_m)(un >> 16);
										}
										#define MU_WLES24 muu_wles24
									#endif

									// @DOCLINE * `MU_RBEU24` - reads an unsigned 24-bit integer from big-endian byte data; overridable macro to `muu_rbeu24`.
									#ifndef MU_RBEU24
										MUDEF inline uint32_m muu_rbeu24(muByte* b) {
											return (
												((uint32_m)(b[2]) << 0) |
												((uint32_m)(b[1]) << 8) |
												((uint32_m)(b[0]) << 16)
											);
										}
										#define MU_RBEU24 muu_rbeu24
									#endif

									// @DOCLINE * `MU_WBEU24` - writes an unsigned 24-bit integer to big-endian byte data; overridable macro to `muu_wbeu24`.
									#ifndef MU_WBEU24
										MUDEF inline void muu_wbeu24(muByte* b, uint32_m n) {
											b[2] = (uint8_m)(n >> 0);
											b[1] = (uint8_m)(n >> 8);
											b[0] = (uint8_m)(n >> 16);
										}
										#define MU_WBEU24 muu_wbeu24
									#endif

									// @DOCLINE * `MU_RBES24` - reads a signed 24-bit integer from big-endian byte data; overridable macro to `muu_rbes24`.
									#ifndef MU_RBES24
										MUDEF inline int32_m muu_rbes24(muByte* b) {
											uint32_m u24 = muu_rbeu24(b);
											return *(int32_m*)&u24;
										}
										#define MU_RBES24 muu_rbes24
									#endif

									// @DOCLINE * `MU_WBES24` - writes a signed 24-bit integer to big-endian byte data; overridable macro to `muu_wbes24`.
									#ifndef MU_WBES24
										MUDEF inline void muu_wbes24(muByte* b, int32_m n) {
											uint32_m un = *(uint32_m*)&n;
											b[2] = (uint8_m)(un >> 0);
											b[1] = (uint8_m)(un >> 8);
											b[0] = (uint8_m)(un >> 16);
										}
										#define MU_WBES24 muu_wbes24
									#endif

								// @DOCLINE ## 32-bit

									// @DOCLINE The following macros exist for byte manipulation regarding 32-bit integers:

									// @DOCLINE * `MU_RLEU32` - reads an unsigned 32-bit integer from little-endian byte data; overridable macro to `muu_rleu32`.
									#ifndef MU_RLEU32
										MUDEF inline uint32_m muu_rleu32(muByte* b) {
											return (
												((uint32_m)(b[0]) << 0)  |
												((uint32_m)(b[1]) << 8)  |
												((uint32_m)(b[2]) << 16) |
												((uint32_m)(b[3]) << 24)
											);
										}
										#define MU_RLEU32 muu_rleu32
									#endif

									// @DOCLINE * `MU_WLEU32` - writes an unsigned 32-bit integer to little-endian byte data; overridable macro to `muu_wleu32`.
									#ifndef MU_WLEU32
										MUDEF inline void muu_wleu32(muByte* b, uint32_m n) {
											b[0] = (uint8_m)(n >> 0);
											b[1] = (uint8_m)(n >> 8);
											b[2] = (uint8_m)(n >> 16);
											b[3] = (uint8_m)(n >> 24);
										}
										#define MU_WLEU32 muu_wleu32
									#endif

									// @DOCLINE * `MU_RLES32` - reads a signed 32-bit integer from little-endian byte data; overridable macro to `muu_rles32`.
									#ifndef MU_RLES32
										MUDEF inline int32_m muu_rles32(muByte* b) {
											uint32_m u32 = muu_rleu32(b);
											return *(int32_m*)&u32;
										}
										#define MU_RLES32 muu_rles32
									#endif

									// @DOCLINE * `MU_WLES32` - writes a signed 32-bit integer to little-endian byte data; overridable macro to `muu_wles32`.
									#ifndef MU_WLES32
										MUDEF inline void muu_wles32(muByte* b, int32_m n) {
											uint32_m un = *(uint32_m*)&n;
											b[0] = (uint8_m)(un >> 0);
											b[1] = (uint8_m)(un >> 8);
											b[2] = (uint8_m)(un >> 16);
											b[3] = (uint8_m)(un >> 24);
										}
										#define MU_WLES32 muu_wles32
									#endif

									// @DOCLINE * `MU_RBEU32` - reads an unsigned 32-bit integer from big-endian byte data; overridable macro to `muu_rbeu32`.
									#ifndef MU_RBEU32
										MUDEF inline uint32_m muu_rbeu32(muByte* b) {
											return (
												((uint32_m)(b[3]) << 0)  |
												((uint32_m)(b[2]) << 8)  |
												((uint32_m)(b[1]) << 16) |
												((uint32_m)(b[0]) << 24)
											);
										}
										#define MU_RBEU32 muu_rbeu32
									#endif

									// @DOCLINE * `MU_WBEU32` - writes an unsigned 32-bit integer to big-endian byte data; overridable macro to `muu_wbeu32`.
									#ifndef MU_WBEU32
										MUDEF inline void muu_wbeu32(muByte* b, uint32_m n) {
											b[3] = (uint8_m)(n >> 0);
											b[2] = (uint8_m)(n >> 8);
											b[1] = (uint8_m)(n >> 16);
											b[0] = (uint8_m)(n >> 24);
										}
										#define MU_WBEU32 muu_wbeu32
									#endif

									// @DOCLINE * `MU_RBES32` - reads a signed 32-bit integer from big-endian byte data; overridable macro to `muu_rbes32`.
									#ifndef MU_RBES32
										MUDEF inline int32_m muu_rbes32(muByte* b) {
											uint32_m u32 = muu_rbeu32(b);
											return *(int32_m*)&u32;
										}
										#define MU_RBES32 muu_rbes32
									#endif

									// @DOCLINE * `MU_WBES32` - writes a signed 32-bit integer to big-endian byte data; overridable macro to `muu_wbes32`.
									#ifndef MU_WBES32
										MUDEF inline void muu_wbes32(muByte* b, int32_m n) {
											uint32_m un = *(uint32_m*)&n;
											b[3] = (uint8_m)(un >> 0);
											b[2] = (uint8_m)(un >> 8);
											b[1] = (uint8_m)(un >> 16);
											b[0] = (uint8_m)(un >> 24);
										}
										#define MU_WBES32 muu_wbes32
									#endif

								// @DOCLINE ## 64-bit

									// @DOCLINE The following macros exist for byte manipulation regarding 64-bit integers:

									// @DOCLINE * `MU_RLEU64` - reads an unsigned 64-bit integer from little-endian byte data; overridable macro to `muu_rleu64`.
									#ifndef MU_RLEU64
										MUDEF inline uint64_m muu_rleu64(muByte* b) {
											return (
												((uint64_m)(b[0]) << 0)  |
												((uint64_m)(b[1]) << 8)  |
												((uint64_m)(b[2]) << 16) |
												((uint64_m)(b[3]) << 24) |
												((uint64_m)(b[4]) << 32) |
												((uint64_m)(b[5]) << 40) |
												((uint64_m)(b[6]) << 48) |
												((uint64_m)(b[7]) << 56)
											);
										}
										#define MU_RLEU64 muu_rleu64
									#endif

									// @DOCLINE * `MU_WLEU64` - writes an unsigned 64-bit integer to little-endian byte data; overridable macro to `muu_wleu64`.
									#ifndef MU_WLEU64
										MUDEF inline void muu_wleu64(muByte* b, uint64_m n) {
											b[0] = (uint8_m)(n >> 0);
											b[1] = (uint8_m)(n >> 8);
											b[2] = (uint8_m)(n >> 16);
											b[3] = (uint8_m)(n >> 24);
											b[4] = (uint8_m)(n >> 32);
											b[5] = (uint8_m)(n >> 40);
											b[6] = (uint8_m)(n >> 48);
											b[7] = (uint8_m)(n >> 56);
										}
										#define MU_WLEU64 muu_wleu64
									#endif

									// @DOCLINE * `MU_RLES64` - reads a signed 64-bit integer from little-endian byte data; overridable macro to `muu_rles64`.
									#ifndef MU_RLES64
										MUDEF inline int64_m muu_rles64(muByte* b) {
											uint64_m u64 = muu_rleu64(b);
											return *(int64_m*)&u64;
										}
										#define MU_RLES64 muu_rles64
									#endif

									// @DOCLINE * `MU_WLES64` - writes a signed 64-bit integer to little-endian byte data; overridable macro to `muu_wles64`.
									#ifndef MU_WLES64
										MUDEF inline void muu_wles64(muByte* b, int64_m n) {
											uint64_m un = *(uint64_m*)&n;
											b[0] = (uint8_m)(un >> 0);
											b[1] = (uint8_m)(un >> 8);
											b[2] = (uint8_m)(un >> 16);
											b[3] = (uint8_m)(un >> 24);
											b[4] = (uint8_m)(un >> 32);
											b[5] = (uint8_m)(un >> 40);
											b[6] = (uint8_m)(un >> 48);
											b[7] = (uint8_m)(un >> 56);
										}
										#define MU_WLES64 muu_wles64
									#endif

									// @DOCLINE * `MU_RBEU64` - reads an unsigned 64-bit integer from big-endian byte data; overridable macro to `muu_rbeu64`.
									#ifndef MU_RBEU64
										MUDEF inline uint64_m muu_rbeu64(muByte* b) {
											return (
												((uint64_m)(b[7]) << 0)  |
												((uint64_m)(b[6]) << 8)  |
												((uint64_m)(b[5]) << 16) |
												((uint64_m)(b[4]) << 24) |
												((uint64_m)(b[3]) << 32) |
												((uint64_m)(b[2]) << 40) |
												((uint64_m)(b[1]) << 48) |
												((uint64_m)(b[0]) << 56)
											);
										}
										#define MU_RBEU64 muu_rbeu64
									#endif

									// @DOCLINE * `MU_WBEU64` - writes an unsigned 64-bit integer to big-endian byte data; overridable macro to `muu_wbeu64`.
									#ifndef MU_WBEU64
										MUDEF inline void muu_wbeu64(muByte* b, uint64_m n) {
											b[7] = (uint8_m)(n >> 0);
											b[6] = (uint8_m)(n >> 8);
											b[5] = (uint8_m)(n >> 16);
											b[4] = (uint8_m)(n >> 24);
											b[3] = (uint8_m)(n >> 32);
											b[2] = (uint8_m)(n >> 40);
											b[1] = (uint8_m)(n >> 48);
											b[0] = (uint8_m)(n >> 56);
										}
										#define MU_WBEU64 muu_wbeu64
									#endif

									// @DOCLINE * `MU_RBES64` - reads a signed 64-bit integer from big-endian byte data; overridable macro to `muu_rbes64`.
									#ifndef MU_RBES64
										MUDEF inline int64_m muu_rbes64(muByte* b) {
											uint64_m u64 = muu_rbeu64(b);
											return *(int64_m*)&u64;
										}
										#define MU_RBES64 muu_rbes64
									#endif

									// @DOCLINE * `MU_WBES64` - writes a signed 64-bit integer to big-endian byte data; overridable macro to `muu_wbes64`.
									#ifndef MU_WBES64
										MUDEF inline void muu_wbes64(muByte* b, int64_m n) {
											uint64_m un = *(uint64_m*)&n;
											b[7] = (uint8_m)(un >> 0);
											b[6] = (uint8_m)(un >> 8);
											b[5] = (uint8_m)(un >> 16);
											b[4] = (uint8_m)(un >> 24);
											b[3] = (uint8_m)(un >> 32);
											b[2] = (uint8_m)(un >> 40);
											b[1] = (uint8_m)(un >> 48);
											b[0] = (uint8_m)(un >> 56);
										}
										#define MU_WBES64 muu_wbes64
									#endif

							// @DOCLINE # Set result

								/* @DOCBEGIN

								The `MU_SET_RESULT(res, val)` macro is an overridable function that checks if the given parameter `res` is a null pointer. If it is, it does nothing, but if it isn't, it dereferences the pointer and sets the value to `val`. This macro saves a lot of code, shrinking down what would be this:

								```c
								if (result) {
									*result = ...;
								}
								```

								into this:

								```c
								MU_SET_RESULT(result, ...)
								```

								@DOCEND */

								#ifndef MU_SET_RESULT
									#define MU_SET_RESULT(res, val) if(res){*res=val;}
								#endif

							// @DOCLINE # Enum

								/* @DOCBEGIN

								The `MU_ENUM(name, ...)` macro is an overridable function used to declare an enumerator. `name` is the name of the enumerator type, and `...` are all of the values. The reason why one would prefer this over the traditional way of declaring enumerators is because it actually makes it a `size_m`, which can avoid errors on certain compilers (looking at you, Microsoft) in regards to treating enumerators like values. It expands like this:

								```c
								enum _##name {
									__VA_ARGS__
								};
								typedef enum _##name _##name;
								typedef size_m name;
								```

								@DOCEND */

								#define MU_ENUM(name, ...) enum _##name{__VA_ARGS__};typedef enum _##name _##name;typedef size_m name;

							// @DOCLINE # Operating system recognition

								/* @DOCBEGIN

								The macros `MU_WIN32` or `MU_LINUX` are defined (if neither were defined before) in order to allow mu libraries to easily check if they're running on a Windows or Linux system.

								`MU_WIN32` will be defined if `WIN32` or `_WIN32` are defined, one of which is usually pre-defined on Windows systems.

								`MU_LINUX` will be defined if `__linux__` is defined.

								@DOCEND */

								#if !defined(MU_WIN32) && !defined(MU_LINUX)
									#if defined(WIN32) || defined(_WIN32)
										#define MU_WIN32
									#endif
									#if defined(__linux__)
										#define MU_LINUX
									#endif
								#endif

							MU_CPP_EXTERN_END

						#endif /* MUU_H */
					// @ATTENTION

					// @DOCLINE > Note that mu libraries store their dependencies within their files, so you don't need to import these dependencies yourself; this section is purely to give more information about the contents that this file defines. The libraries listed may also have other dependencies that they also include that aren't explicitly listed here.

					// Types elaborated later on
					typedef uint16_m muCOSAResult; // (65,536 error results including success)
					typedef uint16_m muKeyboardKey;
					typedef uint8_m muKeyboardState;
					typedef uint16_m muMouseKey;

					// @DOCLINE # Version

						// @DOCLINE The macros `MUCOSA_VERSION_MAJOR`, `MUCOSA_VERSION_MINOR`, and `MUCOSA_VERSION_PATCH` are defined to match its respective release version, following the formatting of `MAJOR.MINOR.PATCH`.

						#define MUCOSA_VERSION_MAJOR 2
						#define MUCOSA_VERSION_MINOR 0
						#define MUCOSA_VERSION_PATCH 0

					MU_CPP_EXTERN_START

					// @DOCLINE # Window systems

						// @DOCLINE The type `muWindowSystem` (typedef for `uint8_m`) is used to define all of the currently supported window systems. It has the following defined values:
						typedef uint8_m muWindowSystem;

						// @DOCLINE * `MU_WINDOW_NULL` - Unknown/Null window system; real value 0. This value can also act as an "auto" window system, such as when creating a muCOSA context with it, automatically picking the best currently available window system.
						#define MU_WINDOW_NULL 0

						// @DOCLINE * `MU_WINDOW_WIN32` - Win32; real value 1.
						#define MU_WINDOW_WIN32 1

						// @DOCLINE > Note that although on most operating systems, only one window system can exist (such as macOS or Windows), some operating systems can have more than one window system, such as Linux with X11 or Wayland. Just in case, muCOSA allows more than one window system to be defined at once in its API, tying each muCOSA context to a particular window system, theoretically allowing for multiple muCOSA contexts to exist at once with different window systems in one program.

						// @DOCLINE ## Window system names
						#ifdef MUCOSA_NAMES

						// @DOCLINE The name function `mu_window_system_get_name` returns a `const char*` representation of the given window sytem (for example, `MU_WINDOW_NULL` returns "MU_WINDOW_NULL"), defined below: @NLNT
						MUDEF const char* mu_window_system_get_name(muWindowSystem system);

						// @DOCLINE It will return "MU_UNKNOWN" in the case that `system` is an invalid window system value.

						// @DOCLINE The name function `mu_window_system_get_nice_name` does the same thing, but with a nicer and more readable `const char*` representation (for example, `MU_WINDOW_NULL` returns "Unknown/Auto"), defined below: @NLNT
						MUDEF const char* mu_window_system_get_nice_name(muWindowSystem system);

						// @DOCLINE It will return "Unknown" in the case that `system` is an invalid window system value.

						// @DOCLINE > These functions are "name" functions, and therefore are only defined if `MUCOSA_NAMES` is also defined by the user.

						#endif

						// @DOCLINE ## Manual window system support

						// @DOCLINE Not all window systems are supported upfront, so it is automatically guessed based on the current operating system that the code is compiling for using macros provided by muUtility.

						// @DOCLINE The explicit functionality is this:

						// @DOCLINE * `MUCOSA_WIN32` is defined if `MU_WIN32` is defined (automatically provided by muUtility), which toggles support for Win32.

						// @DOCLINE All of this functionality can be overrided by defining the macro `MUCOSA_MANUAL_OS_SUPPORT`, in which case, none of this is performed, and it is up to the user to manually define that operating systems are supported by defining their respective muCOSA macros (ie, if you're compiling on Windows and disabled automatic operating system recognition, you would need to define `MUCOSA_WIN32` yourself).

					// @DOCLINE # Global context

						// @DOCLINE muCOSA operates in a context, encapsulated by the type `muCOSAContext`, which has the following members:

						struct muCOSAContext {
							// @DOCLINE * `@NLFT* inner` - pointer to internally-used manually-allocated memory for running the current operating system context. The user should never interact with this member.
							void* inner;
							// @DOCLINE * `@NLFT result` - the result of the latest non-successful non-result-checking function call regarding the context; starting value upon context creation is `MUCOSA_SUCCESS`, and is set to another value if a function whose result is not set manually by the user doesn't return a success result value.
							muCOSAResult result;
						};
						typedef struct muCOSAContext muCOSAContext;

						// @DOCLINE ## Creation and destruction

						// @DOCLINE To create the context, the function `muCOSA_context_create` is used, defined below: @NLNT
						MUDEF void muCOSA_context_create(muCOSAContext* context, muWindowSystem system, muBool set_context);

						// @DOCLINE The result of this function is stored within `context->result`. Upon success, this function automatically calls `muCOSA_context_set` on the created context unless `set_context` is equal to `MU_FALSE`.

						// > It is valid for `system` to be `MU_WINDOW_NULL` for this function, in which case, the best currently available window system will be automatically chosen by muCOSA.

						// @DOCLINE For every successfully created context, it must be destroyed, which is done with the function `muCOSA_context_destroy`, defined below: @NLNT
						MUDEF void muCOSA_context_destroy(muCOSAContext* context);
						// @DOCLINE This function cannot fail if given a valid pointer to an active context (otherwise, a crash is likely), so no result value is ever indicated by this function via any means.

						// @DOCLINE ## Global context

						MUDEF muCOSAContext* muCOSA_global_context;

						// @DOCLINE muCOSA uses a global variable to reference the currently "set" context whenever a function is called that assumes a context (ie it doesn't take a parameter for context). This global variable can be changed to reference a certain context via the function `muCOSA_context_set`, defined below: @NLNT
						MUDEF void muCOSA_context_set(muCOSAContext* context);

						// @DOCLINE ## Get context information

						// @DOCLINE The only information that can be retrieved about an active context is its window system, using the function `muCOSA_context_get_window_system`, defined below: @NLNT
						MUDEF muWindowSystem muCOSA_context_get_window_system(muCOSAContext* context);

						// @DOCLINE ## Non-result/context checking functions

						/* @DOCBEGIN
						If a function takes a `muCOSAContext` and `muCOSAResult` parameter, there will likely be two defined macros for calling the function without explicitly passing these parameters, with the current global context being assumed for both parameters.

						Non-result-checking functions are functions that assume the `muCOSAContext` parameter to be the current global context, and assume that the `muCOSAResult` parameter to be the current global context's result member. These functions' parameters are simply the normal function's parameters but without the context or result paramter, instead being routed to the current global context. The name of these functions are simply the normal name but `muCOSA_...` being replaced with just `mu_...`.

						Result-checking functions are functions that also assume (and thus don't make you specify) the `muCOSAContext` parameter to be the current global context, but they still make you specify the `muCOSAResult` parameter, and the global context's result member goes unmodified. The name of these functions is the same as the non-result-checking functions, but with an underscore appended at the end.

						For example, if the function `muCOSA_window_do_something` existed with these parameters:

						```c
						MUDEF void muCOSA_window_do_something(muCOSAContext* context, muCOSAResult* result, int a, int b);
						```

						then the function `mu_window_do_something`, the "non-result-checking function", exists with these parameters:

						```c
						MUDEF void mu_window_do_something(int a, int b);
						// Calls muCOSA_window_do_something with the global context
						// and its respective result member.
						```

						and the function `mu_window_do_something_`, the "result-checking function", exists with these parameters:

						```c
						MUDEF void mu_window_do_something_(muCOSAResult* result, int a, int b);
						// Calls muCOSA_window_do_something with the global context
						// and the given result pointer.
						```

						> Note that, in reality, the non-result and result-checking functions aren't defined as actual independent functions, but instead, macros to the original function. More information about the type `muCOSAResult` can be found in the [result section](#result).

						@DOCEND */

					// @DOCLINE # The window

						// @DOCLINE muCOSA works in a desktop OS environment, and therefore, its functionality primarily centers around the window (respective type `muWindow`; macro for `void*`).
						#define muWindow void*

						// @DOCLINE The documentation of this library will use the term "surface" in regards to the window's rendering space, and is simply the part of the window that is rendered to, excluding the "frame extents", which is comprised of things like the side bars, title bar, close button, etc.

						// @DOCLINE ## Window information

							// @DOCLINE The struct `muWindowInfo` represents information about a window. It has the following members:

							typedef struct muPixelFormat muPixelFormat;
							typedef struct muWindowCallbacks muWindowCallbacks;
							struct muWindowInfo {
								// @DOCLINE * `@NLFT* title` - the title of the window shown to the user in most interfaces (primarily the title bar).
								char* title;
								// @DOCLINE * `@NLFT width` - the width of the window's surface, in pixels.
								uint32_m width;
								// @DOCLINE * `@NLFT height` - the height of the window's surface, in pixels.
								uint32_m height;
								// @DOCLINE * `@NLFT min_width` - the minimum width of the window's surface, in pixels; a value of 0 implies no minimum.
								uint32_m min_width;
								// @DOCLINE * `@NLFT min_height` - the minimum height of the window's surface, in pixels; a value of 0 implies no minimum.
								uint32_m min_height;
								// @DOCLINE * `@NLFT max_width` - the maximum width of the window's surface, in pixels; a value of 0 implies no maximum.
								uint32_m max_width;
								// @DOCLINE * `@NLFT max_height` - the maximum height of the window's surface, in pixels; a value of 0 implies no maximum.
								uint32_m max_height;
								// @DOCLINE * `@NLFT x` - the x-coordinate of the top-leftest pixel in the window's surface relative to the entire window space of the window system.
								int32_m x;
								// @DOCLINE * `@NLFT y` - the y-coordinate of the top-leftest pixel in the window's surface relative to the entire window space of the window system.
								int32_m y;
								// @DOCLINE * `@NLFT* pixel_format` - the pixel format for the window's surface. If the value of this member is equal to 0, no pixel format is specified, and a default compatible one is chosen. If the pixel format is specified, muCOSA attempts to choose it, and if unsupported, muCOSA will throw a non-fatal error and default on a compatible pixel format.
								muPixelFormat* pixel_format;
								// @DOCLINE * `@NLFT* callbacks` - the [callback functions](#window-callbacks) for various attributes of the window. If this member is equal to 0, no callbacks are specified. If this member is not equal to 0, it should be a valid pointer to a `muWindowCallbacks` struct specifying callbacks for the window.
								muWindowCallbacks* callbacks;
							};
							typedef struct muWindowInfo muWindowInfo;

							// @DOCLINE > Due to restrictions on certain operating systems, the minimum width that will work on all operating systems is 120 pixels, and the minimum height that will surely work is 1 pixel. Additionally, negative coordiantes may not function properly for a given window in regards to setting them to that value, and should not be relied upon for functionality.

						// @DOCLINE ## Window creation and destruction

							// @DOCLINE In order to create a window, the function `muCOSA_window_create` must be called, defined below: @NLNT
							MUDEF muWindow muCOSA_window_create(muCOSAContext* context, muCOSAResult* result, muWindowInfo* info);

							// @DOCLINE Upon failure (marked by the value of `result`), the creation function returns 0.

							// @DOCLINE > The macro `mu_window_create` is the non-result-checking equivalent, and the macro `mu_window_create_` is the result-checking equivalent.
							#define mu_window_create(...) muCOSA_window_create(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
							#define mu_window_create_(result, ...) muCOSA_window_create(muCOSA_global_context, result, __VA_ARGS__)

							// @DOCLINE Every successfully created window must be destroyed at some point during the relevant context's lifetime using the function `muCOSA_window_destroy`, defined below: @NLNT
							MUDEF muWindow muCOSA_window_destroy(muCOSAContext* context, muWindow win);

							// @DOCLINE The destruction function cannot fail if given a proper context and window, and thus, there is no `result` parameter.

							// @DOCLINE > The macro `mu_window_destroy` is the non-result-checking equivalent.
							#define mu_window_destroy(...) muCOSA_window_destroy(muCOSA_global_context, __VA_ARGS__)

						// @DOCLINE ## The main loop

							// @DOCLINE The "main loop" of a window consists of two functions that are called each "frame": `muCOSA_window_get_closed` and `muCOSA_window_update`. A main loop with these functions generally looks like this:

							/* @DOCBEGIN
							```c
							// Initialization / Window creation here...

							while (!mu_window_get_closed(window)) {

								// ... (This is where the frame-by-frame logic would go) ...

								mu_window_update(window);
							}

							// Termination here...
							```
							@DOCEND */

							// @DOCLINE OpenGL contexts slightly change this; an OpenGL context renders directly to the window's surface, meaning that swapping the buffers of a window needs to occur each frame with a valid OpenGL context binded. For example, if `window` had an OpenGL context created from it named `gl`, a main loop for it would look like this:

							/* @DOCBEGIN
							```c
							// Initialization / Window / OpenGL context creation here...

							// Bind OpenGL context (necessary for mu_gl_swap_buffers to work)
							mu_gl_bind(gl);
							
							while (!mu_window_get_closed(window)) {
								
								// ... (This is where the frame-by-frame logic would go) ...

								mu_gl_swap_buffers(window);
								mu_window_update(window);
							}

							```
							@DOCEND */

							// @DOCLINE More information is provided in the [OpenGL context](#opengl-context) section.

							// @DOCLINE ### Get closed

							// @DOCLINE The function `muCOSA_window_get_closed` returns whether or not a given window has been closed, defined below: @NLNT
							MUDEF muBool muCOSA_window_get_closed(muCOSAContext* context, muCOSAResult* result, muWindow win);

							// @DOCLINE Once this function returns `MU_FALSE`, it is no longer usable in all circumstances other than destroying it with `muCOSA_window_destroy`; a window cannot be revived once closed.

							// @DOCLINE > The macro `mu_window_get_closed` is the non-result-checking equivalent, and the macro `mu_window_get_closed_` is the result-checking equivalent.
							#define mu_window_get_closed(...) muCOSA_window_get_closed(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
							#define mu_window_get_closed_(result, ...) muCOSA_window_get_closed(muCOSA_global_context, result, __VA_ARGS__)

							// @DOCLINE ### Close window

							// @DOCLINE The function `muCOSA_window_close` closes a given window, defined below: @NLNT
							MUDEF void muCOSA_window_close(muCOSAContext* context, muWindow win);

							// @DOCLINE This function cannot if given a valid unclosed window and a valid context corresponding to the window, and thus, has no result parameter.

							// @DOCLINE > The macro `mu_window_close` is the non-result-checking equivalent.
							#define mu_window_close(...) muCOSA_window_close(muCOSA_global_context, __VA_ARGS__)

							// @DOCLINE ### Update

							// @DOCLINE The function `muCOSA_window_update` updates/refreshes a window and triggers all relevant callbacks, presenting the contents of the surface, defined below: @NLNT
							MUDEF void muCOSA_window_update(muCOSAContext* context, muCOSAResult* result, muWindow win);

							// @DOCLINE > The macro `mu_window_update` is the non-result-checking equivalent, and the macro `mu_window_update_` is the result-checking equivalent.
							#define mu_window_update(...) muCOSA_window_update(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
							#define mu_window_update_(result, ...) muCOSA_window_update(muCOSA_global_context, result, __VA_ARGS__)

						// @DOCLINE ## Window attributes

							typedef uint16_m muWindowAttrib;
							// @DOCLINE The window is described by several attributes, with each attribute represented by the type `muWindowAttrib` (typedef for `uint16_m`). It has the following values:

							// @DOCLINE * `MU_WINDOW_TITLE` - the title of the window, represented by a `char*` UTF-8 string. This cannot be "get", but can be "set".
							#define MU_WINDOW_TITLE 0

							// @DOCLINE * `MU_WINDOW_DIMENSIONS` - the width and height of the window's surface, in pixels, represented by an array of two `uint32_m`s, where the first element is the width, and the second element is the height. This can be "get" and "set".
							#define MU_WINDOW_DIMENSIONS 1

							// @DOCLINE * `MU_WINDOW_POSITION` - the x- and y-coordinates of the top-leftest pixel of the window's surface relative to the entire window space of the window system, represented by an array of two `int32_m`s, where the first element is the x-coordinate, and the second element is the y-coordinate. This can be "get" and "set".
							#define MU_WINDOW_POSITION 2

							// @DOCLINE * `MU_WINDOW_KEYBOARD_MAP` - the [keyboard keymap](#keyboard-keymap), represented by a pointer to an array of booleans (type `muBool`) representing the state of each readable keyboard key. This can be "get", but not "set".
							#define MU_WINDOW_KEYBOARD_MAP 3
							// @DOCLINE    > Note that when being read, the data is not expected to be the actual array, but instead a pointer that will be set to the internally-used keymap array, which remains consistent for an entire window's lifespan. More information about the keyboard keymap can be found in the [keymap](#keymaps) section.

							// @DOCLINE * `MU_WINDOW_KEYSTATE_MAP` - the [keystate keymap](#keystate-keymap), represented by a pointer to an array of booleans (type `muBool`) representing the state of all keyboard states (such as caps lock, for example). This can be "get" but not "set".
							#define MU_WINDOW_KEYSTATE_MAP 4

							// @DOCLINE * `MU_WINDOW_MOUSE_MAP` - the [mouse keymap](#mouse-keymap), represented by a pointer to an array of booleans (type `muBool`) representing the state of each readable mouse key. This can be "get", but not "set".
							#define MU_WINDOW_MOUSE_MAP 5

							// @DOCLINE * `MU_WINDOW_SCROLL_LEVEL` - the scroll level of the cursor associated with the window, represented by a single `int32_m` value representing how far it is scrolled up (positive) or down (negative). This can be "get" and "set".
							#define MU_WINDOW_SCROLL_LEVEL 6
							// @DOCLINE > One full scroll up/down on a mouse wheel is worth 120 units.

							// @DOCLINE * `MU_WINDOW_CURSOR` - the x- and y-coordinates of the visual cursor relative to the position of the window's surface, represented by an array of two `int32_m`s, where the first element is the x-coordinate, and the second element is the y-coordinate. This can be "get" and "set".
							#define MU_WINDOW_CURSOR 7

							// @DOCLINE * `MU_WINDOW_CURSOR_STYLE` - the [style of the cursor](#cursor-style), represented by a single value `muCursorStyle`. This can be "get" and "set".
							#define MU_WINDOW_CURSOR_STYLE 8

							// @DOCLINE * `MU_WINDOW_..._CALLBACK` - the callback attributes. These all cannot be "get", but can be "set".
							// @DOCLINE    * `MU_WINDOW_DIMENSIONS_CALLBACK` - the dimensions callback.
							#define MU_WINDOW_DIMENSIONS_CALLBACK 9
							// @DOCLINE    * `MU_WINDOW_POSITION_CALLBACK` - the position callback.
							#define MU_WINDOW_POSITION_CALLBACK 10
							// @DOCLINE    * `MU_WINDOW_KEYBOARD_CALLBACK` - the keyboard callback.
							#define MU_WINDOW_KEYBOARD_CALLBACK 11
							// @DOCLINE    * `MU_WINDOW_KEYSTATE_CALLBACK` - the keystate callback.
							#define MU_WINDOW_KEYSTATE_CALLBACK 12
							// @DOCLINE    * `MU_WINDOW_MOUSE_KEY_CALLBACK` - the mouse key callback.
							#define MU_WINDOW_MOUSE_KEY_CALLBACK 13
							// @DOCLINE    * `MU_WINDOW_CURSOR_CALLBACK` - the cursor callback.
							#define MU_WINDOW_CURSOR_CALLBACK 14
							// @DOCLINE    * `MU_WINDOW_SCROLL_CALLBACK` - the scroll callback.
							#define MU_WINDOW_SCROLL_CALLBACK 15
							// @DOCLINE    > When callbacks are being set via `muCOSA_window_set`, note that they are pointers *to* the function pointers; function "`fun`" would be set via `muCOSA_window_set(..., &fun)`. For more information about the callbacks, see the (callbacks section)(#window-callbacks). The types listed in the `muWindowCallbacks` struct match the types expected for the callback window attributes.

							// @DOCLINE A value is "get" if calling `muCOSA_window_get` with it is valid, and a value is "set" if calling `muCOSA_window_set` with it is valid.

							// @DOCLINE ### Names
							#ifdef MUCOSA_NAMES

							// @DOCLINE The name function for `muWindowAttrib` is `mu_window_attrib_get_name`, defined below: @NLNT
							MUDEF const char* mu_window_attrib_get_name(muWindowAttrib attrib);

							// @DOCLINE > This function returns "MU_UNKNOWN" if the value of `attrib` is unrecognized.

							// @DOCLINE The nice name function for `muWindowAttrib` is `mu_window_attrib_get_nice_name`, defined below: @NLNT
							MUDEF const char* mu_window_attrib_get_nice_name(muWindowAttrib attrib);

							// @DOCLINE > This function returns "Unknown" if the value of `attrib` is unrecognized.
							#endif

							// @DOCLINE ### Get and set window attributes

							// @DOCLINE The function `muCOSA_window_get` retrieves an attribute of a window, defined below: @NLNT
							MUDEF void muCOSA_window_get(muCOSAContext* context, muCOSAResult* result, muWindow win, muWindowAttrib attrib, void* data);

							// @DOCLINE > The macro `mu_window_get` is the non-result-checking equivalent, and the macro `mu_window_get_` is the result-checking equivalent.
							#define mu_window_get(...) muCOSA_window_get(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
							#define mu_window_get_(result, ...) muCOSA_window_get(muCOSA_global_context, result, __VA_ARGS__)

							// @DOCLINE The function `muCOSA_window_set` modifies an attribute of a window, defined below: @NLNT
							MUDEF void muCOSA_window_set(muCOSAContext* context, muCOSAResult* result, muWindow win, muWindowAttrib attrib, void* data);

							// @DOCLINE > The macro `mu_window_set` is the non-result-checking equivalent, and the macro `mu_window_set_` is the result-checking equivalent.
							#define mu_window_set(...) muCOSA_window_set(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
							#define mu_window_set_(result, ...) muCOSA_window_set(muCOSA_global_context, result, __VA_ARGS__)

							// @DOCLINE For both functions, `data` is a pointer to data dictated by the value of `attrib`. In the case of `muCOOSA_window_get`, the data is derefenced and filled in corresponding to the window's requested attribute (if successful); in the case of `muCOSA_window_set`, the data is dereferenced and read, and the requested window attribute is changed to the given value(s) (if successful).

							// @DOCLINE > `mu_window_set` will only read from `data` and never modify it. Likewise, `mu_window_get` will only dereference `data` and never read from it.

						// @DOCLINE ## Window callbacks

							// @DOCLINE A window callback is a function that is called whenever the window registers that a certain attribute has changed. Every callback function is called while the window is being updated via the function `muCOSA_window_update`.

							// @DOCLINE Window callbacks are specified in the struct `muWindowCallbacks`, which has the following members:

							struct muWindowCallbacks {
								// @DOCLINE * `void (*dimensions)` - the dimensions callback, called every time that the window's dimensions are modified, defined below: @NLNT
								void (*dimensions)(muWindow win, uint32_m width, uint32_m height);
								// @DOCLINE * `void (*position)` - the position callback, called every time that the window's position is modified, defined below: @NLNT
								void (*position)(muWindow win, int32_m x, int32_m y);
								// @DOCLINE * `void (*keyboard)` - the keyboard callback, called every time that the status of a keyboard key on the [keyboard keymap](#keyboard-keymap) changes, defined below: @NLNT
								void (*keyboard)(muWindow win, muKeyboardKey key, muBool status);
								// @DOCLINE * `void (*keystate)` - the keystate callback, called every time that the status of a keystate on the [keystate keymap](#keystate-keymap) changes, defined below: @NLNT
								void (*keystate)(muWindow win, muKeyboardState state, muBool status);
								// @DOCLINE * `void (*mouse_key)` - the mouse key callback, called every time that the status of a mouse key on the [mouse keymap](#mouse-keymap) changes, defined below: @NLNT
								void (*mouse_key)(muWindow win, muMouseKey key, muBool status);
								// @DOCLINE * `void (*cursor)` - the cursor position callback, called every time that the cursor position changes, defined below: @NLNT
								void (*cursor)(muWindow win, int32_m x, int32_m y);
								// @DOCLINE * `void (*scroll)` - the scroll callback, called every time that the scroll level changes, defined below: @NLNT
								void (*scroll)(muWindow win, int32_m add);
							};

							// @DOCLINE Setting the value of any member to 0 dictates no callback function. Changes to an attribute made via the program (such as a `mu_window_set` call) are not guaranteed to generate corresponding callbacks.

							// @DOCLINE Initial callbacks, AKA callbacks involving the window's attributes being set upon creation, are not guaranteed to be called. Additionally, duplicate callbacks (ie callbacks issuing the same value, such a keyboard callback issuing a key status that already matched the previous key status, AKA a key being pressed/released twice) can occur; in fact, duplicate key presses are used to represent a key being held down for a long time, and the rate of repetition varies between window systems and their settings.

							// @DOCLINE Callbacks involving queryable attributes of a window are called after their attribute has been updated. For example, if a keyboard callback is triggered, the keyboard keymap for the corresponding window has already been updated for the key in question.

							// @DOCLINE Users should also avoid possible callback loops, such as a position callback that changes the position, which can theoretically trigger an infinite loop and cause a nasty crash.

						// @DOCLINE ## Keymaps

							// @DOCLINE In order to make input require as minimal overhead as possible, muCOSA allows the user to read key input using "keymaps". A keymap is an array of booleans (type `muBool`) that dictate the state of each key. Therefore, if a user wanted to check a particular key's state, they would retrieve the keymap, and index into it based on what key they want to check. This array is stored internally somewhere in the API, and, when retrieved (via a "get" function call), a pointer to this array is given. Since the keymap is stored as a pointer to inner memory used by muCOSA, it is automatically updated every call to `muCOSA_window_update`.

							// @DOCLINE The big advantage of keymaps is that the array remains at the same point in memory for the entire window's lifespan; a user could grab the pointer to the keymap once at the beginning of the program, and instantly know the state of any key via indexing into it, without any need for refreshing or extra function calls as long as the window remains alive.

							// @DOCLINE > Note that when the keymap is retrieved, a *pointer* to it is retrieved, not the array itself. This means that when calling a "get" function with a keymap, a pointer to a pointer should be given, to which muCOSA will dereference the initial pointer and set the pointer to the address of the keymap array.

							// @DOCLINE > Note that keymaps are only meant to be read, not modified. Changing any value within a keymap array will result in undefined behavior.

							// @DOCLINE ### Keyboard keymap

							// @DOCLINE The keyboard keymap represents keys on the keyboard readable by muCOSA, using type `muKeyboardKey` (typedef for `uint16_m`) as index. The length of the keymap is `MU_KEYBOARD_LENGTH`. It has the following indexes:

							// @DOCLINE * `MU_KEYBOARD_UNKNOWN` - unknown key.
							#define MU_KEYBOARD_UNKNOWN 0
							// @DOCLINE * `MU_KEYBOARD_BACKSPACE` - the [backspace key](https://en.wikipedia.org/wiki/Backspace).
							#define MU_KEYBOARD_BACKSPACE 1
							// @DOCLINE * `MU_KEYBOARD_TAB` - the [tab key](https://en.wikipedia.org/wiki/Tab_key).
							#define MU_KEYBOARD_TAB 2
							// @DOCLINE * `MU_KEYBOARD_CLEAR` - the [clear key](https://en.wikipedia.org/wiki/Clear_key).
							#define MU_KEYBOARD_CLEAR 3
							// @DOCLINE * `MU_KEYBOARD_RETURN` - the [return key](https://en.wikipedia.org/wiki/Return_key).
							#define MU_KEYBOARD_RETURN 4
							// @DOCLINE * `MU_KEYBOARD_PAUSE` - the [pause key](https://en.wikipedia.org/wiki/Pause_key).
							#define MU_KEYBOARD_PAUSE 5
							// @DOCLINE * `MU_KEYBOARD_ESCAPE` - the [escape key](https://en.wikipedia.org/wiki/Escape_key).
							#define MU_KEYBOARD_ESCAPE 6
							// @DOCLINE * `MU_KEYBOARD_MODECHANGE` - the [modechange key](https://en.wikipedia.org/wiki/Language_input_keys).
							#define MU_KEYBOARD_MODECHANGE 7
							// @DOCLINE * `MU_KEYBOARD_SPACE` - the [space key](https://en.wikipedia.org/wiki/Space_bar).
							#define MU_KEYBOARD_SPACE 8
							// @DOCLINE * `MU_KEYBOARD_PRIOR` - the [page up key](https://en.wikipedia.org/wiki/Page_Up_and_Page_Down_keys).
							#define MU_KEYBOARD_PRIOR 9
							// @DOCLINE * `MU_KEYBOARD_NEXT` - the [page down key](https://en.wikipedia.org/wiki/Page_Up_and_Page_Down_keys).
							#define MU_KEYBOARD_NEXT 10
							// @DOCLINE * `MU_KEYBOARD_END` - the [end key](https://en.wikipedia.org/wiki/End_key).
							#define MU_KEYBOARD_END 11
							// @DOCLINE * `MU_KEYBOARD_HOME` - the [home key](https://en.wikipedia.org/wiki/Home_key).
							#define MU_KEYBOARD_HOME 12
							// @DOCLINE * `MU_KEYBOARD_(LEFT/UP/RIGHT/DOWN)` - the left, up, right, and down (arrow keys)[https://en.wikipedia.org/wiki/Arrow_keys].
							#define MU_KEYBOARD_LEFT 13
							#define MU_KEYBOARD_UP 14
							#define MU_KEYBOARD_RIGHT 15
							#define MU_KEYBOARD_DOWN 16
							// @DOCLINE * `MU_KEYBOARD_SELECT` - the [select key](https://stackoverflow.com/questions/23995537/what-is-the-select-key).
							#define MU_KEYBOARD_SELECT 17
							// @DOCLINE * `MU_KEYBOARD_PRINT` - the [print key](https://en.wikipedia.org/wiki/Print_Screen).
							#define MU_KEYBOARD_PRINT 18
							// @DOCLINE * `MU_KEYBOARD_EXECUTE` - the execute key.
							#define MU_KEYBOARD_EXECUTE 19
							// @DOCLINE * `MU_KEYBOARD_INSERT` - the [insert key](https://en.wikipedia.org/wiki/Insert_key).
							#define MU_KEYBOARD_INSERT 20
							// @DOCLINE * `MU_KEYBOARD_DELETE` - the [delete key](https://en.wikipedia.org/wiki/Delete_key).
							#define MU_KEYBOARD_DELETE 21
							// @DOCLINE * `MU_KEYBOARD_HELP` - the [help key](https://en.wikipedia.org/wiki/Help_key).
							#define MU_KEYBOARD_HELP 22
							// @DOCLINE * `MU_KEYBOARD_(0...9)` - [the number keys](https://en.wikipedia.org/wiki/Numerical_digit) (0-9).
							#define MU_KEYBOARD_0 23
							#define MU_KEYBOARD_1 24
							#define MU_KEYBOARD_2 25
							#define MU_KEYBOARD_3 26
							#define MU_KEYBOARD_4 27
							#define MU_KEYBOARD_5 28
							#define MU_KEYBOARD_6 29
							#define MU_KEYBOARD_7 30
							#define MU_KEYBOARD_8 31
							#define MU_KEYBOARD_9 32
							// @DOCLINE * `MU_KEYBOARD_(A...Z)` - the [alphabet keys](https://en.wikipedia.org/wiki/Keyboard_layout#Character_keys) (A-Z).
							#define MU_KEYBOARD_A 33
							#define MU_KEYBOARD_B 34
							#define MU_KEYBOARD_C 35
							#define MU_KEYBOARD_D 36
							#define MU_KEYBOARD_E 37
							#define MU_KEYBOARD_F 38
							#define MU_KEYBOARD_G 39
							#define MU_KEYBOARD_H 40
							#define MU_KEYBOARD_I 41
							#define MU_KEYBOARD_J 42
							#define MU_KEYBOARD_K 43
							#define MU_KEYBOARD_L 44
							#define MU_KEYBOARD_M 45
							#define MU_KEYBOARD_N 46
							#define MU_KEYBOARD_O 47
							#define MU_KEYBOARD_P 48
							#define MU_KEYBOARD_Q 49
							#define MU_KEYBOARD_R 50
							#define MU_KEYBOARD_S 51
							#define MU_KEYBOARD_T 52
							#define MU_KEYBOARD_U 53
							#define MU_KEYBOARD_V 54
							#define MU_KEYBOARD_W 55
							#define MU_KEYBOARD_X 56
							#define MU_KEYBOARD_Y 57
							#define MU_KEYBOARD_Z 58
							// @DOCLINE * `MU_KEYBOARD_(LEFT/RIGHT)_WINDOWS` - the left and right [Windows](https://en.wikipedia.org/wiki/Windows_key)/[super](https://en.wikipedia.org/wiki/Super_key_(keyboard_button))/[command](https://en.wikipedia.org/wiki/Command_key) keys.
							#define MU_KEYBOARD_LEFT_WINDOWS 59
							#define MU_KEYBOARD_RIGHT_WINDOWS 60
							// @DOCLINE * `MU_KEYBOARD_NUMPAD_(0...9)` - the [numpad number keys](https://en.wikipedia.org/wiki/Numeric_keypad) (0-9).
							#define MU_KEYBOARD_NUMPAD_0 61
							#define MU_KEYBOARD_NUMPAD_1 62
							#define MU_KEYBOARD_NUMPAD_2 63
							#define MU_KEYBOARD_NUMPAD_3 64
							#define MU_KEYBOARD_NUMPAD_4 65
							#define MU_KEYBOARD_NUMPAD_5 66
							#define MU_KEYBOARD_NUMPAD_6 67
							#define MU_KEYBOARD_NUMPAD_7 68
							#define MU_KEYBOARD_NUMPAD_8 69
							#define MU_KEYBOARD_NUMPAD_9 70
							// @DOCLINE * `MU_KEYBOARD_(ADD/SUBTRACT/MULTIPLY/DIVIDE)` - the addition, subtraction, multiplication, and division [numpad keys](https://en.wikipedia.org/wiki/Numeric_keypad).
							#define MU_KEYBOARD_ADD      71
							#define MU_KEYBOARD_SUBTRACT 72
							#define MU_KEYBOARD_MULTIPLY 73
							#define MU_KEYBOARD_DIVIDE   74
							// @DOCLINE * `MU_KEYBOARD_SEPARATOR` - the [separator key](https://stackoverflow.com/questions/67916923/what-physical-key-maps-to-keycode-108-vk-separator).
							#define MU_KEYBOARD_SEPARATOR 75
							// @DOCLINE * `MU_KEYBOARD_DECIMAL` - the [decimal/period/dot key](https://en.wikipedia.org/wiki/Numeric_keypad).
							#define MU_KEYBOARD_DECIMAL 76
							// @DOCLINE * `MU_KEYBOARD_F(1...24)` - the [function keys](https://en.wikipedia.org/wiki/Function_key) (1-24).
							#define MU_KEYBOARD_F1  77
							#define MU_KEYBOARD_F2  78
							#define MU_KEYBOARD_F3  79
							#define MU_KEYBOARD_F4  80
							#define MU_KEYBOARD_F5  81
							#define MU_KEYBOARD_F6  82
							#define MU_KEYBOARD_F7  83
							#define MU_KEYBOARD_F8  84
							#define MU_KEYBOARD_F9  85
							#define MU_KEYBOARD_F10 86
							#define MU_KEYBOARD_F11 87
							#define MU_KEYBOARD_F12 88
							#define MU_KEYBOARD_F13 89
							#define MU_KEYBOARD_F14 90
							#define MU_KEYBOARD_F15 91
							#define MU_KEYBOARD_F16 92
							#define MU_KEYBOARD_F17 93
							#define MU_KEYBOARD_F18 94
							#define MU_KEYBOARD_F19 95
							#define MU_KEYBOARD_F20 96
							#define MU_KEYBOARD_F21 97
							#define MU_KEYBOARD_F22 98
							#define MU_KEYBOARD_F23 99
							#define MU_KEYBOARD_F24 100
							// @DOCLINE * `MU_KEYBOARD_NUMLOCK` - the [Num Lock key](https://en.wikipedia.org/wiki/Num_Lock).
							#define MU_KEYBOARD_NUMLOCK 101
							// @DOCLINE * `MU_KEYBOARD_SCROLL` - the [Scroll Lock key](https://en.wikipedia.org/wiki/Scroll_Lock).
							#define MU_KEYBOARD_SCROLL 102
							// @DOCLINE * `MU_KEYBOARD_(LEFT/RIGHT)_SHIFT` - the left/right [shift keys](https://en.wikipedia.org/wiki/Shift_key).
							#define MU_KEYBOARD_LEFT_SHIFT 103
							#define MU_KEYBOARD_RIGHT_SHIFT 104
							// @DOCLINE * `MU_KEYBOARD_(LEFT/RIGHT)_CONTROL` - the left/right [control keys](https://en.wikipedia.org/wiki/Control_key).
							#define MU_KEYBOARD_LEFT_CONTROL 105
							#define MU_KEYBOARD_RIGHT_CONTROL 106
							// @DOCLINE * `MU_KEYBOARD_(LEFT/RIGHT)_MENU` - the left/right [menu keys](https://en.wikipedia.org/wiki/Menu_key).
							#define MU_KEYBOARD_LEFT_MENU 107
							#define MU_KEYBOARD_RIGHT_MENU 108
							// @DOCLINE * `MU_KEYBOARD_ATTN` - the [ATTN key](https://www.ibm.com/support/pages/apar/II04992).
							#define MU_KEYBOARD_ATTN 109
							// @DOCLINE * `MU_KEYBOARD_CRSEL` - the [CRSEL key](https://learn.microsoft.com/en-us/dotnet/api/system.windows.forms.keys?view=windowsdesktop-8.0).
							#define MU_KEYBOARD_CRSEL 110
							// @DOCLINE * `MU_KEYBOARD_EXSEL` - the [EXSEL key](https://learn.microsoft.com/en-us/dotnet/api/system.windows.forms.keys?view=windowsdesktop-8.0).
							#define MU_KEYBOARD_EXSEL 111
							// @DOCLINE * `MU_KEYBOARD_EREOF` - the [EREOF key](https://www.ibm.com/docs/en/wsfz-and-o/1.1?topic=descriptions-ereof-erase-end-field-key-statement).
							#define MU_KEYBOARD_EREOF 112
							// @DOCLINE * `MU_KEYBOARD_PLAY` - the play key.
							#define MU_KEYBOARD_PLAY 113
							// @DOCLINE * `MU_KEYBOARD_PA1` - the [PA1 key](https://www.ibm.com/docs/en/zos-basic-skills?topic=ispf-keyboard-keys-functions).
							#define MU_KEYBOARD_PA1 114

							#define MU_KEYBOARD_LENGTH 115

							// @DOCLINE Once the pointer to the keyboard keymap array has been retrieved via `muCOSA_window_get`, these values can be used as indexes to see the status of any keyboard key, in which `MU_TRUE` indicates that the key is being pressed down, and `MU_FALSE` indicates that the key is released.

							#ifdef MUCOSA_NAMES
							// @DOCLINE #### Keyboard names

							// @DOCLINE The name function `mu_keyboard_key_get_name` returns a `const char*` representation of the given keyboard key (for example, `MU_KEYBOARD_W` returns "MU_KEYBOARD_W"), defined below: @NLNT
							MUDEF const char* mu_keyboard_key_get_name(muKeyboardKey key);
							// @DOCLINE It will return "MU_UNKNOWN" in the case that `key` is an invalid keyboard key value.

							// @DOCLINE The name function `mu_keyboard_key_get_nice_name` does the same thing, but with a nicer and more readable `const char*` representation (for example, `MU_KEYBOARD_W` returns "W"), defined below: @NLNT
							MUDEF const char* mu_keyboard_key_get_nice_name(muKeyboardKey key);
							// @DOCLINE It will return "Unknown" in the case that `key` is an invalid keyboard key value.

							// @DOCLINE > These functions are "name" functions, and therefore are only defined if `MUCOSA_NAMES` is also defined by the user.

							#endif

							// @DOCLINE ### Keystate keymap

							// @DOCLINE The keystate keymap represents the state of certain modifiers on the keyboard readable by muCOSA, using type `muKeyboardState` (typedef for `uint8_m`) as index. The length of the keymap is `MU_KEYSTATE_LENGTH`. It has the following indexes:

							// @DOCLINE * `MU_KEYSTATE_UNKNOWN` - unknown keystate.
							#define MU_KEYSTATE_UNKNOWN 0
							// @DOCLINE * `MU_KEYSTATE_CAPS_LOCK` - the [caps lock](https://en.wikipedia.org/wiki/Caps_Lock) state.
							#define MU_KEYSTATE_CAPS_LOCK 1
							// @DOCLINE * `MU_KEYSTATE_SCROLL_LOCK` - the [scroll lock](https://en.wikipedia.org/wiki/Scroll_Lock) state.
							#define MU_KEYSTATE_SCROLL_LOCK 2
							// @DOCLINE * `MU_KEYSTATE_NUM_LOCK` - the [num lock](https://en.wikipedia.org/wiki/Num_Lock) state.
							#define MU_KEYSTATE_NUM_LOCK 3

							#define MU_KEYSTATE_LENGTH 4

							// @DOCLINE Once the pointer to the keystate keymap array has been retrieved via `muCOSA_window_get`, these values can be used as indexes to see the status of any mouse key, in which `MU_TRUE` indicates that the keystate is active, and `MU_FALSE` indicates that the keystate is inactive.

							#ifdef MUCOSA_NAMES
							// @DOCLINE #### Keystate names

							// @DOCLINE The name function `mu_keystate_get_name` returns a `const char*` representation of the given keystate (for example, `MU_KEYSTATE_CAPS_LOCK` returns "MU_KEYSTATE_CAPS_LOCK"), defined below: @NLNT
							MUDEF const char* mu_keystate_get_name(muKeyboardState state);
							// @DOCLINE It will return "MU_UNKNOWN" in the case that `state` is an invalid keystate value.

							// @DOCLINE The name function `mu_keystate_get_nice_name` does the same thing, but with a nicer and more readable `const char*` representation (for example, `MU_KEYSTATE_CAPS_LOCK` returns "Caps Lock"), defined below: @NLNT
							MUDEF const char* mu_keystate_get_nice_name(muKeyboardState state);
							// @DOCLINE It will return "Unknown" in the case that `state` is an invalid keystate value.

							// @DOCLINE > These functions are "name" functions, and therefore are only defined if `MUCOSA_NAMES` is also defined by the user.

							#endif

							// @DOCLINE ### Mouse keymap

							// @DOCLINE The mouse keymap represents the keys on a common computer mouse readable by muCOSA, using type `muMouseKey` (typedef for `uint16_m`) as index. The length of the keymap is `MU_MOUSE_LENGTH`. It has the following indexes:

							// @DOCLINE * `MU_MOUSE_UNKNOWN` - unknown mouse key.
							#define MU_MOUSE_UNKNOWN 0
							// @DOCLINE * `MU_MOUSE_LEFT` - the left mouse key.
							#define MU_MOUSE_LEFT 1
							// @DOCLINE * `MU_MOUSE_RIGHT` - the right mouse key.
							#define MU_MOUSE_RIGHT 2
							// @DOCLINE * `MU_MOUSE_MIDDLE` - the middle mouse key; this indicates whether or not the middle mouse key (usually the scroll wheel) is being clicked, not scrolled.
							#define MU_MOUSE_MIDDLE 3

							#define MU_MOUSE_LENGTH 4

							// @DOCLINE Once the pointer to the mouse keymap array has been retrieved via `muCOSA_window_get`, these values can be used as indexes to see the status of any mouse key, in which `MU_TRUE` indicates that the key is being pressed down, and `MU_FALSE` indicates that the key is released.

							#ifdef MUCOSA_NAMES
							// @DOCLINE #### Mouse key names

							// @DOCLINE The name function `mu_mouse_key_get_name` returns a `const char*` representation of the given mouse key (for example, `MU_MOUSE_LEFT` returns "MU_MOUSE_LEFT"), defined below: @NLNT
							MUDEF const char* mu_mouse_key_get_name(muMouseKey key);
							// @DOCLINE It will return "MU_UNKNOWN" in the case that `key` is an invalid mouse key value.

							// @DOCLINE The name function `mu_mouse_key_get_nice_name` does the same thing, but with a nicer and more readable `const char*` representation (for example, `MU_MOUSE_LEFT` returns "Left"), defined below: @NLNT
							MUDEF const char* mu_mouse_key_get_nice_name(muMouseKey key);
							// @DOCLINE It will return "Unknown" in the case that `key` is an invalid mouse key value.

							// @DOCLINE > These functions are "name" functions, and therefore are only defined if `MUCOSA_NAMES` is also defined by the user.

							#endif

						// @DOCLINE ## Cursor style

							typedef uint16_m muCursorStyle;

							// @DOCLINE The style of a cursor determines how it visually appears based on a number of presets for its look that the window system provides. Its type is `muCursorStyle` (typedef for `uint16_m`), and has the following values:

							// @DOCLINE * `MU_CURSOR_UNKNOWN` - unknown cursor style.
							#define MU_CURSOR_UNKNOWN 0
							// @DOCLINE * `MU_CURSOR_ARROW` - the normal arrow-looking cursor style; equivalent to `IDC_ARROW` in Win32.
							#define MU_CURSOR_ARROW 1
							// @DOCLINE * `MU_CURSOR_IBEAM` - the text-select cursor style, appearing as a vertical beam; equivalent to `IDC_IBEAM` in Win32.
							#define MU_CURSOR_IBEAM 2
							// @DOCLINE * `MU_CURSOR_WAIT` - the waiting/busy/loading cursor style; equivalent to `IDC_WAIT` in Win32.
							#define MU_CURSOR_WAIT 3
							// @DOCLINE * `MU_CURSOR_WAIT_ARROW` - the waiting/busy/loading cursor style, but also with the normal arrow visible; equivalent to `IDC_APPSTARTING` in Win32.
							#define MU_CURSOR_WAIT_ARROW 4
							// @DOCLINE * `MU_CURSOR_CROSSHAIR` - the crosshair cursor style; equivalent to `IDC_CROSS` in Win32.
							#define MU_CURSOR_CROSSHAIR 5
							// @DOCLINE * `MU_CURSOR_HAND` - the finger-pointing/link-select cursor style; equivalent to `IDC_HAND` in Win32.
							#define MU_CURSOR_HAND 6
							// @DOCLINE * `MU_CURSOR_SIZE_LR` - the resize cursor style, pointing left-to-right horizontally; equivalent to `IDC_SIZEWE` in Win32.
							#define MU_CURSOR_SIZE_LR 7
							// @DOCLINE * `MU_CURSOR_SIZE_TB` - the resize cursor style, pointing up-to-down vertically; equivalent to `IDC_SIZENS` in Win32.
							#define MU_CURSOR_SIZE_TB 8
							// @DOCLINE * `MU_CURSOR_SIZE_TL_BR` - the resize cursor style, pointing from top-left to bottom-right sideways; equivalent to `IDC_NWSE` in Win32.
							#define MU_CURSOR_SIZE_TL_BR 9
							// @DOCLINE * `MU_CURSOR_SIZE_TR_BL` - the resize cursor style, pointing from top-right to bottom-left sideways; equivalent to `IDC_NESW` in Win32.
							#define MU_CURSOR_SIZE_TR_BL 10
							// @DOCLINE * `MU_CURSOR_SIZE_ALL` - the move/resize-all cursor style, pointing outwards in all directions; equivalent to `IDC_SIZEALL` in Win32.
							#define MU_CURSOR_SIZE_ALL 11
							// @DOCLINE * `MU_CURSOR_NO` - the disallowing/error/not-allowed cursor style; equivalent to `IDC_NO` in WIn32.
							#define MU_CURSOR_NO 12

							#ifdef MUCOSA_NAMES
							// @DOCLINE ### Cursor style names

							// @DOCLINE The name function `mu_cursor_style_get_name` returns a `const char*` representation of the given cursor style (for example, `MU_CURSOR_HAND` returns "MU_CURSOR_HAND"), defined below: @NLNT
							MUDEF const char* mu_cursor_style_get_name(muCursorStyle style);
							// @DOCLINE It will return "MU_UNKNOWN" in the case that `style` is an invalid cursor style value.

							// @DOCLINE The name function `mu_cursor_style_get_nice_name` does the same thing, but with a nicer and more readable `const char*` representation (for example, `MU_CURSOR_HAND` returns "Hand"), defined below: @NLNT
							MUDEF const char* mu_cursor_style_get_nice_name(muCursorStyle style);
							// @DOCLINE It will return "Unknown" in the case that `style` is an invalid cursor style value.

							// @DOCLINE > These functions are "name" functions, and therefore are only defined if `MUCOSA_NAMES` is also defined by the user.
							#endif

						// @DOCLINE ## Text input

							// @DOCLINE muCOSA is able to get text input from a window, which uses a callback to give Unicode character codepoints based on what the user is typing. This system is recommended over trying to emulate text input via keyboard callbacks, as it automatically handles variables such as key states and input method managers.

							// @DOCLINE muCOSA needs to know where the text input is being taken visually on the window's surface; for this, the "text cursor" exists, whose position dictates where features like virtual keyboards render their preview characters, and is relative to the top-leftest pixel on the window's surface, and whose coordinates should always be within the window's surface (AKA less than the window's dimensions).

							// @DOCLINE ### Get text input focus

							// @DOCLINE The function `muCOSA_window_get_text_input` gets text input from a window, defined below: @NLNT
							MUDEF void muCOSA_window_get_text_input(muCOSAContext* context, muCOSAResult* result, muWindow win, uint32_m text_cursor_x, uint32_m text_cursor_y, void (*callback)(muWindow window, uint8_m* data));

							// @DOCLINE The callback will be called with a UTF-8-encoded character representing what character has been input by the user. This callback will only be called while the given window is updating (AKA while `muCOSA_window_update` is being called on it), just like all other window callbacks.

							// @DOCLINE Once text input is successfully retrieved for a window, it should be manually let go of by the user at some point before the window is destroyed and before this function is called on another window. Text input stops being sent to the window while it's unfocused, but text input focus is still retained, and does not need to be called again one the window is refocused.

							// @DOCLINE > The macro `mu_window_get_text_input` is the non-result-checking equivalent, and the macro `mu_window_get_text_input_focus_` is the result-checking equivalent.
							#define mu_window_get_text_input(...) muCOSA_window_get_text_input(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
							#define mu_window_get_text_input_(result, ...) muCOSA_window_get_text_input(muCOSA_global_context, result, __VA_ARGS__)

							// @DOCLINE ### Let go of text input focus

							// @DOCLINE The function `muCOSA_window_let_text_input` lets go of text input focus for the given window that has text input focus, defined below: @NLNT
							MUDEF void muCOSA_window_let_text_input(muCOSAContext* context, muWindow win);

							// @DOCLINE The given window must have text input focus before this function is called. If the parameters are valid, this function cannot fail, and thus, has no `result` parameter.

							// @DOCLINE > The macro `mu_window_destroy` is the non-result-checking equivalent.
							#define mu_window_let_text_input(...) muCOSA_window_let_text_input(muCOSA_global_context, __VA_ARGS__)

							// @DOCLINE ### Update text cursor position

							// @DOCLINE The function `muCOSA_window_update_text_cursor` updates the position of a text cursor for a window that has text input focus, defined below: @NLNT
							MUDEF void muCOSA_window_update_text_cursor(muCOSAContext* context, muCOSAResult* result, muWindow win, uint32_m x, uint32_m y);

							// @DOCLINE The given window must have text input focus before this function is called.

							// @DOCLINE > The macro `mu_window_update_text_cursor` is the non-result-checking equivalent, and the macro `mu_window_update_text_cursor_` is the result-checking equivalent.
							#define mu_window_update_text_cursor(...) muCOSA_window_update_text_cursor(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
							#define mu_window_update_text_cursor_(result, ...) muCOSA_window_update_text_cursor(muCOSA_global_context, result, __VA_ARGS__)

						// @DOCLINE ## Pixel format

							// @DOCLINE A window's pixel format is used to define what data will be used when representing the window's surface. Its respective type is `muPixelFormat`, and has the following members:

							struct muPixelFormat {
								// @DOCLINE * `@NLFT red_bits` - the amount of bits used for the red channel.
								uint16_m red_bits;
								// @DOCLINE * `@NLFT green_bits` - the amount of bits used for the green channel.
								uint16_m green_bits;
								// @DOCLINE * `@NLFT blue_bits` - the amount of bits used for the blue channel.
								uint16_m blue_bits;
								// @DOCLINE * `@NLFT alpha_bits` - the amount of bits used for the alpha channel.
								uint16_m alpha_bits;

								// @DOCLINE * `@NLFT depth_bits` - the amount of bits used for the depth channel.
								uint16_m depth_bits;
								// @DOCLINE * `@NLFT stencil_bits` - the amount of bits used for the stencil channel.
								uint16_m stencil_bits;

								// @DOCLINE * `@NLFT samples` - the amount of samples used for each pixel. A value of 1 means no multi-sampling is performed. Any value other than 1 indicates multi-sampling, and must be a power of 2.
								uint8_m samples;
							};

							// @DOCLINE 0 bits means that the data does not include it; for example, if `depth_bits` is equal to 0, then no depth data is defined in the pixel format.

						// @DOCLINE ## Graphics APIs

							typedef uint16_m muGraphicsAPI;

							// @DOCLINE muCOSA supports windows creating graphics APIs associated with them (respective type `muGraphicsAPI`, typedef for `uint16_m`). These are the currently defined and supported graphics APIs:

							// @DOCLINE * `MU_NULL_GRAPHICS_API` - unknown/no graphics API. This value can be given when the user does not want a window to be associated with a particular graphics API, and will attempt to load no graphics API.
							#define MU_NULL_GRAPHICS_API 0

							// == OpenGL (1-256) ==

							// @DOCLINE * `MU_OPENGL_1_0` - [OpenGL v1.0](https://registry.khronos.org/OpenGL/specs/gl/glspec10.pdf).
							#define MU_OPENGL_1_0 1
							// @DOCLINE * `MU_OPENGL_1_1` - [OpenGL v1.1](https://registry.khronos.org/OpenGL/specs/gl/glspec11.pdf).
							#define MU_OPENGL_1_1 2
							// @DOCLINE * `MU_OPENGL_1_2` - [OpenGL v1.2](https://registry.khronos.org/OpenGL/specs/gl/glspec121.pdf).
							#define MU_OPENGL_1_2 3
							// @DOCLINE * `MU_OPENGL_1_2_1` - [OpenGL v1.2.1](https://registry.khronos.org/OpenGL/specs/gl/glspec121.pdf).
							#define MU_OPENGL_1_2_1 4
							// @DOCLINE * `MU_OPENGL_1_3` - [OpenGL v1.3](https://registry.khronos.org/OpenGL/specs/gl/glspec13.pdf).
							#define MU_OPENGL_1_3 5
							// @DOCLINE * `MU_OPENGL_1_4` - [OpenGL v1.4](https://registry.khronos.org/OpenGL/specs/gl/glspec14.pdf).
							#define MU_OPENGL_1_4 6
							// @DOCLINE * `MU_OPENGL_1_5` - [OpenGL v1.5](https://registry.khronos.org/OpenGL/specs/gl/glspec15.pdf).
							#define MU_OPENGL_1_5 7
							// @DOCLINE * `MU_OPENGL_2_0` - [OpenGL v2.0](https://registry.khronos.org/OpenGL/specs/gl/glspec20.pdf).
							#define MU_OPENGL_2_0 8
							// @DOCLINE * `MU_OPENGL_2_1` - [OpenGL v2.1](https://registry.khronos.org/OpenGL/specs/gl/glspec21.pdf).
							#define MU_OPENGL_2_1 9
							// @DOCLINE * `MU_OPENGL_3_0` - [OpenGL v3.0](https://registry.khronos.org/OpenGL/specs/gl/glspec30.pdf).
							#define MU_OPENGL_3_0 10
							// @DOCLINE * `MU_OPENGL_3_1` - [OpenGL v3.1](https://registry.khronos.org/OpenGL/specs/gl/glspec31.pdf).
							#define MU_OPENGL_3_1 11
							// @DOCLINE * `MU_OPENGL_3_2_CORE` - [OpenGL v3.2 Core](https://registry.khronos.org/OpenGL/specs/gl/glspec32.core.pdf).
							#define MU_OPENGL_3_2_CORE 12
							// @DOCLINE * `MU_OPENGL_3_2_COMPATIBILITY` - [OpenGL v3.2 Compatibility](https://registry.khronos.org/OpenGL/specs/gl/glspec32.compatibility.pdf).
							#define MU_OPENGL_3_2_COMPATIBILITY 13
							// @DOCLINE * `MU_OPENGL_3_3_CORE` - [OpenGL v3.3 Core](https://registry.khronos.org/OpenGL/specs/gl/glspec33.core.pdf).
							#define MU_OPENGL_3_3_CORE 14
							// @DOCLINE * `MU_OPENGL_3_3_COMPATIBILITY` - [OpenGL v3.3 Compatibility](https://registry.khronos.org/OpenGL/specs/gl/glspec33.compatibility.pdf).
							#define MU_OPENGL_3_3_COMPATIBILITY 15
							// @DOCLINE * `MU_OPENGL_4_0_CORE` - [OpenGL v4.0 Core](https://registry.khronos.org/OpenGL/specs/gl/glspec40.core.pdf).
							#define MU_OPENGL_4_0_CORE 16
							// @DOCLINE * `MU_OPENGL_4_0_COMPATIBILITY` - [OpenGL v4.0 Compatibility](https://registry.khronos.org/OpenGL/specs/gl/glspec40.compatibility.pdf).
							#define MU_OPENGL_4_0_COMPATIBILITY 17
							// @DOCLINE * `MU_OPENGL_4_1_CORE` - [OpenGL v4.1 Core](https://registry.khronos.org/OpenGL/specs/gl/glspec41.core.pdf).
							#define MU_OPENGL_4_1_CORE 18
							// @DOCLINE * `MU_OPENGL_4_1_COMPATIBILITY` - [OpenGL v4.1 Compatibility](https://registry.khronos.org/OpenGL/specs/gl/glspec41.compatibility.pdf).
							#define MU_OPENGL_4_1_COMPATIBILITY 19
							// @DOCLINE * `MU_OPENGL_4_2_CORE` - [OpenGL v4.2 Core](https://registry.khronos.org/OpenGL/specs/gl/glspec42.core.pdf).
							#define MU_OPENGL_4_2_CORE 20
							// @DOCLINE * `MU_OPENGL_4_2_COMPATIBILITY` - [OpenGL v4.2 Compatibility](https://registry.khronos.org/OpenGL/specs/gl/glspec42.compatibility.pdf).
							#define MU_OPENGL_4_2_COMPATIBILITY 21
							// @DOCLINE * `MU_OPENGL_4_3_CORE` - [OpenGL v4.3 Core](https://registry.khronos.org/OpenGL/specs/gl/glspec43.core.pdf).
							#define MU_OPENGL_4_3_CORE 22
							// @DOCLINE * `MU_OPENGL_4_3_COMPATIBILITY` - [OpenGL v4.3 Compatibility](https://registry.khronos.org/OpenGL/specs/gl/glspec43.compatibility.pdf).
							#define MU_OPENGL_4_3_COMPATIBILITY 23
							// @DOCLINE * `MU_OPENGL_4_4_CORE` - [OpenGL v4.4 Core](https://registry.khronos.org/OpenGL/specs/gl/glspec44.core.pdf).
							#define MU_OPENGL_4_4_CORE 24
							// @DOCLINE * `MU_OPENGL_4_4_COMPATIBILITY` - [OpenGL v4.4 Compatibility](https://registry.khronos.org/OpenGL/specs/gl/glspec44.compatibility.pdf).
							#define MU_OPENGL_4_4_COMPATIBILITY 25
							// @DOCLINE * `MU_OPENGL_4_5_CORE` - [OpenGL v4.5 Core](https://registry.khronos.org/OpenGL/specs/gl/glspec45.core.pdf).
							#define MU_OPENGL_4_5_CORE 26
							// @DOCLINE * `MU_OPENGL_4_5_COMPATIBILITY` - [OpenGL v4.5 Compatibility](https://registry.khronos.org/OpenGL/specs/gl/glspec45.compatibility.pdf).
							#define MU_OPENGL_4_5_COMPATIBILITY 27
							// @DOCLINE * `MU_OPENGL_4_6_CORE` - [OpenGL v4.6 Core](https://registry.khronos.org/OpenGL/specs/gl/glspec46.core.pdf).
							#define MU_OPENGL_4_6_CORE 28
							// @DOCLINE * `MU_OPENGL_4_6_COMPATIBILITY` - [OpenGL v4.6 Compatibility](https://registry.khronos.org/OpenGL/specs/gl/glspec46.compatibility.pdf).
							#define MU_OPENGL_4_6_COMPATIBILITY 29

							// @DOCLINE Note that OpenGL will only work if `MU_SUPPORT_OPENGL` is defined before `muCOSA.h` is first included.

							// @DOCLINE ### Graphics API macro customization

							// @DOCLINE Files necessary to define OpenGL features (such as `gl/gh.`/`gl/glu.h` on Win32) are automatically included if `MU_SUPPORT_OPENGL` is defined; the inclusion of these files can be manually turned off (in case they have already been included) via defining `MUCOSA_NO_INCLUDE_OPENGL`.

							#ifdef MUCOSA_NAMES
							// @DOCLINE ### Graphics API names

							// @DOCLINE The name function `mu_graphics_api_get_name` returns a `const char*` representation of the given graphics API (for example, `MU_OPENGL_3_3_CORE` returns "MU_OPENGL_3_3_CORE"), defined below: @NLNT
							MUDEF const char* mu_graphics_api_get_name(muGraphicsAPI api);
							// @DOCLINE It will return "MU_UNKNOWN" in the case that `api` is an invalid graphics API value.

							// @DOCLINE The name function `mu_graphics_api_get_nice_name` does the same thing, but with a nicer and more readable `const char*` representation (for example, `MU_OPENGL_3_3_CORE` returns "OpenGL 3.3 (Core Profile)"), defined below: @NLNT
							MUDEF const char* mu_graphics_api_get_nice_name(muGraphicsAPI api);
							// @DOCLINE It will return "Unknown" in the case that `api` is an invalid graphics API value.

							// @DOCLINE > These functions are "name" functions, and therefore are only defined if `MUCOSA_NAMES` is also defined by the user.
							#endif

						// @DOCLINE ## OpenGL context

							#define muGLContext void*

							// @DOCLINE A window can be used to create a valid OpenGL context that is linked to the window (respective type `muGLContext`; macro for `void*`). Any operations to an OpenGL context are regarding to the currently bound OpenGL context on the current thread (besides the binding function itself), so some OpenGL context-related functions do not take the actual OpenGL context as a parameter.

							// @DOCLINE ### Create / Destroy OpenGL context

							// @DOCLINE The function `muCOSA_gl_context_create` creates an OpenGL context, defined below: @NLNT
							MUDEF muGLContext muCOSA_gl_context_create(muCOSAContext* context, muCOSAResult* result, muWindow win, muGraphicsAPI api);

							// @DOCLINE Every successfully created OpenGL context must be destroyed before the window that created it is destroyed. This function does not modify any previously binded OpenGL context.

							// @DOCLINE > The macro `mu_gl_context_create` is the non-result-checking equivalent, and the macro `mu_gl_context_create_` is the result-checking equivalent.
							#define mu_gl_context_create(...) muCOSA_gl_context_create(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
							#define mu_gl_context_create_(result, ...) muCOSA_gl_context_create(muCOSA_global_context, result, __VA_ARGS__)

							// @DOCLINE The function `muCOSA_gl_context_destroy` destroys an OpenGL context, defined below: @NLNT
							MUDEF muGLContext muCOSA_gl_context_destroy(muCOSAContext* context, muWindow win, muGLContext gl_context);

							// @DOCLINE The destruction function cannot fail if given a proper context and window, and thus, there is no `result` parameter.

							// @DOCLINE > The macro `mu_gl_context_destroy` is the non-result-checking equivalent.
							#define mu_gl_context_destroy(...) muCOSA_gl_context_destroy(muCOSA_global_context, __VA_ARGS__)

							// @DOCLINE ### Bind OpenGL context

							// @DOCLINE The function `muCOSA_gl_bind` binds an OpenGL context to the current thread, defined below: @NLNT
							MUDEF void muCOSA_gl_bind(muCOSAContext* context, muCOSAResult* result, muWindow win, muGLContext gl_context);

							// @DOCLINE If `gl_context` is not 0, it is expected to be a valid OpenGL context. If `gl_context` is 0, this function unbinds any OpenGL context currently binded on the calling thread; doing this without any OpenGL context previously binded is undefined behavior.

							// @DOCLINE This function is, unfortunately, one of the few functions whose defined functionality can differ based on the currently running window system. If this function gives a fatal return value on Win32, the previously binded OpenGL context is unbinded; on X11, the previously binded OpenGL context stays binded.

							// @DOCLINE > The macro `mu_gl_bind` is the non-result-checking equivalent, and the macro `mu_gl_bind_` is the result-checking equivalent.
							#define mu_gl_bind(...) muCOSA_gl_bind(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
							#define mu_gl_bind_(result, ...) muCOSA_gl_bind(muCOSA_global_context, result, __VA_ARGS__)

							// @DOCLINE ### Swap buffers for OpenGL

							// @DOCLINE The function `muCOSA_gl_swap_buffers` swaps the buffers of a window associated with at least one OpenGL context, defined below: @NLNT
							MUDEF void muCOSA_gl_swap_buffers(muCOSAContext* context, muCOSAResult* result, muWindow win);

							// @DOCLINE Because this function acts purely on the window, and not directly with any associated OpenGL context, this should only be called if an OpenGL context exists that was created from the given window, and should be called every frame before `muCOSA_window_update` for as long as there is an OpenGL context associated with the window.

							// @DOCLINE > The macro `mu_gl_swap_buffers` is the non-result-checking equivalent, and the macro `mu_gl_swap_buffers_` is the result-checking equivalent.
							#define mu_gl_swap_buffers(...) muCOSA_gl_swap_buffers(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
							#define mu_gl_swap_buffers_(result, ...) muCOSA_gl_swap_buffers(muCOSA_global_context, result, __VA_ARGS__)

							// @DOCLINE ### Get procedure address

							// @DOCLINE The function `muCOSA_gl_get_proc_address` returns the address of a requested OpenGL function by name, defined below: @NLNT
							MUDEF void* muCOSA_gl_get_proc_address(muCOSAContext* context, const char* name);

							// @DOCLINE This function must be called with a valid OpenGL context binded. On failure, this function returns 0, and on success, returns a non-zero value. A valid address given by this function can only be guaranteed to be valid in relation to the currently binded OpenGL context.

							// @DOCLINE > The macro `mu_gl_get_proc_address` is the non-result-checking equivalent.
							#define mu_gl_get_proc_address(...) muCOSA_gl_get_proc_address(muCOSA_global_context, __VA_ARGS__)

							// @DOCLINE ### Swap interval

							// @DOCLINE The function `muCOSA_gl_swap_interval` acts as a call to `wglSwapIntervalEXT`, defined below: @NLNT
							MUDEF muBool muCOSA_gl_swap_interval(muCOSAContext* context, muCOSAResult* result, int interval);

							// @DOCLINE On Win32, this function returns the return value of `wglSwapIntervalEXT` if `result` is set to a non-fatal value, and 0 if otherwise.

							// @DOCLINE > The macro `mu_gl_swap_interval` is the non-result-checking equivalent, and the macro `mu_gl_swap_interval_` is the result-checking equivalent.
							#define mu_gl_swap_interval(...) muCOSA_gl_swap_interval(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
							#define mu_gl_swap_interval_(result, ...) muCOSA_gl_swap_interval(muCOSA_global_context, result, __VA_ARGS__)

					// @DOCLINE # Time

						// @DOCLINE Every muCOSA context has a "fixed time", which refers to the amount of seconds it has been since the context was first created, stored internally as a double. The "fixed time" is different than the "time", which is usually equal to the fixed time, unless it is manually overwritten by the user, which is available in the muCOSA API.

						// @DOCLINE ## Get fixed time

						// @DOCLINE The function `muCOSA_fixed_time_get` retrieves the current amount of fixed time for a muCOSA context, defined below: @NLNT
						MUDEF double muCOSA_fixed_time_get(muCOSAContext* context);

						// @DOCLINE This function cannot fail if the parameter(s) are valid.

						// @DOCLINE > The macro `mu_fixed_time_get` is the non-result-checking equivalent.
						#define mu_fixed_time_get() muCOSA_fixed_time_get(muCOSA_global_context)

						// @DOCLINE ## Get/Set time

						// @DOCLINE The function `muCOSA_time_get` retrieves the current amount of time for a muCOSA context, defined below: @NLNT
						MUDEF double muCOSA_time_get(muCOSAContext* context);

						// @DOCLINE > The macro `mu_time_get` is the non-result-checking equivalent.
						#define mu_time_get() muCOSA_time_get(muCOSA_global_context)

						// @DOCLINE The function `muCOSA_time_set` overwrites the current time for a muCOSA context, defined below: @NLNT
						MUDEF void muCOSA_time_set(muCOSAContext* context, double time);

						// @DOCLINE This function is valid to call with negative values.

						// @DOCLINE > The macro `mu_time_set` is the non-result-checking equivalent.
						#define mu_time_set(...) muCOSA_time_set(muCOSA_global_context, __VA_ARGS__)

						// @DOCLINE These functions cannot fail if the parameter(s) are valid.

					// @DOCLINE # Sleep

						// @DOCLINE The sleep function `muCOSA_sleep` is used to sleep for a given amount of seconds, defined below: @NLNT
						MUDEF void muCOSA_sleep(muCOSAContext* context, double time);

						// @DOCLINE This function cannot fail if the parameter(s) are valid.

						// @DOCLINE > The macro `mu_sleep` is the non-result-checking equivalent.
						#define mu_sleep(...) muCOSA_sleep(muCOSA_global_context, __VA_ARGS__)

					// @DOCLINE # Clipboard

						// @DOCLINE muCOSA offers functionality for getting and setting the current text clipboard.

						// @DOCLINE ## Get clipboard

						// @DOCLINE The function `muCOSA_clipboard_get` retrieves the current text clipboard, defined below: @NLNT
						MUDEF uint8_m* muCOSA_clipboard_get(muCOSAContext* context, muCOSAResult* result);

						// @DOCLINE On success, this function rather returns 0 (implying that there is no text clipboard set), or a pointer to data manually allocated by muCOSA; in the latter circumstance, it must be freed by the user manually when they are finished using the data.
						// @DOCLINE On failure, this function returns 0, and `result` is set to the failure value.

						// @DOCLINE > The macro `mu_clipboard_get` is the non-result-checking equivalent, and the macro `mu_clipboard_get_` is the result-checking equivalent.
						#define mu_clipboard_get() muCOSA_clipboard_get(muCOSA_global_context, &muCOSA_global_context->result)
						#define mu_clipboard_get_(result) muCOSA_clipboard_get(muCOSA_global_context, result)

						// @DOCLINE ## Set clipboard

						// @DOCLINE The function `muCOSA_clipboard_set` sets the current text clipboard, defined below: @NLNT
						MUDEF void muCOSA_clipboard_set(muCOSAContext* context, muCOSAResult* result, uint8_m* data, size_m datalen);

						// @DOCLINE On success, this function sets the current text clipboard to the given UTF-8 text data, of length `datalen` (including null-terminating character).

						// @DOCLINE > The macro `mu_clipboard_set` is the non-result-checking equivalent, and the macro `mu_clipboard_set_` is the result-checking equivalent.
						#define mu_clipboard_set(...) muCOSA_clipboard_set(muCOSA_global_context, &muCOSA_global_context->result, __VA_ARGS__)
						#define mu_clipboard_set_(result, ...) muCOSA_clipboard_set(muCOSA_global_context, result, __VA_ARGS__)

					// @DOCLINE # Result

						// @DOCLINE The type `muCOSAResult` (typedef for `uint16_m`) is used to represent how a task in muCOSA went. It has the following defined values:

						// == MUCOSA_... 0-4095 ==

						// @DOCLINE * `MUCOSA_SUCCESS` - the task succeeded; real value 0.
						#define MUCOSA_SUCCESS 0

						// @DOCLINE * `MUCOSA_FAILED_NULL_WINDOW_SYSTEM` - rather an invalid window system value was given by the user, the window system value given by the user was unsupported, or no supported window system could be found.
						#define MUCOSA_FAILED_NULL_WINDOW_SYSTEM 1

						// @DOCLINE * `MUCOSA_FAILED_MALLOC` - a call to `mu_malloc` failed, meaning that there is insufficient memory available to perform the task.
						#define MUCOSA_FAILED_MALLOC 2

						// @DOCLINE * `MUCOSA_FAILED_UNKNOWN_WINDOW_ATTRIB` - an invalid `muWindowAttrib` value was given by the user.
						#define MUCOSA_FAILED_UNKNOWN_WINDOW_ATTRIB 3

						// @DOCLINE * `MUCOSA_FAILED_REALLOC` - a call to `mu_realloc` failed, meaning that there is insufficient memory available to perform the task.
						#define MUCOSA_FAILED_REALLOC 4

						// @DOCLINE * `MUCOSA_FAILED_UNKNOWN_GRAPHICS_API` - an invalid `muGraphicsAPI` value was given by the user.
						#define MUCOSA_FAILED_UNKNOWN_GRAPHICS_API 5

						// @DOCLINE * `MUCOSA_FAILED_UNSUPPORTED_GRAPHICS_API` - a function relating to a graphics API was called despite the fact that support for the graphics API was not enabled.
						#define MUCOSA_FAILED_UNSUPPORTED_GRAPHICS_API 6

						// == MUCOSA_WIN32_... 4096-8191 ==

						// @DOCLINE * `MUCOSA_WIN32_FAILED_CONVERT_UTF8_TO_WCHAR` - a conversion from a UTF-8 string to a wide character string failed, rather due to the conversion itself failing or the allocation of memory required for the conversion; this is exclusive to Win32.
						#define MUCOSA_WIN32_FAILED_CONVERT_UTF8_TO_WCHAR 4096

						// @DOCLINE * `MUCOSA_WIN32_FAILED_REGISTER_WINDOW_CLASS` - a call to `RegisterClassExW` failed, meaning that the window class needed to create the window could not be created; this is exclusive to Win32.
						#define MUCOSA_WIN32_FAILED_REGISTER_WINDOW_CLASS 4097

						// @DOCLINE * `MUCOSA_WIN32_FAILED_CREATE_WINDOW` - a call to `CreateWindowExW` failed, meaning that the window could not be created; this is exclusive to Win32.
						#define MUCOSA_WIN32_FAILED_CREATE_WINDOW 4098

						// @DOCLINE * `MUCOSA_WIN32_FAILED_GET_WINDOW_ATTRIB` - whatever function needed to retrieve the requested window attribute returned a non-success value; this is exclusive to Win32.
						#define MUCOSA_WIN32_FAILED_GET_WINDOW_ATTRIB 4099
						// @DOCLINE    * In the case of dimensions, `GetClientRect` failed.
						// @DOCLINE    * In the case of position, `GetWindowRect` failed.
						// @DOCLINE    * In the case of cursor, rather `GetCursorPos` or `muCOSA_window_get(...MU_WINDOW_POSITION)` failed.

						// @DOCLINE * `MUCOSA_WIN32_FAILED_SET_WINDOW_ATTRIB` - whatever function needed to modify the requested window attribute returned a non-success value; this is exclusive to Win32.
						#define MUCOSA_WIN32_FAILED_SET_WINDOW_ATTRIB 4100
						// @DOCLINE    * In the case of title, `SetWindowTextW` failed.
						// @DOCLINE    * In the case of dimensions, rather `GetWindowInfo`, `AdjustWindowRect`, or `SetWindowPos` failed.
						// @DOCLINE    * In the case of position, `SetWindowPos` failed.
						// @DOCLINE    * In the case of cursor, rather `SetCursorPos` or `muCOSA_window_get(...MU_WINDOW_POSITION)` failed.

						// @DOCLINE * `MUCOSA_WIN32_FAILED_REGISTER_DUMMY_WGL_WINDOW_CLASS` - a call to `RegisterClassA` in the process of loading OpenGL functions failed; this is exclusive to Win32. This result is non-fatal, and the context will still be created, but any attempt to create an OpenGL context will fail.
						#define MUCOSA_WIN32_FAILED_REGISTER_DUMMY_WGL_WINDOW_CLASS 4101

						// @DOCLINE * `MUCOSA_WIN32_FAILED_CREATE_DUMMY_WGL_WINDOW` - a call to `CreateWindowExA` in the process of loading OpenGL functions failed; this is exclusive to Win32. This result is non-fatal, and the context will still be created, but any attempt to create an OpenGL context will fail.
						#define MUCOSA_WIN32_FAILED_CREATE_DUMMY_WGL_WINDOW 4102

						// @DOCLINE * `MUCOSA_WIN32_FAILED_GET_DUMMY_WGL_PIXEL_FORMAT` - a call to `GetPixelFormat` in the process of loading OpenGL functions failed; this is exclusive to Win32. This result is non-fatal, and the context will still be created, but any attempt to create an OpenGL context will fail.
						#define MUCOSA_WIN32_FAILED_GET_DUMMY_WGL_PIXEL_FORMAT 4103

						// @DOCLINE * `MUCOSA_WIN32_FAILED_DESCRIBE_DUMMY_WGL_PIXEL_FORMAT` - a call to `DescribePixelFormat` in the process of loading OpenGL functions failed; this is exclusive to Win32. This result is non-fatal, and the context will still be created, but any attempt to create an OpenGL context will fail.
						#define MUCOSA_WIN32_FAILED_DESCRIBE_DUMMY_WGL_PIXEL_FORMAT 4104

						// @DOCLINE * `MUCOSA_WIN32_FAILED_SET_DUMMY_WGL_PIXEL_FORMAT` - a call to `SetPixelFormat` in the process of loading OpenGL functions failed; this is exclusive to Win32. This result is non-fatal, and the context will still be created, but any attempt to create an OpenGL context will fail.
						#define MUCOSA_WIN32_FAILED_SET_DUMMY_WGL_PIXEL_FORMAT 4105

						// @DOCLINE * `MUCOSA_WIN32_FAILED_CREATE_DUMMY_WGL_CONTEXT` - a call to `wglCreateContext` in the process of loading OpenGL functions failed; this is exclusive to Win32. This result is non-fatal, and the context will still be created, but any attempt to create an OpenGL context will fail.
						#define MUCOSA_WIN32_FAILED_CREATE_DUMMY_WGL_CONTEXT 4106

						// @DOCLINE * `MUCOSA_WIN32_FAILED_BIND_DUMMY_WGL_CONTEXT` - a call to `wglMakeCurrent` in the process of loading OpenGL functions failed; this is exclusive to Win32. This result is non-fatal, and the context will still be created, but any attempt to create an OpenGL context will fail.
						#define MUCOSA_WIN32_FAILED_BIND_DUMMY_WGL_CONTEXT 4107

						// @DOCLINE * `MUCOSA_WIN32_FAILED_FIND_WGL_CREATE_CONTEXT_ATTRIBS` - the function `wglCreateContextAttribsARB` could not be found, which is necessary to creating OpenGL functions; this is exclusive to Win32. This result is non-fatal, and the context will still be created, but any attempt to create an OpenGL context will fail.
						#define MUCOSA_WIN32_FAILED_FIND_WGL_CREATE_CONTEXT_ATTRIBS 4108

						// @DOCLINE * `MUCOSA_WIN32_FAILED_FIND_WGL_CHOOSE_PIXEL_FORMAT` - the function `wglChoosePixelFormatARB` could not be found, which is necessary to creating OpenGL functions; this is exclusive to Win32. This result is non-fatal, and the context will still be created, but any attempt to create an OpenGL context will fail.
						#define MUCOSA_WIN32_FAILED_FIND_WGL_CHOOSE_PIXEL_FORMAT 4109

						// @DOCLINE * `MUCOSA_WIN32_FAILED_CHOOSE_WGL_PIXEL_FORMAT` - the function `wglChoosePixelFormatARB` returned a failure value when creating an OpenGL context; this is exclusive to Win32. This result is non-fatal, and the context will still be created, but the pixel format will likely not be what the user requested.
						#define MUCOSA_WIN32_FAILED_CHOOSE_WGL_PIXEL_FORMAT 4110

						// @DOCLINE * `MUCOSA_WIN32_FAILED_GET_WGL_PIXEL_FORMAT` - the function `GetPixelFormat` returned a failure value when creating an OpenGL context; this is exclusive to Win32.
						#define MUCOSA_WIN32_FAILED_GET_WGL_PIXEL_FORMAT 4111

						// @DOCLINE * `MUCOSA_WIN32_FAILED_DESCRIBE_WGL_PIXEL_FORMAT` - the function `DescribePixelFormat` returned a failure value when creating an OpenGL context; this is exclusive to Win32.
						#define MUCOSA_WIN32_FAILED_DESCRIBE_WGL_PIXEL_FORMAT 4112

						// @DOCLINE * `MUCOSA_WIN32_FAILED_SET_WGL_PIXEL_FORMAT` - the function `SetPixelFormat` returned a failure value when creating an OpenGL context; this is exclusive to Win32.
						#define MUCOSA_WIN32_FAILED_SET_WGL_PIXEL_FORMAT 4113

						// @DOCLINE * `MUCOSA_WIN32_FAILED_CREATE_WGL_CONTEXT` - the function `wglCreateContextAttribsARB` returned a failure value when creating an OpenGL context; this is exclusive to Win32.
						#define MUCOSA_WIN32_FAILED_CREATE_WGL_CONTEXT 4114

						// @DOCLINE * `MUCOSA_WIN32_FAILED_SET_WGL_CONTEXT` - the function `wglMakeCurrent` returned a failure value when binding the OpenGL context; this is exclusive to Win32.
						#define MUCOSA_WIN32_FAILED_SET_WGL_CONTEXT 4115

						// @DOCLINE * `MUCOSA_WIN32_FAILED_SWAP_WGL_BUFFERS` - the function `SwapBuffers` returned a failure value when swapping the buffers; this is exclusive to Win32.
						#define MUCOSA_WIN32_FAILED_SWAP_WGL_BUFFERS 4116

						// @DOCLINE * `MUCOSA_WIN32_FAILED_FIND_WGL_FUNCTION` - the corresponding OpenGL function could not be located; this is exclusive to Win32.
						#define MUCOSA_WIN32_FAILED_FIND_WGL_FUNCTION 4117

						// @DOCLINE * `MUCOSA_WIN32_FAILED_ASSOCIATE_IMM` - the function `ImmAssociateContextEx` returned a failure value when getting text input focus; this is exclusive to Win32.
						#define MUCOSA_WIN32_FAILED_ASSOCIATE_IMM 4118

						// @DOCLINE * `MUCOSA_WIN32_FAILED_SET_COMPOSITION_WINDOW_POSITION` - the function `ImmSetCompositionWindow` returned a failure value when attempting to move it to the current text cursor position; this is exclusive to Win32.
						#define MUCOSA_WIN32_FAILED_SET_COMPOSITION_WINDOW_POSITION 4119

						// @DOCLINE * `MUCOSA_WIN32_FAILED_HOLD_CLIPBOARD` - the function `OpenClipboard` returned a failure value when attempting to retrieve the clipboard data (`muCOSA_clipboard_get`) or overwrite it (`muCOSA_clipboard_set`); this is exclusive to Win32.
						#define MUCOSA_WIN32_FAILED_HOLD_CLIPBOARD 4120

						// @DOCLINE * `MUCOSA_WIN32_FAILED_GET_CLIPBOARD_DATA` - the function `GlobalLock` returned a failure value when attempting to retrieve a pointer to the clipboard data when attempting to retrieve the clipboard data (`muCOSA_clipboard_get`); this is exclusive to Win32.
						#define MUCOSA_WIN32_FAILED_GET_CLIPBOARD_DATA 4121

						// @DOCLINE * `MUCOSA_WIN32_FAILED_CONVERT_CLIPBOARD_DATA_FORMAT` - the conversion between UTF-16 wide-character data and UTF-8 `uint8_m*` data (rather converting from UTF-8 to UTF-16 when setting the clipboard data (`muCOSA_clipboard_set`), or converting from UTF-16 to UTF-8 when getting the clipboard data (`muCOSA_clipboard_get`)) failed, rather due to allocation or to the data itself being invalid; this is exclusive to Win32.
						#define MUCOSA_WIN32_FAILED_CONVERT_CLIPBOARD_DATA_FORMAT 4122

						// @DOCLINE * `MUCOSA_WIN32_FAILED_ALLOCATE_CLIPBOARD_DATA` - rather the function `GlobalAlloc` or `GlobalLock` failed when attempting to allocate and get a pointer to the global data for the clipboard when setting the clipboard (`muCOSA_clipboard_set`); this is exclusive to Win32.
						#define MUCOSA_WIN32_FAILED_ALLOCATE_CLIPBOARD_DATA 4123

						// @DOCLINE * `MUCOSA_WIN32_FAILED_SET_CLIPBOARD_DATA` - the function `SetClipboardData` failed when attempting to set the clipboard data; this is exclusive to Win32.
						#define MUCOSA_WIN32_FAILED_SET_CLIPBOARD_DATA 4124

						// @DOCLINE All non-success values (unless explicitly stated otherwise) mean that the function fully failed, AKA it was "fatal", and the library continues as if the function had never been called; so, for example, if something was supposed to be allocated, but the function fatally failed, nothing was allocated.

						// @DOCLINE There are non-fatal failure values, which mean that the function still executed, but not fully to the extent that the user would expect from the function. The function `muCOSA_result_is_fatal` returns whether or not a given result function value is fatal, defined below: @NLNT
						MUDEF muBool muCOSA_result_is_fatal(muCOSAResult result);

						// @DOCLINE > This function returns `MU_TRUE` if the value of `result` is invalid. It returns `MU_FALSE` if `result` is equal to `MUCOSA_SUCCESS`.

						// @DOCLINE ## Result names
						#ifdef MUCOSA_NAMES

						// @DOCLINE The name function `muCOSA_result_get_name` returns a `const char*` representation of the given result value (for example, `MUCOSA_SUCCESS` returns "MUCOSA_SUCCESS"), defined below: @NLNT
						MUDEF const char* muCOSA_result_get_name(muCOSAResult result);

						// @DOCLINE It will return "MU_UNKNOWN" in the case that `result` is an invalid result value.

						// @DOCLINE > This function is a "name" function, and therefore is only defined if `MUCOSA_NAMES` is also defined by the user.

						#endif

					// @DOCLINE # C standard library dependencies

						// @DOCLINE muCOSA has several C standard library dependencies, all of which are overridable by defining them before the inclusion of its header. The following is a list of those dependencies.

						// @DOCLINE ## `string.h` dependencies
						#if !defined(mu_memcpy) || \
							!defined(mu_memset)

							#include <string.h>

							// @DOCLINE * `mu_memcpy` - equivalent to `memcpy`.
							#ifndef mu_memcpy
								#define mu_memcpy memcpy
							#endif

							// @DOCLINE * `mu_memset` - equivalent to `memset`.
							#ifndef mu_memset
								#define mu_memset memset
							#endif

						#endif /* string.h */

						// @DOCLINE ## `stdlib.h` dependencies
						#if !defined(mu_malloc) || \
							!defined (mu_free) || \
							!defined(mu_realloc)

							#include <stdlib.h>

							// @DOCLINE * `mu_malloc` - equivalent to `malloc`.
							#ifndef mu_malloc
								#define mu_malloc malloc
							#endif

							// @DOCLINE * `mu_free` - equivalent to `free`.
							#ifndef mu_free
								#define mu_free free
							#endif

							// @DOCLINE * `mu_realloc` - equivalent to `realloc`.
							#ifndef mu_realloc
								#define mu_realloc realloc
							#endif

						#endif /* stdlib.h */

					MU_CPP_EXTERN_END

				#endif /* MUCOSA_H */
			// @ATTENTION

			// @DOCLINE * [glad 2.0.6 GL v3.3 Core](https://github.com/Dav1dde/glad/releases/tag/v2.0.6). glad is only included in the implementation of mug, meaning that only defining the header of mug won't give the user access to OpenGL.

			// @DOCLINE > Note that mu libraries store their dependencies within their files, so you don't need to import these dependencies yourself; this section is purely to give more information about the contents that this file defines. The libraries listed may also have other dependencies that they also include that aren't explicitly listed here.

			MU_CPP_EXTERN_START

			// Typedefs explained later
			typedef uint16_m mugResult;
			typedef void* mugTexture;

			// @DOCLINE # Graphics API support macros

				// @DOCLINE Each graphics API needs to have a macro defined to enable support for it. The following macros are defined for the given graphics APIs to be supported:

				// @DOCLINE * OpenGL 3.3 Core - `MU_SUPPORT_OPENGL`.

				// @DOCLINE So, for example, an inclusion of mug with OpenGL 3.3 Core would look like:

				/* @DOCBEGIN
				```c
				#define MU_SUPPORT_OPENGL
				#define MUG_IMPLEMENTATION
				#include "muGraphics.h"
				```
				@DOCEND */

			// @DOCLINE # Version

				// @DOCLINE The macros `MUG_VERSION_MAJOR`, `MUG_VERSION_MINOR`, and `MUG_VERSION_PATCH` are defined to match its respective release version, following the formatting of `MAJOR.MINOR.PATCH`.

				#define MUG_VERSION_MAJOR 1
				#define MUG_VERSION_MINOR 0
				#define MUG_VERSION_PATCH 0

			// @DOCLINE # mug context

				// @DOCLINE mug operates in a context, encapsulated by the type `mugContext`, which has the following members:

				struct mugContext {
					// @DOCLINE * `@NLFT result` - the result of the latest non-successful non-result-checking function call regarding the context; starting value upon context creation is `MUG_SUCCESS`, and is set to another value if a function whose result is not set manually by the user doesn't return a success result value.
					mugResult result;
					// @DOCLINE * `@NLFT cosa` - the muCOSA context used by the mug context.
					muCOSAContext cosa;
				};
				typedef struct mugContext mugContext;

				// @DOCLINE ## Creation and destruction

					// @DOCLINE The function `mug_context_create` creates a mug context, defined below: @NLNT
					MUDEF void mug_context_create(mugContext* context, muWindowSystem system, muBool set_context);

					// @DOCLINE The result of this function is stored within `context->result`. Upon success, this function automatically calls `mug_context_set` on the created context, as well as `muCOSA_context_set` on the created muCOSA context, unless `set_context` is equal to `MU_FALSE`.

					// @DOCLINE > It is valid for `system` to be `MU_WINDOW_NULL` for this function, in which case, the best currently available window system will be automatically chosen by muCOSA. More information about the window system is available via muCOSA's documentation.

					// @DOCLINE For every successfully created context, it must be destroyed, which is done with the function `mug_context_destroy`, defined below: @NLNT
					MUDEF void mug_context_destroy(mugContext* context);
					// @DOCLINE This function cannot fail if given a valid pointer to an active context (otherwise, a crash is likely), so no result value is ever indicated by this function via any means.

				// @DOCLINE ## Global context

					MUDEF mugContext* mug_global_context;

					// @DOCLINE mug uses a global variable to reference the currently "set" context whenever a function is called that assumes a context (ie it doesn't take a parameter for context). This global variable can be changed to reference a certain context via the function `mug_context_set`, defined below: @NLNT
					MUDEF void mug_context_set(mugContext* context);

				// @DOCLINE ## Non-result/context-checking functions

					/* @DOCBEGIN
					If a function takes a `mugContext` and `mugResult` parameter, there will likely be two defined macros for calling the function without explicitly passing those parameters, with the current global context being assumed for both parameters.

					Non-result-checking functions are functions that assume the `mugContext` parameter to be the current global context, and assume the `mugResult` parameter to be the current global context's result member. These functions' parameters are simply the normal function's parameters, but without the context or result parameter, instead being routed to the current global context. The name of these functions are simply the normal name, but `mug_...` is replaced with just `mu_...`.

					Result-checking functions are functions that also assume (and thus don't make you specify) the `mugContext` parameter to be the current global context, but they still make you specify the `mugResult` parameter, and the global context's result member goes unreferenced. The name of these functions is the same as the non-result-checking functions, but with an underscore appended at the end.

					For example, if you the function `mug_graphic_do_something` existed with these parameters:
					```c
					MUDEF void mug_graphic_do_something(mugContext* context, mugResult* result, int a, int b);
					```

					then the function `mu_graphic_do_something`, the "non-result-checking function", exists with these parameters:
					```c
					MUDEF void mu_graphic_do_something(int a, int b);
					// Calls mug_graphic_do_something with the global context
					// and its respective result member.
					```

					and the function `mu_graphic_do_something_`, the "result-checking function", exists with these parameters:
					```c
					MUDEF void mu_graphic_do_something_(mugResult* result, int a, int b);
					// Calls mug_graphic_do_something with the global context
					// and the given result pointer.
					```

					> Note that, in reality, the non-result and result-checking functions aren't defined as actual independent functions, but instead, as macros to the original function. More information about the type `mugResult` can be found in the [result section](#result).
					@DOCEND */

			// @DOCLINE # Graphic

				typedef void* muGraphic;

				// @DOCLINE The "graphic" (respective type `muGraphic`; typedef for `void*`) is a surface being rendered to by some graphics API. Since a graphic is just a reference to a rendering surface, it must be created via some other object that encapsulates a rendering surface. This is why muCOSA is included in mug, as it is able to create objects that have these surfaces, such as a window.

				// @DOCLINE ## Graphic system

					typedef uint8_m muGraphicSystem;

					// @DOCLINE The "graphic system" (respective type `muGraphicSystem`; typedef for `uint8_m`) is a value representing which supported system is being used to render to a given graphic. It has the following supported values:

					// @DOCLINE * `MU_GRAPHIC_OPENGL` - OpenGL 3.3 Core.
					#define MU_GRAPHIC_OPENGL 1

				// @DOCLINE ## Destroy graphic

					// @DOCLINE The function `mug_graphic_destroy` destroys a given graphic, defined below: @NLNT
					MUDEF muGraphic mug_graphic_destroy(mugContext* context, muGraphic gfx);

					// @DOCLINE This function must be called on every successfully-created graphic before the destruction of the context used to create it. This function returns 0.

					// @DOCLINE > The macro `mu_graphic_destroy` is the non-result-checking equivalent.
					#define mu_graphic_destroy(...) mug_graphic_destroy(mug_global_context, __VA_ARGS__)

				// @DOCLINE ## Window graphic

					// @DOCLINE A "window graphic" is a graphic created via a muCOSA window.

					// @DOCLINE ### Creation

						// @DOCLINE The function `mug_graphic_create_window` creates a `muGraphic` by creating a `muWindow` object, defined below: @NLNT
						MUDEF muGraphic mug_graphic_create_window(mugContext* context, mugResult* result, muGraphicSystem system, muWindowInfo* info);

						// @DOCLINE This function allows the user to set their desired pixel format. This means that the user can disable depth testing by not including any depth bits (assuming that their pixel format is chosen as desired).

						// @DOCLINE > The macro `mu_graphic_create_window` is the non-result-checking equivalent, and the macro `mu_graphic_create_window_` is the result-checking equivalent.
						#define mu_graphic_create_window(...) mug_graphic_create_window(mug_global_context, &mug_global_context->result, __VA_ARGS__)
						#define mu_graphic_create_window_(result, ...) mug_graphic_create_window(mug_global_context, result, __VA_ARGS__)

					// @DOCLINE ### Get window

						// @DOCLINE The function `mug_graphic_get_window` retrieves the window handle generated from a successfully-created graphic that was created using a muCOSA window, defined below: @NLNT
						MUDEF muWindow mug_graphic_get_window(mugContext* context, muGraphic gfx);

						// @DOCLINE > The macro `mu_graphic_get_window` is the non-result-checking equivalent.
						#define mu_graphic_get_window(...) mug_graphic_get_window(mug_global_context, __VA_ARGS__)

				// @DOCLINE ## Main loop graphic functions

					// @DOCLINE There are four primary functions that must be called every single frame in a particular manner; these functions are `mug_graphic_exists`, `mug_graphic_clear`, `mug_graphic_swap_buffers`, and `mug_graphic_update`. A valid mug frame loop is formatted like so:

					/* @DOCBEGIN
					```c
					// Loop frame-by-frame while the graphic exists:
					while (mu_graphic_exists(gfx))
					{
						// Clear the graphic with black
						mu_graphic_clear(gfx, 0.f, 0.f, 0.f);

						// ...Frame-by-frame logic and rendering should occur here...

						// Swap graphic buffers (to present image)
						mu_graphic_swap_buffers(gfx);

						// Update graphic at ~100 FPS
						mu_graphic_update(gfx, 100.f);
					}
					```
					@DOCEND */

					// @DOCLINE All of the functions demonstrated in this section should only be called once each frame, and in the order given. Explicit documentation on these functions and what they do is provided below.

					// @DOCLINE ### Get graphic existence status

						// @DOCLINE The graphic can stop existing for several reasons, such as in the case of it coming from a window and the user closing the window. For this, the function `mug_graphic_exists` exists, which returns whether or not the graphic exists at any given, and is used to establish a loop for as long as the graphic is alive, defined below: @NLNT
						MUDEF muBool mug_graphic_exists(mugContext* context, muGraphic gfx);

						// @DOCLINE The status of this function is updated every time `mug_graphic_update` is called, meaning that the two should always be called consecutively. For this reason, `mug_graphic_update` should be called at the end of the `mug_graphic_exists` loop, so that upon the existence check being called, the graphic's state for this has been updated directly beforehand.

						// @DOCLINE > The macro `mu_graphic_exists` is the non-result-checking equivalent.
						#define mu_graphic_exists(...) mug_graphic_exists(mug_global_context, __VA_ARGS__)

					// @DOCLINE ### Clear graphic

						// @DOCLINE Once the current frame has started, the contents of the graphic are undefined (both in the case of the current frame being the first frame or it not being the first frame). To resolve this, the graphic must be cleared with the function `mug_graphic_clear`, defined below: @NLNT
						MUDEF void mug_graphic_clear(mugContext* context, mugResult* result, muGraphic gfx, float r, float g, float b);

						// @DOCLINE This function clears the color of each pixel in the graphic with the given RGB values (each value must range between `0.f` and `1.f`). It also clears the depth-buffer of the graphic. Not calling this function before calling any rendering function, `mug_graphic_swap_buffers`, or `mug_graphic_update`, will result in undefined behavior, as the contents of the graphic are unknown in the state between the beginning of the frame and the execution of the function `mug_graphic_clear`.

						// @DOCLINE > The macro `mu_graphic_clear` is the non-result-checking equivalent, and the macro `mu_graphic_clear_` is the result-checking equivalent.
						#define mu_graphic_clear(...) mug_graphic_clear(mug_global_context, &mug_global_context->result, __VA_ARGS__)
						#define mu_graphic_clear_(result, ...) mug_graphic_clear(mug_global_context, result, __VA_ARGS__)

					// @DOCLINE ### Swap graphic buffers

						// @DOCLINE One all rendering calls for a given frame have been executed, it must be presented to the screen, which is what `mug_graphic_swap_buffers` does, defined below: @NLNT
						MUDEF void mug_graphic_swap_buffers(mugContext* context, mugResult* result, muGraphic gfx);

						// @DOCLINE This function should be called after all rendering functions have been called for a given frame, and no rendering functions should be called after this function for the given frame.

						// @DOCLINE > The macro `mu_graphic_swap_buffers` is the non-result-checking equivalent, and the macro `mu_graphic_swap_buffers_` is the result-checking equivalent.
						#define mu_graphic_swap_buffers(...) mug_graphic_swap_buffers(mug_global_context, &mug_global_context->result, __VA_ARGS__)
						#define mu_graphic_swap_buffers_(result, ...) mug_graphic_swap_buffers(mug_global_context, result, __VA_ARGS__)

					// @DOCLINE ### Update graphic

						// @DOCLINE Once the graphic has been presented, before the next frame potentially starts, the graphic's state needs to be internally updated; this is performed with the function `mug_graphic_update`, defined below: @NLNT
						MUDEF void mug_graphic_update(mugContext* context, mugResult* result, muGraphic gfx, float target_fps);

						// @DOCLINE The parameter `target_fps` specifies the desired amount of frames that the user wants to be processed each second, given as a rate of frames per second. `mug_graphic_update` accomplishes this by sleeping for the time required to reach this desired FPS, keeping track internally of the approximate time it has taken for each frame to be processed. This does mean that no compensation is performed if the approximate time is at or above the time needed to reach the desired FPS rate. This functionality is not performed if `target_fps` is less than or equal to `0.f`.

						// @DOCLINE > The macro `mu_graphic_update` is the non-result-checking equivalent, and the macro `mu_graphic_update_` is the result-checking equivalent.
						#define mu_graphic_update(...) mug_graphic_update(mug_global_context, &mug_global_context->result, __VA_ARGS__)
						#define mu_graphic_update_(result, ...) mug_graphic_update(mug_global_context, result, __VA_ARGS__)

			// @DOCLINE # Objects

				// @DOCLINE An ***object*** in mug (commonly called a "gobject" in the API) is something that is rendered to the screen. Its ***type*** defines what type of object it is, such as a triangle object.

				// @DOCLINE ## Object types

					typedef uint16_m mugObjectType;

					// @DOCLINE All object types defined by mug are represented by the type `mugObjectType` (typedef for `uint16_m`). It has the following defined values:

					// Inner note: these macro values must start from 1 and range to the last value with no gaps
					// Changing around the values is OK

					// @DOCLINE * `MUG_OBJECT_POINT` - a [point](#point).
					#define MUG_OBJECT_POINT 1

					// @DOCLINE * `MUG_OBJECT_LINE` - a [line](#line).
					#define MUG_OBJECT_LINE 2

					// @DOCLINE * `MUG_OBJECT_TRIANGLE` - a [triangle](#triangle).
					#define MUG_OBJECT_TRIANGLE 3

					// @DOCLINE * `MUG_OBJECT_RECT` - a [rect](#rect).
					#define MUG_OBJECT_RECT 4

					// @DOCLINE * `MUG_OBJECT_CIRCLE` - a [circle](#circle).
					#define MUG_OBJECT_CIRCLE 5

					// @DOCLINE * `MUG_OBJECT_SQUIRCLE` - a [squircle](#squircle).
					#define MUG_OBJECT_SQUIRCLE 6

					// @DOCLINE * `MUG_OBJECT_ROUND_RECT` - a [round rect](#round-rect).
					#define MUG_OBJECT_ROUND_RECT 7

					// @DOCLINE * `MUG_OBJECT_TEXTURE_2D` - a [two-dimensional texture rect](#2d-texture-rect).
					#define MUG_OBJECT_TEXTURE_2D 8

					// @DOCLINE * `MUG_OBJECT_TEXTURE_2D_ARRAY` - a [two-dimensional texture array rect](#2d-texture-array-rect).
					#define MUG_OBJECT_TEXTURE_2D_ARRAY 9

					#define MUG_OBJECT_FIRST MUG_OBJECT_POINT
					#define MUG_OBJECT_LAST MUG_OBJECT_TEXTURE_2D_ARRAY

				// @DOCLINE ## Load object type

					// @DOCLINE The ability to render a certain object type can be pre-loaded via the function `mug_gobject_load`, defined below: @NLNT
					MUDEF void mug_gobject_load(mugContext* context, mugResult* result, muGraphic gfx, mugObjectType obj_type);

					// @DOCLINE Object types are also loaded automatically when an object buffer is created with the type, so this function doesn't need to be called. This function just gives a successful result if the object type was already loaded.

					// @DOCLINE > The macro `mu_gobject_load` is the non-result-checking equivalent, and the macro `mu_gobject_load_` is the result-checking equivalent.
					#define mu_gobject_load(...) mug_gobject_load(mug_global_context, &mug_global_context->result, __VA_ARGS__)
					#define mu_gobject_load_(result, ...) mug_gobject_load(mug_global_context, result, __VA_ARGS__)

				// @DOCLINE ## Deload object type

					// @DOCLINE No object types are automatically loaded upon context creation. However, once an object type is loaded in mug, rather via an explicit call to `mug_gobject_load` or automatically loaded via the creation of an object buffer of the given type, it is never automatically deloaded until the graphic is destroyed. An object type can be manually deloaded via the function `mug_gobject_deload`, defined below: @NLNT
					MUDEF void mug_gobject_deload(mugContext* context, muGraphic gfx, mugObjectType obj_type);

					// @DOCLINE Calling this function with an object type who has currently active buffers will result in undefined behavior.

					// @DOCLINE > The macro `mu_gobject_deload` is the non-result-checking equivalent.
					#define mu_gobject_deload(...) mug_gobject_deload(mug_global_context, __VA_ARGS__)

				// @DOCLINE ## Object type modifiers

					typedef uint8_m mugObjectMod;

					// @DOCLINE There are certain attributes about each object type can be universally (per mug context) changed for each object of that given type. These attributes are represented by the type `mugObjectMod` (typedef for `uint8_m`), and has the following defined values:

					// @DOCLINE * `MUG_OBJECT_ADD_POS` - an array of three floats representing an x-, y-, and z-offset (respectively) added to the position of every object of a given type. The default values of all three offsets are 0.
					#define MUG_OBJECT_ADD_POS 1
					// @DOCLINE * `MUG_OBJECT_MUL_POS` - an array of three floats representing an x-, y-, and z-scale (respectively) multiplied to the position of every object of a given type. The default values of all three scales are 1.
					#define MUG_OBJECT_MUL_POS 2
					// @DOCLINE * `MUG_OBJECT_ADD_COL` - an array of four floats representing an r-, g-, b-, and a-offset (respectively) added to the color channels of every object of a given type. The default values of all four offsets are 0.
					#define MUG_OBJECT_ADD_COL 3
					// @DOCLINE * `MUG_OBJECT_MUL_COL` - an array of four floats representing an r-, g-, b-, and a-scale (respectively) multiplied to the color channels of every object of a given type. The default values of all four scales are 1.
					#define MUG_OBJECT_MUL_COL 4

					// @DOCLINE Multiplication is performed first, followed by addition. For objects that use non-polygonal borders (those being circles), the math for the color of each pixel is performed first, setting the alpha to 0 (or some intermediary between that and the given alpha value) if the pixel is not within the object, followed by the modification. This could be used to flip the alpha result (via setting the multipler to -1 and the adder to 1, thus inversing the alpha value), inverting the object's borders.

					// @DOCLINE ### Set object type modifier

						// @DOCLINE The values of an object type modifier can be modified using the function `mug_gobject_mod`, defined below: @NLNT
						MUDEF void mug_gobject_mod(mugContext* context, muGraphic gfx, mugObjectType type, mugObjectMod mod, float* data);

						// @DOCLINE This function must only be called on an object type that is currently loaded. Once an object type is deloaded and loaded again, the values of an attribute go back to their defaults.

						// @DOCLINE > The macro `mu_gobject_mod` is the non-result-checking equivalent.
						#define mu_gobject_mod(...) mug_gobject_mod(mug_global_context, __VA_ARGS__)

				// @DOCLINE ## Point

					// @DOCLINE A "point" in mug is a single-pixel point. Its respective struct is `mugPoint`, and has the following members:

					struct mugPoint {
						// @DOCLINE * `@NLFT pos[3]` - the position of the point, with indexes 0, 1, and 2 being the x-, y-, and z-coordinates respectively. All x- and y-coordiantes of a point visible on the graphic range between (0) and (the width of the graphic minus 1). All z-coordinates of a point should range between 0 and 1; any other value is not guaranteed to render properly. A pixel with a z-coordinate greater than or equal to the pixel rendered beforehand will draw over it.
						float pos[3];
						// @DOCLINE * `@NLFT col[4]` - the colors of the point, with indexes 0, 1, 2, and 3 being the red, green, blue, and alpha channels respectively. All channels' values should range between 0 and 1; any other value is not guaranteed to render properly. A value of 0 means none of the channel, and a value of 1 means the maximum intensity of the channel.
						float col[4];
					};
					typedef struct mugPoint mugPoint;

				// @DOCLINE ## Line

					// @DOCLINE A "line" in mug is a single-pixel line. Its respective struct is `mugLine`, and has the following members:

					struct mugLine {
						// @DOCLINE * `@NLFT points[2]` - the two points defining the line.
						mugPoint points[2];
					};
					typedef struct mugLine mugLine;

				// @DOCLINE ## Triangle

					// @DOCLINE A "triangle" in mug is a filled-in triangle defined by three points connected to each other. Its respective struct is `mugTriangle`, and has the following members:

					struct mugTriangle {
						// @DOCLINE * `@NLFT points[3]` - the three points defining the triangle.
						mugPoint points[3];
					};
					typedef struct mugTriangle mugTriangle;

				// @DOCLINE ## Rect

					// @DOCLINE A "rect" in mug is a filled-in rectangle defined by a center point, width/height, and rotation around the center point. Its respective struct is `mugRect`, and has the following members:

					struct mugRect {
						// @DOCLINE * `@NLFT center` - the center point of the rect. The point's color determines the color of the rect.
						mugPoint center;
						// @DOCLINE * `@NLFT dim[2]` - the dimensions of the rect, in width (`dim[0]`) and height (`dim[1]`).
						float dim[2];
						// @DOCLINE * `@NLFT rot` - the rotation of the rect around the center point, in radians.
						float rot;
					};
					typedef struct mugRect mugRect;

				// @DOCLINE ## Circle

					// @DOCLINE A "circle" in mug is a filled-in circle defined by a center point and radius. Its respective struct is `mugCircle`, and has the following members:

					struct mugCircle {
						// @DOCLINE * `@NLFT center` - the center point of the circle. The point's color determines the color of the circle.
						mugPoint center;
						// @DOCLINE * `@NLFT radius` - the radius of the circle.
						float radius;
					};
					typedef struct mugCircle mugCircle;

				// @DOCLINE ## Squircle

					// @DOCLINE A "squircle" in mug is a filled-in [squircle](https://en.wikipedia.org/wiki/Squircle), defined by a center point, a radius, and an exponent. Its respective struct is `mugSquircle` and has the following members:

					struct mugSquircle {
						// @DOCLINE * `@NLFT center` - the center point of the squircle. The point's color determines the color of the squircle.
						mugPoint center;
						// @DOCLINE * `@NLFT rot` - the rotation of the squircle around the center, in radians.
						float rot;
						// @DOCLINE * `@NLFT radius` - the radius of the squircle.
						float radius;
						// @DOCLINE * `@NLFT exp` - the exponent used for the squircle equation (`|x-a|**exp + |y-b|**exp = |radius|**exp`). The higher the number, the sharper the edges, and vice versa. Any value for this member below 2 is not guaranteed to render correctly.
						float exp;
					};
					typedef struct mugSquircle mugSquircle;

				// @DOCLINE ## Round rect

					// @DOCLINE A "round rect" in mug is a filled-in rectangle with perfectly rounded edges, defined by a center point, dimensions, and the radius of the rounded edges. Its respective struct is `mugRoundRect` and has the following members:

					struct mugRoundRect {
						// @DOCLINE * `@NLFT center` - the center point of the round rect. The point's color determines the color of the round rect.
						mugPoint center;
						// @DOCLINE * `@NLFT dim[2]` - the dimensions of the round rect (`dim[0]` is width and `dim[1]` is height).
						float dim[2];
						// @DOCLINE * `@NLFT rot` - the rotation of the round rect around the center point, in radians.
						float rot;
						// @DOCLINE * `@NLFT radius `- the radius of the rounded edges.
						float radius;
					};
					typedef struct mugRoundRect mugRoundRect;

				// @DOCLINE ## 2D texture rect

					// @DOCLINE A "2D texture rect" in mug is a rect texture object, acting as a rect with a two-dimensional texture rendered over it. Its respective struct is `mug2DTextureRect`, and has the following members:

					struct mug2DTextureRect {
						// @DOCLINE * `@NLFT center` - the center of the rect. The point's color determines the color of the rect, which is multiplied with the color values of the texture.
						mugPoint center;
						// @DOCLINE * `@NLFT dim[2]` - the dimensions of the rect, in width (`dim[0]`) and height (`dim[1]`).
						float dim[2];
						// @DOCLINE * `@NLFT rot` - the rotation of the rect around the center point, in radians.
						float rot;
						// @DOCLINE * `@NLFT tex_pos[2]` - the position of the [texture cutout](#texture-cutout).
						float tex_pos[2];
						// @DOCLINE * `@NLFT tex_dim[2]` - the dimensions of the [texture cutout](#texture-cutout).
						float tex_dim[2];
					};
					typedef struct mug2DTextureRect mug2DTextureRect;

					// @DOCLINE The texture that gets rendered onto the rect is the [buffer's texture](#object-buffer-texture). The type of the texture must be `MUG_TEXTURE_2D`.

				// @DOCLINE ## 2D texture array rect

					// @DOCLINE A "2D texture array rect" in mug is a rect texture object, acting as a rect with a two-dimension texture array rendered over it. Its respective struct is `mug2DTextureArrayRect`, and has the following members:

					struct mug2DTextureArrayRect {
						// @DOCLINE * `@NLFT center` - the center of the rect. The point's color determines the color of the rect, which is multiplied with the color values of the texture.
						mugPoint center;
						// @DOCLINE * `@NLFT dim[2]` - the dimensions of the rect, in width (`dim[0]`) and height (`dim[1]`).
						float dim[2];
						// @DOCLINE * `@NLFT rot` - the rotation of the rect around the center point, in radians.
						float rot;
						// @DOCLINE * `@NLFT tex_pos[3]` - the position of the [texture cutout](#texture-cutout).
						float tex_pos[3];
						// @DOCLINE * `@NLFT tex_dim[2]` - the dimensions of the [texture cutout](#texture-cutout).
						float tex_dim[2];
					};
					typedef struct mug2DTextureArrayRect mug2DTextureArrayRect;

					// @DOCLINE The texture that gets rendered onto the rect is the [buffer's texture](#object-buffer-texture). The type of the texture must be `MUG_TEXTURE_2D_ARRAY`.

			// @DOCLINE # Object buffers

				typedef void* mugObjects;

				// @DOCLINE Objects are collectively stored, updated, and rendered in buffers (referred to as "gobjects" in the API), which are stored for rendering. The type for an object buffer is `mugObjects` (typedef for `void*`).

				// @DOCLINE ## Create object buffer

					// @DOCLINE An object buffer can be created via the function `mug_gobjects_create`, defined below: @NLNT
					MUDEF mugObjects mug_gobjects_create(mugContext* context, mugResult* result, muGraphic gfx, mugObjectType type, uint32_m obj_count, void* objs);

					// @DOCLINE If `objs` is 0, the buffer is simply allocated, but filled with undefined data. If `objs` is not 0, it should be a pointer to an array of objects whose type matches the object type indicated by `type`, and whose length (in objects) should match `obj_count`.

					// @DOCLINE Every object buffer that is created must be destroyed before the graphic that was used to create it is destroyed.

					// @DOCLINE > The macro `mu_gobjects_create` is the non-result-checking equivalent, and the macro `mu_gobjects_create_` is the result-checking equivalent.
					#define mu_gobjects_create(...) mug_gobjects_create(mug_global_context, &mug_global_context->result, __VA_ARGS__)
					#define mu_gobjects_create_(result, ...) mug_gobjects_create(mug_global_context, result, __VA_ARGS__)

				// @DOCLINE ## Destroy object buffer

					// @DOCLINE Every successfully created object buffer must be destroyed at some point with the function `mug_gobjects_destroy`, defined below: @NLNT
					MUDEF mugObjects mug_gobjects_destroy(mugContext* context, muGraphic gfx, mugObjects objs);

					// @DOCLINE If any portion of the object buffer has been rendered during the current frame, it should not be destroyed until the entire frame has passed (AKA after `mug_graphic_update` has been called). This function returns 0.

					// @DOCLINE > The macro `mu_gobjects_destroy` is the non-result-checking equivalent.
					#define mu_gobjects_destroy(...) mug_gobjects_destroy(mug_global_context, __VA_ARGS__)

				// @DOCLINE ## Render object buffer

					// @DOCLINE An object buffer can be rendered to a graphic using the function `mug_gobjects_render`, defined below: @NLNT
					MUDEF void mug_gobjects_render(mugContext* context, mugResult* result, muGraphic gfx, mugObjects objs);

					// @DOCLINE Once an object buffer has been rendered, no fill/subfill/destroy actions should be performed on the object buffer.

					// @DOCLINE > The macro `mu_gobjects_render` is the non-result-checking equivalent, and the macro `mu_gobjects_render_` is the result-checking equivalent.
					#define mu_gobjects_render(...) mug_gobjects_render(mug_global_context, &mug_global_context->result, __VA_ARGS__)
					#define mu_gobjects_render_(result, ...) mug_gobjects_render(mug_global_context, result, __VA_ARGS__)

				// @DOCLINE ## Subrender object buffer

					// @DOCLINE An object buffer can be rendered like normal, except for with a specified offset (by an amount of objects) into the buffer and a specified length (also in amount of objects) using the function `mug_gobjects_subrender`, defined below: @NLNT
					MUDEF void mug_gobjects_subrender(mugContext* context, mugResult* result, muGraphic gfx, mugObjects objs, uint32_m offset, uint32_m count);

					// @DOCLINE Rules about buffer rendering from `mug_gobjects_render` apply here as well.

					// @DOCLINE > The macro `mu_gobjects_subrender` is the non-result-checking equivalent, and the macro `mu_gobjects_subrender_` is the result-checking equivalent.
					#define mu_gobjects_subrender(...) mug_gobjects_subrender(mug_global_context, &mug_global_context->result, __VA_ARGS__)
					#define mu_gobjects_subrender_(result, ...) mug_gobjects_subrender(mug_global_context, result, __VA_ARGS__)

				// @DOCLINE ## Fill

					// @DOCLINE An object buffer's contents can be replaced using the function `mug_gobjects_fill`, defined below: @NLNT
					MUDEF void mug_gobjects_fill(mugContext* context, mugResult* result, muGraphic gfx, mugObjects objs, void* data);

					// @DOCLINE `data` must be a valid pointer to an array of objects whose type matches the object type of the object buffer, and whose length matches the object count of the buffer.

					// @DOCLINE > The macro `mu_gobjects_fill` is the non-result-checking equivalent, and the macro `mu_gobjects_fill_` is the result-checking equivalent.
					#define mu_gobjects_fill(...) mug_gobjects_fill(mug_global_context, &mug_global_context->result, __VA_ARGS__)
					#define mu_gobjects_fill_(result, ...) mug_gobjects_fill(mug_global_context, result, __VA_ARGS__)

				// @DOCLINE ## Subfill

					// @DOCLINE An object buffer's contents can be partially filled via specifying an offset and length (both in units of objects) into the buffer using the function `mug_gobjects_subfill`, defined below: @NLNT
					MUDEF void mug_gobjects_subfill(mugContext* context, mugResult* result, muGraphic gfx, mugObjects objs, uint32_m offset, uint32_m count, void* data);

					// @DOCLINE `data` must be a valid pointer to an array of objects whose type matches the object type of the object buffer, and whose length matches the length specified by `count`.

					// @DOCLINE > The macro `mu_gobjects_subfill` is the non-result-checking equivalent, and the macro `mu_gobjects_subfill_` is the result-checking equivalent.
					#define mu_gobjects_subfill(...) mug_gobjects_subfill(mug_global_context, &mug_global_context->result, __VA_ARGS__)
					#define mu_gobjects_subfill_(result, ...) mug_gobjects_subfill(mug_global_context, result, __VA_ARGS__)

				// @DOCLINE ## Object buffer texture

					// @DOCLINE For object buffers that rely on rendering a texture, their texture can be set via the function `mug_gobjects_texture`, defined below: @NLNT
					MUDEF void mug_gobjects_texture(mugContext* context, muGraphic gfx, mugObjects obj, mugTexture tex);

					// @DOCLINE Once the object buffer's texture has been set, all subsequent calls to render/subrender the buffer will be rendered with the given texture until this function is called with another texture.

					// @DOCLINE Calling this function on a buffer whose object type does not use texture rendering will result in undefined behavior. Calling this function with a texture type that doesn't match the intended texture type of the object type (for example, setting a two-dimensional texture rect buffer's texture as a three-dimensional texture) will result in undefined behavior.

					// @DOCLINE > The macro `mu_gobjects_texture` is the non-result-checking equivalent.
					#define mu_gobjects_texture(...) mug_gobjects_texture(mug_global_context, __VA_ARGS__)

			// @DOCLINE # Texture

				// @DOCLINE A "texture" in mug is a pixel bitmap stored for rendering (often called a "gtexture" in the API), and is used in rendering to draw images to the screen using a texture object buffer. Its respective type is `mugTexture` (typedef for `void*`).

				// @DOCLINE ## Texture types

					typedef uint16_m mugTextureType;

					// @DOCLINE There are different ways that a texture's pixel data can be layed out. For this, there are different "types" of textures, represented by the type `mugTextureType` (typedef for `uint16_m`). It has the following defined values:

					// @DOCLINE * `MUG_TEXTURE_2D` - a two-dimensional texture.
					#define MUG_TEXTURE_2D 0

					// @DOCLINE * `MUG_TEXTURE_2D_ARRAY` - a two-dimensional texture array.
					#define MUG_TEXTURE_2D_ARRAY 1

					// @DOCLINE All types expect pixel data ordered left-to-right and top-to-bottom. All textures that have more than two dimensions are ordered starting from layer 0 incrementally.

					// @DOCLINE Texture arrays act as multiple 2D textures stored one after the other, with the first one specified being referenced as layer/depth 0.

				// @DOCLINE ## Texture format

					// @DOCLINE A texture format specifies how the raw byte data of the pixels should be interpreted into bitmap data values, represented by the type `mugTextureFormat` (typedef for `uint16_m`). It has the following defined values:
					typedef uint16_m mugTextureFormat;

					// @DOCLINE * `MUG_TEXTURE_U8_R` - red-channel unsigned 8-bit integer normalized texture format.
					#define MUG_TEXTURE_U8_R 0
					// @DOCLINE * `MUG_TEXTURE_U8_RGB` - red-green-blue-channel unsigned 8-bit integer normalized texture format.
					#define MUG_TEXTURE_U8_RGB 1
					// @DOCLINE * `MUG_TEXTURE_U8_RGBA` - red-green-blue-alpha-channel unsigned 8-bit integer normalized texture format.
					#define MUG_TEXTURE_U8_RGBA 2

				// @DOCLINE ## Texture wrapping

					typedef uint8_m mugTextureWrapping;

					// @DOCLINE When a [texture cutout](#texture-cutout) generates texture coordinates out of range (thus rendering parts of the texture that aren't defined), wrapping occurs, which helps to give valid data when this occurs. The behavior of the wrapping is customizable via the type `mugTextureWrapping` (typedef for `uint8_m`), which has the following defined values:

					// @DOCLINE * `MUG_TEXTURE_REPEAT` - the texture repeats when out-of-range texture coordinate values are given.
					#define MUG_TEXTURE_REPEAT 0

					// @DOCLINE * `MUG_TEXTURE_MIRRORED_REPEAT` - the texture repeats when out-of-range texture coordinate values are given, mirroring the image for every time the texture coordinates wrap around.
					#define MUG_TEXTURE_MIRRORED_REPEAT 1

					// @DOCLINE * `MUG_TEXTURE_CLAMP` - the texture's coordinates are clamped into a valid range. This has the effect of repeating the same coordinate value for out-of-range coordinates on a given axis, which visually stretches the single-pixel boundaries of the image indefinitely.
					#define MUG_TEXTURE_CLAMP 2

				// @DOCLINE ## Texture filtering

					typedef uint8_m mugTextureFiltering;

					// @DOCLINE mug allows the user to specify the [texture filtering](https://en.wikipedia.org/wiki/Texture_filtering) of a given texture, using the type `mugTextureFiltering` (typedef for `uint8_m`), which has the folowing defined values:

					// @DOCLINE * `MUG_TEXTURE_NEAREST` - [nearest neighbor interpolation](https://en.wikipedia.org/wiki/Nearest-neighbor_interpolation).
					#define MUG_TEXTURE_NEAREST 0

					// @DOCLINE * `MUG_TEXTURE_BILINEAR` - [bilinear interpolation](https://en.wikipedia.org/wiki/Bilinear_interpolation).
					#define MUG_TEXTURE_BILINEAR 1

				// @DOCLINE ## Texture info

					// @DOCLINE Information about how a texture is stored within mug is represented with the struct `mugTextureInfo`. It has the following members:

					struct mugTextureInfo {
						// @DOCLINE * `@NLFT type` - the [texture type](#texture-types).
						mugTextureType type;
						// @DOCLINE * `@NLFT format` - the [texture format](#texture-format).
						mugTextureFormat format;
						// @DOCLINE * `@NLFT wrapping[2]` - the [texture wrapping](#texture-wrapping) on the x (`wrapping[0]`) and y (`wrapping[1]`).
						mugTextureWrapping wrapping[2];
						// @DOCLINE * `@NLFT filtering[2]` - the [texture filtering](#texture-filtering) when upscaling (`filtering[0]`) and downscaling (`filtering[1]`).
						mugTextureFiltering filtering[2];
					};
					typedef struct mugTextureInfo mugTextureInfo;

				// @DOCLINE ## Texture cutout

					// @DOCLINE When a texture is rendered onto a rect, exactly what part of the texture is being mapped needs to be specified, which is detailed in the form of a "cutout". The cutout takes a portion of the texture and renders only that portion of the texture over the rect. The texture cutout is specified in texture coordinates, ranging from a top-left origin of (0,0) to bottom-right (1, 1). The cutout itself is defined by a *position* and *dimensions*.

					// @DOCLINE The position is made up of an x-, y-, and sometimes a z-coordinate. The x- and y-coordinates specify the top-leftest point of the cutout in texture coordinates. The z-coordinate is only relevant (and respectively defined) if the texture is a texture array, in which case the z-coordinate specifies the index into the texture array to render.

					// @DOCLINE The dimensions are made up of a width and height value, which specify how far the texture cutout reaches from the position in texture coordinates. Negative values will work, and will flip the texture's appearance correspondingly.

					// @DOCLINE Any cutout that will result in the rendering of texture coordinates outside of the valid texture coordinates range ([0,0], [1,1]) will cause wrapping, to which behavior is defined by the [texture wrapping](#texture-wrapping) of the texture currently being rendered. Any z-coordinate values for texture arrays that aren't a perfect integer value or who are out of range in the texture array's depth

				// @DOCLINE ## Texture creation

					// @DOCLINE To create a handle to the texture for rendering, the function `mug_gtexture_create` is used, defined below: @NLNT
					MUDEF mugTexture mug_gtexture_create(mugContext* context, mugResult* result, muGraphic gfx, mugTextureInfo* info, uint32_m* dim, muByte* data);

					// @DOCLINE `dim`'s length is dictated by the dimensions of the format used for the texture (specified in `info`). For example, if the texture is `MUG_TEXTURE_2D`, `dim` is expected to be an array of 2 values, but if the texture is `MUG_TEXTURE_2D_ARRAY`, `dim` is expected to be an array of 3 values, with `dim[2]` corresponding to the depth of the texture array (AKA the texture array length).

					// @DOCLINE The texture width and height's minimum supported value is 2048. The texture depth's minimum supported value is 256.

					// @DOCLINE Once this function is finished, the pointer to the data (`data`) is no longer held onto.

					// @DOCLINE Every successfully created texture must be [destroyed](#texture-destruction) at some point.

					// @DOCLINE > The macro `mu_gtexture_create` is the non-result-checking equivalent, and the macro `mu_gtexture_create_` is the result-checking equivalent.
					#define mu_gtexture_create(...) mug_gtexture_create(mug_global_context, &mug_global_context->result, __VA_ARGS__)
					#define mu_gtexture_create_(result, ...) mug_gtexture_create(mug_global_context, result, __VA_ARGS__)

				// @DOCLINE ## Texture destruction

					// @DOCLINE Once a texture is successfully created, the function `mug_gtexture_destroy` must be called on it at some point, defined below: @NLNT
					MUDEF mugTexture mug_gtexture_destroy(mugContext* context, muGraphic gfx, mugTexture tex);

					// @DOCLINE > The macro `mu_gtexture_destroy` is the non-result-checking equivalent.
					#define mu_gtexture_destroy(...) mug_gtexture_destroy(mug_global_context, __VA_ARGS__)

			// @DOCLINE # Min/Max supported values

				// @DOCLINE mug has several minimums and maximums in regards to several values, such as a texture's width and height. mug's [minimum values](#minimum-supported-values) are constants that are guaranteed to be supported on any system that runs mug successfully. mug's [maximum values](#maximum-supported-values) can differ from device to device, and are retrieved at runtime.

				// @DOCLINE ## Minimum supported values

					// @DOCLINE mug defines several macros for minimum values for various parts of mug. If mug is running successfully, these minimums are guaranteed for the system.

					// @DOCLINE ### Texture minimum supported values

						// @DOCLINE The following macros for minimum values regarding textures are defined:

						// @DOCLINE * `MUG_MIN_TEXTURE_WIDTH_HEIGHT` - the minimum texture width and height. This value is 1024.
						#define MUG_MIN_TEXTURE_WIDTH_HEIGHT 1024

						// @DOCLINE * `MUG_MIN_TEXTURE_DEPTH` - the minimum texture depth (for texture arrays). This value is 256.
						#define MUG_MIN_TEXTURE_DEPTH 256

				// @DOCLINE ## Maximum supported values

					typedef uint16_m mugMax;

					// @DOCLINE mug is able to retrieve maximum values for the current device using the function `mug_max`, defined below: @NLNT
					MUDEF uint32_m mug_max(mugContext* context, muGraphic gfx, mugMax max);

					// @DOCLINE `max` is a value specifying what maximum is being requested; values for the type `mugMax` are defined below. If `max` is an unrecognized value, 0 is returned.

				// @DOCLINE ### Texture maximum supported values
					// 1 -> 512

					// @DOCLINE The following macros for maximum values (for the type `mugMax`) regarding textures are defined:

					// @DOCLINE * `MUG_MAX_TEXTURE_WIDTH_HEIGHT` - the maximum texture width and height.
					#define MUG_MAX_TEXTURE_WIDTH_HEIGHT 1

					// @DOCLINE * `MUG_MAX_TEXTURE_DEPTH` - the maximum texture depth (for texture arrays).
					#define MUG_MAX_TEXTURE_DEPTH 2

			// @DOCLINE # Result

				// @DOCLINE The type `mugResult` (typedef for `uint16_m`) is used to represent how a task in mug went. It has the following defined values:

				// == MUG_... 0-4095 ==

				// @DOCLINE * `MUG_SUCCESS` - the task succeeded; real value 0.
				#define MUG_SUCCESS 0

				// @DOCLINE * `MUG_FAILED_MALLOC` - `mu_malloc` returned a failure value, and the task was unable to be completed.
				#define MUG_FAILED_MALLOC 1
				// @DOCLINE * `MUG_FAILED_REALLOC` - `mu_realloc` returned a failure value, and the task was unable to be completed.
				#define MUG_FAILED_REALLOC 2

				// @DOCLINE * `MUG_UNKNOWN_GRAPHIC_SYSTEM` - a `muGraphicSystem` value given by the user was unrecognized. This could happen because support for the requested graphics API was not defined by the user, such as passing `MU_GRAPHIC_OPENGL` without defining `MU_SUPPORT_OPENGL`.
				#define MUG_UNKNOWN_GRAPHIC_SYSTEM 3

				// @DOCLINE * `MUG_UNKNOWN_OBJECT_TYPE` - a `mugObjectType` value given by the user was unrecognized.
				#define MUG_UNKNOWN_OBJECT_TYPE 4
				// @DOCLINE * `MUG_UNKNOWN_OBJECT_MOD` - a `mugObjectMod` value given by the user was unrecognized.
				#define MUG_UNKNOWN_OBJECT_MOD 5

				// == MUG_MUCOSA_... 4096-8191 ==

				// @DOCLINE * `MUG_MUCOSA_...` - a muCOSA function was called, which gave a non-success result value, which has been converted to a `mugResult` equivalent. There is a `mugResult` equivalent for any `muCOSAResult` value (besides `MUCOSA_SUCCESS`), and the conditions of the given `muCOSAResult` value apply based on the muCOSA documentation. Note that the value of the muCOSA-equivalent does not necessarily match the value of the mug version.
					#define MUG_MUCOSA_FAILED_NULL_WINDOW_SYSTEM 4096
					#define MUG_MUCOSA_FAILED_MALLOC 4097
					#define MUG_MUCOSA_FAILED_UNKNOWN_WINDOW_ATTRIB 4098
					#define MUG_MUCOSA_FAILED_REALLOC 4099
					#define MUG_MUCOSA_FAILED_UNKNOWN_GRAPHICS_API 4100
					#define MUG_MUCOSA_FAILED_UNSUPPORTED_GRAPHICS_API 4101
					#define MUG_MUCOSA_WIN32_FAILED_CONVERT_UTF8_TO_WCHAR 4102
					#define MUG_MUCOSA_WIN32_FAILED_REGISTER_WINDOW_CLASS 4103
					#define MUG_MUCOSA_WIN32_FAILED_CREATE_WINDOW 4104
					#define MUG_MUCOSA_WIN32_FAILED_GET_WINDOW_ATTRIB 4105
					#define MUG_MUCOSA_WIN32_FAILED_SET_WINDOW_ATTRIB 4106
					#define MUG_MUCOSA_WIN32_FAILED_REGISTER_DUMMY_WGL_WINDOW_CLASS 4107
					#define MUG_MUCOSA_WIN32_FAILED_CREATE_DUMMY_WGL_WINDOW 4108
					#define MUG_MUCOSA_WIN32_FAILED_GET_DUMMY_WGL_PIXEL_FORMAT 4109
					#define MUG_MUCOSA_WIN32_FAILED_DESCRIBE_DUMMY_WGL_PIXEL_FORMAT 4110
					#define MUG_MUCOSA_WIN32_FAILED_SET_DUMMY_WGL_PIXEL_FORMAT 4111
					#define MUG_MUCOSA_WIN32_FAILED_CREATE_DUMMY_WGL_CONTEXT 4112
					#define MUG_MUCOSA_WIN32_FAILED_BIND_DUMMY_WGL_CONTEXT 4113
					#define MUG_MUCOSA_WIN32_FAILED_FIND_WGL_CREATE_CONTEXT_ATTRIBS 4114
					#define MUG_MUCOSA_WIN32_FAILED_FIND_WGL_CHOOSE_PIXEL_FORMAT 4115
					#define MUG_MUCOSA_WIN32_FAILED_CHOOSE_WGL_PIXEL_FORMAT 4116
					#define MUG_MUCOSA_WIN32_FAILED_GET_WGL_PIXEL_FORMAT 4117
					#define MUG_MUCOSA_WIN32_FAILED_DESCRIBE_WGL_PIXEL_FORMAT 4118
					#define MUG_MUCOSA_WIN32_FAILED_SET_WGL_PIXEL_FORMAT 4119
					#define MUG_MUCOSA_WIN32_FAILED_CREATE_WGL_CONTEXT 4120
					#define MUG_MUCOSA_WIN32_FAILED_SET_WGL_CONTEXT 4121
					#define MUG_MUCOSA_WIN32_FAILED_SWAP_WGL_BUFFERS 4122
					#define MUG_MUCOSA_WIN32_FAILED_FIND_WGL_FUNCTION 4123
					#define MUG_MUCOSA_WIN32_FAILED_ASSOCIATE_IMM 4124
					#define MUG_MUCOSA_WIN32_FAILED_SET_COMPOSITION_WINDOW_POSITION 4125
					#define MUG_MUCOSA_WIN32_FAILED_HOLD_CLIPBOARD 4126
					#define MUG_MUCOSA_WIN32_FAILED_GET_CLIPBOARD_DATA 4127
					#define MUG_MUCOSA_WIN32_FAILED_CONVERT_CLIPBOARD_DATA_FORMAT 4128
					#define MUG_MUCOSA_WIN32_FAILED_ALLOCATE_CLIPBOARD_DATA 4129
					#define MUG_MUCOSA_WIN32_FAILED_SET_CLIPBOARD_DATA 4130

				// == MUG_GL_... 8192-12287 ==

				// @DOCLINE * `MUG_GL_FAILED_LOAD` - the required OpenGL functionality failed to load from the function call to `gladLoadGL`.
				#define MUG_GL_FAILED_LOAD 8192

				// @DOCLINE * `MUG_GL_FAILED_COMPILE_VERTEX_SHADER` - a vertex shader in OpenGL necessary to perform the task failed to compile.
				#define MUG_GL_FAILED_COMPILE_VERTEX_SHADER 8193
				// @DOCLINE * `MUG_GL_FAILED_COMPILE_FRAGMENT_SHADER` - a fragment shader in OpenGL necessary to perform the task failed to compile.
				#define MUG_GL_FAILED_COMPILE_FRAGMENT_SHADER 8194
				// @DOCLINE * `MUG_GL_FAILED_LINK_SHADERS` - shaders necessary to be linked to a shader program in OpenGL to perform the task failed to link.
				#define MUG_GL_FAILED_LINK_SHADERS 8195

				// @DOCLINE * `MUG_GL_FAILED_ALLOCATE_BUFFER` - an internal OpenGL buffer needed to perform the task was unable to be allocated to the correct size.
				#define MUG_GL_FAILED_ALLOCATE_BUFFER 8196
				// @DOCLINE * `MUG_GL_FAILED_CREATE_BUFFER` - a necessary call to create an OpenGL buffer failed.
				#define MUG_GL_FAILED_CREATE_BUFFER 8197
				// @DOCLINE * `MUG_GL_FAILED_CREATE_VERTEX_ARRAY` - a necessary call to create an OpenGL vertex array failed.
				#define MUG_GL_FAILED_CREATE_VERTEX_ARRAY 8198

				// @DOCLINE * `MUG_GL_FAILED_GENERATE_TEXTURE` - a necessary call to generate an OpenGL texture failed.
				#define MUG_GL_FAILED_GENERATE_TEXTURE 8199

				// @DOCLINE All non-success values (unless explicitly stated otherwise) mean that the function fully failed; AKA, it was "fatal", and the library continues as if the function had never been called. So, for example, if something was supposed to be allocated, but the function fatally failed, nothing was allocated.

				// @DOCLINE There are non-fatal failure values, which mean that the function still executed, but not fully to the extent that the user would expect from the function. The function `mug_result_is_fatal` returns whether or not a given result function value is fatal, defined below: @NLNT
				MUDEF muBool mug_result_is_fatal(mugResult result);

				// @DOCLINE This function also considers the fatal status of `mugResult` values representing `muCOSAResult` values.

				// @DOCLINE > This function returns `MU_TRUE` if the value of `result` is invalid. It returns `MU_FALSE` if `result` is equal to `MUG_SUCCESS`.

				// @DOCLINE ## Result names
					#ifdef MUG_NAMES

					// @DOCLINE The name function `mug_result_get_name` returns a `const char*` representation of the given result value (for example, `MUG_SUCCESS` returns "MUG_SUCCESS"), defined below: @NLNT
					MUDEF const char* mug_result_get_name(mugResult result);

					// @DOCLINE It will return "MU_UNKNOWN" in the case that `result` is an invalid result value.

					// @DOCLINE > This function is a "name" function, and therefore is only defined if `MUG_NAMES` is also defined by the user.
					#endif

				// @DOCLINE ## muCOSA-mug result to muCOSA result

					// @DOCLINE The function `mug_to_muCOSA_result` returns the muCOSA-equivalent of a mug result that represents a `muCOSAResult` value (for example, `MUG_MUCOSA_FAILED_MALLOC` returns `MUCOSA_FAILED_MALLOC`), defined below: @NLNT
					MUDEF muCOSAResult mug_to_muCOSA_result(mugResult result);

					// @DOCLINE This function returns `MUG_SUCCESS`/`MUCOSA_SUCCESS` (same value) if no muCOSA equivalent exists for the given `mugResult` value.

				// @DOCLINE ## muCOSA result to mug result

					// @DOCLINE The function `muCOSA_to_mug_result` returns the mug-equivalent of a muCOSA result value (for example, `MUCOSA_FAILED_MALLOC` returns `MUG_MUCOSA_FAILED_MALLOC`), defined below: @NLNT
					MUDEF mugResult muCOSA_to_mug_result(muCOSAResult result);

					// @DOCLINE This function returns `MUG_SUCCESS`/`MUCOSA_SUCCESS` (same value) if no mug equivalent exists for the given `muCOSAResult` value, rather because the given `muCOSAResult` value is invalid or is equal to `MUCOSA_SUCCESS`.

			// @DOCLINE # C standard library dependencies

				// @DOCLINE mug has several C standard library dependencies, all of which are overridable by defining them before the inclusion of its header. The following is a list of those dependencies.

				#if !defined(mu_sinf) || \
					!defined(mu_cosf)

					// @DOCLINE ## `math.h` dependencies
					#include <math.h>

					// @DOCLINE * `mu_sinf` - equivalent to `sinf`.
					#ifndef mu_sinf
						#define mu_sinf sinf
					#endif

					// @DOCLINE * `mu_cosf` - equivalent to `cosf`.
					#ifndef mu_cosf
						#define mu_cosf cosf
					#endif

				#endif /* math.h */

			MU_CPP_EXTERN_END
		#endif /* MUG_H */
	#endif /* MUSDK_MUG */
	// @ATTENTION

	// @DOCLINE ## muTrueType
	// @DOCLINE muSDK has support for muTrueType ([commit 178b8fe](https://github.com/Muukid/muTrueType/tree/178b8fe6e73c5e1c51badf35de0a0090a62bf3d5)). The macro to define it is `MUSDK_MUTT`.
	// @IGNORE
	#ifdef MUSDK_MUTT
		// Names
		#ifdef MUSDK_ALL_NAMES
			#define MUTT_NAMES
		#endif

		// Debug inclusion
		#ifdef MUSDK_INCLUDE_LIBS
			#include "muTrueType.h"
		#endif

		// Header
		#ifndef MUTT_H
			#define MUTT_H
			
			// @DOCLINE # Other library dependencies
				// @DOCLINE mutt has a dependency on:
				
				// @DOCLINE * [muUtility v2.0.1](https://github.com/Muukid/muUtility/releases/tag/v2.0.1).
				// @IGNORE
					#if !defined(MU_CHECK_VERSION_MISMATCHING) && defined(MUU_H) && \
						(MUU_VERSION_MAJOR != 2 || MUU_VERSION_MINOR != 0 || MUU_VERSION_PATCH != 1)

						#pragma message("[MUTT] muUtility's header has already been defined, but version doesn't match the version that this library is built for. This may lead to errors, warnings, or unexpected behavior. Define MU_CHECK_VERSION_MISMATCHING before this to turn off this message.")

					#endif

					#ifndef MUU_H
						#define MUU_H

						// @DOCLINE # Version
							// @DOCLINE The macros `MUU_VERSION_MAJOR`, `MUU_VERSION_MINOR`, and `MUU_VERSION_PATCH` are defined to match its respective release version, following the formatting of `MAJOR.MINOR.PATCH`.
							
							#define MUU_VERSION_MAJOR 2
							#define MUU_VERSION_MINOR 0
							#define MUU_VERSION_PATCH 1

						// @DOCLINE # `MUDEF`
							// @DOCLINE The `MUDEF` macro is used by virtually all mu libraries, and is generally added before a header-defined variable or function. Its default value is `extern`, but can be changed to `static` by defining `MU_STATIC` before the header section of muUtility is defined. Its value can also be overwritten entirely to anything else by directly defining `MUDEF`.
							
							#ifndef MUDEF
								#ifdef MU_STATIC
									#define MUDEF static
								#else
									#define MUDEF extern
								#endif
							#endif
						
						// @DOCLINE # Secure warnings
							// @DOCLINE mu libraries often use non-secure functions that will trigger warnings on certain compilers. These warnings are, to put it lightly, dumb, so muUtility defines `_CRT_SECURE_NO_WARNINGS`. However, it is not guaranteed that this definition will actually turn the warnings off, which, at that point, they have to be manually turned off by the user. This functionality can be turned off by defining `MU_SECURE_WARNINGS`.
							#if !defined(MU_SECURE_WARNINGS) && !defined(_CRT_SECURE_NO_WARNINGS)
								#define _CRT_SECURE_NO_WARNINGS
							#endif
						
						// @DOCLINE # C++ extern
							// @DOCLINE Every mu library's primary code externs "C" within the main chunks of their code; macros `MU_CPP_EXTERN_START` and `MU_CPP_EXTERN_END` are defined to make this process easier, and would read like this:
							/* @DOCBEGIN
							```
							MU_CPP_EXTERN_START

							// Library code goes here...

							MU_CPP_EXTERN_END
							```
							@DOCEND */
							#ifdef __cplusplus
								#define MU_CPP_EXTERN_START extern "C" {
								#define MU_CPP_EXTERN_END   }
							#else
								#define MU_CPP_EXTERN_START
								#define MU_CPP_EXTERN_END
							#endif
						
						MU_CPP_EXTERN_START

						// @DOCLINE # C standard library dependencies

							// @DOCLINE muUtility has several C standard library dependencies, all of which are overridable by defining them before the inclusion of the file. The following is a list of those dependencies.

							// @DOCLINE ## `stdint.h` dependencies
							#if !defined(int8_m) || \
								!defined(uint8_m) || \
								!defined(int16_m) || \
								!defined(uint16_m) || \
								!defined(int32_m) || \
								!defined(uint32_m) || \
								!defined(int64_m) || \
								!defined(uint64_m) || \
								!defined(MU_SIZE_MAX)

								#define __STDC_LIMIT_MACROS
								#define __STDC_CONSTANT_MACROS
								#include <stdint.h>
								
								// @DOCLINE * `int8_m` - equivalent to `int8_t` if `INT8_MAX` is defined; `char` if otherwise.
								#ifndef int8_m
									#ifdef INT8_MAX
										#define int8_m int8_t
									#else
										#define int8_m char
									#endif
								#endif

								// @DOCLINE * `uint8_m` - equivalent to `uint8_t` if `UINT8_MAX` is defined; `unsigned char` if otherwise.
								#ifndef uint8_m
									#ifdef UINT8_MAX
										#define uint8_m uint8_t
									#else
										#define uint8_m unsigned char
									#endif
								#endif

								// @DOCLINE * `int16_m` - equivalent to `int16_t` if `INT16_MAX` is defined; `short` if otherwise.
								#ifndef int16_m
									#ifdef INT16_MAX
										#define int16_m int16_t
									#else
										#define int16_m short
									#endif
								#endif

								// @DOCLINE * `uint16_m` - equivalent to `uint16_t` if `UINT16_MAX` is defined; `unsigned short` if otherwise.
								#ifndef uint16_m
									#ifdef UINT16_MAX
										#define uint16_m uint16_t
									#else
										#define uint16_m unsigned short
									#endif
								#endif

								// @DOCLINE * `int32_m` - equivalent to `int32_t` if `INT32_MAX` is defined; `long` if otherwise.
								#ifndef int32_m
									#ifdef INT32_MAX
										#define int32_m int32_t
									#else
										#define int32_m long
									#endif
								#endif

								// @DOCLINE * `uint32_m` - equivalent to `uint32_t` if `UINT32_MAX` is defined; `unsigned long` if otherwise.
								#ifndef uint32_m
									#ifdef UINT32_MAX
										#define uint32_m uint32_t
									#else
										#define uint32_m unsigned long
									#endif
								#endif

								// @DOCLINE * `int64_m` - equivalent to `int64_t` if `INT64_MAX` is defined; `long long` if otherwise.
								#ifndef int64_m
									#ifdef INT64_MAX
										#define int64_m int64_t
									#else
										#define int64_m long long
									#endif
								#endif

								// @DOCLINE * `uint64_m` - equivalent to `uint64_t` if `UINT64_MAX` is defined; `unsigned long long` if otherwise.
								#ifndef uint64_m
									#ifdef UINT64_MAX
										#define uint64_m uint64_t
									#else
										#define uint64_m unsigned long long
									#endif
								#endif

								// @DOCLINE * `MU_SIZE_MAX` - equivalent to `SIZE_MAX`.
								#ifndef MU_SIZE_MAX
									#define MU_SIZE_MAX SIZE_MAX
								#endif

							#endif /* stdint.h */

							// @DOCLINE ## `stddef.h` dependencies
							#if !defined(size_m)

								#include <stddef.h>

								// @DOCLINE * `size_m` - equivalent to `size_t`.
								#ifndef size_m
									#define size_m size_t
								#endif

							#endif /* stddef.h */

							// @DOCLINE ## `stdbool.h` dependencies
							#if !defined(muBool) || \
								!defined(MU_TRUE) || \
								!defined(MU_FALSE)

								#include <stdbool.h>

								// @DOCLINE * `muBool` - equivalent to `bool`.
								#ifndef muBool
									#define muBool bool
								#endif

								// @DOCLINE * `MU_TRUE` - equivalent to `true`.
								#ifndef MU_TRUE
									#define MU_TRUE true
								#endif

								// @DOCLINE * `MU_FALSE` - equivalent to `false`.
								#ifndef MU_FALSE
									#define MU_FALSE false
								#endif

							#endif /* stdbool.h */

						// @DOCLINE # Zero struct

							// @DOCLINE There are two macros, `MU_ZERO_STRUCT` and `MU_ZERO_STRUCT_CONST`, which are functions used to zero-out a struct's contents, with their only parameter being the struct type. The reason this needs to be defined is because the way C and C++ syntax handles an empty struct are different, and need to be adjusted for. These macros are overridable by defining them before `muUtility.h` is included.

							#ifndef MU_ZERO_STRUCT
								#ifdef __cplusplus
									#define MU_ZERO_STRUCT(s) {}
								#else
									#define MU_ZERO_STRUCT(s) (s){0}
								#endif
							#endif

							#ifndef MU_ZERO_STRUCT_CONST
								#ifdef __cplusplus
									#define MU_ZERO_STRUCT_CONST(s) {}
								#else
									#define MU_ZERO_STRUCT_CONST(s) {0}
								#endif
							#endif

						// @DOCLINE # Byte manipulation

							// @DOCLINE muUtility defines several inline functions that read a value from a given array of bytes. Internally, they're all defined with the prefix `muu_...`, and then a macro is defined for them as `MU_...` (with change in capitalization after the prefix as well). The macros for these functions can be overridden, and, in such case, the original function will go undefined. For example, the function `muu_rleu8` is primarily referenced via the macro `MU_RLEU8`, and if `MU_RLEU8` is overridden, `muu_rleu8` is never defined and is not referenceable.

							// @DOCLINE All reading functions take in a pointer of bytes as their only parameter and have a return type of the fixed-width size of bits in question; for example, `muu_rleu8` is defined as:

							/* @DOCBEGIN
							```
							MUDEF inline uint8_m muu_rleu8(muByte* b);
							```
							@DOCEND */

							// @DOCLINE All writing functions take in a pointer of bytes as their first parameter, the number to be written as the second parameter, and have a return type of void; for example, `muu_wleu8` is defined as:

							/* @DOCBEGIN
							```
							MUDEF inline void muu_wleu8(muByte* b, uint8_m n);
							```
							@DOCEND */

							// @DOCLINE The exception to this is 24-bit, in which case, the fixed-width types are 32-bit (`uint32_m` / `int32_m`).

							// @DOCLINE ## Byte type

								// @DOCLINE muUtility defines the type `muByte` to refer to a byte. It is defined as `uint8_m`, and is overridable.
								#ifndef muByte
									#define muByte uint8_m
								#endif

							// @DOCLINE ## 8-bit

								// @DOCLINE The following macros exist for byte manipulation regarding 8-bit integers:

								// @DOCLINE * `MU_RLEU8` - reads an unsigned 8-bit integer from little-endian byte data; overridable macro to `muu_rleu8`.
								#ifndef MU_RLEU8
									MUDEF inline uint8_m muu_rleu8(muByte* b) {
										return b[0];
									}
									#define MU_RLEU8 muu_rleu8
								#endif

								// @DOCLINE * `MU_WLEU8` - writes an unsigned 8-bit integer to little-endian byte data; overridable macro to `muu_wleu8`.
								#ifndef MU_WLEU8
									MUDEF inline void muu_wleu8(muByte* b, uint8_m n) {
										b[0] = n;
									}
									#define MU_WLEU8 muu_wleu8
								#endif

								// @DOCLINE * `MU_RLES8` - reads a signed 8-bit integer from little-endian byte data; overridable macro to `muu_rles8`.
								#ifndef MU_RLES8
									MUDEF inline int8_m muu_rles8(muByte* b) {
										// I'm PRETTY sure this is okay...
										return *(int8_m*)b;
									}
									#define MU_RLES8 muu_rles8
								#endif

								// @DOCLINE * `MU_WLES8` - writes a signed 8-bit integer to little-endian byte data; overridable macro to `muu_wles8`.
								#ifndef MU_WLES8
									MUDEF inline void muu_wles8(muByte* b, int8_m n) {
										((int8_m*)(b))[0] = n;
									}
									#define MU_WLES8 muu_wles8
								#endif

								// @DOCLINE * `MU_RBEU8` - reads an unsigned 8-bit integer from big-endian byte data; overridable macro to `muu_rbeu8`.
								#ifndef MU_RBEU8
									MUDEF inline uint8_m muu_rbeu8(muByte* b) {
										return b[0];
									}
									#define MU_RBEU8 muu_rbeu8
								#endif

								// @DOCLINE * `MU_WBEU8` - writes an unsigned 8-bit integer to big-endian byte data; overridable macro to `muu_wbeu8`.
								#ifndef MU_WBEU8
									MUDEF inline void muu_wbeu8(muByte* b, uint8_m n) {
										b[0] = n;
									}
									#define MU_WBEU8 muu_wbeu8
								#endif

								// @DOCLINE * `MU_RBES8` - reads a signed 8-bit integer from big-endian byte data; overridable macro to `muu_rbes8`.
								#ifndef MU_RBES8
									MUDEF inline int8_m muu_rbes8(muByte* b) {
										return *(int8_m*)b;
									}
									#define MU_RBES8 muu_rbes8
								#endif

								// @DOCLINE * `MU_WBES8` - writes a signed 8-bit integer to big-endian byte data; overridable macro to `muu_wbes8`.
								#ifndef MU_WBES8
									MUDEF inline void muu_wbes8(muByte* b, int8_m n) {
										((int8_m*)(b))[0] = n;
									}
									#define MU_WBES8 muu_wbes8
								#endif

							// @DOCLINE ## 16-bit

								// @DOCLINE The following macros exist for byte manipulation regarding 16-bit integers:

								// @DOCLINE * `MU_RLEU16` - reads an unsigned 16-bit integer from little-endian byte data; overridable macro to `muu_rleu16`.
								#ifndef MU_RLEU16
									MUDEF inline uint16_m muu_rleu16(muByte* b) {
										return (
											((uint16_m)(b[0]) << 0) |
											((uint16_m)(b[1]) << 8)
										);
									}
									#define MU_RLEU16 muu_rleu16
								#endif

								// @DOCLINE * `MU_WLEU16` - writes an unsigned 16-bit integer to little-endian byte data; overridable macro to `muu_wleu16`.
								#ifndef MU_WLEU16
									MUDEF inline void muu_wleu16(muByte* b, uint16_m n) {
										b[0] = (uint8_m)(n >> 0);
										b[1] = (uint8_m)(n >> 8);
									}
									#define MU_WLEU16 muu_wleu16
								#endif

								// @DOCLINE * `MU_RLES16` - reads a signed 16-bit integer from little-endian byte data; overridable macro to `muu_rles16`.
								#ifndef MU_RLES16
									MUDEF inline int16_m muu_rles16(muByte* b) {
										uint16_m u16 = muu_rleu16(b);
										return *(int16_m*)&u16;
									}
									#define MU_RLES16 muu_rles16
								#endif

								// @DOCLINE * `MU_WLES16` - writes a signed 16-bit integer to little-endian byte data; overridable macro to `muu_wles16`.
								#ifndef MU_WLES16
									MUDEF inline void muu_wles16(muByte* b, int16_m n) {
										uint16_m un = *(uint16_m*)&n;
										b[0] = (uint8_m)(un >> 0);
										b[1] = (uint8_m)(un >> 8);
									}
									#define MU_WLES16 muu_wles16
								#endif

								// @DOCLINE * `MU_RBEU16` - reads an unsigned 16-bit integer from big-endian byte data; overridable macro to `muu_rbeu16`.
								#ifndef MU_RBEU16
									MUDEF inline uint16_m muu_rbeu16(muByte* b) {
										return (
											((uint16_m)(b[1]) << 0) |
											((uint16_m)(b[0]) << 8)
										);
									}
									#define MU_RBEU16 muu_rbeu16
								#endif

								// @DOCLINE * `MU_WBEU16` - writes an unsigned 16-bit integer to big-endian byte data; overridable macro to `muu_wbeu16`.
								#ifndef MU_WBEU16
									MUDEF inline void muu_wbeu16(muByte* b, uint16_m n) {
										b[1] = (uint8_m)(n >> 0);
										b[0] = (uint8_m)(n >> 8);
									}
									#define MU_WBEU16 muu_wbeu16
								#endif

								// @DOCLINE * `MU_RBES16` - reads a signed 16-bit integer from big-endian byte data; overridable macro to `muu_rbes16`.
								#ifndef MU_RBES16
									MUDEF inline int16_m muu_rbes16(muByte* b) {
										uint16_m u16 = muu_rbeu16(b);
										return *(int16_m*)&u16;
									}
									#define MU_RBES16 muu_rbes16
								#endif

								// @DOCLINE * `MU_WBES16` - writes a signed 16-bit integer to big-endian byte data; overridable macro to `muu_wbes16`.
								#ifndef MU_WBES16
									MUDEF inline void muu_wbes16(muByte* b, int16_m n) {
										uint16_m un = *(uint16_m*)&n;
										b[1] = (uint8_m)(un >> 0);
										b[0] = (uint8_m)(un >> 8);
									}
									#define MU_WBES16 muu_wbes16
								#endif

							// @DOCLINE ## 24-bit

								// @DOCLINE The following macros exist for byte manipulation regarding 24-bit integers:

								// @DOCLINE * `MU_RLEU24` - reads an unsigned 24-bit integer from little-endian byte data; overridable macro to `muu_rleu24`.
								#ifndef MU_RLEU24
									MUDEF inline uint32_m muu_rleu24(muByte* b) {
										return (
											((uint32_m)(b[0]) << 0) |
											((uint32_m)(b[1]) << 8) |
											((uint32_m)(b[2]) << 16)
										);
									}
									#define MU_RLEU24 muu_rleu24
								#endif

								// @DOCLINE * `MU_WLEU24` - writes an unsigned 24-bit integer to little-endian byte data; overridable macro to `muu_wleu24`.
								#ifndef MU_WLEU24
									MUDEF inline void muu_wleu24(muByte* b, uint32_m n) {
										b[0] = (uint8_m)(n >> 0);
										b[1] = (uint8_m)(n >> 8);
										b[2] = (uint8_m)(n >> 16);
									}
									#define MU_WLEU24 muu_wleu24
								#endif

								// @DOCLINE * `MU_RLES24` - reads a signed 24-bit integer from little-endian byte data; overridable macro to `muu_rles24`.
								#ifndef MU_RLES24
									MUDEF inline int32_m muu_rles24(muByte* b) {
										uint32_m u24 = muu_rleu24(b);
										return *(int32_m*)&u24;
									}
									#define MU_RLES24 muu_rles24
								#endif

								// @DOCLINE * `MU_WLES24` - writes a signed 24-bit integer to little-endian byte data; overridable macro to `muu_wles24`.
								#ifndef MU_WLES24
									MUDEF inline void muu_wles24(muByte* b, int32_m n) {
										// Probably definitely doesn't work with signed integers; fix later
										uint32_m un = *(uint32_m*)&n;
										b[0] = (uint8_m)(un >> 0);
										b[1] = (uint8_m)(un >> 8);
										b[2] = (uint8_m)(un >> 16);
									}
									#define MU_WLES24 muu_wles24
								#endif

								// @DOCLINE * `MU_RBEU24` - reads an unsigned 24-bit integer from big-endian byte data; overridable macro to `muu_rbeu24`.
								#ifndef MU_RBEU24
									MUDEF inline uint32_m muu_rbeu24(muByte* b) {
										return (
											((uint32_m)(b[2]) << 0) |
											((uint32_m)(b[1]) << 8) |
											((uint32_m)(b[0]) << 16)
										);
									}
									#define MU_RBEU24 muu_rbeu24
								#endif

								// @DOCLINE * `MU_WBEU24` - writes an unsigned 24-bit integer to big-endian byte data; overridable macro to `muu_wbeu24`.
								#ifndef MU_WBEU24
									MUDEF inline void muu_wbeu24(muByte* b, uint32_m n) {
										b[2] = (uint8_m)(n >> 0);
										b[1] = (uint8_m)(n >> 8);
										b[0] = (uint8_m)(n >> 16);
									}
									#define MU_WBEU24 muu_wbeu24
								#endif

								// @DOCLINE * `MU_RBES24` - reads a signed 24-bit integer from big-endian byte data; overridable macro to `muu_rbes24`.
								#ifndef MU_RBES24
									MUDEF inline int32_m muu_rbes24(muByte* b) {
										uint32_m u24 = muu_rbeu24(b);
										return *(int32_m*)&u24;
									}
									#define MU_RBES24 muu_rbes24
								#endif

								// @DOCLINE * `MU_WBES24` - writes a signed 24-bit integer to big-endian byte data; overridable macro to `muu_wbes24`.
								#ifndef MU_WBES24
									MUDEF inline void muu_wbes24(muByte* b, int32_m n) {
										uint32_m un = *(uint32_m*)&n;
										b[2] = (uint8_m)(un >> 0);
										b[1] = (uint8_m)(un >> 8);
										b[0] = (uint8_m)(un >> 16);
									}
									#define MU_WBES24 muu_wbes24
								#endif

							// @DOCLINE ## 32-bit

								// @DOCLINE The following macros exist for byte manipulation regarding 32-bit integers:

								// @DOCLINE * `MU_RLEU32` - reads an unsigned 32-bit integer from little-endian byte data; overridable macro to `muu_rleu32`.
								#ifndef MU_RLEU32
									MUDEF inline uint32_m muu_rleu32(muByte* b) {
										return (
											((uint32_m)(b[0]) << 0)  |
											((uint32_m)(b[1]) << 8)  |
											((uint32_m)(b[2]) << 16) |
											((uint32_m)(b[3]) << 24)
										);
									}
									#define MU_RLEU32 muu_rleu32
								#endif

								// @DOCLINE * `MU_WLEU32` - writes an unsigned 32-bit integer to little-endian byte data; overridable macro to `muu_wleu32`.
								#ifndef MU_WLEU32
									MUDEF inline void muu_wleu32(muByte* b, uint32_m n) {
										b[0] = (uint8_m)(n >> 0);
										b[1] = (uint8_m)(n >> 8);
										b[2] = (uint8_m)(n >> 16);
										b[3] = (uint8_m)(n >> 24);
									}
									#define MU_WLEU32 muu_wleu32
								#endif

								// @DOCLINE * `MU_RLES32` - reads a signed 32-bit integer from little-endian byte data; overridable macro to `muu_rles32`.
								#ifndef MU_RLES32
									MUDEF inline int32_m muu_rles32(muByte* b) {
										uint32_m u32 = muu_rleu32(b);
										return *(int32_m*)&u32;
									}
									#define MU_RLES32 muu_rles32
								#endif

								// @DOCLINE * `MU_WLES32` - writes a signed 32-bit integer to little-endian byte data; overridable macro to `muu_wles32`.
								#ifndef MU_WLES32
									MUDEF inline void muu_wles32(muByte* b, int32_m n) {
										uint32_m un = *(uint32_m*)&n;
										b[0] = (uint8_m)(un >> 0);
										b[1] = (uint8_m)(un >> 8);
										b[2] = (uint8_m)(un >> 16);
										b[3] = (uint8_m)(un >> 24);
									}
									#define MU_WLES32 muu_wles32
								#endif

								// @DOCLINE * `MU_RBEU32` - reads an unsigned 32-bit integer from big-endian byte data; overridable macro to `muu_rbeu32`.
								#ifndef MU_RBEU32
									MUDEF inline uint32_m muu_rbeu32(muByte* b) {
										return (
											((uint32_m)(b[3]) << 0)  |
											((uint32_m)(b[2]) << 8)  |
											((uint32_m)(b[1]) << 16) |
											((uint32_m)(b[0]) << 24)
										);
									}
									#define MU_RBEU32 muu_rbeu32
								#endif

								// @DOCLINE * `MU_WBEU32` - writes an unsigned 32-bit integer to big-endian byte data; overridable macro to `muu_wbeu32`.
								#ifndef MU_WBEU32
									MUDEF inline void muu_wbeu32(muByte* b, uint32_m n) {
										b[3] = (uint8_m)(n >> 0);
										b[2] = (uint8_m)(n >> 8);
										b[1] = (uint8_m)(n >> 16);
										b[0] = (uint8_m)(n >> 24);
									}
									#define MU_WBEU32 muu_wbeu32
								#endif

								// @DOCLINE * `MU_RBES32` - reads a signed 32-bit integer from big-endian byte data; overridable macro to `muu_rbes32`.
								#ifndef MU_RBES32
									MUDEF inline int32_m muu_rbes32(muByte* b) {
										uint32_m u32 = muu_rbeu32(b);
										return *(int32_m*)&u32;
									}
									#define MU_RBES32 muu_rbes32
								#endif

								// @DOCLINE * `MU_WBES32` - writes a signed 32-bit integer to big-endian byte data; overridable macro to `muu_wbes32`.
								#ifndef MU_WBES32
									MUDEF inline void muu_wbes32(muByte* b, int32_m n) {
										uint32_m un = *(uint32_m*)&n;
										b[3] = (uint8_m)(un >> 0);
										b[2] = (uint8_m)(un >> 8);
										b[1] = (uint8_m)(un >> 16);
										b[0] = (uint8_m)(un >> 24);
									}
									#define MU_WBES32 muu_wbes32
								#endif

							// @DOCLINE ## 64-bit

								// @DOCLINE The following macros exist for byte manipulation regarding 64-bit integers:

								// @DOCLINE * `MU_RLEU64` - reads an unsigned 64-bit integer from little-endian byte data; overridable macro to `muu_rleu64`.
								#ifndef MU_RLEU64
									MUDEF inline uint64_m muu_rleu64(muByte* b) {
										return (
											((uint64_m)(b[0]) << 0)  |
											((uint64_m)(b[1]) << 8)  |
											((uint64_m)(b[2]) << 16) |
											((uint64_m)(b[3]) << 24) |
											((uint64_m)(b[4]) << 32) |
											((uint64_m)(b[5]) << 40) |
											((uint64_m)(b[6]) << 48) |
											((uint64_m)(b[7]) << 56)
										);
									}
									#define MU_RLEU64 muu_rleu64
								#endif

								// @DOCLINE * `MU_WLEU64` - writes an unsigned 64-bit integer to little-endian byte data; overridable macro to `muu_wleu64`.
								#ifndef MU_WLEU64
									MUDEF inline void muu_wleu64(muByte* b, uint64_m n) {
										b[0] = (uint8_m)(n >> 0);
										b[1] = (uint8_m)(n >> 8);
										b[2] = (uint8_m)(n >> 16);
										b[3] = (uint8_m)(n >> 24);
										b[4] = (uint8_m)(n >> 32);
										b[5] = (uint8_m)(n >> 40);
										b[6] = (uint8_m)(n >> 48);
										b[7] = (uint8_m)(n >> 56);
									}
									#define MU_WLEU64 muu_wleu64
								#endif

								// @DOCLINE * `MU_RLES64` - reads a signed 64-bit integer from little-endian byte data; overridable macro to `muu_rles64`.
								#ifndef MU_RLES64
									MUDEF inline int64_m muu_rles64(muByte* b) {
										uint64_m u64 = muu_rleu64(b);
										return *(int64_m*)&u64;
									}
									#define MU_RLES64 muu_rles64
								#endif

								// @DOCLINE * `MU_WLES64` - writes a signed 64-bit integer to little-endian byte data; overridable macro to `muu_wles64`.
								#ifndef MU_WLES64
									MUDEF inline void muu_wles64(muByte* b, int64_m n) {
										uint64_m un = *(uint64_m*)&n;
										b[0] = (uint8_m)(un >> 0);
										b[1] = (uint8_m)(un >> 8);
										b[2] = (uint8_m)(un >> 16);
										b[3] = (uint8_m)(un >> 24);
										b[4] = (uint8_m)(un >> 32);
										b[5] = (uint8_m)(un >> 40);
										b[6] = (uint8_m)(un >> 48);
										b[7] = (uint8_m)(un >> 56);
									}
									#define MU_WLES64 muu_wles64
								#endif

								// @DOCLINE * `MU_RBEU64` - reads an unsigned 64-bit integer from big-endian byte data; overridable macro to `muu_rbeu64`.
								#ifndef MU_RBEU64
									MUDEF inline uint64_m muu_rbeu64(muByte* b) {
										return (
											((uint64_m)(b[7]) << 0)  |
											((uint64_m)(b[6]) << 8)  |
											((uint64_m)(b[5]) << 16) |
											((uint64_m)(b[4]) << 24) |
											((uint64_m)(b[3]) << 32) |
											((uint64_m)(b[2]) << 40) |
											((uint64_m)(b[1]) << 48) |
											((uint64_m)(b[0]) << 56)
										);
									}
									#define MU_RBEU64 muu_rbeu64
								#endif

								// @DOCLINE * `MU_WBEU64` - writes an unsigned 64-bit integer to big-endian byte data; overridable macro to `muu_wbeu64`.
								#ifndef MU_WBEU64
									MUDEF inline void muu_wbeu64(muByte* b, uint64_m n) {
										b[7] = (uint8_m)(n >> 0);
										b[6] = (uint8_m)(n >> 8);
										b[5] = (uint8_m)(n >> 16);
										b[4] = (uint8_m)(n >> 24);
										b[3] = (uint8_m)(n >> 32);
										b[2] = (uint8_m)(n >> 40);
										b[1] = (uint8_m)(n >> 48);
										b[0] = (uint8_m)(n >> 56);
									}
									#define MU_WBEU64 muu_wbeu64
								#endif

								// @DOCLINE * `MU_RBES64` - reads a signed 64-bit integer from big-endian byte data; overridable macro to `muu_rbes64`.
								#ifndef MU_RBES64
									MUDEF inline int64_m muu_rbes64(muByte* b) {
										uint64_m u64 = muu_rbeu64(b);
										return *(int64_m*)&u64;
									}
									#define MU_RBES64 muu_rbes64
								#endif

								// @DOCLINE * `MU_WBES64` - writes a signed 64-bit integer to big-endian byte data; overridable macro to `muu_wbes64`.
								#ifndef MU_WBES64
									MUDEF inline void muu_wbes64(muByte* b, int64_m n) {
										uint64_m un = *(uint64_m*)&n;
										b[7] = (uint8_m)(un >> 0);
										b[6] = (uint8_m)(un >> 8);
										b[5] = (uint8_m)(un >> 16);
										b[4] = (uint8_m)(un >> 24);
										b[3] = (uint8_m)(un >> 32);
										b[2] = (uint8_m)(un >> 40);
										b[1] = (uint8_m)(un >> 48);
										b[0] = (uint8_m)(un >> 56);
									}
									#define MU_WBES64 muu_wbes64
								#endif

						// @DOCLINE # Set result

							/* @DOCBEGIN

							The `MU_SET_RESULT(res, val)` macro is an overridable function that checks if the given parameter `res` is a null pointer. If it is, it does nothing, but if it isn't, it dereferences the pointer and sets the value to `val`. This macro saves a lot of code, shrinking down what would be this:

							```c
							if (result) {
								*result = ...;
							}
							```

							into this:

							```c
							MU_SET_RESULT(result, ...)
							```

							@DOCEND */

							#ifndef MU_SET_RESULT
								#define MU_SET_RESULT(res, val) if(res){*res=val;}
							#endif

						// @DOCLINE # Enum

							/* @DOCBEGIN

							The `MU_ENUM(name, ...)` macro is an overridable function used to declare an enumerator. `name` is the name of the enumerator type, and `...` are all of the values. The reason why one would prefer this over the traditional way of declaring enumerators is because it actually makes it a `size_m`, which can avoid errors on certain compilers (looking at you, Microsoft) in regards to treating enumerators like values. It expands like this:

							```c
							enum _##name {
								__VA_ARGS__
							};
							typedef enum _##name _##name;
							typedef size_m name;
							```

							@DOCEND */

							#define MU_ENUM(name, ...) enum _##name{__VA_ARGS__};typedef enum _##name _##name;typedef size_m name;

						// @DOCLINE # Operating system recognition

							/* @DOCBEGIN

							The macros `MU_WIN32` or `MU_LINUX` are defined (if neither were defined before) in order to allow mu libraries to easily check if they're running on a Windows or Linux system.

							`MU_WIN32` will be defined if `WIN32` or `_WIN32` are defined, one of which is usually pre-defined on Windows systems.

							`MU_LINUX` will be defined if `__linux__` is defined.

							@DOCEND */

							#if !defined(MU_WIN32) && !defined(MU_LINUX)
								#if defined(WIN32) || defined(_WIN32)
									#define MU_WIN32
								#endif
								#if defined(__linux__)
									#define MU_LINUX
								#endif
							#endif

						MU_CPP_EXTERN_END

					#endif /* MUU_H */
				// @ATTENTION

				// @DOCLINE > Note that mu libraries store their dependencies within their files, so you don't need to import these dependencies yourself; this section is purely to provide more information about the contents that this file defines. The libraries listed may also have other dependencies that they also include that aren't explicitly listed here.

			MU_CPP_EXTERN_START

			// Typedefs explained later
			typedef uint32_m muttResult;

			// @DOCLINE # Loading a TrueType font

				typedef struct muttFont muttFont;

				// @DOCLINE All major parts of the mutt API rely on loading a TrueType font and then reading data from it, which is encapsulated in the `muttFont` struct, which is described [later in the lower-level API section](#font-struct). Most casual usage of the mutt API only needs to treat `muttFont` as a handle to the font itself.

				// @DOCLINE ## Loading a font

					typedef uint32_m muttLoadFlags;

					// @DOCLINE To load a TrueType font into a `muttFont` struct, the function `mutt_load` is used, defined below: @NLNT
					MUDEF muttResult mutt_load(muByte* data, uint64_m datalen, muttFont* font, muttLoadFlags load_flags);

					// @DOCLINE `data` and `datalen` should be the raw binary data of the font file, and should be loaded by the user themselves. `font` is the `muttFont` struct to be filled in with information after loading. `load_flags` is a value whose bits indicate which tables to load; more elaboration on this is given in the [load flags section](#font-load-flags).

					// @DOCLINE If the result returned by mutt is fatal, the contents of `font` are undefined. If the result returned by mutt isn't fatal, the font has been successfully loaded, and must be deloaded at some point.

					// @DOCLINE Once this function has finished executing, there are no internal dependencies on the pointer to the data given, and can be safely freed.

				// @DOCLINE ## Deloading a font

					// @DOCLINE To deload a font, the function `mutt_deload` is used, defined below: @NLNT
					MUDEF void mutt_deload(muttFont* font);

					// @DOCLINE This function must be called on every successfully loaded font at some point. The contents of `font` are undefined after `mutt_deload` has been called on it.

				// @DOCLINE ## Font load flags

					// @DOCLINE To customize what tables are loaded when loading a TrueType font, the type `muttLoadFlags` exists (typedef for `uint32_m`) whose bits indicate what tables should be loaded. It has the following defined values:

					// @DOCLINE * [0x00000001] `MUTT_LOAD_MAXP` - load the [maxp table](#maxp-table).
					#define MUTT_LOAD_MAXP 0x00000001
					// @DOCLINE * [0x00000002] `MUTT_LOAD_HEAD` - load the [head table](#head-table).
					#define MUTT_LOAD_HEAD 0x00000002
					// @DOCLINE * [0x00000004] `MUTT_LOAD_HHEA` - load the [hhea table](#hhea-table).
					#define MUTT_LOAD_HHEA 0x00000004
					// @DOCLINE * [0x00000008] `MUTT_LOAD_HMTX` - load the [hmtx table](#hmtx-table).
					#define MUTT_LOAD_HMTX 0x00000008
					// @DOCLINE * [0x00000010] `MUTT_LOAD_LOCA` - load the [loca table](#loca-table).
					#define MUTT_LOAD_LOCA 0x00000010
					// @DOCLINE * [0x00000020] `MUTT_LOAD_POST` - load the post table.
					#define MUTT_LOAD_POST 0x00000020
					// @DOCLINE * [0x00000040] `MUTT_LOAD_NAME` - load the [name table](#name-table).
					#define MUTT_LOAD_NAME 0x00000040
					// @DOCLINE * [0x00000080] `MUTT_LOAD_GLYF` - load the [glyf table](#glyf-table).
					#define MUTT_LOAD_GLYF 0x00000080
					// @DOCLINE * [0x00000100] `MUTT_LOAD_CMAP` - load the [cmap table](#cmap-table).
					#define MUTT_LOAD_CMAP 0x00000100

					// @DOCLINE To see which tables successfully loaded, see the [section covering the font struct](#font-struct).

					// @DOCLINE ### Font load flag groups

						// @DOCLINE For most users, it is unnecessary or confusing to specify all the tables they want manually, so several macros are defined that set the bits for several tables. These are the defined flag groups:

						// @DOCLINE * [0x000001FF] `MUTT_LOAD_REQUIRED` - load the tables required by the TrueType specification (maxp, head, hhea, hmtx, loca, post, name, glyf, and cmap).
						#define MUTT_LOAD_REQUIRED 0x000001FF

						// @DOCLINE * [0xFFFFFFFF] `MUTT_LOAD_ALL` - loads all tables that could be supported by mutt.
						#define MUTT_LOAD_ALL 0xFFFFFFFF

			// @DOCLINE # Low-level API

				// @DOCLINE The low-level API of mutt is designed to support reading information from the tables provided by TrueType. It is used internally by all other parts of the mutt API. All values provided by the low-level API have been checked to be valid, and are guaranteed to be valid once given to the user, unless explicitly stated otherwise.

				// @DOCLINE ## Font struct

					typedef struct muttDirectory muttDirectory;
					typedef struct muttMaxp muttMaxp;
					typedef struct muttHead muttHead;
					typedef struct muttHhea muttHhea;
					typedef struct muttHmtx muttHmtx;
					typedef union  muttLoca muttLoca;
					typedef struct muttPost muttPost;
					typedef struct muttName muttName;
					typedef struct muttGlyf muttGlyf;
					typedef struct muttCmap muttCmap;

					// @DOCLINE The font struct, `muttFont`, is the primary way of reading information from TrueType tables, holding pointers to each table's defined data, and is automatically filled using the function [`mutt_load`](#loading-a-font). It has the following members:

					struct muttFont {
						// @DOCLINE * `@NLFT load_flags` - flags indicating which requested tables successfully loaded.
						muttLoadFlags load_flags;
						// @DOCLINE * `@NLFT fail_load_flags` - flags indicating which requested tables did not successfully load.
						muttLoadFlags fail_load_flags;

						// @DOCLINE * `@NLFT* directory` - a pointer to the [font directory](#font-directory).
						muttDirectory* directory;

						// @DOCLINE * `@NLFT* maxp` - a pointer to the [maxp table](#maxp-table).
						muttMaxp* maxp;
						// @DOCLINE * `@NLFT maxp_res` - the result of attempting to load the maxp table.
						muttResult maxp_res;

						// @DOCLINE * `@NLFT* head` - a pointer to the [head table](#head-table).
						muttHead* head;
						// @DOCLINE * `@NLFT head_res` - the result of attempting to load the head table.
						muttResult head_res;

						// @DOCLINE * `@NLFT* hhea` - a pointer to the [hhea table](#hhea-table).
						muttHhea* hhea;
						// @DOCLINE * `@NLFT hhea_res` - the result of attempting to load the hhea table.
						muttResult hhea_res;

						// @DOCLINE * `@NLFT* hmtx` - a pointer to the [hmtx table](#hmtx-table).
						muttHmtx* hmtx;
						// @DOCLINE * `@NLFT hmtx_res` - the result of attempting to load the hmtx table.
						muttResult hmtx_res;

						// @DOCLINE * `@NLFT* loca` - a pointer to the [loca table](#loca-table).
						muttLoca* loca;
						// @DOCLINE * `@NLFT loca_res` - the result of attempting to load the loca table.
						muttResult loca_res;

						// @DOCLINE * `@NLFT* post` - a pointer to the post table.
						muttPost* post;
						// @DOCLINE * `@NLFT post_res` - the result of attempting to load the post table.
						muttResult post_res;

						// @DOCLINE * `@NLFT* name` - a pointer to the [name table](#name-table).
						muttName* name;
						// @DOCLINE * `@NLFT name_res` - the result of attempting to load the name table.
						muttResult name_res;

						// @DOCLINE * `@NLFT* glyf` - a pointer to the [glyf table](#glyf-table).
						muttGlyf* glyf;
						// @DOCLINE * `@NLFT glyf_res` - the result of attempting to load the glyf table.
						muttResult glyf_res;

						// @DOCLINE * `@NLFT* cmap` - a pointer to the [cmap table](#cmap-table).
						muttCmap* cmap;
						// @DOCLINE * `@NLFT cmap_res` - the result of attempting to load the cmap table.
						muttResult cmap_res;
					};

					// @DOCLINE For each optionally-loadable table within the `muttFont` struct, there exists two members: one that exists as a pointer to the table, and a result value storing the result of attempting to load the table itself. If the respective result value is fatal, or the user never requested for the table to be loaded, the pointer to the table will be 0. Otherwise, the member will be a valid pointer to the table information.

				// @DOCLINE ## Font directory

					typedef struct muttTableRecord muttTableRecord;

					// @DOCLINE The struct `muttDirectory` is used to list all of the tables provided by a TrueType font. It is stored in the struct `muttFont` as `muttFont.directory` and is similar to TrueType's table directory. It has the following members:

					struct muttDirectory {
						// @DOCLINE * `@NLFT num_tables` - the amount of tables within the font; equivalent to "numTables" in the table directory.
						uint16_m num_tables;
						// @DOCLINE * `@NLFT* records` - pointer to an array of each [table record](#table-record); similar to "tableRecords" in the table directory. This array is of length `num_tables`.
						muttTableRecord* records;
					};

					// @DOCLINE ### Table record

						// @DOCLINE The struct `muttTableRecord` represents a table record in the table directory. It has the following members:

						struct muttTableRecord {
							// @DOCLINE * `@NLFT table_tag_u8[4]` - the table tag, represented by four consecutive unsigned 8-bit values representing each character of the table tag.
							uint8_m table_tag_u8[4];
							// @DOCLINE * `@NLFT table_tag_u32`- the table tag, represented by an unsigned 32-bit value representing the characters of the table tag read as big-endian.
							uint32_m table_tag_u32;
							// @DOCLINE * `@NLFT checksum` - equivalent to "checksum" in the table record.
							uint32_m checksum;
							// @DOCLINE * `@NLFT offset` - equivalent to "offset" in the table record.
							uint32_m offset;
							// @DOCLINE * `@NLFT length` - equivalent to "length" in the table record.
							uint32_m length;
						};

						// @DOCLINE The checksum value is not validated for the head table, as the head table itself includes a checksum value.

				// @DOCLINE ## Maxp table

					// @DOCLINE The struct `muttMaxp` is used to represent the maxp table provided by a TrueType font, stored in the struct `muttFont` as the pointer member "`maxp`", and loaded with the flag `MUTT_LOAD_MAXP`. It has the following members:

					struct muttMaxp {
						// @DOCLINE * `@NLFT version_high` - the high bytes of "version" in the version 1.0 maxp table.
						uint16_m version_high;
						// @DOCLINE * `@NLFT version_low` - the low bytes of "version" in the version 1.0 maxp table.
						uint16_m version_low;
						// @DOCLINE * `@NLFT num_glyphs` - equivalent to "numGlyphs" in the version 1.0 maxp table.
						uint16_m num_glyphs;
						// @DOCLINE * `@NLFT max_points` - equivalent to "maxPoints" in the version 1.0 maxp table.
						uint16_m max_points;
						// @DOCLINE * `@NLFT max_contours` - equivalent to "maxContours" in the version 1.0 maxp table.
						uint16_m max_contours;
						// @DOCLINE * `@NLFT max_composite_points` - equivalent to "maxCompositePoints" in the version 1.0 maxp table.
						uint16_m max_composite_points;
						// @DOCLINE * `@NLFT max_composite_contours` - equivalent to "maxCompositeContours" in the version 1.0 maxp table.
						uint16_m max_composite_contours;
						// @DOCLINE * `@NLFT max_zones` - equivalent to "maxZones" in the version 1.0 maxp table.
						uint16_m max_zones;
						// @DOCLINE * `@NLFT max_twilight_points` - equivalent to "maxTwilightPoints" in the version 1.0 maxp table.
						uint16_m max_twilight_points;
						// @DOCLINE * `@NLFT max_storage` - equivalent to "maxStorage" in the version 1.0 maxp table.
						uint16_m max_storage;
						// @DOCLINE * `@NLFT max_function_defs` - equivalent to "maxFunctionDefs" in the version 1.0 maxp table.
						uint16_m max_function_defs;
						// @DOCLINE * `@NLFT max_instruction_defs` - equivalent to "maxInstructionDefs" in the version 1.0 maxp table.
						uint16_m max_instruction_defs;
						// @DOCLINE * `@NLFT max_stack_elements` - equivalent to "maxStackElements" in the version 1.0 maxp table.
						uint16_m max_stack_elements;
						// @DOCLINE * `@NLFT max_size_of_instructions` - equivalent to "maxSizeOfInstructions" in the version 1.0 maxp table.
						uint16_m max_size_of_instructions;
						// @DOCLINE * `@NLFT max_component_elements` - equivalent to "maxComponentElements" in the version 1.0 maxp table.
						uint16_m max_component_elements;
						// @DOCLINE * `@NLFT max_component_depth` - equivalent to "maxComponentDepth" in the version 1.0 maxp table.
						uint16_m max_component_depth;
					};

					// @DOCLINE Since most values given in this table are just maximums, there are only checks performed for the version, numGlyph, and maxZones values. All other values dictate maximums that other tables must follow, and checks will be performed on said tables to ensure they stay within the maximums dictated by maxp.

				// @DOCLINE ## Head table

					// @DOCLINE The struct `muttHead` is used to represent the head table provided by a TrueType font, stored in the struct `muttFont` as the pointer member "`head`", and loaded with the flag `MUTT_LOAD_HEAD`. It has the following members:

					struct muttHead {
						// @DOCLINE * `@NLFT font_revision_high` - equivalent to the high bytes of "fontRevision" in the head table.
						int16_m font_revision_high;
						// @DOCLINE * `@NLFT font_revision_low` - equivalent to the low bytes of "fontRevision" in the head table.
						uint16_m font_revision_low;
						// @DOCLINE * `@NLFT checksum_adjustment` - equivalent to "checksumAdjustment" in the head table.
						uint32_m checksum_adjustment;
						// @DOCLINE * `@NLFT flags` - equivalent to "flags" in the head table.
						uint16_m flags;
						// @DOCLINE * `@NLFT units_per_em` - equivalent to "unitsPerEm" in the head table.
						uint16_m units_per_em;
						// @DOCLINE * `@NLFT created` - equivalent to "created" in the head table.
						int64_m created;
						// @DOCLINE * `@NLFT modified` - equivalent to "modified" in the head table.
						int64_m modified;
						// @DOCLINE * `@NLFT x_min` - equivalent to "xMin" in the head table.
						int16_m x_min;
						// @DOCLINE * `@NLFT y_min` - equivalent to "yMin" in the head table.
						int16_m y_min;
						// @DOCLINE * `@NLFT x_max` - equivalent to "xMax" in the head table.
						int16_m x_max;
						// @DOCLINE * `@NLFT y_max` - equivalent to "yMax" in the head table.
						int16_m y_max;
						// @DOCLINE * `@NLFT mac_style` - equivalent to "macStyle" in the head table.
						uint16_m mac_style;
						// @DOCLINE * `@NLFT lowest_rec_ppem` - equivalent to "lowestRecPPEM" in the head table.
						uint16_m lowest_rec_ppem;
						// @DOCLINE * `@NLFT font_direction_hint` - equivalent to "fontDirectionHint" in the head table.
						int16_m font_direction_hint;
						// @DOCLINE * `@NLFT index_to_loc_format` - equivalent to "indexToLocFormat" in the head table.
						int16_m index_to_loc_format;
					};

					// @DOCLINE Currently, the values for "checksumAdjustment" and "fontDirectionHint" are not checked.

					// @DOCLINE ### indexToLocFormat macros

						// @DOCLINE The macros `MUTT_OFFSET_16` (0) and `MUTT_OFFSET_32` (1) are defined to make reading the value of "indexToLocFormat" easier.
						#define MUTT_OFFSET_16 0
						#define MUTT_OFFSET_32 1

				// @DOCLINE ## Hhea table

					// @DOCLINE The struct `muttHhea` is used to represent the hhea table provided by a TrueType font, stored in the struct `muttFont` as the pointer member "`hhea`", and loaded with the flag `MUTT_LOAD_HHEA` (`MUTT_LOAD_MAXP` must also be defined). It has the following members:

					struct muttHhea {
						// @DOCLINE * `@NLFT ascender` - equivalent to "ascender" in the hhea table.
						int16_m ascender;
						// @DOCLINE * `@NLFT descender` - equivalent to "descender" in the hhea table.
						int16_m descender;
						// @DOCLINE * `@NLFT line_gap` - equivalent to "lineGap" in the hhea table.
						int16_m line_gap;
						// @DOCLINE * `@NLFT advance_width_max` - equivalent to "advanceWidthMax" in the hhea table.
						uint16_m advance_width_max;
						// @DOCLINE * `@NLFT min_left_side_bearing` - equivalent to "minLeftSideBearing" in the hhea table.
						int16_m min_left_side_bearing;
						// @DOCLINE * `@NLFT min_right_side_bearing` - equivalent to "minRightSideBearing" in the hhea table.
						int16_m min_right_side_bearing;
						// @DOCLINE * `@NLFT x_max_extent` - equivalent to "xMaxExtent" in the hhea table.
						int16_m x_max_extent;
						// @DOCLINE * `@NLFT caret_slope_rise` - equivalent to "caretSlopeRise" in the hhea table.
						int16_m caret_slope_rise;
						// @DOCLINE * `@NLFT caret_slope_run` - equivalent to "caretSlopeRun" in the hhea table.
						int16_m caret_slope_run;
						// @DOCLINE * `@NLFT caret_offset` - equivalent to "caretOffset" in the hhea table.
						int16_m caret_offset;
						// @DOCLINE * `@NLFT number_of_hmetrics` - equivalent to "numberOfHMetrics" in the hhea table.
						uint16_m number_of_hmetrics;
					};

					// @DOCLINE All values provided in the `muttHhea` struct are not checked (besides numberOfHMetrics, since it must be less than or equal to `maxp->num_glyphs` in order to generate a valid array length for "leftSideBearings" within hmtx), as virtually all of them have no technically "incorrect" values (from what I'm aware).

				// @DOCLINE ## Hmtx table

					typedef struct muttLongHorMetric muttLongHorMetric;

					// @DOCLINE The struct `muttHmtx` is used to represent the hmtx table provided by a TrueType font, stored in the struct `muttFont` as the pointer member "`hmtx`", and loaded with the flag `MUTT_LOAD_HMTX` (`MUTT_LOAD_MAXP` and `MUTT_LOAD_HHEA` must also be defined). It has the following members:

					struct muttHmtx {
						// @DOCLINE * `@NLFT* hmetrics` - an array of horizontal metric records; equiavlent to "hMetrics" in the hmtx table. Its length is equivalent to `hhea->number_of_hmetrics`.
						muttLongHorMetric* hmetrics;
						// @DOCLINE * `@NLFT* left_side_bearings` - equivalent to "leftSideBearings" in the hmtx table. Its length is equivalent to `maxp->num_glyphs - hhea->number_of_hmetrics`.
						int16_m* left_side_bearings;
					};

					// @DOCLINE The struct `muttLongHorMetrics` has the following members:

					struct muttLongHorMetric {
						// @DOCLINE * `@NLFT advance_width` - equivalent to "advanceWidth" in the LongHorMetric record.
						uint16_m advance_width;
						// @DOCLINE * `@NLFT lsb` - equivalent to "lsb" in the LongHorMetric record.
						int16_m lsb;
					};

					// @DOCLINE All values provided in the `muttHmtx` struct (AKA the values in `muttLongHorMetrics`) are not checked, as virtually all of them have no technically "incorrect" values (from what I'm aware).

				// @DOCLINE ## Loca table

					// @DOCLINE The union `muttLoca` is used to represent the loca table provided by a TrueType font, stored in the struct `muttFont` as the pointer member "`loca`", and loaded with the flag `MUTT_LOAD_LOCA` (`MUTT_LOAD_MAXP`, `MUTT_LOAD_HEAD`, and `MUTT_LOAD_GLYF` must also be defined). It has the following members:

					union muttLoca {
						// @DOCLINE * `@NLFT* offsets16` - equivalent to the short-format offsets array in the loca table. This member is to be read from if `head->index_to_loc_format` is equal to `MUTT_OFFSET_16`.
						uint16_m* offsets16;
						// @DOCLINE * `@NLFT* offsets32` - equivalent to the long-format offsets array in the loca table. This member is to be read from if `head->index_to_loc_format` is equal to `MUTT_OFFSET_32`.
						uint32_m* offsets32;
					};

					// @DOCLINE The offsets are verified to be within range of the glyf table, along with all of the other rules within the specification.

				// @DOCLINE ## Name table

					typedef struct muttNameRecord muttNameRecord;
					typedef struct muttLangTagRecord muttLangTagRecord;

					// @DOCLINE The struct `muttName` is used to represent the name table provided by a TrueType font, stored in the struct `muttFont` as the pointer member "`name`", and loaded with the flag `MUTT_LOAD_NAME`. It has the following members:

					struct muttName {
						// @DOCLINE * `@NLFT version` - equivalent to "version" in the naming table header.
						uint16_m version;
						// @DOCLINE * `@NLFT count` - the amount of name records specified; equivalent to "count" in the naming table header.
						uint16_m count;
						// @DOCLINE * `@NLFT* name_records` - all [name records](#name-record) provided (length `count`); equivalent to "nameRecord" in the naming table header.
						muttNameRecord* name_records;
						// @DOCLINE * `@NLFT lang_tag_count` - the amount of language tags specified; equivalent to "langTagCount" in the naming table header.
						uint16_m lang_tag_count;
						// @DOCLINE * `@NLFT* lang_tag_records` - all [language tag records](#lang-tag-record) provided (length `lang_tag_count`); equivalent to "langTagRecord" in the naming table header.
						muttLangTagRecord* lang_tag_records;
						// @DOCLINE * `@NLFT* string_data` - the raw string data provided by the name table. All pointers to strings provided by the name table are pointers to parts of this data.
						muByte* string_data;
					};

					// @DOCLINE ### Name record

						// @DOCLINE The struct `muttNameRecord` represents a name record in TrueType. It has the following members:

						struct muttNameRecord {
							// @DOCLINE * `@NLFT platform_id` - the [platform ID](#platform-id); equivalent to "platformID" in the name record.
							uint16_m platform_id;
							// @DOCLINE * `@NLFT encoding_id` - the [encoding ID](#encoding-id); equivalent to "encodingID" in the name record.
							uint16_m encoding_id;
							// @DOCLINE * `@NLFT language_id` - the [language ID](#language-id); equivalent to "languageID" in the name record.
							uint16_m language_id;
							// @DOCLINE * `@NLFT name_id` - the [name ID](#name-id); equivalent to "nameID" in the name record.
							uint16_m name_id;
							// @DOCLINE * `@NLFT length` - the length of the string, in bytes; equivalent to "length" in the name record.
							uint16_m length;
							// @DOCLINE * `@NLFT* string` - a pointer to the string data stored within `muttName->string_data` for this given name record.
							muByte* string;
						};

						// @DOCLINE No platform, encoding, language, or name IDs give bad result values unless the specification explicitly states that the range of values that it's within will never be supported. The provided pointer for `string` is checked to be a pointer to valid data for the given length.

					// @DOCLINE ### Lang tag record

						// @DOCLINE The struct `muttLangTagRecord` represents a language tag in TrueType. It has the following members:

						struct muttLangTagRecord {
							// @DOCLINE * `@NLFT length` - the length of the string, in bytes; equivalent to "length" in the lang tag record.
							uint16_m length;
							// @DOCLINE * `@NLFT* lang_tag` - a pointer to the string data stored within `muttName->string_data` for this given name record.
							muByte* lang_tag;
						};

						// @DOCLINE The provided pointer for `lang_tag` is checked to be a pointer to valid data for the given length.

				// @DOCLINE ## Glyf table

						typedef struct muttGlyphHeader muttGlyphHeader;
						typedef struct muttSimpleGlyph muttSimpleGlyph;
						typedef struct muttCompositeGlyph muttCompositeGlyph;

						// @DOCLINE The struct `muttGlyf` is used to represent the glyf table provided by a TrueType font, stored in the struct `muttFont` as the pointer member "`glyf`", and loaded with the flag `MUTT_LOAD_GLYF` (`MUTT_LOAD_MAXP`, `MUTT_LOAD_HEAD`, and `MUTT_LOAD_LOCA` must also be defined). It has the following members:

						struct muttGlyf {
							// @DOCLINE * `@NLFT len` - the length of the glyf table, in bytes.
							uint32_m len;
							// @DOCLINE * `@NLFT* data` - the raw byte data of the glyf table (length `len`).
							muByte* data;
						};

						// @DOCLINE Unlike most low-level table structs, `muttGlyf` provides virtually no information about any glyphs upfront. This is because expanding every single glyph's information can be taxing, so instead, an API is provided to load individual glyph information using the `muttGlyf` struct, which is described below.

						// @DOCLINE ### Glyph header

							// @DOCLINE Every glyph, simple or composite, is described initially by its header, which is represented in mutt with the struct `muttGlyphHeader`, which has the following members:

							struct muttGlyphHeader {
								// @DOCLINE * `@NLFT number_of_contours` - equivalent to "numberOfContours" in the glyph header; if this value is negative, the glyph is composite, and if positive or zero, it is simple.
								int16_m number_of_contours;
								// @DOCLINE * `@NLFT x_min` - equivalent to "xMin" in the glyph header; minimum for x-coordinate data.
								int16_m x_min;
								// @DOCLINE * `@NLFT y_min` - equivalent to "yMin" in the glyph header; minimum for y-coordinate data.
								int16_m y_min;
								// @DOCLINE * `@NLFT x_max` - equivalent to "xMax" in the glyph header; maximum for x-coordinate data.
								int16_m x_max;
								// @DOCLINE * `@NLFT y_max` - equivalent to "yMax" in the glyph header; maximum for y-coordinate data.
								int16_m y_max;
								// @DOCLINE * `@NLFT* data` - a pointer to byte data in `glyf->data` after the header. This is primarily used internally by mutt.
								muByte* data;
								// @DOCLINE * `@NLFT length` - the length of the data after the header in bytes. If this member is equal to 0, the given glyph has no outline, and should not be called with any functions.
								uint32_m length;
							};

							// @DOCLINE The minimums and maximums for x- and y-coordinates within the glyph header are not checked initially (besides making sure the minimums are less than or equal to the maximums, and that they're within range of the values provided by the head table); if the actual glyph coordinates are not confined within the given minimums and maximums, a non-fatal result will be provided upon loading the simple glyph data. Either way, the header's listed x/y min/max values are overwritten with mutt's upon loading a simple glyph.

							// @DOCLINE #### Get glyph header

								// @DOCLINE In order to load a glyph header for a given glyph ID, the function `mutt_glyph_header` is used, defined below: @NLNT
								MUDEF muttResult mutt_glyph_header(muttFont* font, uint16_m glyph_id, muttGlyphHeader* header);

								// @DOCLINE Upon a non-fatal result, `header` is filled with valid header information for the given glyph ID. Upon a fatal result, the contents of `header` are undefined. The given header information is only valid for as long as `font` is not deloaded.

								// @DOCLINE `glyph_id` must be a valid glyph ID for the given font (AKA less than `font->head->num_glyphs`).

						// @DOCLINE ### Simple glyph

							typedef struct muttGlyphPoint muttGlyphPoint;

							// @DOCLINE The struct `muttSimpleGlyph` represents a simple glyph in mutt, and has the following members:

							struct muttSimpleGlyph {
								// @DOCLINE * `@NLFT* end_pts_of_contours` - equivalent to "endPtsOfContours" in the simple glyph table.
								uint16_m* end_pts_of_contours;
								// @DOCLINE * `@NLFT instruction_length` - equivalent to "instructionLength" in the simple glyph table; the length of `instructions`, in bytes.
								uint16_m instruction_length;
								// @DOCLINE * `@NLFT* instructions` - equivalent to "instructions" in the simple glyph table; the instructions for the given glyph.
								muByte* instructions;
								// @DOCLINE * `@NLFT* points` - each point for the simple glyph. The number of points is equal to `end_pts_of_contours[muttGlyphHeader->number_of_contours-1]+1` if `muttGlyphHeader->number_of_contours` is over 0; if `muttGlyphHeader->number_of_contours` is equal to 0, `points` will be equal to 0 as well.
								muttGlyphPoint* points;
							};

							// @DOCLINE The struct `muttGlyphPoint` represents a point in a simple glyph, and has the following members:
							struct muttGlyphPoint {
								// @DOCLINE * `@NLFT flags` - equivalent to a value within the "flags" array in the simple glyph table; the [flags of the given point](#glyph-point-flags).
								uint8_m flags;
								// @DOCLINE * `@NLFT x` - the x-coordinate of the point, in FUnits.
								int16_m x;
								// @DOCLINE * `@NLFT y` - the y-coordinate of the point, in FUnits.
								int16_m y;
							};

							// @DOCLINE #### Glyph point flags

								// @DOCLINE The following macros are defined for bitmasking a glyph point's flags:

								// @DOCLINE * [0x01] `MUTT_ON_CURVE_POINT`
								#define MUTT_ON_CURVE_POINT 0x01
								// @DOCLINE * [0x02] `MUTT_X_SHORT_VECTOR`
								#define MUTT_X_SHORT_VECTOR 0x02
								// @DOCLINE * [0x04] `MUTT_Y_SHORT_VECTOR`
								#define MUTT_Y_SHORT_VECTOR 0x04
								// @DOCLINE * [0x08] `MUTT_REPEAT_FLAG`
								#define MUTT_REPEAT_FLAG 0x08
								// @DOCLINE * [0x10] `MUTT_X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR`
								#define MUTT_X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR 0x10
								// @DOCLINE * [0x20] `MUTT_Y_IS_SAME_OR_POSITIVE_Y_SHORT_VECTOR`
								#define MUTT_Y_IS_SAME_OR_POSITIVE_Y_SHORT_VECTOR 0x20
								// @DOCLINE * [0x40] `MUTT_OVERLAP_SIMPLE`
								#define MUTT_OVERLAP_SIMPLE 0x40

								// @DOCLINE Note that since the value of `flags` is directly copied from the raw TrueType data, usage of these macros is optional, and the user can bitmask as they please in accordance to the TrueType specification.

							// @DOCLINE #### Load simple glyph

								// @DOCLINE In order to load a simple glyph, the function `mutt_simple_glyph` is used, defined below: @NLNT
								MUDEF muttResult mutt_simple_glyph(muttFont* font, muttGlyphHeader* header, muttSimpleGlyph* glyph, muByte* data, uint32_m* written);

								// @DOCLINE Upon a non-fatal result, `glyph` is filled with valid simple glyph information for the given glyph ID using memory from `data`. Upon a fatal result, the contents of `glyph` and `data` are undefined. The given glyph information is only valid for as long as `font` is not deloaded, and as long as `data` goes unmodified.

								// @DOCLINE This function follows the format of a user-allocated function. For an explanation of how `data` and `written` are supposed to be used within this function, see [the user-allocated function section](#user-allocated-functions).

								// @DOCLINE The x/y min/max values within `header` are overwritten upon a call to this function with correct values. If the values provided in `header` were invalid before mutt overwrites them, the non-fatal result values `MUTT_INVALID_GLYF_SIMPLE_X_COORD` or `MUTT_INVALID_GLYF_SIMPLE_X_COORD` will be given.

								// @DOCLINE > This function checks if all of the values are compliant with information from other tables (especially maxp) and compliant with TrueType's specification with a few exceptions: as far as I'm aware, it's invalid to have a flag that uses values from a prior point (such as X_IS_SAME...) when the current flag is the first flag specified, since in that case, there's no "previous value" to repeat from. This is done in several common fonts, however, so mutt permits this, setting the value to 0 in this case.

								// @DOCLINE > It's also invalid (from what I'm aware) to have the first point be off-curve, but in the case that such happens, mutt permits this, pretending that the previous point was an on-curve point at (0,0). It's also invalid (from what I'm aware) to have a repeat flag count that exceeds the amount of points, but since it's easy to internally make sure to simply not go over the point count, mutt permits this.

							// @DOCLINE #### Simple glyph memory maximum

								// @DOCLINE The maximum amount of memory that will be needed for loading a simple glyph, in bytes, is provided by the function `mutt_simple_glyph_max_size`, defined below: @NLNT
								MUDEF uint32_m mutt_simple_glyph_max_size(muttFont* font);

							// @DOCLINE #### Simple glyph min/max

								// @DOCLINE The minimum/maximum x/y ranges for a simple glyph can be calculated using the function `mutt_simple_glyph_min_max`, defined below: @NLNT
								MUDEF muttResult mutt_simple_glyph_min_max(muttFont* font, muttGlyphHeader* header);

								// @DOCLINE The values `x_min`, `y_min`, `x_max`, and `y_max` are filled in for `header` upon a non-fatal result. The given glyph must have at least one contour.

							// @DOCLINE #### Simple glyph point count

								// @DOCLINE Getting just the amount of points within a simple glyph based on its header is a fairly cheap operation, requiring no manual allocation. The function `mutt_simple_glyph_points` calculates the amount of points that a simple glyph contains, defined below: @NLNT
								MUDEF muttResult mutt_simple_glyph_points(muttFont* font, muttGlyphHeader* header, uint16_m* num_points);

								// @DOCLINE Upon a non-fatal result, `num_points` is dereferenced and set to the amount of points within the simple glyph indicated by `header`.

						// @DOCLINE ### Composite glyph

							typedef struct muttComponentGlyph muttComponentGlyph;

							// @DOCLINE The struct `muttCompositeGlyph` represents a composite glyph in mutt, and has the following members:

							struct muttCompositeGlyph {
								// @DOCLINE * `@NLFT component_count` - the number of components within the composite glyph.
								uint16_m component_count;
								// @DOCLINE * `@NLFT* components` - an array of each component within the composite glyph.
								muttComponentGlyph* components;
								// @DOCLINE * `@NLFT instruction_length` - the length of the instructions for the composite glyph, in bytes.
								uint16_m instruction_length;
								// @DOCLINE * `@NLFT* instructions` - the instructions for the composite glyph.
								muByte* instructions;
							};

							// @DOCLINE The struct `muttComponentGlyph` represents a component in a composite glyph, and has the following members:
							struct muttComponentGlyph {
								// @DOCLINE * `@NLFT flags` - equivalent to "flags" in the component glyph record; the [flags for the given component glyph](#glyph-component-flags).
								uint16_m flags;
								// @DOCLINE * `@NLFT glyph_index` - equivalent to "glyphIndex" in the component glyph record; the glyph ID of the given component.
								uint16_m glyph_index;
								// @DOCLINE * `@NLFT argument1` - equivalent to "argument1" in the component glyph record.
								int32_m argument1;
								// @DOCLINE * `@NLFT argument2` - equivalent to "argument2" in the component glyph record.
								int32_m argument2;
								// @DOCLINE * `@NLFT scales[4]` - the transform data of the component.
								float scales[4];
							};

							// @DOCLINE The data of `scales` depends on the value of `flags` (see TrueType/OpenType documentation for more information on how this data works); the following conditions exist:

							// @DOCLINE * If the `MUTT_WE_HAVE_A_SCALE` bit is 1, `scales[0]` is the scale; the contents of all other float indexes are undefined.
							// @DOCLINE * If the `MUTT_WE_HAVE_AN_X_AND_Y_SCALE` bit is 1, `scales[0]` and `scales[1]` are the x- and y-scales respectively; the contents of all other float indexes are undefined.
							// @DOCLINE * If the `MUTT_WE_HAVE_A_TWO_BY_TWO` bit is 1, `scales[0]`, `scales[1]`, `scales[2]`, and `scales[3]` are the 2-by-2 affine transformation values (xscale, scale01, scale10, and yscale respectively).
							// @DOCLINE * If none of the bits mentioned above are 1, the values of `scales` are undefined.

							// @DOCLINE The value for `glyph_index` is not verified to be a non-infinite loop of composite glyphs, and must be manually checked for by the user, unless being converted to a pixel glyph, in which case the conversion checks for this case.

							// @DOCLINE #### Glyph component flags

								// @DOCLINE The following macros are defined for bitmasking a glyph component's flags:

								// @DOCLINE * [0x0001] `MUTT_ARG_1_AND_2_ARE_WORDS`
								#define MUTT_ARG_1_AND_2_ARE_WORDS 0x0001
								// @DOCLINE * [0x0002] `MUTT_ARGS_ARE_XY_VALUES`
								#define MUTT_ARGS_ARE_XY_VALUES 0x0002
								// @DOCLINE * [0x0004] `MUTT_ROUND_XY_TO_GRID`
								#define MUTT_ROUND_XY_TO_GRID 0x0004
								// @DOCLINE * [0x0008] `MUTT_WE_HAVE_A_SCALE`
								#define MUTT_WE_HAVE_A_SCALE 0x0008
								// @DOCLINE * [0x0020] `MUTT_MORE_COMPONENTS`
								#define MUTT_MORE_COMPONENTS 0x0020
								// @DOCLINE * [0x0040] `MUTT_WE_HAVE_AN_X_AND_Y_SCALE`
								#define MUTT_WE_HAVE_AN_X_AND_Y_SCALE 0x0040
								// @DOCLINE * [0x0080] `MUTT_WE_HAVE_A_TWO_BY_TWO`
								#define MUTT_WE_HAVE_A_TWO_BY_TWO 0x0080
								// @DOCLINE * [0x0100] `MUTT_WE_HAVE_INSTRUCTIONS`
								#define MUTT_WE_HAVE_INSTRUCTIONS 0x0100
								// @DOCLINE * [0x0200] `MUTT_USE_MY_METRICS`
								#define MUTT_USE_MY_METRICS 0x0200
								// @DOCLINE * [0x0400] `MUTT_OVERLAP_COMPOUND`
								#define MUTT_OVERLAP_COMPOUND 0x0400
								// @DOCLINE * [0x0800] `MUTT_SCALED_COMPONENT_OFFSET`
								#define MUTT_SCALED_COMPONENT_OFFSET 0x0800
								// @DOCLINE * [0x1000] `MUTT_UNSCALED_COMPONENT_OFFSET`
								#define MUTT_UNSCALED_COMPONENT_OFFSET 0x1000

								// @DOCLINE Note that since the value of `flags` is retrieved from the TrueType data, usage of these macros is optional, and the user can bitmask as they please in accordance to the TrueType specification.

							// @DOCLINE #### Load composite glyph

								// @DOCLINE In order to load a composite glyph, the function `mutt_composite_glyph` is used, defined below: @NLNT
								MUDEF muttResult mutt_composite_glyph(muttFont* font, muttGlyphHeader* header, muttCompositeGlyph* glyph, muByte* data, uint32_m* written);

								// @DOCLINE Upon a non-fatal result, `glyph` is filled with valid composite glyph information for the given glyph ID using memory from `data`. Upon a fatal result, the contents of `glyph` and `data` are undefined. The given glyph information is only valid for as long as `font` is not deloaded, and as long as `data` goes unmodified.

								// @DOCLINE This function follows the format of a user-allocated function. For an explanation of how `data` and `written` are supposed to be used within this function, see [the user-allocated function section](#user-allocated-functions).

								// @DOCLINE This function performs no checks on the validity of the components' range within the minimum/maximum coordinate ranges specified for the glyph in the respective header. Therefore, this function does allow composite glyphs to successfully load that have points that are out of range. This is due to the fact that properly verifying the points' coordinates would entail fully decompressing the composite glyph's components, which is not performed in the lower-level API of mutt.

							// @DOCLINE #### Composite glyph memory maximum

								// @DOCLINE The maximum amount of memory that will be needed for loading a composite glyph, in bytes, is provided by the function `mutt_composite_glyph_max_size`, defined below: @NLNT
								MUDEF uint32_m mutt_composite_glyph_max_size(muttFont* font);

							// @DOCLINE #### Composite min max

								// @DOCLINE The minimum/maximum x/y ranges for a composite glyph can be calculated using the function `mutt_composite_glyph_min_max`, defined below: @NLNT
								MUDEF muttResult mutt_composite_glyph_min_max(muttFont* font, muttGlyphHeader* header);

								// @DOCLINE The values `x_min`, `y_min`, `x_max`, and `y_max` are filled in for `header` upon a non-fatal result. The given glyph must have at least one contour.

								// @DOCLINE Since composite glyphs allow for non-integer coordinates, the coordinates filled in are the ceiling equivalents.

							// @DOCLINE #### Composite glyph component retrieval

								// @DOCLINE A composite glyph can be processed component-by-component using the function `mutt_composite_component`, defined below: @NLNT
								MUDEF muttResult mutt_composite_component(muttFont* font, muttGlyphHeader* header, muByte** prog, muttComponentGlyph* component, muBool* no_more);

								// @DOCLINE The upside of this is that it requires no upfront manual allocation. This function performs the same checks for validity as `mutt_composite_glyph` besides the component count being valid, which must be tracked by the user themself.

								// @DOCLINE `prog` is a pointer to a pointer holding where mutt is reading TrueType data from, and should be initialized to `header->data` on the first call to `mutt_composite_component`.

								// @DOCLINE Upon a non-fatal result, `component` will be dereferenced & set to the next component within the composite glyph (or the first component upon the first call to `mutt_composite_component`). `prog` will be incremented to the location of the next component, and if there *isn't* a next component, `no_more` will be dereferenced and set to `MU_TRUE`.

							// @DOCLINE #### Composite glyph glyph indexes

								// @DOCLINE The ability to go through each component within a composite glyph individually to get their glyph indexes with limited error checking is provided by the function `mutt_composite_component_glyph`, defined below: @NLNT
								MUDEF muttResult mutt_composite_component_glyph(muttFont* font, muttGlyphHeader* header, muByte** prog, uint16_m* glyph_index, muBool* no_more);

								// @DOCLINE `prog` is a pointer to a pointer holding where mutt is reading TrueType data from, and should be initialized to `header->data` on the first call to `mutt_composite_component_glyph`.

								// @DOCLINE Upon a non-fatal result, `glyph_index` will be dereferenced & set to the glyph index of the current component, `prog` will be incremented to the next component, and if there *isn't* a next component, `no_more` will be dereferenced and set to `MU_TRUE`.

								// @DOCLINE Limited error checking is performed on the composite glyph with this function. For example, the amount of components is not checked to be valid, nor the flag exclusivity of the transform flags when moving past them. The user should make sure that, when calling this function, they make sure that the number of components being scanned is valid when compared to the maximums listed within the maxp table. The glyph index is checkd to be valid before being given to the user.

						// @DOCLINE ### Max glyph size

							// @DOCLINE The maximum amount of memory that will be needed for loading a glyph, simple or composite, in bytes, is provided by the function `mutt_glyph_max_size`, defined below: @NLNT
							MUDEF uint32_m mutt_glyph_max_size(muttFont* font);

							// @DOCLINE This function returns the largest value between `mutt_simple_glyph_max_size` and `mutt_composite_glyph_max_size`.

				// @DOCLINE ## Cmap table

					typedef struct muttEncodingRecord muttEncodingRecord;
					typedef union muttCmapFormat muttCmapFormat;
					typedef struct muttCmap0 muttCmap0;
					typedef struct muttCmap4 muttCmap4;
					typedef struct muttCmap12 muttCmap12;

					// @DOCLINE The struct `muttCmap` is used to represent the cmap table provided by a TrueType font, stored in the struct `muttFont` as the pointer member "`cmap`", and loaded with the flag `MUTT_LOAD_CMAP` (`MUTT_LOAD_MAXP` must also be defined). It has the following members:

					struct muttCmap {
						// @DOCLINE * `@NLFT num_tables` - equivalent to "numTables" in the cmap header; the number of encoding records in the `encoding_records` array.
						uint16_m num_tables;
						// @DOCLINE * `@NLFT* encoding_records` - equivalent to "encodingRecords" in the cmap header; an array of each encoding record in the cmap table.
						muttEncodingRecord* encoding_records;
					};

					// @DOCLINE The union `muttCmapFormat` represents a cmap format for a cmap encoding record. It has the following members:
					union muttCmapFormat {
						// @DOCLINE * `@NLFT* f0` - [format 0](#format-0).
						muttCmap0* f0;
						// @DOCLINE * `@NLFT* f4` - [format 4](#format-4).
						muttCmap4* f4;
						// @DOCLINE * `@NLFT* f12` - [format 12](#format-12).
						muttCmap12* f12;
					};

					// @DOCLINE The struct `muttEncodingRecord` represents an encoding record in the cmap table. It has the following members:
					struct muttEncodingRecord {
						// @DOCLINE * `@NLFT platform_id` - equivalent to "platformID" in the cmap encoding record; the platform ID of the encoding record.
						uint16_m platform_id;
						// @DOCLINE * `@NLFT encoding_id` - equivalent to "encodingID" in the cmap encoding record; the encoding ID of the encoding record.
						uint16_m encoding_id;
						// @DOCLINE * `@NLFT format` - equivalent to "format" in a given cmap subtable format; the format of the encoding record.
						uint16_m format;
						// @DOCLINE * `@NLFT encoding` - a union holding a pointer to the information for the cmap subtable format.
						muttCmapFormat encoding;
						// @DOCLINE * `@NLFT result` - the result of attempting to load the cmap subtable format.
						muttResult result;
					};

					// @DOCLINE If `result` is a fatal result (most commonly due to the format not being supported (`MUTT_INVALID_CMAP_ENCODING_RECORD_FORMAT`)), the relevant member of `encoding` is 0, unless the format is unsupported, in which case the value of `encoding` is undefined.

					// @DOCLINE The following sections detail how to convert codepoint values to glyph ID values and vice versa, using (at the highest level) the cmap table as a whole, and (at the lowest level) reading data from each format's struct. ***WARNING:*** The glyph ID values returned by a cmap format subtable are *not* checked to be valid upon loading; they are checked to be valid once a function is called for conversion. This means that if the user is using the structs of each format to get glyph ID values instead of using the respective functions, they need to check if the glyph ID values that they retrieve are valid or not, since they are unchecked. Normal checks for all values being within data range are still performed; for example, the loading of format 4 checks if all values within its segments generate a valid index into its glyphIdArray, it just doesn't check if the values within glyphIdArray are valid glyph IDs, that it performed in `mutt_cmap4_get_glyph`.

					// @DOCLINE Codepoint values passed into functions for conversion can be invalid; 0 shall just be returned. Codepoint values returned by a conversion function may not be valid codepoint values for the relevant encoding; the user must check these values themselves. Invalid glyph ID values should not be passed into conversion functions, and conversion functions should not return invalid glyph ID values.

					// @DOCLINE ### Top-level cmap

						// @DOCLINE Every implemented cmap format in mutt can retrieve a glyph ID based on a given codepoint and vice versa.

						// @DOCLINE The function `mutt_get_glyph` searches each cmap encoding record specified for the given font and attempts to convert the given codepoint value to a valid glyph ID, defined below: @NLNT
						MUDEF uint16_m mutt_get_glyph(muttFont* font, uint32_m codepoint);

						// @DOCLINE The function `mutt_get_codepoint` searches each cmap encoding record specified for the given font and attempts to convert the given glyph ID to a codepoint value, defined below: @NLNT
						MUDEF uint32_m mutt_get_codepoint(muttFont* font, uint16_m glyph_id);

						// @DOCLINE Both functions return 0 if no equivalent could be found in the conversion process for any cmap encoding record.

					// @DOCLINE ### Cmap encoding

						// @DOCLINE The function `mutt_cmap_encoding_get_glyph` converts a given codepoint value to a glyph ID value using the given cmap encoding record, defined below: @NLNT
						MUDEF uint16_m mutt_cmap_encoding_get_glyph(muttFont* font, muttEncodingRecord* record, uint32_m codepoint);

						// @DOCLINE The function `mutt_cmap_encoding_get_codepoint` converts a given glyph ID to a codepoint value using the given cmap encoding record, defined below: @NLNT
						MUDEF uint32_m mutt_cmap_encoding_get_codepoint(muttFont* font, muttEncodingRecord* record, uint16_m glyph_id);

						// @DOCLINE Both functions return 0 if no equivalent could be found in the conversion process for the format of the given encoding record.

					// @DOCLINE ### Format 0

						// @DOCLINE The struct `muttCmap0` represents a cmap format 0 subtable, and has the following members:
						struct muttCmap0 {
							// @DOCLINE * `@NLFT language` - equivalent to "language" in the cmap format 0 subtable.
							uint16_m language;
							// @DOCLINE * `@NLFT glyph_ids[256]` - equivalent to "glyphIdArray" in the cmap format 0 subtable.
							uint8_m glyph_ids[256];
						};

						// @DOCLINE The function `mutt_cmap0_get_glyph` converts a given codepoint value to a glyph ID value using the given format 0 cmap subtable, defined below: @NLNT
						MUDEF uint16_m mutt_cmap0_get_glyph(muttFont* font, muttCmap0* f0, uint8_m codepoint);

						// @DOCLINE The function `mutt_cmap0_get_codepoint` converts a given glyph ID to a codepoint value using the given format 0 cmap subtable, defined below: @NLNT
						MUDEF uint8_m mutt_cmap0_get_codepoint(muttFont* font, muttCmap0* f0, uint16_m glyph);

						// @DOCLINE Both functions return 0 if no equivalent could be found in the conversion process.

					// @DOCLINE ### Format 4

						typedef struct muttCmap4Segment muttCmap4Segment;

						// @DOCLINE The struct `muttCmap4` represents a cmap format 4 subtable, and has the following members:
						struct muttCmap4 {
							// @DOCLINE * `@NLFT language` - equivalent to "language" in the cmap format 4 subtable.
							uint16_m language;
							// @DOCLINE * `@NLFT seg_count` - equivalent to half of "segCountX2" in the cmap format 4 subtable; the amount of segments in the `seg` array.
							uint16_m seg_count;
							// @DOCLINE * `@NLFT* seg` - an array of each segment within the cmap format 4 subtable.
							muttCmap4Segment* seg;
							// @DOCLINE * `@NLFT* glyph_ids` - equivalent to "glyphIdArray" in the cmap format 4 subtable; the glyph index array that each segment should return indexes into.
							uint16_m* glyph_ids;
						};

						// @DOCLINE Internally, mutt does not verify or use the values for "searchRange", "entrySelector", or "rangeShift".

						// @DOCLINE The struct `muttCmap4Segment` represents a segment in the cmap format 4 subtable, and has the following members:
						struct muttCmap4Segment {
							// @DOCLINE * `@NLFT end_code` - equivalent to the value for the given segment in the "endCode" array in the cmap format 4 subtable; the end character code for the given segment.
							uint16_m end_code;
							// @DOCLINE * `@NLFT start_code` - equivalent to the value for the given segment in the "startCode" array in the cmap format 4 subtable; the start character code for the given segment.
							uint16_m start_code;
							// @DOCLINE * `@NLFT id_delta` - equivalent to the value for the given segment in the "idDelta" array in the cmap format 4 subtable; the delta for the character codes of the given segment.
							int16_m id_delta;
							// @DOCLINE * `@NLFT id_range_offset` - equivalent to the value for the given segment in the "idRangeOffset" array in the cmap format 4 subtable, but divided by 2 and with (`muttCmap4->seg_count` - the index for the given segment) subtracted; the start code index offset into `muttCmap4->glyph_ids`.
							uint16_m id_range_offset;
							// @DOCLINE * `@NLFT id_range_offset_orig` - equivalent to the value for the given segment in the "idRangeOffset" array in the cmap format 4 subtable.
							uint16_m id_range_offset_orig;
							// @DOCLINE * `@NLFT start_glyph_id` - the calculated first glyph ID of the segment. This is not checked to be a valid glyph ID, and is used when converting glyph IDs into codepoints.
							uint16_m start_glyph_id;
							// @DOCLINE * `@NLFT end_glyph_id` - the calculated last glyph ID of the segment. This is not checked to be a valid glyph ID, and is used when converting glyph IDs into codepoints.
							uint16_m end_glyph_id;
						};

						// @DOCLINE > In TrueType, idRangeOffset is stored as an offset into the glyphIdArray for the start code, to which every code in the segment adds an offset to get the next value (`codepoint-start_code` incrementally offsets, beginning at the start code as 0). However, this offset is stored relative to `&idRangeOffset[segment]`, so this offset is accounted for when loading the format in mutt by subtracting `seg_count-segment` for each value in idRangeOffset. The value is also first divided by 2 (before the subtraction) since, internally, the value is accessed by directly indexing into `glyph_ids`, but idRangeOffset values store actual byte offsets, so the 2 bytes per glyph ID must be accounted for. Glyph IDs are then internally retrieved (after being verified to be valid offsets) via `f4->glyph_ids[seg->id_range_offset + (codepoint - seg->start_code)]` (followed by delta logic), and this is how the user should also retrieve them when manually reading from the `muttCmap4` struct.

						// @DOCLINE The function `mutt_cmap4_get_glyph` converts a given codepoint value to a glyph ID value using the given format 4 cmap subtable, defined below: @NLNT
						MUDEF uint16_m mutt_cmap4_get_glyph(muttFont* font, muttCmap4* f4, uint16_m codepoint);

						// @DOCLINE The function `mutt_cmap4_get_codepoint` converts a given glyph ID to a codepoint value using the given format 4 cmap subtable, defined below: @NLNT
						MUDEF uint16_m mutt_cmap4_get_codepoint(muttFont* font, muttCmap4* f4, uint16_m glyph);

						// @DOCLINE Both functions return 0 if no equivalent could be found in the conversion process.

					// @DOCLINE ### Format 12

						typedef struct muttCmap12Group muttCmap12Group;

						// @DOCLINE The struct `muttCmap12` represents a cmap format 12 subtable, and has the following members:
						struct muttCmap12 {
							// @DOCLINE * `@NLFT language` - equivalent to "language" in the cmap format 12 subtable.
							uint32_m language;
							// @DOCLINE * `@NLFT num_groups` - equivalent to "numGroups" in the cmap format 12 subtable; the amount of groups in the `groups` array.
							uint32_m num_groups;
							// @DOCLINE * `@NLFT* groups` - equivalent to "groups" in the cmap format 12 subtable; an array of each map group.
							muttCmap12Group* groups;
						};

						// @DOCLINE The struct `muttCmap12Group` represents a sequential map group in the cmap format 12 subtable, and has the following members:
						struct muttCmap12Group {
							// @DOCLINE * `@NLFT start_char_code` - equivalent to "startCharCode" in the sequential map group record; the first character code for the given group.
							uint32_m start_char_code;
							// @DOCLINE * `@NLFT end_char_code` - equivalent to "endCharCode" in the sequential map group record; the last character code for the given group.
							uint32_m end_char_code;
							// @DOCLINE * `@NLFT start_glyph_id` - equivalent to "startGlyphID" in the sequential map group record; the glyph ID for the first character code.
							uint32_m start_glyph_id;
						};

						// @DOCLINE The function `mutt_cmap12_get_glyph` converts a given codepoint value to a glyph ID value using the given format 12 cmap subtable, defined below: @NLNT
						MUDEF uint16_m mutt_cmap12_get_glyph(muttFont* font, muttCmap12* f12, uint32_m codepoint);

						// @DOCLINE The function `mutt_cmap12_get_codepoint` converts a given glyph ID to a codepoint value using the given format 12 cmap subtable, defined below: @NLNT
						MUDEF uint32_m mutt_cmap12_get_codepoint(muttFont* font, muttCmap12* f12, uint16_m glyph);

						// @DOCLINE Both functions return 0 if no equivalent could be found in the conversion process.

				// @DOCLINE ## User allocated functions

					/* @DOCBEGIN
					"User-allocated functions" are functions used in mutt to allow the user to handle allocation of memory necessary to perform certain low-level operations. These functions usually have two distinct members that make this possible, `muByte* data` and `uint32_m* written`, with the functions usually following the format of:

					```c
					MUDEF muttResult mutt_whatever(muttFont* font, ... muByte* data, uint32_m* written);
					```

					The way these functions work depends on the value of `data`:

					* If `data` is 0, `written` is dereferenced and set to how many bytes are needed within `data` in order to perform the operation.
					
					* If `data` is not 0, `data` is used as allocated memory to perform whatever task is needed, and assumes that `data` is large enough to perform the operation. If `written` is not 0 in this instance, `written` is dereferenced and set to how much data was used in the operation.

					These functions are setup so that the user has full control over the allocation of the data for the given process.

					If these functions are ever used to fill information about something (in particular, if it's filling up a struct like `mutt_simple_glyph` or `mutt_composite_glyph`), the filled-out structs use `data` as memory for the information provided by them. This means that the information with the struct is only valid for as long as `data` goes unmodified by the user.

					User-allocated functions do not expect the given memory to be zero'd out, and the contents of `data`, once filled in, are undefined.

					User-allocated functions usually also have provided maximums for the maximum amount of memory that will be needed to perform the operation for any given variables, which can be used to pre-allocate and use the same memory for multiple passes of the operation. For example, `mutt_simple_glyph` has a memory maximum provided by the function `mutt_simple_glyph_max_size`, which means that a user can allocate memory of byte-length `mutt_simple_glyph_max_size(...)` and use that same memory for a call to `mutt_simple_glyph` for any glyphs that they need to load, avoiding the need to reallocate any memory when processing simple glyphs, although at the cost of only being able to process one simple glyph at a time.
					@DOCEND */

				// @DOCLINE ## String macros

					// @DOCLINE This section covers macros defined for platform, encoding, language, and name IDs. Note that values may be given that don't fit into any of the given macros.

					// @DOCLINE ### Platform ID

						// @DOCLINE The following macros are defined for platform IDs:

						// @DOCLINE * [0x0000] `MUTT_PLATFORM_UNICODE` - [Unicode platform](#unicode-encoding).
						#define MUTT_PLATFORM_UNICODE 0x0000
						// @DOCLINE * [0x0001] `MUTT_PLATFORM_MACINTOSH` - [Macintosh platform](#macintosh-encoding).
						#define MUTT_PLATFORM_MACINTOSH 0x0001
						// @DOCLINE * [0x0002] `MUTT_PLATFORM_ISO` - [ISO platform](#iso-encoding).
						#define MUTT_PLATFORM_ISO 0x0002
						// @DOCLINE * [0x0003] `MUTT_PLATFORM_WINDOWS` - [Windows platform](#windows-encoding).
						#define MUTT_PLATFORM_WINDOWS 0x0003
						// @DOCLINE * [0x0004] `MUTT_PLATFORM_CUSTOM` - custom encoding.
						#define MUTT_PLATFORM_CUSTOM 0x0004

						#ifdef MUTT_NAMES
						// @DOCLINE #### Platform ID names

							// @DOCLINE The name function `mutt_platform_get_name` returns a `const char*` representation of a given platform ID (for example, `MUTT_PLATFORM_UNICODE` returns "MUTT_PLATFORM_UNICODE"), defined below: @NLNT
							MUDEF const char* mutt_platform_get_name(uint16_m platform_id);
							// @DOCLINE This function returns "MU_UNKNOWN" in the case that `platform_id` is an unrecognized value.

							// @DOCLINE The name function `mutt_platform_get_nice_name` does the same thing, but returns a more readable version of it (for example, `MUTT_PLATFORM_UNICODE` returns "Unicode"), defined below: @NLNT
							MUDEF const char* mutt_platform_get_nice_name(uint16_m platform_id);
							// @DOCLINE This function returns "Unknown" in the case that `platform_id` is an unrecognized value.

							// @DOCLINE > Note that these are name functions, and are only defined if `MUTT_NAMES` is also defined.
						#endif /* MUTT_NAMES */

					// @DOCLINE ### Encoding ID

						// @DOCLINE The following macros are defined for various platform encoding IDs:

						// @DOCLINE #### Unicode encoding

							// @DOCLINE * [0x0000] `MUTT_UNICODE_1_0` - Unicode 1.0.
							#define MUTT_UNICODE_1_0 0x0000
							// @DOCLINE * [0x0001] `MUTT_UNICODE_1_1` - Unicode 1.1.
							#define MUTT_UNICODE_1_1 0x0001
							// @DOCLINE * [0x0002] `MUTT_UNICODE_ISO_IEC_10646` - ISO/IEC 10646.
							#define MUTT_UNICODE_ISO_IEC_10646 0x0002
							// @DOCLINE * [0x0003] `MUTT_UNICODE_2_0_BMP` - Unicode 2.0, BMP only.
							#define MUTT_UNICODE_2_0_BMP 0x0003
							// @DOCLINE * [0x0004] `MUTT_UNICODE_2_0` - Unicode 2.0.
							#define MUTT_UNICODE_2_0 0x0004
							// @DOCLINE * [0x0005] `MUTT_UNICODE_VARIATION` - Unicode variation sequences.
							#define MUTT_UNICODE_VARIATION 0x0005
							// @DOCLINE * [0x0006] `MUTT_UNICODE_FULL` - Unicode "full repertoire".
							#define MUTT_UNICODE_FULL 0x0006

							#ifdef MUTT_NAMES
							// @DOCLINE ##### Unicode encoding names

								// @DOCLINE The name function `mutt_unicode_encoding_get_name` returns a `const char*` representation of a given Unicode encoding ID (for example, `MUTT_UNICODE_1_0` returns "MUTT_UNICODE_1_0"), defined below: @NLNT
								MUDEF const char* mutt_unicode_encoding_get_name(uint16_m encoding_id);
								// @DOCLINE This function returns "MU_UNKNOWN" in the case that `encoding_id` is an unrecognized value.

								// @DOCLINE The name function `mutt_unicode_encoding_get_nice_name` does the same thing, but returns a more readable version of it (for example, `MUTT_UNICODE_1_0` returns "Unicode 1.0"), defined below: @NLNT
								MUDEF const char* mutt_unicode_encoding_get_nice_name(uint16_m encoding_id);
								// @DOCLINE This function returns "Unknown" in the case that `encoding_id` is an unrecognized value.

								// @DOCLINE > Note that these are name functions, and are only defined if `MUTT_NAMES` is also defined.
							#endif /* MUTT_NAMES */

						// @DOCLINE #### Macintosh encoding

							// @DOCLINE * [0x0000] `MUTT_MACINTOSH_ROMAN` - Roman.
							#define MUTT_MACINTOSH_ROMAN 0x0000
							// @DOCLINE * [0x0001] `MUTT_MACINTOSH_JAPANESE` - Japanese.
							#define MUTT_MACINTOSH_JAPANESE 0x0001
							// @DOCLINE * [0x0002] `MUTT_MACINTOSH_CHINESE_TRADITIONAL` - Chinese (Traditional).
							#define MUTT_MACINTOSH_CHINESE_TRADITIONAL 0x0002
							// @DOCLINE * [0x0003] `MUTT_MACINTOSH_KOREAN` - Korean.
							#define MUTT_MACINTOSH_KOREAN 0x0003
							// @DOCLINE * [0x0004] `MUTT_MACINTOSH_ARABIC` - Arabic.
							#define MUTT_MACINTOSH_ARABIC 0x0004
							// @DOCLINE * [0x0005] `MUTT_MACINTOSH_HEBREW` - Hebrew.
							#define MUTT_MACINTOSH_HEBREW 0x0005
							// @DOCLINE * [0x0006] `MUTT_MACINTOSH_GREEK` - Greek.
							#define MUTT_MACINTOSH_GREEK 0x0006
							// @DOCLINE * [0x0007] `MUTT_MACINTOSH_RUSSIAN` - Russian.
							#define MUTT_MACINTOSH_RUSSIAN 0x0007
							// @DOCLINE * [0x0008] `MUTT_MACINTOSH_RSYMBOL` - RSymbol.
							#define MUTT_MACINTOSH_RSYMBOL 0x0008
							// @DOCLINE * [0x0009] `MUTT_MACINTOSH_DEVANAGARI` - Devanagari.
							#define MUTT_MACINTOSH_DEVANAGARI 0x0009
							// @DOCLINE * [0x000A] `MUTT_MACINTOSH_GURMUKHI` - Gurmukhi.
							#define MUTT_MACINTOSH_GURMUKHI 0x000A
							// @DOCLINE * [0x000B] `MUTT_MACINTOSH_GUJARATI` - Gujarati.
							#define MUTT_MACINTOSH_GUJARATI 0x000B
							// @DOCLINE * [0x000C] `MUTT_MACINTOSH_ODIA` - Odia.
							#define MUTT_MACINTOSH_ODIA 0x000C
							// @DOCLINE * [0x000D] `MUTT_MACINTOSH_BANGLA` - Bangla.
							#define MUTT_MACINTOSH_BANGLA 0x000D
							// @DOCLINE * [0x000E] `MUTT_MACINTOSH_TAMIL` - Tamil.
							#define MUTT_MACINTOSH_TAMIL 0x000E
							// @DOCLINE * [0x000F] `MUTT_MACINTOSH_TELUGU` - Telugu.
							#define MUTT_MACINTOSH_TELUGU 0x000F
							// @DOCLINE * [0x0010] `MUTT_MACINTOSH_KANNADA` - Kannada.
							#define MUTT_MACINTOSH_KANNADA 0x0010
							// @DOCLINE * [0x0011] `MUTT_MACINTOSH_MALAYALAM` - Malayalam.
							#define MUTT_MACINTOSH_MALAYALAM 0x0011
							// @DOCLINE * [0x0012] `MUTT_MACINTOSH_SINHALESE` - Sinhalese.
							#define MUTT_MACINTOSH_SINHALESE 0x0012
							// @DOCLINE * [0x0013] `MUTT_MACINTOSH_BURMESE` - Burmese.
							#define MUTT_MACINTOSH_BURMESE 0x0013
							// @DOCLINE * [0x0014] `MUTT_MACINTOSH_KHMER` - Khmer.
							#define MUTT_MACINTOSH_KHMER 0x0014
							// @DOCLINE * [0x0015] `MUTT_MACINTOSH_THAI` - Thai.
							#define MUTT_MACINTOSH_THAI 0x0015
							// @DOCLINE * [0x0016] `MUTT_MACINTOSH_LAOTIAN` - Laotian.
							#define MUTT_MACINTOSH_LAOTIAN 0x0016
							// @DOCLINE * [0x0017] `MUTT_MACINTOSH_GEORGIAN` - Georgian.
							#define MUTT_MACINTOSH_GEORGIAN 0x0017
							// @DOCLINE * [0x0018] `MUTT_MACINTOSH_ARMENIAN` - Armenian.
							#define MUTT_MACINTOSH_ARMENIAN 0x0018
							// @DOCLINE * [0x0019] `MUTT_MACINTOSH_CHINESE_SIMPLIFIED` - Chinese (Simplified).
							#define MUTT_MACINTOSH_CHINESE_SIMPLIFIED 0x0019
							// @DOCLINE * [0x001A] `MUTT_MACINTOSH_TIBETAN` - Tibetan.
							#define MUTT_MACINTOSH_TIBETAN 0x001A
							// @DOCLINE * [0x001B] `MUTT_MACINTOSH_MONGOLIAN` - Mongolian.
							#define MUTT_MACINTOSH_MONGOLIAN 0x001B
							// @DOCLINE * [0x001C] `MUTT_MACINTOSH_GEEZ` - Geez.
							#define MUTT_MACINTOSH_GEEZ 0x001C
							// @DOCLINE * [0x001D] `MUTT_MACINTOSH_SLAVIC` - Slavic.
							#define MUTT_MACINTOSH_SLAVIC 0x001D
							// @DOCLINE * [0x001E] `MUTT_MACINTOSH_VIETNAMESE` - Vietnamese.
							#define MUTT_MACINTOSH_VIETNAMESE 0x001E
							// @DOCLINE * [0x001F] `MUTT_MACINTOSH_SINDHI` - Sindhi.
							#define MUTT_MACINTOSH_SINDHI 0x001F
							// @DOCLINE * [0x0020] `MUTT_MACINTOSH_UNINTERPRETED` - Uninterpreted.
							#define MUTT_MACINTOSH_UNINTERPRETED 0x0020

							#ifdef MUTT_NAMES
							// @DOCLINE ##### Macintosh encoding names

								// @DOCLINE The name function `mutt_macintosh_encoding_get_name` returns a `const char*` representation of a given Macintosh encoding ID (for example, `MUTT_MACINTOSH_ROMAN` returns "MUTT_MACINTOSH_ROMAN"), defined below: @NLNT
								MUDEF const char* mutt_macintosh_encoding_get_name(uint16_m encoding_id);
								// @DOCLINE This function returns "MU_UNKNOWN" in the case that `encoding_id` is an unrecognized value.

								// @DOCLINE The name function `mutt_macintosh_encoding_get_nice_name` does the same thing, but returns a more readable version of it (for example, `MUTT_MACINTOSH_ROMAN` returns "Roman"), defined below: @NLNT
								MUDEF const char* mutt_macintosh_encoding_get_nice_name(uint16_m encoding_id);
								// @DOCLINE This function returns "Unknown" in the case that `encoding_id` is an unrecognized value.

								// @DOCLINE > Note that these are name functions, and are only defined if `MUTT_NAMES` is also defined.
							#endif /* MUTT_NAMES */

						// @DOCLINE #### ISO encoding

							// @DOCLINE * [0x0000] `MUTT_ISO_7_BIT_ASCII` - 7-bit ASCII.
							#define MUTT_ISO_7_BIT_ASCII 0x0000
							// @DOCLINE * [0x0001] `MUTT_ISO_10646` - ISO 10646.
							#define MUTT_ISO_10646 0x0001
							// @DOCLINE * [0x0002] `MUTT_ISO_8859_1` - ISO 8859-1.
							#define MUTT_ISO_8859_1 0x0002

						// @DOCLINE #### Windows encoding

							// @DOCLINE * [0x0000] `MUTT_WINDOWS_SYMBOL` - Symbol.
							#define MUTT_WINDOWS_SYMBOL 0x0000
							// @DOCLINE * [0x0001] `MUTT_WINDOWS_UNICODE_BMP` - Unicode BMP.
							#define MUTT_WINDOWS_UNICODE_BMP 0x0001
							// @DOCLINE * [0x0002] `MUTT_WINDOWS_SHIFTJIS` - ShiftJIS.
							#define MUTT_WINDOWS_SHIFTJIS 0x0002
							// @DOCLINE * [0x0003] `MUTT_WINDOWS_PRC` - PRC.
							#define MUTT_WINDOWS_PRC 0x0003
							// @DOCLINE * [0x0004] `MUTT_WINDOWS_BIG5` - Big5.
							#define MUTT_WINDOWS_BIG5 0x0004
							// @DOCLINE * [0x0005] `MUTT_WINDOWS_WANSUNG` - Wansung.
							#define MUTT_WINDOWS_WANSUNG 0x0005
							// @DOCLINE * [0x0006] `MUTT_WINDOWS_JOHAB` - Johab.
							#define MUTT_WINDOWS_JOHAB 0x0006
							// @DOCLINE * [0x000A] `MUTT_WINDOWS_UNICODE` - Unicode full repertoire.
							#define MUTT_WINDOWS_UNICODE 0x000A

							#ifdef MUTT_NAMES
							// @DOCLINE ##### Windows encoding names

								// @DOCLINE The name function `mutt_windows_encoding_get_name` returns a `const char*` representation of a given Windows encoding ID (for example, `MUTT_WINDOWS_SYMBOL` returns "MUTT_WINDOWS_SYMBOL"), defined below: @NLNT
								MUDEF const char* mutt_windows_encoding_get_name(uint16_m encoding_id);
								// @DOCLINE This function returns "MU_UNKNOWN" in the case that `encoding_id` is an unrecognized value.

								// @DOCLINE The name function `mutt_windows_encoding_get_nice_name` does the same thing, but returns a more readable version of it (for example, `MUTT_WINDOWS_SYMBOL` returns "Symbol"), defined below: @NLNT
								MUDEF const char* mutt_windows_encoding_get_nice_name(uint16_m encoding_id);
								// @DOCLINE This function returns "Unknown" in the case that `encoding_id` is an unrecognized value.

								// @DOCLINE > Note that these are name functions, and are only defined if `MUTT_NAMES` is also defined.
							#endif /* MUTT_NAMES */

					// @DOCLINE ### Language ID

						// @DOCLINE The following macros are defined for various language IDs:

						// @DOCLINE #### Macintosh language

							// @DOCLINE * [0x0000] `MUTT_MACINTOSH_LANG_ENGLISH` - English.
							#define MUTT_MACINTOSH_LANG_ENGLISH 0x0000
							// @DOCLINE * [0x0001] `MUTT_MACINTOSH_LANG_FRENCH` - French.
							#define MUTT_MACINTOSH_LANG_FRENCH 0x0001
							// @DOCLINE * [0x0002] `MUTT_MACINTOSH_LANG_GERMAN` - German.
							#define MUTT_MACINTOSH_LANG_GERMAN 0x0002
							// @DOCLINE * [0x0003] `MUTT_MACINTOSH_LANG_ITALIAN` - Italian.
							#define MUTT_MACINTOSH_LANG_ITALIAN 0x0003
							// @DOCLINE * [0x0004] `MUTT_MACINTOSH_LANG_DUTCH` - Dutch.
							#define MUTT_MACINTOSH_LANG_DUTCH 0x0004
							// @DOCLINE * [0x0005] `MUTT_MACINTOSH_LANG_SWEDISH` - Swedish.
							#define MUTT_MACINTOSH_LANG_SWEDISH 0x0005
							// @DOCLINE * [0x0006] `MUTT_MACINTOSH_LANG_SPANISH` - Spanish.
							#define MUTT_MACINTOSH_LANG_SPANISH 0x0006
							// @DOCLINE * [0x0007] `MUTT_MACINTOSH_LANG_DANISH` - Danish.
							#define MUTT_MACINTOSH_LANG_DANISH 0x0007
							// @DOCLINE * [0x0008] `MUTT_MACINTOSH_LANG_PORTUGUESE` - Portuguese.
							#define MUTT_MACINTOSH_LANG_PORTUGUESE 0x0008
							// @DOCLINE * [0x0009] `MUTT_MACINTOSH_LANG_NORWEGIAN` - Norwegian.
							#define MUTT_MACINTOSH_LANG_NORWEGIAN 0x0009
							// @DOCLINE * [0x000A] `MUTT_MACINTOSH_LANG_HEBREW` - Hebrew.
							#define MUTT_MACINTOSH_LANG_HEBREW 0x000A
							// @DOCLINE * [0x000B] `MUTT_MACINTOSH_LANG_JAPANESE` - Japanese.
							#define MUTT_MACINTOSH_LANG_JAPANESE 0x000B
							// @DOCLINE * [0x000C] `MUTT_MACINTOSH_LANG_ARABIC` - Arabic.
							#define MUTT_MACINTOSH_LANG_ARABIC 0x000C
							// @DOCLINE * [0x000D] `MUTT_MACINTOSH_LANG_FINNISH` - Finnish.
							#define MUTT_MACINTOSH_LANG_FINNISH 0x000D
							// @DOCLINE * [0x000E] `MUTT_MACINTOSH_LANG_GREEK` - Greek.
							#define MUTT_MACINTOSH_LANG_GREEK 0x000E
							// @DOCLINE * [0x000F] `MUTT_MACINTOSH_LANG_ICELANDIC` - Icelandic.
							#define MUTT_MACINTOSH_LANG_ICELANDIC 0x000F
							// @DOCLINE * [0x0010] `MUTT_MACINTOSH_LANG_MALTESE` - Maltese.
							#define MUTT_MACINTOSH_LANG_MALTESE 0x0010
							// @DOCLINE * [0x0011] `MUTT_MACINTOSH_LANG_TURKISH` - Turkish.
							#define MUTT_MACINTOSH_LANG_TURKISH 0x0011
							// @DOCLINE * [0x0012] `MUTT_MACINTOSH_LANG_CROATIAN` - Croatian.
							#define MUTT_MACINTOSH_LANG_CROATIAN 0x0012
							// @DOCLINE * [0x0013] `MUTT_MACINTOSH_LANG_CHINESE_TRADITIONAL` - Chinese (traditional).
							#define MUTT_MACINTOSH_LANG_CHINESE_TRADITIONAL 0x0013
							// @DOCLINE * [0x0014] `MUTT_MACINTOSH_LANG_URDU` - Urdu.
							#define MUTT_MACINTOSH_LANG_URDU 0x0014
							// @DOCLINE * [0x0015] `MUTT_MACINTOSH_LANG_HINDI` - Hindi.
							#define MUTT_MACINTOSH_LANG_HINDI 0x0015
							// @DOCLINE * [0x0016] `MUTT_MACINTOSH_LANG_THAI` - Thai.
							#define MUTT_MACINTOSH_LANG_THAI 0x0016
							// @DOCLINE * [0x0017] `MUTT_MACINTOSH_LANG_KOREAN` - Korean.
							#define MUTT_MACINTOSH_LANG_KOREAN 0x0017
							// @DOCLINE * [0x0018] `MUTT_MACINTOSH_LANG_LITHUANIAN` - Lithuanian.
							#define MUTT_MACINTOSH_LANG_LITHUANIAN 0x0018
							// @DOCLINE * [0x0019] `MUTT_MACINTOSH_LANG_POLISH` - Polish.
							#define MUTT_MACINTOSH_LANG_POLISH 0x0019
							// @DOCLINE * [0x001A] `MUTT_MACINTOSH_LANG_HUNGARIAN` - Hungarian.
							#define MUTT_MACINTOSH_LANG_HUNGARIAN 0x001A
							// @DOCLINE * [0x001B] `MUTT_MACINTOSH_LANG_ESTONIAN` - Estonian.
							#define MUTT_MACINTOSH_LANG_ESTONIAN 0x001B
							// @DOCLINE * [0x001C] `MUTT_MACINTOSH_LANG_LATVIAN` - Latvian.
							#define MUTT_MACINTOSH_LANG_LATVIAN 0x001C
							// @DOCLINE * [0x001D] `MUTT_MACINTOSH_LANG_SAMI` - Sami.
							#define MUTT_MACINTOSH_LANG_SAMI 0x001D
							// @DOCLINE * [0x001E] `MUTT_MACINTOSH_LANG_FAROESE` - Faroese.
							#define MUTT_MACINTOSH_LANG_FAROESE 0x001E
							// @DOCLINE * [0x001F] `MUTT_MACINTOSH_LANG_FARSI_PERSIAN` - Farsi/Persian.
							#define MUTT_MACINTOSH_LANG_FARSI_PERSIAN 0x001F
							// @DOCLINE * [0x0020] `MUTT_MACINTOSH_LANG_RUSSIAN` - Russian.
							#define MUTT_MACINTOSH_LANG_RUSSIAN 0x0020
							// @DOCLINE * [0x0021] `MUTT_MACINTOSH_LANG_CHINESE_SIMPLIFIED` - Chinese (simplified).
							#define MUTT_MACINTOSH_LANG_CHINESE_SIMPLIFIED 0x0021
							// @DOCLINE * [0x0022] `MUTT_MACINTOSH_LANG_FLEMISH` - Flemish.
							#define MUTT_MACINTOSH_LANG_FLEMISH 0x0022
							// @DOCLINE * [0x0023] `MUTT_MACINTOSH_LANG_IRISH_GAELIC` - Irish Gaelic.
							#define MUTT_MACINTOSH_LANG_IRISH_GAELIC 0x0023
							// @DOCLINE * [0x0024] `MUTT_MACINTOSH_LANG_ALBANIAN` - Albanian.
							#define MUTT_MACINTOSH_LANG_ALBANIAN 0x0024
							// @DOCLINE * [0x0025] `MUTT_MACINTOSH_LANG_ROMANIAN` - Romanian.
							#define MUTT_MACINTOSH_LANG_ROMANIAN 0x0025
							// @DOCLINE * [0x0026] `MUTT_MACINTOSH_LANG_CZECH` - Czech.
							#define MUTT_MACINTOSH_LANG_CZECH 0x0026
							// @DOCLINE * [0x0027] `MUTT_MACINTOSH_LANG_SLOVAK` - Slovak.
							#define MUTT_MACINTOSH_LANG_SLOVAK 0x0027
							// @DOCLINE * [0x0028] `MUTT_MACINTOSH_LANG_SLOVENIAN` - Slovenian.
							#define MUTT_MACINTOSH_LANG_SLOVENIAN 0x0028
							// @DOCLINE * [0x0029] `MUTT_MACINTOSH_LANG_YIDDISH` - Yiddish.
							#define MUTT_MACINTOSH_LANG_YIDDISH 0x0029
							// @DOCLINE * [0x002A] `MUTT_MACINTOSH_LANG_SERBIAN` - Serbian.
							#define MUTT_MACINTOSH_LANG_SERBIAN 0x002A
							// @DOCLINE * [0x002B] `MUTT_MACINTOSH_LANG_MACEDONIAN` - Macedonian.
							#define MUTT_MACINTOSH_LANG_MACEDONIAN 0x002B
							// @DOCLINE * [0x002C] `MUTT_MACINTOSH_LANG_BULGARIAN` - Bulgarian.
							#define MUTT_MACINTOSH_LANG_BULGARIAN 0x002C
							// @DOCLINE * [0x002D] `MUTT_MACINTOSH_LANG_UKRANIAN` - Ukrainian.
							#define MUTT_MACINTOSH_LANG_UKRANIAN 0x002D
							// @DOCLINE * [0x002E] `MUTT_MACINTOSH_LANG_BYELORUSSIAN` - Byelorussian.
							#define MUTT_MACINTOSH_LANG_BYELORUSSIAN 0x002E
							// @DOCLINE * [0x002F] `MUTT_MACINTOSH_LANG_UZBEK` - Uzbek.
							#define MUTT_MACINTOSH_LANG_UZBEK 0x002F
							// @DOCLINE * [0x0030] `MUTT_MACINTOSH_LANG_KAZAKH` - Kazakh.
							#define MUTT_MACINTOSH_LANG_KAZAKH 0x0030
							// @DOCLINE * [0x0031] `MUTT_MACINTOSH_LANG_AZERBAIJANI_CYRILLIC` - Azerbaijani (Cyrillic script).
							#define MUTT_MACINTOSH_LANG_AZERBAIJANI_CYRILLIC 0x0031
							// @DOCLINE * [0x0032] `MUTT_MACINTOSH_LANG_AZERBAIJANI_ARABIC` - Azerbaijani (Arabic script).
							#define MUTT_MACINTOSH_LANG_AZERBAIJANI_ARABIC 0x0032
							// @DOCLINE * [0x0033] `MUTT_MACINTOSH_LANG_ARMENIAN` - Armenian.
							#define MUTT_MACINTOSH_LANG_ARMENIAN 0x0033
							// @DOCLINE * [0x0034] `MUTT_MACINTOSH_LANG_GEORGIAN` - Georgian.
							#define MUTT_MACINTOSH_LANG_GEORGIAN 0x0034
							// @DOCLINE * [0x0035] `MUTT_MACINTOSH_LANG_MOLDAVIAN` - Moldavian.
							#define MUTT_MACINTOSH_LANG_MOLDAVIAN 0x0035
							// @DOCLINE * [0x0036] `MUTT_MACINTOSH_LANG_KIRGHIZ` - Kirghiz.
							#define MUTT_MACINTOSH_LANG_KIRGHIZ 0x0036
							// @DOCLINE * [0x0037] `MUTT_MACINTOSH_LANG_TAJIKI` - Tajiki.
							#define MUTT_MACINTOSH_LANG_TAJIKI 0x0037
							// @DOCLINE * [0x0038] `MUTT_MACINTOSH_LANG_TURKMEN` - Turkmen.
							#define MUTT_MACINTOSH_LANG_TURKMEN 0x0038
							// @DOCLINE * [0x0039] `MUTT_MACINTOSH_LANG_MONGOLIAN` - Mongolian (Mongolian script).
							#define MUTT_MACINTOSH_LANG_MONGOLIAN 0x0039
							// @DOCLINE * [0x003A] `MUTT_MACINTOSH_LANG_MONGOLIAN_CYRILLIC` - Mongolian (Cyrillic script).
							#define MUTT_MACINTOSH_LANG_MONGOLIAN_CYRILLIC 0x003A
							// @DOCLINE * [0x003B] `MUTT_MACINTOSH_LANG_PASHTO` - Pashto.
							#define MUTT_MACINTOSH_LANG_PASHTO 0x003B
							// @DOCLINE * [0x003C] `MUTT_MACINTOSH_LANG_KURDISH` - Kurdish.
							#define MUTT_MACINTOSH_LANG_KURDISH 0x003C
							// @DOCLINE * [0x003D] `MUTT_MACINTOSH_LANG_KASHMIRI` - Kashmiri.
							#define MUTT_MACINTOSH_LANG_KASHMIRI 0x003D
							// @DOCLINE * [0x003E] `MUTT_MACINTOSH_LANG_SINDHI` - Sindhi.
							#define MUTT_MACINTOSH_LANG_SINDHI 0x003E
							// @DOCLINE * [0x003F] `MUTT_MACINTOSH_LANG_TIBETAN` - Tibetan.
							#define MUTT_MACINTOSH_LANG_TIBETAN 0x003F
							// @DOCLINE * [0x0040] `MUTT_MACINTOSH_LANG_NEPALI` - Nepali.
							#define MUTT_MACINTOSH_LANG_NEPALI 0x0040
							// @DOCLINE * [0x0041] `MUTT_MACINTOSH_LANG_SANSKIRT` - Sanskrit.
							#define MUTT_MACINTOSH_LANG_SANSKIRT 0x0041
							// @DOCLINE * [0x0042] `MUTT_MACINTOSH_LANG_MARATHI` - Marathi.
							#define MUTT_MACINTOSH_LANG_MARATHI 0x0042
							// @DOCLINE * [0x0043] `MUTT_MACINTOSH_LANG_BENGALI` - Bengali.
							#define MUTT_MACINTOSH_LANG_BENGALI 0x0043
							// @DOCLINE * [0x0044] `MUTT_MACINTOSH_LANG_ASSAMESE` - Assamese.
							#define MUTT_MACINTOSH_LANG_ASSAMESE 0x0044
							// @DOCLINE * [0x0045] `MUTT_MACINTOSH_LANG_GUJARATI` - Gujarati.
							#define MUTT_MACINTOSH_LANG_GUJARATI 0x0045
							// @DOCLINE * [0x0046] `MUTT_MACINTOSH_LANG_PUNJABI` - Punjabi.
							#define MUTT_MACINTOSH_LANG_PUNJABI 0x0046
							// @DOCLINE * [0x0047] `MUTT_MACINTOSH_LANG_ORIYA` - Oriya.
							#define MUTT_MACINTOSH_LANG_ORIYA 0x0047
							// @DOCLINE * [0x0048] `MUTT_MACINTOSH_LANG_MALAYALAM` - Malayalam.
							#define MUTT_MACINTOSH_LANG_MALAYALAM 0x0048
							// @DOCLINE * [0x0049] `MUTT_MACINTOSH_LANG_KANNADA` - Kannada.
							#define MUTT_MACINTOSH_LANG_KANNADA 0x0049
							// @DOCLINE * [0x004A] `MUTT_MACINTOSH_LANG_TAMIL` - Tamil.
							#define MUTT_MACINTOSH_LANG_TAMIL 0x004A
							// @DOCLINE * [0x004B] `MUTT_MACINTOSH_LANG_TELUGU` - Telugu.
							#define MUTT_MACINTOSH_LANG_TELUGU 0x004B
							// @DOCLINE * [0x004C] `MUTT_MACINTOSH_LANG_SINHALESE` - Sinhalese.
							#define MUTT_MACINTOSH_LANG_SINHALESE 0x004C
							// @DOCLINE * [0x004D] `MUTT_MACINTOSH_LANG_BURMESE` - Burmese.
							#define MUTT_MACINTOSH_LANG_BURMESE 0x004D
							// @DOCLINE * [0x004E] `MUTT_MACINTOSH_LANG_KHMER` - Khmer.
							#define MUTT_MACINTOSH_LANG_KHMER 0x004E
							// @DOCLINE * [0x004F] `MUTT_MACINTOSH_LANG_LAO` - Lao.
							#define MUTT_MACINTOSH_LANG_LAO 0x004F
							// @DOCLINE * [0x0050] `MUTT_MACINTOSH_LANG_VIETNAMESE` - Vietnamese.
							#define MUTT_MACINTOSH_LANG_VIETNAMESE 0x0050
							// @DOCLINE * [0x0051] `MUTT_MACINTOSH_LANG_INDONESIAN` - Indonesian.
							#define MUTT_MACINTOSH_LANG_INDONESIAN 0x0051
							// @DOCLINE * [0x0052] `MUTT_MACINTOSH_LANG_TAGALOG` - Tagalog.
							#define MUTT_MACINTOSH_LANG_TAGALOG 0x0052
							// @DOCLINE * [0x0053] `MUTT_MACINTOSH_LANG_MALAY_ROMAN` - Malay (Roman script).
							#define MUTT_MACINTOSH_LANG_MALAY_ROMAN 0x0053
							// @DOCLINE * [0x0054] `MUTT_MACINTOSH_LANG_MALAY_ARABIC` - Malay (Arabic script).
							#define MUTT_MACINTOSH_LANG_MALAY_ARABIC 0x0054
							// @DOCLINE * [0x0055] `MUTT_MACINTOSH_LANG_AMHARIC` - Amharic.
							#define MUTT_MACINTOSH_LANG_AMHARIC 0x0055
							// @DOCLINE * [0x0056] `MUTT_MACINTOSH_LANG_TIGRINYA` - Tigrinya.
							#define MUTT_MACINTOSH_LANG_TIGRINYA 0x0056
							// @DOCLINE * [0x0057] `MUTT_MACINTOSH_LANG_GALLA` - Galla.
							#define MUTT_MACINTOSH_LANG_GALLA 0x0057
							// @DOCLINE * [0x0058] `MUTT_MACINTOSH_LANG_SOMALI` - Somali.
							#define MUTT_MACINTOSH_LANG_SOMALI 0x0058
							// @DOCLINE * [0x0059] `MUTT_MACINTOSH_LANG_SWAHILI` - Swahili.
							#define MUTT_MACINTOSH_LANG_SWAHILI 0x0059
							// @DOCLINE * [0x005A] `MUTT_MACINTOSH_LANG_KINYARWANDA_RUANDA` - Kinyarwanda/Ruanda.
							#define MUTT_MACINTOSH_LANG_KINYARWANDA_RUANDA 0x005A
							// @DOCLINE * [0x005B] `MUTT_MACINTOSH_LANG_RUNDI` - Rundi.
							#define MUTT_MACINTOSH_LANG_RUNDI 0x005B
							// @DOCLINE * [0x005C] `MUTT_MACINTOSH_LANG_NYANJA_CHEWA` - Nyanja/Chewa.
							#define MUTT_MACINTOSH_LANG_NYANJA_CHEWA 0x005C
							// @DOCLINE * [0x005D] `MUTT_MACINTOSH_LANG_MALAGASY` - Malagasy.
							#define MUTT_MACINTOSH_LANG_MALAGASY 0x005D
							// @DOCLINE * [0x005E] `MUTT_MACINTOSH_LANG_ESPERANTO` - Esperanto.
							#define MUTT_MACINTOSH_LANG_ESPERANTO 0x005E
							// @DOCLINE * [0x0080] `MUTT_MACINTOSH_LANG_WELSH` - Welsh.
							#define MUTT_MACINTOSH_LANG_WELSH 0x0080
							// @DOCLINE * [0x0081] `MUTT_MACINTOSH_LANG_BASQUE` - Basque.
							#define MUTT_MACINTOSH_LANG_BASQUE 0x0081
							// @DOCLINE * [0x0082] `MUTT_MACINTOSH_LANG_CATALAN` - Catalan.
							#define MUTT_MACINTOSH_LANG_CATALAN 0x0082
							// @DOCLINE * [0x0083] `MUTT_MACINTOSH_LANG_LATIN` - Latin.
							#define MUTT_MACINTOSH_LANG_LATIN 0x0083
							// @DOCLINE * [0x0084] `MUTT_MACINTOSH_LANG_QUECHUA` - Quechua.
							#define MUTT_MACINTOSH_LANG_QUECHUA 0x0084
							// @DOCLINE * [0x0085] `MUTT_MACINTOSH_LANG_GUARANI` - Guarani.
							#define MUTT_MACINTOSH_LANG_GUARANI 0x0085
							// @DOCLINE * [0x0086] `MUTT_MACINTOSH_LANG_AYMARA` - Aymara.
							#define MUTT_MACINTOSH_LANG_AYMARA 0x0086
							// @DOCLINE * [0x0087] `MUTT_MACINTOSH_LANG_TATAR` - Tatar.
							#define MUTT_MACINTOSH_LANG_TATAR 0x0087
							// @DOCLINE * [0x0088] `MUTT_MACINTOSH_LANG_UIGHUR` - Uighur.
							#define MUTT_MACINTOSH_LANG_UIGHUR 0x0088
							// @DOCLINE * [0x0089] `MUTT_MACINTOSH_LANG_DZONGKHA` - Dzongkha.
							#define MUTT_MACINTOSH_LANG_DZONGKHA 0x0089
							// @DOCLINE * [0x008A] `MUTT_MACINTOSH_LANG_JAVANESE_ROMAN` - Javanese (Roman script).
							#define MUTT_MACINTOSH_LANG_JAVANESE_ROMAN 0x008A
							// @DOCLINE * [0x008B] `MUTT_MACINTOSH_LANG_SUNDANESE_ROMAN` - Sundanese (Roman script).
							#define MUTT_MACINTOSH_LANG_SUNDANESE_ROMAN 0x008B
							// @DOCLINE * [0x008C] `MUTT_MACINTOSH_LANG_GALICIAN` - Galician.
							#define MUTT_MACINTOSH_LANG_GALICIAN 0x008C
							// @DOCLINE * [0x008D] `MUTT_MACINTOSH_LANG_AFRIKAANS` - Afrikaans.
							#define MUTT_MACINTOSH_LANG_AFRIKAANS 0x008D
							// @DOCLINE * [0x008E] `MUTT_MACINTOSH_LANG_BRETON` - Breton.
							#define MUTT_MACINTOSH_LANG_BRETON 0x008E
							// @DOCLINE * [0x008F] `MUTT_MACINTOSH_LANG_INUKTITUT` - Inuktitut.
							#define MUTT_MACINTOSH_LANG_INUKTITUT 0x008F
							// @DOCLINE * [0x0090] `MUTT_MACINTOSH_LANG_SCOTTISH_GAELIC` - Scottish Gaelic.
							#define MUTT_MACINTOSH_LANG_SCOTTISH_GAELIC 0x0090
							// @DOCLINE * [0x0091] `MUTT_MACINTOSH_LANG_MANX_GAELIC` - Manx Gaelic.
							#define MUTT_MACINTOSH_LANG_MANX_GAELIC 0x0091
							// @DOCLINE * [0x0092] `MUTT_MACINTOSH_LANG_IRISH_GAELIC_DOT_ABOVE` - Irish Gaelic (with dot above).
							#define MUTT_MACINTOSH_LANG_IRISH_GAELIC_DOT_ABOVE 0x0092
							// @DOCLINE * [0x0093] `MUTT_MACINTOSH_LANG_TONGAN` - Tongan.
							#define MUTT_MACINTOSH_LANG_TONGAN 0x0093
							// @DOCLINE * [0x0094] `MUTT_MACINTOSH_LANG_GREEK_POLYTONIC` - Greek (polytonic).
							#define MUTT_MACINTOSH_LANG_GREEK_POLYTONIC 0x0094
							// @DOCLINE * [0x0095] `MUTT_MACINTOSH_LANG_GREENLANDIC` - Greenlandic.
							#define MUTT_MACINTOSH_LANG_GREENLANDIC 0x0095
							// @DOCLINE * [0x0096] `MUTT_MACINTOSH_LANG_AZERBAIJANI_ROMAN` - Azerbaijani (Roman script).
							#define MUTT_MACINTOSH_LANG_AZERBAIJANI_ROMAN 0x0096

					// @DOCLINE ### Name ID

						// @DOCLINE The follwing macros are defined for various name IDs:

						// @DOCLINE * [0x0000] `MUTT_NAME_COPYRIGHT_NOTICE` - "Copyright notice."
						#define MUTT_NAME_COPYRIGHT_NOTICE 0x0000
						// @DOCLINE * [0x0001] `MUTT_NAME_FONT_FAMILY` - "Font Family name."
						#define MUTT_NAME_FONT_FAMILY 0x0001
						// @DOCLINE * [0x0002] `MUTT_NAME_FONT_SUBFAMILY` - "Font Subfamily name."
						#define MUTT_NAME_FONT_SUBFAMILY 0x0002
						// @DOCLINE * [0x0003] `MUTT_NAME_FONT_IDENTIFIER` - "Unique font identifier."
						#define MUTT_NAME_FONT_IDENTIFIER 0x0003
						// @DOCLINE * [0x0004] `MUTT_NAME_FONT_FULL` - "Full font name that reflects all family and relevant subfamily descriptors".
						#define MUTT_NAME_FONT_FULL 0x0004
						// @DOCLINE * [0x0005] `MUTT_NAME_VERSION` - "Version string."
						#define MUTT_NAME_VERSION 0x0005
						// @DOCLINE * [0x0006] `MUTT_NAME_POSTSCRIPT` - "PostScript name for the font."
						#define MUTT_NAME_POSTSCRIPT 0x0006
						// @DOCLINE * [0x0007] `MUTT_NAME_TRADEMARK` - "Trademark."
						#define MUTT_NAME_TRADEMARK 0x0007
						// @DOCLINE * [0x0008] `MUTT_NAME_MANUFACTURER` - "Manufacturer Name."
						#define MUTT_NAME_MANUFACTURER 0x0008
						// @DOCLINE * [0x0009] `MUTT_NAME_DESIGNER` - "Designer."
						#define MUTT_NAME_DESIGNER 0x0009
						// @DOCLINE * [0x000A] `MUTT_NAME_DESCRIPTION` - "Description."
						#define MUTT_NAME_DESCRIPTION 0x000A
						// @DOCLINE * [0x000B] `MUTT_NAME_VENDOR_URL` - "URL of Vendor."
						#define MUTT_NAME_VENDOR_URL 0x000B
						// @DOCLINE * [0x000C] `MUTT_NAME_DESIGNER_URL` - "URL of Designer."
						#define MUTT_NAME_DESIGNER_URL 0x000C
						// @DOCLINE * [0x000D] `MUTT_NAME_LICENSE` - "License Description."
						#define MUTT_NAME_LICENSE 0x000D
						// @DOCLINE * [0x000E] `MUTT_NAME_LICENSE_URL` - "License Info URL."
						#define MUTT_NAME_LICENSE_URL 0x000E
						// @DOCLINE * [0x0010] `MUTT_NAME_TYPOGRAPHIC_FAMILY` - "Typographic Family name."
						#define MUTT_NAME_TYPOGRAPHIC_FAMILY 0x0010
						// @DOCLINE * [0x0011] `MUTT_NAME_TYPOGRAPHIC_SUBFAMILY` - "Typographic Subfamily name."
						#define MUTT_NAME_TYPOGRAPHIC_SUBFAMILY 0x0011
						// @DOCLINE * [0x0012] `MUTT_NAME_COMPATIBLE_FULL` - "Compatible Full (Macintosh only)."
						#define MUTT_NAME_COMPATIBLE_FULL 0x0012
						// @DOCLINE * [0x0013] `MUTT_NAME_SAMPLE_TEXT` - "Sample text."
						#define MUTT_NAME_SAMPLE_TEXT 0x0013
						// @DOCLINE * [0x0014] `MUTT_NAME_POSTSCRIPT_CID_FINDFONT` - "PostScript CID findfont name."
						#define MUTT_NAME_POSTSCRIPT_CID_FINDFONT 0x0014
						// @DOCLINE * [0x0015] `MUTT_NAME_WWS_FAMILY` - "WWS Family Name."
						#define MUTT_NAME_WWS_FAMILY 0x0015
						// @DOCLINE * [0x0016] `MUTT_NAME_WWS_SUBFAMILY` - "WWS Subfamily Name."
						#define MUTT_NAME_WWS_SUBFAMILY 0x0016
						// @DOCLINE * [0x0017] `MUTT_NAME_LIGHT_BACKGROUND_PALETTE` - "Light Background Palette."
						#define MUTT_NAME_LIGHT_BACKGROUND_PALETTE 0x0017
						// @DOCLINE * [0x0018] `MUTT_NAME_DARK_BACKGROUND_PALETTE` - "Dark Background Palette."
						#define MUTT_NAME_DARK_BACKGROUND_PALETTE 0x0018

						#ifdef MUTT_NAMES
						// @DOCLINE #### Name ID names

							// @DOCLINE The name function `mutt_name_id_get_name` returns a `const char*` representation of a given name ID (for example, `MUTT_NAME_COPYRIGHT_NOTICE` returns "MUTT_NAME_COPYRIGHT_NOTICE"), defined below: @NLNT
							MUDEF const char* mutt_name_id_get_name(uint16_m name_id);
							// @DOCLINE This function returns "MU_UNKNOWN" in the case that `name_id` is an unrecognized value.

							// @DOCLINE The name function `mutt_name_id_get_nice_name` does the same thing, but returns a more readable version of it (for example, `MUTT_NAME_COPYRIGHT_NOTICE` returns "Copyright notice"), defined below: @NLNT
							MUDEF const char* mutt_name_id_get_nice_name(uint16_m name_id);
							// @DOCLINE This function returns "Unknown" in the case that `name_id` is an unrecognized value.

							// @DOCLINE > Note that these are name functions, and are only defined if `MUTT_NAMES` is also defined.
						#endif /* MUTT_NAMES */

				// @DOCLINE ## Internally used low-level functionality

					// @DOCLINE mutt uses several internally-defined low-level things to make certain things easier to perform. This section is a list of them.

					// @DOCLINE ### Reading F2DOT14 values

						// @DOCLINE The macro function `MUTT_F2DOT14` creates an expression for a float equivalent of a given array that stores 2 bytes representing a big-endian F2DOT14, defined below: @NLNT
						#define MUTT_F2DOT14(b) (((float)((*(int8_m*)&b[1]) & 0xC0)) + (((float)(MU_RBEU16(b) & 0xFFFF)) / 16384.f))

					// @DOCLINE ### Delta logic

						// @DOCLINE Some cmap formats use fairly weird logic when using "idDelta" values. The function `mutt_id_delta` figures this logic out automatically, defined below: @NLNT
						MUDEF uint16_m mutt_id_delta(uint16_m character_code, int16_m delta);

			// @DOCLINE # Raster API

				// @DOCLINE The raster API has the ability to [rasterize](#rasterize-glyph) [TrueType-like glyphs](#raster-glyph) onto [a bitmap](#raster-bitmap).

				// @DOCLINE ## Raster glyph

					// @DOCLINE A "raster glyph" (often shortened to "rglyph", respective struct [`muttRGlyph`](#rglyph-struct)) is a glyph described for rasterization in the raster API, being similar to how a simple glyph is defined in the low-level API, and is heavily based on how glyphs are specified in TrueType.

					// @DOCLINE The most common usage of rglyphs is for [rasterizing given glyphs in a TrueType font](#rasterization-of-truetype-glyphs). This can be achieved via [converting a simple](simple-glyph-to-rglyph) or [composite glyph](#composite-glyph-to-rglyph), or [just its header](#glyph-header-to-rglyph), to an rglyph equivalent, which mutt has built-in support for.

					// @DOCLINE Rglyphs don't necessarily need to come from a simple or composite glyph, however. The user can pass in their own rglyphs, and as long as they use the struct correctly, the raster API will rasterize it correctly.

					// @DOCLINE ### Rglyph struct

						typedef uint8_m muttRFlags;
						typedef struct muttRPoint muttRPoint;
						typedef struct muttRGlyph muttRGlyph;

						// @DOCLINE An rglyph is represented via the struct `muttRGlyph`, which has the following members:

						struct muttRGlyph {
							// @DOCLINE * `@NLFT num_points` - the number of points in the `points` array. This value must be at least 1.
							uint16_m num_points;
							// @DOCLINE * `@NLFT* points` - each point for the glyph.
							muttRPoint* points;
							// @DOCLINE * `@NLFT num_contours` - the number of contours in the glyph.
							uint16_m num_contours;
							// @DOCLINE * `@NLFT* contour_ends` - the last point index of each contour, in increasing order. `contour_ends[num_contours-1]+1` must equal `num_points`.
							uint16_m* contour_ends;
							// @DOCLINE * `@NLFT x_max` - the greatest x-coordinate value of any point within the glyph.
							float x_max;
							// @DOCLINE * `@NLFT y_max` - the greatest y-coordinate value of any point within the glyph.
							float y_max;
							// @DOCLINE * `@NLFT ascender` - the ascender value for the given glyph, retrieved from the hhea table. This is only relevant for glyphs converted from TrueType glyphs to rglyphs.
							float ascender;
							// @DOCLINE * `@NLFT descender` - the descender value for the given glyph, retrieved from the hhea table. This is only relevant for glyphs converted from TrueType glyphs to rglyphs.
							float descender;
							// @DOCLINE * `@NLFT lsb` - the left-side bearing for the given glyph, retrieved from the hmtx table. This is only relevant for glyphs converted from TrueType glyphs to rglyphs.
							float lsb;
							// @DOCLINE * `@NLFT advance_width` - the advance width for the given glyph, retrieved from the hmtx table. This is only relevant for glyphs converted from TrueType glyphs to rglyphs.
							float advance_width;
						};

						// @DOCLINE Values for the rglyph's ascender, descender, left-side bearing, and advance width are given due to the fact that the coordinates of a TrueType glyph are transformed upon being converted to an rglyph, making these values difficult for the user to retrieve for rglyphs. They are *not* filled in upon conversion of a TrueType glyph to an rglyph; these values are filled in with the function [`mutt_rglyph_metrics`](#truetype-metrics-to-rglyph-metrics).

						// @DOCLINE A point in an rglyph is represented with the struct `muttRPoint`, which has the following members:
						struct muttRPoint {
							// @DOCLINE * `@NLFT x` - the x-coordinate of the point, in [pixel units](#raster-bitmap).
							float x;
							// @DOCLINE * `@NLFT y` - the y-coordinate of the point, in [pixel units](#raster-bitmap).
							float y;
							// @DOCLINE * `@NLFT flags` - the [flags](#rglyph-flags) of the point.
							muttRFlags flags;
						};

						// @DOCLINE No coordinate values in any point within an rglyph should be negative, or exceed the values indicated by `x_max` and `y_max`.

						// @DOCLINE The ordering of points should follow the non-zero winding number rule that TrueType glyphs also follow: "[Points that have a non-zero winding number are inside the glyph. All other points are outside the glyph.](https://developer.apple.com/fonts/TrueType-Reference-Manual/RM02/Chap2.html#distinguishing)"

						// @DOCLINE All contours must start with an on-curve point.

						// @DOCLINE Some rendering methods have the possibility of "[bleeding](#raster-bleeding)" over pixels that mathematically are completely outside of the glyph, but are one pixel away from another pixel who is at least partially inside of the glyph. For this reason, it is recommended to have each points' coordinates offset by at least 1 pixel, so that no pixel coordinate is exactly at 0. This is automatically performed when converting simple and composite glyphs from the low-level API to an rglyph, and should be done by the user when creating/modifying rglyphs.

					// @DOCLINE #### Rglyph flags

						// @DOCLINE The type `muttRFlags` (typedef for `uint8_m`) represents the flags of a given point in an rglyph. It has the following defined values for bitmasking:

						// @DOCLINE * [0x01] `MUTTR_ON_CURVE` - represents whether or not the point is on (1) or off (0) the curve; conceptually equivalent to "ON_CURVE_POINT" for simple glyphs in TrueType.
						#define MUTTR_ON_CURVE 0x01

						// @DOCLINE No bits other than the ones defined above are read for any point in an rglyph.

				// @DOCLINE ## Raster bitmap

					typedef struct muttRBitmap muttRBitmap;
					typedef uint8_m muttRIOColor;
					typedef uint16_m muttRChannels;

					// @DOCLINE Rasterization of an rglyph is performed on a bitmap. The information about the bitmap is provided by the struct `muttRBitmap`, which has the following members:

					struct muttRBitmap {
						// @DOCLINE * `@NLFT width` - the width of the bitmap, in pixels.
						uint32_m width;
						// @DOCLINE * `@NLFT height` - the height of the bitmap, in pixels.
						uint32_m height;
						// @DOCLINE * `@NLFT channels` - the [channels](#raster-channels) of the bitmap.
						muttRChannels channels;
						// @DOCLINE * `@NLFT stride` - the amount of bytes to move by for each horizontal row of pixels.
						uint32_m stride;
						// @DOCLINE * `@NLFT* pixels` - the pixel data for the bitmap to be filled in, stored from left to right, top to bottom. All values within the pixel data are expected to be pre-initialized to the appropriate out-of-glyph color indicated by `io_color`.
						uint8_m* pixels;
						// @DOCLINE * `@NLFT io_color` - the [in/out color](#raster-in-out-color) of the bitmap.
						muttRIOColor io_color;
					};

					// @DOCLINE ### Raster channels

						// @DOCLINE The type `muttRChannels` (typedef for `uint16_m`) represents the channels of a bitmap. It has the following defined values:

						// @DOCLINE * [0x0000] `MUTTR_R` - one color channel per pixel, corresponding to one value representing how far a pixel is *in* or *out* of the glyph.
						#define MUTTR_R 0x0000
						// @DOCLINE * [0x0002] `MUTTR_RGB` - three color channels: red, green, and blue in that order per pixel.
						#define MUTTR_RGB 0x0002
						// @DOCLINE * [0x0003] `MUTTR_RGBA` - four color channels: red, green, blue, and alpha in that order per pixel.
						#define MUTTR_RGBA 0x0003

						// @DOCLINE How non-singular channel values represent how far a pixel is *in* or *out* of the glyph is dependent on the [raster method](#raster-method).

					// @DOCLINE ### Raster in out color

						// @DOCLINE The type `muttRIOColor` (typedef for `uint8_m`) represents what values indicate whether or not a pixel is *inside* of the glyph or *outside* of the glyph (and the corresponding possible mixing between the two values). It has the following defined values:

						// @DOCLINE * [0x00] `MUTTR_BW` - a smaller value indicates being more outside the glyph, and a larger value indicates being more inside the glyph.
						#define MUTTR_BW 0x00
						// @DOCLINE * [0x01] `MUTTR_WB` - a larger value indicates being more outside the glyph, and a smaller value indicates being more inside the glyph.
						#define MUTTR_WB 0x01

						// @DOCLINE The rules of these values applies to all channels, including alpha.

				// @DOCLINE ## Rasterize glyph

					typedef uint16_m muttRMethod;

					// @DOCLINE Rasterizing a glyph is performed with the function `mutt_raster_glyph`, defined below: @NLNT
					MUDEF muttResult mutt_raster_glyph(muttRGlyph* glyph, muttRBitmap* bitmap, muttRMethod method);

					// @DOCLINE ### Raster method

						// @DOCLINE The type `muttRMethod` (typedef for `uint16_m`) represents what rasterization method to use when rasterizing a glyph. It has the following defined values:

						// @DOCLINE * [0x0000] `MUTTR_FULL_PIXEL_BI_LEVEL` - [full-pixel](#full-pixel) [bi-level](#bi-level) rasterization.
						#define MUTTR_FULL_PIXEL_BI_LEVEL 0x0000
						// @DOCLINE * [0x0001] `MUTTR_FULL_PIXEL_AA2X2` - [full-pixel](#full-pixel) two-by-two [anti-aliased](#anti-aliasing) rasterization.
						#define MUTTR_FULL_PIXEL_AA2X2 0x0001
						// @DOCLINE * [0x0002] `MUTTR_FULL_PIXEL_AA4X4` - [full-pixel](#full-pixel) four-by-four [anti-aliased](#anti-aliasing) rasterization.
						#define MUTTR_FULL_PIXEL_AA4X4 0x0002
						// @DOCLINE * [0x0003] `MUTTR_FULL_PIXEL_AA8X8` - [full-pixel](#full-pixel) eight-by-eight [anti-aliased](#anti-aliasing) rasterization.
						#define MUTTR_FULL_PIXEL_AA8X8 0x0003

						// @DOCLINE Most of the terms used to describe these rendering methods are taken from terms used in [The Raster Tragedy](http://rastertragedy.com).

					// @DOCLINE ### Full-pixel

						// @DOCLINE The term "full-pixel" means that each pixel is used as one value indicating how much a pixel is *inside* or *outside* of the glyph. Each pixel is treated pixel-coordinate-wise as being directly in the center of a pixel in the pixel coordinate grid; for example, the coordinates of the top-leftest pixel in a bitmap is (0.5, 0.5) when internally calculating how much a pixel is inside or outside of the glyph.

					// @DOCLINE ### Bi-level

						// @DOCLINE The term "bi-level" means that each pixel is rather fully inside or outside of the glyph, with no possibility of intermediate values.

					// @DOCLINE ### Anti-aliasing

						// @DOCLINE Anti-aliasing is used in rasterization to smooth jagged edges, taking multiple samples per pixel, calculating whether or not each one is inside or outside of the glyph, and averaging all of those values for the calculated value of a given pixel. This allows for pixels to exist that are *partially* inside or outside of the glyph, and whose pixel values indicate as such, which is the opposite of [bi-level rasterization](#bi-level).

						// @DOCLINE The amount of samples per pixel in the x- and y-direction is controlled by its dimensions, splitting up the pixel into multiple sub-pixels to then be individually calculated. For example, two-by-two anti-aliasing implies taking two samples on the x- and y-axis per pixel, so the top-leftest pixel (coordinates (0.5, 0.5)) would be split up into coordinates (0.25, 0.25), (0.75, 0.25), (0.25, 0.75), and (0.75, 0.75), in no particular order, and individually calculated & averaged for the final pixel value.

					// @DOCLINE ### Raster bleeding

						// @DOCLINE Some rasterization methods have a possibility of setting pixels as (at least partially) inside of the glyph that aren't mathematically inside of the glyph to any degree, but are one pixel away from another pixel that *is* (at least partially) inside of the glyph. This effect is called "bleeding", and can cause pixels to be inside of the glyph that are outside of the range of the glyph's coordinates.

						// @DOCLINE In terms of rglyphs, this is prevented by offsetting the coordinates of each point by 1 pixel, ensuring that there is at least a single pixel to the left and top and of any pixel that is mathematically inside of the glyph, and thus can catch any theoretical bleeding. This is automatically performed when converting simple and composite glyphs from the low-level API to an rglyph, and should be done by the user when creating/modifying rglyphs.

						// @DOCLINE In terms of rasterization, this is prevented by increasing the width and height of the bitmap to be 1 pixel greater than the maximum x- and y-coordinates within the glyph (`glyph->x_max` and `glyph->y_max`). The conversion from the decimal values of `x_max` and `y_max` to an integer width and height should be performed via a ceiling of the final result.

				// @DOCLINE ## Rasterization of TrueType glyphs

					// @DOCLINE The raster API gives access to rasterizing TrueType glyphs by converting them to an rglyph, which can then be [rasterized directly](#rasterize-glyph). This conversion can be done rather by the user directly [giving a simple glyph](#simple-glyph-to-rglyph), [giving a composite glyph](#composite-glyph-to-rglyph), or by [giving the header of a simple or composite glyph](#glyph-header-to-rglyph).

					// @DOCLINE ### Font units to pixel units

						// @DOCLINE The rasterization of any TrueType glyph involves converting the Truetype "font units" (FUnits) to pixel units (which is what a raster glyph uses). This conversion requires a [point size](https://en.wikipedia.org/wiki/Point_(typography)) and the [pixels per inch](https://en.wikipedia.org/wiki/Pixel_density), or PPI, of the display (usually 72 or 96). These two variables allow the coordinates of an rglyph to be rasterized at a predictable and calculatable physical size when displayed.

						// @DOCLINE The function `mutt_funits_to_punits` performs the conversion described above for a given font based on its unitsPerEm value (stored in the head table), defined below: @NLNT
						MUDEF float mutt_funits_to_punits(muttFont* font, float funits, float point_size, float ppi);

						// @DOCLINE Although the font unit range in TrueType can be expressed with a signed 16-bit integer, `funits` is a `float` for the sake of being able to perform the conversion on transformed coordinates in composite glyphs, which can result in decimal numbers.

					// @DOCLINE ### Simple glyph to rglyph

						// @DOCLINE The function `mutt_simple_rglyph` converts a simple glyph to an rglyph, defined below: @NLNT
						MUDEF muttResult mutt_simple_rglyph(muttFont* font, muttGlyphHeader* header, muttSimpleGlyph* glyph, muttRGlyph* rglyph, float point_size, float ppi, muByte* data, uint32_m* written);

						// @DOCLINE Upon a non-fatal result, `rglyph` is filled with valid raster glyph information for the given simple glyph using memory from `data`. Upon a fatal result, the contents of `rglyph` and `data` are undefined. The given rglyph information is only valid for as long as `font` is not deloaded, and as long as `data` goes unmodified.

						// @DOCLINE The given simple glyph must have at least one contour, and that one contour must have points. The simple glyph given must be valid.

						// @DOCLINE This function follows the format of a user-allocated function. For an explanation of how `data` and `written` are supposed to be used within this function, see [the user-allocated function section](#user-allocated-functions).

						// @DOCLINE #### Simple glyph to rglyph memory maximum

							// @DOCLINE The maximum amount of memory that will be needed for converting a simple glyph to a raster glyph for a given font, in bytes, is provided by the function `mutt_simple_rglyph_max`, defined below: @NLNT
							MUDEF uint32_m mutt_simple_rglyph_max(muttFont* font);

					// @DOCLINE ### Composite glyph to rglyph

						// @DOCLINE The function `mutt_composite_rglyph` converts a composite glyph to an rglyph, defined below: @NLNT
						MUDEF muttResult mutt_composite_rglyph(muttFont* font, muttGlyphHeader* header, muttCompositeGlyph* glyph, muttRGlyph* rglyph, float point_size, float ppi, muByte* data);

						// @DOCLINE Upon a non-fatal result, `rglyph` is filled with valid raster glyph information for the given composite glyph using memory from `data`. Upon a fatal result, the contents of `rglyph` and `data` are undefined. The given rglyph information is only valid for as long as `font` is not deloaded, and as long as `data` goes unmodified.

						// @DOCLINE `data` should be a pointer to user-allocated memory of size `mutt_composite_rglyph_max` (in bytes). This makes it ***not*** follow the conventional format of a user-allocated function within mutt: the required amount of memory needed for converting a composite glyph to an rglyph is fixed across each font, and thus, has no `written` parameter.

						// @DOCLINE The given composite glyph must have at least one contour, and that one contour must have points. The composite glyph given must be valid.

						// @DOCLINE This function does ***not*** currently work with composite glyphs that use ***phantom points***, as the table required for processing them (gvar) is not currently supported.

						// @DOCLINE #### Composite glyph to rglyph memory maximum

							// @DOCLINE The maximum amount of memory that will be needed for converting a composite glyph to a raster glyph for a given font, in bytes, is provided by the function `mutt_composite_rglyph_max`, defined below: @NLNT
							MUDEF uint32_m mutt_composite_rglyph_max(muttFont* font);

					// @DOCLINE ### Glyph header to rglyph

						// @DOCLINE The function `mutt_header_rglyph` converts a glyph header to a glyph, defined below: @NLNT
						MUDEF muttResult mutt_header_rglyph(muttFont* font, muttGlyphHeader* header, muttRGlyph* rglyph, float point_size, float ppi, muByte* data, uint32_m* written);

						// @DOCLINE Upon a non-fatal result, `rglyph` is filled with valid raster glyph information for the given glyph based on its header, using memory from `data`. Upon a fatal result, the contents of `rglyph` and `data` are undefined. The given rglyph information is only valid for as long as `font` is not deloaded, and as long as `data` goes unmodified.

						// @DOCLINE The given glyph must have at least one contour, and that one contour must have points. The glyph header given must be valid.

						// @DOCLINE This function follows the format of a user-allocated function. For an explanation of how `data` and `written` are supposed to be used within this function, see [the user-allocated function section](#user-allocated-functions). However, since the conversion of a composite glyph to an rglyph requires a fixed amount of memory per font, if `written` is ever dereferenced and set by this function relative to a composite glyph, it will be set to `mutt_composite_rglyph_max`.

						// @DOCLINE #### Glyph header to rglyph memory maximum

							// @DOCLINE The maximum amount of memory that will be needed for converting a glyph header to a raster glyph for a given font, in bytes, is provided by the function `mutt_header_rglyph_max`, defined below: @NLNT
							MUDEF uint32_m mutt_header_rglyph_max(muttFont* font);

							// @DOCLINE This function rather returns (the sum of `mutt_simple_glyph_max_size` and `mutt_simple_rglyph_max`) or (the sum of `mutt_composite_glyph_max_size` and `mutt_composite_rglyph_max`), whichever is greater. All the table loading requirements of these functions apply.

					// @DOCLINE ### TrueType metrics to rglyph metrics

						// @DOCLINE The function `mutt_rglyph_metrics` fills in the metric information about a TrueType-to-rglyph conversion, converting the TrueType glyph's metrics to the pixel-unit equivalents for the rglyph, defined below: @NLNT
						MUDEF void mutt_rglyph_metrics(muttFont* font, muttGlyphHeader* header, uint16_m glyph_id, muttRGlyph* rglyph, float point_size, float ppi);

						// @DOCLINE The values `rglyph->ascender`, `rglyph->descender`, `rglyph->lsb`, and `rglyph->advance_width` are filled in. `glyph_id` must be a valid glyph ID. The x/y min/max values within `header` must be accurate.

					// @DOCLINE ### TrueType x/y min/max to rglyph x/y max

						// @DOCLINE The function `mutt_funits_punits_min_max` converts a set of x/y min/max values, in FUnits (usually retrieved from loading a simple/composite glyph, with said values being stored in the header), to what an rglyph's x/y maximum values will be when converting it to an rglyph, defined below: @NLNT
						MUDEF void mutt_funits_punits_min_max(muttFont* font, muttGlyphHeader* header, muttRGlyph* rglyph, float point_size, float ppi);

						// @DOCLINE This function takes in the x/y min/max values from `header` and converts them to valid x/y max values within `rglyph`. Note that this function implicitly assumes that the given x/y min/max values are valid for the glyph, which may not be the case in a few scenarios:

						// @DOCLINE * If only the header has been loaded, and not the corresponding simple/composite glyph, the x/y min/max values within the header are only what the font has provided, and may not be fully accurate.

						// @DOCLINE * If the glyph is composite and it has been loaded, both in header form and in `muttCompositeGlyph` form, the x/y min/max values still haven't been validated, since `mutt_composite_glyph` does not check coordinate values; the assuredly correct x/y min/max values for a composite glyph can be retrieved with the function [`mutt_composite_glyph_min_max`](#composite-min-max).

			// @DOCLINE # Result

				// @DOCLINE The type `muttResult` (typedef for `uint32_m`) is defined to represent how a task went. Result values can be "fatal" (meaning that the task completely failed to execute, and the program will continue as if the task had never been attempted), "non-fatal" (meaning that the task partially failed, but was still able to complete the task), and "successful" (meaning that the task fully succeeded).

				// @DOCLINE ## Result values

				// @DOCLINE The following values are defined for `muttResult` (all values not explicitly stated as being fatal, non-fatal, or successful are assumed to be fatal):

				// @DOCLINE ### General result values
				// 0 -> 63 //

					// @DOCLINE * `MUTT_SUCCESS` - the task was successfully completed; real value 0.
					#define MUTT_SUCCESS 0
					// @DOCLINE * `MUTT_FAILED_MALLOC` - a call to malloc failed, implying insufficient available memory to perform the task.
					#define MUTT_FAILED_MALLOC 1
					// @DCOLINE * `MUTT_FAILED_REALLOC` - a call to realloc failed, implying insufficient available memory to perform the task.
					#define MUTT_FAILED_REALLOC 2
					// @DOCLINE * `MUTT_FAILED_FIND_TABLE` - the table could not be located, and is likely not included in the font file.
					#define MUTT_FAILED_FIND_TABLE 3

				// @DOCLINE ### Directory result values
				// 64 -> 127 //

					// @DOCLINE * `MUTT_INVALID_DIRECTORY_LENGTH` - the length of the table directory was invalid. This is the first check performed on the length of the font file data, meaning that if this result is given, it is likely that the data given is not font file data.
					#define MUTT_INVALID_DIRECTORY_LENGTH 64
					// @DOCLINE * `MUTT_INVALID_DIRECTORY_SFNT_VERSION` - the value of "sfntVersion" in the table directory was invalid/unsupported. This is the first check performed on the values within the font file data, meaning that if this result is given, it is likely that the data given is not TrueType font file data. This can also be triggered by the TrueType font using CFF data, which is currently unsupported.
					#define MUTT_INVALID_DIRECTORY_SFNT_VERSION 65
					// @DOCLINE * `MUTT_INVALID_DIRECTORY_NUM_TABLES` - the value of "numTables" in the table directory was invalid; the number of tables must be at least 9 to store all tables required in TrueType.
					#define MUTT_INVALID_DIRECTORY_NUM_TABLES 66
					// @DOCLINE * `MUTT_INVALID_DIRECTORY_RECORD_OFFSET` - the value of "offset" in a table record within the table directory was out of range.
					#define MUTT_INVALID_DIRECTORY_RECORD_OFFSET 67
					// @DOCLINE * `MUTT_INVALID_DIRECTORY_RECORD_LENGTH` - the value of "length" in a table record within the table directory was out of range.
					#define MUTT_INVALID_DIRECTORY_RECORD_LENGTH 68
					// @DOCLINE * `MUTT_INVALID_DIRECTORY_RECORD_CHECKSUM` - the value of "checksum" in a table record within the table directory was invalid, implying that the table data was incorrect.
					#define MUTT_INVALID_DIRECTORY_RECORD_CHECKSUM 69
					// @DOCLINE * `MUTT_INVALID_DIRECTORY_RECORD_TABLE_TAG` - the table tag of a table within the table directory was a duplicate of a previous one.
					#define MUTT_INVALID_DIRECTORY_RECORD_TABLE_TAG 70
					// @DOCLINE * `MUTT_MISSING_DIRECTORY_RECORD_TABLE_TAGS` - one or more tables required by TrueType standards could not be found in the table directory.
					#define MUTT_MISSING_DIRECTORY_RECORD_TABLE_TAGS 71

				// @DOCLINE ### Maxp result values
				// 128 -> 191 //

					// @DOCLINE * `MUTT_INVALID_MAXP_LENGTH` - the length of the maxp table was invalid.
					#define MUTT_INVALID_MAXP_LENGTH 128
					// @DOCLINE * `MUTT_INVALID_MAXP_VERSION` - the version of the maxp table given was invalid/unsupported. This likely means that the font has a CFF/CFF2 font outline, which is currently unsupported.
					#define MUTT_INVALID_MAXP_VERSION 129
					// @DOCLINE * `MUTT_INVALID_MAXP_NUM_GLYPHS` - the value for "numGlyphs" given in the maxp table was invalid.
					#define MUTT_INVALID_MAXP_NUM_GLYPHS 130
					// @DOCLINE * `MUTT_INVALID_MAXP_MAX_ZONES` - the value for "maxZones" given in the maxp table was invalid.
					#define MUTT_INVALID_MAXP_MAX_ZONES 131

				// @DOCLINE ### Head result values
				// 192 -> 255 //

					// @DOCLINE * `MUTT_INVALID_HEAD_LENGTH` - the length of the head table was invalid.
					#define MUTT_INVALID_HEAD_LENGTH 192
					// @DOCLINE * `MUTT_INVALID_HEAD_VERSION` - the version indicated for the head table was invalid/unsupported.
					#define MUTT_INVALID_HEAD_VERSION 193
					// @DOCLINE * `MUTT_INVALID_HEAD_MAGIC_NUMBER` - the value for the magic number in the head table was invalid.
					#define MUTT_INVALID_HEAD_MAGIC_NUMBER 194
					// @DOCLINE * `MUTT_INVALID_HEAD_UNITS_PER_EM` - the value for the units per em in the head table was not within the correct range of 16 to 16384.
					#define MUTT_INVALID_HEAD_UNITS_PER_EM 195
					// @DOCLINE * `MUTT_INVALID_HEAD_X_MIN_COORDINATES` - the value "xMin" in the head table was not within the valid TrueType grid coordinate range of -16384 to 16383.
					#define MUTT_INVALID_HEAD_X_MIN_COORDINATES 196
					// @DOCLINE * `MUTT_INVALID_HEAD_Y_MIN_COORDINATES` - the value "yMin" in the head table was not within the valid TrueType grid coordinate range of -16384 to 16383.
					#define MUTT_INVALID_HEAD_Y_MIN_COORDINATES 197
					// @DOCLINE * `MUTT_INVALID_HEAD_X_MAX_COORDINATES` - the value "xMax" in the head table was not within the valid TrueType grid coordinate range of -16384 to 16383.
					#define MUTT_INVALID_HEAD_X_MAX_COORDINATES 198
					// @DOCLINE * `MUTT_INVALID_HEAD_Y_MAX_COORDINATES` - the value "yMax" in the head table was not within the valid TrueType grid coordinate range of -16384 to 16383.
					#define MUTT_INVALID_HEAD_Y_MAX_COORDINATES 199
					// @DOCLINE * `MUTT_INVALID_HEAD_X_MIN_MAX` - the value established minimum x-value within the head table was greater than the established maximum x-value.
					#define MUTT_INVALID_HEAD_X_MIN_MAX 200
					// @DOCLINE * `MUTT_INVALID_HEAD_Y_MIN_MAX` - the value established minimum y-value within the head table was greater than the established maximum y-value.
					#define MUTT_INVALID_HEAD_Y_MIN_MAX 201
					// @DOCLINE * `MUTT_INVALID_HEAD_INDEX_TO_LOC_FORMAT` - the value for "indexToLocFormat" within the head table was invalid/unsupported; it was not one of the expected values 0 (Offset16) or 1 (Offset32).
					#define MUTT_INVALID_HEAD_INDEX_TO_LOC_FORMAT 202
					// @DOCLINE * `MUTT_INVALID_HEAD_GLYPH_DATA_FORMAT` - the value for "glyphDataFormat" within the head table was invalid/unsupported; it was not the expected value 0.
					#define MUTT_INVALID_HEAD_GLYPH_DATA_FORMAT 203

				// @DOCLINE ### Hhea result values
				// 256 -> 319 //

					// @DOCLINE * `MUTT_INVALID_HHEA_LENGTH` - the length of the hhea table was invalid.
					#define MUTT_INVALID_HHEA_LENGTH 256
					// @DOCLINE * `MUTT_INVALID_HHEA_VERSION` - the version indicated for the hhea table was invalid/unsupported.
					#define MUTT_INVALID_HHEA_VERSION 257
					// @DOCLINE * `MUTT_INVALID_HHEA_METRIC_DATA_FORMAT` - the value for "metricDataFormat" within the hhea table was invalid/unsupported; it was not the expected value 0.
					#define MUTT_INVALID_HHEA_METRIC_DATA_FORMAT 258
					// @DOCLINE * `MUTT_INVALID_HHEA_NUMBER_OF_HMETRICS` - the value for "numberOfHMetrics" within the hhea table was invalid; numberOfHMetrics must be less than or equal to "numGlyphs" in order to generate a valid array length for "leftSideBearings" within hmtx.
					#define MUTT_INVALID_HHEA_NUMBER_OF_HMETRICS 259
					// @DOCLINE * `MUTT_HHEA_REQUIRES_MAXP` - the maxp table rather failed to load or was not requested for loading, and hhea requires maxp to be loaded.
					#define MUTT_HHEA_REQUIRES_MAXP 260

				// @DOCLINE ### Hmtx result values
				// 320 -> 383 //

					// @DOCLINE * `MUTT_INVALID_HMTX_LENGTH` - the length of the hmtx table was invalid.
					#define MUTT_INVALID_HMTX_LENGTH 320
					// @DOCLINE * `MUTT_HMTX_REQUIRES_MAXP` - the maxp table rather failed to load or was not requested for loading, and hmtx requires maxp to be loaded.
					#define MUTT_HMTX_REQUIRES_MAXP 321
					// @DOCLINE * `MUTT_HMTX_REQUIRES_HHEA` - the hhea table rather failed to load or was not requested for loading, and hmtx requires hhea to be loaded.
					#define MUTT_HMTX_REQUIRES_HHEA 322

				// @DOCLINE ### Loca result values
				// 384 -> 447 //

					// @DOCLINE * `MUTT_INVALID_LOCA_LENGTH` - the length of the loca table was invalid.
					#define MUTT_INVALID_LOCA_LENGTH 284
					// @DOCLINE * `MUTT_INVALID_LOCA_OFFSET` - an offset in the loca table was invalid. This could mean that the offset's range was invalid for the glyf table, or that the rule of incremental offsets was violated.
					#define MUTT_INVALID_LOCA_OFFSET 285
					// @DOCLINE * `MUTT_LOCA_REQUIRES_MAXP` - the maxp table rather failed to load or was not requested for loading, and loca requires maxp to be loaded.
					#define MUTT_LOCA_REQUIRES_MAXP 286
					// @DOCLINE * `MUTT_LOCA_REQUIRES_HEAD` - the head table rather failed to load or was not requested for loading, and loca requires head to be loaded.
					#define MUTT_LOCA_REQUIRES_HEAD 287
					// @DOCLINE * `MUTT_LOCA_REQUIRES_GLYF` - the glyf table rather failed to load or was not requested for loading, and loca requires glyf to be loaded.
					#define MUTT_LOCA_REQUIRES_GLYF 288

				// @DOCLINE ### Name result values
				// 448 -> 511 //

					// @DOCLINE * `MUTT_INVALID_NAME_LENGTH` - the length of the name table was invalid.
					#define MUTT_INVALID_NAME_LENGTH 448
					// @DOCLINE * `MUTT_INVALID_NAME_VERSION` - the version of the name table was invalid/unsupported.
					#define MUTT_INVALID_NAME_VERSION 449
					// @DOCLINE * `MUTT_INVALID_NAME_STORAGE_OFFSET` - the offset given for the storage of string data within the name table was invalid / out of range.
					#define MUTT_INVALID_NAME_STORAGE_OFFSET 450
					// @DOCLINE * `MUTT_INVALID_NAME_LENGTH_OFFSET` - the length and offset given for a name record within the name table was invalid / out of range.
					#define MUTT_INVALID_NAME_LENGTH_OFFSET 451

				// @DOCLINE ### Glyf result values
				// 512 -> 575 //

					// @DOCLINE * `MUTT_INVALID_GLYF_HEADER_LENGTH` - the glyph header length given from values by the loca table were invalid; they were above 0, implying an outline, yet the length given was insufficient to store a glyph header.
					#define MUTT_INVALID_GLYF_HEADER_LENGTH 512
					// @DOCLINE * `MUTT_INVALID_GLYF_HEADER_NUMBER_OF_CONTOURS` - the number of contours within the glyph header exceeded the maximum set by the maxp table.
					#define MUTT_INVALID_GLYF_HEADER_NUMBER_OF_CONTOURS 513
					// @DOCLINE * `MUTT_INVALID_GLYF_HEADER_X_MIN` - the glyph header's xMin value was not in range of the head table's listed corresponding value.
					#define MUTT_INVALID_GLYF_HEADER_X_MIN 514
					// @DOCLINE * `MUTT_INVALID_GLYF_HEADER_Y_MIN` - the glyph header's yMin value was not in range of the head table's listed corresponding value.
					#define MUTT_INVALID_GLYF_HEADER_Y_MIN 515
					// @DOCLINE * `MUTT_INVALID_GLYF_HEADER_X_MAX` - the glyph header's xMax value was not in range of the head table's listed corresponding value.
					#define MUTT_INVALID_GLYF_HEADER_X_MAX 516
					// @DOCLINE * `MUTT_INVALID_GLYF_HEADER_Y_MAX` - the glyph header's yMax value was not in range of the head table's listed corresponding value.
					#define MUTT_INVALID_GLYF_HEADER_Y_MAX 517
					// @DOCLINE * `MUTT_INVALID_GLYF_HEADER_X_MIN_MAX` - the glyph header's xMin value was greater than its xMax value or vice versa, which does not make sense.
					#define MUTT_INVALID_GLYF_HEADER_X_MIN_MAX 518
					// @DOCLINE * `MUTT_INVALID_GLYF_HEADER_Y_MIN_MAX` - the glyph header's yMin value was greater than its yMax value or vice versa, which does not make sense.
					#define MUTT_INVALID_GLYF_HEADER_Y_MIN_MAX 519

					// @DOCLINE * `MUTT_INVALID_GLYF_SIMPLE_LENGTH` - the length of the simple glyph description is invalid/insufficient to describe the simple glyph.
					#define MUTT_INVALID_GLYF_SIMPLE_LENGTH 520
					// @DOCLINE * `MUTT_INVALID_GLYF_SIMPLE_END_PTS_OF_CONTOURS` - a value within the endPtsOfContours array of the simple glyph was invalid; rather the value was non-incremental, or the last index was the invalid value 0xFFFF.
					#define MUTT_INVALID_GLYF_SIMPLE_END_PTS_OF_CONTOURS 521
					// @DOCLINE * `MUTT_INVALID_GLYF_SIMPLE_POINT_COUNT` - the amount of points specified within the simple glyph exceeded the maximum set by the maxp table.
					#define MUTT_INVALID_GLYF_SIMPLE_POINT_COUNT 522
					// @DOCLINE * `MUTT_INVALID_GLYF_SIMPLE_INSTRUCTION_LENGTH` - the instruction length given by the simple glyph exceeded the maximum set by the maxp table.
					#define MUTT_INVALID_GLYF_SIMPLE_INSTRUCTION_LENGTH 523
					// @DOCLINE * `MUTT_INVALID_GLYF_SIMPLE_X_COORD` - an x-coordinate within the simple glyph was out of range for its listed minimum/maximum values. This is non-fatal, as mutt automatically overwrites the existing min/max values within the header.
					#define MUTT_INVALID_GLYF_SIMPLE_X_COORD 524
					// @DOCLINE * `MUTT_INVALID_GLYF_SIMPLE_Y_COORD` - a y-coordinate within the simple glyph was out of range for its minimum/maximum values. This is non-fatal, as mutt automatically overwrites the existing min/max values within the header.
					#define MUTT_INVALID_GLYF_SIMPLE_Y_COORD 525

					// @DOCLINE * `MUTT_INVALID_GLYF_COMPOSITE_LENGTH` - the length of the composite glyph description is invalid/insufficient to describe the composite glyph.
					#define MUTT_INVALID_GLYF_COMPOSITE_LENGTH 526
					// @DOCLINE * `MUTT_INVALID_GLYF_COMPOSITE_INSTRUCTION_LENGTH` - the instruction length given by the composite glyph exceeded the maximum set by the maxp table.
					#define MUTT_INVALID_GLYF_COMPOSITE_INSTRUCTION_LENGTH 527
					// @DOCLINE * `MUTT_INVALID_GLYF_COMPOSITE_COMPONENT_COUNT` - the amount of components given in the composite glyph exceeded the maximum set by the maxp table.
					#define MUTT_INVALID_GLYF_COMPOSITE_COMPONENT_COUNT 528
					// @DOCLINE * `MUTT_INVALID_GLYF_COMPOSITE_GLYPH_INDEX` - the value for "glyphIndex" in a component within the composite glyph was an invalid glyph index (out of range for the number of glyphs specified in maxp).
					#define MUTT_INVALID_GLYF_COMPOSITE_GLYPH_INDEX 529
					// @DOCLINE * `MUTT_INVALID_GLYF_COMPOSITE_FLAGS` - the flags in a component within the composite glyph were invalid (multiple mutually exclusive transform data flags were set).
					#define MUTT_INVALID_GLYF_COMPOSITE_FLAGS 530

					// @DOCLINE * `MUTT_INVALID_GLYF_SIMPLE_X_COORD_FUNITS` - an x-coordinate within the simple glyph was out of the font-unit range.
					#define MUTT_INVALID_GLYF_SIMPLE_X_COORD_FUNITS 531
					// @DOCLINE * `MUTT_INVALID_GLYF_SIMPLE_Y_COORD_FUNITS` - a y-coordinate within the simple glyph was out of the font-unit range.
					#define MUTT_INVALID_GLYF_SIMPLE_Y_COORD_FUNITS 532
					// @DOCLINE * `MUTT_INVALID_GLYF_COMPOSITE_X_COORD_FUNITS` - an x-coordinate within the composite glyph was out of the font-unit range. This check is only performed when converting a composite glyph to an rglyph or calculating the x/y min/max range of a composite glyph.
					#define MUTT_INVALID_GLYF_COMPOSITE_X_COORD_FUNITS 533
					// @DOCLINE * `MUTT_INVALID_GLYF_COMPOSITE_Y_COORD_FUNITS` - a y-coordinate within the composite glyph was out of the font-unit range. This check is only performed when converting a composite glyph to an rglyph or calculating the x/y min/max range of a composite glyph.
					#define MUTT_INVALID_GLYF_COMPOSITE_Y_COORD_FUNITS 534

				// @DOCLINE ### Cmap result values
				// 576 -> 639 //

					// @DOCLINE * `MUTT_INVALID_CMAP_LENGTH` - the length of the cmap table was invalid.
					#define MUTT_INVALID_CMAP_LENGTH 576
					// @DOCLINE * `MUTT_INVALID_CMAP_VERSION` - the version of the cmap table was invalid/unsupported.
					#define MUTT_INVALID_CMAP_VERSION 577

					// @DOCLINE * `MUTT_INVALID_CMAP_ENCODING_RECORD_OFFSET` - an encoding record's subtable offset was invalid (AKA out of range for the cmap table).
					#define MUTT_INVALID_CMAP_ENCODING_RECORD_OFFSET 578
					// @DOCLINE * `MUTT_INVALID_CMAP_ENCODING_RECORD_LENGTH` - an encoding record's subtable length was invalid (AKA the offset's distance from the end of the cmap table was not long enough to figure out its format).
					#define MUTT_INVALID_CMAP_ENCODING_RECORD_LENGTH 579
					// @DOCLINE * `MUTT_INVALID_CMAP_ENCODING_RECORD_FORMAT` - the encoding record's format was invalid/unsupported.
					#define MUTT_INVALID_CMAP_ENCODING_RECORD_FORMAT 580

					// @DOCLINE * `MUTT_INVALID_CMAP0_LENGTH` - the length of the cmap format 0 subtable was invalid.
					#define MUTT_INVALID_CMAP0_LENGTH 581

					// @DOCLINE * `MUTT_INVALID_CMAP4_LENGTH` - the length of the cmap format 4 subtable was invalid/insufficient to define the data needed.
					#define MUTT_INVALID_CMAP4_LENGTH 582
					// @DOCLINE * `MUTT_INVALID_CMAP4_SEG_COUNT_X2` - the value given for "segCountX2" in the cmap format 4 subtable was not divisible by 2, and was therefore an invalid value.
					#define MUTT_INVALID_CMAP4_SEG_COUNT_X2 583
					// @DOCLINE * `MUTT_INVALID_CMAP4_END_CODE` - an "endCode" value for a segment in the cmap format 4 subtable broke the incremental order of the endCode values.
					#define MUTT_INVALID_CMAP4_END_CODE 584
					// @DOCLINE * `MUTT_INVALID_CMAP4_LAST_END_CODE` - the last "endCode" value in the cmap format 4 subtable was not 0xFFFF, which is required in the TrueType specification.
					#define MUTT_INVALID_CMAP4_LAST_END_CODE 585
					// @DOCLINE * `MUTT_INVALID_CMAP4_START_CODE` - a "startCode" value for a segment in the cmap format 4 subtable was greater than its endCode.
					#define MUTT_INVALID_CMAP4_START_CODE 586
					// @DOCLINE * `MUTT_INVALID_CMAP4_ID_RANGE_OFFSET` - an "idRangeOffset" value for a segment in the cmap format 4 subtable was rather out of range for indexes into glyphIdArray, or was not divisible by 2 (which it must be, since it's a byte-offset into a 2-byte-integer array, starting from a 2-byte-integer array).
					#define MUTT_INVALID_CMAP4_ID_RANGE_OFFSET 587

					// @DOCLINE * `MUTT_INVALID_CMAP12_LENGTH` - the length of the cmap format 12 subtable was invalid/in sufficient to define the data needed.
					#define MUTT_INVALID_CMAP12_LENGTH 588
					// @DOCLINE * `MUTT_INVALID_CMAP12_START_CHAR_CODE` - a "startCharCode" value for a map group in the cmap format 12 subtable was not incremental compared to the previous group.
					#define MUTT_INVALID_CMAP12_START_CHAR_CODE 589
					// @DOCLINE * `MUTT_INVALID_CMAP12_END_CHAR_CODE` - an "endCharCode" value for a map group in the cmap format 12 subtable was not less than the startCharCode value of the next group.
					#define MUTT_INVALID_CMAP12_END_CHAR_CODE 590

					// @DOCLINE * `MUTT_CMAP_REQUIRES_MAXP` - the maxp table rather failed to load or was not requested for loading, and cmap requires maxp to be loaded.
					#define MUTT_CMAP_REQUIRES_MAXP 639

				// @DOCLINE ### Rasterization result values
				// 640 -> 703 //

					// @DOCLINE * `MUTT_UNKNOWN_RASTER_METHOD` - the given raster method value was unrecognized.
					#define MUTT_UNKNOWN_RASTER_METHOD 640

					// @DOCLINE * `MUTT_INVALID_RGLYPH_COMPOSITE_CONTOUR_COUNT` - the process of converting a composite glyph to an rglyph failed because the given composite glyph had more contours than the maximum contour count indicated in the maxp table.
					#define MUTT_INVALID_RGLYPH_COMPOSITE_CONTOUR_COUNT 641

					// @DOCLINE * `MUTT_INVALID_RGLYPH_COMPOSITE_POINT_COUNT` - the process of converting a composite glyph to an rglyph failed because the given composite glyph had more points than the maximum point count indicated in the maxp table.
					#define MUTT_INVALID_RGLYPH_COMPOSITE_POINT_COUNT 642

					// @DOCLINE * `MUTT_INVALID_RGLYPH_COMPOSITE_DEPTH` - the process of converting a composite glyph to an rglyph failed because the given composite glyph had a larger component depth than the maximum component depth indicated in the maxp table.
					#define MUTT_INVALID_RGLYPH_COMPOSITE_DEPTH 643

					// @DOCLINE * `MUTT_INVALID_RGLYPH_COMPOSITE_COMPONENT_COUNT` - the process of converting a composite glyph to an rglyph failed because the given composite glyph (or a composite component within it) had more components than the maximum component count indicated in the maxp table.
					#define MUTT_INVALID_RGLYPH_COMPOSITE_COMPONENT_COUNT 644

					// @DOCLINE * `MUTT_INVALID_RGLYPH_COMPOSITE_COMPONENT_ARGUMENT1` - the process of converting a composite glyph to an rglyph failed because a simple glyph had an argument1 value giving a point number that was out of range for the parent glyph.
					#define MUTT_INVALID_RGLYPH_COMPOSITE_COMPONENT_ARGUMENT1 645

					// @DOCLINE * `MUTT_INVALID_RGLYPH_COMPOSITE_COMPONENT_ARGUMENT2` - the process of converting a composite glyph to an rglyph failed because a simple glyph had an argument2 value giving a point number that was out of range for the child glyph.
					#define MUTT_INVALID_RGLYPH_COMPOSITE_COMPONENT_ARGUMENT2 646

				// @DOCLINE ## Check if result is fatal

					// @DOCLINE The function `mutt_result_is_fatal` returns whether or not a given `muttResult` value is fatal, defined below: @NLNT
					MUDEF muBool mutt_result_is_fatal(muttResult result);

					// @DOCLINE This function returns `MU_TRUE` if the value of `result` is invalid.

				// @DOCLINE ## Result name

					#ifdef MUTT_NAMES

					// @DOCLINE The function `mutt_result_get_name` returns a `const char*` representation of a given result value (for example, `MUTT_SUCCESS` returns "MUTT_SUCCESS"), defined below: @NLNT
					MUDEF const char* mutt_result_get_name(muttResult result);

					// @DOCLINE This function returns "MU_UNKNOWN" in the case that `result` is an invalid result value.

					// @DOCLINE > This function is a "name" function, and therefore is only defined if `MUTT_NAMES` is also defined.

					#endif

			// @DOCLINE # C standard library dependencies

				// @DOCLINE mutt has several C standard library dependencies, all of which are overridable by defining them before the inclusion of its header. The following is a list of those dependencies.

				#if !defined(mu_malloc) || \
					!defined(mu_free) || \
					!defined(mu_realloc) || \
					!defined(mu_qsort)

					// @DOCLINE ## `stdlib.h` dependencies
					#include <stdlib.h>

					// @DOCLINE * `mu_malloc` - equivalent to `malloc`.
					#ifndef mu_malloc
						#define mu_malloc malloc
					#endif

					// @DOCLINE * `mu_free` - equivalent to `free`.
					#ifndef mu_free
						#define mu_free free
					#endif

					// @DOCLINE * `mu_realloc` - equivalent to `realloc`.
					#ifndef mu_realloc
						#define mu_realloc realloc
					#endif

					// @DOCLINE * `mu_qsort` equivalent to `qsort`.
					#ifndef mu_qsort
						#define mu_qsort qsort
					#endif

				#endif /* stdlib.h */

				#if !defined(mu_memcpy) || \
					!defined(mu_memset)

					// @DOCLINE ## `string.h` dependencies
					#include <string.h>

					// @DOCLINE * `mu_memcpy` - equivalent to `memcpy`.
					#ifndef mu_memcpy
						#define mu_memcpy memcpy
					#endif

					// @DOCLINE * `mu_memset`- equivalent to `memset`.
					#ifndef mu_memset
						#define mu_memset memset
					#endif

				#endif /* string.h */

				#if !defined(mu_fabsf) || \
					!defined(mu_roundf) || \
					!defined(mu_ceilf)

					// @DOCLINE ## `math.h` dependencies
					#include <math.h>

					// @DOCLINE * `mu_fabsf` - equivalent to `fabsf`.
					#ifndef mu_fabsf
						#define mu_fabsf fabsf
					#endif

					// @DOCLINE * `mu_roundf` - equivalent to `roundf`.
					#ifndef mu_roundf
						#define mu_roundf roundf
					#endif

					// @DOCLINE * `mu_ceilf` - equivalent to `ceilf`.
					#ifndef mu_ceilf
						#define mu_ceilf ceilf
					#endif

				#endif /* math.h */

			MU_CPP_EXTERN_END
		#endif /* MUTT_H */
	#endif /* MUSDK_MUTT */
	// @ATTENTION

#endif /* MUSDK_H */

// @IGNORE
#ifdef MUSDK_IMPLEMENTATION

	// mug
	// https://github.com/Muukid/muGraphics/releases/tag/v1.0.0
	#ifdef MUSDK_MUG
	#define MUG_IMPLEMENTATION

	// Debug inclusion:
	#ifdef MUSDK_INCLUDE_LIBS
	#include "muGraphics.h"
	// Standard inclusion:
	#else
	#ifdef MUG_IMPLEMENTATION

		#ifdef MU_SUPPORT_OPENGL
		// glad header and implementation
		// Changes (implementation):
		// - Removed space between last extern "C" and the #endif for GLAD_GL_IMPLEMENTATION.
		#ifndef GLAD_GL_H_
			#define GLAD_GL_H_

			#ifdef __clang__
			#pragma clang diagnostic push
			#pragma clang diagnostic ignored "-Wreserved-id-macro"
			#endif
			#ifdef __gl_h_
			  #error OpenGL (gl.h) header already included (API: gl), remove previous include!
			#endif
			#define __gl_h_ 1
			#ifdef __gl3_h_
			  #error OpenGL (gl3.h) header already included (API: gl), remove previous include!
			#endif
			#define __gl3_h_ 1
			#ifdef __glext_h_
			  #error OpenGL (glext.h) header already included (API: gl), remove previous include!
			#endif
			#define __glext_h_ 1
			#ifdef __gl3ext_h_
			  #error OpenGL (gl3ext.h) header already included (API: gl), remove previous include!
			#endif
			#define __gl3ext_h_ 1
			#ifdef __clang__
			#pragma clang diagnostic pop
			#endif

			#define GLAD_GL
			#define GLAD_OPTION_GL_HEADER_ONLY
			#define GLAD_OPTION_GL_LOADER

			#ifdef __cplusplus
			extern "C" {
			#endif

			#ifndef GLAD_PLATFORM_H_
			#define GLAD_PLATFORM_H_

			#ifndef GLAD_PLATFORM_WIN32
			  #if defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(__MINGW32__)
			    #define GLAD_PLATFORM_WIN32 1
			  #else
			    #define GLAD_PLATFORM_WIN32 0
			  #endif
			#endif

			#ifndef GLAD_PLATFORM_APPLE
			  #ifdef __APPLE__
			    #define GLAD_PLATFORM_APPLE 1
			  #else
			    #define GLAD_PLATFORM_APPLE 0
			  #endif
			#endif

			#ifndef GLAD_PLATFORM_EMSCRIPTEN
			  #ifdef __EMSCRIPTEN__
			    #define GLAD_PLATFORM_EMSCRIPTEN 1
			  #else
			    #define GLAD_PLATFORM_EMSCRIPTEN 0
			  #endif
			#endif

			#ifndef GLAD_PLATFORM_UWP
			  #if defined(_MSC_VER) && !defined(GLAD_INTERNAL_HAVE_WINAPIFAMILY)
			    #ifdef __has_include
			      #if __has_include(<winapifamily.h>)
			        #define GLAD_INTERNAL_HAVE_WINAPIFAMILY 1
			      #endif
			    #elif _MSC_VER >= 1700 && !_USING_V110_SDK71_
			      #define GLAD_INTERNAL_HAVE_WINAPIFAMILY 1
			    #endif
			  #endif

			  #ifdef GLAD_INTERNAL_HAVE_WINAPIFAMILY
			    #include <winapifamily.h>
			    #if !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) && WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP)
			      #define GLAD_PLATFORM_UWP 1
			    #endif
			  #endif

			  #ifndef GLAD_PLATFORM_UWP
			    #define GLAD_PLATFORM_UWP 0
			  #endif
			#endif

			#ifdef __GNUC__
			  #define GLAD_GNUC_EXTENSION __extension__
			#else
			  #define GLAD_GNUC_EXTENSION
			#endif

			#define GLAD_UNUSED(x) (void)(x)

			#ifndef GLAD_API_CALL
			  #if defined(GLAD_API_CALL_EXPORT)
			    #if GLAD_PLATFORM_WIN32 || defined(__CYGWIN__)
			      #if defined(GLAD_API_CALL_EXPORT_BUILD)
			        #if defined(__GNUC__)
			          #define GLAD_API_CALL __attribute__ ((dllexport)) extern
			        #else
			          #define GLAD_API_CALL __declspec(dllexport) extern
			        #endif
			      #else
			        #if defined(__GNUC__)
			          #define GLAD_API_CALL __attribute__ ((dllimport)) extern
			        #else
			          #define GLAD_API_CALL __declspec(dllimport) extern
			        #endif
			      #endif
			    #elif defined(__GNUC__) && defined(GLAD_API_CALL_EXPORT_BUILD)
			      #define GLAD_API_CALL __attribute__ ((visibility ("default"))) extern
			    #else
			      #define GLAD_API_CALL extern
			    #endif
			  #else
			    #define GLAD_API_CALL extern
			  #endif
			#endif

			#ifdef APIENTRY
			  #define GLAD_API_PTR APIENTRY
			#elif GLAD_PLATFORM_WIN32
			  #define GLAD_API_PTR __stdcall
			#else
			  #define GLAD_API_PTR
			#endif

			#ifndef GLAPI
			#define GLAPI GLAD_API_CALL
			#endif

			#ifndef GLAPIENTRY
			#define GLAPIENTRY GLAD_API_PTR
			#endif

			#define GLAD_MAKE_VERSION(major, minor) (major * 10000 + minor)
			#define GLAD_VERSION_MAJOR(version) (version / 10000)
			#define GLAD_VERSION_MINOR(version) (version % 10000)

			#define GLAD_GENERATOR_VERSION "2.0.6"

			typedef void (*GLADapiproc)(void);

			typedef GLADapiproc (*GLADloadfunc)(const char *name);
			typedef GLADapiproc (*GLADuserptrloadfunc)(void *userptr, const char *name);

			typedef void (*GLADprecallback)(const char *name, GLADapiproc apiproc, int len_args, ...);
			typedef void (*GLADpostcallback)(void *ret, const char *name, GLADapiproc apiproc, int len_args, ...);

			#endif /* GLAD_PLATFORM_H_ */

			#define GL_ACTIVE_ATTRIBUTES 0x8B89
			#define GL_ACTIVE_ATTRIBUTE_MAX_LENGTH 0x8B8A
			#define GL_ACTIVE_TEXTURE 0x84E0
			#define GL_ACTIVE_UNIFORMS 0x8B86
			#define GL_ACTIVE_UNIFORM_BLOCKS 0x8A36
			#define GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH 0x8A35
			#define GL_ACTIVE_UNIFORM_MAX_LENGTH 0x8B87
			#define GL_ALIASED_LINE_WIDTH_RANGE 0x846E
			#define GL_ALPHA 0x1906
			#define GL_ALREADY_SIGNALED 0x911A
			#define GL_ALWAYS 0x0207
			#define GL_AND 0x1501
			#define GL_AND_INVERTED 0x1504
			#define GL_AND_REVERSE 0x1502
			#define GL_ANY_SAMPLES_PASSED 0x8C2F
			#define GL_ARRAY_BUFFER 0x8892
			#define GL_ARRAY_BUFFER_BINDING 0x8894
			#define GL_ATTACHED_SHADERS 0x8B85
			#define GL_BACK 0x0405
			#define GL_BACK_LEFT 0x0402
			#define GL_BACK_RIGHT 0x0403
			#define GL_BGR 0x80E0
			#define GL_BGRA 0x80E1
			#define GL_BGRA_INTEGER 0x8D9B
			#define GL_BGR_INTEGER 0x8D9A
			#define GL_BLEND 0x0BE2
			#define GL_BLEND_COLOR 0x8005
			#define GL_BLEND_DST 0x0BE0
			#define GL_BLEND_DST_ALPHA 0x80CA
			#define GL_BLEND_DST_RGB 0x80C8
			#define GL_BLEND_EQUATION 0x8009
			#define GL_BLEND_EQUATION_ALPHA 0x883D
			#define GL_BLEND_EQUATION_RGB 0x8009
			#define GL_BLEND_SRC 0x0BE1
			#define GL_BLEND_SRC_ALPHA 0x80CB
			#define GL_BLEND_SRC_RGB 0x80C9
			#define GL_BLUE 0x1905
			#define GL_BLUE_INTEGER 0x8D96
			#define GL_BOOL 0x8B56
			#define GL_BOOL_VEC2 0x8B57
			#define GL_BOOL_VEC3 0x8B58
			#define GL_BOOL_VEC4 0x8B59
			#define GL_BUFFER_ACCESS 0x88BB
			#define GL_BUFFER_ACCESS_FLAGS 0x911F
			#define GL_BUFFER_MAPPED 0x88BC
			#define GL_BUFFER_MAP_LENGTH 0x9120
			#define GL_BUFFER_MAP_OFFSET 0x9121
			#define GL_BUFFER_MAP_POINTER 0x88BD
			#define GL_BUFFER_SIZE 0x8764
			#define GL_BUFFER_USAGE 0x8765
			#define GL_BYTE 0x1400
			#define GL_CCW 0x0901
			#define GL_CLAMP_READ_COLOR 0x891C
			#define GL_CLAMP_TO_BORDER 0x812D
			#define GL_CLAMP_TO_EDGE 0x812F
			#define GL_CLEAR 0x1500
			#define GL_CLIP_DISTANCE0 0x3000
			#define GL_CLIP_DISTANCE1 0x3001
			#define GL_CLIP_DISTANCE2 0x3002
			#define GL_CLIP_DISTANCE3 0x3003
			#define GL_CLIP_DISTANCE4 0x3004
			#define GL_CLIP_DISTANCE5 0x3005
			#define GL_CLIP_DISTANCE6 0x3006
			#define GL_CLIP_DISTANCE7 0x3007
			#define GL_COLOR 0x1800
			#define GL_COLOR_ATTACHMENT0 0x8CE0
			#define GL_COLOR_ATTACHMENT1 0x8CE1
			#define GL_COLOR_ATTACHMENT10 0x8CEA
			#define GL_COLOR_ATTACHMENT11 0x8CEB
			#define GL_COLOR_ATTACHMENT12 0x8CEC
			#define GL_COLOR_ATTACHMENT13 0x8CED
			#define GL_COLOR_ATTACHMENT14 0x8CEE
			#define GL_COLOR_ATTACHMENT15 0x8CEF
			#define GL_COLOR_ATTACHMENT16 0x8CF0
			#define GL_COLOR_ATTACHMENT17 0x8CF1
			#define GL_COLOR_ATTACHMENT18 0x8CF2
			#define GL_COLOR_ATTACHMENT19 0x8CF3
			#define GL_COLOR_ATTACHMENT2 0x8CE2
			#define GL_COLOR_ATTACHMENT20 0x8CF4
			#define GL_COLOR_ATTACHMENT21 0x8CF5
			#define GL_COLOR_ATTACHMENT22 0x8CF6
			#define GL_COLOR_ATTACHMENT23 0x8CF7
			#define GL_COLOR_ATTACHMENT24 0x8CF8
			#define GL_COLOR_ATTACHMENT25 0x8CF9
			#define GL_COLOR_ATTACHMENT26 0x8CFA
			#define GL_COLOR_ATTACHMENT27 0x8CFB
			#define GL_COLOR_ATTACHMENT28 0x8CFC
			#define GL_COLOR_ATTACHMENT29 0x8CFD
			#define GL_COLOR_ATTACHMENT3 0x8CE3
			#define GL_COLOR_ATTACHMENT30 0x8CFE
			#define GL_COLOR_ATTACHMENT31 0x8CFF
			#define GL_COLOR_ATTACHMENT4 0x8CE4
			#define GL_COLOR_ATTACHMENT5 0x8CE5
			#define GL_COLOR_ATTACHMENT6 0x8CE6
			#define GL_COLOR_ATTACHMENT7 0x8CE7
			#define GL_COLOR_ATTACHMENT8 0x8CE8
			#define GL_COLOR_ATTACHMENT9 0x8CE9
			#define GL_COLOR_BUFFER_BIT 0x00004000
			#define GL_COLOR_CLEAR_VALUE 0x0C22
			#define GL_COLOR_LOGIC_OP 0x0BF2
			#define GL_COLOR_WRITEMASK 0x0C23
			#define GL_COMPARE_REF_TO_TEXTURE 0x884E
			#define GL_COMPILE_STATUS 0x8B81
			#define GL_COMPRESSED_RED 0x8225
			#define GL_COMPRESSED_RED_RGTC1 0x8DBB
			#define GL_COMPRESSED_RG 0x8226
			#define GL_COMPRESSED_RGB 0x84ED
			#define GL_COMPRESSED_RGBA 0x84EE
			#define GL_COMPRESSED_RG_RGTC2 0x8DBD
			#define GL_COMPRESSED_SIGNED_RED_RGTC1 0x8DBC
			#define GL_COMPRESSED_SIGNED_RG_RGTC2 0x8DBE
			#define GL_COMPRESSED_SRGB 0x8C48
			#define GL_COMPRESSED_SRGB_ALPHA 0x8C49
			#define GL_COMPRESSED_TEXTURE_FORMATS 0x86A3
			#define GL_CONDITION_SATISFIED 0x911C
			#define GL_CONSTANT_ALPHA 0x8003
			#define GL_CONSTANT_COLOR 0x8001
			#define GL_CONTEXT_COMPATIBILITY_PROFILE_BIT 0x00000002
			#define GL_CONTEXT_CORE_PROFILE_BIT 0x00000001
			#define GL_CONTEXT_FLAGS 0x821E
			#define GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT 0x00000001
			#define GL_CONTEXT_PROFILE_MASK 0x9126
			#define GL_COPY 0x1503
			#define GL_COPY_INVERTED 0x150C
			#define GL_COPY_READ_BUFFER 0x8F36
			#define GL_COPY_WRITE_BUFFER 0x8F37
			#define GL_CULL_FACE 0x0B44
			#define GL_CULL_FACE_MODE 0x0B45
			#define GL_CURRENT_PROGRAM 0x8B8D
			#define GL_CURRENT_QUERY 0x8865
			#define GL_CURRENT_VERTEX_ATTRIB 0x8626
			#define GL_CW 0x0900
			#define GL_DECR 0x1E03
			#define GL_DECR_WRAP 0x8508
			#define GL_DELETE_STATUS 0x8B80
			#define GL_DEPTH 0x1801
			#define GL_DEPTH24_STENCIL8 0x88F0
			#define GL_DEPTH32F_STENCIL8 0x8CAD
			#define GL_DEPTH_ATTACHMENT 0x8D00
			#define GL_DEPTH_BUFFER_BIT 0x00000100
			#define GL_DEPTH_CLAMP 0x864F
			#define GL_DEPTH_CLEAR_VALUE 0x0B73
			#define GL_DEPTH_COMPONENT 0x1902
			#define GL_DEPTH_COMPONENT16 0x81A5
			#define GL_DEPTH_COMPONENT24 0x81A6
			#define GL_DEPTH_COMPONENT32 0x81A7
			#define GL_DEPTH_COMPONENT32F 0x8CAC
			#define GL_DEPTH_FUNC 0x0B74
			#define GL_DEPTH_RANGE 0x0B70
			#define GL_DEPTH_STENCIL 0x84F9
			#define GL_DEPTH_STENCIL_ATTACHMENT 0x821A
			#define GL_DEPTH_TEST 0x0B71
			#define GL_DEPTH_WRITEMASK 0x0B72
			#define GL_DITHER 0x0BD0
			#define GL_DONT_CARE 0x1100
			#define GL_DOUBLE 0x140A
			#define GL_DOUBLEBUFFER 0x0C32
			#define GL_DRAW_BUFFER 0x0C01
			#define GL_DRAW_BUFFER0 0x8825
			#define GL_DRAW_BUFFER1 0x8826
			#define GL_DRAW_BUFFER10 0x882F
			#define GL_DRAW_BUFFER11 0x8830
			#define GL_DRAW_BUFFER12 0x8831
			#define GL_DRAW_BUFFER13 0x8832
			#define GL_DRAW_BUFFER14 0x8833
			#define GL_DRAW_BUFFER15 0x8834
			#define GL_DRAW_BUFFER2 0x8827
			#define GL_DRAW_BUFFER3 0x8828
			#define GL_DRAW_BUFFER4 0x8829
			#define GL_DRAW_BUFFER5 0x882A
			#define GL_DRAW_BUFFER6 0x882B
			#define GL_DRAW_BUFFER7 0x882C
			#define GL_DRAW_BUFFER8 0x882D
			#define GL_DRAW_BUFFER9 0x882E
			#define GL_DRAW_FRAMEBUFFER 0x8CA9
			#define GL_DRAW_FRAMEBUFFER_BINDING 0x8CA6
			#define GL_DST_ALPHA 0x0304
			#define GL_DST_COLOR 0x0306
			#define GL_DYNAMIC_COPY 0x88EA
			#define GL_DYNAMIC_DRAW 0x88E8
			#define GL_DYNAMIC_READ 0x88E9
			#define GL_ELEMENT_ARRAY_BUFFER 0x8893
			#define GL_ELEMENT_ARRAY_BUFFER_BINDING 0x8895
			#define GL_EQUAL 0x0202
			#define GL_EQUIV 0x1509
			#define GL_EXTENSIONS 0x1F03
			#define GL_FALSE 0
			#define GL_FASTEST 0x1101
			#define GL_FILL 0x1B02
			#define GL_FIRST_VERTEX_CONVENTION 0x8E4D
			#define GL_FIXED_ONLY 0x891D
			#define GL_FLOAT 0x1406
			#define GL_FLOAT_32_UNSIGNED_INT_24_8_REV 0x8DAD
			#define GL_FLOAT_MAT2 0x8B5A
			#define GL_FLOAT_MAT2x3 0x8B65
			#define GL_FLOAT_MAT2x4 0x8B66
			#define GL_FLOAT_MAT3 0x8B5B
			#define GL_FLOAT_MAT3x2 0x8B67
			#define GL_FLOAT_MAT3x4 0x8B68
			#define GL_FLOAT_MAT4 0x8B5C
			#define GL_FLOAT_MAT4x2 0x8B69
			#define GL_FLOAT_MAT4x3 0x8B6A
			#define GL_FLOAT_VEC2 0x8B50
			#define GL_FLOAT_VEC3 0x8B51
			#define GL_FLOAT_VEC4 0x8B52
			#define GL_FRAGMENT_SHADER 0x8B30
			#define GL_FRAGMENT_SHADER_DERIVATIVE_HINT 0x8B8B
			#define GL_FRAMEBUFFER 0x8D40
			#define GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE 0x8215
			#define GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE 0x8214
			#define GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING 0x8210
			#define GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE 0x8211
			#define GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE 0x8216
			#define GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE 0x8213
			#define GL_FRAMEBUFFER_ATTACHMENT_LAYERED 0x8DA7
			#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME 0x8CD1
			#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE 0x8CD0
			#define GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE 0x8212
			#define GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE 0x8217
			#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE 0x8CD3
			#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER 0x8CD4
			#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL 0x8CD2
			#define GL_FRAMEBUFFER_BINDING 0x8CA6
			#define GL_FRAMEBUFFER_COMPLETE 0x8CD5
			#define GL_FRAMEBUFFER_DEFAULT 0x8218
			#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT 0x8CD6
			#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER 0x8CDB
			#define GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS 0x8DA8
			#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT 0x8CD7
			#define GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE 0x8D56
			#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER 0x8CDC
			#define GL_FRAMEBUFFER_SRGB 0x8DB9
			#define GL_FRAMEBUFFER_UNDEFINED 0x8219
			#define GL_FRAMEBUFFER_UNSUPPORTED 0x8CDD
			#define GL_FRONT 0x0404
			#define GL_FRONT_AND_BACK 0x0408
			#define GL_FRONT_FACE 0x0B46
			#define GL_FRONT_LEFT 0x0400
			#define GL_FRONT_RIGHT 0x0401
			#define GL_FUNC_ADD 0x8006
			#define GL_FUNC_REVERSE_SUBTRACT 0x800B
			#define GL_FUNC_SUBTRACT 0x800A
			#define GL_GEOMETRY_INPUT_TYPE 0x8917
			#define GL_GEOMETRY_OUTPUT_TYPE 0x8918
			#define GL_GEOMETRY_SHADER 0x8DD9
			#define GL_GEOMETRY_VERTICES_OUT 0x8916
			#define GL_GEQUAL 0x0206
			#define GL_GREATER 0x0204
			#define GL_GREEN 0x1904
			#define GL_GREEN_INTEGER 0x8D95
			#define GL_HALF_FLOAT 0x140B
			#define GL_INCR 0x1E02
			#define GL_INCR_WRAP 0x8507
			#define GL_INFO_LOG_LENGTH 0x8B84
			#define GL_INT 0x1404
			#define GL_INTERLEAVED_ATTRIBS 0x8C8C
			#define GL_INT_2_10_10_10_REV 0x8D9F
			#define GL_INT_SAMPLER_1D 0x8DC9
			#define GL_INT_SAMPLER_1D_ARRAY 0x8DCE
			#define GL_INT_SAMPLER_2D 0x8DCA
			#define GL_INT_SAMPLER_2D_ARRAY 0x8DCF
			#define GL_INT_SAMPLER_2D_MULTISAMPLE 0x9109
			#define GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY 0x910C
			#define GL_INT_SAMPLER_2D_RECT 0x8DCD
			#define GL_INT_SAMPLER_3D 0x8DCB
			#define GL_INT_SAMPLER_BUFFER 0x8DD0
			#define GL_INT_SAMPLER_CUBE 0x8DCC
			#define GL_INT_VEC2 0x8B53
			#define GL_INT_VEC3 0x8B54
			#define GL_INT_VEC4 0x8B55
			#define GL_INVALID_ENUM 0x0500
			#define GL_INVALID_FRAMEBUFFER_OPERATION 0x0506
			#define GL_INVALID_INDEX 0xFFFFFFFF
			#define GL_INVALID_OPERATION 0x0502
			#define GL_INVALID_VALUE 0x0501
			#define GL_INVERT 0x150A
			#define GL_KEEP 0x1E00
			#define GL_LAST_VERTEX_CONVENTION 0x8E4E
			#define GL_LEFT 0x0406
			#define GL_LEQUAL 0x0203
			#define GL_LESS 0x0201
			#define GL_LINE 0x1B01
			#define GL_LINEAR 0x2601
			#define GL_LINEAR_MIPMAP_LINEAR 0x2703
			#define GL_LINEAR_MIPMAP_NEAREST 0x2701
			#define GL_LINES 0x0001
			#define GL_LINES_ADJACENCY 0x000A
			#define GL_LINE_LOOP 0x0002
			#define GL_LINE_SMOOTH 0x0B20
			#define GL_LINE_SMOOTH_HINT 0x0C52
			#define GL_LINE_STRIP 0x0003
			#define GL_LINE_STRIP_ADJACENCY 0x000B
			#define GL_LINE_WIDTH 0x0B21
			#define GL_LINE_WIDTH_GRANULARITY 0x0B23
			#define GL_LINE_WIDTH_RANGE 0x0B22
			#define GL_LINK_STATUS 0x8B82
			#define GL_LOGIC_OP_MODE 0x0BF0
			#define GL_LOWER_LEFT 0x8CA1
			#define GL_MAJOR_VERSION 0x821B
			#define GL_MAP_FLUSH_EXPLICIT_BIT 0x0010
			#define GL_MAP_INVALIDATE_BUFFER_BIT 0x0008
			#define GL_MAP_INVALIDATE_RANGE_BIT 0x0004
			#define GL_MAP_READ_BIT 0x0001
			#define GL_MAP_UNSYNCHRONIZED_BIT 0x0020
			#define GL_MAP_WRITE_BIT 0x0002
			#define GL_MAX 0x8008
			#define GL_MAX_3D_TEXTURE_SIZE 0x8073
			#define GL_MAX_ARRAY_TEXTURE_LAYERS 0x88FF
			#define GL_MAX_CLIP_DISTANCES 0x0D32
			#define GL_MAX_COLOR_ATTACHMENTS 0x8CDF
			#define GL_MAX_COLOR_TEXTURE_SAMPLES 0x910E
			#define GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS 0x8A33
			#define GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS 0x8A32
			#define GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS 0x8B4D
			#define GL_MAX_COMBINED_UNIFORM_BLOCKS 0x8A2E
			#define GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS 0x8A31
			#define GL_MAX_CUBE_MAP_TEXTURE_SIZE 0x851C
			#define GL_MAX_DEPTH_TEXTURE_SAMPLES 0x910F
			#define GL_MAX_DRAW_BUFFERS 0x8824
			#define GL_MAX_DUAL_SOURCE_DRAW_BUFFERS 0x88FC
			#define GL_MAX_ELEMENTS_INDICES 0x80E9
			#define GL_MAX_ELEMENTS_VERTICES 0x80E8
			#define GL_MAX_FRAGMENT_INPUT_COMPONENTS 0x9125
			#define GL_MAX_FRAGMENT_UNIFORM_BLOCKS 0x8A2D
			#define GL_MAX_FRAGMENT_UNIFORM_COMPONENTS 0x8B49
			#define GL_MAX_GEOMETRY_INPUT_COMPONENTS 0x9123
			#define GL_MAX_GEOMETRY_OUTPUT_COMPONENTS 0x9124
			#define GL_MAX_GEOMETRY_OUTPUT_VERTICES 0x8DE0
			#define GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS 0x8C29
			#define GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS 0x8DE1
			#define GL_MAX_GEOMETRY_UNIFORM_BLOCKS 0x8A2C
			#define GL_MAX_GEOMETRY_UNIFORM_COMPONENTS 0x8DDF
			#define GL_MAX_INTEGER_SAMPLES 0x9110
			#define GL_MAX_PROGRAM_TEXEL_OFFSET 0x8905
			#define GL_MAX_RECTANGLE_TEXTURE_SIZE 0x84F8
			#define GL_MAX_RENDERBUFFER_SIZE 0x84E8
			#define GL_MAX_SAMPLES 0x8D57
			#define GL_MAX_SAMPLE_MASK_WORDS 0x8E59
			#define GL_MAX_SERVER_WAIT_TIMEOUT 0x9111
			#define GL_MAX_TEXTURE_BUFFER_SIZE 0x8C2B
			#define GL_MAX_TEXTURE_IMAGE_UNITS 0x8872
			#define GL_MAX_TEXTURE_LOD_BIAS 0x84FD
			#define GL_MAX_TEXTURE_SIZE 0x0D33
			#define GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS 0x8C8A
			#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS 0x8C8B
			#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS 0x8C80
			#define GL_MAX_UNIFORM_BLOCK_SIZE 0x8A30
			#define GL_MAX_UNIFORM_BUFFER_BINDINGS 0x8A2F
			#define GL_MAX_VARYING_COMPONENTS 0x8B4B
			#define GL_MAX_VARYING_FLOATS 0x8B4B
			#define GL_MAX_VERTEX_ATTRIBS 0x8869
			#define GL_MAX_VERTEX_OUTPUT_COMPONENTS 0x9122
			#define GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS 0x8B4C
			#define GL_MAX_VERTEX_UNIFORM_BLOCKS 0x8A2B
			#define GL_MAX_VERTEX_UNIFORM_COMPONENTS 0x8B4A
			#define GL_MAX_VIEWPORT_DIMS 0x0D3A
			#define GL_MIN 0x8007
			#define GL_MINOR_VERSION 0x821C
			#define GL_MIN_PROGRAM_TEXEL_OFFSET 0x8904
			#define GL_MIRRORED_REPEAT 0x8370
			#define GL_MULTISAMPLE 0x809D
			#define GL_NAND 0x150E
			#define GL_NEAREST 0x2600
			#define GL_NEAREST_MIPMAP_LINEAR 0x2702
			#define GL_NEAREST_MIPMAP_NEAREST 0x2700
			#define GL_NEVER 0x0200
			#define GL_NICEST 0x1102
			#define GL_NONE 0
			#define GL_NOOP 0x1505
			#define GL_NOR 0x1508
			#define GL_NOTEQUAL 0x0205
			#define GL_NO_ERROR 0
			#define GL_NUM_COMPRESSED_TEXTURE_FORMATS 0x86A2
			#define GL_NUM_EXTENSIONS 0x821D
			#define GL_OBJECT_TYPE 0x9112
			#define GL_ONE 1
			#define GL_ONE_MINUS_CONSTANT_ALPHA 0x8004
			#define GL_ONE_MINUS_CONSTANT_COLOR 0x8002
			#define GL_ONE_MINUS_DST_ALPHA 0x0305
			#define GL_ONE_MINUS_DST_COLOR 0x0307
			#define GL_ONE_MINUS_SRC1_ALPHA 0x88FB
			#define GL_ONE_MINUS_SRC1_COLOR 0x88FA
			#define GL_ONE_MINUS_SRC_ALPHA 0x0303
			#define GL_ONE_MINUS_SRC_COLOR 0x0301
			#define GL_OR 0x1507
			#define GL_OR_INVERTED 0x150D
			#define GL_OR_REVERSE 0x150B
			#define GL_OUT_OF_MEMORY 0x0505
			#define GL_PACK_ALIGNMENT 0x0D05
			#define GL_PACK_IMAGE_HEIGHT 0x806C
			#define GL_PACK_LSB_FIRST 0x0D01
			#define GL_PACK_ROW_LENGTH 0x0D02
			#define GL_PACK_SKIP_IMAGES 0x806B
			#define GL_PACK_SKIP_PIXELS 0x0D04
			#define GL_PACK_SKIP_ROWS 0x0D03
			#define GL_PACK_SWAP_BYTES 0x0D00
			#define GL_PIXEL_PACK_BUFFER 0x88EB
			#define GL_PIXEL_PACK_BUFFER_BINDING 0x88ED
			#define GL_PIXEL_UNPACK_BUFFER 0x88EC
			#define GL_PIXEL_UNPACK_BUFFER_BINDING 0x88EF
			#define GL_POINT 0x1B00
			#define GL_POINTS 0x0000
			#define GL_POINT_FADE_THRESHOLD_SIZE 0x8128
			#define GL_POINT_SIZE 0x0B11
			#define GL_POINT_SIZE_GRANULARITY 0x0B13
			#define GL_POINT_SIZE_RANGE 0x0B12
			#define GL_POINT_SPRITE_COORD_ORIGIN 0x8CA0
			#define GL_POLYGON_MODE 0x0B40
			#define GL_POLYGON_OFFSET_FACTOR 0x8038
			#define GL_POLYGON_OFFSET_FILL 0x8037
			#define GL_POLYGON_OFFSET_LINE 0x2A02
			#define GL_POLYGON_OFFSET_POINT 0x2A01
			#define GL_POLYGON_OFFSET_UNITS 0x2A00
			#define GL_POLYGON_SMOOTH 0x0B41
			#define GL_POLYGON_SMOOTH_HINT 0x0C53
			#define GL_PRIMITIVES_GENERATED 0x8C87
			#define GL_PRIMITIVE_RESTART 0x8F9D
			#define GL_PRIMITIVE_RESTART_INDEX 0x8F9E
			#define GL_PROGRAM_POINT_SIZE 0x8642
			#define GL_PROVOKING_VERTEX 0x8E4F
			#define GL_PROXY_TEXTURE_1D 0x8063
			#define GL_PROXY_TEXTURE_1D_ARRAY 0x8C19
			#define GL_PROXY_TEXTURE_2D 0x8064
			#define GL_PROXY_TEXTURE_2D_ARRAY 0x8C1B
			#define GL_PROXY_TEXTURE_2D_MULTISAMPLE 0x9101
			#define GL_PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY 0x9103
			#define GL_PROXY_TEXTURE_3D 0x8070
			#define GL_PROXY_TEXTURE_CUBE_MAP 0x851B
			#define GL_PROXY_TEXTURE_RECTANGLE 0x84F7
			#define GL_QUADS_FOLLOW_PROVOKING_VERTEX_CONVENTION 0x8E4C
			#define GL_QUERY_BY_REGION_NO_WAIT 0x8E16
			#define GL_QUERY_BY_REGION_WAIT 0x8E15
			#define GL_QUERY_COUNTER_BITS 0x8864
			#define GL_QUERY_NO_WAIT 0x8E14
			#define GL_QUERY_RESULT 0x8866
			#define GL_QUERY_RESULT_AVAILABLE 0x8867
			#define GL_QUERY_WAIT 0x8E13
			#define GL_R11F_G11F_B10F 0x8C3A
			#define GL_R16 0x822A
			#define GL_R16F 0x822D
			#define GL_R16I 0x8233
			#define GL_R16UI 0x8234
			#define GL_R16_SNORM 0x8F98
			#define GL_R32F 0x822E
			#define GL_R32I 0x8235
			#define GL_R32UI 0x8236
			#define GL_R3_G3_B2 0x2A10
			#define GL_R8 0x8229
			#define GL_R8I 0x8231
			#define GL_R8UI 0x8232
			#define GL_R8_SNORM 0x8F94
			#define GL_RASTERIZER_DISCARD 0x8C89
			#define GL_READ_BUFFER 0x0C02
			#define GL_READ_FRAMEBUFFER 0x8CA8
			#define GL_READ_FRAMEBUFFER_BINDING 0x8CAA
			#define GL_READ_ONLY 0x88B8
			#define GL_READ_WRITE 0x88BA
			#define GL_RED 0x1903
			#define GL_RED_INTEGER 0x8D94
			#define GL_RENDERBUFFER 0x8D41
			#define GL_RENDERBUFFER_ALPHA_SIZE 0x8D53
			#define GL_RENDERBUFFER_BINDING 0x8CA7
			#define GL_RENDERBUFFER_BLUE_SIZE 0x8D52
			#define GL_RENDERBUFFER_DEPTH_SIZE 0x8D54
			#define GL_RENDERBUFFER_GREEN_SIZE 0x8D51
			#define GL_RENDERBUFFER_HEIGHT 0x8D43
			#define GL_RENDERBUFFER_INTERNAL_FORMAT 0x8D44
			#define GL_RENDERBUFFER_RED_SIZE 0x8D50
			#define GL_RENDERBUFFER_SAMPLES 0x8CAB
			#define GL_RENDERBUFFER_STENCIL_SIZE 0x8D55
			#define GL_RENDERBUFFER_WIDTH 0x8D42
			#define GL_RENDERER 0x1F01
			#define GL_REPEAT 0x2901
			#define GL_REPLACE 0x1E01
			#define GL_RG 0x8227
			#define GL_RG16 0x822C
			#define GL_RG16F 0x822F
			#define GL_RG16I 0x8239
			#define GL_RG16UI 0x823A
			#define GL_RG16_SNORM 0x8F99
			#define GL_RG32F 0x8230
			#define GL_RG32I 0x823B
			#define GL_RG32UI 0x823C
			#define GL_RG8 0x822B
			#define GL_RG8I 0x8237
			#define GL_RG8UI 0x8238
			#define GL_RG8_SNORM 0x8F95
			#define GL_RGB 0x1907
			#define GL_RGB10 0x8052
			#define GL_RGB10_A2 0x8059
			#define GL_RGB10_A2UI 0x906F
			#define GL_RGB12 0x8053
			#define GL_RGB16 0x8054
			#define GL_RGB16F 0x881B
			#define GL_RGB16I 0x8D89
			#define GL_RGB16UI 0x8D77
			#define GL_RGB16_SNORM 0x8F9A
			#define GL_RGB32F 0x8815
			#define GL_RGB32I 0x8D83
			#define GL_RGB32UI 0x8D71
			#define GL_RGB4 0x804F
			#define GL_RGB5 0x8050
			#define GL_RGB5_A1 0x8057
			#define GL_RGB8 0x8051
			#define GL_RGB8I 0x8D8F
			#define GL_RGB8UI 0x8D7D
			#define GL_RGB8_SNORM 0x8F96
			#define GL_RGB9_E5 0x8C3D
			#define GL_RGBA 0x1908
			#define GL_RGBA12 0x805A
			#define GL_RGBA16 0x805B
			#define GL_RGBA16F 0x881A
			#define GL_RGBA16I 0x8D88
			#define GL_RGBA16UI 0x8D76
			#define GL_RGBA16_SNORM 0x8F9B
			#define GL_RGBA2 0x8055
			#define GL_RGBA32F 0x8814
			#define GL_RGBA32I 0x8D82
			#define GL_RGBA32UI 0x8D70
			#define GL_RGBA4 0x8056
			#define GL_RGBA8 0x8058
			#define GL_RGBA8I 0x8D8E
			#define GL_RGBA8UI 0x8D7C
			#define GL_RGBA8_SNORM 0x8F97
			#define GL_RGBA_INTEGER 0x8D99
			#define GL_RGB_INTEGER 0x8D98
			#define GL_RG_INTEGER 0x8228
			#define GL_RIGHT 0x0407
			#define GL_SAMPLER_1D 0x8B5D
			#define GL_SAMPLER_1D_ARRAY 0x8DC0
			#define GL_SAMPLER_1D_ARRAY_SHADOW 0x8DC3
			#define GL_SAMPLER_1D_SHADOW 0x8B61
			#define GL_SAMPLER_2D 0x8B5E
			#define GL_SAMPLER_2D_ARRAY 0x8DC1
			#define GL_SAMPLER_2D_ARRAY_SHADOW 0x8DC4
			#define GL_SAMPLER_2D_MULTISAMPLE 0x9108
			#define GL_SAMPLER_2D_MULTISAMPLE_ARRAY 0x910B
			#define GL_SAMPLER_2D_RECT 0x8B63
			#define GL_SAMPLER_2D_RECT_SHADOW 0x8B64
			#define GL_SAMPLER_2D_SHADOW 0x8B62
			#define GL_SAMPLER_3D 0x8B5F
			#define GL_SAMPLER_BINDING 0x8919
			#define GL_SAMPLER_BUFFER 0x8DC2
			#define GL_SAMPLER_CUBE 0x8B60
			#define GL_SAMPLER_CUBE_SHADOW 0x8DC5
			#define GL_SAMPLES 0x80A9
			#define GL_SAMPLES_PASSED 0x8914
			#define GL_SAMPLE_ALPHA_TO_COVERAGE 0x809E
			#define GL_SAMPLE_ALPHA_TO_ONE 0x809F
			#define GL_SAMPLE_BUFFERS 0x80A8
			#define GL_SAMPLE_COVERAGE 0x80A0
			#define GL_SAMPLE_COVERAGE_INVERT 0x80AB
			#define GL_SAMPLE_COVERAGE_VALUE 0x80AA
			#define GL_SAMPLE_MASK 0x8E51
			#define GL_SAMPLE_MASK_VALUE 0x8E52
			#define GL_SAMPLE_POSITION 0x8E50
			#define GL_SCISSOR_BOX 0x0C10
			#define GL_SCISSOR_TEST 0x0C11
			#define GL_SEPARATE_ATTRIBS 0x8C8D
			#define GL_SET 0x150F
			#define GL_SHADER_SOURCE_LENGTH 0x8B88
			#define GL_SHADER_TYPE 0x8B4F
			#define GL_SHADING_LANGUAGE_VERSION 0x8B8C
			#define GL_SHORT 0x1402
			#define GL_SIGNALED 0x9119
			#define GL_SIGNED_NORMALIZED 0x8F9C
			#define GL_SMOOTH_LINE_WIDTH_GRANULARITY 0x0B23
			#define GL_SMOOTH_LINE_WIDTH_RANGE 0x0B22
			#define GL_SMOOTH_POINT_SIZE_GRANULARITY 0x0B13
			#define GL_SMOOTH_POINT_SIZE_RANGE 0x0B12
			#define GL_SRC1_ALPHA 0x8589
			#define GL_SRC1_COLOR 0x88F9
			#define GL_SRC_ALPHA 0x0302
			#define GL_SRC_ALPHA_SATURATE 0x0308
			#define GL_SRC_COLOR 0x0300
			#define GL_SRGB 0x8C40
			#define GL_SRGB8 0x8C41
			#define GL_SRGB8_ALPHA8 0x8C43
			#define GL_SRGB_ALPHA 0x8C42
			#define GL_STATIC_COPY 0x88E6
			#define GL_STATIC_DRAW 0x88E4
			#define GL_STATIC_READ 0x88E5
			#define GL_STENCIL 0x1802
			#define GL_STENCIL_ATTACHMENT 0x8D20
			#define GL_STENCIL_BACK_FAIL 0x8801
			#define GL_STENCIL_BACK_FUNC 0x8800
			#define GL_STENCIL_BACK_PASS_DEPTH_FAIL 0x8802
			#define GL_STENCIL_BACK_PASS_DEPTH_PASS 0x8803
			#define GL_STENCIL_BACK_REF 0x8CA3
			#define GL_STENCIL_BACK_VALUE_MASK 0x8CA4
			#define GL_STENCIL_BACK_WRITEMASK 0x8CA5
			#define GL_STENCIL_BUFFER_BIT 0x00000400
			#define GL_STENCIL_CLEAR_VALUE 0x0B91
			#define GL_STENCIL_FAIL 0x0B94
			#define GL_STENCIL_FUNC 0x0B92
			#define GL_STENCIL_INDEX 0x1901
			#define GL_STENCIL_INDEX1 0x8D46
			#define GL_STENCIL_INDEX16 0x8D49
			#define GL_STENCIL_INDEX4 0x8D47
			#define GL_STENCIL_INDEX8 0x8D48
			#define GL_STENCIL_PASS_DEPTH_FAIL 0x0B95
			#define GL_STENCIL_PASS_DEPTH_PASS 0x0B96
			#define GL_STENCIL_REF 0x0B97
			#define GL_STENCIL_TEST 0x0B90
			#define GL_STENCIL_VALUE_MASK 0x0B93
			#define GL_STENCIL_WRITEMASK 0x0B98
			#define GL_STEREO 0x0C33
			#define GL_STREAM_COPY 0x88E2
			#define GL_STREAM_DRAW 0x88E0
			#define GL_STREAM_READ 0x88E1
			#define GL_SUBPIXEL_BITS 0x0D50
			#define GL_SYNC_CONDITION 0x9113
			#define GL_SYNC_FENCE 0x9116
			#define GL_SYNC_FLAGS 0x9115
			#define GL_SYNC_FLUSH_COMMANDS_BIT 0x00000001
			#define GL_SYNC_GPU_COMMANDS_COMPLETE 0x9117
			#define GL_SYNC_STATUS 0x9114
			#define GL_TEXTURE 0x1702
			#define GL_TEXTURE0 0x84C0
			#define GL_TEXTURE1 0x84C1
			#define GL_TEXTURE10 0x84CA
			#define GL_TEXTURE11 0x84CB
			#define GL_TEXTURE12 0x84CC
			#define GL_TEXTURE13 0x84CD
			#define GL_TEXTURE14 0x84CE
			#define GL_TEXTURE15 0x84CF
			#define GL_TEXTURE16 0x84D0
			#define GL_TEXTURE17 0x84D1
			#define GL_TEXTURE18 0x84D2
			#define GL_TEXTURE19 0x84D3
			#define GL_TEXTURE2 0x84C2
			#define GL_TEXTURE20 0x84D4
			#define GL_TEXTURE21 0x84D5
			#define GL_TEXTURE22 0x84D6
			#define GL_TEXTURE23 0x84D7
			#define GL_TEXTURE24 0x84D8
			#define GL_TEXTURE25 0x84D9
			#define GL_TEXTURE26 0x84DA
			#define GL_TEXTURE27 0x84DB
			#define GL_TEXTURE28 0x84DC
			#define GL_TEXTURE29 0x84DD
			#define GL_TEXTURE3 0x84C3
			#define GL_TEXTURE30 0x84DE
			#define GL_TEXTURE31 0x84DF
			#define GL_TEXTURE4 0x84C4
			#define GL_TEXTURE5 0x84C5
			#define GL_TEXTURE6 0x84C6
			#define GL_TEXTURE7 0x84C7
			#define GL_TEXTURE8 0x84C8
			#define GL_TEXTURE9 0x84C9
			#define GL_TEXTURE_1D 0x0DE0
			#define GL_TEXTURE_1D_ARRAY 0x8C18
			#define GL_TEXTURE_2D 0x0DE1
			#define GL_TEXTURE_2D_ARRAY 0x8C1A
			#define GL_TEXTURE_2D_MULTISAMPLE 0x9100
			#define GL_TEXTURE_2D_MULTISAMPLE_ARRAY 0x9102
			#define GL_TEXTURE_3D 0x806F
			#define GL_TEXTURE_ALPHA_SIZE 0x805F
			#define GL_TEXTURE_ALPHA_TYPE 0x8C13
			#define GL_TEXTURE_BASE_LEVEL 0x813C
			#define GL_TEXTURE_BINDING_1D 0x8068
			#define GL_TEXTURE_BINDING_1D_ARRAY 0x8C1C
			#define GL_TEXTURE_BINDING_2D 0x8069
			#define GL_TEXTURE_BINDING_2D_ARRAY 0x8C1D
			#define GL_TEXTURE_BINDING_2D_MULTISAMPLE 0x9104
			#define GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY 0x9105
			#define GL_TEXTURE_BINDING_3D 0x806A
			#define GL_TEXTURE_BINDING_BUFFER 0x8C2C
			#define GL_TEXTURE_BINDING_CUBE_MAP 0x8514
			#define GL_TEXTURE_BINDING_RECTANGLE 0x84F6
			#define GL_TEXTURE_BLUE_SIZE 0x805E
			#define GL_TEXTURE_BLUE_TYPE 0x8C12
			#define GL_TEXTURE_BORDER_COLOR 0x1004
			#define GL_TEXTURE_BUFFER 0x8C2A
			#define GL_TEXTURE_BUFFER_DATA_STORE_BINDING 0x8C2D
			#define GL_TEXTURE_COMPARE_FUNC 0x884D
			#define GL_TEXTURE_COMPARE_MODE 0x884C
			#define GL_TEXTURE_COMPRESSED 0x86A1
			#define GL_TEXTURE_COMPRESSED_IMAGE_SIZE 0x86A0
			#define GL_TEXTURE_COMPRESSION_HINT 0x84EF
			#define GL_TEXTURE_CUBE_MAP 0x8513
			#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X 0x8516
			#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y 0x8518
			#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z 0x851A
			#define GL_TEXTURE_CUBE_MAP_POSITIVE_X 0x8515
			#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y 0x8517
			#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z 0x8519
			#define GL_TEXTURE_CUBE_MAP_SEAMLESS 0x884F
			#define GL_TEXTURE_DEPTH 0x8071
			#define GL_TEXTURE_DEPTH_SIZE 0x884A
			#define GL_TEXTURE_DEPTH_TYPE 0x8C16
			#define GL_TEXTURE_FIXED_SAMPLE_LOCATIONS 0x9107
			#define GL_TEXTURE_GREEN_SIZE 0x805D
			#define GL_TEXTURE_GREEN_TYPE 0x8C11
			#define GL_TEXTURE_HEIGHT 0x1001
			#define GL_TEXTURE_INTERNAL_FORMAT 0x1003
			#define GL_TEXTURE_LOD_BIAS 0x8501
			#define GL_TEXTURE_MAG_FILTER 0x2800
			#define GL_TEXTURE_MAX_LEVEL 0x813D
			#define GL_TEXTURE_MAX_LOD 0x813B
			#define GL_TEXTURE_MIN_FILTER 0x2801
			#define GL_TEXTURE_MIN_LOD 0x813A
			#define GL_TEXTURE_RECTANGLE 0x84F5
			#define GL_TEXTURE_RED_SIZE 0x805C
			#define GL_TEXTURE_RED_TYPE 0x8C10
			#define GL_TEXTURE_SAMPLES 0x9106
			#define GL_TEXTURE_SHARED_SIZE 0x8C3F
			#define GL_TEXTURE_STENCIL_SIZE 0x88F1
			#define GL_TEXTURE_SWIZZLE_A 0x8E45
			#define GL_TEXTURE_SWIZZLE_B 0x8E44
			#define GL_TEXTURE_SWIZZLE_G 0x8E43
			#define GL_TEXTURE_SWIZZLE_R 0x8E42
			#define GL_TEXTURE_SWIZZLE_RGBA 0x8E46
			#define GL_TEXTURE_WIDTH 0x1000
			#define GL_TEXTURE_WRAP_R 0x8072
			#define GL_TEXTURE_WRAP_S 0x2802
			#define GL_TEXTURE_WRAP_T 0x2803
			#define GL_TIMEOUT_EXPIRED 0x911B
			#define GL_TIMEOUT_IGNORED 0xFFFFFFFFFFFFFFFF
			#define GL_TIMESTAMP 0x8E28
			#define GL_TIME_ELAPSED 0x88BF
			#define GL_TRANSFORM_FEEDBACK_BUFFER 0x8C8E
			#define GL_TRANSFORM_FEEDBACK_BUFFER_BINDING 0x8C8F
			#define GL_TRANSFORM_FEEDBACK_BUFFER_MODE 0x8C7F
			#define GL_TRANSFORM_FEEDBACK_BUFFER_SIZE 0x8C85
			#define GL_TRANSFORM_FEEDBACK_BUFFER_START 0x8C84
			#define GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN 0x8C88
			#define GL_TRANSFORM_FEEDBACK_VARYINGS 0x8C83
			#define GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH 0x8C76
			#define GL_TRIANGLES 0x0004
			#define GL_TRIANGLES_ADJACENCY 0x000C
			#define GL_TRIANGLE_FAN 0x0006
			#define GL_TRIANGLE_STRIP 0x0005
			#define GL_TRIANGLE_STRIP_ADJACENCY 0x000D
			#define GL_TRUE 1
			#define GL_UNIFORM_ARRAY_STRIDE 0x8A3C
			#define GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS 0x8A42
			#define GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES 0x8A43
			#define GL_UNIFORM_BLOCK_BINDING 0x8A3F
			#define GL_UNIFORM_BLOCK_DATA_SIZE 0x8A40
			#define GL_UNIFORM_BLOCK_INDEX 0x8A3A
			#define GL_UNIFORM_BLOCK_NAME_LENGTH 0x8A41
			#define GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER 0x8A46
			#define GL_UNIFORM_BLOCK_REFERENCED_BY_GEOMETRY_SHADER 0x8A45
			#define GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER 0x8A44
			#define GL_UNIFORM_BUFFER 0x8A11
			#define GL_UNIFORM_BUFFER_BINDING 0x8A28
			#define GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT 0x8A34
			#define GL_UNIFORM_BUFFER_SIZE 0x8A2A
			#define GL_UNIFORM_BUFFER_START 0x8A29
			#define GL_UNIFORM_IS_ROW_MAJOR 0x8A3E
			#define GL_UNIFORM_MATRIX_STRIDE 0x8A3D
			#define GL_UNIFORM_NAME_LENGTH 0x8A39
			#define GL_UNIFORM_OFFSET 0x8A3B
			#define GL_UNIFORM_SIZE 0x8A38
			#define GL_UNIFORM_TYPE 0x8A37
			#define GL_UNPACK_ALIGNMENT 0x0CF5
			#define GL_UNPACK_IMAGE_HEIGHT 0x806E
			#define GL_UNPACK_LSB_FIRST 0x0CF1
			#define GL_UNPACK_ROW_LENGTH 0x0CF2
			#define GL_UNPACK_SKIP_IMAGES 0x806D
			#define GL_UNPACK_SKIP_PIXELS 0x0CF4
			#define GL_UNPACK_SKIP_ROWS 0x0CF3
			#define GL_UNPACK_SWAP_BYTES 0x0CF0
			#define GL_UNSIGNALED 0x9118
			#define GL_UNSIGNED_BYTE 0x1401
			#define GL_UNSIGNED_BYTE_2_3_3_REV 0x8362
			#define GL_UNSIGNED_BYTE_3_3_2 0x8032
			#define GL_UNSIGNED_INT 0x1405
			#define GL_UNSIGNED_INT_10F_11F_11F_REV 0x8C3B
			#define GL_UNSIGNED_INT_10_10_10_2 0x8036
			#define GL_UNSIGNED_INT_24_8 0x84FA
			#define GL_UNSIGNED_INT_2_10_10_10_REV 0x8368
			#define GL_UNSIGNED_INT_5_9_9_9_REV 0x8C3E
			#define GL_UNSIGNED_INT_8_8_8_8 0x8035
			#define GL_UNSIGNED_INT_8_8_8_8_REV 0x8367
			#define GL_UNSIGNED_INT_SAMPLER_1D 0x8DD1
			#define GL_UNSIGNED_INT_SAMPLER_1D_ARRAY 0x8DD6
			#define GL_UNSIGNED_INT_SAMPLER_2D 0x8DD2
			#define GL_UNSIGNED_INT_SAMPLER_2D_ARRAY 0x8DD7
			#define GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE 0x910A
			#define GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY 0x910D
			#define GL_UNSIGNED_INT_SAMPLER_2D_RECT 0x8DD5
			#define GL_UNSIGNED_INT_SAMPLER_3D 0x8DD3
			#define GL_UNSIGNED_INT_SAMPLER_BUFFER 0x8DD8
			#define GL_UNSIGNED_INT_SAMPLER_CUBE 0x8DD4
			#define GL_UNSIGNED_INT_VEC2 0x8DC6
			#define GL_UNSIGNED_INT_VEC3 0x8DC7
			#define GL_UNSIGNED_INT_VEC4 0x8DC8
			#define GL_UNSIGNED_NORMALIZED 0x8C17
			#define GL_UNSIGNED_SHORT 0x1403
			#define GL_UNSIGNED_SHORT_1_5_5_5_REV 0x8366
			#define GL_UNSIGNED_SHORT_4_4_4_4 0x8033
			#define GL_UNSIGNED_SHORT_4_4_4_4_REV 0x8365
			#define GL_UNSIGNED_SHORT_5_5_5_1 0x8034
			#define GL_UNSIGNED_SHORT_5_6_5 0x8363
			#define GL_UNSIGNED_SHORT_5_6_5_REV 0x8364
			#define GL_UPPER_LEFT 0x8CA2
			#define GL_VALIDATE_STATUS 0x8B83
			#define GL_VENDOR 0x1F00
			#define GL_VERSION 0x1F02
			#define GL_VERTEX_ARRAY_BINDING 0x85B5
			#define GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING 0x889F
			#define GL_VERTEX_ATTRIB_ARRAY_DIVISOR 0x88FE
			#define GL_VERTEX_ATTRIB_ARRAY_ENABLED 0x8622
			#define GL_VERTEX_ATTRIB_ARRAY_INTEGER 0x88FD
			#define GL_VERTEX_ATTRIB_ARRAY_NORMALIZED 0x886A
			#define GL_VERTEX_ATTRIB_ARRAY_POINTER 0x8645
			#define GL_VERTEX_ATTRIB_ARRAY_SIZE 0x8623
			#define GL_VERTEX_ATTRIB_ARRAY_STRIDE 0x8624
			#define GL_VERTEX_ATTRIB_ARRAY_TYPE 0x8625
			#define GL_VERTEX_PROGRAM_POINT_SIZE 0x8642
			#define GL_VERTEX_SHADER 0x8B31
			#define GL_VIEWPORT 0x0BA2
			#define GL_WAIT_FAILED 0x911D
			#define GL_WRITE_ONLY 0x88B9
			#define GL_XOR 0x1506
			#define GL_ZERO 0


			#ifndef __khrplatform_h_
			#define __khrplatform_h_

			/*
			** Copyright (c) 2008-2018 The Khronos Group Inc.
			**
			** Permission is hereby granted, free of charge, to any person obtaining a
			** copy of this software and/or associated documentation files (the
			** "Materials"), to deal in the Materials without restriction, including
			** without limitation the rights to use, copy, modify, merge, publish,
			** distribute, sublicense, and/or sell copies of the Materials, and to
			** permit persons to whom the Materials are furnished to do so, subject to
			** the following conditions:
			**
			** The above copyright notice and this permission notice shall be included
			** in all copies or substantial portions of the Materials.
			**
			** THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
			** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
			** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
			** IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
			** CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
			** TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
			** MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.
			*/

			/* Khronos platform-specific types and definitions.
			 *
			 * The master copy of khrplatform.h is maintained in the Khronos EGL
			 * Registry repository at https://github.com/KhronosGroup/EGL-Registry
			 * The last semantic modification to khrplatform.h was at commit ID:
			 *      67a3e0864c2d75ea5287b9f3d2eb74a745936692
			 *
			 * Adopters may modify this file to suit their platform. Adopters are
			 * encouraged to submit platform specific modifications to the Khronos
			 * group so that they can be included in future versions of this file.
			 * Please submit changes by filing pull requests or issues on
			 * the EGL Registry repository linked above.
			 *
			 *
			 * See the Implementer's Guidelines for information about where this file
			 * should be located on your system and for more details of its use:
			 *    http://www.khronos.org/registry/implementers_guide.pdf
			 *
			 * This file should be included as
			 *        #include <KHR/khrplatform.h>
			 * by Khronos client API header files that use its types and defines.
			 *
			 * The types in khrplatform.h should only be used to define API-specific types.
			 *
			 * Types defined in khrplatform.h:
			 *    khronos_int8_t              signed   8  bit
			 *    khronos_uint8_t             unsigned 8  bit
			 *    khronos_int16_t             signed   16 bit
			 *    khronos_uint16_t            unsigned 16 bit
			 *    khronos_int32_t             signed   32 bit
			 *    khronos_uint32_t            unsigned 32 bit
			 *    khronos_int64_t             signed   64 bit
			 *    khronos_uint64_t            unsigned 64 bit
			 *    khronos_intptr_t            signed   same number of bits as a pointer
			 *    khronos_uintptr_t           unsigned same number of bits as a pointer
			 *    khronos_ssize_t             signed   size
			 *    khronos_usize_t             unsigned size
			 *    khronos_float_t             signed   32 bit floating point
			 *    khronos_time_ns_t           unsigned 64 bit time in nanoseconds
			 *    khronos_utime_nanoseconds_t unsigned time interval or absolute time in
			 *                                         nanoseconds
			 *    khronos_stime_nanoseconds_t signed time interval in nanoseconds
			 *    khronos_boolean_enum_t      enumerated boolean type. This should
			 *      only be used as a base type when a client API's boolean type is
			 *      an enum. Client APIs which use an integer or other type for
			 *      booleans cannot use this as the base type for their boolean.
			 *
			 * Tokens defined in khrplatform.h:
			 *
			 *    KHRONOS_FALSE, KHRONOS_TRUE Enumerated boolean false/true values.
			 *
			 *    KHRONOS_SUPPORT_INT64 is 1 if 64 bit integers are supported; otherwise 0.
			 *    KHRONOS_SUPPORT_FLOAT is 1 if floats are supported; otherwise 0.
			 *
			 * Calling convention macros defined in this file:
			 *    KHRONOS_APICALL
			 *    KHRONOS_GLAD_API_PTR
			 *    KHRONOS_APIATTRIBUTES
			 *
			 * These may be used in function prototypes as:
			 *
			 *      KHRONOS_APICALL void KHRONOS_GLAD_API_PTR funcname(
			 *                                  int arg1,
			 *                                  int arg2) KHRONOS_APIATTRIBUTES;
			 */

			#if defined(__SCITECH_SNAP__) && !defined(KHRONOS_STATIC)
			#   define KHRONOS_STATIC 1
			#endif

			/*-------------------------------------------------------------------------
			 * Definition of KHRONOS_APICALL
			 *-------------------------------------------------------------------------
			 * This precedes the return type of the function in the function prototype.
			 */
			#if defined(KHRONOS_STATIC)
			    /* If the preprocessor constant KHRONOS_STATIC is defined, make the
			     * header compatible with static linking. */
			#   define KHRONOS_APICALL
			#elif defined(_WIN32)
			#   define KHRONOS_APICALL __declspec(dllimport)
			#elif defined (__SYMBIAN32__)
			#   define KHRONOS_APICALL IMPORT_C
			#elif defined(__ANDROID__)
			#   define KHRONOS_APICALL __attribute__((visibility("default")))
			#else
			#   define KHRONOS_APICALL
			#endif

			/*-------------------------------------------------------------------------
			 * Definition of KHRONOS_GLAD_API_PTR
			 *-------------------------------------------------------------------------
			 * This follows the return type of the function  and precedes the function
			 * name in the function prototype.
			 */
			#if defined(_WIN32) && !defined(_WIN32_WCE) && !defined(__SCITECH_SNAP__)
			    /* Win32 but not WinCE */
			#   define KHRONOS_GLAD_API_PTR __stdcall
			#else
			#   define KHRONOS_GLAD_API_PTR
			#endif

			/*-------------------------------------------------------------------------
			 * Definition of KHRONOS_APIATTRIBUTES
			 *-------------------------------------------------------------------------
			 * This follows the closing parenthesis of the function prototype arguments.
			 */
			#if defined (__ARMCC_2__)
			#define KHRONOS_APIATTRIBUTES __softfp
			#else
			#define KHRONOS_APIATTRIBUTES
			#endif

			/*-------------------------------------------------------------------------
			 * basic type definitions
			 *-----------------------------------------------------------------------*/
			#if (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L) || defined(__GNUC__) || defined(__SCO__) || defined(__USLC__)


			/*
			 * Using <stdint.h>
			 */
			#include <stdint.h>
			typedef int32_t                 khronos_int32_t;
			typedef uint32_t                khronos_uint32_t;
			typedef int64_t                 khronos_int64_t;
			typedef uint64_t                khronos_uint64_t;
			#define KHRONOS_SUPPORT_INT64   1
			#define KHRONOS_SUPPORT_FLOAT   1
			/*
			 * To support platform where unsigned long cannot be used interchangeably with
			 * inptr_t (e.g. CHERI-extended ISAs), we can use the stdint.h intptr_t.
			 * Ideally, we could just use (u)intptr_t everywhere, but this could result in
			 * ABI breakage if khronos_uintptr_t is changed from unsigned long to
			 * unsigned long long or similar (this results in different C++ name mangling).
			 * To avoid changes for existing platforms, we restrict usage of intptr_t to
			 * platforms where the size of a pointer is larger than the size of long.
			 */
			#if defined(__SIZEOF_LONG__) && defined(__SIZEOF_POINTER__)
			#if __SIZEOF_POINTER__ > __SIZEOF_LONG__
			#define KHRONOS_USE_INTPTR_T
			#endif
			#endif

			#elif defined(__VMS ) || defined(__sgi)

			/*
			 * Using <inttypes.h>
			 */
			#include <inttypes.h>
			typedef int32_t                 khronos_int32_t;
			typedef uint32_t                khronos_uint32_t;
			typedef int64_t                 khronos_int64_t;
			typedef uint64_t                khronos_uint64_t;
			#define KHRONOS_SUPPORT_INT64   1
			#define KHRONOS_SUPPORT_FLOAT   1

			#elif defined(_WIN32) && !defined(__SCITECH_SNAP__)

			/*
			 * Win32
			 */
			typedef __int32                 khronos_int32_t;
			typedef unsigned __int32        khronos_uint32_t;
			typedef __int64                 khronos_int64_t;
			typedef unsigned __int64        khronos_uint64_t;
			#define KHRONOS_SUPPORT_INT64   1
			#define KHRONOS_SUPPORT_FLOAT   1

			#elif defined(__sun__) || defined(__digital__)

			/*
			 * Sun or Digital
			 */
			typedef int                     khronos_int32_t;
			typedef unsigned int            khronos_uint32_t;
			#if defined(__arch64__) || defined(_LP64)
			typedef long int                khronos_int64_t;
			typedef unsigned long int       khronos_uint64_t;
			#else
			typedef long long int           khronos_int64_t;
			typedef unsigned long long int  khronos_uint64_t;
			#endif /* __arch64__ */
			#define KHRONOS_SUPPORT_INT64   1
			#define KHRONOS_SUPPORT_FLOAT   1

			#elif 0

			/*
			 * Hypothetical platform with no float or int64 support
			 */
			typedef int                     khronos_int32_t;
			typedef unsigned int            khronos_uint32_t;
			#define KHRONOS_SUPPORT_INT64   0
			#define KHRONOS_SUPPORT_FLOAT   0

			#else

			/*
			 * Generic fallback
			 */
			#include <stdint.h>
			typedef int32_t                 khronos_int32_t;
			typedef uint32_t                khronos_uint32_t;
			typedef int64_t                 khronos_int64_t;
			typedef uint64_t                khronos_uint64_t;
			#define KHRONOS_SUPPORT_INT64   1
			#define KHRONOS_SUPPORT_FLOAT   1

			#endif


			/*
			 * Types that are (so far) the same on all platforms
			 */
			typedef signed   char          khronos_int8_t;
			typedef unsigned char          khronos_uint8_t;
			typedef signed   short int     khronos_int16_t;
			typedef unsigned short int     khronos_uint16_t;

			/*
			 * Types that differ between LLP64 and LP64 architectures - in LLP64,
			 * pointers are 64 bits, but 'long' is still 32 bits. Win64 appears
			 * to be the only LLP64 architecture in current use.
			 */
			#ifdef KHRONOS_USE_INTPTR_T
			typedef intptr_t               khronos_intptr_t;
			typedef uintptr_t              khronos_uintptr_t;
			#elif defined(_WIN64)
			typedef signed   long long int khronos_intptr_t;
			typedef unsigned long long int khronos_uintptr_t;
			#else
			typedef signed   long  int     khronos_intptr_t;
			typedef unsigned long  int     khronos_uintptr_t;
			#endif

			#if defined(_WIN64)
			typedef signed   long long int khronos_ssize_t;
			typedef unsigned long long int khronos_usize_t;
			#else
			typedef signed   long  int     khronos_ssize_t;
			typedef unsigned long  int     khronos_usize_t;
			#endif

			#if KHRONOS_SUPPORT_FLOAT
			/*
			 * Float type
			 */
			typedef          float         khronos_float_t;
			#endif

			#if KHRONOS_SUPPORT_INT64
			/* Time types
			 *
			 * These types can be used to represent a time interval in nanoseconds or
			 * an absolute Unadjusted System Time.  Unadjusted System Time is the number
			 * of nanoseconds since some arbitrary system event (e.g. since the last
			 * time the system booted).  The Unadjusted System Time is an unsigned
			 * 64 bit value that wraps back to 0 every 584 years.  Time intervals
			 * may be either signed or unsigned.
			 */
			typedef khronos_uint64_t       khronos_utime_nanoseconds_t;
			typedef khronos_int64_t        khronos_stime_nanoseconds_t;
			#endif

			/*
			 * Dummy value used to pad enum types to 32 bits.
			 */
			#ifndef KHRONOS_MAX_ENUM
			#define KHRONOS_MAX_ENUM 0x7FFFFFFF
			#endif

			/*
			 * Enumerated boolean type
			 *
			 * Values other than zero should be considered to be true.  Therefore
			 * comparisons should not be made against KHRONOS_TRUE.
			 */
			typedef enum {
			    KHRONOS_FALSE = 0,
			    KHRONOS_TRUE  = 1,
			    KHRONOS_BOOLEAN_ENUM_FORCE_SIZE = KHRONOS_MAX_ENUM
			} khronos_boolean_enum_t;

			#endif /* __khrplatform_h_ */
			typedef unsigned int GLenum;
			typedef unsigned char GLboolean;
			typedef unsigned int GLbitfield;
			typedef void GLvoid;
			typedef khronos_int8_t GLbyte;
			typedef khronos_uint8_t GLubyte;
			typedef khronos_int16_t GLshort;
			typedef khronos_uint16_t GLushort;
			typedef int GLint;
			typedef unsigned int GLuint;
			typedef khronos_int32_t GLclampx;
			typedef int GLsizei;
			typedef khronos_float_t GLfloat;
			typedef khronos_float_t GLclampf;
			typedef double GLdouble;
			typedef double GLclampd;
			typedef void *GLeglClientBufferEXT;
			typedef void *GLeglImageOES;
			typedef char GLchar;
			typedef char GLcharARB;
			#ifdef __APPLE__
			typedef void *GLhandleARB;
			#else
			typedef unsigned int GLhandleARB;
			#endif
			typedef khronos_uint16_t GLhalf;
			typedef khronos_uint16_t GLhalfARB;
			typedef khronos_int32_t GLfixed;
			#if defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__) && (__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ > 1060)
			typedef khronos_intptr_t GLintptr;
			#else
			typedef khronos_intptr_t GLintptr;
			#endif
			#if defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__) && (__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ > 1060)
			typedef khronos_intptr_t GLintptrARB;
			#else
			typedef khronos_intptr_t GLintptrARB;
			#endif
			#if defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__) && (__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ > 1060)
			typedef khronos_ssize_t GLsizeiptr;
			#else
			typedef khronos_ssize_t GLsizeiptr;
			#endif
			#if defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__) && (__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ > 1060)
			typedef khronos_ssize_t GLsizeiptrARB;
			#else
			typedef khronos_ssize_t GLsizeiptrARB;
			#endif
			typedef khronos_int64_t GLint64;
			typedef khronos_int64_t GLint64EXT;
			typedef khronos_uint64_t GLuint64;
			typedef khronos_uint64_t GLuint64EXT;
			typedef struct __GLsync *GLsync;
			struct _cl_context;
			struct _cl_event;
			typedef void (GLAD_API_PTR *GLDEBUGPROC)(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar *message,const void *userParam);
			typedef void (GLAD_API_PTR *GLDEBUGPROCARB)(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar *message,const void *userParam);
			typedef void (GLAD_API_PTR *GLDEBUGPROCKHR)(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar *message,const void *userParam);
			typedef void (GLAD_API_PTR *GLDEBUGPROCAMD)(GLuint id,GLenum category,GLenum severity,GLsizei length,const GLchar *message,void *userParam);
			typedef unsigned short GLhalfNV;
			typedef GLintptr GLvdpauSurfaceNV;
			typedef void (GLAD_API_PTR *GLVULKANPROCNV)(void);


			#define GL_VERSION_1_0 1
			GLAD_API_CALL int GLAD_GL_VERSION_1_0;
			#define GL_VERSION_1_1 1
			GLAD_API_CALL int GLAD_GL_VERSION_1_1;
			#define GL_VERSION_1_2 1
			GLAD_API_CALL int GLAD_GL_VERSION_1_2;
			#define GL_VERSION_1_3 1
			GLAD_API_CALL int GLAD_GL_VERSION_1_3;
			#define GL_VERSION_1_4 1
			GLAD_API_CALL int GLAD_GL_VERSION_1_4;
			#define GL_VERSION_1_5 1
			GLAD_API_CALL int GLAD_GL_VERSION_1_5;
			#define GL_VERSION_2_0 1
			GLAD_API_CALL int GLAD_GL_VERSION_2_0;
			#define GL_VERSION_2_1 1
			GLAD_API_CALL int GLAD_GL_VERSION_2_1;
			#define GL_VERSION_3_0 1
			GLAD_API_CALL int GLAD_GL_VERSION_3_0;
			#define GL_VERSION_3_1 1
			GLAD_API_CALL int GLAD_GL_VERSION_3_1;
			#define GL_VERSION_3_2 1
			GLAD_API_CALL int GLAD_GL_VERSION_3_2;
			#define GL_VERSION_3_3 1
			GLAD_API_CALL int GLAD_GL_VERSION_3_3;


			typedef void (GLAD_API_PTR *PFNGLACTIVETEXTUREPROC)(GLenum texture);
			typedef void (GLAD_API_PTR *PFNGLATTACHSHADERPROC)(GLuint program, GLuint shader);
			typedef void (GLAD_API_PTR *PFNGLBEGINCONDITIONALRENDERPROC)(GLuint id, GLenum mode);
			typedef void (GLAD_API_PTR *PFNGLBEGINQUERYPROC)(GLenum target, GLuint id);
			typedef void (GLAD_API_PTR *PFNGLBEGINTRANSFORMFEEDBACKPROC)(GLenum primitiveMode);
			typedef void (GLAD_API_PTR *PFNGLBINDATTRIBLOCATIONPROC)(GLuint program, GLuint index, const GLchar * name);
			typedef void (GLAD_API_PTR *PFNGLBINDBUFFERPROC)(GLenum target, GLuint buffer);
			typedef void (GLAD_API_PTR *PFNGLBINDBUFFERBASEPROC)(GLenum target, GLuint index, GLuint buffer);
			typedef void (GLAD_API_PTR *PFNGLBINDBUFFERRANGEPROC)(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
			typedef void (GLAD_API_PTR *PFNGLBINDFRAGDATALOCATIONPROC)(GLuint program, GLuint color, const GLchar * name);
			typedef void (GLAD_API_PTR *PFNGLBINDFRAGDATALOCATIONINDEXEDPROC)(GLuint program, GLuint colorNumber, GLuint index, const GLchar * name);
			typedef void (GLAD_API_PTR *PFNGLBINDFRAMEBUFFERPROC)(GLenum target, GLuint framebuffer);
			typedef void (GLAD_API_PTR *PFNGLBINDRENDERBUFFERPROC)(GLenum target, GLuint renderbuffer);
			typedef void (GLAD_API_PTR *PFNGLBINDSAMPLERPROC)(GLuint unit, GLuint sampler);
			typedef void (GLAD_API_PTR *PFNGLBINDTEXTUREPROC)(GLenum target, GLuint texture);
			typedef void (GLAD_API_PTR *PFNGLBINDVERTEXARRAYPROC)(GLuint array);
			typedef void (GLAD_API_PTR *PFNGLBLENDCOLORPROC)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
			typedef void (GLAD_API_PTR *PFNGLBLENDEQUATIONPROC)(GLenum mode);
			typedef void (GLAD_API_PTR *PFNGLBLENDEQUATIONSEPARATEPROC)(GLenum modeRGB, GLenum modeAlpha);
			typedef void (GLAD_API_PTR *PFNGLBLENDFUNCPROC)(GLenum sfactor, GLenum dfactor);
			typedef void (GLAD_API_PTR *PFNGLBLENDFUNCSEPARATEPROC)(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
			typedef void (GLAD_API_PTR *PFNGLBLITFRAMEBUFFERPROC)(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
			typedef void (GLAD_API_PTR *PFNGLBUFFERDATAPROC)(GLenum target, GLsizeiptr size, const void * data, GLenum usage);
			typedef void (GLAD_API_PTR *PFNGLBUFFERSUBDATAPROC)(GLenum target, GLintptr offset, GLsizeiptr size, const void * data);
			typedef GLenum (GLAD_API_PTR *PFNGLCHECKFRAMEBUFFERSTATUSPROC)(GLenum target);
			typedef void (GLAD_API_PTR *PFNGLCLAMPCOLORPROC)(GLenum target, GLenum clamp);
			typedef void (GLAD_API_PTR *PFNGLCLEARPROC)(GLbitfield mask);
			typedef void (GLAD_API_PTR *PFNGLCLEARBUFFERFIPROC)(GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil);
			typedef void (GLAD_API_PTR *PFNGLCLEARBUFFERFVPROC)(GLenum buffer, GLint drawbuffer, const GLfloat * value);
			typedef void (GLAD_API_PTR *PFNGLCLEARBUFFERIVPROC)(GLenum buffer, GLint drawbuffer, const GLint * value);
			typedef void (GLAD_API_PTR *PFNGLCLEARBUFFERUIVPROC)(GLenum buffer, GLint drawbuffer, const GLuint * value);
			typedef void (GLAD_API_PTR *PFNGLCLEARCOLORPROC)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
			typedef void (GLAD_API_PTR *PFNGLCLEARDEPTHPROC)(GLdouble depth);
			typedef void (GLAD_API_PTR *PFNGLCLEARSTENCILPROC)(GLint s);
			typedef GLenum (GLAD_API_PTR *PFNGLCLIENTWAITSYNCPROC)(GLsync sync, GLbitfield flags, GLuint64 timeout);
			typedef void (GLAD_API_PTR *PFNGLCOLORMASKPROC)(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
			typedef void (GLAD_API_PTR *PFNGLCOLORMASKIPROC)(GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a);
			typedef void (GLAD_API_PTR *PFNGLCOMPILESHADERPROC)(GLuint shader);
			typedef void (GLAD_API_PTR *PFNGLCOMPRESSEDTEXIMAGE1DPROC)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void * data);
			typedef void (GLAD_API_PTR *PFNGLCOMPRESSEDTEXIMAGE2DPROC)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void * data);
			typedef void (GLAD_API_PTR *PFNGLCOMPRESSEDTEXIMAGE3DPROC)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void * data);
			typedef void (GLAD_API_PTR *PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void * data);
			typedef void (GLAD_API_PTR *PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void * data);
			typedef void (GLAD_API_PTR *PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void * data);
			typedef void (GLAD_API_PTR *PFNGLCOPYBUFFERSUBDATAPROC)(GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
			typedef void (GLAD_API_PTR *PFNGLCOPYTEXIMAGE1DPROC)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
			typedef void (GLAD_API_PTR *PFNGLCOPYTEXIMAGE2DPROC)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
			typedef void (GLAD_API_PTR *PFNGLCOPYTEXSUBIMAGE1DPROC)(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
			typedef void (GLAD_API_PTR *PFNGLCOPYTEXSUBIMAGE2DPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
			typedef void (GLAD_API_PTR *PFNGLCOPYTEXSUBIMAGE3DPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
			typedef GLuint (GLAD_API_PTR *PFNGLCREATEPROGRAMPROC)(void);
			typedef GLuint (GLAD_API_PTR *PFNGLCREATESHADERPROC)(GLenum type);
			typedef void (GLAD_API_PTR *PFNGLCULLFACEPROC)(GLenum mode);
			typedef void (GLAD_API_PTR *PFNGLDELETEBUFFERSPROC)(GLsizei n, const GLuint * buffers);
			typedef void (GLAD_API_PTR *PFNGLDELETEFRAMEBUFFERSPROC)(GLsizei n, const GLuint * framebuffers);
			typedef void (GLAD_API_PTR *PFNGLDELETEPROGRAMPROC)(GLuint program);
			typedef void (GLAD_API_PTR *PFNGLDELETEQUERIESPROC)(GLsizei n, const GLuint * ids);
			typedef void (GLAD_API_PTR *PFNGLDELETERENDERBUFFERSPROC)(GLsizei n, const GLuint * renderbuffers);
			typedef void (GLAD_API_PTR *PFNGLDELETESAMPLERSPROC)(GLsizei count, const GLuint * samplers);
			typedef void (GLAD_API_PTR *PFNGLDELETESHADERPROC)(GLuint shader);
			typedef void (GLAD_API_PTR *PFNGLDELETESYNCPROC)(GLsync sync);
			typedef void (GLAD_API_PTR *PFNGLDELETETEXTURESPROC)(GLsizei n, const GLuint * textures);
			typedef void (GLAD_API_PTR *PFNGLDELETEVERTEXARRAYSPROC)(GLsizei n, const GLuint * arrays);
			typedef void (GLAD_API_PTR *PFNGLDEPTHFUNCPROC)(GLenum func);
			typedef void (GLAD_API_PTR *PFNGLDEPTHMASKPROC)(GLboolean flag);
			typedef void (GLAD_API_PTR *PFNGLDEPTHRANGEPROC)(GLdouble n, GLdouble f);
			typedef void (GLAD_API_PTR *PFNGLDETACHSHADERPROC)(GLuint program, GLuint shader);
			typedef void (GLAD_API_PTR *PFNGLDISABLEPROC)(GLenum cap);
			typedef void (GLAD_API_PTR *PFNGLDISABLEVERTEXATTRIBARRAYPROC)(GLuint index);
			typedef void (GLAD_API_PTR *PFNGLDISABLEIPROC)(GLenum target, GLuint index);
			typedef void (GLAD_API_PTR *PFNGLDRAWARRAYSPROC)(GLenum mode, GLint first, GLsizei count);
			typedef void (GLAD_API_PTR *PFNGLDRAWARRAYSINSTANCEDPROC)(GLenum mode, GLint first, GLsizei count, GLsizei instancecount);
			typedef void (GLAD_API_PTR *PFNGLDRAWBUFFERPROC)(GLenum buf);
			typedef void (GLAD_API_PTR *PFNGLDRAWBUFFERSPROC)(GLsizei n, const GLenum * bufs);
			typedef void (GLAD_API_PTR *PFNGLDRAWELEMENTSPROC)(GLenum mode, GLsizei count, GLenum type, const void * indices);
			typedef void (GLAD_API_PTR *PFNGLDRAWELEMENTSBASEVERTEXPROC)(GLenum mode, GLsizei count, GLenum type, const void * indices, GLint basevertex);
			typedef void (GLAD_API_PTR *PFNGLDRAWELEMENTSINSTANCEDPROC)(GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei instancecount);
			typedef void (GLAD_API_PTR *PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC)(GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei instancecount, GLint basevertex);
			typedef void (GLAD_API_PTR *PFNGLDRAWRANGEELEMENTSPROC)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void * indices);
			typedef void (GLAD_API_PTR *PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void * indices, GLint basevertex);
			typedef void (GLAD_API_PTR *PFNGLENABLEPROC)(GLenum cap);
			typedef void (GLAD_API_PTR *PFNGLENABLEVERTEXATTRIBARRAYPROC)(GLuint index);
			typedef void (GLAD_API_PTR *PFNGLENABLEIPROC)(GLenum target, GLuint index);
			typedef void (GLAD_API_PTR *PFNGLENDCONDITIONALRENDERPROC)(void);
			typedef void (GLAD_API_PTR *PFNGLENDQUERYPROC)(GLenum target);
			typedef void (GLAD_API_PTR *PFNGLENDTRANSFORMFEEDBACKPROC)(void);
			typedef GLsync (GLAD_API_PTR *PFNGLFENCESYNCPROC)(GLenum condition, GLbitfield flags);
			typedef void (GLAD_API_PTR *PFNGLFINISHPROC)(void);
			typedef void (GLAD_API_PTR *PFNGLFLUSHPROC)(void);
			typedef void (GLAD_API_PTR *PFNGLFLUSHMAPPEDBUFFERRANGEPROC)(GLenum target, GLintptr offset, GLsizeiptr length);
			typedef void (GLAD_API_PTR *PFNGLFRAMEBUFFERRENDERBUFFERPROC)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
			typedef void (GLAD_API_PTR *PFNGLFRAMEBUFFERTEXTUREPROC)(GLenum target, GLenum attachment, GLuint texture, GLint level);
			typedef void (GLAD_API_PTR *PFNGLFRAMEBUFFERTEXTURE1DPROC)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
			typedef void (GLAD_API_PTR *PFNGLFRAMEBUFFERTEXTURE2DPROC)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
			typedef void (GLAD_API_PTR *PFNGLFRAMEBUFFERTEXTURE3DPROC)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
			typedef void (GLAD_API_PTR *PFNGLFRAMEBUFFERTEXTURELAYERPROC)(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
			typedef void (GLAD_API_PTR *PFNGLFRONTFACEPROC)(GLenum mode);
			typedef void (GLAD_API_PTR *PFNGLGENBUFFERSPROC)(GLsizei n, GLuint * buffers);
			typedef void (GLAD_API_PTR *PFNGLGENFRAMEBUFFERSPROC)(GLsizei n, GLuint * framebuffers);
			typedef void (GLAD_API_PTR *PFNGLGENQUERIESPROC)(GLsizei n, GLuint * ids);
			typedef void (GLAD_API_PTR *PFNGLGENRENDERBUFFERSPROC)(GLsizei n, GLuint * renderbuffers);
			typedef void (GLAD_API_PTR *PFNGLGENSAMPLERSPROC)(GLsizei count, GLuint * samplers);
			typedef void (GLAD_API_PTR *PFNGLGENTEXTURESPROC)(GLsizei n, GLuint * textures);
			typedef void (GLAD_API_PTR *PFNGLGENVERTEXARRAYSPROC)(GLsizei n, GLuint * arrays);
			typedef void (GLAD_API_PTR *PFNGLGENERATEMIPMAPPROC)(GLenum target);
			typedef void (GLAD_API_PTR *PFNGLGETACTIVEATTRIBPROC)(GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name);
			typedef void (GLAD_API_PTR *PFNGLGETACTIVEUNIFORMPROC)(GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name);
			typedef void (GLAD_API_PTR *PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC)(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei * length, GLchar * uniformBlockName);
			typedef void (GLAD_API_PTR *PFNGLGETACTIVEUNIFORMBLOCKIVPROC)(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint * params);
			typedef void (GLAD_API_PTR *PFNGLGETACTIVEUNIFORMNAMEPROC)(GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei * length, GLchar * uniformName);
			typedef void (GLAD_API_PTR *PFNGLGETACTIVEUNIFORMSIVPROC)(GLuint program, GLsizei uniformCount, const GLuint * uniformIndices, GLenum pname, GLint * params);
			typedef void (GLAD_API_PTR *PFNGLGETATTACHEDSHADERSPROC)(GLuint program, GLsizei maxCount, GLsizei * count, GLuint * shaders);
			typedef GLint (GLAD_API_PTR *PFNGLGETATTRIBLOCATIONPROC)(GLuint program, const GLchar * name);
			typedef void (GLAD_API_PTR *PFNGLGETBOOLEANI_VPROC)(GLenum target, GLuint index, GLboolean * data);
			typedef void (GLAD_API_PTR *PFNGLGETBOOLEANVPROC)(GLenum pname, GLboolean * data);
			typedef void (GLAD_API_PTR *PFNGLGETBUFFERPARAMETERI64VPROC)(GLenum target, GLenum pname, GLint64 * params);
			typedef void (GLAD_API_PTR *PFNGLGETBUFFERPARAMETERIVPROC)(GLenum target, GLenum pname, GLint * params);
			typedef void (GLAD_API_PTR *PFNGLGETBUFFERPOINTERVPROC)(GLenum target, GLenum pname, void ** params);
			typedef void (GLAD_API_PTR *PFNGLGETBUFFERSUBDATAPROC)(GLenum target, GLintptr offset, GLsizeiptr size, void * data);
			typedef void (GLAD_API_PTR *PFNGLGETCOMPRESSEDTEXIMAGEPROC)(GLenum target, GLint level, void * img);
			typedef void (GLAD_API_PTR *PFNGLGETDOUBLEVPROC)(GLenum pname, GLdouble * data);
			typedef GLenum (GLAD_API_PTR *PFNGLGETERRORPROC)(void);
			typedef void (GLAD_API_PTR *PFNGLGETFLOATVPROC)(GLenum pname, GLfloat * data);
			typedef GLint (GLAD_API_PTR *PFNGLGETFRAGDATAINDEXPROC)(GLuint program, const GLchar * name);
			typedef GLint (GLAD_API_PTR *PFNGLGETFRAGDATALOCATIONPROC)(GLuint program, const GLchar * name);
			typedef void (GLAD_API_PTR *PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)(GLenum target, GLenum attachment, GLenum pname, GLint * params);
			typedef void (GLAD_API_PTR *PFNGLGETINTEGER64I_VPROC)(GLenum target, GLuint index, GLint64 * data);
			typedef void (GLAD_API_PTR *PFNGLGETINTEGER64VPROC)(GLenum pname, GLint64 * data);
			typedef void (GLAD_API_PTR *PFNGLGETINTEGERI_VPROC)(GLenum target, GLuint index, GLint * data);
			typedef void (GLAD_API_PTR *PFNGLGETINTEGERVPROC)(GLenum pname, GLint * data);
			typedef void (GLAD_API_PTR *PFNGLGETMULTISAMPLEFVPROC)(GLenum pname, GLuint index, GLfloat * val);
			typedef void (GLAD_API_PTR *PFNGLGETPROGRAMINFOLOGPROC)(GLuint program, GLsizei bufSize, GLsizei * length, GLchar * infoLog);
			typedef void (GLAD_API_PTR *PFNGLGETPROGRAMIVPROC)(GLuint program, GLenum pname, GLint * params);
			typedef void (GLAD_API_PTR *PFNGLGETQUERYOBJECTI64VPROC)(GLuint id, GLenum pname, GLint64 * params);
			typedef void (GLAD_API_PTR *PFNGLGETQUERYOBJECTIVPROC)(GLuint id, GLenum pname, GLint * params);
			typedef void (GLAD_API_PTR *PFNGLGETQUERYOBJECTUI64VPROC)(GLuint id, GLenum pname, GLuint64 * params);
			typedef void (GLAD_API_PTR *PFNGLGETQUERYOBJECTUIVPROC)(GLuint id, GLenum pname, GLuint * params);
			typedef void (GLAD_API_PTR *PFNGLGETQUERYIVPROC)(GLenum target, GLenum pname, GLint * params);
			typedef void (GLAD_API_PTR *PFNGLGETRENDERBUFFERPARAMETERIVPROC)(GLenum target, GLenum pname, GLint * params);
			typedef void (GLAD_API_PTR *PFNGLGETSAMPLERPARAMETERIIVPROC)(GLuint sampler, GLenum pname, GLint * params);
			typedef void (GLAD_API_PTR *PFNGLGETSAMPLERPARAMETERIUIVPROC)(GLuint sampler, GLenum pname, GLuint * params);
			typedef void (GLAD_API_PTR *PFNGLGETSAMPLERPARAMETERFVPROC)(GLuint sampler, GLenum pname, GLfloat * params);
			typedef void (GLAD_API_PTR *PFNGLGETSAMPLERPARAMETERIVPROC)(GLuint sampler, GLenum pname, GLint * params);
			typedef void (GLAD_API_PTR *PFNGLGETSHADERINFOLOGPROC)(GLuint shader, GLsizei bufSize, GLsizei * length, GLchar * infoLog);
			typedef void (GLAD_API_PTR *PFNGLGETSHADERSOURCEPROC)(GLuint shader, GLsizei bufSize, GLsizei * length, GLchar * source);
			typedef void (GLAD_API_PTR *PFNGLGETSHADERIVPROC)(GLuint shader, GLenum pname, GLint * params);
			typedef const GLubyte * (GLAD_API_PTR *PFNGLGETSTRINGPROC)(GLenum name);
			typedef const GLubyte * (GLAD_API_PTR *PFNGLGETSTRINGIPROC)(GLenum name, GLuint index);
			typedef void (GLAD_API_PTR *PFNGLGETSYNCIVPROC)(GLsync sync, GLenum pname, GLsizei count, GLsizei * length, GLint * values);
			typedef void (GLAD_API_PTR *PFNGLGETTEXIMAGEPROC)(GLenum target, GLint level, GLenum format, GLenum type, void * pixels);
			typedef void (GLAD_API_PTR *PFNGLGETTEXLEVELPARAMETERFVPROC)(GLenum target, GLint level, GLenum pname, GLfloat * params);
			typedef void (GLAD_API_PTR *PFNGLGETTEXLEVELPARAMETERIVPROC)(GLenum target, GLint level, GLenum pname, GLint * params);
			typedef void (GLAD_API_PTR *PFNGLGETTEXPARAMETERIIVPROC)(GLenum target, GLenum pname, GLint * params);
			typedef void (GLAD_API_PTR *PFNGLGETTEXPARAMETERIUIVPROC)(GLenum target, GLenum pname, GLuint * params);
			typedef void (GLAD_API_PTR *PFNGLGETTEXPARAMETERFVPROC)(GLenum target, GLenum pname, GLfloat * params);
			typedef void (GLAD_API_PTR *PFNGLGETTEXPARAMETERIVPROC)(GLenum target, GLenum pname, GLint * params);
			typedef void (GLAD_API_PTR *PFNGLGETTRANSFORMFEEDBACKVARYINGPROC)(GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLsizei * size, GLenum * type, GLchar * name);
			typedef GLuint (GLAD_API_PTR *PFNGLGETUNIFORMBLOCKINDEXPROC)(GLuint program, const GLchar * uniformBlockName);
			typedef void (GLAD_API_PTR *PFNGLGETUNIFORMINDICESPROC)(GLuint program, GLsizei uniformCount, const GLchar *const* uniformNames, GLuint * uniformIndices);
			typedef GLint (GLAD_API_PTR *PFNGLGETUNIFORMLOCATIONPROC)(GLuint program, const GLchar * name);
			typedef void (GLAD_API_PTR *PFNGLGETUNIFORMFVPROC)(GLuint program, GLint location, GLfloat * params);
			typedef void (GLAD_API_PTR *PFNGLGETUNIFORMIVPROC)(GLuint program, GLint location, GLint * params);
			typedef void (GLAD_API_PTR *PFNGLGETUNIFORMUIVPROC)(GLuint program, GLint location, GLuint * params);
			typedef void (GLAD_API_PTR *PFNGLGETVERTEXATTRIBIIVPROC)(GLuint index, GLenum pname, GLint * params);
			typedef void (GLAD_API_PTR *PFNGLGETVERTEXATTRIBIUIVPROC)(GLuint index, GLenum pname, GLuint * params);
			typedef void (GLAD_API_PTR *PFNGLGETVERTEXATTRIBPOINTERVPROC)(GLuint index, GLenum pname, void ** pointer);
			typedef void (GLAD_API_PTR *PFNGLGETVERTEXATTRIBDVPROC)(GLuint index, GLenum pname, GLdouble * params);
			typedef void (GLAD_API_PTR *PFNGLGETVERTEXATTRIBFVPROC)(GLuint index, GLenum pname, GLfloat * params);
			typedef void (GLAD_API_PTR *PFNGLGETVERTEXATTRIBIVPROC)(GLuint index, GLenum pname, GLint * params);
			typedef void (GLAD_API_PTR *PFNGLHINTPROC)(GLenum target, GLenum mode);
			typedef GLboolean (GLAD_API_PTR *PFNGLISBUFFERPROC)(GLuint buffer);
			typedef GLboolean (GLAD_API_PTR *PFNGLISENABLEDPROC)(GLenum cap);
			typedef GLboolean (GLAD_API_PTR *PFNGLISENABLEDIPROC)(GLenum target, GLuint index);
			typedef GLboolean (GLAD_API_PTR *PFNGLISFRAMEBUFFERPROC)(GLuint framebuffer);
			typedef GLboolean (GLAD_API_PTR *PFNGLISPROGRAMPROC)(GLuint program);
			typedef GLboolean (GLAD_API_PTR *PFNGLISQUERYPROC)(GLuint id);
			typedef GLboolean (GLAD_API_PTR *PFNGLISRENDERBUFFERPROC)(GLuint renderbuffer);
			typedef GLboolean (GLAD_API_PTR *PFNGLISSAMPLERPROC)(GLuint sampler);
			typedef GLboolean (GLAD_API_PTR *PFNGLISSHADERPROC)(GLuint shader);
			typedef GLboolean (GLAD_API_PTR *PFNGLISSYNCPROC)(GLsync sync);
			typedef GLboolean (GLAD_API_PTR *PFNGLISTEXTUREPROC)(GLuint texture);
			typedef GLboolean (GLAD_API_PTR *PFNGLISVERTEXARRAYPROC)(GLuint array);
			typedef void (GLAD_API_PTR *PFNGLLINEWIDTHPROC)(GLfloat width);
			typedef void (GLAD_API_PTR *PFNGLLINKPROGRAMPROC)(GLuint program);
			typedef void (GLAD_API_PTR *PFNGLLOGICOPPROC)(GLenum opcode);
			typedef void * (GLAD_API_PTR *PFNGLMAPBUFFERPROC)(GLenum target, GLenum access);
			typedef void * (GLAD_API_PTR *PFNGLMAPBUFFERRANGEPROC)(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
			typedef void (GLAD_API_PTR *PFNGLMULTIDRAWARRAYSPROC)(GLenum mode, const GLint * first, const GLsizei * count, GLsizei drawcount);
			typedef void (GLAD_API_PTR *PFNGLMULTIDRAWELEMENTSPROC)(GLenum mode, const GLsizei * count, GLenum type, const void *const* indices, GLsizei drawcount);
			typedef void (GLAD_API_PTR *PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC)(GLenum mode, const GLsizei * count, GLenum type, const void *const* indices, GLsizei drawcount, const GLint * basevertex);
			typedef void (GLAD_API_PTR *PFNGLPIXELSTOREFPROC)(GLenum pname, GLfloat param);
			typedef void (GLAD_API_PTR *PFNGLPIXELSTOREIPROC)(GLenum pname, GLint param);
			typedef void (GLAD_API_PTR *PFNGLPOINTPARAMETERFPROC)(GLenum pname, GLfloat param);
			typedef void (GLAD_API_PTR *PFNGLPOINTPARAMETERFVPROC)(GLenum pname, const GLfloat * params);
			typedef void (GLAD_API_PTR *PFNGLPOINTPARAMETERIPROC)(GLenum pname, GLint param);
			typedef void (GLAD_API_PTR *PFNGLPOINTPARAMETERIVPROC)(GLenum pname, const GLint * params);
			typedef void (GLAD_API_PTR *PFNGLPOINTSIZEPROC)(GLfloat size);
			typedef void (GLAD_API_PTR *PFNGLPOLYGONMODEPROC)(GLenum face, GLenum mode);
			typedef void (GLAD_API_PTR *PFNGLPOLYGONOFFSETPROC)(GLfloat factor, GLfloat units);
			typedef void (GLAD_API_PTR *PFNGLPRIMITIVERESTARTINDEXPROC)(GLuint index);
			typedef void (GLAD_API_PTR *PFNGLPROVOKINGVERTEXPROC)(GLenum mode);
			typedef void (GLAD_API_PTR *PFNGLQUERYCOUNTERPROC)(GLuint id, GLenum target);
			typedef void (GLAD_API_PTR *PFNGLREADBUFFERPROC)(GLenum src);
			typedef void (GLAD_API_PTR *PFNGLREADPIXELSPROC)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void * pixels);
			typedef void (GLAD_API_PTR *PFNGLRENDERBUFFERSTORAGEPROC)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
			typedef void (GLAD_API_PTR *PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
			typedef void (GLAD_API_PTR *PFNGLSAMPLECOVERAGEPROC)(GLfloat value, GLboolean invert);
			typedef void (GLAD_API_PTR *PFNGLSAMPLEMASKIPROC)(GLuint maskNumber, GLbitfield mask);
			typedef void (GLAD_API_PTR *PFNGLSAMPLERPARAMETERIIVPROC)(GLuint sampler, GLenum pname, const GLint * param);
			typedef void (GLAD_API_PTR *PFNGLSAMPLERPARAMETERIUIVPROC)(GLuint sampler, GLenum pname, const GLuint * param);
			typedef void (GLAD_API_PTR *PFNGLSAMPLERPARAMETERFPROC)(GLuint sampler, GLenum pname, GLfloat param);
			typedef void (GLAD_API_PTR *PFNGLSAMPLERPARAMETERFVPROC)(GLuint sampler, GLenum pname, const GLfloat * param);
			typedef void (GLAD_API_PTR *PFNGLSAMPLERPARAMETERIPROC)(GLuint sampler, GLenum pname, GLint param);
			typedef void (GLAD_API_PTR *PFNGLSAMPLERPARAMETERIVPROC)(GLuint sampler, GLenum pname, const GLint * param);
			typedef void (GLAD_API_PTR *PFNGLSCISSORPROC)(GLint x, GLint y, GLsizei width, GLsizei height);
			typedef void (GLAD_API_PTR *PFNGLSHADERSOURCEPROC)(GLuint shader, GLsizei count, const GLchar *const* string, const GLint * length);
			typedef void (GLAD_API_PTR *PFNGLSTENCILFUNCPROC)(GLenum func, GLint ref, GLuint mask);
			typedef void (GLAD_API_PTR *PFNGLSTENCILFUNCSEPARATEPROC)(GLenum face, GLenum func, GLint ref, GLuint mask);
			typedef void (GLAD_API_PTR *PFNGLSTENCILMASKPROC)(GLuint mask);
			typedef void (GLAD_API_PTR *PFNGLSTENCILMASKSEPARATEPROC)(GLenum face, GLuint mask);
			typedef void (GLAD_API_PTR *PFNGLSTENCILOPPROC)(GLenum fail, GLenum zfail, GLenum zpass);
			typedef void (GLAD_API_PTR *PFNGLSTENCILOPSEPARATEPROC)(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
			typedef void (GLAD_API_PTR *PFNGLTEXBUFFERPROC)(GLenum target, GLenum internalformat, GLuint buffer);
			typedef void (GLAD_API_PTR *PFNGLTEXIMAGE1DPROC)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void * pixels);
			typedef void (GLAD_API_PTR *PFNGLTEXIMAGE2DPROC)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void * pixels);
			typedef void (GLAD_API_PTR *PFNGLTEXIMAGE2DMULTISAMPLEPROC)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
			typedef void (GLAD_API_PTR *PFNGLTEXIMAGE3DPROC)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void * pixels);
			typedef void (GLAD_API_PTR *PFNGLTEXIMAGE3DMULTISAMPLEPROC)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
			typedef void (GLAD_API_PTR *PFNGLTEXPARAMETERIIVPROC)(GLenum target, GLenum pname, const GLint * params);
			typedef void (GLAD_API_PTR *PFNGLTEXPARAMETERIUIVPROC)(GLenum target, GLenum pname, const GLuint * params);
			typedef void (GLAD_API_PTR *PFNGLTEXPARAMETERFPROC)(GLenum target, GLenum pname, GLfloat param);
			typedef void (GLAD_API_PTR *PFNGLTEXPARAMETERFVPROC)(GLenum target, GLenum pname, const GLfloat * params);
			typedef void (GLAD_API_PTR *PFNGLTEXPARAMETERIPROC)(GLenum target, GLenum pname, GLint param);
			typedef void (GLAD_API_PTR *PFNGLTEXPARAMETERIVPROC)(GLenum target, GLenum pname, const GLint * params);
			typedef void (GLAD_API_PTR *PFNGLTEXSUBIMAGE1DPROC)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void * pixels);
			typedef void (GLAD_API_PTR *PFNGLTEXSUBIMAGE2DPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void * pixels);
			typedef void (GLAD_API_PTR *PFNGLTEXSUBIMAGE3DPROC)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void * pixels);
			typedef void (GLAD_API_PTR *PFNGLTRANSFORMFEEDBACKVARYINGSPROC)(GLuint program, GLsizei count, const GLchar *const* varyings, GLenum bufferMode);
			typedef void (GLAD_API_PTR *PFNGLUNIFORM1FPROC)(GLint location, GLfloat v0);
			typedef void (GLAD_API_PTR *PFNGLUNIFORM1FVPROC)(GLint location, GLsizei count, const GLfloat * value);
			typedef void (GLAD_API_PTR *PFNGLUNIFORM1IPROC)(GLint location, GLint v0);
			typedef void (GLAD_API_PTR *PFNGLUNIFORM1IVPROC)(GLint location, GLsizei count, const GLint * value);
			typedef void (GLAD_API_PTR *PFNGLUNIFORM1UIPROC)(GLint location, GLuint v0);
			typedef void (GLAD_API_PTR *PFNGLUNIFORM1UIVPROC)(GLint location, GLsizei count, const GLuint * value);
			typedef void (GLAD_API_PTR *PFNGLUNIFORM2FPROC)(GLint location, GLfloat v0, GLfloat v1);
			typedef void (GLAD_API_PTR *PFNGLUNIFORM2FVPROC)(GLint location, GLsizei count, const GLfloat * value);
			typedef void (GLAD_API_PTR *PFNGLUNIFORM2IPROC)(GLint location, GLint v0, GLint v1);
			typedef void (GLAD_API_PTR *PFNGLUNIFORM2IVPROC)(GLint location, GLsizei count, const GLint * value);
			typedef void (GLAD_API_PTR *PFNGLUNIFORM2UIPROC)(GLint location, GLuint v0, GLuint v1);
			typedef void (GLAD_API_PTR *PFNGLUNIFORM2UIVPROC)(GLint location, GLsizei count, const GLuint * value);
			typedef void (GLAD_API_PTR *PFNGLUNIFORM3FPROC)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
			typedef void (GLAD_API_PTR *PFNGLUNIFORM3FVPROC)(GLint location, GLsizei count, const GLfloat * value);
			typedef void (GLAD_API_PTR *PFNGLUNIFORM3IPROC)(GLint location, GLint v0, GLint v1, GLint v2);
			typedef void (GLAD_API_PTR *PFNGLUNIFORM3IVPROC)(GLint location, GLsizei count, const GLint * value);
			typedef void (GLAD_API_PTR *PFNGLUNIFORM3UIPROC)(GLint location, GLuint v0, GLuint v1, GLuint v2);
			typedef void (GLAD_API_PTR *PFNGLUNIFORM3UIVPROC)(GLint location, GLsizei count, const GLuint * value);
			typedef void (GLAD_API_PTR *PFNGLUNIFORM4FPROC)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
			typedef void (GLAD_API_PTR *PFNGLUNIFORM4FVPROC)(GLint location, GLsizei count, const GLfloat * value);
			typedef void (GLAD_API_PTR *PFNGLUNIFORM4IPROC)(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
			typedef void (GLAD_API_PTR *PFNGLUNIFORM4IVPROC)(GLint location, GLsizei count, const GLint * value);
			typedef void (GLAD_API_PTR *PFNGLUNIFORM4UIPROC)(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
			typedef void (GLAD_API_PTR *PFNGLUNIFORM4UIVPROC)(GLint location, GLsizei count, const GLuint * value);
			typedef void (GLAD_API_PTR *PFNGLUNIFORMBLOCKBINDINGPROC)(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);
			typedef void (GLAD_API_PTR *PFNGLUNIFORMMATRIX2FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
			typedef void (GLAD_API_PTR *PFNGLUNIFORMMATRIX2X3FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
			typedef void (GLAD_API_PTR *PFNGLUNIFORMMATRIX2X4FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
			typedef void (GLAD_API_PTR *PFNGLUNIFORMMATRIX3FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
			typedef void (GLAD_API_PTR *PFNGLUNIFORMMATRIX3X2FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
			typedef void (GLAD_API_PTR *PFNGLUNIFORMMATRIX3X4FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
			typedef void (GLAD_API_PTR *PFNGLUNIFORMMATRIX4FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
			typedef void (GLAD_API_PTR *PFNGLUNIFORMMATRIX4X2FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
			typedef void (GLAD_API_PTR *PFNGLUNIFORMMATRIX4X3FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat * value);
			typedef GLboolean (GLAD_API_PTR *PFNGLUNMAPBUFFERPROC)(GLenum target);
			typedef void (GLAD_API_PTR *PFNGLUSEPROGRAMPROC)(GLuint program);
			typedef void (GLAD_API_PTR *PFNGLVALIDATEPROGRAMPROC)(GLuint program);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB1DPROC)(GLuint index, GLdouble x);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB1DVPROC)(GLuint index, const GLdouble * v);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB1FPROC)(GLuint index, GLfloat x);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB1FVPROC)(GLuint index, const GLfloat * v);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB1SPROC)(GLuint index, GLshort x);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB1SVPROC)(GLuint index, const GLshort * v);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB2DPROC)(GLuint index, GLdouble x, GLdouble y);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB2DVPROC)(GLuint index, const GLdouble * v);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB2FPROC)(GLuint index, GLfloat x, GLfloat y);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB2FVPROC)(GLuint index, const GLfloat * v);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB2SPROC)(GLuint index, GLshort x, GLshort y);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB2SVPROC)(GLuint index, const GLshort * v);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB3DPROC)(GLuint index, GLdouble x, GLdouble y, GLdouble z);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB3DVPROC)(GLuint index, const GLdouble * v);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB3FPROC)(GLuint index, GLfloat x, GLfloat y, GLfloat z);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB3FVPROC)(GLuint index, const GLfloat * v);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB3SPROC)(GLuint index, GLshort x, GLshort y, GLshort z);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB3SVPROC)(GLuint index, const GLshort * v);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB4NBVPROC)(GLuint index, const GLbyte * v);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB4NIVPROC)(GLuint index, const GLint * v);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB4NSVPROC)(GLuint index, const GLshort * v);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB4NUBPROC)(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB4NUBVPROC)(GLuint index, const GLubyte * v);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB4NUIVPROC)(GLuint index, const GLuint * v);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB4NUSVPROC)(GLuint index, const GLushort * v);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB4BVPROC)(GLuint index, const GLbyte * v);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB4DPROC)(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB4DVPROC)(GLuint index, const GLdouble * v);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB4FPROC)(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB4FVPROC)(GLuint index, const GLfloat * v);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB4IVPROC)(GLuint index, const GLint * v);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB4SPROC)(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB4SVPROC)(GLuint index, const GLshort * v);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB4UBVPROC)(GLuint index, const GLubyte * v);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB4UIVPROC)(GLuint index, const GLuint * v);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIB4USVPROC)(GLuint index, const GLushort * v);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBDIVISORPROC)(GLuint index, GLuint divisor);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBI1IPROC)(GLuint index, GLint x);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBI1IVPROC)(GLuint index, const GLint * v);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBI1UIPROC)(GLuint index, GLuint x);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBI1UIVPROC)(GLuint index, const GLuint * v);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBI2IPROC)(GLuint index, GLint x, GLint y);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBI2IVPROC)(GLuint index, const GLint * v);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBI2UIPROC)(GLuint index, GLuint x, GLuint y);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBI2UIVPROC)(GLuint index, const GLuint * v);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBI3IPROC)(GLuint index, GLint x, GLint y, GLint z);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBI3IVPROC)(GLuint index, const GLint * v);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBI3UIPROC)(GLuint index, GLuint x, GLuint y, GLuint z);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBI3UIVPROC)(GLuint index, const GLuint * v);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBI4BVPROC)(GLuint index, const GLbyte * v);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBI4IPROC)(GLuint index, GLint x, GLint y, GLint z, GLint w);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBI4IVPROC)(GLuint index, const GLint * v);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBI4SVPROC)(GLuint index, const GLshort * v);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBI4UBVPROC)(GLuint index, const GLubyte * v);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBI4UIPROC)(GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBI4UIVPROC)(GLuint index, const GLuint * v);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBI4USVPROC)(GLuint index, const GLushort * v);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBIPOINTERPROC)(GLuint index, GLint size, GLenum type, GLsizei stride, const void * pointer);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBP1UIPROC)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBP1UIVPROC)(GLuint index, GLenum type, GLboolean normalized, const GLuint * value);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBP2UIPROC)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBP2UIVPROC)(GLuint index, GLenum type, GLboolean normalized, const GLuint * value);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBP3UIPROC)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBP3UIVPROC)(GLuint index, GLenum type, GLboolean normalized, const GLuint * value);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBP4UIPROC)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBP4UIVPROC)(GLuint index, GLenum type, GLboolean normalized, const GLuint * value);
			typedef void (GLAD_API_PTR *PFNGLVERTEXATTRIBPOINTERPROC)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void * pointer);
			typedef void (GLAD_API_PTR *PFNGLVIEWPORTPROC)(GLint x, GLint y, GLsizei width, GLsizei height);
			typedef void (GLAD_API_PTR *PFNGLWAITSYNCPROC)(GLsync sync, GLbitfield flags, GLuint64 timeout);

			GLAD_API_CALL PFNGLACTIVETEXTUREPROC glad_glActiveTexture;
			#define glActiveTexture glad_glActiveTexture
			GLAD_API_CALL PFNGLATTACHSHADERPROC glad_glAttachShader;
			#define glAttachShader glad_glAttachShader
			GLAD_API_CALL PFNGLBEGINCONDITIONALRENDERPROC glad_glBeginConditionalRender;
			#define glBeginConditionalRender glad_glBeginConditionalRender
			GLAD_API_CALL PFNGLBEGINQUERYPROC glad_glBeginQuery;
			#define glBeginQuery glad_glBeginQuery
			GLAD_API_CALL PFNGLBEGINTRANSFORMFEEDBACKPROC glad_glBeginTransformFeedback;
			#define glBeginTransformFeedback glad_glBeginTransformFeedback
			GLAD_API_CALL PFNGLBINDATTRIBLOCATIONPROC glad_glBindAttribLocation;
			#define glBindAttribLocation glad_glBindAttribLocation
			GLAD_API_CALL PFNGLBINDBUFFERPROC glad_glBindBuffer;
			#define glBindBuffer glad_glBindBuffer
			GLAD_API_CALL PFNGLBINDBUFFERBASEPROC glad_glBindBufferBase;
			#define glBindBufferBase glad_glBindBufferBase
			GLAD_API_CALL PFNGLBINDBUFFERRANGEPROC glad_glBindBufferRange;
			#define glBindBufferRange glad_glBindBufferRange
			GLAD_API_CALL PFNGLBINDFRAGDATALOCATIONPROC glad_glBindFragDataLocation;
			#define glBindFragDataLocation glad_glBindFragDataLocation
			GLAD_API_CALL PFNGLBINDFRAGDATALOCATIONINDEXEDPROC glad_glBindFragDataLocationIndexed;
			#define glBindFragDataLocationIndexed glad_glBindFragDataLocationIndexed
			GLAD_API_CALL PFNGLBINDFRAMEBUFFERPROC glad_glBindFramebuffer;
			#define glBindFramebuffer glad_glBindFramebuffer
			GLAD_API_CALL PFNGLBINDRENDERBUFFERPROC glad_glBindRenderbuffer;
			#define glBindRenderbuffer glad_glBindRenderbuffer
			GLAD_API_CALL PFNGLBINDSAMPLERPROC glad_glBindSampler;
			#define glBindSampler glad_glBindSampler
			GLAD_API_CALL PFNGLBINDTEXTUREPROC glad_glBindTexture;
			#define glBindTexture glad_glBindTexture
			GLAD_API_CALL PFNGLBINDVERTEXARRAYPROC glad_glBindVertexArray;
			#define glBindVertexArray glad_glBindVertexArray
			GLAD_API_CALL PFNGLBLENDCOLORPROC glad_glBlendColor;
			#define glBlendColor glad_glBlendColor
			GLAD_API_CALL PFNGLBLENDEQUATIONPROC glad_glBlendEquation;
			#define glBlendEquation glad_glBlendEquation
			GLAD_API_CALL PFNGLBLENDEQUATIONSEPARATEPROC glad_glBlendEquationSeparate;
			#define glBlendEquationSeparate glad_glBlendEquationSeparate
			GLAD_API_CALL PFNGLBLENDFUNCPROC glad_glBlendFunc;
			#define glBlendFunc glad_glBlendFunc
			GLAD_API_CALL PFNGLBLENDFUNCSEPARATEPROC glad_glBlendFuncSeparate;
			#define glBlendFuncSeparate glad_glBlendFuncSeparate
			GLAD_API_CALL PFNGLBLITFRAMEBUFFERPROC glad_glBlitFramebuffer;
			#define glBlitFramebuffer glad_glBlitFramebuffer
			GLAD_API_CALL PFNGLBUFFERDATAPROC glad_glBufferData;
			#define glBufferData glad_glBufferData
			GLAD_API_CALL PFNGLBUFFERSUBDATAPROC glad_glBufferSubData;
			#define glBufferSubData glad_glBufferSubData
			GLAD_API_CALL PFNGLCHECKFRAMEBUFFERSTATUSPROC glad_glCheckFramebufferStatus;
			#define glCheckFramebufferStatus glad_glCheckFramebufferStatus
			GLAD_API_CALL PFNGLCLAMPCOLORPROC glad_glClampColor;
			#define glClampColor glad_glClampColor
			GLAD_API_CALL PFNGLCLEARPROC glad_glClear;
			#define glClear glad_glClear
			GLAD_API_CALL PFNGLCLEARBUFFERFIPROC glad_glClearBufferfi;
			#define glClearBufferfi glad_glClearBufferfi
			GLAD_API_CALL PFNGLCLEARBUFFERFVPROC glad_glClearBufferfv;
			#define glClearBufferfv glad_glClearBufferfv
			GLAD_API_CALL PFNGLCLEARBUFFERIVPROC glad_glClearBufferiv;
			#define glClearBufferiv glad_glClearBufferiv
			GLAD_API_CALL PFNGLCLEARBUFFERUIVPROC glad_glClearBufferuiv;
			#define glClearBufferuiv glad_glClearBufferuiv
			GLAD_API_CALL PFNGLCLEARCOLORPROC glad_glClearColor;
			#define glClearColor glad_glClearColor
			GLAD_API_CALL PFNGLCLEARDEPTHPROC glad_glClearDepth;
			#define glClearDepth glad_glClearDepth
			GLAD_API_CALL PFNGLCLEARSTENCILPROC glad_glClearStencil;
			#define glClearStencil glad_glClearStencil
			GLAD_API_CALL PFNGLCLIENTWAITSYNCPROC glad_glClientWaitSync;
			#define glClientWaitSync glad_glClientWaitSync
			GLAD_API_CALL PFNGLCOLORMASKPROC glad_glColorMask;
			#define glColorMask glad_glColorMask
			GLAD_API_CALL PFNGLCOLORMASKIPROC glad_glColorMaski;
			#define glColorMaski glad_glColorMaski
			GLAD_API_CALL PFNGLCOMPILESHADERPROC glad_glCompileShader;
			#define glCompileShader glad_glCompileShader
			GLAD_API_CALL PFNGLCOMPRESSEDTEXIMAGE1DPROC glad_glCompressedTexImage1D;
			#define glCompressedTexImage1D glad_glCompressedTexImage1D
			GLAD_API_CALL PFNGLCOMPRESSEDTEXIMAGE2DPROC glad_glCompressedTexImage2D;
			#define glCompressedTexImage2D glad_glCompressedTexImage2D
			GLAD_API_CALL PFNGLCOMPRESSEDTEXIMAGE3DPROC glad_glCompressedTexImage3D;
			#define glCompressedTexImage3D glad_glCompressedTexImage3D
			GLAD_API_CALL PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC glad_glCompressedTexSubImage1D;
			#define glCompressedTexSubImage1D glad_glCompressedTexSubImage1D
			GLAD_API_CALL PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC glad_glCompressedTexSubImage2D;
			#define glCompressedTexSubImage2D glad_glCompressedTexSubImage2D
			GLAD_API_CALL PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC glad_glCompressedTexSubImage3D;
			#define glCompressedTexSubImage3D glad_glCompressedTexSubImage3D
			GLAD_API_CALL PFNGLCOPYBUFFERSUBDATAPROC glad_glCopyBufferSubData;
			#define glCopyBufferSubData glad_glCopyBufferSubData
			GLAD_API_CALL PFNGLCOPYTEXIMAGE1DPROC glad_glCopyTexImage1D;
			#define glCopyTexImage1D glad_glCopyTexImage1D
			GLAD_API_CALL PFNGLCOPYTEXIMAGE2DPROC glad_glCopyTexImage2D;
			#define glCopyTexImage2D glad_glCopyTexImage2D
			GLAD_API_CALL PFNGLCOPYTEXSUBIMAGE1DPROC glad_glCopyTexSubImage1D;
			#define glCopyTexSubImage1D glad_glCopyTexSubImage1D
			GLAD_API_CALL PFNGLCOPYTEXSUBIMAGE2DPROC glad_glCopyTexSubImage2D;
			#define glCopyTexSubImage2D glad_glCopyTexSubImage2D
			GLAD_API_CALL PFNGLCOPYTEXSUBIMAGE3DPROC glad_glCopyTexSubImage3D;
			#define glCopyTexSubImage3D glad_glCopyTexSubImage3D
			GLAD_API_CALL PFNGLCREATEPROGRAMPROC glad_glCreateProgram;
			#define glCreateProgram glad_glCreateProgram
			GLAD_API_CALL PFNGLCREATESHADERPROC glad_glCreateShader;
			#define glCreateShader glad_glCreateShader
			GLAD_API_CALL PFNGLCULLFACEPROC glad_glCullFace;
			#define glCullFace glad_glCullFace
			GLAD_API_CALL PFNGLDELETEBUFFERSPROC glad_glDeleteBuffers;
			#define glDeleteBuffers glad_glDeleteBuffers
			GLAD_API_CALL PFNGLDELETEFRAMEBUFFERSPROC glad_glDeleteFramebuffers;
			#define glDeleteFramebuffers glad_glDeleteFramebuffers
			GLAD_API_CALL PFNGLDELETEPROGRAMPROC glad_glDeleteProgram;
			#define glDeleteProgram glad_glDeleteProgram
			GLAD_API_CALL PFNGLDELETEQUERIESPROC glad_glDeleteQueries;
			#define glDeleteQueries glad_glDeleteQueries
			GLAD_API_CALL PFNGLDELETERENDERBUFFERSPROC glad_glDeleteRenderbuffers;
			#define glDeleteRenderbuffers glad_glDeleteRenderbuffers
			GLAD_API_CALL PFNGLDELETESAMPLERSPROC glad_glDeleteSamplers;
			#define glDeleteSamplers glad_glDeleteSamplers
			GLAD_API_CALL PFNGLDELETESHADERPROC glad_glDeleteShader;
			#define glDeleteShader glad_glDeleteShader
			GLAD_API_CALL PFNGLDELETESYNCPROC glad_glDeleteSync;
			#define glDeleteSync glad_glDeleteSync
			GLAD_API_CALL PFNGLDELETETEXTURESPROC glad_glDeleteTextures;
			#define glDeleteTextures glad_glDeleteTextures
			GLAD_API_CALL PFNGLDELETEVERTEXARRAYSPROC glad_glDeleteVertexArrays;
			#define glDeleteVertexArrays glad_glDeleteVertexArrays
			GLAD_API_CALL PFNGLDEPTHFUNCPROC glad_glDepthFunc;
			#define glDepthFunc glad_glDepthFunc
			GLAD_API_CALL PFNGLDEPTHMASKPROC glad_glDepthMask;
			#define glDepthMask glad_glDepthMask
			GLAD_API_CALL PFNGLDEPTHRANGEPROC glad_glDepthRange;
			#define glDepthRange glad_glDepthRange
			GLAD_API_CALL PFNGLDETACHSHADERPROC glad_glDetachShader;
			#define glDetachShader glad_glDetachShader
			GLAD_API_CALL PFNGLDISABLEPROC glad_glDisable;
			#define glDisable glad_glDisable
			GLAD_API_CALL PFNGLDISABLEVERTEXATTRIBARRAYPROC glad_glDisableVertexAttribArray;
			#define glDisableVertexAttribArray glad_glDisableVertexAttribArray
			GLAD_API_CALL PFNGLDISABLEIPROC glad_glDisablei;
			#define glDisablei glad_glDisablei
			GLAD_API_CALL PFNGLDRAWARRAYSPROC glad_glDrawArrays;
			#define glDrawArrays glad_glDrawArrays
			GLAD_API_CALL PFNGLDRAWARRAYSINSTANCEDPROC glad_glDrawArraysInstanced;
			#define glDrawArraysInstanced glad_glDrawArraysInstanced
			GLAD_API_CALL PFNGLDRAWBUFFERPROC glad_glDrawBuffer;
			#define glDrawBuffer glad_glDrawBuffer
			GLAD_API_CALL PFNGLDRAWBUFFERSPROC glad_glDrawBuffers;
			#define glDrawBuffers glad_glDrawBuffers
			GLAD_API_CALL PFNGLDRAWELEMENTSPROC glad_glDrawElements;
			#define glDrawElements glad_glDrawElements
			GLAD_API_CALL PFNGLDRAWELEMENTSBASEVERTEXPROC glad_glDrawElementsBaseVertex;
			#define glDrawElementsBaseVertex glad_glDrawElementsBaseVertex
			GLAD_API_CALL PFNGLDRAWELEMENTSINSTANCEDPROC glad_glDrawElementsInstanced;
			#define glDrawElementsInstanced glad_glDrawElementsInstanced
			GLAD_API_CALL PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC glad_glDrawElementsInstancedBaseVertex;
			#define glDrawElementsInstancedBaseVertex glad_glDrawElementsInstancedBaseVertex
			GLAD_API_CALL PFNGLDRAWRANGEELEMENTSPROC glad_glDrawRangeElements;
			#define glDrawRangeElements glad_glDrawRangeElements
			GLAD_API_CALL PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC glad_glDrawRangeElementsBaseVertex;
			#define glDrawRangeElementsBaseVertex glad_glDrawRangeElementsBaseVertex
			GLAD_API_CALL PFNGLENABLEPROC glad_glEnable;
			#define glEnable glad_glEnable
			GLAD_API_CALL PFNGLENABLEVERTEXATTRIBARRAYPROC glad_glEnableVertexAttribArray;
			#define glEnableVertexAttribArray glad_glEnableVertexAttribArray
			GLAD_API_CALL PFNGLENABLEIPROC glad_glEnablei;
			#define glEnablei glad_glEnablei
			GLAD_API_CALL PFNGLENDCONDITIONALRENDERPROC glad_glEndConditionalRender;
			#define glEndConditionalRender glad_glEndConditionalRender
			GLAD_API_CALL PFNGLENDQUERYPROC glad_glEndQuery;
			#define glEndQuery glad_glEndQuery
			GLAD_API_CALL PFNGLENDTRANSFORMFEEDBACKPROC glad_glEndTransformFeedback;
			#define glEndTransformFeedback glad_glEndTransformFeedback
			GLAD_API_CALL PFNGLFENCESYNCPROC glad_glFenceSync;
			#define glFenceSync glad_glFenceSync
			GLAD_API_CALL PFNGLFINISHPROC glad_glFinish;
			#define glFinish glad_glFinish
			GLAD_API_CALL PFNGLFLUSHPROC glad_glFlush;
			#define glFlush glad_glFlush
			GLAD_API_CALL PFNGLFLUSHMAPPEDBUFFERRANGEPROC glad_glFlushMappedBufferRange;
			#define glFlushMappedBufferRange glad_glFlushMappedBufferRange
			GLAD_API_CALL PFNGLFRAMEBUFFERRENDERBUFFERPROC glad_glFramebufferRenderbuffer;
			#define glFramebufferRenderbuffer glad_glFramebufferRenderbuffer
			GLAD_API_CALL PFNGLFRAMEBUFFERTEXTUREPROC glad_glFramebufferTexture;
			#define glFramebufferTexture glad_glFramebufferTexture
			GLAD_API_CALL PFNGLFRAMEBUFFERTEXTURE1DPROC glad_glFramebufferTexture1D;
			#define glFramebufferTexture1D glad_glFramebufferTexture1D
			GLAD_API_CALL PFNGLFRAMEBUFFERTEXTURE2DPROC glad_glFramebufferTexture2D;
			#define glFramebufferTexture2D glad_glFramebufferTexture2D
			GLAD_API_CALL PFNGLFRAMEBUFFERTEXTURE3DPROC glad_glFramebufferTexture3D;
			#define glFramebufferTexture3D glad_glFramebufferTexture3D
			GLAD_API_CALL PFNGLFRAMEBUFFERTEXTURELAYERPROC glad_glFramebufferTextureLayer;
			#define glFramebufferTextureLayer glad_glFramebufferTextureLayer
			GLAD_API_CALL PFNGLFRONTFACEPROC glad_glFrontFace;
			#define glFrontFace glad_glFrontFace
			GLAD_API_CALL PFNGLGENBUFFERSPROC glad_glGenBuffers;
			#define glGenBuffers glad_glGenBuffers
			GLAD_API_CALL PFNGLGENFRAMEBUFFERSPROC glad_glGenFramebuffers;
			#define glGenFramebuffers glad_glGenFramebuffers
			GLAD_API_CALL PFNGLGENQUERIESPROC glad_glGenQueries;
			#define glGenQueries glad_glGenQueries
			GLAD_API_CALL PFNGLGENRENDERBUFFERSPROC glad_glGenRenderbuffers;
			#define glGenRenderbuffers glad_glGenRenderbuffers
			GLAD_API_CALL PFNGLGENSAMPLERSPROC glad_glGenSamplers;
			#define glGenSamplers glad_glGenSamplers
			GLAD_API_CALL PFNGLGENTEXTURESPROC glad_glGenTextures;
			#define glGenTextures glad_glGenTextures
			GLAD_API_CALL PFNGLGENVERTEXARRAYSPROC glad_glGenVertexArrays;
			#define glGenVertexArrays glad_glGenVertexArrays
			GLAD_API_CALL PFNGLGENERATEMIPMAPPROC glad_glGenerateMipmap;
			#define glGenerateMipmap glad_glGenerateMipmap
			GLAD_API_CALL PFNGLGETACTIVEATTRIBPROC glad_glGetActiveAttrib;
			#define glGetActiveAttrib glad_glGetActiveAttrib
			GLAD_API_CALL PFNGLGETACTIVEUNIFORMPROC glad_glGetActiveUniform;
			#define glGetActiveUniform glad_glGetActiveUniform
			GLAD_API_CALL PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC glad_glGetActiveUniformBlockName;
			#define glGetActiveUniformBlockName glad_glGetActiveUniformBlockName
			GLAD_API_CALL PFNGLGETACTIVEUNIFORMBLOCKIVPROC glad_glGetActiveUniformBlockiv;
			#define glGetActiveUniformBlockiv glad_glGetActiveUniformBlockiv
			GLAD_API_CALL PFNGLGETACTIVEUNIFORMNAMEPROC glad_glGetActiveUniformName;
			#define glGetActiveUniformName glad_glGetActiveUniformName
			GLAD_API_CALL PFNGLGETACTIVEUNIFORMSIVPROC glad_glGetActiveUniformsiv;
			#define glGetActiveUniformsiv glad_glGetActiveUniformsiv
			GLAD_API_CALL PFNGLGETATTACHEDSHADERSPROC glad_glGetAttachedShaders;
			#define glGetAttachedShaders glad_glGetAttachedShaders
			GLAD_API_CALL PFNGLGETATTRIBLOCATIONPROC glad_glGetAttribLocation;
			#define glGetAttribLocation glad_glGetAttribLocation
			GLAD_API_CALL PFNGLGETBOOLEANI_VPROC glad_glGetBooleani_v;
			#define glGetBooleani_v glad_glGetBooleani_v
			GLAD_API_CALL PFNGLGETBOOLEANVPROC glad_glGetBooleanv;
			#define glGetBooleanv glad_glGetBooleanv
			GLAD_API_CALL PFNGLGETBUFFERPARAMETERI64VPROC glad_glGetBufferParameteri64v;
			#define glGetBufferParameteri64v glad_glGetBufferParameteri64v
			GLAD_API_CALL PFNGLGETBUFFERPARAMETERIVPROC glad_glGetBufferParameteriv;
			#define glGetBufferParameteriv glad_glGetBufferParameteriv
			GLAD_API_CALL PFNGLGETBUFFERPOINTERVPROC glad_glGetBufferPointerv;
			#define glGetBufferPointerv glad_glGetBufferPointerv
			GLAD_API_CALL PFNGLGETBUFFERSUBDATAPROC glad_glGetBufferSubData;
			#define glGetBufferSubData glad_glGetBufferSubData
			GLAD_API_CALL PFNGLGETCOMPRESSEDTEXIMAGEPROC glad_glGetCompressedTexImage;
			#define glGetCompressedTexImage glad_glGetCompressedTexImage
			GLAD_API_CALL PFNGLGETDOUBLEVPROC glad_glGetDoublev;
			#define glGetDoublev glad_glGetDoublev
			GLAD_API_CALL PFNGLGETERRORPROC glad_glGetError;
			#define glGetError glad_glGetError
			GLAD_API_CALL PFNGLGETFLOATVPROC glad_glGetFloatv;
			#define glGetFloatv glad_glGetFloatv
			GLAD_API_CALL PFNGLGETFRAGDATAINDEXPROC glad_glGetFragDataIndex;
			#define glGetFragDataIndex glad_glGetFragDataIndex
			GLAD_API_CALL PFNGLGETFRAGDATALOCATIONPROC glad_glGetFragDataLocation;
			#define glGetFragDataLocation glad_glGetFragDataLocation
			GLAD_API_CALL PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC glad_glGetFramebufferAttachmentParameteriv;
			#define glGetFramebufferAttachmentParameteriv glad_glGetFramebufferAttachmentParameteriv
			GLAD_API_CALL PFNGLGETINTEGER64I_VPROC glad_glGetInteger64i_v;
			#define glGetInteger64i_v glad_glGetInteger64i_v
			GLAD_API_CALL PFNGLGETINTEGER64VPROC glad_glGetInteger64v;
			#define glGetInteger64v glad_glGetInteger64v
			GLAD_API_CALL PFNGLGETINTEGERI_VPROC glad_glGetIntegeri_v;
			#define glGetIntegeri_v glad_glGetIntegeri_v
			GLAD_API_CALL PFNGLGETINTEGERVPROC glad_glGetIntegerv;
			#define glGetIntegerv glad_glGetIntegerv
			GLAD_API_CALL PFNGLGETMULTISAMPLEFVPROC glad_glGetMultisamplefv;
			#define glGetMultisamplefv glad_glGetMultisamplefv
			GLAD_API_CALL PFNGLGETPROGRAMINFOLOGPROC glad_glGetProgramInfoLog;
			#define glGetProgramInfoLog glad_glGetProgramInfoLog
			GLAD_API_CALL PFNGLGETPROGRAMIVPROC glad_glGetProgramiv;
			#define glGetProgramiv glad_glGetProgramiv
			GLAD_API_CALL PFNGLGETQUERYOBJECTI64VPROC glad_glGetQueryObjecti64v;
			#define glGetQueryObjecti64v glad_glGetQueryObjecti64v
			GLAD_API_CALL PFNGLGETQUERYOBJECTIVPROC glad_glGetQueryObjectiv;
			#define glGetQueryObjectiv glad_glGetQueryObjectiv
			GLAD_API_CALL PFNGLGETQUERYOBJECTUI64VPROC glad_glGetQueryObjectui64v;
			#define glGetQueryObjectui64v glad_glGetQueryObjectui64v
			GLAD_API_CALL PFNGLGETQUERYOBJECTUIVPROC glad_glGetQueryObjectuiv;
			#define glGetQueryObjectuiv glad_glGetQueryObjectuiv
			GLAD_API_CALL PFNGLGETQUERYIVPROC glad_glGetQueryiv;
			#define glGetQueryiv glad_glGetQueryiv
			GLAD_API_CALL PFNGLGETRENDERBUFFERPARAMETERIVPROC glad_glGetRenderbufferParameteriv;
			#define glGetRenderbufferParameteriv glad_glGetRenderbufferParameteriv
			GLAD_API_CALL PFNGLGETSAMPLERPARAMETERIIVPROC glad_glGetSamplerParameterIiv;
			#define glGetSamplerParameterIiv glad_glGetSamplerParameterIiv
			GLAD_API_CALL PFNGLGETSAMPLERPARAMETERIUIVPROC glad_glGetSamplerParameterIuiv;
			#define glGetSamplerParameterIuiv glad_glGetSamplerParameterIuiv
			GLAD_API_CALL PFNGLGETSAMPLERPARAMETERFVPROC glad_glGetSamplerParameterfv;
			#define glGetSamplerParameterfv glad_glGetSamplerParameterfv
			GLAD_API_CALL PFNGLGETSAMPLERPARAMETERIVPROC glad_glGetSamplerParameteriv;
			#define glGetSamplerParameteriv glad_glGetSamplerParameteriv
			GLAD_API_CALL PFNGLGETSHADERINFOLOGPROC glad_glGetShaderInfoLog;
			#define glGetShaderInfoLog glad_glGetShaderInfoLog
			GLAD_API_CALL PFNGLGETSHADERSOURCEPROC glad_glGetShaderSource;
			#define glGetShaderSource glad_glGetShaderSource
			GLAD_API_CALL PFNGLGETSHADERIVPROC glad_glGetShaderiv;
			#define glGetShaderiv glad_glGetShaderiv
			GLAD_API_CALL PFNGLGETSTRINGPROC glad_glGetString;
			#define glGetString glad_glGetString
			GLAD_API_CALL PFNGLGETSTRINGIPROC glad_glGetStringi;
			#define glGetStringi glad_glGetStringi
			GLAD_API_CALL PFNGLGETSYNCIVPROC glad_glGetSynciv;
			#define glGetSynciv glad_glGetSynciv
			GLAD_API_CALL PFNGLGETTEXIMAGEPROC glad_glGetTexImage;
			#define glGetTexImage glad_glGetTexImage
			GLAD_API_CALL PFNGLGETTEXLEVELPARAMETERFVPROC glad_glGetTexLevelParameterfv;
			#define glGetTexLevelParameterfv glad_glGetTexLevelParameterfv
			GLAD_API_CALL PFNGLGETTEXLEVELPARAMETERIVPROC glad_glGetTexLevelParameteriv;
			#define glGetTexLevelParameteriv glad_glGetTexLevelParameteriv
			GLAD_API_CALL PFNGLGETTEXPARAMETERIIVPROC glad_glGetTexParameterIiv;
			#define glGetTexParameterIiv glad_glGetTexParameterIiv
			GLAD_API_CALL PFNGLGETTEXPARAMETERIUIVPROC glad_glGetTexParameterIuiv;
			#define glGetTexParameterIuiv glad_glGetTexParameterIuiv
			GLAD_API_CALL PFNGLGETTEXPARAMETERFVPROC glad_glGetTexParameterfv;
			#define glGetTexParameterfv glad_glGetTexParameterfv
			GLAD_API_CALL PFNGLGETTEXPARAMETERIVPROC glad_glGetTexParameteriv;
			#define glGetTexParameteriv glad_glGetTexParameteriv
			GLAD_API_CALL PFNGLGETTRANSFORMFEEDBACKVARYINGPROC glad_glGetTransformFeedbackVarying;
			#define glGetTransformFeedbackVarying glad_glGetTransformFeedbackVarying
			GLAD_API_CALL PFNGLGETUNIFORMBLOCKINDEXPROC glad_glGetUniformBlockIndex;
			#define glGetUniformBlockIndex glad_glGetUniformBlockIndex
			GLAD_API_CALL PFNGLGETUNIFORMINDICESPROC glad_glGetUniformIndices;
			#define glGetUniformIndices glad_glGetUniformIndices
			GLAD_API_CALL PFNGLGETUNIFORMLOCATIONPROC glad_glGetUniformLocation;
			#define glGetUniformLocation glad_glGetUniformLocation
			GLAD_API_CALL PFNGLGETUNIFORMFVPROC glad_glGetUniformfv;
			#define glGetUniformfv glad_glGetUniformfv
			GLAD_API_CALL PFNGLGETUNIFORMIVPROC glad_glGetUniformiv;
			#define glGetUniformiv glad_glGetUniformiv
			GLAD_API_CALL PFNGLGETUNIFORMUIVPROC glad_glGetUniformuiv;
			#define glGetUniformuiv glad_glGetUniformuiv
			GLAD_API_CALL PFNGLGETVERTEXATTRIBIIVPROC glad_glGetVertexAttribIiv;
			#define glGetVertexAttribIiv glad_glGetVertexAttribIiv
			GLAD_API_CALL PFNGLGETVERTEXATTRIBIUIVPROC glad_glGetVertexAttribIuiv;
			#define glGetVertexAttribIuiv glad_glGetVertexAttribIuiv
			GLAD_API_CALL PFNGLGETVERTEXATTRIBPOINTERVPROC glad_glGetVertexAttribPointerv;
			#define glGetVertexAttribPointerv glad_glGetVertexAttribPointerv
			GLAD_API_CALL PFNGLGETVERTEXATTRIBDVPROC glad_glGetVertexAttribdv;
			#define glGetVertexAttribdv glad_glGetVertexAttribdv
			GLAD_API_CALL PFNGLGETVERTEXATTRIBFVPROC glad_glGetVertexAttribfv;
			#define glGetVertexAttribfv glad_glGetVertexAttribfv
			GLAD_API_CALL PFNGLGETVERTEXATTRIBIVPROC glad_glGetVertexAttribiv;
			#define glGetVertexAttribiv glad_glGetVertexAttribiv
			GLAD_API_CALL PFNGLHINTPROC glad_glHint;
			#define glHint glad_glHint
			GLAD_API_CALL PFNGLISBUFFERPROC glad_glIsBuffer;
			#define glIsBuffer glad_glIsBuffer
			GLAD_API_CALL PFNGLISENABLEDPROC glad_glIsEnabled;
			#define glIsEnabled glad_glIsEnabled
			GLAD_API_CALL PFNGLISENABLEDIPROC glad_glIsEnabledi;
			#define glIsEnabledi glad_glIsEnabledi
			GLAD_API_CALL PFNGLISFRAMEBUFFERPROC glad_glIsFramebuffer;
			#define glIsFramebuffer glad_glIsFramebuffer
			GLAD_API_CALL PFNGLISPROGRAMPROC glad_glIsProgram;
			#define glIsProgram glad_glIsProgram
			GLAD_API_CALL PFNGLISQUERYPROC glad_glIsQuery;
			#define glIsQuery glad_glIsQuery
			GLAD_API_CALL PFNGLISRENDERBUFFERPROC glad_glIsRenderbuffer;
			#define glIsRenderbuffer glad_glIsRenderbuffer
			GLAD_API_CALL PFNGLISSAMPLERPROC glad_glIsSampler;
			#define glIsSampler glad_glIsSampler
			GLAD_API_CALL PFNGLISSHADERPROC glad_glIsShader;
			#define glIsShader glad_glIsShader
			GLAD_API_CALL PFNGLISSYNCPROC glad_glIsSync;
			#define glIsSync glad_glIsSync
			GLAD_API_CALL PFNGLISTEXTUREPROC glad_glIsTexture;
			#define glIsTexture glad_glIsTexture
			GLAD_API_CALL PFNGLISVERTEXARRAYPROC glad_glIsVertexArray;
			#define glIsVertexArray glad_glIsVertexArray
			GLAD_API_CALL PFNGLLINEWIDTHPROC glad_glLineWidth;
			#define glLineWidth glad_glLineWidth
			GLAD_API_CALL PFNGLLINKPROGRAMPROC glad_glLinkProgram;
			#define glLinkProgram glad_glLinkProgram
			GLAD_API_CALL PFNGLLOGICOPPROC glad_glLogicOp;
			#define glLogicOp glad_glLogicOp
			GLAD_API_CALL PFNGLMAPBUFFERPROC glad_glMapBuffer;
			#define glMapBuffer glad_glMapBuffer
			GLAD_API_CALL PFNGLMAPBUFFERRANGEPROC glad_glMapBufferRange;
			#define glMapBufferRange glad_glMapBufferRange
			GLAD_API_CALL PFNGLMULTIDRAWARRAYSPROC glad_glMultiDrawArrays;
			#define glMultiDrawArrays glad_glMultiDrawArrays
			GLAD_API_CALL PFNGLMULTIDRAWELEMENTSPROC glad_glMultiDrawElements;
			#define glMultiDrawElements glad_glMultiDrawElements
			GLAD_API_CALL PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC glad_glMultiDrawElementsBaseVertex;
			#define glMultiDrawElementsBaseVertex glad_glMultiDrawElementsBaseVertex
			GLAD_API_CALL PFNGLPIXELSTOREFPROC glad_glPixelStoref;
			#define glPixelStoref glad_glPixelStoref
			GLAD_API_CALL PFNGLPIXELSTOREIPROC glad_glPixelStorei;
			#define glPixelStorei glad_glPixelStorei
			GLAD_API_CALL PFNGLPOINTPARAMETERFPROC glad_glPointParameterf;
			#define glPointParameterf glad_glPointParameterf
			GLAD_API_CALL PFNGLPOINTPARAMETERFVPROC glad_glPointParameterfv;
			#define glPointParameterfv glad_glPointParameterfv
			GLAD_API_CALL PFNGLPOINTPARAMETERIPROC glad_glPointParameteri;
			#define glPointParameteri glad_glPointParameteri
			GLAD_API_CALL PFNGLPOINTPARAMETERIVPROC glad_glPointParameteriv;
			#define glPointParameteriv glad_glPointParameteriv
			GLAD_API_CALL PFNGLPOINTSIZEPROC glad_glPointSize;
			#define glPointSize glad_glPointSize
			GLAD_API_CALL PFNGLPOLYGONMODEPROC glad_glPolygonMode;
			#define glPolygonMode glad_glPolygonMode
			GLAD_API_CALL PFNGLPOLYGONOFFSETPROC glad_glPolygonOffset;
			#define glPolygonOffset glad_glPolygonOffset
			GLAD_API_CALL PFNGLPRIMITIVERESTARTINDEXPROC glad_glPrimitiveRestartIndex;
			#define glPrimitiveRestartIndex glad_glPrimitiveRestartIndex
			GLAD_API_CALL PFNGLPROVOKINGVERTEXPROC glad_glProvokingVertex;
			#define glProvokingVertex glad_glProvokingVertex
			GLAD_API_CALL PFNGLQUERYCOUNTERPROC glad_glQueryCounter;
			#define glQueryCounter glad_glQueryCounter
			GLAD_API_CALL PFNGLREADBUFFERPROC glad_glReadBuffer;
			#define glReadBuffer glad_glReadBuffer
			GLAD_API_CALL PFNGLREADPIXELSPROC glad_glReadPixels;
			#define glReadPixels glad_glReadPixels
			GLAD_API_CALL PFNGLRENDERBUFFERSTORAGEPROC glad_glRenderbufferStorage;
			#define glRenderbufferStorage glad_glRenderbufferStorage
			GLAD_API_CALL PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC glad_glRenderbufferStorageMultisample;
			#define glRenderbufferStorageMultisample glad_glRenderbufferStorageMultisample
			GLAD_API_CALL PFNGLSAMPLECOVERAGEPROC glad_glSampleCoverage;
			#define glSampleCoverage glad_glSampleCoverage
			GLAD_API_CALL PFNGLSAMPLEMASKIPROC glad_glSampleMaski;
			#define glSampleMaski glad_glSampleMaski
			GLAD_API_CALL PFNGLSAMPLERPARAMETERIIVPROC glad_glSamplerParameterIiv;
			#define glSamplerParameterIiv glad_glSamplerParameterIiv
			GLAD_API_CALL PFNGLSAMPLERPARAMETERIUIVPROC glad_glSamplerParameterIuiv;
			#define glSamplerParameterIuiv glad_glSamplerParameterIuiv
			GLAD_API_CALL PFNGLSAMPLERPARAMETERFPROC glad_glSamplerParameterf;
			#define glSamplerParameterf glad_glSamplerParameterf
			GLAD_API_CALL PFNGLSAMPLERPARAMETERFVPROC glad_glSamplerParameterfv;
			#define glSamplerParameterfv glad_glSamplerParameterfv
			GLAD_API_CALL PFNGLSAMPLERPARAMETERIPROC glad_glSamplerParameteri;
			#define glSamplerParameteri glad_glSamplerParameteri
			GLAD_API_CALL PFNGLSAMPLERPARAMETERIVPROC glad_glSamplerParameteriv;
			#define glSamplerParameteriv glad_glSamplerParameteriv
			GLAD_API_CALL PFNGLSCISSORPROC glad_glScissor;
			#define glScissor glad_glScissor
			GLAD_API_CALL PFNGLSHADERSOURCEPROC glad_glShaderSource;
			#define glShaderSource glad_glShaderSource
			GLAD_API_CALL PFNGLSTENCILFUNCPROC glad_glStencilFunc;
			#define glStencilFunc glad_glStencilFunc
			GLAD_API_CALL PFNGLSTENCILFUNCSEPARATEPROC glad_glStencilFuncSeparate;
			#define glStencilFuncSeparate glad_glStencilFuncSeparate
			GLAD_API_CALL PFNGLSTENCILMASKPROC glad_glStencilMask;
			#define glStencilMask glad_glStencilMask
			GLAD_API_CALL PFNGLSTENCILMASKSEPARATEPROC glad_glStencilMaskSeparate;
			#define glStencilMaskSeparate glad_glStencilMaskSeparate
			GLAD_API_CALL PFNGLSTENCILOPPROC glad_glStencilOp;
			#define glStencilOp glad_glStencilOp
			GLAD_API_CALL PFNGLSTENCILOPSEPARATEPROC glad_glStencilOpSeparate;
			#define glStencilOpSeparate glad_glStencilOpSeparate
			GLAD_API_CALL PFNGLTEXBUFFERPROC glad_glTexBuffer;
			#define glTexBuffer glad_glTexBuffer
			GLAD_API_CALL PFNGLTEXIMAGE1DPROC glad_glTexImage1D;
			#define glTexImage1D glad_glTexImage1D
			GLAD_API_CALL PFNGLTEXIMAGE2DPROC glad_glTexImage2D;
			#define glTexImage2D glad_glTexImage2D
			GLAD_API_CALL PFNGLTEXIMAGE2DMULTISAMPLEPROC glad_glTexImage2DMultisample;
			#define glTexImage2DMultisample glad_glTexImage2DMultisample
			GLAD_API_CALL PFNGLTEXIMAGE3DPROC glad_glTexImage3D;
			#define glTexImage3D glad_glTexImage3D
			GLAD_API_CALL PFNGLTEXIMAGE3DMULTISAMPLEPROC glad_glTexImage3DMultisample;
			#define glTexImage3DMultisample glad_glTexImage3DMultisample
			GLAD_API_CALL PFNGLTEXPARAMETERIIVPROC glad_glTexParameterIiv;
			#define glTexParameterIiv glad_glTexParameterIiv
			GLAD_API_CALL PFNGLTEXPARAMETERIUIVPROC glad_glTexParameterIuiv;
			#define glTexParameterIuiv glad_glTexParameterIuiv
			GLAD_API_CALL PFNGLTEXPARAMETERFPROC glad_glTexParameterf;
			#define glTexParameterf glad_glTexParameterf
			GLAD_API_CALL PFNGLTEXPARAMETERFVPROC glad_glTexParameterfv;
			#define glTexParameterfv glad_glTexParameterfv
			GLAD_API_CALL PFNGLTEXPARAMETERIPROC glad_glTexParameteri;
			#define glTexParameteri glad_glTexParameteri
			GLAD_API_CALL PFNGLTEXPARAMETERIVPROC glad_glTexParameteriv;
			#define glTexParameteriv glad_glTexParameteriv
			GLAD_API_CALL PFNGLTEXSUBIMAGE1DPROC glad_glTexSubImage1D;
			#define glTexSubImage1D glad_glTexSubImage1D
			GLAD_API_CALL PFNGLTEXSUBIMAGE2DPROC glad_glTexSubImage2D;
			#define glTexSubImage2D glad_glTexSubImage2D
			GLAD_API_CALL PFNGLTEXSUBIMAGE3DPROC glad_glTexSubImage3D;
			#define glTexSubImage3D glad_glTexSubImage3D
			GLAD_API_CALL PFNGLTRANSFORMFEEDBACKVARYINGSPROC glad_glTransformFeedbackVaryings;
			#define glTransformFeedbackVaryings glad_glTransformFeedbackVaryings
			GLAD_API_CALL PFNGLUNIFORM1FPROC glad_glUniform1f;
			#define glUniform1f glad_glUniform1f
			GLAD_API_CALL PFNGLUNIFORM1FVPROC glad_glUniform1fv;
			#define glUniform1fv glad_glUniform1fv
			GLAD_API_CALL PFNGLUNIFORM1IPROC glad_glUniform1i;
			#define glUniform1i glad_glUniform1i
			GLAD_API_CALL PFNGLUNIFORM1IVPROC glad_glUniform1iv;
			#define glUniform1iv glad_glUniform1iv
			GLAD_API_CALL PFNGLUNIFORM1UIPROC glad_glUniform1ui;
			#define glUniform1ui glad_glUniform1ui
			GLAD_API_CALL PFNGLUNIFORM1UIVPROC glad_glUniform1uiv;
			#define glUniform1uiv glad_glUniform1uiv
			GLAD_API_CALL PFNGLUNIFORM2FPROC glad_glUniform2f;
			#define glUniform2f glad_glUniform2f
			GLAD_API_CALL PFNGLUNIFORM2FVPROC glad_glUniform2fv;
			#define glUniform2fv glad_glUniform2fv
			GLAD_API_CALL PFNGLUNIFORM2IPROC glad_glUniform2i;
			#define glUniform2i glad_glUniform2i
			GLAD_API_CALL PFNGLUNIFORM2IVPROC glad_glUniform2iv;
			#define glUniform2iv glad_glUniform2iv
			GLAD_API_CALL PFNGLUNIFORM2UIPROC glad_glUniform2ui;
			#define glUniform2ui glad_glUniform2ui
			GLAD_API_CALL PFNGLUNIFORM2UIVPROC glad_glUniform2uiv;
			#define glUniform2uiv glad_glUniform2uiv
			GLAD_API_CALL PFNGLUNIFORM3FPROC glad_glUniform3f;
			#define glUniform3f glad_glUniform3f
			GLAD_API_CALL PFNGLUNIFORM3FVPROC glad_glUniform3fv;
			#define glUniform3fv glad_glUniform3fv
			GLAD_API_CALL PFNGLUNIFORM3IPROC glad_glUniform3i;
			#define glUniform3i glad_glUniform3i
			GLAD_API_CALL PFNGLUNIFORM3IVPROC glad_glUniform3iv;
			#define glUniform3iv glad_glUniform3iv
			GLAD_API_CALL PFNGLUNIFORM3UIPROC glad_glUniform3ui;
			#define glUniform3ui glad_glUniform3ui
			GLAD_API_CALL PFNGLUNIFORM3UIVPROC glad_glUniform3uiv;
			#define glUniform3uiv glad_glUniform3uiv
			GLAD_API_CALL PFNGLUNIFORM4FPROC glad_glUniform4f;
			#define glUniform4f glad_glUniform4f
			GLAD_API_CALL PFNGLUNIFORM4FVPROC glad_glUniform4fv;
			#define glUniform4fv glad_glUniform4fv
			GLAD_API_CALL PFNGLUNIFORM4IPROC glad_glUniform4i;
			#define glUniform4i glad_glUniform4i
			GLAD_API_CALL PFNGLUNIFORM4IVPROC glad_glUniform4iv;
			#define glUniform4iv glad_glUniform4iv
			GLAD_API_CALL PFNGLUNIFORM4UIPROC glad_glUniform4ui;
			#define glUniform4ui glad_glUniform4ui
			GLAD_API_CALL PFNGLUNIFORM4UIVPROC glad_glUniform4uiv;
			#define glUniform4uiv glad_glUniform4uiv
			GLAD_API_CALL PFNGLUNIFORMBLOCKBINDINGPROC glad_glUniformBlockBinding;
			#define glUniformBlockBinding glad_glUniformBlockBinding
			GLAD_API_CALL PFNGLUNIFORMMATRIX2FVPROC glad_glUniformMatrix2fv;
			#define glUniformMatrix2fv glad_glUniformMatrix2fv
			GLAD_API_CALL PFNGLUNIFORMMATRIX2X3FVPROC glad_glUniformMatrix2x3fv;
			#define glUniformMatrix2x3fv glad_glUniformMatrix2x3fv
			GLAD_API_CALL PFNGLUNIFORMMATRIX2X4FVPROC glad_glUniformMatrix2x4fv;
			#define glUniformMatrix2x4fv glad_glUniformMatrix2x4fv
			GLAD_API_CALL PFNGLUNIFORMMATRIX3FVPROC glad_glUniformMatrix3fv;
			#define glUniformMatrix3fv glad_glUniformMatrix3fv
			GLAD_API_CALL PFNGLUNIFORMMATRIX3X2FVPROC glad_glUniformMatrix3x2fv;
			#define glUniformMatrix3x2fv glad_glUniformMatrix3x2fv
			GLAD_API_CALL PFNGLUNIFORMMATRIX3X4FVPROC glad_glUniformMatrix3x4fv;
			#define glUniformMatrix3x4fv glad_glUniformMatrix3x4fv
			GLAD_API_CALL PFNGLUNIFORMMATRIX4FVPROC glad_glUniformMatrix4fv;
			#define glUniformMatrix4fv glad_glUniformMatrix4fv
			GLAD_API_CALL PFNGLUNIFORMMATRIX4X2FVPROC glad_glUniformMatrix4x2fv;
			#define glUniformMatrix4x2fv glad_glUniformMatrix4x2fv
			GLAD_API_CALL PFNGLUNIFORMMATRIX4X3FVPROC glad_glUniformMatrix4x3fv;
			#define glUniformMatrix4x3fv glad_glUniformMatrix4x3fv
			GLAD_API_CALL PFNGLUNMAPBUFFERPROC glad_glUnmapBuffer;
			#define glUnmapBuffer glad_glUnmapBuffer
			GLAD_API_CALL PFNGLUSEPROGRAMPROC glad_glUseProgram;
			#define glUseProgram glad_glUseProgram
			GLAD_API_CALL PFNGLVALIDATEPROGRAMPROC glad_glValidateProgram;
			#define glValidateProgram glad_glValidateProgram
			GLAD_API_CALL PFNGLVERTEXATTRIB1DPROC glad_glVertexAttrib1d;
			#define glVertexAttrib1d glad_glVertexAttrib1d
			GLAD_API_CALL PFNGLVERTEXATTRIB1DVPROC glad_glVertexAttrib1dv;
			#define glVertexAttrib1dv glad_glVertexAttrib1dv
			GLAD_API_CALL PFNGLVERTEXATTRIB1FPROC glad_glVertexAttrib1f;
			#define glVertexAttrib1f glad_glVertexAttrib1f
			GLAD_API_CALL PFNGLVERTEXATTRIB1FVPROC glad_glVertexAttrib1fv;
			#define glVertexAttrib1fv glad_glVertexAttrib1fv
			GLAD_API_CALL PFNGLVERTEXATTRIB1SPROC glad_glVertexAttrib1s;
			#define glVertexAttrib1s glad_glVertexAttrib1s
			GLAD_API_CALL PFNGLVERTEXATTRIB1SVPROC glad_glVertexAttrib1sv;
			#define glVertexAttrib1sv glad_glVertexAttrib1sv
			GLAD_API_CALL PFNGLVERTEXATTRIB2DPROC glad_glVertexAttrib2d;
			#define glVertexAttrib2d glad_glVertexAttrib2d
			GLAD_API_CALL PFNGLVERTEXATTRIB2DVPROC glad_glVertexAttrib2dv;
			#define glVertexAttrib2dv glad_glVertexAttrib2dv
			GLAD_API_CALL PFNGLVERTEXATTRIB2FPROC glad_glVertexAttrib2f;
			#define glVertexAttrib2f glad_glVertexAttrib2f
			GLAD_API_CALL PFNGLVERTEXATTRIB2FVPROC glad_glVertexAttrib2fv;
			#define glVertexAttrib2fv glad_glVertexAttrib2fv
			GLAD_API_CALL PFNGLVERTEXATTRIB2SPROC glad_glVertexAttrib2s;
			#define glVertexAttrib2s glad_glVertexAttrib2s
			GLAD_API_CALL PFNGLVERTEXATTRIB2SVPROC glad_glVertexAttrib2sv;
			#define glVertexAttrib2sv glad_glVertexAttrib2sv
			GLAD_API_CALL PFNGLVERTEXATTRIB3DPROC glad_glVertexAttrib3d;
			#define glVertexAttrib3d glad_glVertexAttrib3d
			GLAD_API_CALL PFNGLVERTEXATTRIB3DVPROC glad_glVertexAttrib3dv;
			#define glVertexAttrib3dv glad_glVertexAttrib3dv
			GLAD_API_CALL PFNGLVERTEXATTRIB3FPROC glad_glVertexAttrib3f;
			#define glVertexAttrib3f glad_glVertexAttrib3f
			GLAD_API_CALL PFNGLVERTEXATTRIB3FVPROC glad_glVertexAttrib3fv;
			#define glVertexAttrib3fv glad_glVertexAttrib3fv
			GLAD_API_CALL PFNGLVERTEXATTRIB3SPROC glad_glVertexAttrib3s;
			#define glVertexAttrib3s glad_glVertexAttrib3s
			GLAD_API_CALL PFNGLVERTEXATTRIB3SVPROC glad_glVertexAttrib3sv;
			#define glVertexAttrib3sv glad_glVertexAttrib3sv
			GLAD_API_CALL PFNGLVERTEXATTRIB4NBVPROC glad_glVertexAttrib4Nbv;
			#define glVertexAttrib4Nbv glad_glVertexAttrib4Nbv
			GLAD_API_CALL PFNGLVERTEXATTRIB4NIVPROC glad_glVertexAttrib4Niv;
			#define glVertexAttrib4Niv glad_glVertexAttrib4Niv
			GLAD_API_CALL PFNGLVERTEXATTRIB4NSVPROC glad_glVertexAttrib4Nsv;
			#define glVertexAttrib4Nsv glad_glVertexAttrib4Nsv
			GLAD_API_CALL PFNGLVERTEXATTRIB4NUBPROC glad_glVertexAttrib4Nub;
			#define glVertexAttrib4Nub glad_glVertexAttrib4Nub
			GLAD_API_CALL PFNGLVERTEXATTRIB4NUBVPROC glad_glVertexAttrib4Nubv;
			#define glVertexAttrib4Nubv glad_glVertexAttrib4Nubv
			GLAD_API_CALL PFNGLVERTEXATTRIB4NUIVPROC glad_glVertexAttrib4Nuiv;
			#define glVertexAttrib4Nuiv glad_glVertexAttrib4Nuiv
			GLAD_API_CALL PFNGLVERTEXATTRIB4NUSVPROC glad_glVertexAttrib4Nusv;
			#define glVertexAttrib4Nusv glad_glVertexAttrib4Nusv
			GLAD_API_CALL PFNGLVERTEXATTRIB4BVPROC glad_glVertexAttrib4bv;
			#define glVertexAttrib4bv glad_glVertexAttrib4bv
			GLAD_API_CALL PFNGLVERTEXATTRIB4DPROC glad_glVertexAttrib4d;
			#define glVertexAttrib4d glad_glVertexAttrib4d
			GLAD_API_CALL PFNGLVERTEXATTRIB4DVPROC glad_glVertexAttrib4dv;
			#define glVertexAttrib4dv glad_glVertexAttrib4dv
			GLAD_API_CALL PFNGLVERTEXATTRIB4FPROC glad_glVertexAttrib4f;
			#define glVertexAttrib4f glad_glVertexAttrib4f
			GLAD_API_CALL PFNGLVERTEXATTRIB4FVPROC glad_glVertexAttrib4fv;
			#define glVertexAttrib4fv glad_glVertexAttrib4fv
			GLAD_API_CALL PFNGLVERTEXATTRIB4IVPROC glad_glVertexAttrib4iv;
			#define glVertexAttrib4iv glad_glVertexAttrib4iv
			GLAD_API_CALL PFNGLVERTEXATTRIB4SPROC glad_glVertexAttrib4s;
			#define glVertexAttrib4s glad_glVertexAttrib4s
			GLAD_API_CALL PFNGLVERTEXATTRIB4SVPROC glad_glVertexAttrib4sv;
			#define glVertexAttrib4sv glad_glVertexAttrib4sv
			GLAD_API_CALL PFNGLVERTEXATTRIB4UBVPROC glad_glVertexAttrib4ubv;
			#define glVertexAttrib4ubv glad_glVertexAttrib4ubv
			GLAD_API_CALL PFNGLVERTEXATTRIB4UIVPROC glad_glVertexAttrib4uiv;
			#define glVertexAttrib4uiv glad_glVertexAttrib4uiv
			GLAD_API_CALL PFNGLVERTEXATTRIB4USVPROC glad_glVertexAttrib4usv;
			#define glVertexAttrib4usv glad_glVertexAttrib4usv
			GLAD_API_CALL PFNGLVERTEXATTRIBDIVISORPROC glad_glVertexAttribDivisor;
			#define glVertexAttribDivisor glad_glVertexAttribDivisor
			GLAD_API_CALL PFNGLVERTEXATTRIBI1IPROC glad_glVertexAttribI1i;
			#define glVertexAttribI1i glad_glVertexAttribI1i
			GLAD_API_CALL PFNGLVERTEXATTRIBI1IVPROC glad_glVertexAttribI1iv;
			#define glVertexAttribI1iv glad_glVertexAttribI1iv
			GLAD_API_CALL PFNGLVERTEXATTRIBI1UIPROC glad_glVertexAttribI1ui;
			#define glVertexAttribI1ui glad_glVertexAttribI1ui
			GLAD_API_CALL PFNGLVERTEXATTRIBI1UIVPROC glad_glVertexAttribI1uiv;
			#define glVertexAttribI1uiv glad_glVertexAttribI1uiv
			GLAD_API_CALL PFNGLVERTEXATTRIBI2IPROC glad_glVertexAttribI2i;
			#define glVertexAttribI2i glad_glVertexAttribI2i
			GLAD_API_CALL PFNGLVERTEXATTRIBI2IVPROC glad_glVertexAttribI2iv;
			#define glVertexAttribI2iv glad_glVertexAttribI2iv
			GLAD_API_CALL PFNGLVERTEXATTRIBI2UIPROC glad_glVertexAttribI2ui;
			#define glVertexAttribI2ui glad_glVertexAttribI2ui
			GLAD_API_CALL PFNGLVERTEXATTRIBI2UIVPROC glad_glVertexAttribI2uiv;
			#define glVertexAttribI2uiv glad_glVertexAttribI2uiv
			GLAD_API_CALL PFNGLVERTEXATTRIBI3IPROC glad_glVertexAttribI3i;
			#define glVertexAttribI3i glad_glVertexAttribI3i
			GLAD_API_CALL PFNGLVERTEXATTRIBI3IVPROC glad_glVertexAttribI3iv;
			#define glVertexAttribI3iv glad_glVertexAttribI3iv
			GLAD_API_CALL PFNGLVERTEXATTRIBI3UIPROC glad_glVertexAttribI3ui;
			#define glVertexAttribI3ui glad_glVertexAttribI3ui
			GLAD_API_CALL PFNGLVERTEXATTRIBI3UIVPROC glad_glVertexAttribI3uiv;
			#define glVertexAttribI3uiv glad_glVertexAttribI3uiv
			GLAD_API_CALL PFNGLVERTEXATTRIBI4BVPROC glad_glVertexAttribI4bv;
			#define glVertexAttribI4bv glad_glVertexAttribI4bv
			GLAD_API_CALL PFNGLVERTEXATTRIBI4IPROC glad_glVertexAttribI4i;
			#define glVertexAttribI4i glad_glVertexAttribI4i
			GLAD_API_CALL PFNGLVERTEXATTRIBI4IVPROC glad_glVertexAttribI4iv;
			#define glVertexAttribI4iv glad_glVertexAttribI4iv
			GLAD_API_CALL PFNGLVERTEXATTRIBI4SVPROC glad_glVertexAttribI4sv;
			#define glVertexAttribI4sv glad_glVertexAttribI4sv
			GLAD_API_CALL PFNGLVERTEXATTRIBI4UBVPROC glad_glVertexAttribI4ubv;
			#define glVertexAttribI4ubv glad_glVertexAttribI4ubv
			GLAD_API_CALL PFNGLVERTEXATTRIBI4UIPROC glad_glVertexAttribI4ui;
			#define glVertexAttribI4ui glad_glVertexAttribI4ui
			GLAD_API_CALL PFNGLVERTEXATTRIBI4UIVPROC glad_glVertexAttribI4uiv;
			#define glVertexAttribI4uiv glad_glVertexAttribI4uiv
			GLAD_API_CALL PFNGLVERTEXATTRIBI4USVPROC glad_glVertexAttribI4usv;
			#define glVertexAttribI4usv glad_glVertexAttribI4usv
			GLAD_API_CALL PFNGLVERTEXATTRIBIPOINTERPROC glad_glVertexAttribIPointer;
			#define glVertexAttribIPointer glad_glVertexAttribIPointer
			GLAD_API_CALL PFNGLVERTEXATTRIBP1UIPROC glad_glVertexAttribP1ui;
			#define glVertexAttribP1ui glad_glVertexAttribP1ui
			GLAD_API_CALL PFNGLVERTEXATTRIBP1UIVPROC glad_glVertexAttribP1uiv;
			#define glVertexAttribP1uiv glad_glVertexAttribP1uiv
			GLAD_API_CALL PFNGLVERTEXATTRIBP2UIPROC glad_glVertexAttribP2ui;
			#define glVertexAttribP2ui glad_glVertexAttribP2ui
			GLAD_API_CALL PFNGLVERTEXATTRIBP2UIVPROC glad_glVertexAttribP2uiv;
			#define glVertexAttribP2uiv glad_glVertexAttribP2uiv
			GLAD_API_CALL PFNGLVERTEXATTRIBP3UIPROC glad_glVertexAttribP3ui;
			#define glVertexAttribP3ui glad_glVertexAttribP3ui
			GLAD_API_CALL PFNGLVERTEXATTRIBP3UIVPROC glad_glVertexAttribP3uiv;
			#define glVertexAttribP3uiv glad_glVertexAttribP3uiv
			GLAD_API_CALL PFNGLVERTEXATTRIBP4UIPROC glad_glVertexAttribP4ui;
			#define glVertexAttribP4ui glad_glVertexAttribP4ui
			GLAD_API_CALL PFNGLVERTEXATTRIBP4UIVPROC glad_glVertexAttribP4uiv;
			#define glVertexAttribP4uiv glad_glVertexAttribP4uiv
			GLAD_API_CALL PFNGLVERTEXATTRIBPOINTERPROC glad_glVertexAttribPointer;
			#define glVertexAttribPointer glad_glVertexAttribPointer
			GLAD_API_CALL PFNGLVIEWPORTPROC glad_glViewport;
			#define glViewport glad_glViewport
			GLAD_API_CALL PFNGLWAITSYNCPROC glad_glWaitSync;
			#define glWaitSync glad_glWaitSync





			GLAD_API_CALL int gladLoadGLUserPtr( GLADuserptrloadfunc load, void *userptr);
			GLAD_API_CALL int gladLoadGL( GLADloadfunc load);


			#ifdef GLAD_GL

			GLAD_API_CALL int gladLoaderLoadGL(void);
			GLAD_API_CALL void gladLoaderUnloadGL(void);

			#endif

			#ifdef __cplusplus
			}
			#endif
		#endif
		//#ifdef GLAD_GL_IMPLEMENTATION
			/**
			 * SPDX-License-Identifier: (WTFPL OR CC0-1.0) AND Apache-2.0
			 */
			#include <stdio.h>
			#include <stdlib.h>
			#include <string.h>

			#ifndef GLAD_IMPL_UTIL_C_
			#define GLAD_IMPL_UTIL_C_

			#ifdef _MSC_VER
			#define GLAD_IMPL_UTIL_SSCANF sscanf_s
			#else
			#define GLAD_IMPL_UTIL_SSCANF sscanf
			#endif

			#endif /* GLAD_IMPL_UTIL_C_ */

			#ifdef __cplusplus
			extern "C" {
			#endif



			int GLAD_GL_VERSION_1_0 = 0;
			int GLAD_GL_VERSION_1_1 = 0;
			int GLAD_GL_VERSION_1_2 = 0;
			int GLAD_GL_VERSION_1_3 = 0;
			int GLAD_GL_VERSION_1_4 = 0;
			int GLAD_GL_VERSION_1_5 = 0;
			int GLAD_GL_VERSION_2_0 = 0;
			int GLAD_GL_VERSION_2_1 = 0;
			int GLAD_GL_VERSION_3_0 = 0;
			int GLAD_GL_VERSION_3_1 = 0;
			int GLAD_GL_VERSION_3_2 = 0;
			int GLAD_GL_VERSION_3_3 = 0;



			PFNGLACTIVETEXTUREPROC glad_glActiveTexture = NULL;
			PFNGLATTACHSHADERPROC glad_glAttachShader = NULL;
			PFNGLBEGINCONDITIONALRENDERPROC glad_glBeginConditionalRender = NULL;
			PFNGLBEGINQUERYPROC glad_glBeginQuery = NULL;
			PFNGLBEGINTRANSFORMFEEDBACKPROC glad_glBeginTransformFeedback = NULL;
			PFNGLBINDATTRIBLOCATIONPROC glad_glBindAttribLocation = NULL;
			PFNGLBINDBUFFERPROC glad_glBindBuffer = NULL;
			PFNGLBINDBUFFERBASEPROC glad_glBindBufferBase = NULL;
			PFNGLBINDBUFFERRANGEPROC glad_glBindBufferRange = NULL;
			PFNGLBINDFRAGDATALOCATIONPROC glad_glBindFragDataLocation = NULL;
			PFNGLBINDFRAGDATALOCATIONINDEXEDPROC glad_glBindFragDataLocationIndexed = NULL;
			PFNGLBINDFRAMEBUFFERPROC glad_glBindFramebuffer = NULL;
			PFNGLBINDRENDERBUFFERPROC glad_glBindRenderbuffer = NULL;
			PFNGLBINDSAMPLERPROC glad_glBindSampler = NULL;
			PFNGLBINDTEXTUREPROC glad_glBindTexture = NULL;
			PFNGLBINDVERTEXARRAYPROC glad_glBindVertexArray = NULL;
			PFNGLBLENDCOLORPROC glad_glBlendColor = NULL;
			PFNGLBLENDEQUATIONPROC glad_glBlendEquation = NULL;
			PFNGLBLENDEQUATIONSEPARATEPROC glad_glBlendEquationSeparate = NULL;
			PFNGLBLENDFUNCPROC glad_glBlendFunc = NULL;
			PFNGLBLENDFUNCSEPARATEPROC glad_glBlendFuncSeparate = NULL;
			PFNGLBLITFRAMEBUFFERPROC glad_glBlitFramebuffer = NULL;
			PFNGLBUFFERDATAPROC glad_glBufferData = NULL;
			PFNGLBUFFERSUBDATAPROC glad_glBufferSubData = NULL;
			PFNGLCHECKFRAMEBUFFERSTATUSPROC glad_glCheckFramebufferStatus = NULL;
			PFNGLCLAMPCOLORPROC glad_glClampColor = NULL;
			PFNGLCLEARPROC glad_glClear = NULL;
			PFNGLCLEARBUFFERFIPROC glad_glClearBufferfi = NULL;
			PFNGLCLEARBUFFERFVPROC glad_glClearBufferfv = NULL;
			PFNGLCLEARBUFFERIVPROC glad_glClearBufferiv = NULL;
			PFNGLCLEARBUFFERUIVPROC glad_glClearBufferuiv = NULL;
			PFNGLCLEARCOLORPROC glad_glClearColor = NULL;
			PFNGLCLEARDEPTHPROC glad_glClearDepth = NULL;
			PFNGLCLEARSTENCILPROC glad_glClearStencil = NULL;
			PFNGLCLIENTWAITSYNCPROC glad_glClientWaitSync = NULL;
			PFNGLCOLORMASKPROC glad_glColorMask = NULL;
			PFNGLCOLORMASKIPROC glad_glColorMaski = NULL;
			PFNGLCOMPILESHADERPROC glad_glCompileShader = NULL;
			PFNGLCOMPRESSEDTEXIMAGE1DPROC glad_glCompressedTexImage1D = NULL;
			PFNGLCOMPRESSEDTEXIMAGE2DPROC glad_glCompressedTexImage2D = NULL;
			PFNGLCOMPRESSEDTEXIMAGE3DPROC glad_glCompressedTexImage3D = NULL;
			PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC glad_glCompressedTexSubImage1D = NULL;
			PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC glad_glCompressedTexSubImage2D = NULL;
			PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC glad_glCompressedTexSubImage3D = NULL;
			PFNGLCOPYBUFFERSUBDATAPROC glad_glCopyBufferSubData = NULL;
			PFNGLCOPYTEXIMAGE1DPROC glad_glCopyTexImage1D = NULL;
			PFNGLCOPYTEXIMAGE2DPROC glad_glCopyTexImage2D = NULL;
			PFNGLCOPYTEXSUBIMAGE1DPROC glad_glCopyTexSubImage1D = NULL;
			PFNGLCOPYTEXSUBIMAGE2DPROC glad_glCopyTexSubImage2D = NULL;
			PFNGLCOPYTEXSUBIMAGE3DPROC glad_glCopyTexSubImage3D = NULL;
			PFNGLCREATEPROGRAMPROC glad_glCreateProgram = NULL;
			PFNGLCREATESHADERPROC glad_glCreateShader = NULL;
			PFNGLCULLFACEPROC glad_glCullFace = NULL;
			PFNGLDELETEBUFFERSPROC glad_glDeleteBuffers = NULL;
			PFNGLDELETEFRAMEBUFFERSPROC glad_glDeleteFramebuffers = NULL;
			PFNGLDELETEPROGRAMPROC glad_glDeleteProgram = NULL;
			PFNGLDELETEQUERIESPROC glad_glDeleteQueries = NULL;
			PFNGLDELETERENDERBUFFERSPROC glad_glDeleteRenderbuffers = NULL;
			PFNGLDELETESAMPLERSPROC glad_glDeleteSamplers = NULL;
			PFNGLDELETESHADERPROC glad_glDeleteShader = NULL;
			PFNGLDELETESYNCPROC glad_glDeleteSync = NULL;
			PFNGLDELETETEXTURESPROC glad_glDeleteTextures = NULL;
			PFNGLDELETEVERTEXARRAYSPROC glad_glDeleteVertexArrays = NULL;
			PFNGLDEPTHFUNCPROC glad_glDepthFunc = NULL;
			PFNGLDEPTHMASKPROC glad_glDepthMask = NULL;
			PFNGLDEPTHRANGEPROC glad_glDepthRange = NULL;
			PFNGLDETACHSHADERPROC glad_glDetachShader = NULL;
			PFNGLDISABLEPROC glad_glDisable = NULL;
			PFNGLDISABLEVERTEXATTRIBARRAYPROC glad_glDisableVertexAttribArray = NULL;
			PFNGLDISABLEIPROC glad_glDisablei = NULL;
			PFNGLDRAWARRAYSPROC glad_glDrawArrays = NULL;
			PFNGLDRAWARRAYSINSTANCEDPROC glad_glDrawArraysInstanced = NULL;
			PFNGLDRAWBUFFERPROC glad_glDrawBuffer = NULL;
			PFNGLDRAWBUFFERSPROC glad_glDrawBuffers = NULL;
			PFNGLDRAWELEMENTSPROC glad_glDrawElements = NULL;
			PFNGLDRAWELEMENTSBASEVERTEXPROC glad_glDrawElementsBaseVertex = NULL;
			PFNGLDRAWELEMENTSINSTANCEDPROC glad_glDrawElementsInstanced = NULL;
			PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC glad_glDrawElementsInstancedBaseVertex = NULL;
			PFNGLDRAWRANGEELEMENTSPROC glad_glDrawRangeElements = NULL;
			PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC glad_glDrawRangeElementsBaseVertex = NULL;
			PFNGLENABLEPROC glad_glEnable = NULL;
			PFNGLENABLEVERTEXATTRIBARRAYPROC glad_glEnableVertexAttribArray = NULL;
			PFNGLENABLEIPROC glad_glEnablei = NULL;
			PFNGLENDCONDITIONALRENDERPROC glad_glEndConditionalRender = NULL;
			PFNGLENDQUERYPROC glad_glEndQuery = NULL;
			PFNGLENDTRANSFORMFEEDBACKPROC glad_glEndTransformFeedback = NULL;
			PFNGLFENCESYNCPROC glad_glFenceSync = NULL;
			PFNGLFINISHPROC glad_glFinish = NULL;
			PFNGLFLUSHPROC glad_glFlush = NULL;
			PFNGLFLUSHMAPPEDBUFFERRANGEPROC glad_glFlushMappedBufferRange = NULL;
			PFNGLFRAMEBUFFERRENDERBUFFERPROC glad_glFramebufferRenderbuffer = NULL;
			PFNGLFRAMEBUFFERTEXTUREPROC glad_glFramebufferTexture = NULL;
			PFNGLFRAMEBUFFERTEXTURE1DPROC glad_glFramebufferTexture1D = NULL;
			PFNGLFRAMEBUFFERTEXTURE2DPROC glad_glFramebufferTexture2D = NULL;
			PFNGLFRAMEBUFFERTEXTURE3DPROC glad_glFramebufferTexture3D = NULL;
			PFNGLFRAMEBUFFERTEXTURELAYERPROC glad_glFramebufferTextureLayer = NULL;
			PFNGLFRONTFACEPROC glad_glFrontFace = NULL;
			PFNGLGENBUFFERSPROC glad_glGenBuffers = NULL;
			PFNGLGENFRAMEBUFFERSPROC glad_glGenFramebuffers = NULL;
			PFNGLGENQUERIESPROC glad_glGenQueries = NULL;
			PFNGLGENRENDERBUFFERSPROC glad_glGenRenderbuffers = NULL;
			PFNGLGENSAMPLERSPROC glad_glGenSamplers = NULL;
			PFNGLGENTEXTURESPROC glad_glGenTextures = NULL;
			PFNGLGENVERTEXARRAYSPROC glad_glGenVertexArrays = NULL;
			PFNGLGENERATEMIPMAPPROC glad_glGenerateMipmap = NULL;
			PFNGLGETACTIVEATTRIBPROC glad_glGetActiveAttrib = NULL;
			PFNGLGETACTIVEUNIFORMPROC glad_glGetActiveUniform = NULL;
			PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC glad_glGetActiveUniformBlockName = NULL;
			PFNGLGETACTIVEUNIFORMBLOCKIVPROC glad_glGetActiveUniformBlockiv = NULL;
			PFNGLGETACTIVEUNIFORMNAMEPROC glad_glGetActiveUniformName = NULL;
			PFNGLGETACTIVEUNIFORMSIVPROC glad_glGetActiveUniformsiv = NULL;
			PFNGLGETATTACHEDSHADERSPROC glad_glGetAttachedShaders = NULL;
			PFNGLGETATTRIBLOCATIONPROC glad_glGetAttribLocation = NULL;
			PFNGLGETBOOLEANI_VPROC glad_glGetBooleani_v = NULL;
			PFNGLGETBOOLEANVPROC glad_glGetBooleanv = NULL;
			PFNGLGETBUFFERPARAMETERI64VPROC glad_glGetBufferParameteri64v = NULL;
			PFNGLGETBUFFERPARAMETERIVPROC glad_glGetBufferParameteriv = NULL;
			PFNGLGETBUFFERPOINTERVPROC glad_glGetBufferPointerv = NULL;
			PFNGLGETBUFFERSUBDATAPROC glad_glGetBufferSubData = NULL;
			PFNGLGETCOMPRESSEDTEXIMAGEPROC glad_glGetCompressedTexImage = NULL;
			PFNGLGETDOUBLEVPROC glad_glGetDoublev = NULL;
			PFNGLGETERRORPROC glad_glGetError = NULL;
			PFNGLGETFLOATVPROC glad_glGetFloatv = NULL;
			PFNGLGETFRAGDATAINDEXPROC glad_glGetFragDataIndex = NULL;
			PFNGLGETFRAGDATALOCATIONPROC glad_glGetFragDataLocation = NULL;
			PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC glad_glGetFramebufferAttachmentParameteriv = NULL;
			PFNGLGETINTEGER64I_VPROC glad_glGetInteger64i_v = NULL;
			PFNGLGETINTEGER64VPROC glad_glGetInteger64v = NULL;
			PFNGLGETINTEGERI_VPROC glad_glGetIntegeri_v = NULL;
			PFNGLGETINTEGERVPROC glad_glGetIntegerv = NULL;
			PFNGLGETMULTISAMPLEFVPROC glad_glGetMultisamplefv = NULL;
			PFNGLGETPROGRAMINFOLOGPROC glad_glGetProgramInfoLog = NULL;
			PFNGLGETPROGRAMIVPROC glad_glGetProgramiv = NULL;
			PFNGLGETQUERYOBJECTI64VPROC glad_glGetQueryObjecti64v = NULL;
			PFNGLGETQUERYOBJECTIVPROC glad_glGetQueryObjectiv = NULL;
			PFNGLGETQUERYOBJECTUI64VPROC glad_glGetQueryObjectui64v = NULL;
			PFNGLGETQUERYOBJECTUIVPROC glad_glGetQueryObjectuiv = NULL;
			PFNGLGETQUERYIVPROC glad_glGetQueryiv = NULL;
			PFNGLGETRENDERBUFFERPARAMETERIVPROC glad_glGetRenderbufferParameteriv = NULL;
			PFNGLGETSAMPLERPARAMETERIIVPROC glad_glGetSamplerParameterIiv = NULL;
			PFNGLGETSAMPLERPARAMETERIUIVPROC glad_glGetSamplerParameterIuiv = NULL;
			PFNGLGETSAMPLERPARAMETERFVPROC glad_glGetSamplerParameterfv = NULL;
			PFNGLGETSAMPLERPARAMETERIVPROC glad_glGetSamplerParameteriv = NULL;
			PFNGLGETSHADERINFOLOGPROC glad_glGetShaderInfoLog = NULL;
			PFNGLGETSHADERSOURCEPROC glad_glGetShaderSource = NULL;
			PFNGLGETSHADERIVPROC glad_glGetShaderiv = NULL;
			PFNGLGETSTRINGPROC glad_glGetString = NULL;
			PFNGLGETSTRINGIPROC glad_glGetStringi = NULL;
			PFNGLGETSYNCIVPROC glad_glGetSynciv = NULL;
			PFNGLGETTEXIMAGEPROC glad_glGetTexImage = NULL;
			PFNGLGETTEXLEVELPARAMETERFVPROC glad_glGetTexLevelParameterfv = NULL;
			PFNGLGETTEXLEVELPARAMETERIVPROC glad_glGetTexLevelParameteriv = NULL;
			PFNGLGETTEXPARAMETERIIVPROC glad_glGetTexParameterIiv = NULL;
			PFNGLGETTEXPARAMETERIUIVPROC glad_glGetTexParameterIuiv = NULL;
			PFNGLGETTEXPARAMETERFVPROC glad_glGetTexParameterfv = NULL;
			PFNGLGETTEXPARAMETERIVPROC glad_glGetTexParameteriv = NULL;
			PFNGLGETTRANSFORMFEEDBACKVARYINGPROC glad_glGetTransformFeedbackVarying = NULL;
			PFNGLGETUNIFORMBLOCKINDEXPROC glad_glGetUniformBlockIndex = NULL;
			PFNGLGETUNIFORMINDICESPROC glad_glGetUniformIndices = NULL;
			PFNGLGETUNIFORMLOCATIONPROC glad_glGetUniformLocation = NULL;
			PFNGLGETUNIFORMFVPROC glad_glGetUniformfv = NULL;
			PFNGLGETUNIFORMIVPROC glad_glGetUniformiv = NULL;
			PFNGLGETUNIFORMUIVPROC glad_glGetUniformuiv = NULL;
			PFNGLGETVERTEXATTRIBIIVPROC glad_glGetVertexAttribIiv = NULL;
			PFNGLGETVERTEXATTRIBIUIVPROC glad_glGetVertexAttribIuiv = NULL;
			PFNGLGETVERTEXATTRIBPOINTERVPROC glad_glGetVertexAttribPointerv = NULL;
			PFNGLGETVERTEXATTRIBDVPROC glad_glGetVertexAttribdv = NULL;
			PFNGLGETVERTEXATTRIBFVPROC glad_glGetVertexAttribfv = NULL;
			PFNGLGETVERTEXATTRIBIVPROC glad_glGetVertexAttribiv = NULL;
			PFNGLHINTPROC glad_glHint = NULL;
			PFNGLISBUFFERPROC glad_glIsBuffer = NULL;
			PFNGLISENABLEDPROC glad_glIsEnabled = NULL;
			PFNGLISENABLEDIPROC glad_glIsEnabledi = NULL;
			PFNGLISFRAMEBUFFERPROC glad_glIsFramebuffer = NULL;
			PFNGLISPROGRAMPROC glad_glIsProgram = NULL;
			PFNGLISQUERYPROC glad_glIsQuery = NULL;
			PFNGLISRENDERBUFFERPROC glad_glIsRenderbuffer = NULL;
			PFNGLISSAMPLERPROC glad_glIsSampler = NULL;
			PFNGLISSHADERPROC glad_glIsShader = NULL;
			PFNGLISSYNCPROC glad_glIsSync = NULL;
			PFNGLISTEXTUREPROC glad_glIsTexture = NULL;
			PFNGLISVERTEXARRAYPROC glad_glIsVertexArray = NULL;
			PFNGLLINEWIDTHPROC glad_glLineWidth = NULL;
			PFNGLLINKPROGRAMPROC glad_glLinkProgram = NULL;
			PFNGLLOGICOPPROC glad_glLogicOp = NULL;
			PFNGLMAPBUFFERPROC glad_glMapBuffer = NULL;
			PFNGLMAPBUFFERRANGEPROC glad_glMapBufferRange = NULL;
			PFNGLMULTIDRAWARRAYSPROC glad_glMultiDrawArrays = NULL;
			PFNGLMULTIDRAWELEMENTSPROC glad_glMultiDrawElements = NULL;
			PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC glad_glMultiDrawElementsBaseVertex = NULL;
			PFNGLPIXELSTOREFPROC glad_glPixelStoref = NULL;
			PFNGLPIXELSTOREIPROC glad_glPixelStorei = NULL;
			PFNGLPOINTPARAMETERFPROC glad_glPointParameterf = NULL;
			PFNGLPOINTPARAMETERFVPROC glad_glPointParameterfv = NULL;
			PFNGLPOINTPARAMETERIPROC glad_glPointParameteri = NULL;
			PFNGLPOINTPARAMETERIVPROC glad_glPointParameteriv = NULL;
			PFNGLPOINTSIZEPROC glad_glPointSize = NULL;
			PFNGLPOLYGONMODEPROC glad_glPolygonMode = NULL;
			PFNGLPOLYGONOFFSETPROC glad_glPolygonOffset = NULL;
			PFNGLPRIMITIVERESTARTINDEXPROC glad_glPrimitiveRestartIndex = NULL;
			PFNGLPROVOKINGVERTEXPROC glad_glProvokingVertex = NULL;
			PFNGLQUERYCOUNTERPROC glad_glQueryCounter = NULL;
			PFNGLREADBUFFERPROC glad_glReadBuffer = NULL;
			PFNGLREADPIXELSPROC glad_glReadPixels = NULL;
			PFNGLRENDERBUFFERSTORAGEPROC glad_glRenderbufferStorage = NULL;
			PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC glad_glRenderbufferStorageMultisample = NULL;
			PFNGLSAMPLECOVERAGEPROC glad_glSampleCoverage = NULL;
			PFNGLSAMPLEMASKIPROC glad_glSampleMaski = NULL;
			PFNGLSAMPLERPARAMETERIIVPROC glad_glSamplerParameterIiv = NULL;
			PFNGLSAMPLERPARAMETERIUIVPROC glad_glSamplerParameterIuiv = NULL;
			PFNGLSAMPLERPARAMETERFPROC glad_glSamplerParameterf = NULL;
			PFNGLSAMPLERPARAMETERFVPROC glad_glSamplerParameterfv = NULL;
			PFNGLSAMPLERPARAMETERIPROC glad_glSamplerParameteri = NULL;
			PFNGLSAMPLERPARAMETERIVPROC glad_glSamplerParameteriv = NULL;
			PFNGLSCISSORPROC glad_glScissor = NULL;
			PFNGLSHADERSOURCEPROC glad_glShaderSource = NULL;
			PFNGLSTENCILFUNCPROC glad_glStencilFunc = NULL;
			PFNGLSTENCILFUNCSEPARATEPROC glad_glStencilFuncSeparate = NULL;
			PFNGLSTENCILMASKPROC glad_glStencilMask = NULL;
			PFNGLSTENCILMASKSEPARATEPROC glad_glStencilMaskSeparate = NULL;
			PFNGLSTENCILOPPROC glad_glStencilOp = NULL;
			PFNGLSTENCILOPSEPARATEPROC glad_glStencilOpSeparate = NULL;
			PFNGLTEXBUFFERPROC glad_glTexBuffer = NULL;
			PFNGLTEXIMAGE1DPROC glad_glTexImage1D = NULL;
			PFNGLTEXIMAGE2DPROC glad_glTexImage2D = NULL;
			PFNGLTEXIMAGE2DMULTISAMPLEPROC glad_glTexImage2DMultisample = NULL;
			PFNGLTEXIMAGE3DPROC glad_glTexImage3D = NULL;
			PFNGLTEXIMAGE3DMULTISAMPLEPROC glad_glTexImage3DMultisample = NULL;
			PFNGLTEXPARAMETERIIVPROC glad_glTexParameterIiv = NULL;
			PFNGLTEXPARAMETERIUIVPROC glad_glTexParameterIuiv = NULL;
			PFNGLTEXPARAMETERFPROC glad_glTexParameterf = NULL;
			PFNGLTEXPARAMETERFVPROC glad_glTexParameterfv = NULL;
			PFNGLTEXPARAMETERIPROC glad_glTexParameteri = NULL;
			PFNGLTEXPARAMETERIVPROC glad_glTexParameteriv = NULL;
			PFNGLTEXSUBIMAGE1DPROC glad_glTexSubImage1D = NULL;
			PFNGLTEXSUBIMAGE2DPROC glad_glTexSubImage2D = NULL;
			PFNGLTEXSUBIMAGE3DPROC glad_glTexSubImage3D = NULL;
			PFNGLTRANSFORMFEEDBACKVARYINGSPROC glad_glTransformFeedbackVaryings = NULL;
			PFNGLUNIFORM1FPROC glad_glUniform1f = NULL;
			PFNGLUNIFORM1FVPROC glad_glUniform1fv = NULL;
			PFNGLUNIFORM1IPROC glad_glUniform1i = NULL;
			PFNGLUNIFORM1IVPROC glad_glUniform1iv = NULL;
			PFNGLUNIFORM1UIPROC glad_glUniform1ui = NULL;
			PFNGLUNIFORM1UIVPROC glad_glUniform1uiv = NULL;
			PFNGLUNIFORM2FPROC glad_glUniform2f = NULL;
			PFNGLUNIFORM2FVPROC glad_glUniform2fv = NULL;
			PFNGLUNIFORM2IPROC glad_glUniform2i = NULL;
			PFNGLUNIFORM2IVPROC glad_glUniform2iv = NULL;
			PFNGLUNIFORM2UIPROC glad_glUniform2ui = NULL;
			PFNGLUNIFORM2UIVPROC glad_glUniform2uiv = NULL;
			PFNGLUNIFORM3FPROC glad_glUniform3f = NULL;
			PFNGLUNIFORM3FVPROC glad_glUniform3fv = NULL;
			PFNGLUNIFORM3IPROC glad_glUniform3i = NULL;
			PFNGLUNIFORM3IVPROC glad_glUniform3iv = NULL;
			PFNGLUNIFORM3UIPROC glad_glUniform3ui = NULL;
			PFNGLUNIFORM3UIVPROC glad_glUniform3uiv = NULL;
			PFNGLUNIFORM4FPROC glad_glUniform4f = NULL;
			PFNGLUNIFORM4FVPROC glad_glUniform4fv = NULL;
			PFNGLUNIFORM4IPROC glad_glUniform4i = NULL;
			PFNGLUNIFORM4IVPROC glad_glUniform4iv = NULL;
			PFNGLUNIFORM4UIPROC glad_glUniform4ui = NULL;
			PFNGLUNIFORM4UIVPROC glad_glUniform4uiv = NULL;
			PFNGLUNIFORMBLOCKBINDINGPROC glad_glUniformBlockBinding = NULL;
			PFNGLUNIFORMMATRIX2FVPROC glad_glUniformMatrix2fv = NULL;
			PFNGLUNIFORMMATRIX2X3FVPROC glad_glUniformMatrix2x3fv = NULL;
			PFNGLUNIFORMMATRIX2X4FVPROC glad_glUniformMatrix2x4fv = NULL;
			PFNGLUNIFORMMATRIX3FVPROC glad_glUniformMatrix3fv = NULL;
			PFNGLUNIFORMMATRIX3X2FVPROC glad_glUniformMatrix3x2fv = NULL;
			PFNGLUNIFORMMATRIX3X4FVPROC glad_glUniformMatrix3x4fv = NULL;
			PFNGLUNIFORMMATRIX4FVPROC glad_glUniformMatrix4fv = NULL;
			PFNGLUNIFORMMATRIX4X2FVPROC glad_glUniformMatrix4x2fv = NULL;
			PFNGLUNIFORMMATRIX4X3FVPROC glad_glUniformMatrix4x3fv = NULL;
			PFNGLUNMAPBUFFERPROC glad_glUnmapBuffer = NULL;
			PFNGLUSEPROGRAMPROC glad_glUseProgram = NULL;
			PFNGLVALIDATEPROGRAMPROC glad_glValidateProgram = NULL;
			PFNGLVERTEXATTRIB1DPROC glad_glVertexAttrib1d = NULL;
			PFNGLVERTEXATTRIB1DVPROC glad_glVertexAttrib1dv = NULL;
			PFNGLVERTEXATTRIB1FPROC glad_glVertexAttrib1f = NULL;
			PFNGLVERTEXATTRIB1FVPROC glad_glVertexAttrib1fv = NULL;
			PFNGLVERTEXATTRIB1SPROC glad_glVertexAttrib1s = NULL;
			PFNGLVERTEXATTRIB1SVPROC glad_glVertexAttrib1sv = NULL;
			PFNGLVERTEXATTRIB2DPROC glad_glVertexAttrib2d = NULL;
			PFNGLVERTEXATTRIB2DVPROC glad_glVertexAttrib2dv = NULL;
			PFNGLVERTEXATTRIB2FPROC glad_glVertexAttrib2f = NULL;
			PFNGLVERTEXATTRIB2FVPROC glad_glVertexAttrib2fv = NULL;
			PFNGLVERTEXATTRIB2SPROC glad_glVertexAttrib2s = NULL;
			PFNGLVERTEXATTRIB2SVPROC glad_glVertexAttrib2sv = NULL;
			PFNGLVERTEXATTRIB3DPROC glad_glVertexAttrib3d = NULL;
			PFNGLVERTEXATTRIB3DVPROC glad_glVertexAttrib3dv = NULL;
			PFNGLVERTEXATTRIB3FPROC glad_glVertexAttrib3f = NULL;
			PFNGLVERTEXATTRIB3FVPROC glad_glVertexAttrib3fv = NULL;
			PFNGLVERTEXATTRIB3SPROC glad_glVertexAttrib3s = NULL;
			PFNGLVERTEXATTRIB3SVPROC glad_glVertexAttrib3sv = NULL;
			PFNGLVERTEXATTRIB4NBVPROC glad_glVertexAttrib4Nbv = NULL;
			PFNGLVERTEXATTRIB4NIVPROC glad_glVertexAttrib4Niv = NULL;
			PFNGLVERTEXATTRIB4NSVPROC glad_glVertexAttrib4Nsv = NULL;
			PFNGLVERTEXATTRIB4NUBPROC glad_glVertexAttrib4Nub = NULL;
			PFNGLVERTEXATTRIB4NUBVPROC glad_glVertexAttrib4Nubv = NULL;
			PFNGLVERTEXATTRIB4NUIVPROC glad_glVertexAttrib4Nuiv = NULL;
			PFNGLVERTEXATTRIB4NUSVPROC glad_glVertexAttrib4Nusv = NULL;
			PFNGLVERTEXATTRIB4BVPROC glad_glVertexAttrib4bv = NULL;
			PFNGLVERTEXATTRIB4DPROC glad_glVertexAttrib4d = NULL;
			PFNGLVERTEXATTRIB4DVPROC glad_glVertexAttrib4dv = NULL;
			PFNGLVERTEXATTRIB4FPROC glad_glVertexAttrib4f = NULL;
			PFNGLVERTEXATTRIB4FVPROC glad_glVertexAttrib4fv = NULL;
			PFNGLVERTEXATTRIB4IVPROC glad_glVertexAttrib4iv = NULL;
			PFNGLVERTEXATTRIB4SPROC glad_glVertexAttrib4s = NULL;
			PFNGLVERTEXATTRIB4SVPROC glad_glVertexAttrib4sv = NULL;
			PFNGLVERTEXATTRIB4UBVPROC glad_glVertexAttrib4ubv = NULL;
			PFNGLVERTEXATTRIB4UIVPROC glad_glVertexAttrib4uiv = NULL;
			PFNGLVERTEXATTRIB4USVPROC glad_glVertexAttrib4usv = NULL;
			PFNGLVERTEXATTRIBDIVISORPROC glad_glVertexAttribDivisor = NULL;
			PFNGLVERTEXATTRIBI1IPROC glad_glVertexAttribI1i = NULL;
			PFNGLVERTEXATTRIBI1IVPROC glad_glVertexAttribI1iv = NULL;
			PFNGLVERTEXATTRIBI1UIPROC glad_glVertexAttribI1ui = NULL;
			PFNGLVERTEXATTRIBI1UIVPROC glad_glVertexAttribI1uiv = NULL;
			PFNGLVERTEXATTRIBI2IPROC glad_glVertexAttribI2i = NULL;
			PFNGLVERTEXATTRIBI2IVPROC glad_glVertexAttribI2iv = NULL;
			PFNGLVERTEXATTRIBI2UIPROC glad_glVertexAttribI2ui = NULL;
			PFNGLVERTEXATTRIBI2UIVPROC glad_glVertexAttribI2uiv = NULL;
			PFNGLVERTEXATTRIBI3IPROC glad_glVertexAttribI3i = NULL;
			PFNGLVERTEXATTRIBI3IVPROC glad_glVertexAttribI3iv = NULL;
			PFNGLVERTEXATTRIBI3UIPROC glad_glVertexAttribI3ui = NULL;
			PFNGLVERTEXATTRIBI3UIVPROC glad_glVertexAttribI3uiv = NULL;
			PFNGLVERTEXATTRIBI4BVPROC glad_glVertexAttribI4bv = NULL;
			PFNGLVERTEXATTRIBI4IPROC glad_glVertexAttribI4i = NULL;
			PFNGLVERTEXATTRIBI4IVPROC glad_glVertexAttribI4iv = NULL;
			PFNGLVERTEXATTRIBI4SVPROC glad_glVertexAttribI4sv = NULL;
			PFNGLVERTEXATTRIBI4UBVPROC glad_glVertexAttribI4ubv = NULL;
			PFNGLVERTEXATTRIBI4UIPROC glad_glVertexAttribI4ui = NULL;
			PFNGLVERTEXATTRIBI4UIVPROC glad_glVertexAttribI4uiv = NULL;
			PFNGLVERTEXATTRIBI4USVPROC glad_glVertexAttribI4usv = NULL;
			PFNGLVERTEXATTRIBIPOINTERPROC glad_glVertexAttribIPointer = NULL;
			PFNGLVERTEXATTRIBP1UIPROC glad_glVertexAttribP1ui = NULL;
			PFNGLVERTEXATTRIBP1UIVPROC glad_glVertexAttribP1uiv = NULL;
			PFNGLVERTEXATTRIBP2UIPROC glad_glVertexAttribP2ui = NULL;
			PFNGLVERTEXATTRIBP2UIVPROC glad_glVertexAttribP2uiv = NULL;
			PFNGLVERTEXATTRIBP3UIPROC glad_glVertexAttribP3ui = NULL;
			PFNGLVERTEXATTRIBP3UIVPROC glad_glVertexAttribP3uiv = NULL;
			PFNGLVERTEXATTRIBP4UIPROC glad_glVertexAttribP4ui = NULL;
			PFNGLVERTEXATTRIBP4UIVPROC glad_glVertexAttribP4uiv = NULL;
			PFNGLVERTEXATTRIBPOINTERPROC glad_glVertexAttribPointer = NULL;
			PFNGLVIEWPORTPROC glad_glViewport = NULL;
			PFNGLWAITSYNCPROC glad_glWaitSync = NULL;


			static void glad_gl_load_GL_VERSION_1_0( GLADuserptrloadfunc load, void* userptr) {
			    if(!GLAD_GL_VERSION_1_0) return;
			    glad_glBlendFunc = (PFNGLBLENDFUNCPROC) load(userptr, "glBlendFunc");
			    glad_glClear = (PFNGLCLEARPROC) load(userptr, "glClear");
			    glad_glClearColor = (PFNGLCLEARCOLORPROC) load(userptr, "glClearColor");
			    glad_glClearDepth = (PFNGLCLEARDEPTHPROC) load(userptr, "glClearDepth");
			    glad_glClearStencil = (PFNGLCLEARSTENCILPROC) load(userptr, "glClearStencil");
			    glad_glColorMask = (PFNGLCOLORMASKPROC) load(userptr, "glColorMask");
			    glad_glCullFace = (PFNGLCULLFACEPROC) load(userptr, "glCullFace");
			    glad_glDepthFunc = (PFNGLDEPTHFUNCPROC) load(userptr, "glDepthFunc");
			    glad_glDepthMask = (PFNGLDEPTHMASKPROC) load(userptr, "glDepthMask");
			    glad_glDepthRange = (PFNGLDEPTHRANGEPROC) load(userptr, "glDepthRange");
			    glad_glDisable = (PFNGLDISABLEPROC) load(userptr, "glDisable");
			    glad_glDrawBuffer = (PFNGLDRAWBUFFERPROC) load(userptr, "glDrawBuffer");
			    glad_glEnable = (PFNGLENABLEPROC) load(userptr, "glEnable");
			    glad_glFinish = (PFNGLFINISHPROC) load(userptr, "glFinish");
			    glad_glFlush = (PFNGLFLUSHPROC) load(userptr, "glFlush");
			    glad_glFrontFace = (PFNGLFRONTFACEPROC) load(userptr, "glFrontFace");
			    glad_glGetBooleanv = (PFNGLGETBOOLEANVPROC) load(userptr, "glGetBooleanv");
			    glad_glGetDoublev = (PFNGLGETDOUBLEVPROC) load(userptr, "glGetDoublev");
			    glad_glGetError = (PFNGLGETERRORPROC) load(userptr, "glGetError");
			    glad_glGetFloatv = (PFNGLGETFLOATVPROC) load(userptr, "glGetFloatv");
			    glad_glGetIntegerv = (PFNGLGETINTEGERVPROC) load(userptr, "glGetIntegerv");
			    glad_glGetString = (PFNGLGETSTRINGPROC) load(userptr, "glGetString");
			    glad_glGetTexImage = (PFNGLGETTEXIMAGEPROC) load(userptr, "glGetTexImage");
			    glad_glGetTexLevelParameterfv = (PFNGLGETTEXLEVELPARAMETERFVPROC) load(userptr, "glGetTexLevelParameterfv");
			    glad_glGetTexLevelParameteriv = (PFNGLGETTEXLEVELPARAMETERIVPROC) load(userptr, "glGetTexLevelParameteriv");
			    glad_glGetTexParameterfv = (PFNGLGETTEXPARAMETERFVPROC) load(userptr, "glGetTexParameterfv");
			    glad_glGetTexParameteriv = (PFNGLGETTEXPARAMETERIVPROC) load(userptr, "glGetTexParameteriv");
			    glad_glHint = (PFNGLHINTPROC) load(userptr, "glHint");
			    glad_glIsEnabled = (PFNGLISENABLEDPROC) load(userptr, "glIsEnabled");
			    glad_glLineWidth = (PFNGLLINEWIDTHPROC) load(userptr, "glLineWidth");
			    glad_glLogicOp = (PFNGLLOGICOPPROC) load(userptr, "glLogicOp");
			    glad_glPixelStoref = (PFNGLPIXELSTOREFPROC) load(userptr, "glPixelStoref");
			    glad_glPixelStorei = (PFNGLPIXELSTOREIPROC) load(userptr, "glPixelStorei");
			    glad_glPointSize = (PFNGLPOINTSIZEPROC) load(userptr, "glPointSize");
			    glad_glPolygonMode = (PFNGLPOLYGONMODEPROC) load(userptr, "glPolygonMode");
			    glad_glReadBuffer = (PFNGLREADBUFFERPROC) load(userptr, "glReadBuffer");
			    glad_glReadPixels = (PFNGLREADPIXELSPROC) load(userptr, "glReadPixels");
			    glad_glScissor = (PFNGLSCISSORPROC) load(userptr, "glScissor");
			    glad_glStencilFunc = (PFNGLSTENCILFUNCPROC) load(userptr, "glStencilFunc");
			    glad_glStencilMask = (PFNGLSTENCILMASKPROC) load(userptr, "glStencilMask");
			    glad_glStencilOp = (PFNGLSTENCILOPPROC) load(userptr, "glStencilOp");
			    glad_glTexImage1D = (PFNGLTEXIMAGE1DPROC) load(userptr, "glTexImage1D");
			    glad_glTexImage2D = (PFNGLTEXIMAGE2DPROC) load(userptr, "glTexImage2D");
			    glad_glTexParameterf = (PFNGLTEXPARAMETERFPROC) load(userptr, "glTexParameterf");
			    glad_glTexParameterfv = (PFNGLTEXPARAMETERFVPROC) load(userptr, "glTexParameterfv");
			    glad_glTexParameteri = (PFNGLTEXPARAMETERIPROC) load(userptr, "glTexParameteri");
			    glad_glTexParameteriv = (PFNGLTEXPARAMETERIVPROC) load(userptr, "glTexParameteriv");
			    glad_glViewport = (PFNGLVIEWPORTPROC) load(userptr, "glViewport");
			}
			static void glad_gl_load_GL_VERSION_1_1( GLADuserptrloadfunc load, void* userptr) {
			    if(!GLAD_GL_VERSION_1_1) return;
			    glad_glBindTexture = (PFNGLBINDTEXTUREPROC) load(userptr, "glBindTexture");
			    glad_glCopyTexImage1D = (PFNGLCOPYTEXIMAGE1DPROC) load(userptr, "glCopyTexImage1D");
			    glad_glCopyTexImage2D = (PFNGLCOPYTEXIMAGE2DPROC) load(userptr, "glCopyTexImage2D");
			    glad_glCopyTexSubImage1D = (PFNGLCOPYTEXSUBIMAGE1DPROC) load(userptr, "glCopyTexSubImage1D");
			    glad_glCopyTexSubImage2D = (PFNGLCOPYTEXSUBIMAGE2DPROC) load(userptr, "glCopyTexSubImage2D");
			    glad_glDeleteTextures = (PFNGLDELETETEXTURESPROC) load(userptr, "glDeleteTextures");
			    glad_glDrawArrays = (PFNGLDRAWARRAYSPROC) load(userptr, "glDrawArrays");
			    glad_glDrawElements = (PFNGLDRAWELEMENTSPROC) load(userptr, "glDrawElements");
			    glad_glGenTextures = (PFNGLGENTEXTURESPROC) load(userptr, "glGenTextures");
			    glad_glIsTexture = (PFNGLISTEXTUREPROC) load(userptr, "glIsTexture");
			    glad_glPolygonOffset = (PFNGLPOLYGONOFFSETPROC) load(userptr, "glPolygonOffset");
			    glad_glTexSubImage1D = (PFNGLTEXSUBIMAGE1DPROC) load(userptr, "glTexSubImage1D");
			    glad_glTexSubImage2D = (PFNGLTEXSUBIMAGE2DPROC) load(userptr, "glTexSubImage2D");
			}
			static void glad_gl_load_GL_VERSION_1_2( GLADuserptrloadfunc load, void* userptr) {
			    if(!GLAD_GL_VERSION_1_2) return;
			    glad_glCopyTexSubImage3D = (PFNGLCOPYTEXSUBIMAGE3DPROC) load(userptr, "glCopyTexSubImage3D");
			    glad_glDrawRangeElements = (PFNGLDRAWRANGEELEMENTSPROC) load(userptr, "glDrawRangeElements");
			    glad_glTexImage3D = (PFNGLTEXIMAGE3DPROC) load(userptr, "glTexImage3D");
			    glad_glTexSubImage3D = (PFNGLTEXSUBIMAGE3DPROC) load(userptr, "glTexSubImage3D");
			}
			static void glad_gl_load_GL_VERSION_1_3( GLADuserptrloadfunc load, void* userptr) {
			    if(!GLAD_GL_VERSION_1_3) return;
			    glad_glActiveTexture = (PFNGLACTIVETEXTUREPROC) load(userptr, "glActiveTexture");
			    glad_glCompressedTexImage1D = (PFNGLCOMPRESSEDTEXIMAGE1DPROC) load(userptr, "glCompressedTexImage1D");
			    glad_glCompressedTexImage2D = (PFNGLCOMPRESSEDTEXIMAGE2DPROC) load(userptr, "glCompressedTexImage2D");
			    glad_glCompressedTexImage3D = (PFNGLCOMPRESSEDTEXIMAGE3DPROC) load(userptr, "glCompressedTexImage3D");
			    glad_glCompressedTexSubImage1D = (PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC) load(userptr, "glCompressedTexSubImage1D");
			    glad_glCompressedTexSubImage2D = (PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC) load(userptr, "glCompressedTexSubImage2D");
			    glad_glCompressedTexSubImage3D = (PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC) load(userptr, "glCompressedTexSubImage3D");
			    glad_glGetCompressedTexImage = (PFNGLGETCOMPRESSEDTEXIMAGEPROC) load(userptr, "glGetCompressedTexImage");
			    glad_glSampleCoverage = (PFNGLSAMPLECOVERAGEPROC) load(userptr, "glSampleCoverage");
			}
			static void glad_gl_load_GL_VERSION_1_4( GLADuserptrloadfunc load, void* userptr) {
			    if(!GLAD_GL_VERSION_1_4) return;
			    glad_glBlendColor = (PFNGLBLENDCOLORPROC) load(userptr, "glBlendColor");
			    glad_glBlendEquation = (PFNGLBLENDEQUATIONPROC) load(userptr, "glBlendEquation");
			    glad_glBlendFuncSeparate = (PFNGLBLENDFUNCSEPARATEPROC) load(userptr, "glBlendFuncSeparate");
			    glad_glMultiDrawArrays = (PFNGLMULTIDRAWARRAYSPROC) load(userptr, "glMultiDrawArrays");
			    glad_glMultiDrawElements = (PFNGLMULTIDRAWELEMENTSPROC) load(userptr, "glMultiDrawElements");
			    glad_glPointParameterf = (PFNGLPOINTPARAMETERFPROC) load(userptr, "glPointParameterf");
			    glad_glPointParameterfv = (PFNGLPOINTPARAMETERFVPROC) load(userptr, "glPointParameterfv");
			    glad_glPointParameteri = (PFNGLPOINTPARAMETERIPROC) load(userptr, "glPointParameteri");
			    glad_glPointParameteriv = (PFNGLPOINTPARAMETERIVPROC) load(userptr, "glPointParameteriv");
			}
			static void glad_gl_load_GL_VERSION_1_5( GLADuserptrloadfunc load, void* userptr) {
			    if(!GLAD_GL_VERSION_1_5) return;
			    glad_glBeginQuery = (PFNGLBEGINQUERYPROC) load(userptr, "glBeginQuery");
			    glad_glBindBuffer = (PFNGLBINDBUFFERPROC) load(userptr, "glBindBuffer");
			    glad_glBufferData = (PFNGLBUFFERDATAPROC) load(userptr, "glBufferData");
			    glad_glBufferSubData = (PFNGLBUFFERSUBDATAPROC) load(userptr, "glBufferSubData");
			    glad_glDeleteBuffers = (PFNGLDELETEBUFFERSPROC) load(userptr, "glDeleteBuffers");
			    glad_glDeleteQueries = (PFNGLDELETEQUERIESPROC) load(userptr, "glDeleteQueries");
			    glad_glEndQuery = (PFNGLENDQUERYPROC) load(userptr, "glEndQuery");
			    glad_glGenBuffers = (PFNGLGENBUFFERSPROC) load(userptr, "glGenBuffers");
			    glad_glGenQueries = (PFNGLGENQUERIESPROC) load(userptr, "glGenQueries");
			    glad_glGetBufferParameteriv = (PFNGLGETBUFFERPARAMETERIVPROC) load(userptr, "glGetBufferParameteriv");
			    glad_glGetBufferPointerv = (PFNGLGETBUFFERPOINTERVPROC) load(userptr, "glGetBufferPointerv");
			    glad_glGetBufferSubData = (PFNGLGETBUFFERSUBDATAPROC) load(userptr, "glGetBufferSubData");
			    glad_glGetQueryObjectiv = (PFNGLGETQUERYOBJECTIVPROC) load(userptr, "glGetQueryObjectiv");
			    glad_glGetQueryObjectuiv = (PFNGLGETQUERYOBJECTUIVPROC) load(userptr, "glGetQueryObjectuiv");
			    glad_glGetQueryiv = (PFNGLGETQUERYIVPROC) load(userptr, "glGetQueryiv");
			    glad_glIsBuffer = (PFNGLISBUFFERPROC) load(userptr, "glIsBuffer");
			    glad_glIsQuery = (PFNGLISQUERYPROC) load(userptr, "glIsQuery");
			    glad_glMapBuffer = (PFNGLMAPBUFFERPROC) load(userptr, "glMapBuffer");
			    glad_glUnmapBuffer = (PFNGLUNMAPBUFFERPROC) load(userptr, "glUnmapBuffer");
			}
			static void glad_gl_load_GL_VERSION_2_0( GLADuserptrloadfunc load, void* userptr) {
			    if(!GLAD_GL_VERSION_2_0) return;
			    glad_glAttachShader = (PFNGLATTACHSHADERPROC) load(userptr, "glAttachShader");
			    glad_glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC) load(userptr, "glBindAttribLocation");
			    glad_glBlendEquationSeparate = (PFNGLBLENDEQUATIONSEPARATEPROC) load(userptr, "glBlendEquationSeparate");
			    glad_glCompileShader = (PFNGLCOMPILESHADERPROC) load(userptr, "glCompileShader");
			    glad_glCreateProgram = (PFNGLCREATEPROGRAMPROC) load(userptr, "glCreateProgram");
			    glad_glCreateShader = (PFNGLCREATESHADERPROC) load(userptr, "glCreateShader");
			    glad_glDeleteProgram = (PFNGLDELETEPROGRAMPROC) load(userptr, "glDeleteProgram");
			    glad_glDeleteShader = (PFNGLDELETESHADERPROC) load(userptr, "glDeleteShader");
			    glad_glDetachShader = (PFNGLDETACHSHADERPROC) load(userptr, "glDetachShader");
			    glad_glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC) load(userptr, "glDisableVertexAttribArray");
			    glad_glDrawBuffers = (PFNGLDRAWBUFFERSPROC) load(userptr, "glDrawBuffers");
			    glad_glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC) load(userptr, "glEnableVertexAttribArray");
			    glad_glGetActiveAttrib = (PFNGLGETACTIVEATTRIBPROC) load(userptr, "glGetActiveAttrib");
			    glad_glGetActiveUniform = (PFNGLGETACTIVEUNIFORMPROC) load(userptr, "glGetActiveUniform");
			    glad_glGetAttachedShaders = (PFNGLGETATTACHEDSHADERSPROC) load(userptr, "glGetAttachedShaders");
			    glad_glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC) load(userptr, "glGetAttribLocation");
			    glad_glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC) load(userptr, "glGetProgramInfoLog");
			    glad_glGetProgramiv = (PFNGLGETPROGRAMIVPROC) load(userptr, "glGetProgramiv");
			    glad_glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC) load(userptr, "glGetShaderInfoLog");
			    glad_glGetShaderSource = (PFNGLGETSHADERSOURCEPROC) load(userptr, "glGetShaderSource");
			    glad_glGetShaderiv = (PFNGLGETSHADERIVPROC) load(userptr, "glGetShaderiv");
			    glad_glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC) load(userptr, "glGetUniformLocation");
			    glad_glGetUniformfv = (PFNGLGETUNIFORMFVPROC) load(userptr, "glGetUniformfv");
			    glad_glGetUniformiv = (PFNGLGETUNIFORMIVPROC) load(userptr, "glGetUniformiv");
			    glad_glGetVertexAttribPointerv = (PFNGLGETVERTEXATTRIBPOINTERVPROC) load(userptr, "glGetVertexAttribPointerv");
			    glad_glGetVertexAttribdv = (PFNGLGETVERTEXATTRIBDVPROC) load(userptr, "glGetVertexAttribdv");
			    glad_glGetVertexAttribfv = (PFNGLGETVERTEXATTRIBFVPROC) load(userptr, "glGetVertexAttribfv");
			    glad_glGetVertexAttribiv = (PFNGLGETVERTEXATTRIBIVPROC) load(userptr, "glGetVertexAttribiv");
			    glad_glIsProgram = (PFNGLISPROGRAMPROC) load(userptr, "glIsProgram");
			    glad_glIsShader = (PFNGLISSHADERPROC) load(userptr, "glIsShader");
			    glad_glLinkProgram = (PFNGLLINKPROGRAMPROC) load(userptr, "glLinkProgram");
			    glad_glShaderSource = (PFNGLSHADERSOURCEPROC) load(userptr, "glShaderSource");
			    glad_glStencilFuncSeparate = (PFNGLSTENCILFUNCSEPARATEPROC) load(userptr, "glStencilFuncSeparate");
			    glad_glStencilMaskSeparate = (PFNGLSTENCILMASKSEPARATEPROC) load(userptr, "glStencilMaskSeparate");
			    glad_glStencilOpSeparate = (PFNGLSTENCILOPSEPARATEPROC) load(userptr, "glStencilOpSeparate");
			    glad_glUniform1f = (PFNGLUNIFORM1FPROC) load(userptr, "glUniform1f");
			    glad_glUniform1fv = (PFNGLUNIFORM1FVPROC) load(userptr, "glUniform1fv");
			    glad_glUniform1i = (PFNGLUNIFORM1IPROC) load(userptr, "glUniform1i");
			    glad_glUniform1iv = (PFNGLUNIFORM1IVPROC) load(userptr, "glUniform1iv");
			    glad_glUniform2f = (PFNGLUNIFORM2FPROC) load(userptr, "glUniform2f");
			    glad_glUniform2fv = (PFNGLUNIFORM2FVPROC) load(userptr, "glUniform2fv");
			    glad_glUniform2i = (PFNGLUNIFORM2IPROC) load(userptr, "glUniform2i");
			    glad_glUniform2iv = (PFNGLUNIFORM2IVPROC) load(userptr, "glUniform2iv");
			    glad_glUniform3f = (PFNGLUNIFORM3FPROC) load(userptr, "glUniform3f");
			    glad_glUniform3fv = (PFNGLUNIFORM3FVPROC) load(userptr, "glUniform3fv");
			    glad_glUniform3i = (PFNGLUNIFORM3IPROC) load(userptr, "glUniform3i");
			    glad_glUniform3iv = (PFNGLUNIFORM3IVPROC) load(userptr, "glUniform3iv");
			    glad_glUniform4f = (PFNGLUNIFORM4FPROC) load(userptr, "glUniform4f");
			    glad_glUniform4fv = (PFNGLUNIFORM4FVPROC) load(userptr, "glUniform4fv");
			    glad_glUniform4i = (PFNGLUNIFORM4IPROC) load(userptr, "glUniform4i");
			    glad_glUniform4iv = (PFNGLUNIFORM4IVPROC) load(userptr, "glUniform4iv");
			    glad_glUniformMatrix2fv = (PFNGLUNIFORMMATRIX2FVPROC) load(userptr, "glUniformMatrix2fv");
			    glad_glUniformMatrix3fv = (PFNGLUNIFORMMATRIX3FVPROC) load(userptr, "glUniformMatrix3fv");
			    glad_glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC) load(userptr, "glUniformMatrix4fv");
			    glad_glUseProgram = (PFNGLUSEPROGRAMPROC) load(userptr, "glUseProgram");
			    glad_glValidateProgram = (PFNGLVALIDATEPROGRAMPROC) load(userptr, "glValidateProgram");
			    glad_glVertexAttrib1d = (PFNGLVERTEXATTRIB1DPROC) load(userptr, "glVertexAttrib1d");
			    glad_glVertexAttrib1dv = (PFNGLVERTEXATTRIB1DVPROC) load(userptr, "glVertexAttrib1dv");
			    glad_glVertexAttrib1f = (PFNGLVERTEXATTRIB1FPROC) load(userptr, "glVertexAttrib1f");
			    glad_glVertexAttrib1fv = (PFNGLVERTEXATTRIB1FVPROC) load(userptr, "glVertexAttrib1fv");
			    glad_glVertexAttrib1s = (PFNGLVERTEXATTRIB1SPROC) load(userptr, "glVertexAttrib1s");
			    glad_glVertexAttrib1sv = (PFNGLVERTEXATTRIB1SVPROC) load(userptr, "glVertexAttrib1sv");
			    glad_glVertexAttrib2d = (PFNGLVERTEXATTRIB2DPROC) load(userptr, "glVertexAttrib2d");
			    glad_glVertexAttrib2dv = (PFNGLVERTEXATTRIB2DVPROC) load(userptr, "glVertexAttrib2dv");
			    glad_glVertexAttrib2f = (PFNGLVERTEXATTRIB2FPROC) load(userptr, "glVertexAttrib2f");
			    glad_glVertexAttrib2fv = (PFNGLVERTEXATTRIB2FVPROC) load(userptr, "glVertexAttrib2fv");
			    glad_glVertexAttrib2s = (PFNGLVERTEXATTRIB2SPROC) load(userptr, "glVertexAttrib2s");
			    glad_glVertexAttrib2sv = (PFNGLVERTEXATTRIB2SVPROC) load(userptr, "glVertexAttrib2sv");
			    glad_glVertexAttrib3d = (PFNGLVERTEXATTRIB3DPROC) load(userptr, "glVertexAttrib3d");
			    glad_glVertexAttrib3dv = (PFNGLVERTEXATTRIB3DVPROC) load(userptr, "glVertexAttrib3dv");
			    glad_glVertexAttrib3f = (PFNGLVERTEXATTRIB3FPROC) load(userptr, "glVertexAttrib3f");
			    glad_glVertexAttrib3fv = (PFNGLVERTEXATTRIB3FVPROC) load(userptr, "glVertexAttrib3fv");
			    glad_glVertexAttrib3s = (PFNGLVERTEXATTRIB3SPROC) load(userptr, "glVertexAttrib3s");
			    glad_glVertexAttrib3sv = (PFNGLVERTEXATTRIB3SVPROC) load(userptr, "glVertexAttrib3sv");
			    glad_glVertexAttrib4Nbv = (PFNGLVERTEXATTRIB4NBVPROC) load(userptr, "glVertexAttrib4Nbv");
			    glad_glVertexAttrib4Niv = (PFNGLVERTEXATTRIB4NIVPROC) load(userptr, "glVertexAttrib4Niv");
			    glad_glVertexAttrib4Nsv = (PFNGLVERTEXATTRIB4NSVPROC) load(userptr, "glVertexAttrib4Nsv");
			    glad_glVertexAttrib4Nub = (PFNGLVERTEXATTRIB4NUBPROC) load(userptr, "glVertexAttrib4Nub");
			    glad_glVertexAttrib4Nubv = (PFNGLVERTEXATTRIB4NUBVPROC) load(userptr, "glVertexAttrib4Nubv");
			    glad_glVertexAttrib4Nuiv = (PFNGLVERTEXATTRIB4NUIVPROC) load(userptr, "glVertexAttrib4Nuiv");
			    glad_glVertexAttrib4Nusv = (PFNGLVERTEXATTRIB4NUSVPROC) load(userptr, "glVertexAttrib4Nusv");
			    glad_glVertexAttrib4bv = (PFNGLVERTEXATTRIB4BVPROC) load(userptr, "glVertexAttrib4bv");
			    glad_glVertexAttrib4d = (PFNGLVERTEXATTRIB4DPROC) load(userptr, "glVertexAttrib4d");
			    glad_glVertexAttrib4dv = (PFNGLVERTEXATTRIB4DVPROC) load(userptr, "glVertexAttrib4dv");
			    glad_glVertexAttrib4f = (PFNGLVERTEXATTRIB4FPROC) load(userptr, "glVertexAttrib4f");
			    glad_glVertexAttrib4fv = (PFNGLVERTEXATTRIB4FVPROC) load(userptr, "glVertexAttrib4fv");
			    glad_glVertexAttrib4iv = (PFNGLVERTEXATTRIB4IVPROC) load(userptr, "glVertexAttrib4iv");
			    glad_glVertexAttrib4s = (PFNGLVERTEXATTRIB4SPROC) load(userptr, "glVertexAttrib4s");
			    glad_glVertexAttrib4sv = (PFNGLVERTEXATTRIB4SVPROC) load(userptr, "glVertexAttrib4sv");
			    glad_glVertexAttrib4ubv = (PFNGLVERTEXATTRIB4UBVPROC) load(userptr, "glVertexAttrib4ubv");
			    glad_glVertexAttrib4uiv = (PFNGLVERTEXATTRIB4UIVPROC) load(userptr, "glVertexAttrib4uiv");
			    glad_glVertexAttrib4usv = (PFNGLVERTEXATTRIB4USVPROC) load(userptr, "glVertexAttrib4usv");
			    glad_glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC) load(userptr, "glVertexAttribPointer");
			}
			static void glad_gl_load_GL_VERSION_2_1( GLADuserptrloadfunc load, void* userptr) {
			    if(!GLAD_GL_VERSION_2_1) return;
			    glad_glUniformMatrix2x3fv = (PFNGLUNIFORMMATRIX2X3FVPROC) load(userptr, "glUniformMatrix2x3fv");
			    glad_glUniformMatrix2x4fv = (PFNGLUNIFORMMATRIX2X4FVPROC) load(userptr, "glUniformMatrix2x4fv");
			    glad_glUniformMatrix3x2fv = (PFNGLUNIFORMMATRIX3X2FVPROC) load(userptr, "glUniformMatrix3x2fv");
			    glad_glUniformMatrix3x4fv = (PFNGLUNIFORMMATRIX3X4FVPROC) load(userptr, "glUniformMatrix3x4fv");
			    glad_glUniformMatrix4x2fv = (PFNGLUNIFORMMATRIX4X2FVPROC) load(userptr, "glUniformMatrix4x2fv");
			    glad_glUniformMatrix4x3fv = (PFNGLUNIFORMMATRIX4X3FVPROC) load(userptr, "glUniformMatrix4x3fv");
			}
			static void glad_gl_load_GL_VERSION_3_0( GLADuserptrloadfunc load, void* userptr) {
			    if(!GLAD_GL_VERSION_3_0) return;
			    glad_glBeginConditionalRender = (PFNGLBEGINCONDITIONALRENDERPROC) load(userptr, "glBeginConditionalRender");
			    glad_glBeginTransformFeedback = (PFNGLBEGINTRANSFORMFEEDBACKPROC) load(userptr, "glBeginTransformFeedback");
			    glad_glBindBufferBase = (PFNGLBINDBUFFERBASEPROC) load(userptr, "glBindBufferBase");
			    glad_glBindBufferRange = (PFNGLBINDBUFFERRANGEPROC) load(userptr, "glBindBufferRange");
			    glad_glBindFragDataLocation = (PFNGLBINDFRAGDATALOCATIONPROC) load(userptr, "glBindFragDataLocation");
			    glad_glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC) load(userptr, "glBindFramebuffer");
			    glad_glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC) load(userptr, "glBindRenderbuffer");
			    glad_glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC) load(userptr, "glBindVertexArray");
			    glad_glBlitFramebuffer = (PFNGLBLITFRAMEBUFFERPROC) load(userptr, "glBlitFramebuffer");
			    glad_glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC) load(userptr, "glCheckFramebufferStatus");
			    glad_glClampColor = (PFNGLCLAMPCOLORPROC) load(userptr, "glClampColor");
			    glad_glClearBufferfi = (PFNGLCLEARBUFFERFIPROC) load(userptr, "glClearBufferfi");
			    glad_glClearBufferfv = (PFNGLCLEARBUFFERFVPROC) load(userptr, "glClearBufferfv");
			    glad_glClearBufferiv = (PFNGLCLEARBUFFERIVPROC) load(userptr, "glClearBufferiv");
			    glad_glClearBufferuiv = (PFNGLCLEARBUFFERUIVPROC) load(userptr, "glClearBufferuiv");
			    glad_glColorMaski = (PFNGLCOLORMASKIPROC) load(userptr, "glColorMaski");
			    glad_glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC) load(userptr, "glDeleteFramebuffers");
			    glad_glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC) load(userptr, "glDeleteRenderbuffers");
			    glad_glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC) load(userptr, "glDeleteVertexArrays");
			    glad_glDisablei = (PFNGLDISABLEIPROC) load(userptr, "glDisablei");
			    glad_glEnablei = (PFNGLENABLEIPROC) load(userptr, "glEnablei");
			    glad_glEndConditionalRender = (PFNGLENDCONDITIONALRENDERPROC) load(userptr, "glEndConditionalRender");
			    glad_glEndTransformFeedback = (PFNGLENDTRANSFORMFEEDBACKPROC) load(userptr, "glEndTransformFeedback");
			    glad_glFlushMappedBufferRange = (PFNGLFLUSHMAPPEDBUFFERRANGEPROC) load(userptr, "glFlushMappedBufferRange");
			    glad_glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC) load(userptr, "glFramebufferRenderbuffer");
			    glad_glFramebufferTexture1D = (PFNGLFRAMEBUFFERTEXTURE1DPROC) load(userptr, "glFramebufferTexture1D");
			    glad_glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC) load(userptr, "glFramebufferTexture2D");
			    glad_glFramebufferTexture3D = (PFNGLFRAMEBUFFERTEXTURE3DPROC) load(userptr, "glFramebufferTexture3D");
			    glad_glFramebufferTextureLayer = (PFNGLFRAMEBUFFERTEXTURELAYERPROC) load(userptr, "glFramebufferTextureLayer");
			    glad_glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC) load(userptr, "glGenFramebuffers");
			    glad_glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC) load(userptr, "glGenRenderbuffers");
			    glad_glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC) load(userptr, "glGenVertexArrays");
			    glad_glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC) load(userptr, "glGenerateMipmap");
			    glad_glGetBooleani_v = (PFNGLGETBOOLEANI_VPROC) load(userptr, "glGetBooleani_v");
			    glad_glGetFragDataLocation = (PFNGLGETFRAGDATALOCATIONPROC) load(userptr, "glGetFragDataLocation");
			    glad_glGetFramebufferAttachmentParameteriv = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC) load(userptr, "glGetFramebufferAttachmentParameteriv");
			    glad_glGetIntegeri_v = (PFNGLGETINTEGERI_VPROC) load(userptr, "glGetIntegeri_v");
			    glad_glGetRenderbufferParameteriv = (PFNGLGETRENDERBUFFERPARAMETERIVPROC) load(userptr, "glGetRenderbufferParameteriv");
			    glad_glGetStringi = (PFNGLGETSTRINGIPROC) load(userptr, "glGetStringi");
			    glad_glGetTexParameterIiv = (PFNGLGETTEXPARAMETERIIVPROC) load(userptr, "glGetTexParameterIiv");
			    glad_glGetTexParameterIuiv = (PFNGLGETTEXPARAMETERIUIVPROC) load(userptr, "glGetTexParameterIuiv");
			    glad_glGetTransformFeedbackVarying = (PFNGLGETTRANSFORMFEEDBACKVARYINGPROC) load(userptr, "glGetTransformFeedbackVarying");
			    glad_glGetUniformuiv = (PFNGLGETUNIFORMUIVPROC) load(userptr, "glGetUniformuiv");
			    glad_glGetVertexAttribIiv = (PFNGLGETVERTEXATTRIBIIVPROC) load(userptr, "glGetVertexAttribIiv");
			    glad_glGetVertexAttribIuiv = (PFNGLGETVERTEXATTRIBIUIVPROC) load(userptr, "glGetVertexAttribIuiv");
			    glad_glIsEnabledi = (PFNGLISENABLEDIPROC) load(userptr, "glIsEnabledi");
			    glad_glIsFramebuffer = (PFNGLISFRAMEBUFFERPROC) load(userptr, "glIsFramebuffer");
			    glad_glIsRenderbuffer = (PFNGLISRENDERBUFFERPROC) load(userptr, "glIsRenderbuffer");
			    glad_glIsVertexArray = (PFNGLISVERTEXARRAYPROC) load(userptr, "glIsVertexArray");
			    glad_glMapBufferRange = (PFNGLMAPBUFFERRANGEPROC) load(userptr, "glMapBufferRange");
			    glad_glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC) load(userptr, "glRenderbufferStorage");
			    glad_glRenderbufferStorageMultisample = (PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC) load(userptr, "glRenderbufferStorageMultisample");
			    glad_glTexParameterIiv = (PFNGLTEXPARAMETERIIVPROC) load(userptr, "glTexParameterIiv");
			    glad_glTexParameterIuiv = (PFNGLTEXPARAMETERIUIVPROC) load(userptr, "glTexParameterIuiv");
			    glad_glTransformFeedbackVaryings = (PFNGLTRANSFORMFEEDBACKVARYINGSPROC) load(userptr, "glTransformFeedbackVaryings");
			    glad_glUniform1ui = (PFNGLUNIFORM1UIPROC) load(userptr, "glUniform1ui");
			    glad_glUniform1uiv = (PFNGLUNIFORM1UIVPROC) load(userptr, "glUniform1uiv");
			    glad_glUniform2ui = (PFNGLUNIFORM2UIPROC) load(userptr, "glUniform2ui");
			    glad_glUniform2uiv = (PFNGLUNIFORM2UIVPROC) load(userptr, "glUniform2uiv");
			    glad_glUniform3ui = (PFNGLUNIFORM3UIPROC) load(userptr, "glUniform3ui");
			    glad_glUniform3uiv = (PFNGLUNIFORM3UIVPROC) load(userptr, "glUniform3uiv");
			    glad_glUniform4ui = (PFNGLUNIFORM4UIPROC) load(userptr, "glUniform4ui");
			    glad_glUniform4uiv = (PFNGLUNIFORM4UIVPROC) load(userptr, "glUniform4uiv");
			    glad_glVertexAttribI1i = (PFNGLVERTEXATTRIBI1IPROC) load(userptr, "glVertexAttribI1i");
			    glad_glVertexAttribI1iv = (PFNGLVERTEXATTRIBI1IVPROC) load(userptr, "glVertexAttribI1iv");
			    glad_glVertexAttribI1ui = (PFNGLVERTEXATTRIBI1UIPROC) load(userptr, "glVertexAttribI1ui");
			    glad_glVertexAttribI1uiv = (PFNGLVERTEXATTRIBI1UIVPROC) load(userptr, "glVertexAttribI1uiv");
			    glad_glVertexAttribI2i = (PFNGLVERTEXATTRIBI2IPROC) load(userptr, "glVertexAttribI2i");
			    glad_glVertexAttribI2iv = (PFNGLVERTEXATTRIBI2IVPROC) load(userptr, "glVertexAttribI2iv");
			    glad_glVertexAttribI2ui = (PFNGLVERTEXATTRIBI2UIPROC) load(userptr, "glVertexAttribI2ui");
			    glad_glVertexAttribI2uiv = (PFNGLVERTEXATTRIBI2UIVPROC) load(userptr, "glVertexAttribI2uiv");
			    glad_glVertexAttribI3i = (PFNGLVERTEXATTRIBI3IPROC) load(userptr, "glVertexAttribI3i");
			    glad_glVertexAttribI3iv = (PFNGLVERTEXATTRIBI3IVPROC) load(userptr, "glVertexAttribI3iv");
			    glad_glVertexAttribI3ui = (PFNGLVERTEXATTRIBI3UIPROC) load(userptr, "glVertexAttribI3ui");
			    glad_glVertexAttribI3uiv = (PFNGLVERTEXATTRIBI3UIVPROC) load(userptr, "glVertexAttribI3uiv");
			    glad_glVertexAttribI4bv = (PFNGLVERTEXATTRIBI4BVPROC) load(userptr, "glVertexAttribI4bv");
			    glad_glVertexAttribI4i = (PFNGLVERTEXATTRIBI4IPROC) load(userptr, "glVertexAttribI4i");
			    glad_glVertexAttribI4iv = (PFNGLVERTEXATTRIBI4IVPROC) load(userptr, "glVertexAttribI4iv");
			    glad_glVertexAttribI4sv = (PFNGLVERTEXATTRIBI4SVPROC) load(userptr, "glVertexAttribI4sv");
			    glad_glVertexAttribI4ubv = (PFNGLVERTEXATTRIBI4UBVPROC) load(userptr, "glVertexAttribI4ubv");
			    glad_glVertexAttribI4ui = (PFNGLVERTEXATTRIBI4UIPROC) load(userptr, "glVertexAttribI4ui");
			    glad_glVertexAttribI4uiv = (PFNGLVERTEXATTRIBI4UIVPROC) load(userptr, "glVertexAttribI4uiv");
			    glad_glVertexAttribI4usv = (PFNGLVERTEXATTRIBI4USVPROC) load(userptr, "glVertexAttribI4usv");
			    glad_glVertexAttribIPointer = (PFNGLVERTEXATTRIBIPOINTERPROC) load(userptr, "glVertexAttribIPointer");
			}
			static void glad_gl_load_GL_VERSION_3_1( GLADuserptrloadfunc load, void* userptr) {
			    if(!GLAD_GL_VERSION_3_1) return;
			    glad_glBindBufferBase = (PFNGLBINDBUFFERBASEPROC) load(userptr, "glBindBufferBase");
			    glad_glBindBufferRange = (PFNGLBINDBUFFERRANGEPROC) load(userptr, "glBindBufferRange");
			    glad_glCopyBufferSubData = (PFNGLCOPYBUFFERSUBDATAPROC) load(userptr, "glCopyBufferSubData");
			    glad_glDrawArraysInstanced = (PFNGLDRAWARRAYSINSTANCEDPROC) load(userptr, "glDrawArraysInstanced");
			    glad_glDrawElementsInstanced = (PFNGLDRAWELEMENTSINSTANCEDPROC) load(userptr, "glDrawElementsInstanced");
			    glad_glGetActiveUniformBlockName = (PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC) load(userptr, "glGetActiveUniformBlockName");
			    glad_glGetActiveUniformBlockiv = (PFNGLGETACTIVEUNIFORMBLOCKIVPROC) load(userptr, "glGetActiveUniformBlockiv");
			    glad_glGetActiveUniformName = (PFNGLGETACTIVEUNIFORMNAMEPROC) load(userptr, "glGetActiveUniformName");
			    glad_glGetActiveUniformsiv = (PFNGLGETACTIVEUNIFORMSIVPROC) load(userptr, "glGetActiveUniformsiv");
			    glad_glGetIntegeri_v = (PFNGLGETINTEGERI_VPROC) load(userptr, "glGetIntegeri_v");
			    glad_glGetUniformBlockIndex = (PFNGLGETUNIFORMBLOCKINDEXPROC) load(userptr, "glGetUniformBlockIndex");
			    glad_glGetUniformIndices = (PFNGLGETUNIFORMINDICESPROC) load(userptr, "glGetUniformIndices");
			    glad_glPrimitiveRestartIndex = (PFNGLPRIMITIVERESTARTINDEXPROC) load(userptr, "glPrimitiveRestartIndex");
			    glad_glTexBuffer = (PFNGLTEXBUFFERPROC) load(userptr, "glTexBuffer");
			    glad_glUniformBlockBinding = (PFNGLUNIFORMBLOCKBINDINGPROC) load(userptr, "glUniformBlockBinding");
			}
			static void glad_gl_load_GL_VERSION_3_2( GLADuserptrloadfunc load, void* userptr) {
			    if(!GLAD_GL_VERSION_3_2) return;
			    glad_glClientWaitSync = (PFNGLCLIENTWAITSYNCPROC) load(userptr, "glClientWaitSync");
			    glad_glDeleteSync = (PFNGLDELETESYNCPROC) load(userptr, "glDeleteSync");
			    glad_glDrawElementsBaseVertex = (PFNGLDRAWELEMENTSBASEVERTEXPROC) load(userptr, "glDrawElementsBaseVertex");
			    glad_glDrawElementsInstancedBaseVertex = (PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC) load(userptr, "glDrawElementsInstancedBaseVertex");
			    glad_glDrawRangeElementsBaseVertex = (PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC) load(userptr, "glDrawRangeElementsBaseVertex");
			    glad_glFenceSync = (PFNGLFENCESYNCPROC) load(userptr, "glFenceSync");
			    glad_glFramebufferTexture = (PFNGLFRAMEBUFFERTEXTUREPROC) load(userptr, "glFramebufferTexture");
			    glad_glGetBufferParameteri64v = (PFNGLGETBUFFERPARAMETERI64VPROC) load(userptr, "glGetBufferParameteri64v");
			    glad_glGetInteger64i_v = (PFNGLGETINTEGER64I_VPROC) load(userptr, "glGetInteger64i_v");
			    glad_glGetInteger64v = (PFNGLGETINTEGER64VPROC) load(userptr, "glGetInteger64v");
			    glad_glGetMultisamplefv = (PFNGLGETMULTISAMPLEFVPROC) load(userptr, "glGetMultisamplefv");
			    glad_glGetSynciv = (PFNGLGETSYNCIVPROC) load(userptr, "glGetSynciv");
			    glad_glIsSync = (PFNGLISSYNCPROC) load(userptr, "glIsSync");
			    glad_glMultiDrawElementsBaseVertex = (PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC) load(userptr, "glMultiDrawElementsBaseVertex");
			    glad_glProvokingVertex = (PFNGLPROVOKINGVERTEXPROC) load(userptr, "glProvokingVertex");
			    glad_glSampleMaski = (PFNGLSAMPLEMASKIPROC) load(userptr, "glSampleMaski");
			    glad_glTexImage2DMultisample = (PFNGLTEXIMAGE2DMULTISAMPLEPROC) load(userptr, "glTexImage2DMultisample");
			    glad_glTexImage3DMultisample = (PFNGLTEXIMAGE3DMULTISAMPLEPROC) load(userptr, "glTexImage3DMultisample");
			    glad_glWaitSync = (PFNGLWAITSYNCPROC) load(userptr, "glWaitSync");
			}
			static void glad_gl_load_GL_VERSION_3_3( GLADuserptrloadfunc load, void* userptr) {
			    if(!GLAD_GL_VERSION_3_3) return;
			    glad_glBindFragDataLocationIndexed = (PFNGLBINDFRAGDATALOCATIONINDEXEDPROC) load(userptr, "glBindFragDataLocationIndexed");
			    glad_glBindSampler = (PFNGLBINDSAMPLERPROC) load(userptr, "glBindSampler");
			    glad_glDeleteSamplers = (PFNGLDELETESAMPLERSPROC) load(userptr, "glDeleteSamplers");
			    glad_glGenSamplers = (PFNGLGENSAMPLERSPROC) load(userptr, "glGenSamplers");
			    glad_glGetFragDataIndex = (PFNGLGETFRAGDATAINDEXPROC) load(userptr, "glGetFragDataIndex");
			    glad_glGetQueryObjecti64v = (PFNGLGETQUERYOBJECTI64VPROC) load(userptr, "glGetQueryObjecti64v");
			    glad_glGetQueryObjectui64v = (PFNGLGETQUERYOBJECTUI64VPROC) load(userptr, "glGetQueryObjectui64v");
			    glad_glGetSamplerParameterIiv = (PFNGLGETSAMPLERPARAMETERIIVPROC) load(userptr, "glGetSamplerParameterIiv");
			    glad_glGetSamplerParameterIuiv = (PFNGLGETSAMPLERPARAMETERIUIVPROC) load(userptr, "glGetSamplerParameterIuiv");
			    glad_glGetSamplerParameterfv = (PFNGLGETSAMPLERPARAMETERFVPROC) load(userptr, "glGetSamplerParameterfv");
			    glad_glGetSamplerParameteriv = (PFNGLGETSAMPLERPARAMETERIVPROC) load(userptr, "glGetSamplerParameteriv");
			    glad_glIsSampler = (PFNGLISSAMPLERPROC) load(userptr, "glIsSampler");
			    glad_glQueryCounter = (PFNGLQUERYCOUNTERPROC) load(userptr, "glQueryCounter");
			    glad_glSamplerParameterIiv = (PFNGLSAMPLERPARAMETERIIVPROC) load(userptr, "glSamplerParameterIiv");
			    glad_glSamplerParameterIuiv = (PFNGLSAMPLERPARAMETERIUIVPROC) load(userptr, "glSamplerParameterIuiv");
			    glad_glSamplerParameterf = (PFNGLSAMPLERPARAMETERFPROC) load(userptr, "glSamplerParameterf");
			    glad_glSamplerParameterfv = (PFNGLSAMPLERPARAMETERFVPROC) load(userptr, "glSamplerParameterfv");
			    glad_glSamplerParameteri = (PFNGLSAMPLERPARAMETERIPROC) load(userptr, "glSamplerParameteri");
			    glad_glSamplerParameteriv = (PFNGLSAMPLERPARAMETERIVPROC) load(userptr, "glSamplerParameteriv");
			    glad_glVertexAttribDivisor = (PFNGLVERTEXATTRIBDIVISORPROC) load(userptr, "glVertexAttribDivisor");
			    glad_glVertexAttribP1ui = (PFNGLVERTEXATTRIBP1UIPROC) load(userptr, "glVertexAttribP1ui");
			    glad_glVertexAttribP1uiv = (PFNGLVERTEXATTRIBP1UIVPROC) load(userptr, "glVertexAttribP1uiv");
			    glad_glVertexAttribP2ui = (PFNGLVERTEXATTRIBP2UIPROC) load(userptr, "glVertexAttribP2ui");
			    glad_glVertexAttribP2uiv = (PFNGLVERTEXATTRIBP2UIVPROC) load(userptr, "glVertexAttribP2uiv");
			    glad_glVertexAttribP3ui = (PFNGLVERTEXATTRIBP3UIPROC) load(userptr, "glVertexAttribP3ui");
			    glad_glVertexAttribP3uiv = (PFNGLVERTEXATTRIBP3UIVPROC) load(userptr, "glVertexAttribP3uiv");
			    glad_glVertexAttribP4ui = (PFNGLVERTEXATTRIBP4UIPROC) load(userptr, "glVertexAttribP4ui");
			    glad_glVertexAttribP4uiv = (PFNGLVERTEXATTRIBP4UIVPROC) load(userptr, "glVertexAttribP4uiv");
			}



			static void glad_gl_free_extensions(char **exts_i) {
			    if (exts_i != NULL) {
			        unsigned int index;
			        for(index = 0; exts_i[index]; index++) {
			            free((void *) (exts_i[index]));
			        }
			        free((void *)exts_i);
			        exts_i = NULL;
			    }
			}
			static int glad_gl_get_extensions( const char **out_exts, char ***out_exts_i) {
			#if defined(GL_ES_VERSION_3_0) || defined(GL_VERSION_3_0)
			    if (glad_glGetStringi != NULL && glad_glGetIntegerv != NULL) {
			        unsigned int index = 0;
			        unsigned int num_exts_i = 0;
			        char **exts_i = NULL;
			        glad_glGetIntegerv(GL_NUM_EXTENSIONS, (int*) &num_exts_i);
			        exts_i = (char **) malloc((num_exts_i + 1) * (sizeof *exts_i));
			        if (exts_i == NULL) {
			            return 0;
			        }
			        for(index = 0; index < num_exts_i; index++) {
			            const char *gl_str_tmp = (const char*) glad_glGetStringi(GL_EXTENSIONS, index);
			            size_t len = strlen(gl_str_tmp) + 1;

			            char *local_str = (char*) malloc(len * sizeof(char));
			            if(local_str == NULL) {
			                exts_i[index] = NULL;
			                glad_gl_free_extensions(exts_i);
			                return 0;
			            }

			            memcpy(local_str, gl_str_tmp, len * sizeof(char));
			            exts_i[index] = local_str;
			        }
			        exts_i[index] = NULL;

			        *out_exts_i = exts_i;

			        return 1;
			    }
			#else
			    GLAD_UNUSED(out_exts_i);
			#endif
			    if (glad_glGetString == NULL) {
			        return 0;
			    }
			    *out_exts = (const char *)glad_glGetString(GL_EXTENSIONS);
			    return 1;
			}
			static int glad_gl_has_extension(const char *exts, char **exts_i, const char *ext) {
			    if(exts_i) {
			        unsigned int index;
			        for(index = 0; exts_i[index]; index++) {
			            const char *e = exts_i[index];
			            if(strcmp(e, ext) == 0) {
			                return 1;
			            }
			        }
			    } else {
			        const char *extensions;
			        const char *loc;
			        const char *terminator;
			        extensions = exts;
			        if(extensions == NULL || ext == NULL) {
			            return 0;
			        }
			        while(1) {
			            loc = strstr(extensions, ext);
			            if(loc == NULL) {
			                return 0;
			            }
			            terminator = loc + strlen(ext);
			            if((loc == extensions || *(loc - 1) == ' ') &&
			                (*terminator == ' ' || *terminator == '\0')) {
			                return 1;
			            }
			            extensions = terminator;
			        }
			    }
			    return 0;
			}

			static GLADapiproc glad_gl_get_proc_from_userptr(void *userptr, const char* name) {
			    return (GLAD_GNUC_EXTENSION (GLADapiproc (*)(const char *name)) userptr)(name);
			}

			static int glad_gl_find_extensions_gl(void) {
			    const char *exts = NULL;
			    char **exts_i = NULL;
			    if (!glad_gl_get_extensions(&exts, &exts_i)) return 0;

			    GLAD_UNUSED(glad_gl_has_extension);

			    glad_gl_free_extensions(exts_i);

			    return 1;
			}

			static int glad_gl_find_core_gl(void) {
			    int i;
			    const char* version;
			    const char* prefixes[] = {
			        "OpenGL ES-CM ",
			        "OpenGL ES-CL ",
			        "OpenGL ES ",
			        "OpenGL SC ",
			        NULL
			    };
			    int major = 0;
			    int minor = 0;
			    version = (const char*) glad_glGetString(GL_VERSION);
			    if (!version) return 0;
			    for (i = 0;  prefixes[i];  i++) {
			        const size_t length = strlen(prefixes[i]);
			        if (strncmp(version, prefixes[i], length) == 0) {
			            version += length;
			            break;
			        }
			    }

			    GLAD_IMPL_UTIL_SSCANF(version, "%d.%d", &major, &minor);

			    GLAD_GL_VERSION_1_0 = (major == 1 && minor >= 0) || major > 1;
			    GLAD_GL_VERSION_1_1 = (major == 1 && minor >= 1) || major > 1;
			    GLAD_GL_VERSION_1_2 = (major == 1 && minor >= 2) || major > 1;
			    GLAD_GL_VERSION_1_3 = (major == 1 && minor >= 3) || major > 1;
			    GLAD_GL_VERSION_1_4 = (major == 1 && minor >= 4) || major > 1;
			    GLAD_GL_VERSION_1_5 = (major == 1 && minor >= 5) || major > 1;
			    GLAD_GL_VERSION_2_0 = (major == 2 && minor >= 0) || major > 2;
			    GLAD_GL_VERSION_2_1 = (major == 2 && minor >= 1) || major > 2;
			    GLAD_GL_VERSION_3_0 = (major == 3 && minor >= 0) || major > 3;
			    GLAD_GL_VERSION_3_1 = (major == 3 && minor >= 1) || major > 3;
			    GLAD_GL_VERSION_3_2 = (major == 3 && minor >= 2) || major > 3;
			    GLAD_GL_VERSION_3_3 = (major == 3 && minor >= 3) || major > 3;

			    return GLAD_MAKE_VERSION(major, minor);
			}

			int gladLoadGLUserPtr( GLADuserptrloadfunc load, void *userptr) {
			    int version;

			    glad_glGetString = (PFNGLGETSTRINGPROC) load(userptr, "glGetString");
			    if(glad_glGetString == NULL) return 0;
			    version = glad_gl_find_core_gl();

			    glad_gl_load_GL_VERSION_1_0(load, userptr);
			    glad_gl_load_GL_VERSION_1_1(load, userptr);
			    glad_gl_load_GL_VERSION_1_2(load, userptr);
			    glad_gl_load_GL_VERSION_1_3(load, userptr);
			    glad_gl_load_GL_VERSION_1_4(load, userptr);
			    glad_gl_load_GL_VERSION_1_5(load, userptr);
			    glad_gl_load_GL_VERSION_2_0(load, userptr);
			    glad_gl_load_GL_VERSION_2_1(load, userptr);
			    glad_gl_load_GL_VERSION_3_0(load, userptr);
			    glad_gl_load_GL_VERSION_3_1(load, userptr);
			    glad_gl_load_GL_VERSION_3_2(load, userptr);
			    glad_gl_load_GL_VERSION_3_3(load, userptr);

			    if (!glad_gl_find_extensions_gl()) return 0;



			    return version;
			}


			int gladLoadGL( GLADloadfunc load) {
			    return gladLoadGLUserPtr( glad_gl_get_proc_from_userptr, GLAD_GNUC_EXTENSION (void*) load);
			}



			 

			#ifdef GLAD_GL

			#ifndef GLAD_LOADER_LIBRARY_C_
			#define GLAD_LOADER_LIBRARY_C_

			#include <stddef.h>
			#include <stdlib.h>

			#if GLAD_PLATFORM_WIN32
			#include <windows.h>
			#else
			#include <dlfcn.h>
			#endif


			static void* glad_get_dlopen_handle(const char *lib_names[], int length) {
			    void *handle = NULL;
			    int i;

			    for (i = 0; i < length; ++i) {
			#if GLAD_PLATFORM_WIN32
			  #if GLAD_PLATFORM_UWP
			        size_t buffer_size = (strlen(lib_names[i]) + 1) * sizeof(WCHAR);
			        LPWSTR buffer = (LPWSTR) malloc(buffer_size);
			        if (buffer != NULL) {
			            int ret = MultiByteToWideChar(CP_ACP, 0, lib_names[i], -1, buffer, buffer_size);
			            if (ret != 0) {
			                handle = (void*) LoadPackagedLibrary(buffer, 0);
			            }
			            free((void*) buffer);
			        }
			  #else
			        handle = (void*) LoadLibraryA(lib_names[i]);
			  #endif
			#else
			        handle = dlopen(lib_names[i], RTLD_LAZY | RTLD_LOCAL);
			#endif
			        if (handle != NULL) {
			            return handle;
			        }
			    }

			    return NULL;
			}

			static void glad_close_dlopen_handle(void* handle) {
			    if (handle != NULL) {
			#if GLAD_PLATFORM_WIN32
			        FreeLibrary((HMODULE) handle);
			#else
			        dlclose(handle);
			#endif
			    }
			}

			static GLADapiproc glad_dlsym_handle(void* handle, const char *name) {
			    if (handle == NULL) {
			        return NULL;
			    }

			#if GLAD_PLATFORM_WIN32
			    return (GLADapiproc) GetProcAddress((HMODULE) handle, name);
			#else
			    return GLAD_GNUC_EXTENSION (GLADapiproc) dlsym(handle, name);
			#endif
			}

			#endif /* GLAD_LOADER_LIBRARY_C_ */

			typedef void* (GLAD_API_PTR *GLADglprocaddrfunc)(const char*);
			struct _glad_gl_userptr {
			    void *handle;
			    GLADglprocaddrfunc gl_get_proc_address_ptr;
			};

			static GLADapiproc glad_gl_get_proc(void *vuserptr, const char *name) {
			    struct _glad_gl_userptr userptr = *(struct _glad_gl_userptr*) vuserptr;
			    GLADapiproc result = NULL;

			    if(userptr.gl_get_proc_address_ptr != NULL) {
			        result = GLAD_GNUC_EXTENSION (GLADapiproc) userptr.gl_get_proc_address_ptr(name);
			    }
			    if(result == NULL) {
			        result = glad_dlsym_handle(userptr.handle, name);
			    }

			    return result;
			}

			static void* _glad_GL_loader_handle = NULL;

			static void* glad_gl_dlopen_handle(void) {
			#if GLAD_PLATFORM_APPLE
			    static const char *NAMES[] = {
			        "../Frameworks/OpenGL.framework/OpenGL",
			        "/Library/Frameworks/OpenGL.framework/OpenGL",
			        "/System/Library/Frameworks/OpenGL.framework/OpenGL",
			        "/System/Library/Frameworks/OpenGL.framework/Versions/Current/OpenGL"
			    };
			#elif GLAD_PLATFORM_WIN32
			    static const char *NAMES[] = {"opengl32.dll"};
			#else
			    static const char *NAMES[] = {
			  #if defined(__CYGWIN__)
			        "libGL-1.so",
			  #endif
			        "libGL.so.1",
			        "libGL.so"
			    };
			#endif

			    if (_glad_GL_loader_handle == NULL) {
			        _glad_GL_loader_handle = glad_get_dlopen_handle(NAMES, sizeof(NAMES) / sizeof(NAMES[0]));
			    }

			    return _glad_GL_loader_handle;
			}

			static struct _glad_gl_userptr glad_gl_build_userptr(void *handle) {
			    struct _glad_gl_userptr userptr;

			    userptr.handle = handle;
			#if GLAD_PLATFORM_APPLE || defined(__HAIKU__)
			    userptr.gl_get_proc_address_ptr = NULL;
			#elif GLAD_PLATFORM_WIN32
			    userptr.gl_get_proc_address_ptr =
			        (GLADglprocaddrfunc) glad_dlsym_handle(handle, "wglGetProcAddress");
			#else
			    userptr.gl_get_proc_address_ptr =
			        (GLADglprocaddrfunc) glad_dlsym_handle(handle, "glXGetProcAddressARB");
			#endif

			    return userptr;
			}

			int gladLoaderLoadGL(void) {
			    int version = 0;
			    void *handle;
			    int did_load = 0;
			    struct _glad_gl_userptr userptr;

			    did_load = _glad_GL_loader_handle == NULL;
			    handle = glad_gl_dlopen_handle();
			    if (handle) {
			        userptr = glad_gl_build_userptr(handle);

			        version = gladLoadGLUserPtr(glad_gl_get_proc, &userptr);

			        if (did_load) {
			            gladLoaderUnloadGL();
			        }
			    }

			    return version;
			}



			void gladLoaderUnloadGL(void) {
			    if (_glad_GL_loader_handle != NULL) {
			        glad_close_dlopen_handle(_glad_GL_loader_handle);
			        _glad_GL_loader_handle = NULL;
			    }
			}

			#endif /* GLAD_GL */

			#ifdef __cplusplus
			}
			#endif
		//#endif /* GLAD_GL_IMPLEMENTATION */
		#endif /* MU_SUPPORT_OPENGL */

		// muCOSA implementation
		#ifndef MUCOSA_IMPLEMENTATION
			#define MUCOSA_IMPLEMENTATION
			#ifdef MUCOSA_IMPLEMENTATION

				MU_CPP_EXTERN_START

				/* Macros */

					// OS recognition

					#ifndef MUCOSA_MANUAL_OS_SUPPORT

						// Win32
						#ifdef MU_WIN32
							#define MUCOSA_WIN32
							#define MUCOSA_WIN32_CALL(...) __VA_ARGS__
						#else
							#define MUCOSA_WIN32_CALL(...)
						#endif

					#endif /* MUCOSA_MANUAL_OS_SUPPORT */

					// Graphics API support

					#ifdef MU_SUPPORT_OPENGL
						#define MUCOSA_OPENGL_CALL(...) __VA_ARGS__
					#else
						#define MUCOSA_OPENGL_CALL(...)
					#endif

				/* Win32 */

				#ifdef MUCOSA_WIN32

					// For all of the stuff that can't be included directly (CURSE YOU BILL)
					#include <windows.h>

					/* Useful functions */

						// Hinstance retriever (thank you Raymond Chen)
						EXTERN_C IMAGE_DOS_HEADER __ImageBase;
						HINSTANCE muCOSAW32_get_hinstance(void) {
							return ((HINSTANCE)&__ImageBase);
						}

						// UTF-8 -> wchar_t (UTF-16)
						wchar_t* muCOSAW32_utf8_to_wchar(char* str) {
							// Get length needed for conversion
							// Note: we can use -1 here, since it's expected for it to be null-terminated
							int len = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
							if (len == 0) {
								return 0;
							}

							// Allocate data
							wchar_t* wstr = (wchar_t*)mu_malloc(len * sizeof(wchar_t));
							if (wstr == 0) {
								return 0;
							}

							// Perform conversion
							if (MultiByteToWideChar(CP_UTF8, 0, str, -1, (LPWSTR)wstr, len) == 0) {
								mu_free(wstr);
								return 0;
							}

							// Return converted data
							return wstr;
						}

						// wchar_t (UTF-16) -> UTF-8
						uint8_m* muCOSAW32_wchar_to_utf8(wchar_t* wstr) {
							// Get length needed
							int len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
							if (!len) {
								return 0;
							}

							// Allocate data needed for conversion
							uint8_m* str = (uint8_m*)mu_malloc(len);
							if (!str) {
								return 0;
							}

							// Perform conversion
							if (!WideCharToMultiByte(CP_UTF8, 0, wstr, -1, (LPSTR)str, len, NULL, NULL)) {
								mu_free(str);
								return 0;
							}

							return str;
						}

						// Win32 virtual key code to muCOSA key code
						muKeyboardKey muCOSAW32_VK_to_muCOSA(WPARAM vk) {
							switch (vk) {
								default: return MU_KEYBOARD_UNKNOWN; break;
								case VK_BACK: return MU_KEYBOARD_BACKSPACE; break;
								case VK_TAB: return MU_KEYBOARD_TAB; break;
								case VK_CLEAR: return MU_KEYBOARD_CLEAR; break;
								case VK_RETURN: return MU_KEYBOARD_RETURN; break;
								case VK_PAUSE: return MU_KEYBOARD_PAUSE; break;
								case VK_ESCAPE: return MU_KEYBOARD_ESCAPE; break;
								case VK_MODECHANGE: return MU_KEYBOARD_MODECHANGE; break;
								case VK_SPACE: return MU_KEYBOARD_SPACE; break;
								case VK_PRIOR: return MU_KEYBOARD_PRIOR; break;
								case VK_NEXT: return MU_KEYBOARD_NEXT; break;
								case VK_END: return MU_KEYBOARD_END; break;
								case VK_HOME: return MU_KEYBOARD_HOME; break;
								case VK_LEFT: return MU_KEYBOARD_LEFT; break;
								case VK_UP: return MU_KEYBOARD_UP; break;
								case VK_RIGHT: return MU_KEYBOARD_RIGHT; break;
								case VK_DOWN: return MU_KEYBOARD_DOWN; break;
								case VK_SELECT: return MU_KEYBOARD_SELECT; break;
								case VK_PRINT: return MU_KEYBOARD_PRINT; break;
								case VK_EXECUTE: return MU_KEYBOARD_EXECUTE; break;
								case VK_INSERT: return MU_KEYBOARD_INSERT; break;
								case VK_DELETE: return MU_KEYBOARD_DELETE; break;
								case VK_HELP: return MU_KEYBOARD_HELP; break;
								case 0x30: return MU_KEYBOARD_0; break;
								case 0x31: return MU_KEYBOARD_1; break;
								case 0x32: return MU_KEYBOARD_2; break;
								case 0x33: return MU_KEYBOARD_3; break;
								case 0x34: return MU_KEYBOARD_4; break;
								case 0x35: return MU_KEYBOARD_5; break;
								case 0x36: return MU_KEYBOARD_6; break;
								case 0x37: return MU_KEYBOARD_7; break;
								case 0x38: return MU_KEYBOARD_8; break;
								case 0x39: return MU_KEYBOARD_9; break;
								case 0x41: return MU_KEYBOARD_A; break;
								case 0x42: return MU_KEYBOARD_B; break;
								case 0x43: return MU_KEYBOARD_C; break;
								case 0x44: return MU_KEYBOARD_D; break;
								case 0x45: return MU_KEYBOARD_E; break;
								case 0x46: return MU_KEYBOARD_F; break;
								case 0x47: return MU_KEYBOARD_G; break;
								case 0x48: return MU_KEYBOARD_H; break;
								case 0x49: return MU_KEYBOARD_I; break;
								case 0x4A: return MU_KEYBOARD_J; break;
								case 0x4B: return MU_KEYBOARD_K; break;
								case 0x4C: return MU_KEYBOARD_L; break;
								case 0x4D: return MU_KEYBOARD_M; break;
								case 0x4E: return MU_KEYBOARD_N; break;
								case 0x4F: return MU_KEYBOARD_O; break;
								case 0x50: return MU_KEYBOARD_P; break;
								case 0x51: return MU_KEYBOARD_Q; break;
								case 0x52: return MU_KEYBOARD_R; break;
								case 0x53: return MU_KEYBOARD_S; break;
								case 0x54: return MU_KEYBOARD_T; break;
								case 0x55: return MU_KEYBOARD_U; break;
								case 0x56: return MU_KEYBOARD_V; break;
								case 0x57: return MU_KEYBOARD_W; break;
								case 0x58: return MU_KEYBOARD_X; break;
								case 0x59: return MU_KEYBOARD_Y; break;
								case 0x5A: return MU_KEYBOARD_Z; break;
								case VK_LWIN: return MU_KEYBOARD_LEFT_WINDOWS; break;
								case VK_RWIN: return MU_KEYBOARD_RIGHT_WINDOWS; break;
								case VK_NUMPAD0: return MU_KEYBOARD_NUMPAD_0; break;
								case VK_NUMPAD1: return MU_KEYBOARD_NUMPAD_1; break;
								case VK_NUMPAD2: return MU_KEYBOARD_NUMPAD_2; break;
								case VK_NUMPAD3: return MU_KEYBOARD_NUMPAD_3; break;
								case VK_NUMPAD4: return MU_KEYBOARD_NUMPAD_4; break;
								case VK_NUMPAD5: return MU_KEYBOARD_NUMPAD_5; break;
								case VK_NUMPAD6: return MU_KEYBOARD_NUMPAD_6; break;
								case VK_NUMPAD7: return MU_KEYBOARD_NUMPAD_7; break;
								case VK_NUMPAD8: return MU_KEYBOARD_NUMPAD_8; break;
								case VK_NUMPAD9: return MU_KEYBOARD_NUMPAD_9; break;
								case VK_MULTIPLY: return MU_KEYBOARD_MULTIPLY; break;
								case VK_ADD: return MU_KEYBOARD_ADD; break;
								case VK_SEPARATOR: return MU_KEYBOARD_SEPARATOR; break;
								case VK_SUBTRACT: return MU_KEYBOARD_SUBTRACT; break;
								case VK_DECIMAL: return MU_KEYBOARD_DECIMAL; break;
								case VK_DIVIDE: return MU_KEYBOARD_DIVIDE; break;
								case VK_F1: return MU_KEYBOARD_F1; break;
								case VK_F2: return MU_KEYBOARD_F2; break;
								case VK_F3: return MU_KEYBOARD_F3; break;
								case VK_F4: return MU_KEYBOARD_F4; break;
								case VK_F5: return MU_KEYBOARD_F5; break;
								case VK_F6: return MU_KEYBOARD_F6; break;
								case VK_F7: return MU_KEYBOARD_F7; break;
								case VK_F8: return MU_KEYBOARD_F8; break;
								case VK_F9: return MU_KEYBOARD_F9; break;
								case VK_F10: return MU_KEYBOARD_F10; break;
								case VK_F11: return MU_KEYBOARD_F11; break;
								case VK_F12: return MU_KEYBOARD_F12; break;
								case VK_F13: return MU_KEYBOARD_F13; break;
								case VK_F14: return MU_KEYBOARD_F14; break;
								case VK_F15: return MU_KEYBOARD_F15; break;
								case VK_F16: return MU_KEYBOARD_F16; break;
								case VK_F17: return MU_KEYBOARD_F17; break;
								case VK_F18: return MU_KEYBOARD_F18; break;
								case VK_F19: return MU_KEYBOARD_F19; break;
								case VK_F20: return MU_KEYBOARD_F20; break;
								case VK_F21: return MU_KEYBOARD_F21; break;
								case VK_F22: return MU_KEYBOARD_F22; break;
								case VK_F23: return MU_KEYBOARD_F23; break;
								case VK_F24: return MU_KEYBOARD_F24; break;
								case VK_NUMLOCK: return MU_KEYBOARD_NUMLOCK; break;
								case VK_SCROLL: return MU_KEYBOARD_SCROLL; break;
								case VK_LSHIFT: return MU_KEYBOARD_LEFT_SHIFT; break;
								case VK_RSHIFT: return MU_KEYBOARD_RIGHT_SHIFT; break;
								case VK_LCONTROL: return MU_KEYBOARD_LEFT_CONTROL; break;
								case VK_RCONTROL: return MU_KEYBOARD_RIGHT_CONTROL; break;
								case VK_LMENU: return MU_KEYBOARD_LEFT_MENU; break;
								case VK_RMENU: return MU_KEYBOARD_RIGHT_MENU; break;
								case VK_ATTN: return MU_KEYBOARD_ATTN; break;
								case VK_CRSEL: return MU_KEYBOARD_CRSEL; break;
								case VK_EXSEL: return MU_KEYBOARD_EXSEL; break;
								case VK_EREOF: return MU_KEYBOARD_EREOF; break;
								case VK_PLAY: return MU_KEYBOARD_PLAY; break;
								case VK_PA1: return MU_KEYBOARD_PA1; break;
							}
						}

						// muCOSA cursor style to Win32 cursor
						void* muCOSAW32_muCOSA_cursor_to_W32(muCursorStyle style) {
							switch (style) {
								default: return IDC_ARROW; break;
								case MU_CURSOR_ARROW: return IDC_ARROW; break;
								case MU_CURSOR_IBEAM: return IDC_IBEAM; break;
								case MU_CURSOR_WAIT: return IDC_WAIT; break;
								case MU_CURSOR_WAIT_ARROW: return IDC_APPSTARTING; break;
								case MU_CURSOR_CROSSHAIR: return IDC_CROSS; break;
								case MU_CURSOR_HAND: return IDC_HAND; break;
								case MU_CURSOR_SIZE_LR: return IDC_SIZEWE; break;
								case MU_CURSOR_SIZE_TB: return IDC_SIZENS; break;
								case MU_CURSOR_SIZE_TL_BR: return IDC_SIZENWSE; break;
								case MU_CURSOR_SIZE_TR_BL: return IDC_SIZENESW; break;
								case MU_CURSOR_SIZE_ALL: return IDC_SIZEALL; break;
								case MU_CURSOR_NO: return IDC_NO; break;
							}
						}

						// muCOSA keystate to Win32
						int muCOSAW32_keystate_to_W32(muKeyboardState state) {
							switch (state) {
								default: return VK_NONAME; break;
								case MU_KEYSTATE_CAPS_LOCK: return VK_CAPITAL; break;
								case MU_KEYSTATE_SCROLL_LOCK: return VK_SCROLL; break;
								case MU_KEYSTATE_NUM_LOCK: return VK_NUMLOCK; break;
							}
						}

					/* OpenGL */

					#ifdef MU_SUPPORT_OPENGL

						/* Inclusion and common definition */

							// Common OpenGL definitions
							#ifndef MUCOSA_NO_INCLUDE_OPENGL
								// Only include if "glClearColor" isn't defined; this is to avoid
								// accidental double definition in many circumstances.
								#ifndef glClearColor
									#include <gl/gl.h>
									#include <gl/glu.h>
								#endif
							#endif

							// Device context -> valid OpenGL context
							#include <wingdi.h>

							// Typedefs for functions we need
							// - wglCreateContextAttribs; needed for specifying exact version when creating context
							typedef HGLRC WINAPI muCOSAW32_wglCreateContextAttribsARB_type(HDC hdc, HGLRC hShareContext, const int* attribList);
							// - wglChoosePixelFormat; needed for choosing a more thorough pixel format when creating context
							typedef BOOL WINAPI muCOSAW32_wglChoosePixelFormatARB_type(HDC hdc, const int* piAttribIList, const FLOAT* pfAttribFList, UINT uMaxFormats, int* piFormats, UINT* nNumFormats);
							// - wglSwapInterval; not needed in context creation, but used for swap interval later
							typedef BOOL WINAPI muCOSAW32_wglSwapIntervalEXT_type(int interval);

							// Struct to hold WGL functions needed for context creation
							struct muCOSAW32_WGL {
								muCOSAW32_wglCreateContextAttribsARB_type* CreateContextAttribs;
								muCOSAW32_wglChoosePixelFormatARB_type* ChoosePixelFormat;
								muCOSAW32_wglSwapIntervalEXT_type* SwapInterval;
							};
							typedef struct muCOSAW32_WGL muCOSAW32_WGL;

						/* WGL tokens */

							// https://registry.khronos.org/OpenGL/extensions/ARB/WGL_ARB_create_context.txt

							#define MUCOSAWGL_CONTEXT_MAJOR_VERSION_ARB             0x2091
							#define MUCOSAWGL_CONTEXT_MINOR_VERSION_ARB             0x2092
							#define MUCOSAWGL_CONTEXT_LAYER_PLANE_ARB               0x2093
							#define MUCOSAWGL_CONTEXT_FLAGS_ARB                     0x2094
							#define MUCOSAWGL_CONTEXT_PROFILE_MASK_ARB              0x9126

							#define MUCOSAWGL_CONTEXT_DEBUG_BIT_ARB                 0x0001
							#define MUCOSAWGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB    0x0002

							#define MUCOSAWGL_CONTEXT_CORE_PROFILE_BIT_ARB          0x00000001
							#define MUCOSAWGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002

							#define MUCOSAWGL_ERROR_INVALID_VERSION_ARB                 0x2095
							#define MUCOSAWGL_ERROR_INVALID_PROFILE_ARB                 0x2096

							// https://registry.khronos.org/OpenGL/extensions/ARB/WGL_ARB_pixel_format.txt

							#define MUCOSAWGL_NUMBER_PIXEL_FORMATS_ARB    0x2000
							#define MUCOSAWGL_DRAW_TO_WINDOW_ARB          0x2001
							#define MUCOSAWGL_DRAW_TO_BITMAP_ARB          0x2002
							#define MUCOSAWGL_ACCELERATION_ARB            0x2003
							#define MUCOSAWGL_NEED_PALETTE_ARB            0x2004
							#define MUCOSAWGL_NEED_SYSTEM_PALETTE_ARB     0x2005
							#define MUCOSAWGL_SWAP_LAYER_BUFFERS_ARB      0x2006
							#define MUCOSAWGL_SWAP_METHOD_ARB             0x2007
							#define MUCOSAWGL_NUMBER_OVERLAYS_ARB         0x2008
							#define MUCOSAWGL_NUMBER_UNDERLAYS_ARB        0x2009
							#define MUCOSAWGL_TRANSPARENT_ARB             0x200A
							#define MUCOSAWGL_TRANSPARENT_RED_VALUE_ARB   0x2037
							#define MUCOSAWGL_TRANSPARENT_GREEN_VALUE_ARB 0x2038
							#define MUCOSAWGL_TRANSPARENT_BLUE_VALUE_ARB  0x2039
							#define MUCOSAWGL_TRANSPARENT_ALPHA_VALUE_ARB 0x203A
							#define MUCOSAWGL_TRANSPARENT_INDEX_VALUE_ARB 0x203B
							#define MUCOSAWGL_SHARE_DEPTH_ARB             0x200C
							#define MUCOSAWGL_SHARE_STENCIL_ARB           0x200D
							#define MUCOSAWGL_SHARE_ACCUM_ARB             0x200E
							#define MUCOSAWGL_SUPPORT_GDI_ARB             0x200F
							#define MUCOSAWGL_SUPPORT_OPENGL_ARB          0x2010
							#define MUCOSAWGL_DOUBLE_BUFFER_ARB           0x2011
							#define MUCOSAWGL_STEREO_ARB                  0x2012
							#define MUCOSAWGL_PIXEL_TYPE_ARB              0x2013
							#define MUCOSAWGL_COLOR_BITS_ARB              0x2014
							#define MUCOSAWGL_RED_BITS_ARB                0x2015
							#define MUCOSAWGL_RED_SHIFT_ARB               0x2016
							#define MUCOSAWGL_GREEN_BITS_ARB              0x2017
							#define MUCOSAWGL_GREEN_SHIFT_ARB             0x2018
							#define MUCOSAWGL_BLUE_BITS_ARB               0x2019
							#define MUCOSAWGL_BLUE_SHIFT_ARB              0x201A
							#define MUCOSAWGL_ALPHA_BITS_ARB              0x201B
							#define MUCOSAWGL_ALPHA_SHIFT_ARB             0x201C
							#define MUCOSAWGL_ACCUM_BITS_ARB              0x201D
							#define MUCOSAWGL_ACCUM_RED_BITS_ARB          0x201E
							#define MUCOSAWGL_ACCUM_GREEN_BITS_ARB        0x201F
							#define MUCOSAWGL_ACCUM_BLUE_BITS_ARB         0x2020
							#define MUCOSAWGL_ACCUM_ALPHA_BITS_ARB        0x2021
							#define MUCOSAWGL_DEPTH_BITS_ARB              0x2022
							#define MUCOSAWGL_STENCIL_BITS_ARB            0x2023
							#define MUCOSAWGL_AUX_BUFFERS_ARB             0x2024

							#define MUCOSAWGL_NO_ACCELERATION_ARB         0x2025
							#define MUCOSAWGL_GENERIC_ACCELERATION_ARB    0x2026
							#define MUCOSAWGL_FULL_ACCELERATION_ARB       0x2027

							#define MUCOSAWGL_SWAP_EXCHANGE_ARB           0x2028
							#define MUCOSAWGL_SWAP_COPY_ARB               0x2029
							#define MUCOSAWGL_SWAP_UNDEFINED_ARB          0x202A

							#define MUCOSAWGL_TYPE_RGBA_ARB               0x202B
							#define MUCOSAWGL_TYPE_COLORINDEX_ARB         0x202C

							// https://nehe.gamedev.net/tutorial/fullscreen_antialiasing/16008/

							#define MUCOSAWGL_SAMPLE_BUFFERS_ARB  0x2041
							#define MUCOSAWGL_SAMPLES_ARB     0x2042

						/* Functions */

							// Retrieves OpenGL extensions needed for context creation
							muCOSAResult muCOSAW32_get_opengl_extensions(muCOSAW32_WGL* wgl) {
								// Create temp window class
								WNDCLASSA wclass = MU_ZERO_STRUCT(WNDCLASSA);
								wclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
								wclass.lpfnWndProc = DefWindowProcA;
								wclass.hInstance = muCOSAW32_get_hinstance();
								wclass.lpszClassName = "Dummy WGL window class";
								if (!RegisterClassA(&wclass)) {
									return MUCOSA_WIN32_FAILED_REGISTER_DUMMY_WGL_WINDOW_CLASS;
								}

								// Create temp window
								HWND win = CreateWindowExA(
									0, wclass.lpszClassName, "Dummy WGL window", 0,
									CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0,
									wclass.hInstance, 0
								);
								if (win == NULL) {
									UnregisterClassA(wclass.lpszClassName, wclass.hInstance);
									return MUCOSA_WIN32_FAILED_CREATE_DUMMY_WGL_WINDOW;
								}

								// Get device context from this window
								HDC dc = GetDC(win);

								// Use pixel format number 1
								// This is a HACK and a half, but works
								int pixel_format = 1;
								PIXELFORMATDESCRIPTOR format_desc;
								if (!DescribePixelFormat(dc, pixel_format, sizeof(format_desc), &format_desc)) {
									ReleaseDC(win, dc);
									DestroyWindow(win);
									UnregisterClassA(wclass.lpszClassName, wclass.hInstance);
									return MUCOSA_WIN32_FAILED_DESCRIBE_DUMMY_WGL_PIXEL_FORMAT;
								}

								// Set pixel format. This doesn't make much sense, this is already definitionally
								// the pixel format of the device, but it doesn't work if I don't do this, so, :L
								// Note: this might not be okay, and we may need to specify SOME pixel format,
								// but I don't know.
								if (!SetPixelFormat(dc, pixel_format, &format_desc)) {
									ReleaseDC(win, dc);
									DestroyWindow(win);
									UnregisterClassA(wclass.lpszClassName, wclass.hInstance);
									return MUCOSA_WIN32_FAILED_SET_DUMMY_WGL_PIXEL_FORMAT;
								}

								// Create dummy context
								HGLRC context = wglCreateContext(dc);
								if (!context) {
									ReleaseDC(win, dc);
									DestroyWindow(win);
									UnregisterClassA(wclass.lpszClassName, wclass.hInstance);
									return MUCOSA_WIN32_FAILED_CREATE_DUMMY_WGL_CONTEXT;
								}
								// Bind dummy context
								if (!wglMakeCurrent(dc, context)) {
									wglDeleteContext(context);
									ReleaseDC(win, dc);
									DestroyWindow(win);
									UnregisterClassA(wclass.lpszClassName, wclass.hInstance);
									return MUCOSA_WIN32_FAILED_BIND_DUMMY_WGL_CONTEXT;
								}

								// Find necessary functions
								PROC wglCreateContextAttribsARB_proc = wglGetProcAddress("wglCreateContextAttribsARB");
								PROC wglChoosePixelFormatARB_proc    = wglGetProcAddress("wglChoosePixelFormatARB");
								// Find helpful functions
								PROC wglSwapIntervalEXT_proc         = wglGetProcAddress("wglSwapIntervalEXT");

								// Destroy resources
								wglMakeCurrent(dc, 0);
								wglDeleteContext(context);
								ReleaseDC(win, dc);
								DestroyWindow(win);
								UnregisterClassA(wclass.lpszClassName, wclass.hInstance);

								// Detect if any required functions went unfound
								if (!wglCreateContextAttribsARB_proc) {
									return MUCOSA_WIN32_FAILED_FIND_WGL_CREATE_CONTEXT_ATTRIBS;
								}
								if (!wglChoosePixelFormatARB_proc) {
									return MUCOSA_WIN32_FAILED_FIND_WGL_CHOOSE_PIXEL_FORMAT;
								}

								// Copy over functions
								mu_memcpy(&wgl->CreateContextAttribs, &wglCreateContextAttribsARB_proc, sizeof(PROC));
								mu_memcpy(&wgl->ChoosePixelFormat,    &wglChoosePixelFormatARB_proc,    sizeof(PROC));
								mu_memcpy(&wgl->SwapInterval,         &wglSwapIntervalEXT_proc,         sizeof(PROC));
								return MUCOSA_SUCCESS;
							}

							// Creates an OpenGL context
							muCOSAResult muCOSAW32_create_opengl_context(HDC dc, int win_pixel_format, muCOSAW32_WGL* wgl, muPixelFormat* format, HGLRC* context, muGraphicsAPI api, muBool* set) {
								muCOSAResult res = MUCOSA_SUCCESS;
								// Choose pixel format
								int pixel_format;

								// - If a pixel format is supplied by the user:
								if (format) {
									// Set up an attribute field based on pixel format
									int pixel_format_attributes[] = {
										MUCOSAWGL_DRAW_TO_WINDOW_ARB, MU_TRUE,
										MUCOSAWGL_SUPPORT_OPENGL_ARB, MU_TRUE,
										MUCOSAWGL_DOUBLE_BUFFER_ARB,  MU_TRUE,
										MUCOSAWGL_SAMPLE_BUFFERS_ARB, MU_TRUE, // (?)
										MUCOSAWGL_ACCELERATION_ARB,   MUCOSAWGL_FULL_ACCELERATION_ARB, // (?)
										MUCOSAWGL_PIXEL_TYPE_ARB,     MUCOSAWGL_TYPE_RGBA_ARB, // (?)
										MUCOSAWGL_RED_BITS_ARB,       format->red_bits,
										MUCOSAWGL_GREEN_BITS_ARB,     format->green_bits,
										MUCOSAWGL_BLUE_BITS_ARB,      format->blue_bits,
										MUCOSAWGL_ALPHA_BITS_ARB,     format->alpha_bits,
										MUCOSAWGL_DEPTH_BITS_ARB,     format->depth_bits,
										MUCOSAWGL_STENCIL_BITS_ARB,   format->stencil_bits,
										MUCOSAWGL_SAMPLES_ARB,        format->samples,
										0
									};
									// Find corresponding format using WGL
									UINT format_count = 0;
									// - Does the return value of this indicate error? Can't find documentation
									wgl->ChoosePixelFormat(dc, pixel_format_attributes, 0, 1, &pixel_format, &format_count);
									if (!format_count) {
										res = MUCOSA_WIN32_FAILED_CHOOSE_WGL_PIXEL_FORMAT;
									}
								}

								// - If a pixel format isn't supplied by the user (or it failed)
								if ((!format) || (res != MUCOSA_SUCCESS)) {
									// Get default pixel format from window
									pixel_format = win_pixel_format;
								}

								// Describe pixel format
								PIXELFORMATDESCRIPTOR format_desc;
								if (!DescribePixelFormat(dc, pixel_format, sizeof(format_desc), &format_desc)) {
									return MUCOSA_WIN32_FAILED_DESCRIBE_WGL_PIXEL_FORMAT;
								}
								// Set pixel format @TODO Figure this out in window creation
								if (!*set) {
									if (!SetPixelFormat(dc, pixel_format, &format_desc)) {
										return MUCOSA_WIN32_FAILED_SET_WGL_PIXEL_FORMAT;
									}
									*set = MU_TRUE;
								}

								// OpenGL version info
								int opengl_attributes[] = {
									MUCOSAWGL_CONTEXT_MAJOR_VERSION_ARB, 0,
									MUCOSAWGL_CONTEXT_MINOR_VERSION_ARB, 0,
									MUCOSAWGL_CONTEXT_PROFILE_MASK_ARB, MUCOSAWGL_CONTEXT_CORE_PROFILE_BIT_ARB,
									0
								};

								// - Fill in OpenGL version info based on graphics API value
								switch (api) {
									default: return MUCOSA_FAILED_UNKNOWN_GRAPHICS_API; break;
									case MU_OPENGL_1_0: {
										opengl_attributes[1] = 1;
										opengl_attributes[3] = 0;
										opengl_attributes[4] = 0;
									} break;
									case MU_OPENGL_1_1: {
										opengl_attributes[1] = 1;
										opengl_attributes[3] = 1;
										opengl_attributes[4] = 0;
									} break;
									case MU_OPENGL_1_2: {
										opengl_attributes[1] = 1;
										opengl_attributes[3] = 2;
										opengl_attributes[4] = 0;
									} break;
									// Don't know what to do, setting to 1.3
									case MU_OPENGL_1_2_1: {
										opengl_attributes[1] = 1;
										opengl_attributes[3] = 3;
										opengl_attributes[4] = 0;
									} break;
									case MU_OPENGL_1_3: {
										opengl_attributes[1] = 1;
										opengl_attributes[3] = 3;
										opengl_attributes[4] = 0;
									} break;
									case MU_OPENGL_1_4: {
										opengl_attributes[1] = 1;
										opengl_attributes[3] = 4;
										opengl_attributes[4] = 0;
									} break;
									case MU_OPENGL_1_5: {
										opengl_attributes[1] = 1;
										opengl_attributes[3] = 5;
										opengl_attributes[4] = 0;
									} break;
									case MU_OPENGL_2_0: {
										opengl_attributes[1] = 2;
										opengl_attributes[3] = 0;
										opengl_attributes[4] = 0;
									} break;
									case MU_OPENGL_2_1: {
										opengl_attributes[1] = 2;
										opengl_attributes[3] = 1;
										opengl_attributes[4] = 0;
									} break;
									case MU_OPENGL_3_0: {
										opengl_attributes[1] = 3;
										opengl_attributes[3] = 0;
										opengl_attributes[4] = 0;
									} break;
									case MU_OPENGL_3_1: {
										opengl_attributes[1] = 3;
										opengl_attributes[3] = 1;
										opengl_attributes[4] = 0;
									} break;
									case MU_OPENGL_3_2_CORE: {
										opengl_attributes[1] = 3;
										opengl_attributes[3] = 2;
									} break;
									case MU_OPENGL_3_2_COMPATIBILITY: {
										opengl_attributes[1] = 3;
										opengl_attributes[3] = 2;
										opengl_attributes[5] = MUCOSAWGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
									} break;
									case MU_OPENGL_3_3_CORE: {
										opengl_attributes[1] = 3;
										opengl_attributes[3] = 3;
									} break;
									case MU_OPENGL_3_3_COMPATIBILITY: {
										opengl_attributes[1] = 3;
										opengl_attributes[3] = 3;
										opengl_attributes[5] = MUCOSAWGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
									} break;
									case MU_OPENGL_4_0_CORE: {
										opengl_attributes[1] = 4;
										opengl_attributes[3] = 0;
									} break;
									case MU_OPENGL_4_0_COMPATIBILITY: {
										opengl_attributes[1] = 4;
										opengl_attributes[3] = 0;
										opengl_attributes[5] = MUCOSAWGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
									} break;
									case MU_OPENGL_4_1_CORE: {
										opengl_attributes[1] = 4;
										opengl_attributes[3] = 1;
									} break;
									case MU_OPENGL_4_1_COMPATIBILITY: {
										opengl_attributes[1] = 4;
										opengl_attributes[3] = 1;
										opengl_attributes[5] = MUCOSAWGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
									} break;
									case MU_OPENGL_4_2_CORE: {
										opengl_attributes[1] = 4;
										opengl_attributes[3] = 2;
									} break;
									case MU_OPENGL_4_2_COMPATIBILITY: {
										opengl_attributes[1] = 4;
										opengl_attributes[3] = 2;
										opengl_attributes[5] = MUCOSAWGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
									} break;
									case MU_OPENGL_4_3_CORE: {
										opengl_attributes[1] = 4;
										opengl_attributes[3] = 3;
									} break;
									case MU_OPENGL_4_3_COMPATIBILITY: {
										opengl_attributes[1] = 4;
										opengl_attributes[3] = 3;
										opengl_attributes[5] = MUCOSAWGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
									} break;
									case MU_OPENGL_4_4_CORE: {
										opengl_attributes[1] = 4;
										opengl_attributes[3] = 4;
									} break;
									case MU_OPENGL_4_4_COMPATIBILITY: {
										opengl_attributes[1] = 4;
										opengl_attributes[3] = 4;
										opengl_attributes[5] = MUCOSAWGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
									} break;
									case MU_OPENGL_4_5_CORE: {
										opengl_attributes[1] = 4;
										opengl_attributes[3] = 5;
									} break;
									case MU_OPENGL_4_5_COMPATIBILITY: {
										opengl_attributes[1] = 4;
										opengl_attributes[3] = 5;
										opengl_attributes[5] = MUCOSAWGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
									} break;
									case MU_OPENGL_4_6_CORE: {
										opengl_attributes[1] = 4;
										opengl_attributes[3] = 6;
									} break;
									case MU_OPENGL_4_6_COMPATIBILITY: {
										opengl_attributes[1] = 4;
										opengl_attributes[3] = 6;
										opengl_attributes[5] = MUCOSAWGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
									} break;
								}

								// Create context
								*context = wgl->CreateContextAttribs(dc, 0, opengl_attributes);
								if (!*context) {
									return MUCOSA_WIN32_FAILED_CREATE_WGL_CONTEXT;
								}

								return res;
							}

					#endif /* MU_SUPPORT_OPENGL */

					/* Time */

						struct muCOSAW32_Time {
							// @DOCLINE The original time the context was created.
							double orig_time;
							// @DOCLINE The non-overwritable time.
							double fixed_time;
						};
						typedef struct muCOSAW32_Time muCOSAW32_Time;

						// https://stackoverflow.com/questions/1695288/getting-the-current-time-in-milliseconds-from-the-system-clock-in-windows
						double muCOSAW32_get_current_time(void) {
							// Get system time
							FILETIME file_time;
							GetSystemTimeAsFileTime(&file_time);

							// Format low + high time
							LONGLONG ll_now = 
								(LONGLONG)(file_time.dwLowDateTime) + ((LONGLONG)(file_time.dwHighDateTime) << 32LL)
							;
							// Return time properly divided
							return (double)(ll_now) / (double)(1.0e7);
						}

						// Initiates the time struct
						void muCOSAW32_time_init(muCOSAW32_Time* time) {
							// Set time to current time
							time->orig_time = time->fixed_time = muCOSAW32_get_current_time();
						}

					/* Clipboard */

						uint8_m* muCOSAW32_clipboard_get(muCOSAResult* result) {
							// Hold clipboard
							if (!OpenClipboard(NULL)) {
								MU_SET_RESULT(result, MUCOSA_WIN32_FAILED_HOLD_CLIPBOARD)
								return 0;
							}

							// Get handle to clipboard data (UTF-16)
							HANDLE data = GetClipboardData(CF_UNICODETEXT);
							if (!data) {
								// I'm pretty sure this can happen if no clipboard is available, so I'm not
								// throwing an error here
								CloseClipboard();
								return 0;
							}

							// Get clipboard data (UTF-16)
							wchar_t* utf16 = (wchar_t*)GlobalLock(data);
							if (!utf16) {
								MU_SET_RESULT(result, MUCOSA_WIN32_FAILED_GET_CLIPBOARD_DATA)
								CloseClipboard();
								return 0;
							}

							// Convert UTF-16 to UTF-8
							uint8_m* utf8 = muCOSAW32_wchar_to_utf8(utf16);
							if (!utf8) {
								MU_SET_RESULT(result, MUCOSA_WIN32_FAILED_CONVERT_CLIPBOARD_DATA_FORMAT)
								GlobalUnlock(data);
								CloseClipboard();
								return 0;
							}

							// Close and return data
							GlobalUnlock(data);
							CloseClipboard();
							return utf8;
						}

						void muCOSAW32_clipboard_set(muCOSAResult* result, uint8_m* data, size_m datalen) {
							// Get length of UTF-16 equivalent
							int wlen = MultiByteToWideChar(CP_UTF8, 0, (LPCCH)data, datalen, NULL, 0);
							if (!wlen) {
								MU_SET_RESULT(result, MUCOSA_WIN32_FAILED_CONVERT_CLIPBOARD_DATA_FORMAT)
								return;
							}

							// Allocate global memory for UTF-16 string
							HGLOBAL g_mem = GlobalAlloc(GMEM_MOVEABLE, (SIZE_T)(wlen)*sizeof(wchar_t));
							if (!g_mem) {
								MU_SET_RESULT(result, MUCOSA_WIN32_FAILED_ALLOCATE_CLIPBOARD_DATA)
								return;
							}

							// Get memory pointer
							LPVOID p_mem = GlobalLock(g_mem);
							if (!p_mem) {
								MU_SET_RESULT(result, MUCOSA_WIN32_FAILED_ALLOCATE_CLIPBOARD_DATA)
								GlobalFree(g_mem);
								return;
							}

							// Perform conversion
							if (!MultiByteToWideChar(CP_UTF8, 0, (LPCCH)data, datalen, (LPWSTR)p_mem, wlen)) {
								MU_SET_RESULT(result, MUCOSA_WIN32_FAILED_CONVERT_CLIPBOARD_DATA_FORMAT)
								GlobalUnlock(g_mem);
								GlobalFree(g_mem);
								return;
							}

							// Release memory pointer
							GlobalUnlock(g_mem);

							// Hold clipboard
							if (!OpenClipboard(NULL)) {
								MU_SET_RESULT(result, MUCOSA_WIN32_FAILED_HOLD_CLIPBOARD)
								GlobalFree(g_mem);
								return;
							}

							// Empty pre-existing clipboard data
							if (!EmptyClipboard()) {
								// Assuming this can get triggered if the clipboard was already empty,
								// so no error thrown here.
							}

							// Set clipboard data
							if (!SetClipboardData(CF_UNICODETEXT, g_mem)) {
								MU_SET_RESULT(result, MUCOSA_WIN32_FAILED_SET_CLIPBOARD_DATA)
								GlobalFree(g_mem);
								CloseClipboard();
								return;
							}

							// Close clipboard and we're done
							CloseClipboard();
						}

					/* Context */

						struct muCOSAW32_Context {
							muCOSAW32_Time time;
							MUCOSA_OPENGL_CALL(muCOSAW32_WGL wgl;)
						};
						typedef struct muCOSAW32_Context muCOSAW32_Context;

						// Accessing/Deaccessing pointer map functions needed for contexts
						void muCOSAW32_window_pmap_access(void);
						void muCOSAW32_window_pmap_deaccess(void);

						muCOSAResult muCOSAW32_context_init(muCOSAW32_Context* context) {
							muCOSAResult res = MUCOSA_SUCCESS;
							// Mark newly created context for pmap
							muCOSAW32_window_pmap_access();
							// Initiate context time
							muCOSAW32_time_init(&context->time);
							// Load OpenGL extensions
							MUCOSA_OPENGL_CALL(
								mu_memset(&context->wgl, 0, sizeof(context->wgl));
								res = muCOSAW32_get_opengl_extensions(&context->wgl);
							)

							return res;
						}

						void muCOSAW32_context_term(muCOSAW32_Context* context) {
							// Remove context from pmap
							muCOSAW32_window_pmap_deaccess();
							return; if (context) {}
						}

					/* Context time */

						double muCOSAW32_fixed_time_get(muCOSAW32_Context* context) {
							// Return the difference between now and when the context was created
							return muCOSAW32_get_current_time() - context->time.fixed_time;
						}

						double muCOSAW32_time_get(muCOSAW32_Context* context) {
							// Return the difference between now and the overridable original time
							return muCOSAW32_get_current_time() - context->time.orig_time;
						}

						void muCOSAW32_time_set(muCOSAW32_Context* context, double time) {
							// Set time to current time minus the given time
							context->time.orig_time = muCOSAW32_get_current_time() - time;
						}

					/* Sleep */

						void muCOSAW32_sleep(double time) {
							// Sleep for the approx. amount of milliseconds
							Sleep((DWORD)(time*1000.0));
						}

					/* Window structs */

						struct muCOSAW32_Keymaps {
							// Keyboard keys
							muBool keyboard[MU_KEYBOARD_LENGTH];
							// Keystates
							muBool keystates[MU_KEYSTATE_LENGTH];
							// Mouse keys
							muBool mouse[MU_MOUSE_LENGTH];
						};
						typedef struct muCOSAW32_Keymaps muCOSAW32_Keymaps;

						struct muCOSAW32_WindowHandles {
							// Window class name
							wchar_t wclass_name[2];
							// Hinstance
							HINSTANCE hinstance;
							// Window handle
							HWND hwnd;
							// Device context
							HDC dc;
							// Cursor handle
							HCURSOR hcursor;
							// IMM context handle
							HIMC imc;
						};
						typedef struct muCOSAW32_WindowHandles muCOSAW32_WindowHandles;

						// States possibly held by the window that are updated via proc
						struct muCOSAW32_WindowStates {
							// Closed or not
							muBool closed;
							// Cursor style
							muCursorStyle cursor_style;
						};
						typedef struct muCOSAW32_WindowStates muCOSAW32_WindowStates;

						struct muCOSAW32_WindowProperties {
							// Pixel format
							muPixelFormat format;
							muBool use_format;
							muBool format_set;
							// Default pixel format value
							int pixel_format;

							// Min/Max dimensions
							uint32_m min_width;
							uint32_m min_height;
							uint32_m max_width;
							uint32_m max_height;

							// Scroll level
							int32_m scroll_level;

							// Cursor position
							int32_m cursor_x;
							int32_m cursor_y;

							// Text input
							void (*text_input_callback)(muWindow window, uint8_m* data);
							uint32_m text_cursor_x;
							uint32_m text_cursor_y;
						};
						typedef struct muCOSAW32_WindowProperties muCOSAW32_WindowProperties;

						struct muCOSAW32_WindowTemp {
							// High surrogate for cross-WM_CHAR messages
							WCHAR high_surrogate;
						};
						typedef struct muCOSAW32_WindowTemp muCOSAW32_WindowTemp;

						struct muCOSAW32_Window {
							muCOSAW32_WindowHandles handles;
							muCOSAW32_Keymaps keymaps;
							muCOSAW32_WindowStates states;
							muCOSAW32_WindowProperties props;
							muWindowCallbacks callbacks;
							muCOSAW32_WindowTemp temp;
						};
						typedef struct muCOSAW32_Window muCOSAW32_Window;

					/* Pmap */

						// This entire section describes an atom-locked array of muCOSAW32_Window's.
						// This is needed for when we need to find a muCOSAW32_Window based on just
						// the Win32 handle (HWND), which happens in the proc function. Really sucks
						// that I have to do this, but it works!

						// The way the array works is by using a pointer array that doubles in memory
						// to keep up with allocation every time new memory is needed. Each element
						// in the array is a pointer to a valid muCOSA window or 0; when a window is
						// removed, its slot is set to 0, marking it for overwriting, which will be
						// done if a new window needs to be added and there is an empty slot.

						// The limitation to this is that the array cannot "de-expand"; for the
						// entire time that at least one muCOSA context exists, the amount of windows
						// alllocated can only go up. This is never likely to be a large issue, as
						// the VAST majority of programs will never allocate enough windows at once
						// to ever make this considerable in terms of memory allocated, but it is a
						// limitation nonetheless.

						// Pointer map struct
						struct muCOSAW32_WindowPMAP {
							// Pointer to each window's memory
							muCOSAW32_Window** windows;
							// Amount of windows
							size_m wincount;
							// Amount of window array memory allocated; unit is amount of windows
							size_m winlen;
							// Locked state
							LONG volatile locked;
							// Amount of active muCOSA contexts;
							// used to destroy all memory once all muCOSA contexts deactivate
							size_m access_count;
						};
						typedef struct muCOSAW32_WindowPMAP muCOSAW32_WindowPMAP;

						// Global window pmap
						muCOSAW32_WindowPMAP muCOSAW32_GlobalWindowPMAP = MU_ZERO_STRUCT_CONST(muCOSAW32_WindowPMAP);

						// Locks the window pmap to this thread
						void muCOSAW32_window_pmap_lock(void) {
							// Wait until we have it locked for this thread
							while (InterlockedCompareExchange(&muCOSAW32_GlobalWindowPMAP.locked, 1, 0) == 1) {
								// ...
							}
						}

						// Unlocks the window pmap (make sure you have it locked under the current thread!)
						void muCOSAW32_window_pmap_unlock(void) {
							// Set locked bit to 0 atomically
							_interlockedbittestandreset(&muCOSAW32_GlobalWindowPMAP.locked, 0);
						}

						// Adds a window to the window pmap
						muCOSAResult muCOSAW32_window_pmap_add(muCOSAW32_Window* win) {
							// Lock pmap to this thread
							muCOSAW32_window_pmap_lock();

							// Try and find an empty slot to fill
							for (size_m s = 0; s < muCOSAW32_GlobalWindowPMAP.wincount; ++s) {
								if (!muCOSAW32_GlobalWindowPMAP.windows[s]) {
									// If an empty slot is found, fill it and leave
									muCOSAW32_GlobalWindowPMAP.windows[s] = win;
									muCOSAW32_window_pmap_unlock();
									return MUCOSA_SUCCESS;
								}
							}

							// Increment window count by 1
							++muCOSAW32_GlobalWindowPMAP.wincount;

							// If array is non-existent currently:
							if (!muCOSAW32_GlobalWindowPMAP.windows) {
								// Allocate 1 window worth
								muCOSAW32_GlobalWindowPMAP.winlen = 1;
								muCOSAW32_GlobalWindowPMAP.windows = (muCOSAW32_Window**)mu_malloc(sizeof(muCOSAW32_Window*));
								// If unable to allocate:
								if (!muCOSAW32_GlobalWindowPMAP.windows) {
									// Set values to 0
									muCOSAW32_GlobalWindowPMAP.wincount = 0;
									muCOSAW32_GlobalWindowPMAP.winlen = 0;
									// Unlock pmap
									muCOSAW32_window_pmap_unlock();
									// Return bad value
									return MUCOSA_FAILED_MALLOC;
								}
							}

							// If an array already exists, but we don't have enough memory allocated
							else if (muCOSAW32_GlobalWindowPMAP.wincount >= muCOSAW32_GlobalWindowPMAP.winlen) {
								// Multiply window length by 2
								muCOSAW32_GlobalWindowPMAP.winlen *= 2;
								// Reallocate this new length
								muCOSAW32_Window** new_windows = (muCOSAW32_Window**)mu_realloc(
									muCOSAW32_GlobalWindowPMAP.windows,
									sizeof(muCOSAW32_Window*) * muCOSAW32_GlobalWindowPMAP.winlen
								);

								// If we failed to reallocate:
								if (!new_windows) {
									// Deincrement window count
									--muCOSAW32_GlobalWindowPMAP.wincount;
									// Undo winlen multiplication
									muCOSAW32_GlobalWindowPMAP.winlen /= 2;
									// Unlock pmap
									muCOSAW32_window_pmap_unlock();
									// Return bad value
									return MUCOSA_FAILED_REALLOC;
								}

								// If we didn't fail, we point to the newly allocated memory
								muCOSAW32_GlobalWindowPMAP.windows = new_windows;
							}

							// Set newly allocated window spot to the new window
							muCOSAW32_GlobalWindowPMAP.windows[muCOSAW32_GlobalWindowPMAP.wincount-1] = win;

							// Unlock pmap
							muCOSAW32_window_pmap_unlock();
							// Return success
							return MUCOSA_SUCCESS;
						}

						// Removes a window from the pmap
						void muCOSAW32_window_pmap_remove(muCOSAW32_Window* win) {
							// Lock pmap to this thread
							muCOSAW32_window_pmap_lock();

							// Find window pointer within pmap
							for (size_m p = 0; p < muCOSAW32_GlobalWindowPMAP.wincount; ++p) {
								if (muCOSAW32_GlobalWindowPMAP.windows[p] == win) {
									muCOSAW32_GlobalWindowPMAP.windows[p] = 0;
									break;
								}
							}

							// Unlock pmap
							muCOSAW32_window_pmap_unlock();
						}

						// Cleans up all of the memory within the pmap
						// Note that this does NOT lock/unlock; this should only be called with
						// a thread that already has the global pmap locked
						void muCOSAW32_window_pmap_cleanup(void) {
							// Free the memory if it's allocated
							if (muCOSAW32_GlobalWindowPMAP.windows) {
								mu_free(muCOSAW32_GlobalWindowPMAP.windows);
							}
							// Zero-out the global pmap struct
							muCOSAW32_GlobalWindowPMAP = MU_ZERO_STRUCT(muCOSAW32_WindowPMAP);
						}

						// "Accesses" the pmap, marking a context as currently active
						void muCOSAW32_window_pmap_access(void) {
							// Lock the pmap
							muCOSAW32_window_pmap_lock();

							// Increment access count
							++muCOSAW32_GlobalWindowPMAP.access_count;

							// Unlock the pmap
							muCOSAW32_window_pmap_unlock();
						}

						// "De-accesses" the pmap, marking a context as no longer active
						void muCOSAW32_window_pmap_deaccess(void) {
							// Lock the pmap
							muCOSAW32_window_pmap_lock();

							// Deincrement access count
							--muCOSAW32_GlobalWindowPMAP.access_count;

							// If access count is 0, no more contexts are active, which
							// means that we should clean everything up
							if (muCOSAW32_GlobalWindowPMAP.access_count == 0) {
								muCOSAW32_window_pmap_cleanup();
							}

							// Unlock the pmap
							muCOSAW32_window_pmap_unlock();
						}

						// Finds a pointer to a muCOSA window based on the HWND handle
						muCOSAW32_Window* muCOSAW32_window_pmap_find(HWND hwnd) {
							// Lock the pmap
							muCOSAW32_window_pmap_lock();

							// Default pointer
							muCOSAW32_Window* wp = 0;

							// Find pointer in array
							for (size_m p = 0; p < muCOSAW32_GlobalWindowPMAP.wincount; ++p) {
								if (muCOSAW32_GlobalWindowPMAP.windows[p] != 0 && 
									muCOSAW32_GlobalWindowPMAP.windows[p]->handles.hwnd == hwnd
								) {
									wp = muCOSAW32_GlobalWindowPMAP.windows[p];
									break;
								}
							}

							// Unlock the pmap
							muCOSAW32_window_pmap_unlock();
							// Return pointer
							return wp;
						}

					/* Proc */

						// Info about a proc message
						struct muCOSAW32_ProcMsg {
							// Proc stuff
							UINT uMsg;
							WPARAM wParam;
							LPARAM lParam;

							// muCOSA window
							muCOSAW32_Window* win;
						};
						typedef struct muCOSAW32_ProcMsg muCOSAW32_ProcMsg;

						// Handling for WM_DESTROY; when a window is closed
						LRESULT CALLBACK muCOSAW32_DESTROY(muCOSAW32_ProcMsg msg) {
							PostQuitMessage(0);
							msg.win->states.closed = MU_TRUE;
							return 0;
						}

						// Separates left/right keys
						WPARAM muCOSAW32_map_lr_keys(WPARAM vk, LPARAM lParam) {
							// Separate scancode and left/right bit
							UINT scancode = (lParam & 0x00ff0000) >> 16;
							int  extended = (lParam & 0x01000000) != 0;

							// Separate into left/right virtual key codes if need be
							switch (vk) {
								default: return vk; break;
								case VK_SHIFT:   return MapVirtualKey(scancode, MAPVK_VSC_TO_VK_EX); break;
								case VK_CONTROL: return extended ? VK_RCONTROL : VK_LCONTROL; break;
								case VK_MENU:    return extended ? VK_RMENU    : VK_LMENU; break;
							}
						}

						// Handling for WM_KEYDOWN and WM_KEYUP
						LRESULT CALLBACK muCOSAW32_KEY(muCOSAW32_ProcMsg msg, muBool up) {
							// Handle separation of keys based on left/right
							msg.wParam = muCOSAW32_map_lr_keys(msg.wParam, msg.lParam);
							// Convert Win32 key value to muCOSA key value
							muKeyboardKey key = muCOSAW32_VK_to_muCOSA(msg.wParam);
							// Return if key is unknown or out of range
							if (key == MU_KEYBOARD_UNKNOWN || key >= MU_KEYBOARD_LENGTH) {
								return 0;
							}

							// Set key state
							msg.win->keymaps.keyboard[key] = up;
							// Call keyboard callback
							if (msg.win->callbacks.keyboard) {
								msg.win->callbacks.keyboard(msg.win, key, up);
							}

							// Exit
							return 0;
						}

						// Handling for WM_LBUTTONDOWN, WM_RBUTTONDOWN, WM_LBUTTONUP, and WM_RBUTTONDOWN
						LRESULT CALLBACK muCOSAW32_MBUTTON(muCOSAW32_ProcMsg msg, muMouseKey key, muBool up) {
							// Update keymap
							msg.win->keymaps.mouse[key] = up;
							// + Callback
							if (msg.win->callbacks.mouse_key) {
								msg.win->callbacks.mouse_key(msg.win, key, up);
							}
							return 0;
						}

						muCOSAResult muCOSAW32_window_get_dimensions(muCOSAW32_Window* win, uint32_m* data);
						muCOSAResult muCOSAW32_window_get_cursor_pos(muCOSAW32_Window* win, int32_m* data);
						// Handling for WM_SETCURSOR
						LRESULT CALLBACK muCOSAW32_SETCURSOR(muCOSAW32_ProcMsg msg) {
							// Only allow the cursor to change if it's outside the window surface
							// This is a hack and a half, but largely works
							int32_m cur[2]; muCOSAW32_window_get_cursor_pos(msg.win, cur);
							uint32_m dim[2]; muCOSAW32_window_get_dimensions(msg.win, dim);
							int32_m idim[2] = { (int32_m)dim[0], (int32_m)dim[1] };
							if (cur[0] >= 0 && cur[1] >= 0 && cur[0] < idim[0] && cur[1] < idim[1]) {
								SetCursor(msg.win->handles.hcursor);
							} else {
								return DefWindowProcW(msg.win->handles.hwnd, msg.uMsg, msg.wParam, msg.lParam);
							}
							return 0;
						}

						// Handling for WM_GETMINMAXINFO
						LRESULT CALLBACK muCOSAW32_GETMINMAXINFO(muCOSAW32_ProcMsg msg) {
							LPMINMAXINFO lp = (LPMINMAXINFO)msg.lParam;
							// I have no idea why 16 and 39 need to be added here. Can't be borders
							// because these numbers don't even nearly match the border values.
							// If it works, it works :P
							lp->ptMinTrackSize.x = msg.win->props.min_width + 16;
							lp->ptMinTrackSize.y = msg.win->props.min_height + 39;
							lp->ptMaxTrackSize.x = msg.win->props.max_width + 16;
							lp->ptMaxTrackSize.y = msg.win->props.max_height + 39;
							return 0;
						}

						// Handling for WM_MOUSEWHEEL
						LRESULT CALLBACK muCOSAW32_MOUSEWHEEL(muCOSAW32_ProcMsg msg) {
							// Add scroll level from wParam
							int32_m add = GET_WHEEL_DELTA_WPARAM(msg.wParam);
							msg.win->props.scroll_level += add;
							// + Callback
							if (msg.win->callbacks.scroll) {
								msg.win->callbacks.scroll(msg.win, add);
							}

							return 0;
						}

						// Handling for WM_SIZE
						LRESULT CALLBACK muCOSAW32_SIZE(muCOSAW32_ProcMsg msg) {
							// Call dimensions callback
							if (msg.win->callbacks.dimensions) {
								msg.win->callbacks.dimensions(msg.win,
									(uint32_m)(LOWORD(msg.lParam)),
									(uint32_m)(HIWORD(msg.lParam))
								);
							}

							// Not sure why we're calling this
							// I think I vaguely remember this fixing some issue, but idk
							PostMessage(msg.win->handles.hwnd, WM_PAINT, 0, 0);
							return 0;
						}

						// Handling for WM_MOVE
						LRESULT CALLBACK muCOSAW32_MOVE(muCOSAW32_ProcMsg msg) {
							// Call position callback
							if (msg.win->callbacks.position) {
								// https://www.autohotkey.com/boards/viewtopic.php?t=27857
								// I would KISS Bill on the mouth if I ever met him...
								msg.win->callbacks.position(
									msg.win,
									(int32_m)( msg.lParam      & 0x8000 ? - ((~msg.lParam    ) & 0x7FFF)+1 : msg.lParam       & 0x7FFF),
									(int32_m)((msg.lParam>>16) & 0x8000 ? - ((~msg.lParam>>16) & 0X7FFF)+1 : (msg.lParam>>16) & 0x7FFF)
								);
							}

							return 0;
						}

						uint8_m muCOSAW32_UTF8_codepoint_size(uint32_m codepoint) {
							// Storable in 7 bits = 1 byte
							if (codepoint < 128) {
								return 1;
							}
							// Storable in 11 bits = 2 bytes
							if (codepoint < 2048) {
								return 2;
							}
							// Storable in 16 bits = 3 bytes
							if (codepoint < 65536) {
								return 3;
							}
							return 4;
						}

						// Handling for WM_CHAR
						LRESULT CALLBACK muCOSAW32_CHAR(muCOSAW32_ProcMsg msg) {
							// Hold onto high surrogate if it is one
							if (IS_HIGH_SURROGATE(msg.wParam)) {
								msg.win->temp.high_surrogate = (WCHAR)msg.wParam;
								return 0;
							}

							// Return if we aren't taking text input
							if (!msg.win->props.text_input_callback) {
								return DefWindowProcW(msg.win->handles.hwnd, msg.uMsg, msg.wParam, msg.lParam);
							}

							// Convert UTF-16 wParam to wchar_t* UTF-16 string
							WCHAR wstr[3];
							// - Surrogate pair
							if (msg.win->temp.high_surrogate) {
								wstr[0] = msg.win->temp.high_surrogate;
								wstr[1] = (WCHAR)msg.wParam;
								wstr[2] = 0;
								// (Reset high surrogate)
								msg.win->temp.high_surrogate = 0;
							}
							// - Non-surrogate pair
							else {
								wstr[0] = (WCHAR)msg.wParam;
								wstr[1] = 0;
							}

							// Convert wchar_t* UTF-16 string to UTF-8 string
							uint8_m buf[5];
							if (!WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, wstr, -1, (LPSTR)buf, sizeof(buf), NULL, NULL)) {
								return 0;
							}

							// Callback
							msg.win->props.text_input_callback(msg.win, buf);
							return 0;
						}

						// Handling for WM_IME_STARTCOMPOSITION
						LRESULT CALLBACK muCOSAW32_IME_STARTCOMPOSITION(muCOSAW32_ProcMsg msg) {
							// Get IMM context handle
							HIMC imc = ImmGetContext(msg.win->handles.hwnd);

							// Fill out composition form
							COMPOSITIONFORM cf = MU_ZERO_STRUCT(COMPOSITIONFORM);
							cf.dwStyle = CFS_FORCE_POSITION;
							cf.ptCurrentPos.x = msg.win->props.text_cursor_x;
							cf.ptCurrentPos.y = msg.win->props.text_cursor_y;

							// Send composition form
							if (!ImmSetCompositionWindow(imc, &cf)) {
								return 0;
							}

							// Release IMM context handle
							ImmReleaseContext(msg.win->handles.hwnd, imc);
							return 0;
						}

						// Handles a proc message
						LRESULT muCOSAW32_procmsg(muCOSAW32_ProcMsg msg) {
							// Do things based on the message code
							switch (msg.uMsg) {
								// Message code we're not processing; return default handling at end of switch
								default: break;
								// Window closing
								case WM_DESTROY: return muCOSAW32_DESTROY(msg); break;
								// Key down
								case WM_KEYDOWN: return muCOSAW32_KEY(msg, MU_TRUE); break;
								// Key up
								case WM_KEYUP: return muCOSAW32_KEY(msg, MU_FALSE); break;
								// Left mouse up
								case WM_LBUTTONUP: return muCOSAW32_MBUTTON(msg, MU_MOUSE_LEFT, MU_FALSE); break;
								// Left mouse down
								case WM_LBUTTONDOWN: return muCOSAW32_MBUTTON(msg, MU_MOUSE_LEFT, MU_TRUE); break;
								// Right mouse up
								case WM_RBUTTONUP: return muCOSAW32_MBUTTON(msg, MU_MOUSE_RIGHT, MU_FALSE); break;
								// Right mouse down
								case WM_RBUTTONDOWN: return muCOSAW32_MBUTTON(msg, MU_MOUSE_RIGHT, MU_TRUE); break;
								// Middle mouse up
								case WM_MBUTTONUP: return muCOSAW32_MBUTTON(msg, MU_MOUSE_MIDDLE, MU_FALSE); break;
								// Middle mouse down
								case WM_MBUTTONDOWN: return muCOSAW32_MBUTTON(msg, MU_MOUSE_MIDDLE, MU_TRUE); break;
								// Cursor style changing
								case WM_SETCURSOR: return muCOSAW32_SETCURSOR(msg); break;
								// Windows asking for min/max dimensions
								case WM_GETMINMAXINFO: return muCOSAW32_GETMINMAXINFO(msg); break;
								// Scrolling
								case WM_MOUSEWHEEL: return muCOSAW32_MOUSEWHEEL(msg); break;
								// Resizing
								case WM_SIZE: return muCOSAW32_SIZE(msg); break;
								// Movement
								case WM_MOVE: return muCOSAW32_MOVE(msg); break;
								// Character input
								case WM_CHAR: return muCOSAW32_CHAR(msg); break;
								// IME composition position
								case WM_IME_STARTCOMPOSITION: return muCOSAW32_IME_STARTCOMPOSITION(msg); break;
							}

							// Default handling
							return DefWindowProcW(msg.win->handles.hwnd, msg.uMsg, msg.wParam, msg.lParam);
						}

						// Proc function for Win32
						LRESULT CALLBACK muCOSAW32_winproc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
							// Return default if hwnd is NULL
							if (!hwnd) {
								return DefWindowProcW(hwnd, uMsg, wParam, lParam);
							}

							// Start getting info about the proc
							muCOSAW32_ProcMsg msg;

							// Find window based on HWND
							msg.win = muCOSAW32_window_pmap_find(hwnd);
							if (!msg.win) {
								return DefWindowProcW(hwnd, uMsg, wParam, lParam);
							}

							// Copy over other info
							msg.uMsg = uMsg;
							msg.wParam = wParam;
							msg.lParam = lParam;

							// Return proc message handling
							return muCOSAW32_procmsg(msg);
						}

					/* Creation / Destruction */

						muCOSAResult muCOSAW32_window_set_position(muCOSAW32_Window* win, int32_m* data);

						muCOSAResult muCOSAW32_window_create(muWindowInfo* info, muCOSAW32_Window* win) {
							/* Default attributes */

								// Zero-ing-out
								mu_memset(&win->handles, 0, sizeof(win->handles));
								mu_memset(&win->keymaps, 0, sizeof(win->keymaps));

								// Closed
								win->states.closed = MU_FALSE;

								// Cursor style
								win->states.cursor_style = MU_CURSOR_ARROW;

								// Pixel format
								if (info->pixel_format) {
									win->props.use_format = MU_TRUE;
									win->props.format = *info->pixel_format;
								} else {
									win->props.use_format = MU_FALSE;
								}
								win->props.format_set = MU_FALSE;

								// Min/Max dimensions
								win->props.min_width = info->min_width;
								win->props.min_height = info->min_height;
								win->props.max_width = info->max_width;
								win->props.max_height = info->max_height;
								// - (Fix 0 defaults)
								if (win->props.min_width == 0) {
									win->props.min_width = 120;
								}
								if (win->props.min_height == 0) {
									win->props.min_height = 1;
								}
								if (win->props.max_width == 0) {
									win->props.max_width = 0x0FFFFFFF;
								}
								if (win->props.max_height == 0) {
									win->props.max_height = 0x0FFFFFFF;
								}

								// Scroll level
								win->props.scroll_level = 0;

								// Cursor position
								win->props.cursor_x = win->props.cursor_y = 0;

								// Callbacks
								// - Zero-out if no callbacks specified
								if (!info->callbacks) {
									mu_memset(&win->callbacks, 0, sizeof(win->callbacks));
								}
								// - Set all callbacks if specified
								else {
									win->callbacks = *info->callbacks;
								}

								// Text focus
								win->props.text_input_callback = 0;
								win->props.text_cursor_x = win->props.text_cursor_y = 0;

								// High surrogate for WM_CHAR messages
								win->temp.high_surrogate = 0;

							/* Class */

								// Window title
								wchar_t* wname = muCOSAW32_utf8_to_wchar(info->title);
								if (!wname) {
									return MUCOSA_WIN32_FAILED_CONVERT_UTF8_TO_WCHAR;
								}

								// This is some of the jankiest code I've written!
								// To generate a unique window class, I hamfistedly
								// attempt to generate one by adding the actual pointer
								// value of 'win' to a character, creating an often
								// unreadable class title. This needs to be improved
								// at some point, but works for me :P

								win->handles.wclass_name[0] = (wchar_t)'!';

								wchar_t add; // (Purposely left uninitialized for more randomness)
								if (sizeof(wchar_t) < sizeof(win)) {
									mu_memcpy(&add, &win, sizeof(wchar_t));
								} else {
									mu_memcpy(&add, &win, sizeof(win));
								}

								win->handles.wclass_name[0] += add;
								win->handles.wclass_name[1] = (wchar_t)'\0';

								// Hinstance
								win->handles.hinstance = muCOSAW32_get_hinstance();
								// Hcursor
								win->handles.hcursor = LoadCursor(NULL, IDC_ARROW);

								// Create class struct
								WNDCLASSEXW wclass = MU_ZERO_STRUCT(WNDCLASSEXW);
								wclass.cbSize = sizeof(wclass);
								wclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
								wclass.lpfnWndProc = muCOSAW32_winproc;
								wclass.hInstance = win->handles.hinstance;
								wclass.hCursor = win->handles.hcursor;
								wclass.lpszMenuName = wname;
								wclass.lpszClassName = win->handles.wclass_name;

								// Register
								if(!RegisterClassExW(&wclass)) {
									mu_free(wname);
									return MUCOSA_WIN32_FAILED_REGISTER_WINDOW_CLASS;
								}

							/* Window */

								// Style
								DWORD style = WS_OVERLAPPEDWINDOW;

								// Rect for the window's dimensions
								RECT r;
								r.left = r.top = 0;
								r.right = (LONG)info->width;
								r.bottom = (LONG)info->height;
								AdjustWindowRect(&r, style, FALSE);

								// Create window
								win->handles.hwnd = CreateWindowExW(
									// dwExStyle
									0,
									// lpClassName
									wclass.lpszClassName,
									// lpWindowName
									wclass.lpszMenuName,
									// dwStyle
									style,
									// X, Y,
									(int)info->x, (int)info->y,
									// nWidth, nHeight
									r.right-r.left, r.bottom-r.top,
									// hWndParent
									NULL,
									// hMenu
									NULL,
									// hInstance
									win->handles.hinstance,
									// lpParam
									NULL
								);

								// - Handle fail case
								mu_free(wname);
								if (win->handles.hwnd == NULL) {
									UnregisterClassW(win->handles.wclass_name, win->handles.hinstance);
									return MUCOSA_WIN32_FAILED_CREATE_WINDOW;
								}

							/* Pixel format */

								// Get device context
								win->handles.dc = GetDC(win->handles.hwnd);

								// Handle default pixel format:
								if (!info->pixel_format) {
									// Generic pixel format descriptor
									// Based on https://www.khronos.org/opengl/wiki/Creating_an_OpenGL_Context_(WGL)
									PIXELFORMATDESCRIPTOR pfd = {
										sizeof(PIXELFORMATDESCRIPTOR),
										1,
										PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
										PFD_TYPE_RGBA,
										32,
										0, 0, 0, 0, 0, 0,
										0, 0, 0,
										0, 0, 0, 0,
										24, 8, 8,
										PFD_MAIN_PLANE,
										0,
										0, 0, 0
									};

									// Find pixel format; fallback on 1
									win->props.pixel_format = ChoosePixelFormat(win->handles.dc, &pfd);
									if (!win->props.pixel_format) {
										win->props.pixel_format = 1;
									}
								}

								// Show window
								ShowWindow(win->handles.hwnd, SW_NORMAL);

							/* Auto-disable IME */

								if (ImmAssociateContextEx(win->handles.hwnd, NULL, 0) == FALSE) {
									// ?
								}

							/* Set position manually */

								// A bit of a hack considering that it should work on the first try,
								// but frame extents on Win32 will be frame extents on Win32.

								int32_m pos[2] = { info->x, info->y };
								muCOSAW32_window_set_position(win, pos);

							/* Add window to pmap */

								muCOSAResult res = muCOSAW32_window_pmap_add(win);
								if (muCOSA_result_is_fatal(res)) {
									return res;
								}

							return res;
						}

						void muCOSAW32_window_destroy(muCOSAW32_Window* win) {
							// Release device context
							ReleaseDC(win->handles.hwnd, win->handles.dc);
							// Destroy window
							DestroyWindow(win->handles.hwnd);
							// Unregister window class
							UnregisterClassW(win->handles.wclass_name, win->handles.hinstance);
							// Remove window from pmap
							muCOSAW32_window_pmap_remove(win);
						}

					/* Main loop */

						muBool muCOSAW32_window_get_closed(muCOSAW32_Window* win) {
							return win->states.closed;
						}

						void muCOSAW32_window_close(muCOSAW32_Window* win) {
							// Set closed flag
							win->states.closed = MU_TRUE;
						}

						// Checks all keystates and updates accordingly
						void muCOSAW32_update_keystate(muCOSAW32_Window* win) {
							// Loop through each possible keystate
							for (muKeyboardState s = 1; s < MU_KEYSTATE_LENGTH; ++s) {
								// Assume not on at first
								muBool b = MU_FALSE;
								// Convert keystate to Win32
								int s_w32 = muCOSAW32_keystate_to_W32(s);
								// If not recognized, continue:
								if (s_w32 == VK_NONAME) {
									continue;
								}

								// Set boolean to if it's on or off
								b = (GetKeyState(s_w32) & 0x0001) != 0;
								// Change value if different
								if (b != win->keymaps.keystates[s]) {
									win->keymaps.keystates[s] = b;

									// + Callback
									if (win->callbacks.keystate) {
										win->callbacks.keystate(win, s, b);
									}
								}
							}
						}

						// Handles the cursor changing position every frame;
						// no corresponding Win32 proc for this as far as I'm aware :L
						muCOSAResult muCOSAW32_window_get_cursor_pos(muCOSAW32_Window* win, int32_m* data);
						void muCOSAW32_update_cursor(muCOSAW32_Window* win) {
							// Get cursor position
							int32_m c[2];
							if (muCOSA_result_is_fatal(muCOSAW32_window_get_cursor_pos(win, c))) {
								return;
							}

							// Update cursor position if changed
							if (win->props.cursor_x != c[0] || win->props.cursor_y != c[1]) {
								win->props.cursor_x = c[0];
								win->props.cursor_y = c[1];
								// + Callback
								if (win->callbacks.cursor) {
									win->callbacks.cursor(win, c[0], c[1]);
								}
							}
						}

						void muCOSAW32_window_update(muCOSAW32_Window* win) {
							// Process messages
							MSG msg = MU_ZERO_STRUCT(MSG);
							while (PeekMessageA(&msg, 0, 0, 0, PM_REMOVE)) {
								TranslateMessage(&msg);
								DispatchMessage(&msg);
							}

							// Update keystates
							muCOSAW32_update_keystate(win);
							// Update cursor position
							muCOSAW32_update_cursor(win);
						}

					/* Title */

						// Set title
						muCOSAResult muCOSAW32_window_set_title(muCOSAW32_Window* win, char* data) {
							// Convert UTF-8 data to wchar_t* data
							wchar_t* wtitle = muCOSAW32_utf8_to_wchar(data);
							if (!wtitle) {
								return MUCOSA_WIN32_FAILED_CONVERT_UTF8_TO_WCHAR;
							}

							// Set window title
							if (!SetWindowTextW(win->handles.hwnd, wtitle)) {
								mu_free(wtitle);
								return MUCOSA_WIN32_FAILED_SET_WINDOW_ATTRIB;
							}

							mu_free(wtitle);
							return MUCOSA_SUCCESS;
						}

					/* Frame extents */

						// Default frame extents
						// I love you Bill...
						void muCOSAW32_def_window_frame_extents(uint32_m* data) {
							data[0] = data[1] = data[3] = (uint32_m)GetSystemMetrics(SM_CXSIZEFRAME);
							data[2] = (uint32_m)(GetSystemMetrics(SM_CYFRAME) + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(92));
						}

						// Get frame extents for a window
						// I LOVE YOU BILL GATES!!!!!
						void muCOSAW32_window_get_frame_extents(muCOSAW32_Window* win, uint32_m* data) {
							// Fallback
							muCOSAW32_def_window_frame_extents(data);

							// Get window and client rect
							RECT wr, cr;
							if (!GetWindowRect(win->handles.hwnd, &wr)) {
								// (Not returning error since default window frame extents are filled)
								return;
							}
							if (!GetClientRect(win->handles.hwnd, &cr)) {
								return;
							}

							// Map client rect to window points
							if (!MapWindowPoints(win->handles.hwnd, NULL, (LPPOINT)&cr, 2)) {
								return;
							}

							// Set values based on window and client rect
							data[0] = (uint32_m)(cr.left-wr.left);
							data[1] = (uint32_m)(wr.right-cr.right);
							data[2] = (uint32_m)(cr.top-wr.top);
							data[3] = (uint32_m)(wr.bottom-cr.bottom);
						}

					/* Dimensions */

						muCOSAResult muCOSAW32_window_get_dimensions(muCOSAW32_Window* win, uint32_m* data) {
							// Get client rect
							RECT r;
							if (!GetClientRect(win->handles.hwnd, &r)) {
								return MUCOSA_WIN32_FAILED_GET_WINDOW_ATTRIB;
							}

							// Set values
							data[0] = (uint32_m)(r.right-r.left);
							data[1] = (uint32_m)(r.bottom-r.top);
							return MUCOSA_SUCCESS;
						}

						muCOSAResult muCOSAW32_window_set_dimensions(muCOSAW32_Window* win, uint32_m* data) {
							// Get general window info for style
							WINDOWINFO wi;
							if (!GetWindowInfo(win->handles.hwnd, &wi)) {
								return MUCOSA_WIN32_FAILED_SET_WINDOW_ATTRIB;
							}

							// Calculate rect
							RECT r;
							r.left = r.top = 0;
							r.right = data[0];
							r.bottom = data[1];

							// Calculate appropriate window rect for dimensions
							if (!AdjustWindowRect(&r, wi.dwStyle, FALSE)) {
								return MUCOSA_WIN32_FAILED_SET_WINDOW_ATTRIB;
							}

							// Set window rect based on this
							if (!SetWindowPos(win->handles.hwnd, HWND_TOP, 0, 0, r.right-r.left, r.bottom-r.top, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOMOVE)) {
								return MUCOSA_WIN32_FAILED_SET_WINDOW_ATTRIB;
							}
							return MUCOSA_SUCCESS;
						}

					/* Position */

						muCOSAResult muCOSAW32_window_get_position(muCOSAW32_Window* win, int32_m* data) {
							// Get window rect
							RECT r;
							if (!GetWindowRect(win->handles.hwnd, &r)) {
								return MUCOSA_WIN32_FAILED_GET_WINDOW_ATTRIB;
							}

							// Get frame extents
							uint32_m fe[4];
							muCOSAW32_window_get_frame_extents(win, fe);

							// Set position based on rect and relative frame extents
							data[0] = (int32_m)(r.left) + (int32_m)(fe[0]);
							data[1] = (int32_m)(r.top) + (int32_m)(fe[2]);
							return MUCOSA_SUCCESS;
						}

						muCOSAResult muCOSAW32_window_set_position(muCOSAW32_Window* win, int32_m* data) {
							// Get frame extents
							uint32_m fe[4];
							muCOSAW32_window_get_frame_extents(win, fe);

							// Translate x and y based on extents
							int32_m x = data[0] - (int32_m)(fe[0]);
							int32_m y = data[1] - (int32_m)(fe[2]);

							// Set window position
							if (!SetWindowPos(win->handles.hwnd, HWND_TOP, x, y, 0, 0, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE)) {
								return MUCOSA_WIN32_FAILED_SET_WINDOW_ATTRIB;
							}
							return MUCOSA_SUCCESS;
						}

					/* Keymaps */

						muCOSAResult muCOSAW32_window_get_keyboard_map(muCOSAW32_Window* win, muBool** data) {
							// Point to keyboard keymap
							*data = win->keymaps.keyboard;
							return MUCOSA_SUCCESS;
						}

						muCOSAResult muCOSAW32_window_get_keystate_map(muCOSAW32_Window* win, muBool** data) {
							// Point to keystate keymap
							*data = win->keymaps.keystates;
							return MUCOSA_SUCCESS;
						}

						muCOSAResult muCOSAW32_window_get_mouse_map(muCOSAW32_Window* win, muBool** data) {
							// Point to mouse keymap
							*data = win->keymaps.mouse;
							return MUCOSA_SUCCESS;
						}

					/* Cursor */

						muCOSAResult muCOSAW32_window_get_cursor_pos(muCOSAW32_Window* win, int32_m* data) {
							// Get cursor position
							POINT p;
							if (!GetCursorPos(&p)) {
								return MUCOSA_WIN32_FAILED_GET_WINDOW_ATTRIB;
							}

							// Get window position
							int32_m wpos[2];
							muCOSAResult res = muCOSAW32_window_get_position(win, wpos);
							if (muCOSA_result_is_fatal(res)) {
								return MUCOSA_WIN32_FAILED_GET_WINDOW_ATTRIB;
							}

							// Retrieve cursor position relative to window
							data[0] = p.x-wpos[0];
							data[1] = p.y-wpos[1];
							return res;
						}

						muCOSAResult muCOSAW32_window_set_cursor_pos(muCOSAW32_Window* win, int32_m* data) {
							// Get window position
							int32_m wpos[2];
							muCOSAResult res = muCOSAW32_window_get_position(win, wpos);
							if (muCOSA_result_is_fatal(res)) {
								return MUCOSA_WIN32_FAILED_SET_WINDOW_ATTRIB;
							}

							// Set cursor position relative to window
							if (!SetCursorPos(wpos[0]+data[0], wpos[1]+data[1])) {
								return MUCOSA_WIN32_FAILED_SET_WINDOW_ATTRIB;
							}
							return res;
						}

						muCOSAResult muCOSAW32_window_get_cursor_style(muCOSAW32_Window* win, muCursorStyle* data) {
							// Set data to cursor style
							*data = win->states.cursor_style;
							return MUCOSA_SUCCESS;
						}

						muCOSAResult muCOSAW32_window_set_cursor_style(muCOSAW32_Window* win, muCursorStyle* data) {
							// Set internal storage to cursor style
							win->states.cursor_style = *data;
							// Destroy cursor
							DestroyCursor(win->handles.hcursor);
							// Load cursor
							win->handles.hcursor = LoadCursor(0, (LPCSTR)muCOSAW32_muCOSA_cursor_to_W32(*data));
							// Set cursor
							SetCursor(win->handles.hcursor);
							return MUCOSA_SUCCESS;
						}

					/* Scroll */

						void muCOSAW32_window_get_scroll(muCOSAW32_Window* win, int32_m* data) {
							// Give scroll level
							*data = win->props.scroll_level;
						}

						void muCOSAW32_window_set_scroll(muCOSAW32_Window* win, int32_m* data) {
							// Overwrite scroll level
							win->props.scroll_level = *data;
						}

					/* Callbacks */

						void muCOSAW32_window_set_callback(muCOSAW32_Window* win, muWindowAttrib attrib, void* fun) {
							switch (attrib) {
								case MU_WINDOW_DIMENSIONS_CALLBACK: {
									mu_memcpy(&win->callbacks.dimensions, fun, sizeof(win->callbacks.dimensions));
								} break;
								case MU_WINDOW_POSITION_CALLBACK: {
									mu_memcpy(&win->callbacks.position, fun, sizeof(win->callbacks.position));
								} break;
								case MU_WINDOW_KEYBOARD_CALLBACK: {
									mu_memcpy(&win->callbacks.keyboard, fun, sizeof(win->callbacks.keyboard));
								} break;
								case MU_WINDOW_KEYSTATE_CALLBACK: {
									mu_memcpy(&win->callbacks.keystate, fun, sizeof(win->callbacks.keystate));
								} break;
								case MU_WINDOW_MOUSE_KEY_CALLBACK: {
									mu_memcpy(&win->callbacks.mouse_key, fun, sizeof(win->callbacks.mouse_key));
								} break;
								case MU_WINDOW_CURSOR_CALLBACK: {
									mu_memcpy(&win->callbacks.cursor, fun, sizeof(win->callbacks.cursor));
								} break;
								case MU_WINDOW_SCROLL_CALLBACK: {
									mu_memcpy(&win->callbacks.scroll, fun, sizeof(win->callbacks.scroll));
								} break;
							}
						}

					/* Text input */

						void muCOSAW32_window_update_text_cursor(muCOSAResult* result, muCOSAW32_Window* win, uint32_m cx, uint32_m cy);
						void muCOSAW32_window_get_text_input(muCOSAResult* result, muCOSAW32_Window* win, uint32_m cx, uint32_m cy, void (*callback)(muWindow, uint8_m*)) {
							// Associate IMM context with this window
							if (!ImmAssociateContextEx(win->handles.hwnd, NULL, IACE_DEFAULT)) {
								MU_SET_RESULT(result, MUCOSA_WIN32_FAILED_ASSOCIATE_IMM)
								return;
							}

							// Set callback
							win->props.text_input_callback = callback;
							// Update text cursor position
							muCOSAW32_window_update_text_cursor(0, win, cx, cy);
						}

						void muCOSAW32_window_let_text_input(muCOSAW32_Window* win) {
							// Set callback to nothing
							win->props.text_input_callback = 0;
							// Deassociate IMM context
							ImmAssociateContextEx(win->handles.hwnd, NULL, 0);
						}

						void muCOSAW32_window_update_text_cursor(muCOSAResult* result, muCOSAW32_Window* win, uint32_m cx, uint32_m cy) {
							// Do nothing if text input isn't being taken
							if (!win->props.text_input_callback) {
								return;
							}

							// Might not be a good idea to keep grabbing an releasing IMM context,
							// but the entire IMM is so fragile that the smallest change makes it
							// not function at all. So, I'm keeping it like this :L

							// Get IMM context handle
							win->handles.imc = ImmGetContext(win->handles.hwnd);

							// Fill out composition form
							COMPOSITIONFORM cf = MU_ZERO_STRUCT(COMPOSITIONFORM);
							cf.dwStyle = CFS_FORCE_POSITION;
							cf.ptCurrentPos.x = cx;
							cf.ptCurrentPos.y = cy;

							// Send composition form
							if (!ImmSetCompositionWindow(win->handles.imc, &cf)) {
								MU_SET_RESULT(result, MUCOSA_WIN32_FAILED_SET_COMPOSITION_WINDOW_POSITION)
							}

							// Release IMM context handle
							ImmReleaseContext(win->handles.hwnd, win->handles.imc);
						}

					/* OpenGL */

					#ifdef MU_SUPPORT_OPENGL

						struct muCOSAW32_GL {
							HDC dc;
							HGLRC hgl;
						};
						typedef struct muCOSAW32_GL muCOSAW32_GL;

						void* muCOSAW32_gl_context_create(muCOSAW32_Context* context, muCOSAResult* result, muCOSAW32_Window* win, muGraphicsAPI api) {
							// Allocate memory for context
							muCOSAW32_GL* gl = (muCOSAW32_GL*)mu_malloc(sizeof(muCOSAW32_GL));
							if (!gl) {
								MU_SET_RESULT(result, MUCOSA_FAILED_MALLOC)
								return 0;
							}
							gl->dc = win->handles.dc;

							// Create OpenGL context
							muPixelFormat* format = 0;
							if (win->props.use_format) {
								format = &win->props.format;
							}
							muCOSAResult res = muCOSAW32_create_opengl_context(win->handles.dc, win->props.pixel_format, &context->wgl, format, &gl->hgl, api, &win->props.format_set);
							if (res != MUCOSA_SUCCESS) {
								MU_SET_RESULT(result, res)
								if (muCOSA_result_is_fatal(res)) {
									mu_free(gl);
									gl = 0;
								}
							}
							return gl;
						}

						void muCOSAW32_gl_context_destroy(muCOSAW32_GL* gl) {
							wglDeleteContext(gl->hgl);
							mu_free(gl);
						}

						muCOSAResult muCOSAW32_gl_bind(muCOSAW32_GL* gl) {
							if (!wglMakeCurrent(gl->dc, gl->hgl)) {
								return MUCOSA_WIN32_FAILED_SET_WGL_CONTEXT;
							}
							return MUCOSA_SUCCESS;
						}

						muCOSAResult muCOSAW32_gl_swap_buffers(muCOSAW32_Window* win) {
							if (!SwapBuffers(win->handles.dc)) {
								return MUCOSA_WIN32_FAILED_SWAP_WGL_BUFFERS;
							}
							return MUCOSA_SUCCESS;
						}

						void* muCOSAW32_gl_get_proc_address(const char* name) {
							// Get proc address based on name
							PROC p = (PROC)wglGetProcAddress(name);

							// If failed:
							if (p == 0
								|| (p == (PROC)1)
								|| (p == (PROC)2)
								|| (p == (PROC)3)
								|| (p == (PROC)-1)
							) {
								// Try loading from DLL itself
								HMODULE module = LoadLibraryA("opengl32.dll");
								p = (PROC)GetProcAddress(module, name);
							}

							// Convert to void* in a way that avoids warnings
							void* vptr = 0;
							mu_memcpy(&vptr, &p, sizeof(void*));
							return vptr;
						}

						muBool muCOSAW32_gl_swap_interval(muCOSAW32_Context* context, muCOSAResult* result, int interval) {
							// If the swap interval function was not found when loading, quit
							if (!context->wgl.SwapInterval) {
								MU_SET_RESULT(result, MUCOSA_WIN32_FAILED_FIND_WGL_FUNCTION)
								return 0;
							}

							// Return a call to it if we've found it
							return context->wgl.SwapInterval(interval);
						}

					#endif /* MU_SUPPORT_OPENGL */

				#endif /* MUCOSA_WIN32 */

				/* Inner */

					// Inner struct
					struct muCOSA_Inner {
						muWindowSystem system;
						void* context;
					};
					typedef struct muCOSA_Inner muCOSA_Inner;

					// Determines the best window system
					muWindowSystem muCOSA_best_window_system(void) {
						#ifdef MUCOSA_WIN32
							return MU_WINDOW_WIN32;
						#else
							return 0;
						#endif
					}

					// Creates a valid inner struct based on the requested system
					muCOSAResult muCOSA_inner_create(muCOSA_Inner* inner, muWindowSystem system) {
						// Find best window system
						if (system == 0) {
							system = muCOSA_best_window_system();
						}
						inner->system = system;

						// Create context based on system
						switch (system) {
							// Unknown/Unsupported/None supported:
							default: return MUCOSA_FAILED_NULL_WINDOW_SYSTEM; break;

							// Win32
							MUCOSA_WIN32_CALL(
								case MU_WINDOW_WIN32: {
									// Allocate context
									inner->context = mu_malloc(sizeof(muCOSAW32_Context));
									if (!inner->context) {
										return MUCOSA_FAILED_MALLOC;
									}

									// Create context
									muCOSAResult res = muCOSAW32_context_init((muCOSAW32_Context*)inner->context);
									if (muCOSA_result_is_fatal(res)) {
										mu_free(inner->context);
									}
									return res;
								} break;
							)
						}
					}

					// Destroys inner struct based on system
					void muCOSA_inner_destroy(muCOSA_Inner* inner) {
						switch (inner->system) {
							default: break;

							// Win32
							MUCOSA_WIN32_CALL(
								case MU_WINDOW_WIN32: {
									// Destroy context
									muCOSAW32_context_term((muCOSAW32_Context*)inner->context);
								} break;
							)
						}
					}

				// Name call macro
				#ifdef MUCOSA_NAMES
					#define MUCOSA_NAME_CALL(...) __VA_ARGS__
				#else
					#define MUCOSA_NAME_CALL(...)
				#endif

				/* Context */

					MUDEF void muCOSA_context_create(muCOSAContext* context, muWindowSystem system, muBool set_context) {
						// Allocate context
						context->inner = mu_malloc(sizeof(muCOSA_Inner));
						if (!context->inner) {
							context->result = MUCOSA_FAILED_MALLOC;
							return;
						}

						// Create context
						context->result = muCOSA_inner_create((muCOSA_Inner*)context->inner, system);
						if (muCOSA_result_is_fatal(context->result)) {
							mu_free(context->inner);
							return;
						}

						// Set context if necessary
						if (set_context) {
							muCOSA_context_set(context);
						}
					}

					MUDEF void muCOSA_context_destroy(muCOSAContext* context) {
						// Free and destroy if inner contents exist
						if (context->inner) {
							muCOSA_inner_destroy((muCOSA_Inner*)context->inner);
							mu_free(context->inner);
						}
					}

					// Global context
					muCOSAContext* muCOSA_global_context = 0;

					// Set context
					MUDEF void muCOSA_context_set(muCOSAContext* context) {
						muCOSA_global_context = context;
					}

					// Get window system
					MUDEF muWindowSystem muCOSA_context_get_window_system(muCOSAContext* context) {
						return ((muCOSA_Inner*)context->inner)->system;
					}

				/* Window */

					/* Creation / Destruction */

						MUDEF muWindow muCOSA_window_create(muCOSAContext* context, muCOSAResult* result, muWindowInfo* info) {
							// Get inner from context
							muCOSA_Inner* inner = (muCOSA_Inner*)context->inner;

							// Do thing based on window system
							switch (inner->system) {
								default: return 0; break;

								// Win32
								MUCOSA_WIN32_CALL(case MU_WINDOW_WIN32 : {
									// Allocate window memory
									muCOSAW32_Window* win = (muCOSAW32_Window*)mu_malloc(sizeof(muCOSAW32_Window));
									if (!win) {
										MU_SET_RESULT(result, MUCOSA_FAILED_MALLOC)
										return 0;
									}

									// Create window
									muCOSAResult res = muCOSAW32_window_create(info, win);
									if (res != MUCOSA_SUCCESS) {
										MU_SET_RESULT(result, res)
										if (muCOSA_result_is_fatal(res)) {
											mu_free(win);
											MU_SET_RESULT(result, res)
											return 0;
										}
									}
									return win;
								} break;)
							}

							// To avoid unused parameter warnings in some cases
							if (result) {} if (info) {}
						}

						MUDEF muWindow muCOSA_window_destroy(muCOSAContext* context, muWindow win) {
							// Get inner from context
							muCOSA_Inner* inner = (muCOSA_Inner*)context->inner;

							// Do things based on window system
							switch (inner->system) {
								default: return 0; break;

								// Win32
								MUCOSA_WIN32_CALL(case MU_WINDOW_WIN32: {
									// Destroy window
									muCOSAW32_window_destroy((muCOSAW32_Window*)win);
									// Free memory
									mu_free(win);
									return 0;
								} break;)
							}

							// To avoid unused parameter warnings in some cases
							if (win) {}
						}

					/* Main loop */

						MUDEF muBool muCOSA_window_get_closed(muCOSAContext* context, muCOSAResult* result, muWindow win) {
							// Get inner from context
							muCOSA_Inner* inner = (muCOSA_Inner*)context->inner;

							// Do things based on window system
							switch (inner->system) {
								default: return 0; break;

								// Win32
								MUCOSA_WIN32_CALL(case MU_WINDOW_WIN32: {
									return muCOSAW32_window_get_closed((muCOSAW32_Window*)win);
								} break;)
							}

							// To avoid unused parameter warnings in some cases
							if (result) {} if (win) {}
						}

						MUDEF void muCOSA_window_close(muCOSAContext* context, muWindow win) {
							// Get inner from context
							muCOSA_Inner* inner = (muCOSA_Inner*)context->inner;

							// Do things based on window system
							switch (inner->system) {
								default: return; break;

								// Win32
								MUCOSA_WIN32_CALL(case MU_WINDOW_WIN32: {
									muCOSAW32_window_close((muCOSAW32_Window*)win);
									return;
								} break;)
							}

							// To avoid unused parameter warnings in some cases
							if (win) {}
						}

						MUDEF void muCOSA_window_update(muCOSAContext* context, muCOSAResult* result, muWindow win) {
							// Get inner from context
							muCOSA_Inner* inner = (muCOSA_Inner*)context->inner;

							// Do things based on window system
							switch (inner->system) {
								default: return; break;

								// Win32
								MUCOSA_WIN32_CALL(case MU_WINDOW_WIN32: {
									muCOSAW32_window_update((muCOSAW32_Window*)win);
									return;
								} break;)
							}

							// To avoid unused parameter warnings in some cases
							if (result) {} if (win) {}
						}

					/* Get / Set */

						MUDEF void muCOSA_window_get(muCOSAContext* context, muCOSAResult* result, muWindow win, muWindowAttrib attrib, void* data) {
							// Get inner from context
							muCOSA_Inner* inner = (muCOSA_Inner*)context->inner;

							// Do things based on window system
							switch (inner->system) {
								default: return; break;

								// Win32
								MUCOSA_WIN32_CALL(case MU_WINDOW_WIN32: {
									muCOSAResult res = MUCOSA_SUCCESS;
									muCOSAW32_Window* w32_win = (muCOSAW32_Window*)win;

									// Do things based on attribute
									switch (attrib) {
										default: MU_SET_RESULT(result, MUCOSA_FAILED_UNKNOWN_WINDOW_ATTRIB) return; break;

										// Dimensions
										case MU_WINDOW_DIMENSIONS: res = muCOSAW32_window_get_dimensions(w32_win, (uint32_m*)data); break;
										// Position
										case MU_WINDOW_POSITION: res = muCOSAW32_window_get_position(w32_win, (int32_m*)data); break;
										// Keyboard keymap
										case MU_WINDOW_KEYBOARD_MAP: res = muCOSAW32_window_get_keyboard_map(w32_win, (muBool**)data); break;
										// Keystate keymap
										case MU_WINDOW_KEYSTATE_MAP: res = muCOSAW32_window_get_keystate_map(w32_win, (muBool**)data); break;
										// Mouse keymap
										case MU_WINDOW_MOUSE_MAP: res = muCOSAW32_window_get_mouse_map(w32_win, (muBool**)data); break;
										// Scroll
										case MU_WINDOW_SCROLL_LEVEL: muCOSAW32_window_get_scroll(w32_win, (int32_m*)data); return; break;
										// Cursor
										case MU_WINDOW_CURSOR: res = muCOSAW32_window_get_cursor_pos(w32_win, (int32_m*)data); break;
										// Cursor style
										case MU_WINDOW_CURSOR_STYLE: res = muCOSAW32_window_get_cursor_style(w32_win, (muCursorStyle*)data); break;
									}

									if (res != MUCOSA_SUCCESS) {
										MU_SET_RESULT(result, res)
									}
									return;
								} break;)
							}

							// To avoid unused parameter warnings in some cases
							if (result) {} if (win) {} if (attrib) {} if (data) {}
						}

						MUDEF void muCOSA_window_set(muCOSAContext* context, muCOSAResult* result, muWindow win, muWindowAttrib attrib, void* data) {
							// Get inner from context
							muCOSA_Inner* inner = (muCOSA_Inner*)context->inner;

							// Do things based on window system
							switch (inner->system) {
								default: return; break;

								// Win32
								MUCOSA_WIN32_CALL(case MU_WINDOW_WIN32: {
									muCOSAResult res = MUCOSA_SUCCESS;
									muCOSAW32_Window* w32_win = (muCOSAW32_Window*)win;

									// Do things based on attribute
									switch (attrib) {
										default: MU_SET_RESULT(result, MUCOSA_FAILED_UNKNOWN_WINDOW_ATTRIB) return; break;

										// Title
										case MU_WINDOW_TITLE: res = muCOSAW32_window_set_title(w32_win, (char*)data); break;
										// Dimensions
										case MU_WINDOW_DIMENSIONS: res = muCOSAW32_window_set_dimensions(w32_win, (uint32_m*)data); break;
										// Position
										case MU_WINDOW_POSITION: res = muCOSAW32_window_set_position(w32_win, (int32_m*)data); break;
										// Scroll
										case MU_WINDOW_SCROLL_LEVEL: muCOSAW32_window_set_scroll(w32_win, (int32_m*)data); return; break;
										// Cursor
										case MU_WINDOW_CURSOR: res = muCOSAW32_window_set_cursor_pos(w32_win, (int32_m*)data); break;
										// Cursor style
										case MU_WINDOW_CURSOR_STYLE: res = muCOSAW32_window_set_cursor_style(w32_win, (muCursorStyle*)data); break;
										// Callbacks
										case MU_WINDOW_DIMENSIONS_CALLBACK: case MU_WINDOW_POSITION_CALLBACK:
										case MU_WINDOW_KEYBOARD_CALLBACK: case MU_WINDOW_KEYSTATE_CALLBACK:
										case MU_WINDOW_MOUSE_KEY_CALLBACK: case MU_WINDOW_CURSOR_CALLBACK:
										case MU_WINDOW_SCROLL_CALLBACK: muCOSAW32_window_set_callback(w32_win, attrib, data); return; break;
									}

									if (res != MUCOSA_SUCCESS) {
										MU_SET_RESULT(result, res)
									}
									return;
								} break;)
							}

							// To avoid unused parameter warnings in some cases
							if (result) {} if (win) {} if (attrib) {} if (data) {}
						}

					/* Text input */

						MUDEF void muCOSA_window_get_text_input(muCOSAContext* context, muCOSAResult* result, muWindow win, uint32_m text_cursor_x, uint32_m text_cursor_y, void (*callback)(muWindow window, uint8_m* data)) {
							// Get inner from context
							muCOSA_Inner* inner = (muCOSA_Inner*)context->inner;

							// Do things based on window system
							switch (inner->system) {
								default: return; break;

								// Win32
								MUCOSA_WIN32_CALL(case MU_WINDOW_WIN32: {
									muCOSAW32_window_get_text_input(result, (muCOSAW32_Window*)win, text_cursor_x, text_cursor_y, callback);
									return;
								} break;)
							}

							// To avoid unused parameter warnings in some cases
							if (result) {} if (win) {} if (text_cursor_x) {} if (text_cursor_y) {} if (callback) {}
						}

						MUDEF void muCOSA_window_let_text_input(muCOSAContext* context, muWindow win) {
							// Get inner from context
							muCOSA_Inner* inner = (muCOSA_Inner*)context->inner;

							// Do things based on window system
							switch (inner->system) {
								default: return; break;

								// Win32
								MUCOSA_WIN32_CALL(case MU_WINDOW_WIN32: {
									muCOSAW32_window_let_text_input((muCOSAW32_Window*)win);
									return;
								} break;)
							}

							// To avoid unused parameter warnings in some cases
							if (win) {}
						}

						MUDEF void muCOSA_window_update_text_cursor(muCOSAContext* context, muCOSAResult* result, muWindow win, uint32_m x, uint32_m y) {
							// Get inner from context
							muCOSA_Inner* inner = (muCOSA_Inner*)context->inner;

							// Do things based on window system
							switch (inner->system) {
								default: return; break;

								// Win32
								MUCOSA_WIN32_CALL(case MU_WINDOW_WIN32: {
									muCOSAW32_window_update_text_cursor(result, (muCOSAW32_Window*)win, x, y);
									return;
								} break;)
							}

							// To avoid unused parameter warnings in some cases
							if (result) {} if (win) {} if (x) {} if (y) {}
						}

					/* OpenGL */

						MUDEF muGLContext muCOSA_gl_context_create(muCOSAContext* context, muCOSAResult* result, muWindow win, muGraphicsAPI api) {
							#ifdef MU_SUPPORT_OPENGL
							// Get inner from context
							muCOSA_Inner* inner = (muCOSA_Inner*)context->inner;

							// Do things based on window system
							switch (inner->system) {
								default: return 0; break;

								// Win32
								MUCOSA_WIN32_CALL(case MU_WINDOW_WIN32: {
									return muCOSAW32_gl_context_create(
										(muCOSAW32_Context*)inner->context, result,
										(muCOSAW32_Window*)win, api
									);
								} break;)
							}

							// To avoid parameter warnings in certain circumstances
							if (result) {} if (win) {} if (api) {}

							// Fallback for non-OpenGL support:
							#else
							MU_SET_RESULT(result, MUCOSA_FAILED_UNSUPPORTED_GRAPHICS_API)
							return 0;
							if (context) {} if (result) {} if (win) {} if (api) {}
							#endif
						}

						MUDEF muGLContext muCOSA_gl_context_destroy(muCOSAContext* context, muWindow win, muGLContext gl_context) {
							#ifdef MU_SUPPORT_OPENGL
							// Get inner from context
							muCOSA_Inner* inner = (muCOSA_Inner*)context->inner;

							// Do things based on window system
							switch (inner->system) {
								default: return 0; break;

								// Win32
								MUCOSA_WIN32_CALL(case MU_WINDOW_WIN32: {
									muCOSAW32_gl_context_destroy((muCOSAW32_GL*)gl_context);
									return 0;
								} break;)
							}

							// To avoid parameter warnings in certain circumstances
							if (win) {} if (gl_context) {}

							// Fallback for non-OpenGL support:
							#else
							return 0;
							if (context) {} if (win) {} if (gl_context) {}
							#endif
						}

						MUDEF void muCOSA_gl_bind(muCOSAContext* context, muCOSAResult* result, muWindow win, muGLContext gl_context) {
							#ifdef MU_SUPPORT_OPENGL
							// Get inner from context
							muCOSA_Inner* inner = (muCOSA_Inner*)context->inner;

							// Do things based on window system
							switch (inner->system) {
								default: return; break;

								// Win32
								MUCOSA_WIN32_CALL(case MU_WINDOW_WIN32: {
									muCOSAResult res = muCOSAW32_gl_bind((muCOSAW32_GL*)gl_context);
									if (res != MUCOSA_SUCCESS) {
										MU_SET_RESULT(result, res)
									}
									return;
								} break;)
							}

							// To avoid parameter warnings in certain circumstances
							if (result) {} if (win) {} if (gl_context) {}

							// Fallback for non-OpenGL support:
							#else
							MU_SET_RESULT(result, MUCOSA_FAILED_UNSUPPORTED_GRAPHICS_API)
							return;
							if (context) {} if (result) {} if (win) {} if (gl_context) {}
							#endif
						}

						MUDEF void muCOSA_gl_swap_buffers(muCOSAContext* context, muCOSAResult* result, muWindow win) {
							#ifdef MU_SUPPORT_OPENGL
							// Get inner from context
							muCOSA_Inner* inner = (muCOSA_Inner*)context->inner;

							// Do things based on window system
							switch (inner->system) {
								default: return; break;

								// Win32
								MUCOSA_WIN32_CALL(case MU_WINDOW_WIN32: {
									muCOSAResult res = muCOSAW32_gl_swap_buffers((muCOSAW32_Window*)win);
									if (res != MUCOSA_SUCCESS) {
										MU_SET_RESULT(result, res)
									}
									return;
								} break;)
							}

							// To avoid parameter warnings in certain circumstances
							if (result) {} if (win) {}

							// Fallback for non-OpenGL support:
							#else
							MU_SET_RESULT(result, MUCOSA_FAILED_UNSUPPORTED_GRAPHICS_API)
							return;
							if (context) {} if (result) {} if (win) {}
							#endif
						}

						MUDEF void* muCOSA_gl_get_proc_address(muCOSAContext* context, const char* name) {
							#ifdef MU_SUPPORT_OPENGL
							// Get inner from context
							muCOSA_Inner* inner = (muCOSA_Inner*)context->inner;

							// Do things based on window system
							switch (inner->system) {
								default: return 0; break;

								// Win32
								MUCOSA_WIN32_CALL(case MU_WINDOW_WIN32: {
									return muCOSAW32_gl_get_proc_address(name);
								} break;)
							}

							// To avoid parameter warnings in certain circumstances
							if (name) {}

							// Fallback for non-OpenGL support:
							#else
							return 0;
							if (context) {} if (name) {}
							#endif
						}

						MUDEF muBool muCOSA_gl_swap_interval(muCOSAContext* context, muCOSAResult* result, int interval) {
							#ifdef MU_SUPPORT_OPENGL
							// Get inner from context
							muCOSA_Inner* inner = (muCOSA_Inner*)context->inner;

							// Do things based on window system
							switch (inner->system) {
								default: return 0; break;

								// Win32
								MUCOSA_WIN32_CALL(case MU_WINDOW_WIN32: {
									return muCOSAW32_gl_swap_interval((muCOSAW32_Context*)inner->context, result, interval);
								} break;)
							}

							// Fallback for non-OpenGL support
							#else
							MU_SET_RESULT(result, MUCOSA_FAILED_UNSUPPORTED_GRAPHICS_API)
							return 0;
							if (context) {} if (result) {} if (interval) {}
							#endif
						}

				/* Time */

					MUDEF double muCOSA_fixed_time_get(muCOSAContext* context) {
						// Get inner from context
						muCOSA_Inner* inner = (muCOSA_Inner*)context->inner;

						// Do things based on window system
						switch (inner->system) {
							default: return 0.0; break;

							// Win32
							MUCOSA_WIN32_CALL(case MU_WINDOW_WIN32: {
								return muCOSAW32_fixed_time_get((muCOSAW32_Context*)inner->context);
							} break;)
						}
					}

					MUDEF double muCOSA_time_get(muCOSAContext* context) {
						// Get inner from context
						muCOSA_Inner* inner = (muCOSA_Inner*)context->inner;

						// Do things based on window system
						switch (inner->system) {
							default: return 0.0; break;

							// Win32
							MUCOSA_WIN32_CALL(case MU_WINDOW_WIN32: {
								return muCOSAW32_time_get((muCOSAW32_Context*)inner->context);
							} break;)
						}
					}

					MUDEF void muCOSA_time_set(muCOSAContext* context, double time) {
						// Get inner from context
						muCOSA_Inner* inner = (muCOSA_Inner*)context->inner;

						// Do things based on window system
						switch (inner->system) {
							default: return; break;

							// Win32
							MUCOSA_WIN32_CALL(case MU_WINDOW_WIN32: {
								muCOSAW32_time_set((muCOSAW32_Context*)inner->context, time);
								return;
							} break;)
						}

						// To avoid parameter warnings in certain cirumstances
						if (time) {}
					}

				/* Sleep */

					MUDEF void muCOSA_sleep(muCOSAContext* context, double time) {
						// Get inner from context
						muCOSA_Inner* inner = (muCOSA_Inner*)context->inner;

						// Do things based on window system
						switch (inner->system) {
							default: return; break;

							// Win32
							MUCOSA_WIN32_CALL(case MU_WINDOW_WIN32: {
								muCOSAW32_sleep(time);
								return;
							} break;)
						}

						// To avoid parameter warnings in certain circumstances
						if (time) {}
					}

				/* Clipboard */

					MUDEF uint8_m* muCOSA_clipboard_get(muCOSAContext* context, muCOSAResult* result) {
						// Get inner from context
						muCOSA_Inner* inner = (muCOSA_Inner*)context->inner;

						// Do things based on window system
						switch (inner->system) {
							default: return 0; break;

							// Win32
							MUCOSA_WIN32_CALL(case MU_WINDOW_WIN32: {
								return muCOSAW32_clipboard_get(result);
							} break;)
						}

						// To avoid unused parameter warnings in some circumstances
						if (result) {}
					}

					MUDEF void muCOSA_clipboard_set(muCOSAContext* context, muCOSAResult* result, uint8_m* data, size_m datalen) {
						// Get inner from context
						muCOSA_Inner* inner = (muCOSA_Inner*)context->inner;

						// Do things based on window system
						switch (inner->system) {
							default: return; break;

							// Win32
							MUCOSA_WIN32_CALL(case MU_WINDOW_WIN32: {
								muCOSAW32_clipboard_set(result, data, datalen);
								return;
							} break;)
						}

						// To avoid unused parameter warnings in some circumstances
						if (result) {} if (data) {} if (datalen) {}
					}

				/* Result */

					MUDEF muBool muCOSA_result_is_fatal(muCOSAResult result) {
						switch (result) {
							// Fatal errors (assumed fatal):
							default: return MU_TRUE; break;

							// All non-fatal errors:
							case MUCOSA_SUCCESS:
							case MUCOSA_WIN32_FAILED_REGISTER_DUMMY_WGL_WINDOW_CLASS:
							case MUCOSA_WIN32_FAILED_CREATE_DUMMY_WGL_WINDOW:
							case MUCOSA_WIN32_FAILED_GET_DUMMY_WGL_PIXEL_FORMAT:
							case MUCOSA_WIN32_FAILED_DESCRIBE_DUMMY_WGL_PIXEL_FORMAT:
							case MUCOSA_WIN32_FAILED_SET_DUMMY_WGL_PIXEL_FORMAT:
							case MUCOSA_WIN32_FAILED_CREATE_DUMMY_WGL_CONTEXT:
							case MUCOSA_WIN32_FAILED_BIND_DUMMY_WGL_CONTEXT:
							case MUCOSA_WIN32_FAILED_FIND_WGL_CREATE_CONTEXT_ATTRIBS:
							case MUCOSA_WIN32_FAILED_FIND_WGL_CHOOSE_PIXEL_FORMAT:
							case MUCOSA_WIN32_FAILED_CHOOSE_WGL_PIXEL_FORMAT:
								return MU_FALSE;
							break;
						}
					}

				/* Names */

				MUCOSA_NAME_CALL(
					MUDEF const char* muCOSA_result_get_name(muCOSAResult result) {
						switch (result) {
							default: return "MU_UNKNOWN"; break;

							case MUCOSA_SUCCESS: return "MUCOSA_SUCCESS"; break;
							case MUCOSA_FAILED_NULL_WINDOW_SYSTEM: return "MUCOSA_FAILED_NULL_WINDOW_SYSTEM"; break;
							case MUCOSA_FAILED_MALLOC: return "MUCOSA_FAILED_MALLOC"; break;
							case MUCOSA_FAILED_UNKNOWN_WINDOW_ATTRIB: return "MUCOSA_FAILED_UNKNOWN_WINDOW_ATTRIB"; break;
							case MUCOSA_FAILED_REALLOC: return "MUCOSA_FAILED_REALLOC"; break;
							case MUCOSA_FAILED_UNKNOWN_GRAPHICS_API: return "MUCOSA_FAILED_UNKNOWN_GRAPHICS_API"; break;
							case MUCOSA_FAILED_UNSUPPORTED_GRAPHICS_API: return "MUCOSA_FAILED_UNSUPPORTED_GRAPHICS_API"; break;

							case MUCOSA_WIN32_FAILED_CONVERT_UTF8_TO_WCHAR: return "MUCOSA_WIN32_FAILED_CONVERT_UTF8_TO_WCHAR"; break;
							case MUCOSA_WIN32_FAILED_REGISTER_WINDOW_CLASS: return "MUCOSA_WIN32_FAILED_REGISTER_WINDOW_CLASS"; break;
							case MUCOSA_WIN32_FAILED_CREATE_WINDOW: return "MUCOSA_WIN32_FAILED_CREATE_WINDOW"; break;
							case MUCOSA_WIN32_FAILED_GET_WINDOW_ATTRIB: return "MUCOSA_WIN32_FAILED_GET_WINDOW_ATTRIB"; break;
							case MUCOSA_WIN32_FAILED_SET_WINDOW_ATTRIB: return "MUCOSA_WIN32_FAILED_SET_WINDOW_ATTRIB"; break;
							case MUCOSA_WIN32_FAILED_REGISTER_DUMMY_WGL_WINDOW_CLASS: return "MUCOSA_WIN32_FAILED_REGISTER_DUMMY_WGL_WINDOW_CLASS"; break;
							case MUCOSA_WIN32_FAILED_CREATE_DUMMY_WGL_WINDOW: return "MUCOSA_WIN32_FAILED_CREATE_DUMMY_WGL_WINDOW"; break;
							case MUCOSA_WIN32_FAILED_GET_DUMMY_WGL_PIXEL_FORMAT: return "MUCOSA_WIN32_FAILED_GET_DUMMY_WGL_PIXEL_FORMAT"; break;
							case MUCOSA_WIN32_FAILED_DESCRIBE_DUMMY_WGL_PIXEL_FORMAT: return "MUCOSA_WIN32_FAILED_DESCRIBE_DUMMY_WGL_PIXEL_FORMAT"; break;
							case MUCOSA_WIN32_FAILED_SET_DUMMY_WGL_PIXEL_FORMAT: return "MUCOSA_WIN32_FAILED_SET_DUMMY_WGL_PIXEL_FORMAT"; break;
							case MUCOSA_WIN32_FAILED_CREATE_DUMMY_WGL_CONTEXT: return "MUCOSA_WIN32_FAILED_CREATE_DUMMY_WGL_CONTEXT"; break;
							case MUCOSA_WIN32_FAILED_BIND_DUMMY_WGL_CONTEXT: return "MUCOSA_WIN32_FAILED_BIND_DUMMY_WGL_CONTEXT"; break;
							case MUCOSA_WIN32_FAILED_FIND_WGL_CREATE_CONTEXT_ATTRIBS: return "MUCOSA_WIN32_FAILED_FIND_WGL_CREATE_CONTEXT_ATTRIBS"; break;
							case MUCOSA_WIN32_FAILED_FIND_WGL_CHOOSE_PIXEL_FORMAT: return "MUCOSA_WIN32_FAILED_FIND_WGL_CHOOSE_PIXEL_FORMAT"; break;
							case MUCOSA_WIN32_FAILED_CHOOSE_WGL_PIXEL_FORMAT: return "MUCOSA_WIN32_FAILED_CHOOSE_WGL_PIXEL_FORMAT"; break;
							case MUCOSA_WIN32_FAILED_GET_WGL_PIXEL_FORMAT: return "MUCOSA_WIN32_FAILED_GET_WGL_PIXEL_FORMAT"; break;
							case MUCOSA_WIN32_FAILED_DESCRIBE_WGL_PIXEL_FORMAT: return "MUCOSA_WIN32_FAILED_DESCRIBE_WGL_PIXEL_FORMAT"; break;
							case MUCOSA_WIN32_FAILED_SET_WGL_PIXEL_FORMAT: return "MUCOSA_WIN32_FAILED_SET_WGL_PIXEL_FORMAT"; break;
							case MUCOSA_WIN32_FAILED_CREATE_WGL_CONTEXT: return "MUCOSA_WIN32_FAILED_CREATE_WGL_CONTEXT"; break;
							case MUCOSA_WIN32_FAILED_SET_WGL_CONTEXT: return "MUCOSA_WIN32_FAILED_SET_WGL_CONTEXT"; break;
							case MUCOSA_WIN32_FAILED_SWAP_WGL_BUFFERS: return "MUCOSA_WIN32_FAILED_SWAP_WGL_BUFFERS"; break;
							case MUCOSA_WIN32_FAILED_FIND_WGL_FUNCTION: return "MUCOSA_WIN32_FAILED_FIND_WGL_FUNCTION"; break;
							case MUCOSA_WIN32_FAILED_ASSOCIATE_IMM: return "MUCOSA_WIN32_FAILED_ASSOCIATE_IMM"; break;
							case MUCOSA_WIN32_FAILED_SET_COMPOSITION_WINDOW_POSITION: return "MUCOSA_WIN32_FAILED_SET_COMPOSITION_WINDOW_POSITION"; break;
							case MUCOSA_WIN32_FAILED_HOLD_CLIPBOARD: return "MUCOSA_WIN32_FAILED_HOLD_CLIPBOARD"; break;
							case MUCOSA_WIN32_FAILED_GET_CLIPBOARD_DATA: return "MUCOSA_WIN32_FAILED_GET_CLIPBOARD_DATA"; break;
							case MUCOSA_WIN32_FAILED_CONVERT_CLIPBOARD_DATA_FORMAT: return "MUCOSA_WIN32_FAILED_CONVERT_CLIPBOARD_DATA_FORMAT"; break;
							case MUCOSA_WIN32_FAILED_ALLOCATE_CLIPBOARD_DATA: return "MUCOSA_WIN32_FAILED_ALLOCATE_CLIPBOARD_DATA"; break;
							case MUCOSA_WIN32_FAILED_SET_CLIPBOARD_DATA: return "MUCOSA_WIN32_FAILED_SET_CLIPBOARD_DATA"; break;
						}
					}

					MUDEF const char* mu_window_system_get_name(muWindowSystem system) {
						switch (system) {
							default: return "MU_UNKNOWN"; break;

							case MU_WINDOW_NULL: return "MU_WINDOW_NULL"; break;
							case MU_WINDOW_WIN32: return "MU_WINDOW_WIN32"; break;
						}
					}

					MUDEF const char* mu_window_system_get_nice_name(muWindowSystem system) {
						switch (system) {
							default: return "Unknown"; break;

							case MU_WINDOW_NULL: return "Unknown/Auto"; break;
							case MU_WINDOW_WIN32: return "Win32"; break;
						}
					}

					MUDEF const char* mu_window_attrib_get_name(muWindowAttrib attrib) {
						switch (attrib) {
							default: return "MU_UNKNOWN"; break;
							case MU_WINDOW_TITLE: return "MU_WINDOW_TITLE"; break;
							case MU_WINDOW_DIMENSIONS: return "MU_WINDOW_DIMENSIONS"; break;
							case MU_WINDOW_POSITION: return "MU_WINDOW_POSITION"; break;
							case MU_WINDOW_KEYBOARD_MAP: return "MU_WINDOW_KEYBOARD_MAP"; break;
							case MU_WINDOW_KEYSTATE_MAP: return "MU_WINDOW_KEYSTATE_MAP"; break;
							case MU_WINDOW_MOUSE_MAP: return "MU_WINDOW_MOUSE_MAP"; break;
							case MU_WINDOW_SCROLL_LEVEL: return "MU_WINDOW_SCROLL_LEVEL"; break;
							case MU_WINDOW_CURSOR: return "MU_WINDOW_CURSOR"; break;
							case MU_WINDOW_CURSOR_STYLE: return "MU_WINDOW_CURSOR_STYLE"; break;
							case MU_WINDOW_DIMENSIONS_CALLBACK: return "MU_WINDOW_DIMENSIONS_CALLBACK"; break;
							case MU_WINDOW_POSITION_CALLBACK: return "MU_WINDOW_POSITION_CALLBACK"; break;
							case MU_WINDOW_KEYBOARD_CALLBACK: return "MU_WINDOW_KEYBOARD_CALLBACK"; break;
							case MU_WINDOW_KEYSTATE_CALLBACK: return "MU_WINDOW_KEYSTATE_CALLBACK"; break;
							case MU_WINDOW_MOUSE_KEY_CALLBACK: return "MU_WINDOW_MOUSE_KEY_CALLBACK"; break;
							case MU_WINDOW_CURSOR_CALLBACK: return "MU_WINDOW_CURSOR_CALLBACK"; break;
							case MU_WINDOW_SCROLL_CALLBACK: return "MU_WINDOW_SCROLL_CALLBACK"; break;
						}
					}

					MUDEF const char* mu_window_attrib_get_nice_name(muWindowAttrib attrib) {
						switch (attrib) {
							default: return "Unknown"; break;
							case MU_WINDOW_TITLE: return "Title"; break;
							case MU_WINDOW_DIMENSIONS: return "Dimensions"; break;
							case MU_WINDOW_POSITION: return "Position"; break;
							case MU_WINDOW_KEYBOARD_MAP: return "Keyboard map"; break;
							case MU_WINDOW_KEYSTATE_MAP: return "Keystate map"; break;
							case MU_WINDOW_MOUSE_MAP: return "Mouse map"; break;
							case MU_WINDOW_SCROLL_LEVEL: return "Scroll level"; break;
							case MU_WINDOW_CURSOR: return "Cursor"; break;
							case MU_WINDOW_CURSOR_STYLE: return "Cursor style"; break;
							case MU_WINDOW_DIMENSIONS_CALLBACK: return "Dimensions callback"; break;
							case MU_WINDOW_POSITION_CALLBACK: return "Position callback"; break;
							case MU_WINDOW_KEYBOARD_CALLBACK: return "Keyboard callback"; break;
							case MU_WINDOW_KEYSTATE_CALLBACK: return "Keystate callback"; break;
							case MU_WINDOW_MOUSE_KEY_CALLBACK: return "Mouse key callback"; break;
							case MU_WINDOW_CURSOR_CALLBACK: return "Cursor callback"; break;
							case MU_WINDOW_SCROLL_CALLBACK: return "Scroll callback"; break;
						}
					}

					MUDEF const char* mu_keyboard_key_get_name(muKeyboardKey key) {
						switch (key) {
							default: return "MU_UNKNOWN"; break;
							case MU_KEYBOARD_UNKNOWN: return "MU_KEYBOARD_UNKNOWN"; break;
							case MU_KEYBOARD_BACKSPACE: return "MU_KEYBOARD_BACKSPACE"; break;
							case MU_KEYBOARD_TAB: return "MU_KEYBOARD_TAB"; break;
							case MU_KEYBOARD_CLEAR: return "MU_KEYBOARD_CLEAR"; break;
							case MU_KEYBOARD_RETURN: return "MU_KEYBOARD_RETURN"; break;
							case MU_KEYBOARD_PAUSE: return "MU_KEYBOARD_PAUSE"; break;
							case MU_KEYBOARD_ESCAPE: return "MU_KEYBOARD_ESCAPE"; break;
							case MU_KEYBOARD_MODECHANGE: return "MU_KEYBOARD_MODECHANGE"; break;
							case MU_KEYBOARD_SPACE: return "MU_KEYBOARD_SPACE"; break;
							case MU_KEYBOARD_PRIOR: return "MU_KEYBOARD_PRIOR"; break;
							case MU_KEYBOARD_NEXT: return "MU_KEYBOARD_NEXT"; break;
							case MU_KEYBOARD_END: return "MU_KEYBOARD_END"; break;
							case MU_KEYBOARD_HOME: return "MU_KEYBOARD_HOME"; break;
							case MU_KEYBOARD_LEFT: return "MU_KEYBOARD_LEFT"; break;
							case MU_KEYBOARD_UP: return "MU_KEYBOARD_UP"; break;
							case MU_KEYBOARD_RIGHT: return "MU_KEYBOARD_RIGHT"; break;
							case MU_KEYBOARD_DOWN: return "MU_KEYBOARD_DOWN"; break;
							case MU_KEYBOARD_SELECT: return "MU_KEYBOARD_SELECT"; break;
							case MU_KEYBOARD_PRINT: return "MU_KEYBOARD_PRINT"; break;
							case MU_KEYBOARD_EXECUTE: return "MU_KEYBOARD_EXECUTE"; break;
							case MU_KEYBOARD_INSERT: return "MU_KEYBOARD_INSERT"; break;
							case MU_KEYBOARD_DELETE: return "MU_KEYBOARD_DELETE"; break;
							case MU_KEYBOARD_HELP: return "MU_KEYBOARD_HELP"; break;
							case MU_KEYBOARD_0: return "MU_KEYBOARD_0"; break;
							case MU_KEYBOARD_1: return "MU_KEYBOARD_1"; break;
							case MU_KEYBOARD_2: return "MU_KEYBOARD_2"; break;
							case MU_KEYBOARD_3: return "MU_KEYBOARD_3"; break;
							case MU_KEYBOARD_4: return "MU_KEYBOARD_4"; break;
							case MU_KEYBOARD_5: return "MU_KEYBOARD_5"; break;
							case MU_KEYBOARD_6: return "MU_KEYBOARD_6"; break;
							case MU_KEYBOARD_7: return "MU_KEYBOARD_7"; break;
							case MU_KEYBOARD_8: return "MU_KEYBOARD_8"; break;
							case MU_KEYBOARD_9: return "MU_KEYBOARD_9"; break;
							case MU_KEYBOARD_A: return "MU_KEYBOARD_A"; break;
							case MU_KEYBOARD_B: return "MU_KEYBOARD_B"; break;
							case MU_KEYBOARD_C: return "MU_KEYBOARD_C"; break;
							case MU_KEYBOARD_D: return "MU_KEYBOARD_D"; break;
							case MU_KEYBOARD_E: return "MU_KEYBOARD_E"; break;
							case MU_KEYBOARD_F: return "MU_KEYBOARD_F"; break;
							case MU_KEYBOARD_G: return "MU_KEYBOARD_G"; break;
							case MU_KEYBOARD_H: return "MU_KEYBOARD_H"; break;
							case MU_KEYBOARD_I: return "MU_KEYBOARD_I"; break;
							case MU_KEYBOARD_J: return "MU_KEYBOARD_J"; break;
							case MU_KEYBOARD_K: return "MU_KEYBOARD_K"; break;
							case MU_KEYBOARD_L: return "MU_KEYBOARD_L"; break;
							case MU_KEYBOARD_M: return "MU_KEYBOARD_M"; break;
							case MU_KEYBOARD_N: return "MU_KEYBOARD_N"; break;
							case MU_KEYBOARD_O: return "MU_KEYBOARD_O"; break;
							case MU_KEYBOARD_P: return "MU_KEYBOARD_P"; break;
							case MU_KEYBOARD_Q: return "MU_KEYBOARD_Q"; break;
							case MU_KEYBOARD_R: return "MU_KEYBOARD_R"; break;
							case MU_KEYBOARD_S: return "MU_KEYBOARD_S"; break;
							case MU_KEYBOARD_T: return "MU_KEYBOARD_T"; break;
							case MU_KEYBOARD_U: return "MU_KEYBOARD_U"; break;
							case MU_KEYBOARD_V: return "MU_KEYBOARD_V"; break;
							case MU_KEYBOARD_W: return "MU_KEYBOARD_W"; break;
							case MU_KEYBOARD_X: return "MU_KEYBOARD_X"; break;
							case MU_KEYBOARD_Y: return "MU_KEYBOARD_Y"; break;
							case MU_KEYBOARD_Z: return "MU_KEYBOARD_Z"; break;
							case MU_KEYBOARD_LEFT_WINDOWS: return "MU_KEYBOARD_LEFT_WINDOWS"; break;
							case MU_KEYBOARD_RIGHT_WINDOWS: return "MU_KEYBOARD_RIGHT_WINDOWS"; break;
							case MU_KEYBOARD_NUMPAD_0: return "MU_KEYBOARD_NUMPAD_0"; break;
							case MU_KEYBOARD_NUMPAD_1: return "MU_KEYBOARD_NUMPAD_1"; break;
							case MU_KEYBOARD_NUMPAD_2: return "MU_KEYBOARD_NUMPAD_2"; break;
							case MU_KEYBOARD_NUMPAD_3: return "MU_KEYBOARD_NUMPAD_3"; break;
							case MU_KEYBOARD_NUMPAD_4: return "MU_KEYBOARD_NUMPAD_4"; break;
							case MU_KEYBOARD_NUMPAD_5: return "MU_KEYBOARD_NUMPAD_5"; break;
							case MU_KEYBOARD_NUMPAD_6: return "MU_KEYBOARD_NUMPAD_6"; break;
							case MU_KEYBOARD_NUMPAD_7: return "MU_KEYBOARD_NUMPAD_7"; break;
							case MU_KEYBOARD_NUMPAD_8: return "MU_KEYBOARD_NUMPAD_8"; break;
							case MU_KEYBOARD_NUMPAD_9: return "MU_KEYBOARD_NUMPAD_9"; break;
							case MU_KEYBOARD_ADD: return "MU_KEYBOARD_ADD"; break;
							case MU_KEYBOARD_SUBTRACT: return "MU_KEYBOARD_SUBTRACT"; break;
							case MU_KEYBOARD_MULTIPLY: return "MU_KEYBOARD_MULTIPLY"; break;
							case MU_KEYBOARD_DIVIDE: return "MU_KEYBOARD_DIVIDE"; break;
							case MU_KEYBOARD_SEPARATOR: return "MU_KEYBOARD_SEPARATOR"; break;
							case MU_KEYBOARD_DECIMAL: return "MU_KEYBOARD_DECIMAL"; break;
							case MU_KEYBOARD_F1: return "MU_KEYBOARD_F1"; break;
							case MU_KEYBOARD_F2: return "MU_KEYBOARD_F2"; break;
							case MU_KEYBOARD_F3: return "MU_KEYBOARD_F3"; break;
							case MU_KEYBOARD_F4: return "MU_KEYBOARD_F4"; break;
							case MU_KEYBOARD_F5: return "MU_KEYBOARD_F5"; break;
							case MU_KEYBOARD_F6: return "MU_KEYBOARD_F6"; break;
							case MU_KEYBOARD_F7: return "MU_KEYBOARD_F7"; break;
							case MU_KEYBOARD_F8: return "MU_KEYBOARD_F8"; break;
							case MU_KEYBOARD_F9: return "MU_KEYBOARD_F9"; break;
							case MU_KEYBOARD_F10: return "MU_KEYBOARD_F10"; break;
							case MU_KEYBOARD_F11: return "MU_KEYBOARD_F11"; break;
							case MU_KEYBOARD_F12: return "MU_KEYBOARD_F12"; break;
							case MU_KEYBOARD_F13: return "MU_KEYBOARD_F13"; break;
							case MU_KEYBOARD_F14: return "MU_KEYBOARD_F14"; break;
							case MU_KEYBOARD_F15: return "MU_KEYBOARD_F15"; break;
							case MU_KEYBOARD_F16: return "MU_KEYBOARD_F16"; break;
							case MU_KEYBOARD_F17: return "MU_KEYBOARD_F17"; break;
							case MU_KEYBOARD_F18: return "MU_KEYBOARD_F18"; break;
							case MU_KEYBOARD_F19: return "MU_KEYBOARD_F19"; break;
							case MU_KEYBOARD_F20: return "MU_KEYBOARD_F20"; break;
							case MU_KEYBOARD_F21: return "MU_KEYBOARD_F21"; break;
							case MU_KEYBOARD_F22: return "MU_KEYBOARD_F22"; break;
							case MU_KEYBOARD_F23: return "MU_KEYBOARD_F23"; break;
							case MU_KEYBOARD_F24: return "MU_KEYBOARD_F24"; break;
							case MU_KEYBOARD_NUMLOCK: return "MU_KEYBOARD_NUMLOCK"; break;
							case MU_KEYBOARD_SCROLL: return "MU_KEYBOARD_SCROLL"; break;
							case MU_KEYBOARD_LEFT_SHIFT: return "MU_KEYBOARD_LEFT_SHIFT"; break;
							case MU_KEYBOARD_RIGHT_SHIFT: return "MU_KEYBOARD_RIGHT_SHIFT"; break;
							case MU_KEYBOARD_LEFT_CONTROL: return "MU_KEYBOARD_LEFT_CONTROL"; break;
							case MU_KEYBOARD_RIGHT_CONTROL: return "MU_KEYBOARD_RIGHT_CONTROL"; break;
							case MU_KEYBOARD_LEFT_MENU: return "MU_KEYBOARD_LEFT_MENU"; break;
							case MU_KEYBOARD_RIGHT_MENU: return "MU_KEYBOARD_RIGHT_MENU"; break;
							case MU_KEYBOARD_ATTN: return "MU_KEYBOARD_ATTN"; break;
							case MU_KEYBOARD_CRSEL: return "MU_KEYBOARD_CRSEL"; break;
							case MU_KEYBOARD_EXSEL: return "MU_KEYBOARD_EXSEL"; break;
							case MU_KEYBOARD_EREOF: return "MU_KEYBOARD_EREOF"; break;
							case MU_KEYBOARD_PLAY: return "MU_KEYBOARD_PLAY"; break;
							case MU_KEYBOARD_PA1: return "MU_KEYBOARD_PA1"; break;
						}
					}

					MUDEF const char* mu_keyboard_key_get_nice_name(muKeyboardKey key) {
						switch (key) {
							default: return "Unknown"; break;
							case MU_KEYBOARD_UNKNOWN: return "Unknown"; break;
							case MU_KEYBOARD_BACKSPACE: return "Baskspace"; break;
							case MU_KEYBOARD_TAB: return "Tab"; break;
							case MU_KEYBOARD_CLEAR: return "Clear"; break;
							case MU_KEYBOARD_RETURN: return "Return"; break;
							case MU_KEYBOARD_PAUSE: return "Pause"; break;
							case MU_KEYBOARD_ESCAPE: return "Escape"; break;
							case MU_KEYBOARD_MODECHANGE: return "Mode change"; break;
							case MU_KEYBOARD_SPACE: return "Space"; break;
							case MU_KEYBOARD_PRIOR: return "Prior"; break;
							case MU_KEYBOARD_NEXT: return "Next"; break;
							case MU_KEYBOARD_END: return "End"; break;
							case MU_KEYBOARD_HOME: return "Home"; break;
							case MU_KEYBOARD_LEFT: return "Left"; break;
							case MU_KEYBOARD_UP: return "Up"; break;
							case MU_KEYBOARD_RIGHT: return "Right"; break;
							case MU_KEYBOARD_DOWN: return "Down"; break;
							case MU_KEYBOARD_SELECT: return "Select"; break;
							case MU_KEYBOARD_PRINT: return "Print"; break;
							case MU_KEYBOARD_EXECUTE: return "Execute"; break;
							case MU_KEYBOARD_INSERT: return "Insert"; break;
							case MU_KEYBOARD_DELETE: return "Delete"; break;
							case MU_KEYBOARD_HELP: return "Help"; break;
							case MU_KEYBOARD_0: return "0"; break;
							case MU_KEYBOARD_1: return "1"; break;
							case MU_KEYBOARD_2: return "2"; break;
							case MU_KEYBOARD_3: return "3"; break;
							case MU_KEYBOARD_4: return "4"; break;
							case MU_KEYBOARD_5: return "5"; break;
							case MU_KEYBOARD_6: return "6"; break;
							case MU_KEYBOARD_7: return "7"; break;
							case MU_KEYBOARD_8: return "8"; break;
							case MU_KEYBOARD_9: return "9"; break;
							case MU_KEYBOARD_A: return "A"; break;
							case MU_KEYBOARD_B: return "B"; break;
							case MU_KEYBOARD_C: return "C"; break;
							case MU_KEYBOARD_D: return "D"; break;
							case MU_KEYBOARD_E: return "E"; break;
							case MU_KEYBOARD_F: return "F"; break;
							case MU_KEYBOARD_G: return "G"; break;
							case MU_KEYBOARD_H: return "H"; break;
							case MU_KEYBOARD_I: return "I"; break;
							case MU_KEYBOARD_J: return "J"; break;
							case MU_KEYBOARD_K: return "K"; break;
							case MU_KEYBOARD_L: return "L"; break;
							case MU_KEYBOARD_M: return "M"; break;
							case MU_KEYBOARD_N: return "N"; break;
							case MU_KEYBOARD_O: return "O"; break;
							case MU_KEYBOARD_P: return "P"; break;
							case MU_KEYBOARD_Q: return "Q"; break;
							case MU_KEYBOARD_R: return "R"; break;
							case MU_KEYBOARD_S: return "S"; break;
							case MU_KEYBOARD_T: return "T"; break;
							case MU_KEYBOARD_U: return "U"; break;
							case MU_KEYBOARD_V: return "V"; break;
							case MU_KEYBOARD_W: return "W"; break;
							case MU_KEYBOARD_X: return "X"; break;
							case MU_KEYBOARD_Y: return "Y"; break;
							case MU_KEYBOARD_Z: return "Z"; break;
							case MU_KEYBOARD_LEFT_WINDOWS: return "Left Windows/Super/Command"; break;
							case MU_KEYBOARD_RIGHT_WINDOWS: return "Right Windows/Super/Command"; break;
							case MU_KEYBOARD_NUMPAD_0: return "Numpad 0"; break;
							case MU_KEYBOARD_NUMPAD_1: return "Numpad 1"; break;
							case MU_KEYBOARD_NUMPAD_2: return "Numpad 2"; break;
							case MU_KEYBOARD_NUMPAD_3: return "Numpad 3"; break;
							case MU_KEYBOARD_NUMPAD_4: return "Numpad 4"; break;
							case MU_KEYBOARD_NUMPAD_5: return "Numpad 5"; break;
							case MU_KEYBOARD_NUMPAD_6: return "Numpad 6"; break;
							case MU_KEYBOARD_NUMPAD_7: return "Numpad 7"; break;
							case MU_KEYBOARD_NUMPAD_8: return "Numpad 8"; break;
							case MU_KEYBOARD_NUMPAD_9: return "Numpad 9"; break;
							case MU_KEYBOARD_ADD: return "Add"; break;
							case MU_KEYBOARD_SUBTRACT: return "Subtract"; break;
							case MU_KEYBOARD_MULTIPLY: return "Multiply"; break;
							case MU_KEYBOARD_DIVIDE: return "Divide"; break;
							case MU_KEYBOARD_SEPARATOR: return "Separator"; break;
							case MU_KEYBOARD_DECIMAL: return "Decimal"; break;
							case MU_KEYBOARD_F1: return "F1"; break;
							case MU_KEYBOARD_F2: return "F2"; break;
							case MU_KEYBOARD_F3: return "F3"; break;
							case MU_KEYBOARD_F4: return "F4"; break;
							case MU_KEYBOARD_F5: return "F5"; break;
							case MU_KEYBOARD_F6: return "F6"; break;
							case MU_KEYBOARD_F7: return "F7"; break;
							case MU_KEYBOARD_F8: return "F8"; break;
							case MU_KEYBOARD_F9: return "F9"; break;
							case MU_KEYBOARD_F10: return "F10"; break;
							case MU_KEYBOARD_F11: return "F11"; break;
							case MU_KEYBOARD_F12: return "F12"; break;
							case MU_KEYBOARD_F13: return "F13"; break;
							case MU_KEYBOARD_F14: return "F14"; break;
							case MU_KEYBOARD_F15: return "F15"; break;
							case MU_KEYBOARD_F16: return "F16"; break;
							case MU_KEYBOARD_F17: return "F17"; break;
							case MU_KEYBOARD_F18: return "F18"; break;
							case MU_KEYBOARD_F19: return "F19"; break;
							case MU_KEYBOARD_F20: return "F20"; break;
							case MU_KEYBOARD_F21: return "F21"; break;
							case MU_KEYBOARD_F22: return "F22"; break;
							case MU_KEYBOARD_F23: return "F23"; break;
							case MU_KEYBOARD_F24: return "F24"; break;
							case MU_KEYBOARD_NUMLOCK: return "Num Lock"; break;
							case MU_KEYBOARD_SCROLL: return "Scroll"; break;
							case MU_KEYBOARD_LEFT_SHIFT: return "Left shift"; break;
							case MU_KEYBOARD_RIGHT_SHIFT: return "Right shift"; break;
							case MU_KEYBOARD_LEFT_CONTROL: return "Left control"; break;
							case MU_KEYBOARD_RIGHT_CONTROL: return "Right control"; break;
							case MU_KEYBOARD_LEFT_MENU: return "Left menu"; break;
							case MU_KEYBOARD_RIGHT_MENU: return "Right menu"; break;
							case MU_KEYBOARD_ATTN: return "Attn"; break;
							case MU_KEYBOARD_CRSEL: return "CrSel"; break;
							case MU_KEYBOARD_EXSEL: return "ExSel"; break;
							case MU_KEYBOARD_EREOF: return "EreOf"; break;
							case MU_KEYBOARD_PLAY: return "Play"; break;
							case MU_KEYBOARD_PA1: return "PA1"; break;
						}
					}

					MUDEF const char* mu_keystate_get_name(muKeyboardState state) {
						switch (state) {
							default: return "MU_UNKNOWN"; break;
							case MU_KEYSTATE_UNKNOWN: return "MU_KEYSTATE_UNKNOWN"; break;
							case MU_KEYSTATE_CAPS_LOCK: return "MU_KEYSTATE_CAPS_LOCK"; break;
							case MU_KEYSTATE_SCROLL_LOCK: return "MU_KEYSTATE_SCROLL_LOCK"; break;
							case MU_KEYSTATE_NUM_LOCK: return "MU_KEYSTATE_NUM_LOCK"; break;
						}
					}

					MUDEF const char* mu_keystate_get_nice_name(muKeyboardState state) {
						switch (state) {
							default: return "Unknown"; break;
							case MU_KEYSTATE_UNKNOWN: return "Unknown"; break;
							case MU_KEYSTATE_CAPS_LOCK: return "Caps Lock"; break;
							case MU_KEYSTATE_SCROLL_LOCK: return "Scroll Lock"; break;
							case MU_KEYSTATE_NUM_LOCK: return "Num Lock"; break;
						}
					}

					MUDEF const char* mu_mouse_key_get_name(muMouseKey key) {
						switch (key) {
							default: return "MU_UNKNOWN"; break;
							case MU_MOUSE_UNKNOWN: return "MU_MOUSE_UNKNOWN"; break;
							case MU_MOUSE_LEFT: return "MU_MOUSE_LEFT"; break;
							case MU_MOUSE_RIGHT: return "MU_MOUSE_RIGHT"; break;
							case MU_MOUSE_MIDDLE: return "MU_MOUSE_MIDDLE"; break;
						}
					}

					MUDEF const char* mu_mouse_key_get_nice_name(muMouseKey key) {
						switch (key) {
							default: return "Unknown"; break;
							case MU_MOUSE_UNKNOWN: return "Unknown"; break;
							case MU_MOUSE_LEFT: return "Left"; break;
							case MU_MOUSE_RIGHT: return "Right"; break;
							case MU_MOUSE_MIDDLE: return "Middle"; break;
						}
					}

					MUDEF const char* mu_cursor_style_get_name(muCursorStyle style) {
						switch (style) {
							default: return "MU_UNKNOWN"; break;
							case MU_CURSOR_UNKNOWN: return "MU_CURSOR_UNKNOWN"; break;
							case MU_CURSOR_ARROW: return "MU_CURSOR_ARROW"; break;
							case MU_CURSOR_IBEAM: return "MU_CURSOR_IBEAM"; break;
							case MU_CURSOR_WAIT: return "MU_CURSOR_WAIT"; break;
							case MU_CURSOR_WAIT_ARROW: return "MU_CURSOR_WAIT_ARROW"; break;
							case MU_CURSOR_CROSSHAIR: return "MU_CURSOR_CROSSHAIR"; break;
							case MU_CURSOR_HAND: return "MU_CURSOR_HAND"; break;
							case MU_CURSOR_SIZE_LR: return "MU_CURSOR_SIZE_LR"; break;
							case MU_CURSOR_SIZE_TB: return "MU_CURSOR_SIZE_TB"; break;
							case MU_CURSOR_SIZE_TL_BR: return "MU_CURSOR_SIZE_TL_BR"; break;
							case MU_CURSOR_SIZE_TR_BL: return "MU_CURSOR_SIZE_TR_BL"; break;
							case MU_CURSOR_SIZE_ALL: return "MU_CURSOR_SIZE_ALL"; break;
							case MU_CURSOR_NO: return "MU_CURSOR_NO"; break;
						}
					}

					MUDEF const char* mu_cursor_style_get_nice_name(muCursorStyle style) {
						switch (style) {
							default: return "Unknown"; break;
							case MU_CURSOR_UNKNOWN: return "Unknown"; break;
							case MU_CURSOR_ARROW: return "Arrow"; break;
							case MU_CURSOR_IBEAM: return "I-Beam"; break;
							case MU_CURSOR_WAIT: return "Wait"; break;
							case MU_CURSOR_WAIT_ARROW: return "Wait (Arrow)"; break;
							case MU_CURSOR_CROSSHAIR: return "Crosshair"; break;
							case MU_CURSOR_HAND: return "Hand"; break;
							case MU_CURSOR_SIZE_LR: return "Resize (Left to right)"; break;
							case MU_CURSOR_SIZE_TB: return "Resize (Top to bottom)"; break;
							case MU_CURSOR_SIZE_TL_BR: return "Resize (Top-left to bottom-right)"; break;
							case MU_CURSOR_SIZE_TR_BL: return "Resize (Top-right to bottom-left)"; break;
							case MU_CURSOR_SIZE_ALL: return "Resize (All 4 directions)"; break;
							case MU_CURSOR_NO: return "No/Error"; break;
						}
					}

					MUDEF const char* mu_graphics_api_get_name(muGraphicsAPI api) {
						switch (api) {
							default: return "MU_UNKNOWN"; break;
							case MU_NULL_GRAPHICS_API: return "MU_NULL_GRAPHICS_API"; break;
							case MU_OPENGL_1_0: return "MU_OPENGL_1_0"; break;
							case MU_OPENGL_1_1: return "MU_OPENGL_1_1"; break;
							case MU_OPENGL_1_2: return "MU_OPENGL_1_2"; break;
							case MU_OPENGL_1_2_1: return "MU_OPENGL_1_2_1"; break;
							case MU_OPENGL_1_3: return "MU_OPENGL_1_3"; break;
							case MU_OPENGL_1_4: return "MU_OPENGL_1_4"; break;
							case MU_OPENGL_1_5: return "MU_OPENGL_1_5"; break;
							case MU_OPENGL_2_0: return "MU_OPENGL_2_0"; break;
							case MU_OPENGL_2_1: return "MU_OPENGL_2_1"; break;
							case MU_OPENGL_3_0: return "MU_OPENGL_3_0"; break;
							case MU_OPENGL_3_1: return "MU_OPENGL_3_1"; break;
							case MU_OPENGL_3_2_CORE: return "MU_OPENGL_3_2_CORE"; break;
							case MU_OPENGL_3_2_COMPATIBILITY: return "MU_OPENGL_3_2_COMPATIBILITY"; break;
							case MU_OPENGL_3_3_CORE: return "MU_OPENGL_3_3_CORE"; break;
							case MU_OPENGL_3_3_COMPATIBILITY: return "MU_OPENGL_3_3_COMPATIBILITY"; break;
							case MU_OPENGL_4_0_CORE: return "MU_OPENGL_4_0_CORE"; break;
							case MU_OPENGL_4_0_COMPATIBILITY: return "MU_OPENGL_4_0_COMPATIBILITY"; break;
							case MU_OPENGL_4_1_CORE: return "MU_OPENGL_4_1_CORE"; break;
							case MU_OPENGL_4_1_COMPATIBILITY: return "MU_OPENGL_4_1_COMPATIBILITY"; break;
							case MU_OPENGL_4_2_CORE: return "MU_OPENGL_4_2_CORE"; break;
							case MU_OPENGL_4_2_COMPATIBILITY: return "MU_OPENGL_4_2_COMPATIBILITY"; break;
							case MU_OPENGL_4_3_CORE: return "MU_OPENGL_4_3_CORE"; break;
							case MU_OPENGL_4_3_COMPATIBILITY: return "MU_OPENGL_4_3_COMPATIBILITY"; break;
							case MU_OPENGL_4_4_CORE: return "MU_OPENGL_4_4_CORE"; break;
							case MU_OPENGL_4_4_COMPATIBILITY: return "MU_OPENGL_4_4_COMPATIBILITY"; break;
							case MU_OPENGL_4_5_CORE: return "MU_OPENGL_4_5_CORE"; break;
							case MU_OPENGL_4_5_COMPATIBILITY: return "MU_OPENGL_4_5_COMPATIBILITY"; break;
							case MU_OPENGL_4_6_CORE: return "MU_OPENGL_4_6_CORE"; break;
							case MU_OPENGL_4_6_COMPATIBILITY: return "MU_OPENGL_4_6_COMPATIBILITY"; break;
						}
					}

					MUDEF const char* mu_graphics_api_get_nice_name(muGraphicsAPI api) {
						switch (api) {
							default: return "Unknown"; break;
							case MU_NULL_GRAPHICS_API: return "Unknown"; break;
							case MU_OPENGL_1_0: return "OpenGL 1.0"; break;
							case MU_OPENGL_1_1: return "OpenGL 1.1"; break;
							case MU_OPENGL_1_2: return "OpenGL 1.2"; break;
							case MU_OPENGL_1_2_1: return "OpenGL 1.2.1"; break;
							case MU_OPENGL_1_3: return "OpenGL 1.3"; break;
							case MU_OPENGL_1_4: return "OpenGL 1.4"; break;
							case MU_OPENGL_1_5: return "OpenGL 1.5"; break;
							case MU_OPENGL_2_0: return "OpenGL 2.0"; break;
							case MU_OPENGL_2_1: return "OpenGL 2.1"; break;
							case MU_OPENGL_3_0: return "OpenGL 3.0"; break;
							case MU_OPENGL_3_1: return "OpenGL 3.1"; break;
							case MU_OPENGL_3_2_CORE: return "OpenGL 3.2 (Core Profile)"; break;
							case MU_OPENGL_3_2_COMPATIBILITY: return "OpenGL 3.2 (Compatibility Profile)"; break;
							case MU_OPENGL_3_3_CORE: return "OpenGL 3.3 (Core Profile)"; break;
							case MU_OPENGL_3_3_COMPATIBILITY: return "OpenGL 3.3 (Compatibility Profile)"; break;
							case MU_OPENGL_4_0_CORE: return "OpenGL 4.0 (Core Profile)"; break;
							case MU_OPENGL_4_0_COMPATIBILITY: return "OpenGL 4.0 (Compatibility Profile)"; break;
							case MU_OPENGL_4_1_CORE: return "OpenGL 4.1 (Core Profile)"; break;
							case MU_OPENGL_4_1_COMPATIBILITY: return "OpenGL 4.1 (Compatibility Profile)"; break;
							case MU_OPENGL_4_2_CORE: return "OpenGL 4.2 (Core Profile)"; break;
							case MU_OPENGL_4_2_COMPATIBILITY: return "OpenGL 4.2 (Compatibility Profile)"; break;
							case MU_OPENGL_4_3_CORE: return "OpenGL 4.3 (Core Profile)"; break;
							case MU_OPENGL_4_3_COMPATIBILITY: return "OpenGL 4.3 (Compatibility Profile)"; break;
							case MU_OPENGL_4_4_CORE: return "OpenGL 4.4 (Core Profile)"; break;
							case MU_OPENGL_4_4_COMPATIBILITY: return "OpenGL 4.4 (Compatibility Profile)"; break;
							case MU_OPENGL_4_5_CORE: return "OpenGL 4.5 (Core Profile)"; break;
							case MU_OPENGL_4_5_COMPATIBILITY: return "OpenGL 4.5 (Compatibility Profile)"; break;
							case MU_OPENGL_4_6_CORE: return "OpenGL 4.6 (Core Profile)"; break;
							case MU_OPENGL_4_6_COMPATIBILITY: return "OpenGL 4.6 (Compatibility Profile)"; break;
						}
					}
				)

				MU_CPP_EXTERN_END

			#endif /* MUCOSA_IMPLEMENTATION */
		#endif

		MU_CPP_EXTERN_START

		/* mug context */

			// Global context
			mugContext* mug_global_context = 0;

			// Create context
			MUDEF void mug_context_create(mugContext* context, muWindowSystem system, muBool set_context) {
				// Create muCOSA context
				muCOSA_context_create(&context->cosa, system, set_context);
				if (context->cosa.result != MUCOSA_SUCCESS) {
					context->result = muCOSA_to_mug_result(context->cosa.result);
					if (muCOSA_result_is_fatal(context->cosa.result)) {
						return;
					}
				} else {
					context->result = MUG_SUCCESS;
				}

				// Set context
				if (set_context) {
					mug_context_set(context);
				}
			}

			// Destroy context
			MUDEF void mug_context_destroy(mugContext* context) {
				// Destroy muCOSA context
				muCOSA_context_destroy(&context->cosa);
			}

			// Set global context
			MUDEF void mug_context_set(mugContext* context) {
				mug_global_context = context;
			}

		/* Inner graphic logic */

			// This section handles all API-specific functionality
			// so that rendering sections are coded in a completely
			// API-less manner.

			// Union representing a graphic's parent
			union mug_GraphicParent {
				// muCOSA window
				muWindow cosa_win;
			};
			typedef union mug_GraphicParent mug_GraphicParent;

			// Macro used to identify graphic parent type
			typedef uint16_m mug_GraphicParentType;
			#define MUG_GRAPHIC_PARENT_MUWIN 1 // (muWindow)

			// Union representing a graphic's GL implementation
			#ifdef MU_SUPPORT_OPENGL
				union mug_GraphicGL {
					// muCOSA OpenGL context
					muGLContext cosa_gl;
				};
				typedef union mug_GraphicGL mug_GraphicGL;
			#endif

			// Struct representing a graphic
			struct mug_Graphic {
				// The context used by the graphic
				mugContext* context;
				// Graphic system
				muGraphicSystem system;
				// Inner-API data
				void* p;
				// Parent type
				mug_GraphicParentType parent_type;
				// Parent
				mug_GraphicParent parent;

				// OpenGL context
				#ifdef MU_SUPPORT_OPENGL
					mug_GraphicGL gl;
				#endif

				// The time at which the last frame completed
				double last_time;

				// Latest dimensions retrieved
				uint32_m dim[2];
			};
			typedef struct mug_Graphic mug_Graphic;

			// Create graphic via window
			mugResult mugGraphic_window_create(mugContext* context, muWindowInfo* info, mug_Graphic* gfx, muGraphicSystem system) {
				// Create window
				muCOSAResult cosa_res = MUCOSA_SUCCESS;
				gfx->parent_type = MUG_GRAPHIC_PARENT_MUWIN;
				gfx->parent.cosa_win = muCOSA_window_create(&context->cosa, &cosa_res, info);
				if (muCOSA_result_is_fatal(cosa_res)) {
					return muCOSA_to_mug_result(cosa_res);
				}

				// Set generic parameters
				gfx->context = context;
				gfx->system = system;
				gfx->last_time = muCOSA_time_get(&context->cosa);
				gfx->dim[0] = info->width;
				gfx->dim[1] = info->height;

				return muCOSA_to_mug_result(cosa_res);
			}

			// Destroys graphic based on parent
			// Note: does not free internally stored contents
			void mugGraphic_destroy(mug_Graphic* gfx) {
				// Find out parent type
				switch (gfx->parent_type) {
					// muWindow
					case MUG_GRAPHIC_PARENT_MUWIN: {
						muCOSA_window_destroy(&gfx->context->cosa, gfx->parent.cosa_win);
					} break;
				}
			}

			// Retrieves the dimensions of a graphic; stored in gfx->dim
			void mugGraphic_dim(mug_Graphic* gfx) {
				// Retrieve dimensions based on parent type
				switch (gfx->parent_type) {
					case MUG_GRAPHIC_PARENT_MUWIN: {
						muCOSA_window_get(&gfx->context->cosa, 0, gfx->parent.cosa_win, MU_WINDOW_DIMENSIONS, gfx->dim);
					} break;
				}
			}

			// Returns whether or not the graphic still exists
			muBool mugGraphic_exists(mug_Graphic* gfx) {
				// Find out parent type
				switch (gfx->parent_type) {
					default: return MU_FALSE; break;

					// muWindow
					case MUG_GRAPHIC_PARENT_MUWIN: {
						return !muCOSA_window_get_closed(&gfx->context->cosa, 0, gfx->parent.cosa_win);
					} break;
				}
			}

			// Updates the graphic's parent
			mugResult mugGraphic_update(mug_Graphic* gfx) {
				// Find out parent type
				switch (gfx->parent_type) {
					default: return 0; break;

					case MUG_GRAPHIC_PARENT_MUWIN: {
						// Update window
						mugResult res = MUG_SUCCESS;
						muCOSA_window_update(&gfx->context->cosa, &res, gfx->parent.cosa_win);
						return res;
					} break;
				}
			}

			// Handles OpenGL binding
			#ifdef MU_SUPPORT_OPENGL

				// Creates OpenGL context
				mugResult mugGraphicGL_create(mug_Graphic* gfx) {
					// Find out parent type
					switch (gfx->parent_type) {
						// ??
						default: return 1; break;

						// muWindow
						case MUG_GRAPHIC_PARENT_MUWIN: {
							// Create OpenGL context
							muCOSAResult cosa_res = MUCOSA_SUCCESS;
							gfx->gl.cosa_gl = muCOSA_gl_context_create(
								&gfx->context->cosa, &cosa_res, gfx->parent.cosa_win, MU_OPENGL_3_3_CORE
							);

							// Return result
							return muCOSA_to_mug_result(cosa_res);
						} break;
					}
				}

				// Destroys OpenGL context
				void mugGraphicGL_destroy(mug_Graphic* gfx) {
					// Find out parent type
					switch (gfx->parent_type) {
						// muWindow
						case MUG_GRAPHIC_PARENT_MUWIN: {
							muCOSA_gl_context_destroy(&gfx->context->cosa, gfx->parent.cosa_win, gfx->gl.cosa_gl);
						} break;
					}
				}

				// Binds OpenGL context
				void mugGraphicGL_bind(mug_Graphic* gfx) {
					// Find out parent type
					switch (gfx->parent_type) {
						// muWindow
						case MUG_GRAPHIC_PARENT_MUWIN: {
							muCOSA_gl_bind(&gfx->context->cosa, 0, gfx->parent.cosa_win, gfx->gl.cosa_gl);
						} break;
					}
				}

				// Swap buffers of OpenGL context
				mugResult mugGraphicGL_swap_buffers(mug_Graphic* gfx) {
					// Find out parent type
					switch (gfx->parent_type) {
						default: return 0; break;

						// muWindow
						case MUG_GRAPHIC_PARENT_MUWIN: {
							// Swap buffers
							mugResult res = MUG_SUCCESS;
							muCOSA_gl_swap_buffers(&gfx->context->cosa, &res, gfx->parent.cosa_win);
							return res;
						} break;
					}
				}

			#endif /* MU_SUPPORT_OPENGL */

		/* Useful functions */

			/* Math */

				// Rotates a given point (rpx, rpy) around a center (cpx, cpy)
				// Uses srot and crot as sin and cos of rotation
				// Fills in result in pos[0] (x) and pos[1] (y)
				void mugMath_rot_point_point(
					float rpx, float rpy, float cpx, float cpy,
					float srot, float crot,
					float* pos
				) {
					float ox = rpx-cpx, oy = rpy-cpy;
					pos[0] = (ox*crot - oy*srot) + cpx;
					pos[1] = (ox*srot + oy*crot) + cpy;
				}

		/* OpenGL */

		#ifdef MU_SUPPORT_OPENGL

			/* Macros */

				// ...

			/* General shader logic */

				// Struct for a shader
				struct mugGL_Shader {
					// Shader program
					GLuint program;
				};
				typedef struct mugGL_Shader mugGL_Shader;

				// Creates a vertex/fragment shader
				mugResult mugGL_shader_create_vf(mug_Graphic* gfx, mugGL_Shader* shader, const char* vsm, const char* fsm) {
					GLint success;

					// Compile vertex shader
					GLuint vs = glCreateShader(GL_VERTEX_SHADER);
					glShaderSource(vs, 1, &vsm, 0);
					glCompileShader(vs);
					// Make sure it succeeded
					glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
					if (!success) {
						return MUG_GL_FAILED_COMPILE_VERTEX_SHADER;
					}

					// Compile fragment shader
					GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
					glShaderSource(fs, 1, &fsm, 0);
					glCompileShader(fs);
					// Make sure it succeeded
					glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
					if (!success) {
						glDeleteShader(vs);
						return MUG_GL_FAILED_COMPILE_FRAGMENT_SHADER;
					}

					// Create and link shader program
					shader->program = glCreateProgram();
					glAttachShader(shader->program, vs);
					glAttachShader(shader->program, fs);
					glLinkProgram(shader->program);
					// Make sure it succeeded
					glGetProgramiv(shader->program, GL_LINK_STATUS, &success);
					if (!success) {
						glDeleteShader(fs);
						glDeleteShader(vs);
						return MUG_GL_FAILED_LINK_SHADERS;
					}

					// Delete shaders
					glDeleteShader(fs);
					glDeleteShader(vs);

					glUseProgram(shader->program);
					// Set default add/mul pos/col uniform values
					glUniform3f(glGetUniformLocation(shader->program, "aP"), 0.f, 0.f, 0.f);
					glUniform3f(glGetUniformLocation(shader->program, "mP"), 1.f, 1.f, 1.f);
					glUniform4f(glGetUniformLocation(shader->program, "aC"), 0.f, 0.f, 0.f, 0.f);
					glUniform4f(glGetUniformLocation(shader->program, "mC"), 1.f, 1.f, 1.f, 1.f);

					// Set dimensions uniform
					glUniform2f(glGetUniformLocation(shader->program, "d"), ((float)gfx->dim[0])/2.f, ((float)gfx->dim[1])/2.f);
					glUseProgram(0);

					return MUG_SUCCESS;
				}

				// Destroys a shader (safe to call if null)
				void mugGL_shader_destroy(mugGL_Shader* shader) {
					// If program exists:
					if (shader->program) {
						// Destroy program and set to 0
						glDeleteProgram(shader->program);
						shader->program = 0;
					}
				}

				// Binds a shader
				void mugGL_shader_bind(mugGL_Shader* shader) {
					// Bind shader program
					glUseProgram(shader->program);
				}

				// Unbinds a shader
				void mugGL_shader_unbind(mugGL_Shader* shader) {
					// Unbind shader program
					glUseProgram(shader->program);
				}

			/* General texture logic */

				/* Conversions */

					// mugTextureType -> GLenum
					GLenum mugGL_texture_type(mugTextureType type) {
						switch (type) {
							default: return GL_TEXTURE_2D; break;
							case MUG_TEXTURE_2D: return GL_TEXTURE_2D; break;
							case MUG_TEXTURE_2D_ARRAY: return GL_TEXTURE_2D_ARRAY; break;
						}
					}

					// mugTextureWrapping -> GLint
					GLint mugGL_texture_wrapping(mugTextureWrapping wrap) {
						switch (wrap) {
							default: return GL_REPEAT; break;
							case MUG_TEXTURE_REPEAT: return GL_REPEAT; break;
							case MUG_TEXTURE_MIRRORED_REPEAT: return GL_MIRRORED_REPEAT; break;
							case MUG_TEXTURE_CLAMP: return GL_CLAMP_TO_EDGE; break;
						}
					}

					// mugTextureFiltering -> GLint
					GLint mugGL_texture_filtering(mugTextureFiltering fil) {
						switch (fil) {
							default: return GL_NEAREST; break;
							case MUG_TEXTURE_NEAREST: return GL_NEAREST; break;
							case MUG_TEXTURE_BILINEAR: return GL_LINEAR; break;
						}
					}

					// mugTextureFormat -> GLenum format
					GLenum mugGL_texture_format_format(mugTextureFormat format) {
						switch (format) {
							default: return GL_RGB; break;
							case MUG_TEXTURE_U8_R: return GL_RED; break;
							case MUG_TEXTURE_U8_RGB: return GL_RGB; break;
							case MUG_TEXTURE_U8_RGBA: return GL_RGBA; break;
						}
					}

					// mugTextureformat -> GLenum type
					GLenum mugGL_texture_format_type(mugTextureFormat format) {
						switch (format) {
							default: return GL_UNSIGNED_BYTE; break;
							// U8
							case MUG_TEXTURE_U8_R: case MUG_TEXTURE_U8_RGB: case MUG_TEXTURE_U8_RGBA:
								return GL_UNSIGNED_BYTE;
							break;
						}
					}

				// Struct for a texture
				struct mugGL_Texture {
					// Handle to the texture from glGenTextures
					GLuint handle;
					// The format type (like GL_TEXTURE_2D for example)
					GLenum target;
				};
				typedef struct mugGL_Texture mugGL_Texture;

				// Creates a texture
				// 2D: dim[2]
				// 2D-array: dim[3]
				mugGL_Texture* mugGL_texture_create(mugResult* result, mugTextureInfo* info, uint32_m* dim, muByte* data) {
					// Allocate the texture container
					mugGL_Texture* tex = (mugGL_Texture*)mu_malloc(sizeof(mugGL_Texture));
					if (!tex) {
						MU_SET_RESULT(result, MUG_FAILED_MALLOC)
						return 0;
					}

					// Generate texture
					glGenTextures(1, &tex->handle);
					if (!tex->handle) {
						MU_SET_RESULT(result, MUG_GL_FAILED_GENERATE_TEXTURE)
						mu_free(tex);
						return 0;
					}

					// Get equivalent target
					tex->target = mugGL_texture_type(info->type);
					// Bind texture
					glBindTexture(tex->target, tex->handle);

					// Wrapping
					glTexParameteri(tex->target, GL_TEXTURE_WRAP_S, mugGL_texture_wrapping(info->wrapping[0]));
					glTexParameteri(tex->target, GL_TEXTURE_WRAP_T, mugGL_texture_wrapping(info->wrapping[1]));
					// Filtering
					glTexParameteri(tex->target, GL_TEXTURE_MAG_FILTER, mugGL_texture_filtering(info->filtering[0]));
					glTexParameteri(tex->target, GL_TEXTURE_MIN_FILTER, mugGL_texture_filtering(info->filtering[1]));

					// Generate texture
					switch (info->type) {
						// 2D
						case MUG_TEXTURE_2D: {
							GLenum format = mugGL_texture_format_format(info->format);
							glTexImage2D(
								GL_TEXTURE_2D, 0,
								format, dim[0], dim[1], 0, format,
								mugGL_texture_format_type(info->format),
								(const void*)data
							);
						} break;
						// 2D array
						case MUG_TEXTURE_2D_ARRAY: {
							GLenum format = mugGL_texture_format_format(info->format);
							glTexImage3D(
								GL_TEXTURE_2D_ARRAY, 0,
								format, dim[0], dim[1], dim[2], 0, format,
								mugGL_texture_format_type(info->format),
								(const void*)data
							);
						} break;
					}

					// Don't generate mipmap for now
					// Wouldn't this cause issues with 2D texture arrays?
					// glGenerateMipmap(tex->target);

					return tex;
				}

				// Destroys a texture
				void* mugGL_texture_destroy(mugGL_Texture* tex) {
					// Destroy texture
					glDeleteTextures(1, &tex->handle);
					// Free container
					mu_free(tex);
					// Return null
					return 0;
				}

				// Binds a texture
				void mugGL_texture_bind(mugGL_Texture* tex) {
					glBindTexture(tex->target, tex->handle);
				}

			/* General buffer logic */

				// Struct for an object buffer
				typedef struct mugGL_ObjBuffer mugGL_ObjBuffer;
				struct mugGL_ObjBuffer {
					// Vertex array object
					GLuint vao;
					// Vertex buffer object
					GLuint vbo;
					// Index buffer object
					GLuint ebo;

					// Object type
					mugObjectType obj_type;
					// Amount of objects stored
					uint32_m obj_count;

					// Amount of bytes used for each object on vertexes
					uint32_m bv_per_obj;
					// Corresponding vertex buffer size (obj_count*bv_per_obj)
					uint32_m vbuf_size;

					// Amount of bytes used for each object on indexes
					uint32_m bi_per_obj;
					// Corresponding index buffer size (obj_count*bi_per_obj)
					uint32_m ibuf_size;
					// Stores if index data has been filled already to correct size
					// Set this to false to make fill commands regenerate index data;
					// used upon initialization of buffer and (possibly later imp.)
					// resizing. Index data is not considered in subfilling.
					muBool index_filled;

					// Optional handle to a texture
					mugGL_Texture* tex;

					// Function used to fill all vertex data
					void (*fill_vertexes)(GLfloat* v, void* obj, uint32_m c);
					// Function used to fill all index data
					// If this function is null, ebo is not generated
					void (*fill_indexes)(GLuint* i, uint32_m c);
					// Function used to describe data
					void (*desc)(void);
					// Function that renders with all relevant objects already binded
					void (*render)(mugGL_ObjBuffer*);
					// Same but subrenders
					void (*subrender)(uint32_m o, uint32_m c);
				};

				// Checks if a given buffer is the expected size
				muBool mugGL_buffer_size_check(int buftype, uint32_m size) {
					// Get size
					GLint realsize = 0;
					glGetBufferParameteriv(buftype, GL_BUFFER_SIZE, &realsize);

					// Return if buffer size given is valid
					return realsize == (GLint)(size);
				}

				// Fills a buffer; obj can be null to just size buffer
				mugResult mugGL_objects_fill(mugGL_ObjBuffer* buf, void* obj) {
					// Bind VAO
					glBindVertexArray(buf->vao);

					// Vertexes
					if (obj)
					{
						// Allocate vertexes
						GLfloat* vertexes = (GLfloat*)mu_malloc(buf->vbuf_size);
						if (!vertexes) {
							glBindVertexArray(0);
							return MUG_FAILED_MALLOC;
						}

						// Fill vertex data
						buf->fill_vertexes(vertexes, obj, buf->obj_count);

						// Send data to GPU
						glBindBuffer(GL_ARRAY_BUFFER, buf->vbo);
						glBufferData(GL_ARRAY_BUFFER, buf->vbuf_size, vertexes, GL_DYNAMIC_DRAW);

						// Free vertex data
						mu_free(vertexes);

						// Ensure buffer is expected size
						if (!mugGL_buffer_size_check(GL_ARRAY_BUFFER, buf->vbuf_size)) {
							glBindBuffer(GL_ARRAY_BUFFER, 0);
							glBindVertexArray(0);
							return MUG_GL_FAILED_ALLOCATE_BUFFER;
						}
					}
					// No vertexes
					else {
						glBindBuffer(GL_ARRAY_BUFFER, buf->vbo);
						glBufferData(GL_ARRAY_BUFFER, buf->vbuf_size, 0, GL_DYNAMIC_DRAW);
					}

					// Indexes
					if (buf->fill_indexes && !buf->index_filled)
					{
						// Allocate indexes
						GLuint* indexes = (GLuint*)mu_malloc(buf->ibuf_size);
						if (!indexes) {
							glBindBuffer(GL_ARRAY_BUFFER, 0);
							glBindVertexArray(0);
							return MUG_FAILED_MALLOC;
						}

						// Fill index data
						buf->fill_indexes(indexes, buf->obj_count);

						// Send data to GPU
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buf->ebo);
						glBufferData(GL_ELEMENT_ARRAY_BUFFER, buf->ibuf_size, indexes, GL_STATIC_DRAW);

						// Free index data
						mu_free(indexes);

						// Ensure buffer is expected size
						if (!mugGL_buffer_size_check(GL_ELEMENT_ARRAY_BUFFER, buf->ibuf_size)) {
							glBindBuffer(GL_ARRAY_BUFFER, 0);
							glBindVertexArray(0);
							return MUG_GL_FAILED_ALLOCATE_BUFFER;
						}

						// Set index as filled
						buf->index_filled = MU_TRUE;
					}

					// Describe data
					buf->desc();
					// Unbind and return success
					glBindBuffer(GL_ARRAY_BUFFER, 0);
					// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
					glBindVertexArray(0);
					return MUG_SUCCESS;
				}

				// Fills a portion of a buffer; obj cannot be null
				mugResult mugGL_objects_subfill(mugGL_ObjBuffer* buf, uint32_m obj_offset, uint32_m obj_count, void* obj) {
					// Bind VAO
					glBindVertexArray(buf->vao);

					// Vertex data
					{
						// Byte amount calcluations
						uint32_m data_size = obj_count*buf->bv_per_obj;
						uint32_m data_offset = obj_offset*buf->bv_per_obj;

						// Allocate vertexes
						GLfloat* vertexes = (GLfloat*)mu_malloc(data_size);
						if (!vertexes) {
							glBindVertexArray(0);
							return MUG_FAILED_MALLOC;
						}

						// Fill vertex data
						buf->fill_vertexes(vertexes, obj, obj_count);

						// Send data to GPU
						glBindBuffer(GL_ARRAY_BUFFER, buf->vbo);
						glBufferSubData(GL_ARRAY_BUFFER, data_offset, data_size, vertexes);

						// Free vertex data
						mu_free(vertexes);
					}

					// Describe data
					buf->desc();
					// Unbind and return success
					glBindBuffer(GL_ARRAY_BUFFER, 0);
					glBindVertexArray(0);
					return MUG_SUCCESS;
				}

				// Destroys a given buffer
				void mugGL_objects_destroy(mugGL_ObjBuffer* buf) {
					// Destroy VAO, VBO, and (possibly) EBO
					glDeleteVertexArrays(1, &buf->vao);
					glDeleteBuffers(1, &buf->vbo);
					if (buf->fill_indexes) {
						glDeleteBuffers(1, &buf->ebo);
					}
				}

				// Creates a given buffer
				// All fields besides OpenGL objects should be set within buf by now
				mugResult mugGL_objects_create(mugGL_ObjBuffer* buf, void* obj) {
					// Generate VBO
					glGenBuffers(1, &buf->vbo);
					if (!buf->vbo) {
						return MUG_GL_FAILED_CREATE_BUFFER;
					}
					// Generate EBO if necessary
					if (buf->fill_indexes) {
						glGenBuffers(1, &buf->ebo);
						if (!buf->ebo) {
							glDeleteBuffers(1, &buf->vbo);
							return MUG_GL_FAILED_CREATE_BUFFER;
						}
						// + Mark indexes to be filled
						buf->index_filled = MU_FALSE;
					}
					// Generate VAO
					glGenVertexArrays(1, &buf->vao);
					if (!buf->vao) {
						glDeleteBuffers(1, &buf->vbo);
						glDeleteBuffers(1, &buf->ebo);
						return MUG_GL_FAILED_CREATE_VERTEX_ARRAY;
					}

					// Fill buffer
					mugResult res = mugGL_objects_fill(buf, obj);
					if (mug_result_is_fatal(res)) {
						mugGL_objects_destroy(buf);
						return res;
					}

					return MUG_SUCCESS;
				}

				// Resizes a given buffer
				// Buffer's contents are unknown after this if data is NULL
				mugResult mugGL_objects_resize(mugGL_ObjBuffer* buf, uint32_m obj_count, void* obj) {
					// Set count + other related variables
					uint32_m prev_count = buf->obj_count;
					buf->obj_count = obj_count;
					buf->vbuf_size = obj_count*buf->bv_per_obj;
					buf->ibuf_size = obj_count*buf->bi_per_obj;
					buf->index_filled = MU_FALSE;

					// Perform a fill
					mugResult res = mugGL_objects_fill(buf, obj);
					// If fatally failed, set things back to what they were
					if (mug_result_is_fatal(res)) {
						buf->obj_count = prev_count;
						buf->vbuf_size = prev_count*buf->bv_per_obj;
					}

					return res;
				}

				// Renders the object buffer
				void mugGL_objects_render(mugGL_ObjBuffer* buf) {
					// Bind texture if needed
					if (buf->tex) {
						mugGL_texture_bind(buf->tex);
					}
					// Bind VAO
					glBindVertexArray(buf->vao);
					// Call render function
					buf->render(buf);
					// Unbind VAO
					glBindVertexArray(0);
				}

				// Subrenders the object buffer
				void mugGL_objects_subrender(mugGL_ObjBuffer* buf, uint32_m obj_offset, uint32_m obj_count) {
					// Bind texture if needed
					if (buf->tex) {
						mugGL_texture_bind(buf->tex);
					}
					// Bind VAO
					glBindVertexArray(buf->vao);
					// Call subrender function
					buf->subrender(obj_offset, obj_count);
					// Unbind VAO
					glBindVertexArray(0);
				}

				// Sets the given object buffer's texture
				void mugGL_objects_texture(mugGL_ObjBuffer* buf, mugGL_Texture* tex) {
					buf->tex = tex;
				}

			/* Objects */

				/* Points */

					// Data format: { vec3 pos, vec4 col }
					// Rendered as GL_POINTS

					/* Shaders */

						// Vertex shader
						// * Lines, triangles, and rects also use this shader.
						const char* mugGL_pointVS = 
							// Version
							"#version 330 core\n"

							// Input { vec3 pos, vec4 col }
							"layout(location=0)in vec3 vPos;"
							"layout(location=1)in vec4 vCol;"

							// Output { vec4 col }
							"out vec4 fCol;"

							// Dimensions of graphic divided by 2
							"uniform vec2 d;"
							// Modifiers
							"uniform vec3 aP;" // addPos
							"uniform vec3 mP;" // mulPos

							// Main
							"void main(){"
								// Set position
								"gl_Position=vec4("
									// X
									"(((vPos.x*mP.x)+aP.x)-(d.x))/d.x,"
									// Y
									"-(((vPos.y*mP.y)+aP.y)-(d.y))/d.y,"
									// Z
									"((vPos.z*mP.z)+aP.z),"
									// W
									"1.0"
								");"
								// Transfer color to fragment
								"fCol=vCol;"
							"}"
						;

						// Fragment shader
						// * Lines, triangles, and rects also use this shader.
						const char* mugGL_pointFS =
							// Version
							"#version 330 core\n"

							// Input { vec4 col }
							"in vec4 fCol;"
							// Output { vec4 col }
							"out vec4 oCol;"

							// Modifiers
							"uniform vec4 aC;" // addCol
							"uniform vec4 mC;" // mulCol

							// Main
							"void main(){"
								// Set color
								"oCol=(fCol*mC)+aC;"
							"}"
						;

						// Creates program for points
						// * Lines, triangles, & rects also use this function.
						void mugGL_points_shader_load(mug_Graphic* gfx, mugGL_Shader* shader, mugResult* result) {
							// Exit if shader already compiled
							if (shader->program) {
								return;
							}

							// Compile shader
							mugResult res = mugGL_shader_create_vf(gfx, shader, mugGL_pointVS, mugGL_pointFS);
							if (res != MUG_SUCCESS) {
								MU_SET_RESULT(result, res)
							}
						}

					/* Buffer */

						// Fills vertex data
						void mugGL_points_fill_vertexes(GLfloat* v, void* obj, uint32_m c) {
							// Convert obj to struct array
							mugPoint* points = (mugPoint*)obj;

							// Loop through each point
							for (uint32_m i = 0; i < c; ++i) {
								// vec3 pos
								mu_memcpy(v, points->pos, 12);
								v += 3;
								// vec4 col
								mu_memcpy(v, points++->col, 16);
								v += 4;
							}
						}

						// Describes point data
						// * Lines, triangles, and rects also use this function.
						void mugGL_points_desc(void) {
							// vec3 pos
							glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 28, 0);
							glEnableVertexAttribArray(0);
							// vec4 col
							glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 28, (void*)(12));
							glEnableVertexAttribArray(1);
						}

						// Renders points
						void mugGL_points_render(mugGL_ObjBuffer* buf) {
							// Draw arrays
							glDrawArrays(
								GL_POINTS, // Render using points
								0, // Starting index (just 0)
								buf->obj_count // Index count (equal to amount of points)
							);
						}

						// Subrenders points
						void mugGL_points_subrender(uint32_m o, uint32_m c) {
							// Draw arrays
							glDrawArrays(
								GL_POINTS, // Render using points
								o, // Starting index (equal to amount of points)
								c // Index count (equal to amount of points)
							);
						}

						// Fills buffer with needed information
						void mugGL_points_fill(mugGL_ObjBuffer* buf) {
							buf->obj_type = MUG_OBJECT_POINT;

							// Amount of bytes used on vertex per object:
							// one vertex = vec3+vec4 (28)
							// one point = one vertex (28)
							buf->bv_per_obj = 28;

							// Function equivalents
							buf->fill_vertexes = mugGL_points_fill_vertexes;
							buf->fill_indexes = 0;
							buf->desc = mugGL_points_desc;
							buf->render = mugGL_points_render;
							buf->subrender = mugGL_points_subrender;
						}

				/* Lines */

					// Data format: { vec3 pos, vec4 col }
					// Rendered as GL_LINES

					/* Shaders */

						// Vertex shader (same as points)
						#define mugGL_lineVS mugGL_pointVS

						// Fragment shader (same as points)
						#define mugGL_lineFS mugGL_pointFS

						// Creates program for lines (same as points)
						#define mugGL_lines_shader_load mugGL_points_shader_load

					/* Buffer */

						// Fills vertex data
						void mugGL_lines_fill_vertexes(GLfloat* v, void* obj, uint32_m c) {
							// Convert obj to struct array
							mugLine* lines = (mugLine*)obj;

							// Loop through each point
							for (uint32_m i = 0; i < c; ++i) {
								// Point 1
								// - vec3 pos
								mu_memcpy(v, lines->points[0].pos, 12);
								v += 3;
								// - vec4 col
								mu_memcpy(v, lines->points[0].col, 16);
								v += 4;
								// Point 2
								// - vec3 pos
								mu_memcpy(v, lines->points[1].pos, 12);
								v += 3;
								// - vec4 col
								mu_memcpy(v, lines++->points[1].col, 16);
								v += 4;
							}
						}

						// Describes line data (same as points)
						#define mugGL_lines_desc mugGL_points_desc

						// Renders lines
						void mugGL_lines_render(mugGL_ObjBuffer* buf) {
							// Draw arrays
							glDrawArrays(
								GL_LINES, // Render using lines
								0, // Starting index (just 0)
								buf->obj_count*2 // Index count (two points per line)
							);
						}

						// Subrenders lines
						void mugGL_lines_subrender(uint32_m o, uint32_m c) {
							// Draw arrays
							glDrawArrays(
								GL_LINES, // Render using lines
								o*2, // Starting index (two points per line)
								c*2 // Index count (two points per line)
							);
						}

						// Fills buffer with need info
						void mugGL_lines_fill(mugGL_ObjBuffer* buf) {
							buf->obj_type = MUG_OBJECT_LINE;

							// Amount of bytes used on vertex per object:
							// one vertex = vec3+vec4 (28)
							// one line = two vertexes (56)
							buf->bv_per_obj = 56;

							// Function equivalents
							buf->fill_vertexes = mugGL_lines_fill_vertexes;
							buf->fill_indexes = 0;
							buf->desc = mugGL_lines_desc;
							buf->render = mugGL_lines_render;
							buf->subrender = mugGL_lines_subrender;
						}

				/* Triangle */

					// Data format: { vec3 pos, vec4 col }
					// Rendered as GL_TRIANGLES

					/* Shaders */

						// Vertex shader (same as points)
						#define mugGL_triangleVS mugGL_pointVS

						// Fragment shader (same as points)
						#define mugGL_triangleFS mugGL_pointFS

						// Creates program for triangles (same as points)
						#define mugGL_triangles_shader_load mugGL_points_shader_load

					/* Buffer */

						// Fills vertex data
						void mugGL_triangles_fill_vertexes(GLfloat* v, void* obj, uint32_m c) {
							// Convert obj to struct array
							mugTriangle* triangles = (mugTriangle*)obj;

							// Loop through each triangle
							for (uint32_m t = 0; t < c; ++t) {
								// Point 0
								// - vec3 pos
								mu_memcpy(v, triangles->points[0].pos, 12);
								v += 3;
								// - vec4 col
								mu_memcpy(v, triangles->points[0].col, 16);
								v += 4;
								// Point 1
								// - vec3 pos
								mu_memcpy(v, triangles->points[1].pos, 12);
								v += 3;
								// - vec4 col
								mu_memcpy(v, triangles->points[1].col, 16);
								v += 4;
								// Point 2
								// - vec3 pos
								mu_memcpy(v, triangles->points[2].pos, 12);
								v += 3;
								// - vec4 col
								mu_memcpy(v, triangles++->points[2].col, 16);
								v += 4;
							}
						}

						// Describes triangle data (same as points)
						#define mugGL_triangles_desc mugGL_points_desc

						// Renders triangles
						void mugGL_triangles_render(mugGL_ObjBuffer* buf) {
							// Draw arrays
							glDrawArrays(
								GL_TRIANGLES, // Render using triangles
								0, // Starting index (just 0)
								buf->obj_count*3 // Index count (three points per triangle)
							);
						}

						// Subrenders triangles
						void mugGL_triangles_subrender(uint32_m o, uint32_m c) {
							// Draw arrays
							glDrawArrays(
								GL_TRIANGLES, // Render using triangles
								o*3, // Starting index (three points per triangle)
								c*3 // Index count (three points per triangle)
							);
						}

						// Fills buffer with needed info
						void mugGL_triangles_fill(mugGL_ObjBuffer* buf) {
							buf->obj_type = MUG_OBJECT_TRIANGLE;

							// Amount of bytes used on vertex per object:
							// one vertex = vec3+vec4 (28)
							// one triangle = three vertexes (84)
							buf->bv_per_obj = 84;

							// Function equivalents
							buf->fill_vertexes = mugGL_triangles_fill_vertexes;
							buf->fill_indexes = 0;
							buf->desc = mugGL_triangles_desc;
							buf->render = mugGL_triangles_render;
							buf->subrender = mugGL_triangles_subrender;
						}

				/* Rect */

					// Data format: { vec3 pos, vec4 col }
					// Indexed data: { 0, 1, 3, 1, 2, 3 }

					/* Shaders */

						// Vertex shader (same as points)
						#define mugGL_rectVS mugGL_pointVS

						// Fragment shader (same as points)
						#define mugGL_rectFS mugGL_pointFS

						// Creates program for rects (same as points)
						#define mugGL_rects_shader_load mugGL_points_shader_load

					/* Buffer */

						// Fills a particular vertex given what to multiply dimensions by
						// (Used to alternate between 4 corners)
						// Returns how much to increment v by
						static inline size_m mugGL_rects_fill_invertexes(
							// Rect data
							GLfloat* v, mugRect* rect,
							// Half-dim and vector dim
							float hdim[2], float vdim0, float vdim1,
							// Sin/Cos rotation
							float srot, float crot
						) {
							// vec3 pos
							// - x and y
							mugMath_rot_point_point(
								rect->center.pos[0]+(hdim[0]*vdim0), // Point x
								rect->center.pos[1]+(hdim[1]*vdim1), // Point y
								rect->center.pos[0], // Center x
								rect->center.pos[1], // Center y
								srot, crot, // Sin / Cos rotation
								v
							);
							// - z
							v[2] = rect->center.pos[2];

							// vec4 col
							mu_memcpy(&v[3], rect->center.col, 16);
							return 7;
						}

						// Fill vertex data
						void mugGL_rects_fill_vertexes(GLfloat* v, void* obj, uint32_m c) {
							// Convert obj to struct array
							mugRect* rects = (mugRect*)obj;

							// Loop through each rect
							for (uint32_m i = 0; i < c; ++i) {
								// Store half-dim
								float hdim[2] = { rects->dim[0]/2.f, rects->dim[1]/2.f };
								// Store sin/cos of rotation
								// Sin is negative because y-direction is flipped in mug coordinates
								float srot = -mu_sinf(rects->rot);
								float crot = mu_cosf(rects->rot);

								// Top-left
								v += mugGL_rects_fill_invertexes(v, rects  , hdim, -1.f, -1.f, srot, crot);
								// Bottom-left
								v += mugGL_rects_fill_invertexes(v, rects  , hdim, -1.f,  1.f, srot, crot);
								// Bottom-right
								v += mugGL_rects_fill_invertexes(v, rects  , hdim,  1.f,  1.f, srot, crot);
								// Top-right
								v += mugGL_rects_fill_invertexes(v, rects++, hdim,  1.f, -1.f, srot, crot);
							}
						}

						// Fill index data
						// Circles, squircles, round rects, textures, texture arrays also use this
						void mugGL_rects_fill_indexes(GLuint* i, uint32_m c) {
							// Offset for index pattern:
							uint32_m po = 0;

							// Loop through each rect
							for (uint32_m r = 0; r < c; ++r) {
								// Fill out index pattern:
								// { 0, 1, 3, 1, 2, 3 }
								*i++ = po;
								*i++ = po+1;
								*i++ = po+3;
								*i++ = po+1;
								*i++ = po+2;
								*i++ = po+3;
								// Increment po
								po += 4;
							}
						}

						// Describes rect data (same as points)
						#define mugGL_rects_desc mugGL_points_desc

						// Renders rects
						// Circles, squircles, round rects, textures, texture arrays also use this
						void mugGL_rects_render(mugGL_ObjBuffer* buf) {
							// Draw elements
							glDrawElements(
								GL_TRIANGLES, // Rendering triangle-by-triangle
								buf->obj_count*6, // Indexes per object
								GL_UNSIGNED_INT, // Index type (GLuint)
								(const void*)(0) // Offset in index data (in bytes)
							);
						}

						// Subrenders rects
						// Circles, squircles, round rects, textures, texture arrays also use this
						void mugGL_rects_subrender(uint32_m o, uint32_m c) {
							// Draw elements
							glDrawElements(
								GL_TRIANGLES, // Rendering triangle-by-triangle
								c*6, // Indexes per object
								GL_UNSIGNED_INT, // Index type (GLuint)
								(const void*)(((size_m)o)*24) // Offset in index data (in bytes; 24 = 6*4 = 6*sizeof(GLuint))
							);
						}

						// Fills buffer with needed info
						void mugGL_rects_fill(mugGL_ObjBuffer* buf) {
							buf->obj_type = MUG_OBJECT_RECT;

							// Amount of bytes used on vertex per object:
							// one vertex = vec3+vec4 (28)
							// one rect = four vertexes (112)
							buf->bv_per_obj = 112;

							// Amount of bytes used on index per object:
							// one rect = six indexes (24)
							buf->bi_per_obj = 24;

							// Function equivalents
							buf->fill_vertexes = mugGL_rects_fill_vertexes;
							buf->fill_indexes = mugGL_rects_fill_indexes;
							buf->desc = mugGL_rects_desc;
							buf->render = mugGL_rects_render;
							buf->subrender = mugGL_rects_subrender;
						}

				/* Circle */

					// Data format: { vec3 pos, vec4 col, vec2 cen, float rad }
					// Indexed data: { 0, 1, 3, 1, 2, 3 }

					/* Shaders */

						// Vertex shader
						const char* mugGL_circleVS = 
							// Version
							"#version 330 core\n"

							// Input (pos, col, cen, rad)
							"layout(location=0)in vec3 vPos;"
							"layout(location=1)in vec4 vCol;"
							"layout(location=2)in vec2 vCen;"
							"layout(location=3)in float vRad;"

							// Output (col, cen, float rad)
							"out vec4 fCol;"
							"out vec2 fCen;"
							"out float fRad;"

							// Dimensions of graphic divided by 2
							"uniform vec2 d;"
							// Modifiers
							"uniform vec3 aP;" // addPos
							"uniform vec3 mP;" // mulPos

							// Main
							"void main(){"
								// Set position
								"gl_Position=vec4("
									// X
									"(((vPos.x*mP.x)+aP.x)-(d.x))/d.x,"
									// Y
									"-(((vPos.y*mP.y)+aP.y)-(d.y))/d.y,"
									// Z
									"((vPos.z*mP.z)+aP.z),"
									// W
									"1.0"
								");"

								// Transfer data to fragment
								"fCol=vCol;"
								// (Flip y because fragcoord is bottom-left origin)
								"fCen=vec2((vCen.x*mP.x)+aP.x, (d.y*2.0)-((vCen.y*mP.y)+aP.y));"
								"fRad=vRad;"
							"}"
						;

						// Fragment shader
						const char* mugGL_circleFS = 
							// Version
							"#version 330 core\n"

							// Input (col, cen, rad)
							"in vec4 fCol;"
							"in vec2 fCen;"
							"in float fRad;"
							// Output (col)
							"out vec4 oCol;"

							// Modifiers
							"uniform vec4 aC;" // addCol
							"uniform vec4 mC;" // mulCol

							// Returns a smooth transition between 0.0 and 1.0
							// depending on if pixel (p) is outside of circle
							// (center c, radius r)
							// Uses this formula (lol):
							// \max\left(\min\left(-\left(\sqrt{\left(p.x-c.x\right)^{2}+\left(p.y-c.y\right)^{2}}-r\right)+0.5,1\right),0\right)
							"float dis(vec2 p,vec2 c,float r){"
								// Return inverse of:
								"return 1.0-"
									"max(min("
									// Negative if outside, positive if inside
									// Fades between edge of circle; automatic anti-aliasing >:)
									"(sqrt(pow(p.x-c.x,2.0)+pow(p.y-c.y,2.0))-r)+0.5,"
									// Maximum value of 1
									"1.0),"
									// Minimum value of 0
									"0.0);"
							"}"

							// Main
							"void main(){"
								// Calculate color
								"oCol=vec4(fCol.rgb, fCol.a*dis(gl_FragCoord.xy,fCen,fRad));"
								// Apply modifiers
								"oCol=(oCol*mC)+aC;"
							"}"
						;

						// Creates program for circles
						void mugGL_circles_shader_load(mug_Graphic* gfx, mugGL_Shader* shader, mugResult* result) {
							// Exit if shader already compiled
							if (shader->program) {
								return;
							}

							// Compile shader
							mugResult res = mugGL_shader_create_vf(gfx, shader, mugGL_circleVS, mugGL_circleFS);
							if (res != MUG_SUCCESS) {
								MU_SET_RESULT(result, res)
							}
						}

					/* Buffer */

						// Fills a particular vertex given what to multiply radius by
						// (Used to alternate between 4 corners)
						// Returns new v pointer after this vertex
						static inline GLfloat* mugGL_circles_fill_invertexes(
							// Circle data
							GLfloat* v, mugCircle* circle,
							// Radius data
							float vradx, float vrady
						) {
							// vec3 pos
							// - x and y
							*v++ = circle->center.pos[0]+(circle->radius*vradx);
							*v++ = circle->center.pos[1]+(circle->radius*vrady);
							// - z
							*v++ = circle->center.pos[2];

							// vec4 col
							mu_memcpy(v, circle->center.col, 16);
							// vec2 cen
							mu_memcpy(v+4, circle->center.pos, 8);
							// float rad
							v[6] = circle->radius;
							return v+7;
						}

						// Fills vertex data
						void mugGL_circles_fill_vertexes(GLfloat* v, void* obj, uint32_m c) {
							// Convert obj to struct array
							mugCircle* circles = (mugCircle*)obj;

							// Loop through each circle
							for (uint32_m i = 0; i < c; ++i) {
								// Top-left
								v = mugGL_circles_fill_invertexes(v, circles,  -1.f, -1.f);
								// Bottom-left
								v = mugGL_circles_fill_invertexes(v, circles,  -1.f,  1.f);
								// Bottom-right
								v = mugGL_circles_fill_invertexes(v, circles,   1.f,  1.f);
								// Top-right
								v = mugGL_circles_fill_invertexes(v, circles++, 1.f, -1.f);
							}
						}

						// Fill index data (same as rect)
						#define mugGL_circles_fill_indexes mugGL_rects_fill_indexes

						// Describes circle data
						void mugGL_circles_desc(void) {
							// vec3 pos
							glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 40, 0);
							glEnableVertexAttribArray(0);
							// vec4 col
							glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 40, (void*)(12));
							glEnableVertexAttribArray(1);
							// vec2 cen
							glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 40, (void*)(28));
							glEnableVertexAttribArray(2);
							// float rad
							glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 40, (void*)(36));
							glEnableVertexAttribArray(3);
						}

						// Renders circles (same as rect)
						#define mugGL_circles_render mugGL_rects_render
						// Subrenders circles (same as rect)
						#define mugGL_circles_subrender mugGL_rects_subrender

						// Fills buffer with needed info
						void mugGL_circles_fill(mugGL_ObjBuffer* buf) {
							buf->obj_type = MUG_OBJECT_CIRCLE;

							// Amount of bytes used on vertex per object:
							// one vertex = vec3+vec4+vec2+float (40)
							// one circle = four vertexes (160)
							buf->bv_per_obj = 160;

							// Amount of bytes used on index per object:
							// one circle = six indexes (24)
							buf->bi_per_obj = 24;

							// Function equivalents
							buf->fill_vertexes = mugGL_circles_fill_vertexes;
							buf->fill_indexes = mugGL_circles_fill_indexes;
							buf->desc = mugGL_circles_desc;
							buf->render = mugGL_circles_render;
							buf->subrender = mugGL_circles_subrender;
						}

				/* Squircle */

					// Data format: { vec3 pos, vec4 col, float rot, vec2 cen, float rad, float exp }
					// Indexed data: { 0, 1, 3, 1, 2, 3 }

					/* Shaders */

						// Vertex shader
						const char* mugGL_squircleVS = 
							// Version
							"#version 330 core\n"

							// Input (pos, col, rot, cen, rad, exp)
							"layout(location=0)in vec3 vPos;"
							"layout(location=1)in vec4 vCol;"
							"layout(location=2)in float vRot;"
							"layout(location=3)in vec2 vCen;"
							"layout(location=4)in float vRad;"
							"layout(location=5)in float vExp;"

							// Output (rot, col, cen, rad, exp)
							"out float fRot;"
							"out vec4 fCol;"
							"out vec2 fCen;"
							"out float fRad;"
							"out float fExp;"

							// Dimensions of graphic divided by 2
							"uniform vec2 d;"
							// Modifiers
							"uniform vec3 aP;" // addPos
							"uniform vec3 mP;" // mulPos

							// Main
							"void main(){"
								// Set position
								"gl_Position=vec4("
									// X
									"(((vPos.x*mP.x)+aP.x)-(d.x))/d.x,"
									// Y
									"-(((vPos.y*mP.y)+aP.y)-(d.y))/d.y,"
									// Z
									"(vPos.z*mP.z)+aP.z,"
									// W
									"1.0"
								");"

								// Transfer data to fragment
								"fRot=vRot;"
								"fCol=vCol;"
								// (Flip y because fragcoord is bottom-left origin)
								"fCen=vec2((vCen.x*mP.x)+aP.x, (d.y*2.0)-((vCen.y*mP.y)+aP.y));"
								"fRad=vRad;"
								"fExp=vExp;"
							"}"
						;

						// Fragment shader
						const char* mugGL_squircleFS =
							// Version
							"#version 330 core\n"

							// Input (rot, col, cen, rad, exp)
							"in float fRot;"
							"in vec4 fCol;"
							"in vec2 fCen;"
							"in float fRad;"
							"in float fExp;"
							// Output (col)
							"out vec4 oCol;"

							// Modifiers
							"uniform vec4 aC;" // addCol
							"uniform vec4 mC;" // mulCol

							// Rotates point (p) around center (c)
							// Rotation is given by cos (cr) and sin (sr) of rotation
							"vec2 r(vec2 p,vec2 c,float cr,float sr){"
								"vec2 o=p-c;"
								// MATH
								"return vec2((o.x*cr-o.y*sr)+c.x,(o.x*sr+o.y*cr)+c.y);"
							"}"

							// Returns a smooth transition between 0.0 and 1.0
							// depending on if pixel (p) is outside of squircle
							// (center c, radius r, exponent e)
							// Uses this formula:
							// \max\left(\min\left(-\left(\left(\left|p.x-c.x\right|^{b}+\left|p.y-c.y\right|^{b}\right)^{\frac{1}{b}}-r\right)+0.5,1\right),0\right)
							// ^ (b is the exponent)
							"float s(vec2 p,vec2 c,float r,float e){"
								// Return inverse of:
								"return 1.0-"
									"max(min("
									// Negative if outside, positive if inside
									// Fades between edge of squircle; automatic anti-aliasing >:)
									"(pow(pow(abs(p.x-c.x),e)+pow(abs(p.y-c.y),e),1.0/e)-r)+0.5,"
									// Maximum value of 1
									"1.0),"
									// Maximum value of 0
									"0.0)"
								";"
							"}"

							// Main
							"void main(){"
								// Calculate color (rotating FragCoord to adjust for rotation)
								"oCol=vec4(fCol.rgb, fCol.a*s(r(gl_FragCoord.xy,fCen,sin(fRot),cos(fRot)),fCen,fRad,fExp));"
								// Apply modifiers
								"oCol=(oCol*mC)+aC;"
							"}"
						;

						// Creates program for squircles
						void mugGL_squircles_shader_load(mug_Graphic* gfx, mugGL_Shader* shader, mugResult* result) {
							// Exit if shader already compiled
							if (shader->program) {
								return;
							}

							// Compile shader
							mugResult res = mugGL_shader_create_vf(gfx, shader, mugGL_squircleVS, mugGL_squircleFS);
							if (res != MUG_SUCCESS) {
								MU_SET_RESULT(result, res)
							}
						}

					/* Buffer */

						// Fills a particular vertex given what to multiply radius by
						// (Used to alternate between 4 corners)
						// Returns new v pointer after this vertex
						static inline GLfloat* mugGL_squircles_fill_invertexes(
							// Squircle data
							GLfloat* v, mugSquircle* squircle,
							// Radius data
							float vradx, float vrady,
							// Rotation data
							float srot, float crot
						) {
							// vec3 pos
							// - x and y (rotated)
							mugMath_rot_point_point(
								squircle->center.pos[0]+(squircle->radius*vradx), // Point x
								squircle->center.pos[1]+(squircle->radius*vrady), // Point y
								squircle->center.pos[0], // Center x
								squircle->center.pos[1], // Center y
								srot, crot, // Sin / Cos rotation
								v
							);
							// - z
							v[2] = squircle->center.pos[2];
							v += 3;

							// vec4 col
							mu_memcpy(v, squircle->center.col, 16);
							// float rot
							v[4] = squircle->rot;
							// vec2 cen
							mu_memcpy(v+5, squircle->center.pos, 8);
							// float rad
							v[7] = squircle->radius;
							// float exp
							v[8] = squircle->exp;
							return v+9;
						}

						// Fills vertex data
						void mugGL_squircles_fill_vertexes(GLfloat* v, void* obj, uint32_m c) {
							// Convert obj to struct array
							mugSquircle* squircles = (mugSquircle*)obj;

							// Loop through each squircle
							for (uint32_m i = 0; i < c; ++i) {
								// Calculate rotation values
								// Sin is negative because y-direction is flipped in mug coordinates
								float srot = -mu_sinf(squircles->rot);
								float crot = mu_cosf(squircles->rot);

								// Top-left
								v = mugGL_squircles_fill_invertexes(v, squircles,  -1.f, -1.f, srot, crot);
								// Bottom-left
								v = mugGL_squircles_fill_invertexes(v, squircles,  -1.f,  1.f, srot, crot);
								// Bottom-right
								v = mugGL_squircles_fill_invertexes(v, squircles,   1.f,  1.f, srot, crot);
								// Top-right
								v = mugGL_squircles_fill_invertexes(v, squircles++, 1.f, -1.f, srot, crot);
							}
						}

						// Fill index data (same as rect)
						#define mugGL_squircles_fill_indexes mugGL_rects_fill_indexes

						// Describes squircle data
						void mugGL_squircles_desc(void) {
							// vec3 pos
							glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 48, 0);
							glEnableVertexAttribArray(0);
							// vec4 col
							glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 48, (void*)(12));
							glEnableVertexAttribArray(1);
							// float rot
							glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 48, (void*)(28));
							glEnableVertexAttribArray(2);
							// vec2 cen
							glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 48, (void*)(32));
							glEnableVertexAttribArray(3);
							// float rad
							glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 48, (void*)(40));
							glEnableVertexAttribArray(4);
							// float exp
							glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, 48, (void*)(44));
							glEnableVertexAttribArray(5);
						}

						// Renders squircles (same as rect)
						#define mugGL_squircles_render mugGL_rects_render
						// Subrenders squircles (same as rect)
						#define mugGL_squircles_subrender mugGL_rects_subrender

						// Fills buffer with needed info
						void mugGL_squircles_fill(mugGL_ObjBuffer* buf) {
							buf->obj_type = MUG_OBJECT_SQUIRCLE;

							// Amount of bytes used on vertex per object:
							// one vertex = vec3+vec4+float+vec2+float+float (48)
							// one squircle = four vertexes (192)
							buf->bv_per_obj = 192;

							// Amount of bytes used on index per object:
							// one squircle = six indexes (24)
							buf->bi_per_obj = 24;

							// Function equivalents
							buf->fill_vertexes = mugGL_squircles_fill_vertexes;
							buf->fill_indexes = mugGL_squircles_fill_indexes;
							buf->desc = mugGL_squircles_desc;
							buf->render = mugGL_squircles_render;
							buf->subrender = mugGL_squircles_subrender;
						}

				/* Round rect */

					// Data format: { vec3 pos, vec4 col, float rot, vec2 cen, vec2 dim, float rad }
					// * dim is actually half-dimensions
					// Indexed data: { 0, 1, 3, 1, 2, 3 }

					/* Shaders */

						// Vertex shader
						const char* mugGL_roundrectVS = 
							// Version
							"#version 330 core\n"

							// Input ( pos, col, rot, cen, dim, rad )
							"layout(location=0)in vec3 vPos;"
							"layout(location=1)in vec4 vCol;"
							"layout(location=2)in float vRot;"
							"layout(location=3)in vec2 vCen;"
							"layout(location=4)in vec2 vDim;"
							"layout(location=5)in float vRad;"

							// Output (col, rot, cen, dim, rad)
							"out vec4 fCol;"
							"out float fRot;"
							"out vec2 fCen;"
							"out vec2 fDim;"
							"out float fRad;"

							// Dimensions of graphic divided by 2
							"uniform vec2 d;"
							// Modifiers
							"uniform vec3 aP;" // addPos
							"uniform vec3 mP;" // mulPos

							// Main
							"void main(){"
								// Set position
								"gl_Position=vec4("
									// X
									"(((vPos.x*mP.x)+aP.x)-(d.x))/d.x,"
									// Y
									"-(((vPos.y*mP.y)+aP.y)-(d.y))/d.y,"
									// Z
									"(vPos.z*mP.z)+aP.z,"
									// W
									"1.0"
								");"

								// Transfer data to fragment
								"fCol=vCol;"
								"fRot=vRot;"
								// (Flip y because fragcoord is bottom-left origin)
								"fCen=vec2((vCen.x*mP.x)+aP.x,(d.y*2.0)-((vCen.y*mP.y)+aP.y));"
								"fDim=vDim;"
								"fRad=vRad;"
							"}"
						;

						// Fragment shader
						const char* mugGL_roundrectFS =
							// Version
							"#version 330 core\n"

							// Input (col, rot, cen, dim, rad)
							"in vec4 fCol;"
							"in float fRot;"
							"in vec2 fCen;"
							"in vec2 fDim;"
							"in float fRad;"
							// Output (col)
							"out vec4 oCol;"

							// Modifiers
							"uniform vec4 aC;" // addCol
							"uniform vec4 mC;" // mulCol

							// Rotates point (p) around center (c)
							// Rotation is given by cos (cr) and sin(sr) of rotation
							"vec2 r(vec2 p,vec2 c,float cr,float sr){"
								"vec2 o=p-c;"
								// MATH
								"return vec2((o.x*cr-o.y*sr)+c.x,(o.x*sr+o.y*cr)+c.y);"
							"}"

							// Main
							"void main(){"
								// Calculate pixel based on rotation of FragCoord
								"vec2 p=r(gl_FragCoord.xy,fCen,sin(fRot),cos(fRot));"
								// Calculate absolute distance from center
								"vec2 j=vec2(abs(p.x-fCen.x),abs(p.y-fCen.y));"
								// Calculate alpha multiplier:
								"float a="
									// If we're inside of rectangle's bounding box:
									// - \left|x-p.x\right|\le w_{0}\left\{\left|y-p.y\right|\le h_{0}-r\right\}
									// - \left|y-p.y\right|\le h_{0}\left\{\left|x-p.x\right|\le w_{0}-r\right\}
									"((j.x<=fDim.x && j.y<=fDim.y-fRad) || (j.y<=fDim.y && j.x<+fDim.x-fRad))?"
										// If these checks passed, we're in the rectangle; just set to 1.0
										"1.0:"
									// If it didn't succeed, we're in a circle corner
									// In this case, we use the magical equation to return a smooth transition
									// between in and out of the corner (automatic AA)
									// - \max\left(\min\left(-\left(\sqrt{\left|\left|j.x-p.x\right|-w_{0}+r\right|^{2}+\left|\left|j.y-p.y\right|-h_{0}+r\right|^{2}}-r\right)+0.5,1\right),0\right)
									"max(min(-(sqrt(pow(abs(j.x-fDim.x+fRad),2.0)+pow(abs(j.y-fDim.y+fRad),2.0))-fRad)+0.5,1.0),0.0)"
								";"
								// Calculate color
								"oCol=vec4(fCol.rgb,fCol.a*a);"
								// Apply modifiers
								"oCol=(oCol*mC)+aC;"
							"}"
						;

						// Creates program for round rects
						void mugGL_roundrects_shader_load(mug_Graphic* gfx, mugGL_Shader* shader, mugResult* result) {
							// Exit if shader already compiled
							if (shader->program) {
								return;
							}

							// Compile shader
							mugResult res = mugGL_shader_create_vf(gfx, shader, mugGL_roundrectVS, mugGL_roundrectFS);
							if (res != MUG_SUCCESS) {
								MU_SET_RESULT(result, res)
							}
						}

					/* Buffer */

						// Fills a particular vertex given what to multiply dimensions by
						// (Used to alternate between 4 corners)
						// Returns new v pointer after this vertex
						static inline GLfloat* mugGL_roundrects_fill_invertexes(
							// Round rect data
							GLfloat* v, mugRoundRect* roundrect,
							// Dimension data
							float hdim[2], float vdim0, float vdim1,
							// Rotation data
							float srot, float crot
						) {
							// vec3 pos
							// - x and y (rotated)
							mugMath_rot_point_point(
								roundrect->center.pos[0]+(hdim[0]*vdim0), // Point x
								roundrect->center.pos[1]+(hdim[1]*vdim1), // Point y
								roundrect->center.pos[0], // Center x
								roundrect->center.pos[1], // Center y
								srot, crot, // Sin / Cos rotation
								v
							);
							// - z
							v[2] = roundrect->center.pos[2];

							// vec4 col
							mu_memcpy(&v[3], roundrect->center.col, 16);
							// float rot
							v[7] = roundrect->rot;
							// vec2 cen
							mu_memcpy(&v[8], roundrect->center.pos, 8);
							// vec2 dim
							mu_memcpy(&v[10], hdim, 8);
							// float rad
							v[12] = roundrect->radius;

							return v+13;
						}

						// Fills vertex data
						void mugGL_roundrects_fill_vertexes(GLfloat* v, void* obj, uint32_m c) {
							// Convert obj to struct array
							mugRoundRect* roundrects = (mugRoundRect*)obj;

							// Loop through each round rect
							for (uint32_m i = 0; i < c; ++i) {
								// Calculate rotation values
								// Sin is negative because y-direction is flipped in mug coordinates
								float srot = -mu_sinf(roundrects->rot);
								float crot =  mu_cosf(roundrects->rot);
								// Calculate half-dimensions
								float hdim[2] = { roundrects->dim[0]/2.f, roundrects->dim[1]/2.f };

								// Top-left
								v = mugGL_roundrects_fill_invertexes(v, roundrects,   hdim, -1.f, -1.f, srot, crot);
								// Bottom-left
								v = mugGL_roundrects_fill_invertexes(v, roundrects,   hdim, -1.f,  1.f, srot, crot);
								// Bottom-right
								v = mugGL_roundrects_fill_invertexes(v, roundrects,   hdim,  1.f,  1.f, srot, crot);
								// Top-right
								v = mugGL_roundrects_fill_invertexes(v, roundrects++, hdim,  1.f, -1.f, srot, crot);
							}
						}

						// Fill index data (same as rect)
						#define mugGL_roundrects_fill_indexes mugGL_rects_fill_indexes

						// Describes round rect data
						void mugGL_roundrects_desc(void) {
							// vec3 pos
							glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 52, 0);
							glEnableVertexAttribArray(0);
							// vec4 col
							glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 52, (void*)(12));
							glEnableVertexAttribArray(1);
							// float rot
							glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 52, (void*)(28));
							glEnableVertexAttribArray(2);
							// vec2 cen
							glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 52, (void*)(32));
							glEnableVertexAttribArray(3);
							// vec2 dim
							glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 52, (void*)(40));
							glEnableVertexAttribArray(4);
							// float rad
							glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, 52, (void*)(48));
							glEnableVertexAttribArray(5);
						}

						// Renders round rects (same as rect)
						#define mugGL_roundrects_render mugGL_rects_render
						// Subrenders round rects (same as rect)
						#define mugGL_roundrects_subrender mugGL_rects_subrender

						// Fills buffer with needed info
						void mugGL_roundrects_fill(mugGL_ObjBuffer* buf) {
							buf->obj_type = MUG_OBJECT_ROUND_RECT;

							// Amount of bytes used on vertex per object:
							// one vertex = vec3+vec4+float+vec2+vec2+float (52)
							// one round rect = four vertexes (208)
							buf->bv_per_obj = 208;

							// Amount of bytes used on index per object:
							// one round rect = six indexes (24)
							buf->bi_per_obj = 24;

							// Function equivalents
							buf->fill_vertexes = mugGL_roundrects_fill_vertexes;
							buf->fill_indexes = mugGL_roundrects_fill_indexes;
							buf->desc = mugGL_roundrects_desc;
							buf->render = mugGL_roundrects_render;
							buf->subrender = mugGL_roundrects_subrender;
						}

				/* 2D texture */

					// Data format: { vec3 pos, vec4 col, vec2 tex }
					// Indexed data: { 0, 1, 3, 1, 2, 3 }

					/* Shaders */

						// Vertex shader
						const char* mugGL_2DtextureVS = 
							// Version
							"#version 330 core\n"

							// Input { vec3 pos, vec4 col, vec2 tex }
							"layout(location=0)in vec3 vPos;"
							"layout(location=1)in vec4 vCol;"
							"layout(location=2)in vec2 vTex;"

							// Output { vec4 col, vec2 tex }
							"out vec4 fCol;"
							"out vec2 fTex;"

							// Dimensions of graphic divided by 2
							"uniform vec2 d;"
							// Modifiers
							"uniform vec3 aP;" // addPos
							"uniform vec3 mP;" // mulPos

							// Main
							"void main(){"
								// Set position
								"gl_Position=vec4("
									// X
									"(((vPos.x*mP.x)+aP.x)-(d.x))/d.x,"
									// Y
									"-(((vPos.y*mP.y)+aP.y)-(d.y))/d.y,"
									// Z
									"(vPos.z*mP.z)+aP.z,"
									// W
									"1.0"
								");"

								// Transfer to fragment
								"fCol=vCol;"
								"fTex=vTex;"
							"}"
						;

						// Fragment shader
						const char* mugGL_2DtextureFS = 
							// Version
							"#version 330 core\n"

							// Input { vec4 col, vec2 tex }
							"in vec4 fCol;"
							"in vec2 fTex;"
							// Output { vec4 col }
							"out vec4 oCol;"

							// Sampler
							"uniform sampler2D tex;"
							// Modifiers
							"uniform vec4 aC;" // addCol
							"uniform vec4 mC;" // mulCol

							// Main
							"void main(){"
								// Set color
								"oCol=((fCol*texture(tex,fTex))*mC)+aC;"
							"}"
						;

						// Creates program for 2D textures
						void mugGL_2Dtextures_shader_load(mug_Graphic* gfx, mugGL_Shader* shader, mugResult* result) {
							// Exit if shader already compiled
							if (shader->program) {
								return;
							}

							// Compile shader
							mugResult res = mugGL_shader_create_vf(gfx, shader, mugGL_2DtextureVS, mugGL_2DtextureFS);
							if (res != MUG_SUCCESS) {
								MU_SET_RESULT(result, res)
							}
						}

					/* Buffer */

						// Fills a particular vertex given what to multiply dimensions by
						// (Used to alternate between 4 corners)
						// Returns new v pointer
						static inline GLfloat* mugGL_2Dtextures_fill_invertexes(
							// Rect data
							GLfloat* v, mug2DTextureRect* rect,
							// Half-dim and vector dim
							float hdim[2], float vdim0, float vdim1,
							// Center and half-dim of texture cutout
							float ctex[2], float htdim[2],
							// Sin/Cos rotation
							float srot, float crot
						) {
							// vec3 pos
							// - x and y
							mugMath_rot_point_point(
								rect->center.pos[0]+(hdim[0]*vdim0), // Point x
								rect->center.pos[1]+(hdim[1]*vdim1), // Point y
								rect->center.pos[0], // Center x
								rect->center.pos[1], // Center y
								srot, crot, // Sin/Cos rotation
								v
							);
							// - z
							v[2] = rect->center.pos[2];

							// vec4 col
							mu_memcpy(&v[3], rect->center.col, 16);
							// vec2 tex
							v[7] = ctex[0]+(htdim[0]*vdim0);
							v[8] = ctex[1]+(htdim[1]*vdim1);
							return v+9;
						}

						// Fills vertex data
						void mugGL_2Dtextures_fill_vertexes(GLfloat* v, void* obj, uint32_m c) {
							// Convert obj to struct array
							mug2DTextureRect* rects = (mug2DTextureRect*)obj;

							// Loop through each rect
							for (uint32_m i = 0; i < c; ++i) {
								// Store half-dim
								float hdim[2] = { rects->dim[0]/2.f, rects->dim[1]/2.f };
								// Store sin/cos of rotation
								// Sin is negative because y-direction is flipped in mug coordinates
								float srot = -mu_sinf(rects->rot);
								float crot = mu_cosf(rects->rot);
								// Half-texture dim
								float htdim[2] = { rects->tex_dim[0]/2.f, rects->tex_dim[1]/2.f };
								// Center of texture cutout
								float ctex[2] = { rects->tex_pos[0]+htdim[0], rects->tex_pos[1]+htdim[1] };

								// Top-left
								v = mugGL_2Dtextures_fill_invertexes(v, rects,   hdim, -1.f, -1.f, ctex, htdim, srot, crot);
								// Bottom-left
								v = mugGL_2Dtextures_fill_invertexes(v, rects,   hdim, -1.f,  1.f, ctex, htdim, srot, crot);
								// Bottom-right
								v = mugGL_2Dtextures_fill_invertexes(v, rects,   hdim,  1.f,  1.f, ctex, htdim, srot, crot);
								// Top-right
								v = mugGL_2Dtextures_fill_invertexes(v, rects,   hdim,  1.f, -1.f, ctex, htdim, srot, crot);
							}
						}

						// Fills index data (same as rect)
						#define mugGL_2Dtextures_fill_indexes mugGL_rects_fill_indexes

						// Describes data
						void mugGL_2Dtextures_desc(void) {
							// vec3 pos
							glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, 0);
							glEnableVertexAttribArray(0);
							// vec4 col
							glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 36, (void*)(12));
							glEnableVertexAttribArray(1);
							// vec2 tex
							glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 36, (void*)(28));
							glEnableVertexAttribArray(2);
						}

						// Renders data (same as rects)
						#define mugGL_2Dtextures_render mugGL_rects_render
						// Subrenders data (same as rects)
						#define mugGL_2Dtextures_subrender mugGL_rects_subrender

						// Fills buffer with needed info
						void mugGL_2Dtextures_fill(mugGL_ObjBuffer* buf) {
							buf->obj_type = MUG_OBJECT_TEXTURE_2D;

							// Amount of bytes used on vertex per object:
							// one vertex = vec3+vec4+vec2 (36)
							// one rect = four vertexes (144)
							buf->bv_per_obj = 144;

							// Amount of bytes used on index per object:
							// one rect = six indexes (24)
							buf->bi_per_obj = 24;

							// Function equivalents
							buf->fill_vertexes = mugGL_2Dtextures_fill_vertexes;
							buf->fill_indexes = mugGL_2Dtextures_fill_indexes;
							buf->desc = mugGL_2Dtextures_desc;
							buf->render = mugGL_2Dtextures_render;
							buf->subrender = mugGL_2Dtextures_subrender;
						}

				/* 2D texture array */

					// Data format: { vec3 pos, vec4 col, vec3 tex }
					// Indexed data: { 0, 1, 3, 1, 2, 3 }

					/* Shaders */

						// Vertex shader
						const char* mugGL_2DtexturearrVS = 
							// Version
							"#version 330 core\n"

							// Input { vec3 pos, vec4 col, vec3 tex }
							"layout(location=0)in vec3 vPos;"
							"layout(location=1)in vec4 vCol;"
							"layout(location=2)in vec3 vTex;"

							// Output {vec4 col, vec3 tex }
							"out vec4 fCol;"
							"out vec3 fTex;"

							// Dimensions of graphic divided by 2
							"uniform vec2 d;"
							// Modifiers
							"uniform vec3 aP;" // addPos
							"uniform vec3 mP;" // mulPos

							// Main
							"void main(){"
								// Set position
								"gl_Position=vec4("
									// X
									"(((vPos.x*mP.x)+aP.x)-(d.x))/d.x,"
									// Y
									"-(((vPos.y*mP.y)+aP.y)-(d.y))/d.y,"
									// Z
									"(vPos.z*mP.z)+aP.z,"
									// W
									"1.0"
								");"

								// Transfer to fragment
								"fCol=vCol;"
								"fTex=vTex;"
							"}"
						;

						// Fragment shader
						const char* mugGL_2DtexturearrFS = 
							// Version
							"#version 330 core\n"

							// Input { vec4 col, vec3 tex }
							"in vec4 fCol;"
							"in vec3 fTex;"
							// Output { vec4 col }
							"out vec4 oCol;"

							// Sampler
							"uniform sampler2DArray tex;"
							// Modifiers
							"uniform vec4 aC;" // addCol
							"uniform vec4 mC;" // mulCol

							// Main
							"void main(){"
								// Set color
								"oCol=((fCol*texture(tex, fTex))*mC)+aC;"
							"}"
						;

						// Creates program for 2D texture arrays
						void mugGL_2Dtexturearr_shader_load(mug_Graphic* gfx, mugGL_Shader* shader, mugResult* result) {
							// Exit if shader already compiled
							if (shader->program) {
								return;
							}

							// Compile shader
							mugResult res = mugGL_shader_create_vf(gfx, shader, mugGL_2DtexturearrVS, mugGL_2DtexturearrFS);
							if (res != MUG_SUCCESS) {
								MU_SET_RESULT(result, res)
							}
						}

					/* Buffer */

						// Fills a particular vertex given what to multiply dimensions by
						// (Used to alternate between 4 corners)
						// Returns new v pointer
						// This code is considerably similar to mugGL_2Dtextures_fill_invertexes
						static inline GLfloat* mugGL_2Dtexturearr_fill_invertexes(
							// Rect data
							GLfloat* v, mug2DTextureArrayRect* rect,
							// Half-dim and vector dim
							float hdim[2], float vdim0, float vdim1,
							// Center and half-dim of texture cutout
							float ctex[2], float htdim[2],
							// Sin/Cos rotation
							float srot, float crot
						) {
							// vec3 pos
							// - x and y
							mugMath_rot_point_point(
								rect->center.pos[0]+(hdim[0]*vdim0), // Point x
								rect->center.pos[1]+(hdim[1]*vdim1), // Point y
								rect->center.pos[0], // Center x
								rect->center.pos[1], // Center y
								srot, crot, // Sin/Cos rotation
								v
							);
							// - z
							v[2] = rect->center.pos[2];

							// vec4 col
							mu_memcpy(&v[3], rect->center.col, 16);
							// vec3 tex
							v[7] = ctex[0]+(htdim[0]*vdim0);
							v[8] = ctex[1]+(htdim[1]*vdim1);
							v[9] = rect->tex_pos[2];
							return v+10;
						}

						// Fills vertex data
						void mugGL_2Dtexturearr_fill_vertexes(GLfloat* v, void* obj, uint32_m c) {
							// Convert obj to struct array
							mug2DTextureArrayRect* rects = (mug2DTextureArrayRect*)obj;

							// Loop through each rect
							for (uint32_m i = 0; i < c; ++i) {
								// Store half-dim
								float hdim[2] = { rects->dim[0]/2.f, rects->dim[1]/2.f };
								// Store sin/cos of rotation
								// Sin is negative because y-direction is flipped in mug coordinates
								float srot = -mu_sinf(rects->rot);
								float crot =  mu_cosf(rects->rot);
								// Half-texture dim
								float htdim[2] = { rects->tex_dim[0]/2.f, rects->tex_dim[1]/2.f };
								// Center of texture cutout
								float ctex[2] = { rects->tex_pos[0]+htdim[0], rects->tex_pos[1]+htdim[1] };

								// Top-left
								v = mugGL_2Dtexturearr_fill_invertexes(v, rects  , hdim, -1.f, -1.f, ctex, htdim, srot, crot);
								// Bottom-left
								v = mugGL_2Dtexturearr_fill_invertexes(v, rects  , hdim, -1.f,  1.f, ctex, htdim, srot, crot);
								// Bottom-right
								v = mugGL_2Dtexturearr_fill_invertexes(v, rects  , hdim,  1.f,  1.f, ctex, htdim, srot, crot);
								// Top-right
								v = mugGL_2Dtexturearr_fill_invertexes(v, rects++, hdim,  1.f, -1.f, ctex, htdim, srot, crot);
							}
						}

						// Fills index data (same as rect)
						#define mugGL_2Dtexturearr_fill_indexes mugGL_rects_fill_indexes

						// Describes data
						void mugGL_2Dtexturearr_desc(void) {
							// vec3 pos
							glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 40, 0);
							glEnableVertexAttribArray(0);
							// vec4 col
							glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 40, (void*)(12));
							glEnableVertexAttribArray(1);
							// vec3 tex
							glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 40, (void*)(28));
							glEnableVertexAttribArray(2);
						}

						// Renders data (same as rects)
						#define mugGL_2Dtexturearr_render mugGL_rects_render
						// Subrenders data (same as rects)
						#define mugGL_2Dtexturearr_subrender mugGL_rects_subrender

						// Fills buffer with needed info
						void mugGL_2Dtexturearr_fill(mugGL_ObjBuffer* buf) {
							buf->obj_type = MUG_OBJECT_TEXTURE_2D_ARRAY;

							// Amount of bytes used on vertex per object:
							// one vertex = vec3+vec4+vec3 (40)
							// one rect = four vertexes (160)
							buf->bv_per_obj = 160;

							// Amount of bytes used on index per object:
							// one rect = six indexes (24)
							buf->bi_per_obj = 24;

							// Function equivalents
							buf->fill_vertexes = mugGL_2Dtexturearr_fill_vertexes;
							buf->fill_indexes = mugGL_2Dtexturearr_fill_indexes;
							buf->desc = mugGL_2Dtexturearr_desc;
							buf->render = mugGL_2Dtexturearr_render;
							buf->subrender = mugGL_2Dtexturearr_subrender;
						}

			/* Context setup */

				// Struct for shaders
				struct mugGL_Shaders {
					mugGL_Shader point;
					mugGL_Shader line;
					mugGL_Shader triangle;
					mugGL_Shader rect;
					mugGL_Shader circle;
					mugGL_Shader squircle;
					mugGL_Shader roundrect;
					mugGL_Shader textures2D;
					mugGL_Shader texturearrays2D;
				};
				typedef struct mugGL_Shaders mugGL_Shaders;

				// Struct for GL context
				struct mugGL_Context {
					// Shaders
					mugGL_Shaders shaders;
				};
				typedef struct mugGL_Context mugGL_Context;

				// Pointer to loading context
				// This is a non-threadsafe solution that requires
				// no more than one mug context to be created at
				// one time within the program.
				muCOSAContext* mugGL_load_func_context;

				// Finds an OpenGL address
				void* mugGL_load_func(const char* name) {
					return muCOSA_gl_get_proc_address(mugGL_load_func_context, name);
				}

				// Loads a valid OpenGL context
				mugResult mugGL_init_context(mug_Graphic* gfx) {
					// Allocate inner GL context
					gfx->p = mu_malloc(sizeof(mugGL_Context));
					if (!gfx->p) {
						return MUG_FAILED_MALLOC;
					}
					// - Casted handle
					mugGL_Context* ic = (mugGL_Context*)gfx->p;

					// Create OpenGL context
					mugResult res = mugGraphicGL_create(gfx);
					if (mug_result_is_fatal(res)) {
						mu_free(gfx->p);
						return res;
					}

					// Bind OpenGL context
					mugGraphicGL_bind(gfx);

					// Load OpenGL functions
					mugGL_load_func_context = &gfx->context->cosa;
					if (!gladLoadGL((GLADloadfunc)mugGL_load_func)) {
						mu_free(gfx->p);
						mugGraphicGL_destroy(gfx);
						return MUG_GL_FAILED_LOAD;
					}

					// Enable and set blending
					glEnable(GL_BLEND);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					// Enable depth testing
					glEnable(GL_DEPTH_TEST);
					glDepthFunc(GL_GEQUAL);
					// Set initial viewport
					glViewport(0, 0, gfx->dim[0], gfx->dim[1]);

					// Zero-out necessary struct memory
					mu_memset(&ic->shaders, 0, sizeof(ic->shaders));

					return res;
				}

				// Deloads a valid OpenGL context
				void mugGL_term_context(mug_Graphic* gfx) {
					// Free inner context
					mu_free(gfx->p);
					// Destroy OpenGL context
					mugGraphicGL_destroy(gfx);
				}

				// Update function; gets called at end of swap buffers
				void mugGL_update_context(mug_Graphic* gfx) {
					// ...
					return; if (gfx) {}
				}

			/* Objects */

				/* Shader logic */

					// Converts object type to shader pointer
					mugGL_Shader* mugGL_object_type_to_shader(mugGL_Context* context, mugObjectType type) {
						switch (type) {
							default: return 0; break;
							case MUG_OBJECT_POINT: return &context->shaders.point; break;
							case MUG_OBJECT_LINE: return &context->shaders.line; break;
							case MUG_OBJECT_TRIANGLE: return &context->shaders.triangle; break;
							case MUG_OBJECT_RECT: return &context->shaders.rect; break;
							case MUG_OBJECT_CIRCLE: return &context->shaders.circle; break;
							case MUG_OBJECT_SQUIRCLE: return &context->shaders.squircle; break;
							case MUG_OBJECT_ROUND_RECT: return &context->shaders.roundrect; break;
							case MUG_OBJECT_TEXTURE_2D: return &context->shaders.textures2D; break;
							case MUG_OBJECT_TEXTURE_2D_ARRAY: return &context->shaders.texturearrays2D; break;
						}
					}

					// Converts object type to shader program pointer
					GLuint* mugGL_object_type_to_program(mugGL_Context* context, mugObjectType type) {
						// Convert object type to shader pointer
						mugGL_Shader* shader = mugGL_object_type_to_shader(context, type);
						// If NULL, shader doesn't exist
						if (!shader) {
							return 0;
						}
						// Return pointer to shader program if otherwise
						return &shader->program;
					}

					// Updates dimensions shaders for each active shader program
					void mugGL_update_dimensions(mugGL_Context* context, uint32_m dim[2]) {
						// Holder variables
						GLuint* program;
						float fdim[2] = { ((float)dim[0]) / 2.f, ((float)dim[1]) / 2.f };

						// Loop through each valid object type enum
						for (mugObjectType objtype = MUG_OBJECT_FIRST; objtype <= MUG_OBJECT_LAST; ++objtype) {
							// Find program ID
							program = mugGL_object_type_to_program(context, objtype);
							// If program ID exists, update dimensions uniform
							if (program && *program) {
								glUseProgram(*program);
								glUniform2f(glGetUniformLocation(*program, "d"), fdim[0], fdim[1]);
								glUseProgram(0);
							}
						}
					}

					// Loads a given object type
					void mugGL_load_object_type(mug_Graphic* gfx, mugGL_Context* context, mugResult* result, mugObjectType type) {
						// Call function based on object type
						switch (type) {
							default: MU_SET_RESULT(result, MUG_UNKNOWN_OBJECT_TYPE) break;
							case MUG_OBJECT_POINT: mugGL_points_shader_load(gfx, &context->shaders.point, result); break;
							case MUG_OBJECT_LINE: mugGL_lines_shader_load(gfx, &context->shaders.line, result); break;
							case MUG_OBJECT_TRIANGLE: mugGL_triangles_shader_load(gfx, &context->shaders.triangle, result); break;
							case MUG_OBJECT_RECT: mugGL_rects_shader_load(gfx, &context->shaders.rect, result); break;
							case MUG_OBJECT_CIRCLE: mugGL_circles_shader_load(gfx, &context->shaders.circle, result); break;
							case MUG_OBJECT_SQUIRCLE: mugGL_squircles_shader_load(gfx, &context->shaders.squircle, result); break;
							case MUG_OBJECT_ROUND_RECT: mugGL_roundrects_shader_load(gfx, &context->shaders.roundrect, result); break;
							case MUG_OBJECT_TEXTURE_2D: mugGL_2Dtextures_shader_load(gfx, &context->shaders.textures2D, result); break;
							case MUG_OBJECT_TEXTURE_2D_ARRAY: mugGL_2Dtexturearr_shader_load(gfx, &context->shaders.texturearrays2D, result); break;
						}
					}

					// Deloads a given object type
					void mugGL_deload_object_type(mugGL_Context* context, mugObjectType type) {
						// Convert object type to shader pointer
						mugGL_Shader* shader = mugGL_object_type_to_shader(context, type);
						// Destroy if exists
						if (shader) {
							mugGL_shader_destroy(shader);
						}
					}

					// Sets an object type modifier
					void mugGL_object_type_mod(mugGL_Context* context, mugResult* result, mugObjectType type, mugObjectMod mod, float* data) {
						// Get shader handle
						mugGL_Shader* shader = mugGL_object_type_to_shader(context, type);
						if (!shader) {
							MU_SET_RESULT(result, MUG_UNKNOWN_OBJECT_TYPE)
							return;
						}

						// Bind shader
						mugGL_shader_bind(shader);

						// Identify attribute
						switch (mod) {
							default: MU_SET_RESULT(result, MUG_UNKNOWN_OBJECT_MOD) break;
							// addPos
							case MUG_OBJECT_ADD_POS: glUniform3f(glGetUniformLocation(shader->program, "aP"), data[0], data[1], data[2]); break;
							case MUG_OBJECT_MUL_POS: glUniform3f(glGetUniformLocation(shader->program, "mP"), data[0], data[1], data[2]); break;
							case MUG_OBJECT_ADD_COL: glUniform4f(glGetUniformLocation(shader->program, "aC"), data[0], data[1], data[2], data[3]); break;
							case MUG_OBJECT_MUL_COL: glUniform4f(glGetUniformLocation(shader->program, "mC"), data[0], data[1], data[2], data[3]); break;
						}

						// Unbind shader
						mugGL_shader_unbind(shader);
					}

				/* Buffer logic */

					// Fills information about buffer based on the type
					mugResult mugGL_fill_info_buffer(mugGL_ObjBuffer* buf, mugObjectType type) {
						// Set texture handle to null
						buf->tex = 0;

						// Fill based on type
						switch (type) {
							default: return MUG_UNKNOWN_OBJECT_TYPE; break;
							case MUG_OBJECT_POINT: mugGL_points_fill(buf); break;
							case MUG_OBJECT_LINE: mugGL_lines_fill(buf); break;
							case MUG_OBJECT_TRIANGLE: mugGL_triangles_fill(buf); break;
							case MUG_OBJECT_RECT: mugGL_rects_fill(buf); break;
							case MUG_OBJECT_CIRCLE: mugGL_circles_fill(buf); break;
							case MUG_OBJECT_SQUIRCLE: mugGL_squircles_fill(buf); break;
							case MUG_OBJECT_ROUND_RECT: mugGL_roundrects_fill(buf); break;
							case MUG_OBJECT_TEXTURE_2D: mugGL_2Dtextures_fill(buf); break;
							case MUG_OBJECT_TEXTURE_2D_ARRAY: mugGL_2Dtexturearr_fill(buf); break;
						}
						return MUG_SUCCESS;
					}

					// Creates an object buffer
					mugGL_ObjBuffer* mugGL_object_buffer_create(mug_Graphic* gfx, mugGL_Context* context, mugResult* result, mugObjectType type, uint32_m obj_count, void* obj) {
						// Load object type
						mugResult res = MUG_SUCCESS;
						mugGL_load_object_type(gfx, context, &res, type);
						if (res != MUG_SUCCESS) {
							MU_SET_RESULT(result, res)
							if (mug_result_is_fatal(res)) {
								return 0;
							}
						}

						// Allocate struct
						mugGL_ObjBuffer* buf = (mugGL_ObjBuffer*)mu_malloc(sizeof(mugGL_ObjBuffer));
						if (!buf) {
							MU_SET_RESULT(result, MUG_FAILED_MALLOC)
							return 0;
						}

						// Fill info about buffer based on type
						res = mugGL_fill_info_buffer(buf, type);
						if (res != MUG_SUCCESS) {
							MU_SET_RESULT(result, res)
							if (mug_result_is_fatal(res)) {
								mu_free(buf);
								return 0;
							}
						}

						// Fill other calculatable info
						buf->obj_count = obj_count;
						buf->vbuf_size = obj_count*buf->bv_per_obj;
						if (buf->fill_indexes) {
							buf->ibuf_size = obj_count*buf->bi_per_obj;
						}

						// Create buffers
						res = mugGL_objects_create(buf, obj);
						if (res != MUG_SUCCESS) {
							MU_SET_RESULT(result, res)
							if (mug_result_is_fatal(res)) {
								mu_free(buf);
								return 0;
							}
						}

						return buf;
					}

					// Destroys an object buffer
					mugGL_ObjBuffer* mugGL_object_buffer_destroy(mugGL_ObjBuffer* buf) {
						// Destroy inner buffers
						mugGL_objects_destroy(buf);
						// Free malloc'd data
						mu_free(buf);
						// Return null
						return 0;
					}

					// Renders an object buffer
					void mugGL_object_buffer_render(mugGL_Context* context, mugGL_ObjBuffer* buf) {
						// Get shader handle
						mugGL_Shader* shader = mugGL_object_type_to_shader(context, buf->obj_type);
						if (!shader) {
							return;
						}

						// Bind shader
						mugGL_shader_bind(shader);
						// Render buffer
						mugGL_objects_render(buf);
						// Unbind shader
						mugGL_shader_unbind(shader);
					}

					// Subrenders an object buffer
					void mugGL_object_buffer_subrender(mugGL_Context* context, mugGL_ObjBuffer* buf, uint32_m offset, uint32_m count) {
						// Get shader handle
						mugGL_Shader* shader = mugGL_object_type_to_shader(context, buf->obj_type);
						if (!shader) {
							return;
						}

						// Bind shader
						mugGL_shader_bind(shader);
						// Subrender buffer
						mugGL_objects_subrender(buf, offset, count);
						// Unbind shader
						mugGL_shader_unbind(shader);
					}

			/* Misc. */

				// Clears the screen
				void mugGL_clear(mug_Graphic* gfx, float r, float g, float b) {
					// Update dimensions
					{
						// Retrieve dimensions
						uint32_m dim[2] = { gfx->dim[0], gfx->dim[1] };
						mugGraphic_dim(gfx);
						// Viewport if they've changed
						if (dim[0] != gfx->dim[0] || dim[1] != gfx->dim[1]) {
							glViewport(0, 0, gfx->dim[0], gfx->dim[1]);

							// + Update dimensions for shaders
							mugGL_update_dimensions((mugGL_Context*)gfx->p, gfx->dim);
						}
					}

					// Set clear color
					glClearColor(r, g, b, 1.f);
					// Set clear depth
					glClearDepth(0.0);
					// Clear screen color and depth
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				}

				// Maximums
				uint32_m mugGL_max(mugMax max) {
					GLint gi = 0;

					switch (max) {
						default: return 0; break;

						// Texture
						case MUG_MAX_TEXTURE_WIDTH_HEIGHT: {
							glGetIntegerv(GL_MAX_TEXTURE_SIZE, &gi);
							return gi;
						} break;
						case MUG_MAX_TEXTURE_DEPTH: {
							glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &gi);
							return gi;
						} break;
					}
				}

		#endif /* MU_SUPPORT_OPENGL */

		/* Outer graphic logic */

			// Destroy graphic
			MUDEF muGraphic mug_graphic_destroy(mugContext* context, muGraphic gfx) {
				// Get handle to inner graphics
				mug_Graphic* ingfx = (mug_Graphic*)gfx;

				// Destroy system
				switch (ingfx->system) {
					default: break;

					// OpenGL
					#ifdef MU_SUPPORT_OPENGL
						case MU_GRAPHIC_OPENGL: mugGL_term_context(ingfx); break;
					#endif
				}

				// Destroy graphic
				mugGraphic_destroy(ingfx);

				// Free graphic
				mu_free(ingfx);

				// Return null
				return 0;

				// To avoid unused parameter warnings
				if (context) {}
			}

			// Create graphic via window
			MUDEF muGraphic mug_graphic_create_window(mugContext* context, mugResult* result, muGraphicSystem system, muWindowInfo* info) {
				// Allocate handle to graphics
				mug_Graphic* gfx = (mug_Graphic*)mu_malloc(sizeof(mug_Graphic));
				if (!gfx) {
					MU_SET_RESULT(result, MUG_FAILED_MALLOC);
					return 0;
				}

				// Create window graphic
				mugResult res = mugGraphic_window_create(context, info, gfx, system);
				if (res != MUG_SUCCESS) {
					MU_SET_RESULT(result, res)
					if (mug_result_is_fatal(res)) {
						mu_free(gfx);
						return 0;
					}
				}

				// Create context based on given system value
				switch (system) {
					// Unknown system value
					default: {
						// Destroy graphic
						mugGraphic_destroy(gfx);
						// Free graphic
						mu_free(gfx);
						// Return failure value
						MU_SET_RESULT(result, MUG_UNKNOWN_GRAPHIC_SYSTEM)
						return 0;
					} break;

					// OpenGL
					#ifdef MU_SUPPORT_OPENGL
						case MU_GRAPHIC_OPENGL: {
							// Initiate OpenGL context
							res = mugGL_init_context(gfx);

							// Return possible failure
							if (res != MUG_SUCCESS) {
								MU_SET_RESULT(result, res)
								if (mug_result_is_fatal(res)) {
									mugGraphic_destroy(gfx);
									mu_free(gfx);
									return 0;
								}
							}
						} break;
					#endif
				}

				return gfx;
			}

			// Get window handle from graphic
			MUDEF muWindow mug_graphic_get_window(mugContext* context, muGraphic gfx) {
				// Return window handle
				return ((mug_Graphic*)gfx)->parent.cosa_win;
				// To avoid unused parameter warnings
				if (context) {}
			}

			// Returns if the graphic exists
			MUDEF muBool mug_graphic_exists(mugContext* context, muGraphic gfx) {
				// Return existence state
				return mugGraphic_exists((mug_Graphic*)gfx);

				// To avoid unused parameter warnings
				if (context) {}
			}

			// Clear graphic
			MUDEF void mug_graphic_clear(mugContext* context, mugResult* result, muGraphic gfx, float r, float g, float b) {
				// Get inner graphic handle
				mug_Graphic* igfx = (mug_Graphic*)gfx;

				// Clear screen
				switch (igfx->system) {
					default: break;

					// OpenGL
					#ifdef MU_SUPPORT_OPENGL
						case MU_GRAPHIC_OPENGL: {
							// Bind
							mugGraphicGL_bind(igfx);
							// Clear
							mugGL_clear(igfx, r, g, b);
						} break;
					#endif
				}

				// To avoid unused parameter warnings
				return; if (context) {} if (result) {} if (r) {} if (g) {} if (b) {}
			}

			// Swaps buffers, presenting image to screen
			MUDEF void mug_graphic_swap_buffers(mugContext* context, mugResult* result, muGraphic gfx) {
				// Get inner graphic handle
				mug_Graphic* igfx = (mug_Graphic*)gfx;

				// Swap buffers
				switch (igfx->system) {
					default: break;

					// OpenGL
					#ifdef MU_SUPPORT_OPENGL
						case MU_GRAPHIC_OPENGL: {
							// Bind OpenGL
							mugGraphicGL_bind(igfx);

							// Swap buffers
							mugResult res = mugGraphicGL_swap_buffers(igfx);
							if (res != MUG_SUCCESS) {
								MU_SET_RESULT(result, res)
								if (mug_result_is_fatal(res)) {
									return;
								}
							}

							// Update OpenGL afterwards
							mugGL_update_context(igfx);
						} break;
					#endif
				}

				// Avoid unused parameter warnings
				return; if (context) {} if (result) {}
			}

			// Updates the graphic's parent
			MUDEF void mug_graphic_update(mugContext* context, mugResult* result, muGraphic gfx, float target_fps) {
				// Get inner graphic handle
				mug_Graphic* igfx = (mug_Graphic*)gfx;

				// Calculate delta-time
				double current_time = muCOSA_fixed_time_get(&context->cosa);
				double delta_time = current_time - igfx->last_time;
				igfx->last_time = current_time;

				// Sleep, if needed, to adjust for frame time
				if (target_fps > 0.f) {
					double inv_fps = 1.0 / target_fps;
					if (delta_time < inv_fps) {
						muCOSA_sleep(&context->cosa, inv_fps-delta_time);
					}
				}

				// Update graphic
				mugResult res = mugGraphic_update(igfx);
				if (res != MUG_SUCCESS) {
					MU_SET_RESULT(result, res)
				}
			}

		/* Object types */

			// Loads the given object type
			MUDEF void mug_gobject_load(mugContext* context, mugResult* result, muGraphic gfx, mugObjectType obj_type) {
				// Get inner graphic handle
				mug_Graphic* igfx = (mug_Graphic*)gfx;

				// Do thing based on graphic system
				switch (igfx->system) {
					default: break;

					// OpenGL
					#ifdef MU_SUPPORT_OPENGL
						case MU_GRAPHIC_OPENGL: {
							// Bind OpenGL
							mugGraphicGL_bind(igfx);
							// Load object type
							mugGL_load_object_type(igfx, (mugGL_Context*)igfx->p, result, obj_type);
						} break;
					#endif
				}

				// To avoid unused parameter warnings in some circumstances
				return; if (context) {} if (result) {} if (obj_type) {}
			}

			// Deloads the given object type
			MUDEF void mug_gobject_deload(mugContext* context, muGraphic gfx, mugObjectType obj_type) {
				// Get inner graphic handle
				mug_Graphic* igfx = (mug_Graphic*)gfx;

				// Do things based on graphic system
				switch (igfx->system) {
					default: break;

					// OpenGL
					#ifdef MU_SUPPORT_OPENGL
						case MU_GRAPHIC_OPENGL: {
							// Bind OpenGL
							mugGraphicGL_bind(igfx);
							// Deload
							mugGL_deload_object_type((mugGL_Context*)igfx->p, obj_type);
						} break;
					#endif
				}

				// To avoid unused parameter warnings
				return; if (context) {} if (obj_type) {}
			}

			// Sets modification variable for given object type
			MUDEF void mug_gobject_mod(mugContext* context, muGraphic gfx, mugObjectType type, mugObjectMod mod, float* data) {
				// Get inner graphic handle
				mug_Graphic* igfx = (mug_Graphic*)gfx;

				// Do things based on graphic system
				switch (igfx->system) {
					default: break;

					// OpenGL
					#ifdef MU_SUPPORT_OPENGL
						case MU_GRAPHIC_OPENGL: {
							mugGL_object_type_mod((mugGL_Context*)igfx->p, 0, type, mod, data);
						} break;
					#endif
				}

				// To avoid unused parameter warnings
				return; if (context) {} if (type) {} if (mod) {} if (data) {}
			}

		/* Object buffer stuff */

			// Creates an object buffer
			MUDEF mugObjects mug_gobjects_create(mugContext* context, mugResult* result, muGraphic gfx, mugObjectType type, uint32_m obj_count, void* objs) {
				// Get inner graphic handle
				mug_Graphic* igfx = (mug_Graphic*)gfx;

				// Do things based on graphic system
				switch (igfx->system) {
					default: return 0; break;

					// OpenGL
					#ifdef MU_SUPPORT_OPENGL
						case MU_GRAPHIC_OPENGL: {
							mugGraphicGL_bind(igfx);
							return mugGL_object_buffer_create(igfx, (mugGL_Context*)igfx->p, result, type, obj_count, objs);
						} break;
					#endif
				}

				// To avoid unused parameter warnings
				if (context) {} if (result) {} if (type) {} if (obj_count) {} if (objs) {}
			}

			// Destroys an object buffer
			MUDEF mugObjects mug_gobjects_destroy(mugContext* context, muGraphic gfx, mugObjects objs) {
				// Get inner graphic handle
				mug_Graphic* igfx = (mug_Graphic*)gfx;

				// Do things based on graphic system
				switch (igfx->system) {
					default: return 0; break;

					// OpenGL
					#ifdef MU_SUPPORT_OPENGL
						case MU_GRAPHIC_OPENGL: {
							return mugGL_object_buffer_destroy((mugGL_ObjBuffer*)objs);
						} break;
					#endif
				}

				// To avoid unused parameter warnings
				if (context) {} if (objs) {}
			}

			MUDEF void mug_gobjects_render(mugContext* context, mugResult* result, muGraphic gfx, mugObjects objs) {
				// Get inner graphic handle
				mug_Graphic* igfx = (mug_Graphic*)gfx;

				// Do things based on graphic system
				switch (igfx->system) {
					default: break;

					// OpenGL
					#ifdef MU_SUPPORT_OPENGL
						case MU_GRAPHIC_OPENGL: {
							mugGraphicGL_bind(igfx);
							mugGL_object_buffer_render((mugGL_Context*)igfx->p, (mugGL_ObjBuffer*)objs);
						} break;
					#endif
				}

				// To avoid unused parameter warnings
				return; if (context) {} if (result) {} if (objs) {}
			}

			MUDEF void mug_gobjects_subrender(mugContext* context, mugResult* result, muGraphic gfx, mugObjects objs, uint32_m offset, uint32_m count) {
				// Get inner graphic handle
				mug_Graphic* igfx = (mug_Graphic*)gfx;

				// Do things based on graphic system
				switch (igfx->system) {
					default: break;

					// OpenGL
					#ifdef MU_SUPPORT_OPENGL
						case MU_GRAPHIC_OPENGL: {
							mugGraphicGL_bind(igfx);
							mugGL_object_buffer_subrender((mugGL_Context*)igfx->p, (mugGL_ObjBuffer*)objs, offset, count);
						} break;
					#endif
				}

				// To avoid unused parameter warnings
				return; if (context) {} if (result) {} if (objs) {} if (offset) {} if (count) {}
			}

			MUDEF void mug_gobjects_fill(mugContext* context, mugResult* result, muGraphic gfx, mugObjects objs, void* data) {
				// Get inner graphic handle
				mug_Graphic* igfx = (mug_Graphic*)gfx;

				// Do things based on graphic system
				switch (igfx->system) {
					default: break;

					// OpenGL
					#ifdef MU_SUPPORT_OPENGL
						case MU_GRAPHIC_OPENGL: {
							mugResult res = mugGL_objects_fill((mugGL_ObjBuffer*)objs, data);
							if (res != MUG_SUCCESS) {
								MU_SET_RESULT(result, res)
							}
						} break;
					#endif
				}

				// To avoid unused parameter warnings
				return; if (context) {} if (result) {} if (objs) {} if (data) {}
			}

			MUDEF void mug_gobjects_subfill(mugContext* context, mugResult* result, muGraphic gfx, mugObjects objs, uint32_m offset, uint32_m count, void* data) {
				// Get inner graphic handle
				mug_Graphic* igfx = (mug_Graphic*)gfx;

				// Do things based on graphic system
				switch (igfx->system) {
					default: break;

					// OpenGL
					#ifdef MU_SUPPORT_OPENGL
						case MU_GRAPHIC_OPENGL: {
							mugGraphicGL_bind(igfx);
							mugResult res = mugGL_objects_subfill((mugGL_ObjBuffer*)objs, offset, count, data);
							if (res != MUG_SUCCESS) {
								MU_SET_RESULT(result, res)
							}
						} break;
					#endif
				}

				// To avoid unused parameter warnings
				return; if (context) {} if (result) {} if (objs) {} if (offset) {} if (count) {} if (data) {}
			}

			MUDEF void mug_gobjects_texture(mugContext* context, muGraphic gfx, mugObjects obj, mugTexture tex) {
				// Get inner graphic handle
				mug_Graphic* igfx = (mug_Graphic*)gfx;

				// Do things based on graphic system
				switch (igfx->system) {
					default: break;

					// OpenGL
					#ifdef MU_SUPPORT_OPENGL
						case MU_GRAPHIC_OPENGL: {
							mugGraphicGL_bind(igfx);
							mugGL_objects_texture((mugGL_ObjBuffer*)obj, (mugGL_Texture*)tex);
						} break;
					#endif
				}

				// To avoid unused parameter warnings
				return; if (context) {} if (obj) {} if (tex) {}
			}

		/* Texture stuff */

			MUDEF mugTexture mug_gtexture_create(mugContext* context, mugResult* result, muGraphic gfx, mugTextureInfo* info, uint32_m* dim, muByte* data) {
				// Get inner graphic handle
				mug_Graphic* igfx = (mug_Graphic*)gfx;

				// Do things based on graphic system
				switch (igfx->system) {
					default: return 0; break;

					// OpenGL
					#ifdef MU_SUPPORT_OPENGL
						case MU_GRAPHIC_OPENGL: {
							mugGraphicGL_bind(igfx);
							return mugGL_texture_create(result, info, dim, data);
						} break;
					#endif
				}

				// To avoid unused parameter warnings
				if (context) {} if (result) {} if (info) {} if (dim) {} if (data) {}
			}

			MUDEF mugTexture mug_gtexture_destroy(mugContext* context, muGraphic gfx, mugTexture tex) {
				// Get inner graphic handle
				mug_Graphic* igfx = (mug_Graphic*)gfx;

				// Do things based on graphic system
				switch (igfx->system) {
					default: return 0; break;

					// OpenGL
					#ifdef MU_SUPPORT_OPENGL
						case MU_GRAPHIC_OPENGL: {
							mugGraphicGL_bind(igfx);
							return mugGL_texture_destroy((mugGL_Texture*)tex);
						} break;
					#endif
				}

				// To avoid unused parameter warnings
				if (context) {} if (tex) {}
			}

		/* Misc. */

			// Max stuff
			MUDEF uint32_m mug_max(mugContext* context, muGraphic gfx, mugMax max) {
				// Get inner graphic handle
				mug_Graphic* igfx = (mug_Graphic*)gfx;

				// Do things based on graphic system
				switch (igfx->system) {
					default: return 0; break;

					// Opengl
					#ifdef MU_SUPPORT_OPENGL
						case MU_GRAPHIC_OPENGL: {
							mugGraphicGL_bind(igfx);
							return mugGL_max(max);
						} break;
					#endif
				}

				// To avoid unused parameter warnings
				if (context) {} if (max) {}
			}

		/* Result */

			// Fatal-testing function
			MUDEF muBool mug_result_is_fatal(mugResult result) {
				switch (result) {
					default: {
						// If result value is not defined here, it
						// might be a non-fatal muCOSA-equivalent.
						// So, get the muCOSA equivalent:
						muCOSAResult cosa_res = mug_to_muCOSA_result(result);
						// If there is no muCOSA equivalent, return default val:
						if (cosa_res == MUG_SUCCESS) {
							return MU_TRUE;
						}
						// If there is a muCOSA equivalent, returns its fatal status
						return muCOSA_result_is_fatal(cosa_res);
					} break;

					case MUG_SUCCESS: return MU_FALSE; break;
				}
			}

			MUDEF muCOSAResult mug_to_muCOSA_result(mugResult result) {
				switch (result) {
					default: return MUG_SUCCESS; break;
					case MUG_MUCOSA_FAILED_NULL_WINDOW_SYSTEM: return MUCOSA_FAILED_NULL_WINDOW_SYSTEM; break;
					case MUG_MUCOSA_FAILED_MALLOC: return MUCOSA_FAILED_MALLOC; break;
					case MUG_MUCOSA_FAILED_UNKNOWN_WINDOW_ATTRIB: return MUCOSA_FAILED_UNKNOWN_WINDOW_ATTRIB; break;
					case MUG_MUCOSA_FAILED_REALLOC: return MUCOSA_FAILED_REALLOC; break;
					case MUG_MUCOSA_FAILED_UNKNOWN_GRAPHICS_API: return MUCOSA_FAILED_UNKNOWN_GRAPHICS_API; break;
					case MUG_MUCOSA_FAILED_UNSUPPORTED_GRAPHICS_API: return MUCOSA_FAILED_UNSUPPORTED_GRAPHICS_API; break;
					case MUG_MUCOSA_WIN32_FAILED_CONVERT_UTF8_TO_WCHAR: return MUCOSA_WIN32_FAILED_CONVERT_UTF8_TO_WCHAR; break;
					case MUG_MUCOSA_WIN32_FAILED_REGISTER_WINDOW_CLASS: return MUCOSA_WIN32_FAILED_REGISTER_WINDOW_CLASS; break;
					case MUG_MUCOSA_WIN32_FAILED_CREATE_WINDOW: return MUCOSA_WIN32_FAILED_CREATE_WINDOW; break;
					case MUG_MUCOSA_WIN32_FAILED_GET_WINDOW_ATTRIB: return MUCOSA_WIN32_FAILED_GET_WINDOW_ATTRIB; break;
					case MUG_MUCOSA_WIN32_FAILED_SET_WINDOW_ATTRIB: return MUCOSA_WIN32_FAILED_SET_WINDOW_ATTRIB; break;
					case MUG_MUCOSA_WIN32_FAILED_REGISTER_DUMMY_WGL_WINDOW_CLASS: return MUCOSA_WIN32_FAILED_REGISTER_DUMMY_WGL_WINDOW_CLASS; break;
					case MUG_MUCOSA_WIN32_FAILED_CREATE_DUMMY_WGL_WINDOW: return MUCOSA_WIN32_FAILED_CREATE_DUMMY_WGL_WINDOW; break;
					case MUG_MUCOSA_WIN32_FAILED_GET_DUMMY_WGL_PIXEL_FORMAT: return MUCOSA_WIN32_FAILED_GET_DUMMY_WGL_PIXEL_FORMAT; break;
					case MUG_MUCOSA_WIN32_FAILED_DESCRIBE_DUMMY_WGL_PIXEL_FORMAT: return MUCOSA_WIN32_FAILED_DESCRIBE_DUMMY_WGL_PIXEL_FORMAT; break;
					case MUG_MUCOSA_WIN32_FAILED_SET_DUMMY_WGL_PIXEL_FORMAT: return MUCOSA_WIN32_FAILED_SET_DUMMY_WGL_PIXEL_FORMAT; break;
					case MUG_MUCOSA_WIN32_FAILED_CREATE_DUMMY_WGL_CONTEXT: return MUCOSA_WIN32_FAILED_CREATE_DUMMY_WGL_CONTEXT; break;
					case MUG_MUCOSA_WIN32_FAILED_BIND_DUMMY_WGL_CONTEXT: return MUCOSA_WIN32_FAILED_BIND_DUMMY_WGL_CONTEXT; break;
					case MUG_MUCOSA_WIN32_FAILED_FIND_WGL_CREATE_CONTEXT_ATTRIBS: return MUCOSA_WIN32_FAILED_FIND_WGL_CREATE_CONTEXT_ATTRIBS; break;
					case MUG_MUCOSA_WIN32_FAILED_FIND_WGL_CHOOSE_PIXEL_FORMAT: return MUCOSA_WIN32_FAILED_FIND_WGL_CHOOSE_PIXEL_FORMAT; break;
					case MUG_MUCOSA_WIN32_FAILED_CHOOSE_WGL_PIXEL_FORMAT: return MUCOSA_WIN32_FAILED_CHOOSE_WGL_PIXEL_FORMAT; break;
					case MUG_MUCOSA_WIN32_FAILED_GET_WGL_PIXEL_FORMAT: return MUCOSA_WIN32_FAILED_GET_WGL_PIXEL_FORMAT; break;
					case MUG_MUCOSA_WIN32_FAILED_DESCRIBE_WGL_PIXEL_FORMAT: return MUCOSA_WIN32_FAILED_DESCRIBE_WGL_PIXEL_FORMAT; break;
					case MUG_MUCOSA_WIN32_FAILED_SET_WGL_PIXEL_FORMAT: return MUCOSA_WIN32_FAILED_SET_WGL_PIXEL_FORMAT; break;
					case MUG_MUCOSA_WIN32_FAILED_CREATE_WGL_CONTEXT: return MUCOSA_WIN32_FAILED_CREATE_WGL_CONTEXT; break;
					case MUG_MUCOSA_WIN32_FAILED_SET_WGL_CONTEXT: return MUCOSA_WIN32_FAILED_SET_WGL_CONTEXT; break;
					case MUG_MUCOSA_WIN32_FAILED_SWAP_WGL_BUFFERS: return MUCOSA_WIN32_FAILED_SWAP_WGL_BUFFERS; break;
					case MUG_MUCOSA_WIN32_FAILED_FIND_WGL_FUNCTION: return MUCOSA_WIN32_FAILED_FIND_WGL_FUNCTION; break;
					case MUG_MUCOSA_WIN32_FAILED_ASSOCIATE_IMM: return MUCOSA_WIN32_FAILED_ASSOCIATE_IMM; break;
					case MUG_MUCOSA_WIN32_FAILED_SET_COMPOSITION_WINDOW_POSITION: return MUCOSA_WIN32_FAILED_SET_COMPOSITION_WINDOW_POSITION; break;
					case MUG_MUCOSA_WIN32_FAILED_HOLD_CLIPBOARD: return MUCOSA_WIN32_FAILED_HOLD_CLIPBOARD; break;
					case MUG_MUCOSA_WIN32_FAILED_GET_CLIPBOARD_DATA: return MUCOSA_WIN32_FAILED_GET_CLIPBOARD_DATA; break;
					case MUG_MUCOSA_WIN32_FAILED_CONVERT_CLIPBOARD_DATA_FORMAT: return MUCOSA_WIN32_FAILED_CONVERT_CLIPBOARD_DATA_FORMAT; break;
					case MUG_MUCOSA_WIN32_FAILED_ALLOCATE_CLIPBOARD_DATA: return MUCOSA_WIN32_FAILED_ALLOCATE_CLIPBOARD_DATA; break;
					case MUG_MUCOSA_WIN32_FAILED_SET_CLIPBOARD_DATA: return MUCOSA_WIN32_FAILED_SET_CLIPBOARD_DATA; break;
				}
			}

			MUDEF mugResult muCOSA_to_mug_result(muCOSAResult result) {
				switch (result) {
					default: return MUG_SUCCESS; break;
					case MUCOSA_FAILED_NULL_WINDOW_SYSTEM: return MUG_MUCOSA_FAILED_NULL_WINDOW_SYSTEM; break;
					case MUCOSA_FAILED_MALLOC: return MUG_MUCOSA_FAILED_MALLOC; break;
					case MUCOSA_FAILED_UNKNOWN_WINDOW_ATTRIB: return MUG_MUCOSA_FAILED_UNKNOWN_WINDOW_ATTRIB; break;
					case MUCOSA_FAILED_REALLOC: return MUG_MUCOSA_FAILED_REALLOC; break;
					case MUCOSA_FAILED_UNKNOWN_GRAPHICS_API: return MUG_MUCOSA_FAILED_UNKNOWN_GRAPHICS_API; break;
					case MUCOSA_FAILED_UNSUPPORTED_GRAPHICS_API: return MUG_MUCOSA_FAILED_UNSUPPORTED_GRAPHICS_API; break;
					case MUCOSA_WIN32_FAILED_CONVERT_UTF8_TO_WCHAR: return MUG_MUCOSA_WIN32_FAILED_CONVERT_UTF8_TO_WCHAR; break;
					case MUCOSA_WIN32_FAILED_REGISTER_WINDOW_CLASS: return MUG_MUCOSA_WIN32_FAILED_REGISTER_WINDOW_CLASS; break;
					case MUCOSA_WIN32_FAILED_CREATE_WINDOW: return MUG_MUCOSA_WIN32_FAILED_CREATE_WINDOW; break;
					case MUCOSA_WIN32_FAILED_GET_WINDOW_ATTRIB: return MUG_MUCOSA_WIN32_FAILED_GET_WINDOW_ATTRIB; break;
					case MUCOSA_WIN32_FAILED_SET_WINDOW_ATTRIB: return MUG_MUCOSA_WIN32_FAILED_SET_WINDOW_ATTRIB; break;
					case MUCOSA_WIN32_FAILED_REGISTER_DUMMY_WGL_WINDOW_CLASS: return MUG_MUCOSA_WIN32_FAILED_REGISTER_DUMMY_WGL_WINDOW_CLASS; break;
					case MUCOSA_WIN32_FAILED_CREATE_DUMMY_WGL_WINDOW: return MUG_MUCOSA_WIN32_FAILED_CREATE_DUMMY_WGL_WINDOW; break;
					case MUCOSA_WIN32_FAILED_GET_DUMMY_WGL_PIXEL_FORMAT: return MUG_MUCOSA_WIN32_FAILED_GET_DUMMY_WGL_PIXEL_FORMAT; break;
					case MUCOSA_WIN32_FAILED_DESCRIBE_DUMMY_WGL_PIXEL_FORMAT: return MUG_MUCOSA_WIN32_FAILED_DESCRIBE_DUMMY_WGL_PIXEL_FORMAT; break;
					case MUCOSA_WIN32_FAILED_SET_DUMMY_WGL_PIXEL_FORMAT: return MUG_MUCOSA_WIN32_FAILED_SET_DUMMY_WGL_PIXEL_FORMAT; break;
					case MUCOSA_WIN32_FAILED_CREATE_DUMMY_WGL_CONTEXT: return MUG_MUCOSA_WIN32_FAILED_CREATE_DUMMY_WGL_CONTEXT; break;
					case MUCOSA_WIN32_FAILED_BIND_DUMMY_WGL_CONTEXT: return MUG_MUCOSA_WIN32_FAILED_BIND_DUMMY_WGL_CONTEXT; break;
					case MUCOSA_WIN32_FAILED_FIND_WGL_CREATE_CONTEXT_ATTRIBS: return MUG_MUCOSA_WIN32_FAILED_FIND_WGL_CREATE_CONTEXT_ATTRIBS; break;
					case MUCOSA_WIN32_FAILED_FIND_WGL_CHOOSE_PIXEL_FORMAT: return MUG_MUCOSA_WIN32_FAILED_FIND_WGL_CHOOSE_PIXEL_FORMAT; break;
					case MUCOSA_WIN32_FAILED_CHOOSE_WGL_PIXEL_FORMAT: return MUG_MUCOSA_WIN32_FAILED_CHOOSE_WGL_PIXEL_FORMAT; break;
					case MUCOSA_WIN32_FAILED_GET_WGL_PIXEL_FORMAT: return MUG_MUCOSA_WIN32_FAILED_GET_WGL_PIXEL_FORMAT; break;
					case MUCOSA_WIN32_FAILED_DESCRIBE_WGL_PIXEL_FORMAT: return MUG_MUCOSA_WIN32_FAILED_DESCRIBE_WGL_PIXEL_FORMAT; break;
					case MUCOSA_WIN32_FAILED_SET_WGL_PIXEL_FORMAT: return MUG_MUCOSA_WIN32_FAILED_SET_WGL_PIXEL_FORMAT; break;
					case MUCOSA_WIN32_FAILED_CREATE_WGL_CONTEXT: return MUG_MUCOSA_WIN32_FAILED_CREATE_WGL_CONTEXT; break;
					case MUCOSA_WIN32_FAILED_SET_WGL_CONTEXT: return MUG_MUCOSA_WIN32_FAILED_SET_WGL_CONTEXT; break;
					case MUCOSA_WIN32_FAILED_SWAP_WGL_BUFFERS: return MUG_MUCOSA_WIN32_FAILED_SWAP_WGL_BUFFERS; break;
					case MUCOSA_WIN32_FAILED_FIND_WGL_FUNCTION: return MUG_MUCOSA_WIN32_FAILED_FIND_WGL_FUNCTION; break;
					case MUCOSA_WIN32_FAILED_ASSOCIATE_IMM: return MUG_MUCOSA_WIN32_FAILED_ASSOCIATE_IMM; break;
					case MUCOSA_WIN32_FAILED_SET_COMPOSITION_WINDOW_POSITION: return MUG_MUCOSA_WIN32_FAILED_SET_COMPOSITION_WINDOW_POSITION; break;
					case MUCOSA_WIN32_FAILED_HOLD_CLIPBOARD: return MUG_MUCOSA_WIN32_FAILED_HOLD_CLIPBOARD; break;
					case MUCOSA_WIN32_FAILED_GET_CLIPBOARD_DATA: return MUG_MUCOSA_WIN32_FAILED_GET_CLIPBOARD_DATA; break;
					case MUCOSA_WIN32_FAILED_CONVERT_CLIPBOARD_DATA_FORMAT: return MUG_MUCOSA_WIN32_FAILED_CONVERT_CLIPBOARD_DATA_FORMAT; break;
					case MUCOSA_WIN32_FAILED_ALLOCATE_CLIPBOARD_DATA: return MUG_MUCOSA_WIN32_FAILED_ALLOCATE_CLIPBOARD_DATA; break;
					case MUCOSA_WIN32_FAILED_SET_CLIPBOARD_DATA: return MUG_MUCOSA_WIN32_FAILED_SET_CLIPBOARD_DATA; break;
				}
			}

		/* Names */

		#ifdef MUG_NAMES

			MUDEF const char* mug_result_get_name(mugResult result) {
				switch (result) {
					default: return "MU_UNKNOWN"; break;
					case MUG_SUCCESS: return "MUG_SUCCESS"; break;
					case MUG_FAILED_MALLOC: return "MUG_FAILED_MALLOC"; break;
					case MUG_FAILED_REALLOC: return "MUG_FAILED_REALLOC"; break;
					case MUG_UNKNOWN_GRAPHIC_SYSTEM: return "MUG_UNKNOWN_GRAPHIC_SYSTEM"; break;
					case MUG_UNKNOWN_OBJECT_TYPE: return "MUG_UNKNOWN_OBJECT_TYPE"; break;
					case MUG_UNKNOWN_OBJECT_MOD: return "MUG_UNKNOWN_OBJECT_MOD"; break;

					case MUG_GL_FAILED_LOAD: return "MUG_GL_FAILED_LOAD"; break;
					case MUG_GL_FAILED_COMPILE_VERTEX_SHADER: return "MUG_GL_FAILED_COMPILE_VERTEX_SHADER";
					case MUG_GL_FAILED_COMPILE_FRAGMENT_SHADER: return "MUG_GL_FAILED_COMPILE_FRAGMENT_SHADER"; break;
					case MUG_GL_FAILED_LINK_SHADERS: return "MUG_GL_FAILED_LINK_SHADERS"; break;
					case MUG_GL_FAILED_ALLOCATE_BUFFER: return "MUG_GL_FAILED_ALLOCATE_BUFFER"; break;
					case MUG_GL_FAILED_CREATE_BUFFER: return "MUG_GL_FAILED_CREATE_BUFFER"; break;
					case MUG_GL_FAILED_CREATE_VERTEX_ARRAY: return "MUG_GL_FAILED_CREATE_VERTEX_ARRAY"; break;
					case MUG_GL_FAILED_GENERATE_TEXTURE: return "MUG_GL_FAILED_GENERATE_TEXTURE"; break;

					case MUG_MUCOSA_FAILED_NULL_WINDOW_SYSTEM: return "MUG_MUCOSA_FAILED_NULL_WINDOW_SYSTEM"; break;
					case MUG_MUCOSA_FAILED_MALLOC: return "MUG_MUCOSA_FAILED_MALLOC"; break;
					case MUG_MUCOSA_FAILED_UNKNOWN_WINDOW_ATTRIB: return "MUG_MUCOSA_FAILED_UNKNOWN_WINDOW_ATTRIB"; break;
					case MUG_MUCOSA_FAILED_REALLOC: return "MUG_MUCOSA_FAILED_REALLOC"; break;
					case MUG_MUCOSA_FAILED_UNKNOWN_GRAPHICS_API: return "MUG_MUCOSA_FAILED_UNKNOWN_GRAPHICS_API"; break;
					case MUG_MUCOSA_FAILED_UNSUPPORTED_GRAPHICS_API: return "MUG_MUCOSA_FAILED_UNSUPPORTED_GRAPHICS_API"; break;
					case MUG_MUCOSA_WIN32_FAILED_CONVERT_UTF8_TO_WCHAR: return "MUG_MUCOSA_WIN32_FAILED_CONVERT_UTF8_TO_WCHAR"; break;
					case MUG_MUCOSA_WIN32_FAILED_REGISTER_WINDOW_CLASS: return "MUG_MUCOSA_WIN32_FAILED_REGISTER_WINDOW_CLASS"; break;
					case MUG_MUCOSA_WIN32_FAILED_CREATE_WINDOW: return "MUG_MUCOSA_WIN32_FAILED_CREATE_WINDOW"; break;
					case MUG_MUCOSA_WIN32_FAILED_GET_WINDOW_ATTRIB: return "MUG_MUCOSA_WIN32_FAILED_GET_WINDOW_ATTRIB"; break;
					case MUG_MUCOSA_WIN32_FAILED_SET_WINDOW_ATTRIB: return "MUG_MUCOSA_WIN32_FAILED_SET_WINDOW_ATTRIB"; break;
					case MUG_MUCOSA_WIN32_FAILED_REGISTER_DUMMY_WGL_WINDOW_CLASS: return "MUG_MUCOSA_WIN32_FAILED_REGISTER_DUMMY_WGL_WINDOW_CLASS"; break;
					case MUG_MUCOSA_WIN32_FAILED_CREATE_DUMMY_WGL_WINDOW: return "MUG_MUCOSA_WIN32_FAILED_CREATE_DUMMY_WGL_WINDOW"; break;
					case MUG_MUCOSA_WIN32_FAILED_GET_DUMMY_WGL_PIXEL_FORMAT: return "MUG_MUCOSA_WIN32_FAILED_GET_DUMMY_WGL_PIXEL_FORMAT"; break;
					case MUG_MUCOSA_WIN32_FAILED_DESCRIBE_DUMMY_WGL_PIXEL_FORMAT: return "MUG_MUCOSA_WIN32_FAILED_DESCRIBE_DUMMY_WGL_PIXEL_FORMAT"; break;
					case MUG_MUCOSA_WIN32_FAILED_SET_DUMMY_WGL_PIXEL_FORMAT: return "MUG_MUCOSA_WIN32_FAILED_SET_DUMMY_WGL_PIXEL_FORMAT"; break;
					case MUG_MUCOSA_WIN32_FAILED_CREATE_DUMMY_WGL_CONTEXT: return "MUG_MUCOSA_WIN32_FAILED_CREATE_DUMMY_WGL_CONTEXT"; break;
					case MUG_MUCOSA_WIN32_FAILED_BIND_DUMMY_WGL_CONTEXT: return "MUG_MUCOSA_WIN32_FAILED_BIND_DUMMY_WGL_CONTEXT"; break;
					case MUG_MUCOSA_WIN32_FAILED_FIND_WGL_CREATE_CONTEXT_ATTRIBS: return "MUG_MUCOSA_WIN32_FAILED_FIND_WGL_CREATE_CONTEXT_ATTRIBS"; break;
					case MUG_MUCOSA_WIN32_FAILED_FIND_WGL_CHOOSE_PIXEL_FORMAT: return "MUG_MUCOSA_WIN32_FAILED_FIND_WGL_CHOOSE_PIXEL_FORMAT"; break;
					case MUG_MUCOSA_WIN32_FAILED_CHOOSE_WGL_PIXEL_FORMAT: return "MUG_MUCOSA_WIN32_FAILED_CHOOSE_WGL_PIXEL_FORMAT"; break;
					case MUG_MUCOSA_WIN32_FAILED_GET_WGL_PIXEL_FORMAT: return "MUG_MUCOSA_WIN32_FAILED_GET_WGL_PIXEL_FORMAT"; break;
					case MUG_MUCOSA_WIN32_FAILED_DESCRIBE_WGL_PIXEL_FORMAT: return "MUG_MUCOSA_WIN32_FAILED_DESCRIBE_WGL_PIXEL_FORMAT"; break;
					case MUG_MUCOSA_WIN32_FAILED_SET_WGL_PIXEL_FORMAT: return "MUG_MUCOSA_WIN32_FAILED_SET_WGL_PIXEL_FORMAT"; break;
					case MUG_MUCOSA_WIN32_FAILED_CREATE_WGL_CONTEXT: return "MUG_MUCOSA_WIN32_FAILED_CREATE_WGL_CONTEXT"; break;
					case MUG_MUCOSA_WIN32_FAILED_SET_WGL_CONTEXT: return "MUG_MUCOSA_WIN32_FAILED_SET_WGL_CONTEXT"; break;
					case MUG_MUCOSA_WIN32_FAILED_SWAP_WGL_BUFFERS: return "MUG_MUCOSA_WIN32_FAILED_SWAP_WGL_BUFFERS"; break;
					case MUG_MUCOSA_WIN32_FAILED_FIND_WGL_FUNCTION: return "MUG_MUCOSA_WIN32_FAILED_FIND_WGL_FUNCTION"; break;
					case MUG_MUCOSA_WIN32_FAILED_ASSOCIATE_IMM: return "MUG_MUCOSA_WIN32_FAILED_ASSOCIATE_IMM"; break;
					case MUG_MUCOSA_WIN32_FAILED_SET_COMPOSITION_WINDOW_POSITION: return "MUG_MUCOSA_WIN32_FAILED_SET_COMPOSITION_WINDOW_POSITION"; break;
					case MUG_MUCOSA_WIN32_FAILED_HOLD_CLIPBOARD: return "MUG_MUCOSA_WIN32_FAILED_HOLD_CLIPBOARD"; break;
					case MUG_MUCOSA_WIN32_FAILED_GET_CLIPBOARD_DATA: return "MUG_MUCOSA_WIN32_FAILED_GET_CLIPBOARD_DATA"; break;
					case MUG_MUCOSA_WIN32_FAILED_CONVERT_CLIPBOARD_DATA_FORMAT: return "MUG_MUCOSA_WIN32_FAILED_CONVERT_CLIPBOARD_DATA_FORMAT"; break;
					case MUG_MUCOSA_WIN32_FAILED_ALLOCATE_CLIPBOARD_DATA: return "MUG_MUCOSA_WIN32_FAILED_ALLOCATE_CLIPBOARD_DATA"; break;
					case MUG_MUCOSA_WIN32_FAILED_SET_CLIPBOARD_DATA: return "MUG_MUCOSA_WIN32_FAILED_SET_CLIPBOARD_DATA"; break;
				}
			}

		#endif /* MUG_NAMES */

		MU_CPP_EXTERN_END
	#endif /* MUG_IMPLEMENTATION */
	#endif

	#endif /* MUSDK_MUG */

	// mutt
	// https://github.com/Muukid/muTrueType/tree/178b8fe6e73c5e1c51badf35de0a0090a62bf3d5
	#ifdef MUSDK_MUTT
	#define MUTT_IMPLEMENTATION

	// Debug inclusion:
	#ifdef MUSDK_INCLUDE_LIBS
	#include "muTrueType.h"
	// Standard inclusion:
	#else
	#ifdef MUTT_IMPLEMENTATION
		MU_CPP_EXTERN_START

		/* Lower-level API */

			/* Checksum logic */

				// Verfifies the checksum of a given table
				muBool mutt_VerifyTableChecksum(muByte* table, uint32_m length, uint32_m checksum) {
					// Initial vars
					uint32_m current_checksum = 0;
					uint32_m off = length%4;

					// Calculate end of table
					muByte* end = &table[length];

					// Go through each u32 in the table data
					while (table < end) {
						// Handle if we're cutting into the end of the table unevenly:
						if (off != 0 && (table+4) >= end) {
							// 16-bit
							if (off == 2) {
								current_checksum += MU_RBEU16(table) << 16;
							}
							// 24-bit
							else if (off == 3) {
								current_checksum += MU_RBEU24(table) << 8;
							}
							// 8-bit
							else {
								current_checksum += table[0] << 24;
							}
						}
						// Normal cut
						else {
							current_checksum += MU_RBEU32(table);
						}

						// Go to next u32
						table += 4;
					}

					// Verify that checksum matches
					if (checksum != current_checksum) {
						return MU_FALSE;
					}
					return MU_TRUE;
				}

			/* Table directory */

				// Loads the table directory
				// Note: if fails, still call mutt_DeloadTableDirectory
				muttResult mutt_LoadTableDirectory(muttDirectory* dir, muByte* data, uint64_m datalen) {
					muByte* orig_data = data;

					// Verify min. length
					if (datalen < 12) {
						return MUTT_INVALID_DIRECTORY_LENGTH;
					}

					// Read & verify sfntVersion
					if (MU_RBEU32(data) != 0x00010000) {
						return MUTT_INVALID_DIRECTORY_SFNT_VERSION;
					}

					// Read & verify numTables
					dir->num_tables = MU_RBEU16(data+4);
					// - Must be at least 9 for 9 required tables
					if (dir->num_tables < 9) {
						return MUTT_INVALID_DIRECTORY_NUM_TABLES;
					}

					// Verify length based on numTables
					if (datalen < (uint64_m)(12+(dir->num_tables*16))) {
						return MUTT_INVALID_DIRECTORY_LENGTH;
					}

					// Allocate table records
					dir->records = (muttTableRecord*)mu_malloc(sizeof(muttTableRecord)*dir->num_tables);
					if (!dir->records) {
						return MUTT_FAILED_MALLOC;
					}

					// Set up flag for noting which tables we found, which will be used
					// for ensuring that all required tables were found
					muttLoadFlags load_flags = 0;

					// Loop through each table record:
					data += 12;
					muttTableRecord* rec_end = dir->records+dir->num_tables;
					for (muttTableRecord* rec = dir->records; rec < rec_end; ++rec) {
						// Read tableTag
						// - table_tag_u8
						mu_memcpy(rec->table_tag_u8, data, 4);
						// - table_tag_u32
						rec->table_tag_u32 = MU_RBEU32(data);

						// Set load flag based on tag
						switch (rec->table_tag_u32) {
							default: break;
							case 0x6D617870: load_flags |= MUTT_LOAD_MAXP; break;
							case 0x68656164: load_flags |= MUTT_LOAD_HEAD; break;
							case 0x68686561: load_flags |= MUTT_LOAD_HHEA; break;
							case 0x686D7478: load_flags |= MUTT_LOAD_HMTX; break;
							case 0x6C6F6361: load_flags |= MUTT_LOAD_LOCA; break;
							case 0x706F7374: load_flags |= MUTT_LOAD_POST; break;
							case 0x6E616D65: load_flags |= MUTT_LOAD_NAME; break;
							case 0x676C7966: load_flags |= MUTT_LOAD_GLYF; break;
							case 0x636D6170: load_flags |= MUTT_LOAD_CMAP; break;
						}

						// Make sure this table tag is not a duplicate
						for (muttTableRecord* c = dir->records; c < rec; ++c) {
							if (rec->table_tag_u32 == c->table_tag_u32) {
								return MUTT_INVALID_DIRECTORY_RECORD_TABLE_TAG;
							}
						}

						// Read checksum
						rec->checksum = MU_RBEU32(data+4);

						// Read & verify offset
						rec->offset = MU_RBEU32(data+8);
						// - Make sure offset is in range
						if (rec->offset >= datalen) {
							return MUTT_INVALID_DIRECTORY_RECORD_OFFSET;
						}

						// Read & verify length
						rec->length = MU_RBEU32(data+12);
						// - Make sure length+offset is in range
						if (rec->offset+rec->length > datalen) {
							return MUTT_INVALID_DIRECTORY_RECORD_LENGTH;
						}

						// Verify checksum
						// Note: we're not performing this for head, its checksum HAS a checksum in it so it's weird
						if (rec->table_tag_u32 != 0x68656164 &&
							!mutt_VerifyTableChecksum(&orig_data[rec->offset], rec->length, rec->checksum)
						) {
							return MUTT_INVALID_DIRECTORY_RECORD_CHECKSUM;
						}

						// Increment data
						data += 16;
					}

					// Check that all required tables were loaded
					if ((load_flags & MUTT_LOAD_REQUIRED) != MUTT_LOAD_REQUIRED) {
						return MUTT_MISSING_DIRECTORY_RECORD_TABLE_TAGS;
					}

					return MUTT_SUCCESS;
				}

				// Deloads the table directory
				void mutt_DeloadTableDirectory(muttDirectory* dir) {
					// Free table records if they exist
					if (dir->records) {
						mu_free(dir->records);
					}
				}

			/* Basic tables */

				// Loads the maxp table
				muttResult mutt_LoadMaxp(muttFont* font, muByte* data, uint32_m datalen) {
					// Allocate maxp
					muttMaxp* maxp = (muttMaxp*)mu_malloc(sizeof(muttMaxp));
					if (!maxp) {
						return MUTT_FAILED_MALLOC;
					}

					// Verify min. length for version
					if (datalen < 4) {
						mu_free(maxp);
						return MUTT_INVALID_MAXP_LENGTH;
					}

					// Version high
					maxp->version_high = MU_RBEU16(data);
					if (maxp->version_high != 0x0001) {
						mu_free(maxp);
						return MUTT_INVALID_MAXP_VERSION;
					}
					// Version low
					maxp->version_low = MU_RBEU16(data+2);
					if (maxp->version_low != 0x0000) {
						mu_free(maxp);
						return MUTT_INVALID_MAXP_VERSION;
					}

					// Verify min. length
					if (datalen < 32) {
						mu_free(maxp);
						return MUTT_INVALID_MAXP_LENGTH;
					}

					// numGlyphs
					maxp->num_glyphs = MU_RBEU16(data+4);
					if (maxp->num_glyphs < 2) {
						mu_free(maxp);
						return MUTT_INVALID_MAXP_NUM_GLYPHS;
					}

					// maxPoints
					maxp->max_points = MU_RBEU16(data+6);
					// maxContours
					maxp->max_contours = MU_RBEU16(data+8);
					// maxCompositePoints
					maxp->max_composite_points = MU_RBEU16(data+10);
					// maxCompositeContours
					maxp->max_composite_contours = MU_RBEU16(data+12);
					// maxZones
					maxp->max_zones = MU_RBEU16(data+14);
					if (maxp->max_zones != 1 && maxp->max_zones != 2) {
						mu_free(maxp);
						return MUTT_INVALID_MAXP_MAX_ZONES;
					}
					// maxTwilightPoints
					maxp->max_twilight_points = MU_RBEU16(data+16);
					// maxStorage
					maxp->max_storage = MU_RBEU16(data+18);
					// maxFunctionDefs
					maxp->max_function_defs = MU_RBEU16(data+20);
					// maxInstructionDefs
					maxp->max_instruction_defs = MU_RBEU16(data+22);
					// maxStackElements
					maxp->max_stack_elements = MU_RBEU16(data+24);
					// maxSizeOfInstructions
					maxp->max_size_of_instructions = MU_RBEU16(data+26);
					// maxComponentElements
					maxp->max_component_elements = MU_RBEU16(data+28);
					// maxComponentDepth
					maxp->max_component_depth = MU_RBEU16(data+30);

					font->maxp = maxp;
					return MUTT_SUCCESS;
				}

				// Loads the head table
				muttResult mutt_LoadHead(muttFont* font, muByte* data, uint32_m datalen) {
					// Allocate head
					muttHead* head = (muttHead*)mu_malloc(sizeof(muttHead));
					if (!head) {
						return MUTT_FAILED_MALLOC;
					}

					// Verify min. length for version
					if (datalen < 4) {
						mu_free(head);
						return MUTT_INVALID_HEAD_LENGTH;
					}

					// Verify version
					if (MU_RBEU16(data) != 1 || MU_RBEU16(data+2) != 0) {
						mu_free(head);
						return MUTT_INVALID_HEAD_VERSION;
					}

					// Verify min. length for rest of table
					if (datalen < 54) {
						mu_free(head);
						return MUTT_INVALID_HEAD_LENGTH;
					}

					// fontRevision high+low
					head->font_revision_high = MU_RBES16(data+4);
					head->font_revision_low  = MU_RBEU16(data+6);
					// checksumAdjustment
					head->checksum_adjustment = MU_RBEU32(data+8);

					// magicNumber
					if (MU_RBEU32(data+12) != 0x5F0F3CF5) {
						mu_free(head);
						return MUTT_INVALID_HEAD_MAGIC_NUMBER;
					}

					// flags
					head->flags = MU_RBEU16(data+16);

					// unitsPerEm
					head->units_per_em = MU_RBEU16(data+18);
					if (head->units_per_em < 16 || head->units_per_em > 16384) {
						mu_free(head);
						return MUTT_INVALID_HEAD_UNITS_PER_EM;
					}

					// created + modified
					head->created = MU_RBES64(data+20);
					head->modified = MU_RBES64(data+28);

					// xMin + yMin
					head->x_min = MU_RBES16(data+36);
					if (head->x_min < -16384 || head->x_min > 16383) {
						mu_free(head);
						return MUTT_INVALID_HEAD_X_MIN_COORDINATES;
					}
					head->y_min = MU_RBES16(data+38);
					if (head->y_min < -16384 || head->y_min > 16383) {
						mu_free(head);
						return MUTT_INVALID_HEAD_Y_MIN_COORDINATES;
					}
					// xMax + yMax
					head->x_max = MU_RBES16(data+40);
					if (head->x_max < -16384 || head->x_max > 16383) {
						mu_free(head);
						return MUTT_INVALID_HEAD_X_MAX_COORDINATES;
					}
					head->y_max = MU_RBES16(data+42);
					if (head->y_max < -16384 || head->y_max > 16383) {
						mu_free(head);
						return MUTT_INVALID_HEAD_Y_MAX_COORDINATES;
					}
					// + Verify min/max
					if (head->x_min > head->x_max) {
						mu_free(head);
						return MUTT_INVALID_HEAD_X_MIN_MAX;
					}
					if (head->y_min > head->y_max) {
						mu_free(head);
						return MUTT_INVALID_HEAD_Y_MIN_MAX;
					}

					// macStyle
					head->mac_style = MU_RBEU16(data+44);
					// lowestRecPPEM
					head->lowest_rec_ppem = MU_RBEU16(data+46);
					// fontDirectionHint
					head->font_direction_hint = MU_RBES16(data+48);
					// indexToLocFormat
					head->index_to_loc_format = MU_RBES16(data+50);
					if (head->index_to_loc_format != 0 && head->index_to_loc_format != 1) {
						mu_free(head);
						return MUTT_INVALID_HEAD_INDEX_TO_LOC_FORMAT;
					}

					// glyphDataFormat
					if (MU_RBES16(data+52) != 0) {
						mu_free(head);
						return MUTT_INVALID_HEAD_GLYPH_DATA_FORMAT;
					}

					font->head = head;
					return MUTT_SUCCESS;
				}

				// Loads the hhea table
				// Req: maxp
				muttResult mutt_LoadHhea(muttFont* font, muByte* data, uint32_m datalen) {
					// Allocate hhea
					muttHhea* hhea = (muttHhea*)mu_malloc(sizeof(muttHhea));
					if (!hhea) {
						return MUTT_FAILED_MALLOC;
					}

					// Verify min. length for version
					if (datalen < 4) {
						mu_free(hhea);
						return MUTT_INVALID_HHEA_LENGTH;
					}

					// Verify version
					if (MU_RBEU16(data) != 1 || MU_RBEU16(data+2) != 0) {
						mu_free(hhea);
						return MUTT_INVALID_HHEA_VERSION;
					}

					// Verify min. length
					if (datalen < 36) {
						mu_free(hhea);
						return MUTT_INVALID_HHEA_LENGTH;
					}

					// ascender + descender
					hhea->ascender = MU_RBES16(data+4);
					hhea->descender = MU_RBES16(data+6);
					// lineGap
					hhea->line_gap = MU_RBES16(data+8);
					// advanceWidthMax
					hhea->advance_width_max = MU_RBEU16(data+10);
					// min(Left/Right)SideBearing
					hhea->min_left_side_bearing = MU_RBES16(data+12);
					hhea->min_right_side_bearing = MU_RBES16(data+14);
					// xMaxExtent
					hhea->x_max_extent = MU_RBES16(data+16);
					// caretSlope(Rise/Run)
					hhea->caret_slope_rise = MU_RBES16(data+18);
					hhea->caret_slope_run = MU_RBES16(data+20);
					// caretOffset
					hhea->caret_offset = MU_RBES16(data+22);

					// metricDataFormat
					if (MU_RBES16(data+32) != 0) {
						mu_free(hhea);
						return MUTT_INVALID_HHEA_METRIC_DATA_FORMAT;
					}

					// numberOfHMetrics
					hhea->number_of_hmetrics = MU_RBEU16(data+34);
					// - numGlyphs-numberOfHMetrics must be valid for leftSideBearings in hmtx
					if (hhea->number_of_hmetrics > font->maxp->num_glyphs) {
						mu_free(hhea);
						return MUTT_INVALID_HHEA_NUMBER_OF_HMETRICS;
					}

					font->hhea = hhea;
					return MUTT_SUCCESS;
				}

			/* Allocated tables */

				// Loads the hmtx table
				// Req: maxp, hhea
				void mutt_DeloadHmtx(muttHmtx* hmtx);
				muttResult mutt_LoadHmtx(muttFont* font, muByte* data, uint32_m datalen) {
					// Verify length
					if (datalen <
						// hMetrics
						(uint64_m)(4*font->hhea->number_of_hmetrics)
						// leftSideBearings
						+(uint64_m)(2*(font->maxp->num_glyphs-font->hhea->number_of_hmetrics))
					) {
						return MUTT_INVALID_HMTX_LENGTH;
					}

					// Allocate
					muttHmtx* hmtx = (muttHmtx*)mu_malloc(sizeof(muttHmtx));
					if (!hmtx) {
						// mutt_DeloadHmtx(hmtx);
						return MUTT_FAILED_MALLOC;
					}
					hmtx->hmetrics = 0;
					hmtx->left_side_bearings = 0;

					// Allocate hMetrics
					if (font->hhea->number_of_hmetrics == 0) {
						hmtx->hmetrics = 0;
					} else {
						hmtx->hmetrics = (muttLongHorMetric*)mu_malloc(sizeof(muttLongHorMetric)*font->hhea->number_of_hmetrics);
						if (!hmtx->hmetrics) {
							mutt_DeloadHmtx(hmtx);
							return MUTT_FAILED_MALLOC;
						}
					}

					// Allocate leftSideBearings
					uint16_m lsb_len = font->maxp->num_glyphs - font->hhea->number_of_hmetrics;
					if (lsb_len == 0) {
						hmtx->left_side_bearings = 0;
					} else {
						hmtx->left_side_bearings = (int16_m*)mu_malloc(lsb_len*2);
						if (!hmtx->left_side_bearings) {
							mutt_DeloadHmtx(hmtx);
							return MUTT_FAILED_MALLOC;
						}
					}

					// Loop through each hMetrics index
					for (uint16_m h = 0; h < font->hhea->number_of_hmetrics; ++h) {
						// advanceWidth
						hmtx->hmetrics[h].advance_width = MU_RBEU16(data);
						// lsb
						hmtx->hmetrics[h].lsb = MU_RBES16(data+2);
						// Increment data
						data += 4;
					}

					// Loop through each leftSideBearings index
					for (uint16_m l = 0; l < lsb_len; ++l) {
						// leftSideBearings[l]
						hmtx->left_side_bearings[l] = MU_RBES16(data);
						// Increment data
						data += 2;
					}

					font->hmtx = hmtx;
					return MUTT_SUCCESS;
				}

				// Deloads the hmtx table
				void mutt_DeloadHmtx(muttHmtx* hmtx) {
					if (hmtx) {
						if (hmtx->hmetrics) {
							mu_free(hmtx->hmetrics);
						}
						if (hmtx->left_side_bearings) {
							mu_free(hmtx->left_side_bearings);
						}
						mu_free(hmtx);
					}
				}

				// Loads the loca table
				// Req: maxp, head, glyf
				void mutt_DeloadLoca(muttLoca* loca);
				muttResult mutt_LoadLoca(muttFont* font, muByte* data, uint32_m datalen) {
					// Allocate
					muttLoca* loca = (muttLoca*)mu_malloc(sizeof(muttLoca));
					if (!loca) {
						// mutt_DeloadLoca(loca);
						return MUTT_FAILED_MALLOC;
					}
					mu_memset(loca, 0, sizeof(muttLoca));

					// Get offsets count
					uint32_m offsets = ((uint32_m)font->maxp->num_glyphs) + 1;

					// Allocate offsets
					// - 16-bit
					if (font->head->index_to_loc_format == MUTT_OFFSET_16) {
						// - Verify 16-bit length
						if (datalen < offsets*2) {
							mutt_DeloadLoca(loca);
							return MUTT_INVALID_LOCA_LENGTH;
						}
						// - Allocate
						loca->offsets16 = (uint16_m*)mu_malloc(offsets*2);
						if (!loca->offsets16) {
							mutt_DeloadLoca(loca);
							return MUTT_FAILED_MALLOC;
						}
					}
					// 32-bit
					else {
						// - Verify 32-bit length
						if (datalen < offsets*4) {
							mutt_DeloadLoca(loca);
							return MUTT_INVALID_LOCA_LENGTH;
						}
						// - Allocate
						loca->offsets32 = (uint32_m*)mu_malloc(offsets*4);
						if (!loca->offsets32) {
							mutt_DeloadLoca(loca);
							return MUTT_FAILED_MALLOC;
						}
					}

					// Loop through each offset
					// - 16-bit
					if (font->head->index_to_loc_format == MUTT_OFFSET_16) {
						for (uint32_m o = 0; o < offsets; ++o) {
							// Read value and go to next
							loca->offsets16[o] = MU_RBEU16(data);
							data += 2;
							// Verify incremental order
							if (o > 0 && loca->offsets16[o-1] > loca->offsets16[o]) {
								mutt_DeloadLoca(loca);
								return MUTT_INVALID_LOCA_OFFSET;
							}
							// Verify offset is within range of glyf
							uint32_m offset = ((uint32_m)loca->offsets16[o]) * 2;
							if (offset > font->glyf->len) {
								mutt_DeloadLoca(loca);
								return MUTT_INVALID_LOCA_OFFSET;
							}
						}
					}
					// - 32-bit
					else {
						for (uint32_m o = 0; o < offsets; ++o) {
							// Read value and go to next
							loca->offsets32[o] = MU_RBEU32(data);
							data += 4;
							// Verify incremental order
							if (o > 0 && loca->offsets32[o-1] > loca->offsets32[o]) {
								mutt_DeloadLoca(loca);
								return MUTT_INVALID_LOCA_OFFSET;
							}
							// Verify offset is within range of glyf
							if (loca->offsets32[o] > font->glyf->len) {
								mutt_DeloadLoca(loca);
								return MUTT_INVALID_LOCA_OFFSET;
							}
						}
					}

					font->loca = loca;
					return MUTT_SUCCESS;
				}

				// Deloads the loca table
				void mutt_DeloadLoca(muttLoca* loca) {
					if (loca) {
						// I THINK this works with 32 as well...
						if (loca->offsets16) {
							mu_free(loca->offsets16);
						}
						mu_free(loca);
					}
				}

				// Loads the name table
				void mutt_DeloadName(muttName* name);
				muttResult mutt_LoadName(muttFont* font, muByte* data, uint32_m datalen) {
					// Allocate
					muttName* name = (muttName*)mu_malloc(sizeof(muttName));
					if (!name) {
						return MUTT_FAILED_MALLOC;
					}
					mu_memset(name, 0, sizeof(muttName));

					// Verify min. length for version
					if (datalen < 2) {
						mutt_DeloadName(name);
						return MUTT_INVALID_NAME_LENGTH;
					}

					// Verify version
					name->version = MU_RBEU16(data);
					if (name->version != 0 && name->version != 1) {
						mutt_DeloadName(name);
						return MUTT_INVALID_NAME_VERSION;
					}

					// Verify min. length for version, count, and storage offset
					if (datalen < 6) {
						mutt_DeloadName(name);
						return MUTT_INVALID_NAME_LENGTH;
					}

					// Read count + storageOffset
					name->count = MU_RBEU16(data+2);
					uint16_m storage_offset = MU_RBEU16(data+4);
					// Verify length for storage offset
					if (storage_offset > datalen) {
						mutt_DeloadName(name);
						return MUTT_INVALID_NAME_STORAGE_OFFSET;
					}
					// Verify length for nameRecord and storage
					uint32_m req = 6+(name->count*12)+(datalen-storage_offset);
					if (datalen < req) {
						mutt_DeloadName(name);
						return MUTT_INVALID_NAME_LENGTH;
					}

					// Allocate and fill storage (if necessary)
					uint32_m storage_len = datalen-storage_offset;
					if (storage_len) {
						name->string_data = (muByte*)mu_malloc(storage_len);
						if (!name->string_data) {
							mutt_DeloadName(name);
							return MUTT_FAILED_MALLOC;
						}
						mu_memcpy(name->string_data, &data[storage_offset], storage_len);
					}

					// Move up data
					data += 6;

					// If we have name records:
					if (name->count) {
						// Allocate name records
						name->name_records = (muttNameRecord*)mu_malloc(sizeof(muttNameRecord)*((size_m)name->count));
						if (!name->name_records) {
							mutt_DeloadName(name);
							return MUTT_FAILED_MALLOC;
						}

						// Loop through each name record
						for (uint16_m r = 0; r < name->count; ++r) {
							muttNameRecord* rp = &name->name_records[r];

							// Basic numbers
							rp->platform_id = MU_RBEU16(data);
							rp->encoding_id = MU_RBEU16(data+2);
							rp->language_id = MU_RBEU16(data+4);
							rp->name_id = MU_RBEU16(data+6);
							rp->length = MU_RBEU16(data+8);

							// Get & verify offset + length
							uint16_m offset = MU_RBEU16(data+10);
							if (offset+rp->length > storage_len) {
								mutt_DeloadName(name);
								return MUTT_INVALID_NAME_LENGTH_OFFSET;
							}

							// Get string
							rp->string = (storage_len) ?(name->string_data+offset) :(0);
							// Increment data
							data += 12;
						}
					}

					// For verson 1:
					if (name->version == 1) {
						// Verify length for langTagCount
						req += 2;
						if (datalen < req) {
							mutt_DeloadName(name);
							return MUTT_INVALID_NAME_LENGTH;
						}
						// Get langTagCount
						name->lang_tag_count = MU_RBEU16(data);
						data += 2;
						// Verify length for langTagRecord
						req += name->lang_tag_count*4;
						if (datalen < req) {
							mutt_DeloadName(name);
							return MUTT_INVALID_NAME_LENGTH;
						}
					}

					// If we have lang tags:
					if (name->lang_tag_count) {
						// Allocate lang tags
						name->lang_tag_records = (muttLangTagRecord*)mu_malloc(sizeof(muttLangTagRecord)*((size_m)name->lang_tag_count));
						if (!name->lang_tag_records) {
							mutt_DeloadName(name);
							return MUTT_FAILED_MALLOC;
						}

						// Loop through each lang tag record
						for (uint16_m l = 0; l < name->lang_tag_count; ++l) {
							muttLangTagRecord* lr = &name->lang_tag_records[l];

							// Read length
							lr->length = MU_RBEU16(data);

							// Get & verify offset + length
							uint16_m offset = MU_RBEU16(data+2);
							if (offset+lr->length > storage_len) {
								mutt_DeloadName(name);
								return MUTT_INVALID_NAME_LENGTH_OFFSET;
							}

							// Get lang tag
							lr->lang_tag = (storage_len) ?(name->string_data+offset) :(0);
							// Increment data
							data += 4;
						}
					}

					font->name = name;
					return MUTT_SUCCESS;
				}

				// Frees all allocated data for name
				void mutt_DeloadName(muttName* name) {
					if (name) {
						if (name->name_records) {
							mu_free(name->name_records);
						}
						if (name->lang_tag_records) {
							mu_free(name->lang_tag_records);
						}
						if (name->string_data) {
							mu_free(name->string_data);
						}
						mu_free(name);
					}
				}

				// Loads the glyf table
				void mutt_DeloadGlyf(muttGlyf* glyf);
				muttResult mutt_LoadGlyf(muttFont* font, muByte* data, uint32_m datalen) {
					// Allocate glyf
					muttGlyf* glyf = (muttGlyf*)mu_malloc(sizeof(muttGlyf));
					if (!glyf) {
						return MUTT_FAILED_MALLOC;
					}
					mu_memset(glyf, 0, sizeof(muttGlyf));

					// Get glyf length
					glyf->len = datalen;

					if (glyf->len) {
						// Allocate glyf data
						glyf->data = (muByte*)mu_malloc(glyf->len);
						if (!glyf->data) {
							mutt_DeloadGlyf(glyf);
							return MUTT_FAILED_MALLOC;
						}

						// Copy over table memory
						mu_memcpy(glyf->data, data, glyf->len);
					}

					font->glyf = glyf;
					return MUTT_SUCCESS;
				}

				// Frees all allocated data for glyf
				void mutt_DeloadGlyf(muttGlyf* glyf) {
					if (glyf) {
						if (glyf->data) {
							mu_free(glyf->data);
						}
						mu_free(glyf);
					}
				}

			/* Cmap stuff */

				/* Format 0 */

					// Loads format 0
					muttResult mutt_f0Load(muttFont* font, muttCmap0* f0, muByte* data, uint32_m datalen) {
						// Verify length for everything
						if (datalen < 262) {
							return MUTT_INVALID_CMAP0_LENGTH;
						}

						// length
						if (MU_RBEU16(data+2) > datalen) {
							return MUTT_INVALID_CMAP0_LENGTH;
						}
						// language
						f0->language = MU_RBEU16(data+4);
						// glyphIdArray
						mu_memcpy(f0->glyph_ids, data+6, 256);

						return MUTT_SUCCESS; if (font) {}
					}

					// Codepoint -> glyph ID
					MUDEF uint16_m mutt_cmap0_get_glyph(muttFont* font, muttCmap0* f0, uint8_m codepoint) {
						// Get value for glyph ID from array
						// (codepoint will always be a valid index since u8)
						uint16_m glyph = (uint16_m)f0->glyph_ids[codepoint];

						// Return if valid; 0 if invalid
						return (glyph < font->maxp->num_glyphs) ?(glyph) :(0);
					}

					// Glyph ID -> codepoint
					MUDEF uint8_m mutt_cmap0_get_codepoint(muttFont* font, muttCmap0* f0, uint16_m glyph) {
						// Loop through each element in glyph ID array
						for (uint16_m i = 0; i < 256; ++i) {
							// If glyph ID is found:
							if (f0->glyph_ids[i] == glyph) {
								// Return index (codepoint)
								return i;
							}
						}

						// No glyph ID equivalent found; return nada
						return 0; if (font) {}
					}

				/* Format 4 */

					// Loads format 4
					void mutt_f4Deload(muttCmap4* f4);
					muttResult mutt_f4Load(muttFont* font, muttCmap4* f4, muByte* data, uint32_m datalen) {
						// Verify length for format...rangeShift
						// 32 is ok because length is u16 in subtable
						uint32_m req = 14;
						if (datalen < req) {
							return MUTT_INVALID_CMAP4_LENGTH;
						}
						// length
						uint16_m length = MU_RBEU16(data+2);
						if (length > datalen || length < req) {
							return MUTT_INVALID_CMAP4_LENGTH;
						}

						// language
						f4->language = MU_RBEU16(data+4);

						// segCountX2
						f4->seg_count = MU_RBEU16(data+6);
						if ((f4->seg_count%2) != 0) {
							return MUTT_INVALID_CMAP4_SEG_COUNT_X2;
						}
						f4->seg_count /= 2;
						// - Edge case for seg_count being 0
						if (f4->seg_count == 0) {
							f4->seg = 0;
							f4->glyph_ids = 0;
							return MUTT_SUCCESS;
						}

						// Verify length for endCode...idRangeOffset
						req += 2+(8*f4->seg_count);
						if (length < req) {
							return MUTT_INVALID_CMAP4_LENGTH;
						}

						// glyphIdArray length
						uint16_m glyph_id_array_len = length-req;
						if ((glyph_id_array_len%2) != 0) {
							//return MUTT_INVALID_CMAP4_GLYPH_ID_ARRAY_LENGTH;
							// Subtracting 1 instead, just feels better...
							glyph_id_array_len -= 1;
						}
						glyph_id_array_len /= 2;

						// Allocate segments
						f4->seg = (muttCmap4Segment*)mu_malloc(f4->seg_count*sizeof(muttCmap4Segment));
						if (!f4->seg) {
							return MUTT_FAILED_MALLOC;
						}
						// Allocate glyph IDs
						f4->glyph_ids = (uint16_m*)mu_malloc(glyph_id_array_len*2);
						if (!f4->glyph_ids) {
							mu_free(f4->seg);
							return MUTT_FAILED_MALLOC;
						}

						// Placeholder values for progress in data:
						muByte* end_code = data+14;
						muByte* start_code = end_code+(f4->seg_count*2)+2;
						muByte* id_delta = start_code+(f4->seg_count*2);
						muByte* id_range_offset = id_delta+(f4->seg_count*2);
						muByte* glyph_id_array = id_range_offset+(f4->seg_count*2);

						// Loop through each glyphIdArray element
						for (uint16_m g = 0; g < glyph_id_array_len; ++g) {
							// Read value
							f4->glyph_ids[g] = MU_RBEU16(glyph_id_array);
							// Move to next element
							glyph_id_array += 2;
						}

						// Loop through each segment
						for (uint16_m s = 0; s < f4->seg_count; ++s) {
							// Get segment
							muttCmap4Segment* ps = &f4->seg[s];

							// endCode
							ps->end_code = MU_RBEU16(end_code);
							// - Verify incremental
							if (s > 0) {
								if (ps->end_code <= (ps-1)->end_code) {
									mutt_f4Deload(f4);
									return MUTT_INVALID_CMAP4_END_CODE;
								}
							}
							// - Verify last = 0xFFFF
							if (s+1 == f4->seg_count && ps->end_code != 0xFFFF) {
								mutt_f4Deload(f4);
								return MUTT_INVALID_CMAP4_LAST_END_CODE;
							}

							// startCode
							ps->start_code = MU_RBEU16(start_code);
							// - Verify startCode <= endCode
							if (ps->start_code > ps->end_code) {
								mutt_f4Deload(f4);
								return MUTT_INVALID_CMAP4_START_CODE;
							}

							// idDelta
							ps->id_delta = MU_RBES16(id_delta);

							// idRangeOffset
							ps->id_range_offset = ps->id_range_offset_orig = MU_RBEU16(id_range_offset);
							// Further verification is only cared about if idRangeOffset is not 0,
							// since logic with it is only performed under that circumstance
							if (ps->id_range_offset != 0) {
								// - Divide by 2 to account for 2-byte-per offset
								if ((ps->id_range_offset%2) != 0) {
									// Not divisible by 2 :L
									mutt_f4Deload(f4);
									return MUTT_INVALID_CMAP4_ID_RANGE_OFFSET;
								}
								ps->id_range_offset /= 2;
								// - Subtract seg_count-s to counteract offset being relative to &idRangeOffset[s]
								//   (only applies if idRangeOffset is not 0)
								if (ps->id_range_offset < f4->seg_count-s) {
									// Offset is out of range (before glyphIdArray)
									mutt_f4Deload(f4);
									return MUTT_INVALID_CMAP4_ID_RANGE_OFFSET;
								}
								ps->id_range_offset -= f4->seg_count-s;
								// - Ensure within range of glyphIdArray at max distance from it
								if (ps->id_range_offset + (ps->start_code - ps->end_code) >= glyph_id_array_len) {
									mutt_f4Deload(f4);
									return MUTT_INVALID_CMAP4_ID_RANGE_OFFSET;
								}

								// Calculate starting glyph ID
								ps->start_glyph_id = f4->glyph_ids[ps->id_range_offset];
								// - If it's not 0, idDelta logic is performed
								if (ps->start_glyph_id != 0) {
									ps->start_glyph_id = mutt_id_delta(ps->start_glyph_id, ps->id_delta);
								}
								// + Ending glyph ID
								ps->end_glyph_id = f4->glyph_ids[ps->id_range_offset+(ps->start_code-ps->end_code)];
								if (ps->end_glyph_id != 0) {
									ps->end_glyph_id = mutt_id_delta(ps->end_glyph_id, ps->id_delta);
								}
							}
							// If idRangeOffset IS 0, calculating starting/ending glyph ID is just idDelta
							else {
								ps->start_glyph_id = mutt_id_delta(ps->start_code, ps->id_delta);
								ps->end_glyph_id = mutt_id_delta(ps->end_code, ps->id_delta);
							}
							// Now, we can be sure that for any segment:
							// glyphIdArray[idRangeOffset+(codepoint-startCode)]
							// is a valid index into glyphIdArray (besides idRangeOffset==0).
							// idDelta logic goes unverified at this point.

							// Increment relevant placeholder values
							end_code += 2;
							start_code += 2;
							id_delta += 2;
							id_range_offset += 2;
						}

						return MUTT_SUCCESS; if (font) {}
					}

					// Deloads format 4
					void mutt_f4Deload(muttCmap4* f4) {
						if (f4) {
							if (f4->seg) {
								mu_free(f4->seg);
							}
							if (f4->glyph_ids) {
								mu_free(f4->glyph_ids);
							}
							// (Purposely not freeing f4)
						}
					}

					// Codepoint -> glyph ID
					MUDEF uint16_m mutt_cmap4_get_glyph(muttFont* font, muttCmap4* f4, uint16_m codepoint) {
						// Loop through each segment
						for (uint16_m s = 0; s < f4->seg_count; ++s) {
							// Continue on if the codepoint is not within this segment
							if (codepoint < f4->seg[s].start_code || codepoint > f4->seg[s].end_code) {
								continue;
							}
							// If we're here, the segment contains our codepoint
							uint16_m glyph;
							muttCmap4Segment* seg = &f4->seg[s];

							// If idRangeOffset is 0, we're only performing delta logic
							if (seg->id_range_offset_orig == 0) {
								glyph = mutt_id_delta(codepoint, seg->id_delta);
							}
							// If idRangeOffset isn't 0, we must index into glyphIdArray
							else {
								// (This is verified to be valid in mutt_f4Load)
								glyph = f4->glyph_ids[seg->id_range_offset+(codepoint-seg->start_code)];
								// + idDelta if not 0
								if (glyph != 0) {
									glyph = mutt_id_delta(glyph, seg->id_delta);
								}
							}

							// Return glyph if it's valid and not 0
							if (glyph < font->maxp->num_glyphs && glyph != 0) {
								return glyph;
							}
						}

						// Codepoint does not lie within a defined segment
						return 0;
					}

					// Glyph ID -> codepoint
					MUDEF uint16_m mutt_cmap4_get_codepoint(muttFont* font, muttCmap4* f4, uint16_m glyph) {
						// Loop through each segment
						for (uint16_m s = 0; s < f4->seg_count; ++s) {
							// If segment is fully continuous:
							if (f4->seg[s].start_glyph_id <= f4->seg[s].end_glyph_id) {
								// If glyph ID is within this range:
								if (glyph >= f4->seg[s].start_glyph_id && glyph <= f4->seg[s].end_glyph_id) {
									// Just return character code based on offset into glyph ID range
									return f4->seg[s].start_code + (glyph - f4->seg[s].start_glyph_id);
								}
							}
							// For non-continuous segments:
							else {
								// @TODO
							}
						}

						// Glyph ID is not given in any segment
						return 0; if (font) {}
					}

				/* Format 12 */

					// Loads format 12
					void mutt_f12Deload(muttCmap12* f12);
					muttResult mutt_f12Load(muttFont* font, muttCmap12* f12, muByte* data, uint32_m datalen) {
						// Verify length for format...numGroups
						uint64_m req = 16;
						if (datalen < req) {
							return MUTT_INVALID_CMAP12_LENGTH;
						}
						// length
						uint32_m length = MU_RBEU32(data+4);
						if (length > datalen || length < req) {
							return MUTT_INVALID_CMAP12_LENGTH;
						}

						// language
						f12->language = MU_RBEU32(data+8);

						// numGroups
						f12->num_groups = MU_RBEU32(data+12);
						// Exit early if no groups
						if (f12->num_groups == 0) {
							f12->groups = 0;
							return MUTT_SUCCESS;
						}

						// Verify length for groups
						req += f12->num_groups*12;
						if (length < req) {
							return MUTT_INVALID_CMAP12_LENGTH;
						}
						// Allocate groups
						f12->groups = (muttCmap12Group*)mu_malloc(sizeof(muttCmap12Group)*f12->num_groups);
						if (!f12->groups) {
							return MUTT_FAILED_MALLOC;
						}

						// Loop through each group
						data += 16;
						for (uint32_m g = 0; g < f12->num_groups; ++g) {
							// startCharCode
							f12->groups[g].start_char_code = MU_RBEU32(data);
							// endCharCode
							f12->groups[g].end_char_code = MU_RBEU32(data+4);

							// Logic for groups that have a previous group:
							if (g > 0) {
								// Verify incremental startCharCode ordering
								if (f12->groups[g].start_char_code <= f12->groups[g-1].start_char_code) {
									mutt_f12Deload(f12);
									return MUTT_INVALID_CMAP12_START_CHAR_CODE;
								}
								// Verify [-1].end < [0].start
								if (f12->groups[g-1].end_char_code >= f12->groups[g].start_char_code) {
									mutt_f12Deload(f12);
									return MUTT_INVALID_CMAP12_END_CHAR_CODE;
								}
							}

							// startGlyphID
							f12->groups[g].start_glyph_id = MU_RBEU32(data+8);
							// Increment data to next group
							data += 12;
						}

						return MUTT_SUCCESS; if (font) {}
					}

					// Deloads format 12
					void mutt_f12Deload(muttCmap12* f12) {
						if (f12->groups) {
							mu_free(f12->groups);
							// (Purposely not freeing f12)
						}
					}

					// Codepoint -> glyph ID
					MUDEF uint16_m mutt_cmap12_get_glyph(muttFont* font, muttCmap12* f12, uint32_m codepoint) {
						// Loop through each group
						for (uint32_m g = 0; g < f12->num_groups; ++g) {
							// Pass if codepoint is not in range
							if (codepoint < f12->groups[g].start_char_code || codepoint > f12->groups[g].end_char_code) {
								continue;
							}

							// Get glyph ID via start glyph ID and distance from startCharCode
							uint32_m glyph = f12->groups[g].start_glyph_id + (codepoint - f12->groups[g].start_char_code);
							// Return if non-zero and in range
							if (glyph != 0 && glyph < font->maxp->num_glyphs) {
								return (uint16_m)glyph;
							}
						}

						// No good glyph value could be found
						return 0;
					}

					// Glyph ID -> codepoint
					MUDEF uint32_m mutt_cmap12_get_codepoint(muttFont* font, muttCmap12* f12, uint16_m glyph) {
						// Loop through each group
						for (uint32_m g = 0; g < f12->num_groups; ++g) {
							// Pass if glyph is not in range
							if (
								glyph < f12->groups[g].start_glyph_id || 
								glyph > f12->groups[g].start_glyph_id + (f12->groups[g].end_char_code - f12->groups[g].start_char_code)
							) {
								continue;
							}

							// Return codepoint via startCharCode and distance from startGlyphID
							return f12->groups[g].start_char_code + (f12->groups[g].start_glyph_id - glyph);
						}

						// Glyph value was never specified in a range
						return 0; if (font) {}
					}

				/* All formats */

					// Loads a given format
					muttResult mutt_LoadCmapFormat(muttFont* font, uint16_m format, muttCmapFormat* pformat, muByte* data, uint32_m datalen) {
						switch (format) {
							// Unsupported format
							default: {
								return MUTT_INVALID_CMAP_ENCODING_RECORD_FORMAT;
							} break;

							// Format 0
							case 0: {
								// Allocate
								pformat->f0 = (muttCmap0*)mu_malloc(sizeof(muttCmap0));
								if (!pformat->f0) {
									return MUTT_FAILED_MALLOC;
								}
								// Load
								muttResult res = mutt_f0Load(font, pformat->f0, data, datalen);
								if (mutt_result_is_fatal(res)) {
									mu_free(pformat->f0);
									pformat->f0 = 0;
								}
								return res;
							} break;

							// Format 4
							case 4: {
								// Allocate
								pformat->f4 = (muttCmap4*)mu_malloc(sizeof(muttCmap4));
								if (!pformat->f4) {
									return MUTT_FAILED_MALLOC;
								}
								// Load
								muttResult res = mutt_f4Load(font, pformat->f4, data, datalen);
								if (mutt_result_is_fatal(res)) {
									mu_free(pformat->f4);
									pformat->f4 = 0;
								}
								return res;
							} break;

							// Format 12
							case 12: {
								// Allocate
								pformat->f12 = (muttCmap12*)mu_malloc(sizeof(muttCmap12));
								if (!pformat->f12) {
									return MUTT_FAILED_MALLOC;
								}
								// Load
								muttResult res = mutt_f12Load(font, pformat->f12, data, datalen);
								if (mutt_result_is_fatal(res)) {
									mu_free(pformat->f12);
									pformat->f12 = 0;
								}
								return res;
							} break;
						}
					}

					// Deloads a given format
					void mutt_DeloadCmapFormat(uint16_m format, muttCmapFormat* pformat) {
						switch (format) {
							default: break;

							// Format 0
							case 0: {
								// Deallocate
								mu_free(pformat->f0);
							} break;

							// Format 4
							case 4: {
								// Deload
								mutt_f4Deload(pformat->f4);
								// Deallocate
								mu_free(pformat->f4);
							} break;

							// Format 12
							case 12: {
								// Deload
								mutt_f12Deload(pformat->f12);
								// Deallocate
								mu_free(pformat->f12);
							} break;
						}
					}

					// Glyph -> codepoint, for a given format
					MUDEF uint16_m mutt_cmap_encoding_get_glyph(muttFont* font, muttEncodingRecord* record, uint32_m codepoint) {
						switch (record->format) {
							// Unknown
							default: return 0; break;

							// Format 0
							case 0: return mutt_cmap0_get_glyph(font, record->encoding.f0, codepoint); break;
							// Format 4
							case 4: return mutt_cmap4_get_glyph(font, record->encoding.f4, codepoint); break;
							// Format 12
							case 12: return mutt_cmap12_get_glyph(font, record->encoding.f12, codepoint); break;
						}
					}

					// Codepoint -> glyph, for a given format
					MUDEF uint32_m mutt_cmap_encoding_get_codepoint(muttFont* font, muttEncodingRecord* record, uint16_m glyph_id) {
						switch (record->format) {
							// Unknown
							default: return 0; break;

							// Format 0
							case 0: return mutt_cmap0_get_codepoint(font, record->encoding.f0, glyph_id); break;
							// Format 4
							case 4: return mutt_cmap4_get_codepoint(font, record->encoding.f4, glyph_id); break;
							// Format 12
							case 12: return mutt_cmap12_get_codepoint(font, record->encoding.f12, glyph_id); break;
						}
					}

				/* Cmap */

					// Loads the cmap table
					void mutt_DeloadCmap(muttCmap* cmap);
					muttResult mutt_LoadCmap(muttFont* font, muByte* data, uint32_m datalen) {
						// Placeholder values
						muByte* orig_data = data;

						// Verify length for version
						if (datalen < 2) {
							return MUTT_INVALID_CMAP_LENGTH;
						}
						// Verify version
						if (MU_RBEU16(data) != 0) {
							return MUTT_INVALID_CMAP_VERSION;
						}

						// Verify length for version and numTables
						if (datalen < 4) {
							return MUTT_INVALID_CMAP_LENGTH;
						}
						// Allocate cmap
						muttCmap* cmap = (muttCmap*)mu_malloc(sizeof(muttCmap));
						if (!cmap) {
							return MUTT_FAILED_MALLOC;
						}
						mu_memset(cmap, 0, sizeof(muttCmap));

						// numTables
						cmap->num_tables = MU_RBEU16(data+2);
						// Terminate early if no tables specified
						if (cmap->num_tables == 0) {
							font->cmap = cmap;
							return MUTT_SUCCESS;
						}
						// Verify length for tables
						if (datalen < (4 + (uint32_m)(cmap->num_tables*8))) {
							mutt_DeloadCmap(cmap);
							return MUTT_INVALID_CMAP_LENGTH;
						}

						// Allocate tables
						cmap->encoding_records = (muttEncodingRecord*)mu_malloc(sizeof(muttEncodingRecord)*cmap->num_tables);
						if (!cmap->encoding_records) {
							mutt_DeloadCmap(cmap);
							return MUTT_FAILED_MALLOC;
						}
						mu_memset(cmap->encoding_records, 0, sizeof(muttEncodingRecord)*cmap->num_tables);

						// Loop through each table
						data += 4;
						for (uint16_m t = 0; t < cmap->num_tables; ++t) {
							// Get record
							muttEncodingRecord* r = &cmap->encoding_records[t];

							// platformID
							r->platform_id = MU_RBEU16(data);
							// encodingID
							r->encoding_id = MU_RBEU16(data+2);

							// subtableOffset
							uint32_m offset = MU_RBEU32(data+4);
							if (offset >= datalen) {
								mutt_DeloadCmap(cmap);
								return MUTT_INVALID_CMAP_ENCODING_RECORD_OFFSET;
							}
							
							// Length of subtable
							uint32_m subtable_len = datalen-offset;
							// Verify length for format
							if (subtable_len < 2) {
								mutt_DeloadCmap(cmap);
								return MUTT_INVALID_CMAP_ENCODING_RECORD_LENGTH;
							}
							// format
							r->format = MU_RBEU16(orig_data+offset);
							// Load format
							r->result = mutt_LoadCmapFormat(font, r->format, &r->encoding, orig_data+offset, subtable_len);

							// Increment to next table
							data += 8;
						}

						font->cmap = cmap;
						return MUTT_SUCCESS;
					}

					// Deloads the cmap table
					void mutt_DeloadCmap(muttCmap* cmap) {
						// cmap:
						if (cmap) {
							// Encoding records:
							if (cmap->encoding_records) {
								// Per encoding record:
								for (uint16_m t = 0; t < cmap->num_tables; ++t) {
									// Deload format if necessary
									if (!mutt_result_is_fatal(cmap->encoding_records[t].result)) {
										mutt_DeloadCmapFormat(cmap->encoding_records[t].format, &cmap->encoding_records[t].encoding);
									}
								}
								mu_free(cmap->encoding_records);
							}
							mu_free(cmap);
						}
					}

					// Glyph -> codepoint, for all cmap formats
					MUDEF uint16_m mutt_get_glyph(muttFont* font, uint32_m codepoint) {
						// Loop through each encoding record
						for (uint16_m t = 0; t < font->cmap->num_tables; ++t) {
							// Try getting glyph ID for the encoding record
							uint16_m glyph = mutt_cmap_encoding_get_glyph(font, &font->cmap->encoding_records[t], codepoint);
							// Return if not 0
							if (glyph != 0) {
								return glyph;
							}
						}

						// Non-zero glyph ID couldn't be found
						return 0;
					}

					// Codepoint -> glyph, for all cmap formats
					MUDEF uint32_m mutt_get_codepoint(muttFont* font, uint16_m glyph_id) {
						// Loop through each encoding record
						for (uint16_m t = 0; t < font->cmap->num_tables; ++t) {
							// Try getting codepoint for the encoding record
							uint32_m codepoint = mutt_cmap_encoding_get_codepoint(font, &font->cmap->encoding_records[t], glyph_id);
							// Return if not 0
							if (codepoint != 0) {
								return codepoint;
							}
						}

						// Non-zero codepoint couldn't be found
						return 0;
					}

			/* Loading / Deloading */

				// Initializes all flag/result states of each table to "failed to find"
				void mutt_InitTables(muttFont* font, muttLoadFlags load_flags) {
					// maxp
					font->maxp_res = (load_flags & MUTT_LOAD_MAXP) ? MUTT_FAILED_FIND_TABLE : 0;
					font->fail_load_flags |= (load_flags & MUTT_LOAD_MAXP);
					// head
					font->head_res = (load_flags & MUTT_LOAD_HEAD) ? MUTT_FAILED_FIND_TABLE : 0;
					font->fail_load_flags |= (load_flags & MUTT_LOAD_HEAD);
					// hhea
					font->hhea_res = (load_flags & MUTT_LOAD_HHEA) ? MUTT_FAILED_FIND_TABLE : 0;
					font->fail_load_flags |= (load_flags & MUTT_LOAD_HHEA);
					// hmtx
					font->hmtx_res = (load_flags & MUTT_LOAD_HMTX) ? MUTT_FAILED_FIND_TABLE : 0;
					font->fail_load_flags |= (load_flags & MUTT_LOAD_HMTX);
					// loca
					font->loca_res = (load_flags & MUTT_LOAD_LOCA) ? MUTT_FAILED_FIND_TABLE : 0;
					font->fail_load_flags |= (load_flags & MUTT_LOAD_LOCA);
					// name
					font->name_res = (load_flags & MUTT_LOAD_NAME) ? MUTT_FAILED_FIND_TABLE : 0;
					font->fail_load_flags |= (load_flags & MUTT_LOAD_NAME);
					// glyf
					font->glyf_res = (load_flags & MUTT_LOAD_GLYF) ? MUTT_FAILED_FIND_TABLE : 0;
					font->fail_load_flags |= (load_flags & MUTT_LOAD_GLYF);
					// cmap
					font->cmap_res = (load_flags & MUTT_LOAD_CMAP) ? MUTT_FAILED_FIND_TABLE : 0;
					font->fail_load_flags |= (load_flags & MUTT_LOAD_CMAP);
				}

				// Does one pass through each table load
				void mutt_LoadTables(muttFont* font, muByte* data, muttLoadFlags* first, muBool dep_pass, muttLoadFlags* waiting) {
					// Loop through each table
					for (uint16_m i = 0; i < font->directory->num_tables; ++i) {
						// Get record information
						muttTableRecord rec = font->directory->records[i];

						// Do things based on table tag
						switch (rec.table_tag_u32) {
							default: break;

							// maxp
							case 0x6D617870: {
								// Account for first
								if (dep_pass) {
									*first |= MUTT_LOAD_MAXP;
								}
								// Skip if already processed
								// This works because all tables to be processed are initialized
								// to "MUTT_FAILED_FIND_TABLE" and set to something else once
								// processed.
								if (font->maxp_res != MUTT_FAILED_FIND_TABLE) {
									break;
								}

								// Load
								font->maxp_res = mutt_LoadMaxp(font, &data[rec.offset], rec.length);
								if (font->maxp) {
									font->load_flags |= MUTT_LOAD_MAXP;
									font->fail_load_flags &= ~MUTT_LOAD_MAXP;
								} else {
									font->fail_load_flags |= MUTT_LOAD_MAXP;
									font->load_flags &= ~MUTT_LOAD_MAXP;
								}
							} break;

							// head
							case 0x68656164: {
								// Account for first
								if (dep_pass) {
									*first |= MUTT_LOAD_HEAD;
								}
								// Skip if already processed
								if (font->head_res != MUTT_FAILED_FIND_TABLE) {
									break;
								}

								// Load
								font->head_res = mutt_LoadHead(font, &data[rec.offset], rec.length);
								if (font->head) {
									font->load_flags |= MUTT_LOAD_HEAD;
									font->fail_load_flags &= ~MUTT_LOAD_HEAD;
								} else {
									font->fail_load_flags |= MUTT_LOAD_HEAD;
									font->load_flags &= ~MUTT_LOAD_HEAD;
								}
							} break;

							// hhea; req maxp
							case 0x68686561: {
								// Account for first
								if (dep_pass) {
									*first |= MUTT_LOAD_HHEA;
								}
								// Skip if already processed
								if (font->hhea_res != MUTT_FAILED_FIND_TABLE) {
									break;
								}

								// Give bad result if missing dependency
								if (!dep_pass && !(*first & MUTT_LOAD_MAXP)) {
									font->hhea_res = MUTT_HHEA_REQUIRES_MAXP;
									break;
								}
								// Continue if dependencies aren't processed
								if (!font->maxp) {
									*waiting |= MUTT_LOAD_HHEA;
									break;
								}
								// Mark as no longer waiting
								*waiting &= ~MUTT_LOAD_HHEA;

								// Load
								font->hhea_res = mutt_LoadHhea(font, &data[rec.offset], rec.length);
								if (font->hhea) {
									font->load_flags |= MUTT_LOAD_HHEA;
									font->fail_load_flags &= ~MUTT_LOAD_HHEA;
								} else {
									font->fail_load_flags |= MUTT_LOAD_HHEA;
									font->load_flags &= ~MUTT_LOAD_HHEA;
								}
							} break;

							// hmtx; req maxp, hhea
							case 0x686D7478: {
								// Account for first
								if (dep_pass) {
									*first |= MUTT_LOAD_HMTX;
								}
								// Skip if already processed
								if (font->hmtx_res != MUTT_FAILED_FIND_TABLE) {
									break;
								}

								// Give bad result if missing dependency
								if (!dep_pass) {
									// maxp
									if (!(*first & MUTT_LOAD_MAXP)) {
										font->hmtx_res = MUTT_HMTX_REQUIRES_MAXP;
										break;
									}
									// hhea
									if (!(*first & MUTT_LOAD_HHEA)) {
										font->hmtx_res = MUTT_HMTX_REQUIRES_HHEA;
										break;
									}
								}
								// Continue if dependencies aren't processed
								if (!(font->maxp) || !(font->hhea)) {
									*waiting |= MUTT_LOAD_HMTX;
									break;
								}
								// Mark as no longer waiting
								*waiting &= ~MUTT_LOAD_HMTX;

								// Load
								font->hmtx_res = mutt_LoadHmtx(font, &data[rec.offset], rec.length);
								if (font->hmtx) {
									font->load_flags |= MUTT_LOAD_HMTX;
									font->fail_load_flags &= ~MUTT_LOAD_HMTX;
								} else {
									font->fail_load_flags |= MUTT_LOAD_HMTX;
									font->load_flags &= ~MUTT_LOAD_HMTX;
								}
							} break;

							// loca; req maxp, head, glyf
							case 0x6C6F6361: {
								// Account for first
								if (dep_pass) {
									*first |= MUTT_LOAD_LOCA;
								}
								// Skip if already processed
								if (font->loca_res != MUTT_FAILED_FIND_TABLE) {
									break;
								}

								// Give bad result if missing dependency
								if (!dep_pass) {
									// maxp
									if (!(*first & MUTT_LOAD_MAXP)) {
										font->loca_res = MUTT_LOCA_REQUIRES_MAXP;
										break;
									}
									// head
									if (!(*first & MUTT_LOAD_HEAD)) {
										font->loca_res = MUTT_LOCA_REQUIRES_HEAD;
										break;
									}
									// glyf
									if (!(*first & MUTT_LOAD_GLYF)) {
										font->loca_res = MUTT_LOCA_REQUIRES_GLYF;
										break;
									}
								}
								// Continue if dependencies aren't processed
								if (!(font->maxp) || !(font->head) || !(font->glyf)) {
									*waiting |= MUTT_LOAD_LOCA;
									break;
								}
								// Mark as no longer waiting
								*waiting &= ~MUTT_LOAD_LOCA;

								// Load
								font->loca_res = mutt_LoadLoca(font, &data[rec.offset], rec.length);
								if (font->loca) {
									font->load_flags |= MUTT_LOAD_LOCA;
									font->fail_load_flags &= ~MUTT_LOAD_LOCA;
								} else {
									font->fail_load_flags |= MUTT_LOAD_LOCA;
									font->load_flags &= ~MUTT_LOAD_LOCA;
								}
							} break;

							// name
							case 0x6E616D65: {
								// Account for first
								if (dep_pass) {
									*first |= MUTT_LOAD_NAME;
								}
								// Skip if already processed
								if (font->name_res != MUTT_FAILED_FIND_TABLE) {
									break;
								}

								// Load
								font->name_res = mutt_LoadName(font, &data[rec.offset], rec.length);
								if (font->name) {
									font->load_flags |= MUTT_LOAD_NAME;
									font->fail_load_flags &= ~MUTT_LOAD_NAME;
								} else {
									font->fail_load_flags |= MUTT_LOAD_NAME;
									font->load_flags &= ~MUTT_LOAD_NAME;
								}
							} break;

							// glyf
							case 0x676C7966: {
								// Account for first
								if (dep_pass) {
									*first |= MUTT_LOAD_GLYF;
								}
								// Skip if already processed
								if (font->glyf_res != MUTT_FAILED_FIND_TABLE) {
									break;
								}

								// Load
								font->glyf_res = mutt_LoadGlyf(font, &data[rec.offset], rec.length);
								if (font->glyf) {
									font->load_flags |= MUTT_LOAD_GLYF;
									font->fail_load_flags &= ~MUTT_LOAD_GLYF;
								} else {
									font->fail_load_flags |= MUTT_LOAD_GLYF;
									font->load_flags &= ~MUTT_LOAD_GLYF;
								}
							} break;

							// cmap
							case 0x636D6170: {
								// Account for first
								if (dep_pass) {
									*first |= MUTT_LOAD_CMAP;
								}
								// Skip if already processed
								if (font->cmap_res != MUTT_FAILED_FIND_TABLE) {
									break;
								}

								// Give bad result if missing dependency
								if (!dep_pass && !(*first & MUTT_LOAD_MAXP)) {
									font->cmap_res = MUTT_CMAP_REQUIRES_MAXP;
									break;
								}
								// Continue if dependencies aren't processed
								if (!font->maxp) {
									*waiting |= MUTT_LOAD_CMAP;
									break;
								}
								// Mark as no longer waiting
								*waiting &= ~MUTT_LOAD_CMAP;

								// Load
								font->cmap_res = mutt_LoadCmap(font, &data[rec.offset], rec.length);
								if (font->cmap) {
									font->load_flags |= MUTT_LOAD_CMAP;
									font->fail_load_flags &= ~MUTT_LOAD_CMAP;
								} else {
									font->fail_load_flags |= MUTT_LOAD_CMAP;
									font->load_flags &= ~MUTT_LOAD_CMAP;
								}
							} break;
						}
					}
				}

				// Deallocates all loaded tables
				void mutt_DeloadTables(muttFont* font) {
					// Basic tables
					if (font->maxp) {
						mu_free(font->maxp);
					}
					if (font->head) {
						mu_free(font->head);
					}
					if (font->hhea) {
						mu_free(font->hhea);
					}

					// Allocated tables
					mutt_DeloadHmtx(font->hmtx);
					mutt_DeloadLoca(font->loca);
					mutt_DeloadName(font->name);
					mutt_DeloadGlyf(font->glyf);
					mutt_DeloadCmap(font->cmap);
				}

				MUDEF muttResult mutt_load(muByte* data, uint64_m datalen, muttFont* font, muttLoadFlags load_flags) {
					muttResult res;

					// Zero-out font
					mu_memset(font, 0, sizeof(muttFont));

					// Load table directory
					// - Allocate
					font->directory = (muttDirectory*)mu_malloc(sizeof(muttDirectory));
					if (!font->directory) {
						// mutt_deload(font);
						return MUTT_FAILED_MALLOC;
					}
					// - Load
					res = mutt_LoadTableDirectory(font->directory, data, datalen);
					if (mutt_result_is_fatal(res)) {
						mutt_deload(font);
						return res;
					}

					// Init and load tables
					mutt_InitTables(font, load_flags);

					muttLoadFlags temp_flags = 0;
					muttLoadFlags wait_flags = 0;
					mutt_LoadTables(font, data, &temp_flags, MU_TRUE, &wait_flags);
					while (wait_flags) {
						mutt_LoadTables(font, data, &temp_flags, MU_FALSE, &wait_flags);
					}

					return MUTT_SUCCESS;
				}

				MUDEF void mutt_deload(muttFont* font) {
					// Deload tables
					mutt_DeloadTables(font);
					// Deload table directory
					if (font->directory) {
						mutt_DeloadTableDirectory(font->directory);
						mu_free(font->directory);
					}
				}

			/* Glyf stuff */

				// Fills in the "muttGlyphHeader" struct
				MUDEF muttResult mutt_glyph_header(muttFont* font, uint16_m glyph_id, muttGlyphHeader* header) {
					// Get length and data of glyph
					// - 16-bit
					if (font->head->index_to_loc_format == 0) {
						header->data = &font->glyf->data[((uint32_m)font->loca->offsets16[glyph_id])*2];
						header->length = (((uint32_m)font->loca->offsets16[glyph_id+1])*2) - (((uint32_m)font->loca->offsets16[glyph_id])*2);
					}
					// - 32-bit
					else {
						header->data = &font->glyf->data[font->loca->offsets32[glyph_id]];
						header->length = font->loca->offsets32[glyph_id+1] - font->loca->offsets32[glyph_id];
					}

					// If header length is 0, we set everything to 0 and return
					// (because no outline)
					if (header->length == 0) {
						mu_memset(header, 0, sizeof(muttGlyphHeader));
						return MUTT_SUCCESS;
					}

					// Verify minimum length for header
					if (header->length < 10) {
						return MUTT_INVALID_GLYF_HEADER_LENGTH;
					}

					// numberOfContours
					header->number_of_contours = MU_RBES16(header->data);
					if (header->number_of_contours > font->maxp->max_contours) {
						return MUTT_INVALID_GLYF_HEADER_NUMBER_OF_CONTOURS;
					}

					// xMin
					header->x_min = MU_RBES16(header->data+2);
					if (header->x_min < font->head->x_min) {
						return MUTT_INVALID_GLYF_HEADER_X_MIN;
					}
					// yMin
					header->y_min = MU_RBES16(header->data+4);
					if (header->y_min < font->head->y_min) {
						return MUTT_INVALID_GLYF_HEADER_Y_MIN;
					}
					// xMax
					header->x_max = MU_RBES16(header->data+6);
					if (header->x_max > font->head->x_max) {
						return MUTT_INVALID_GLYF_HEADER_X_MAX;
					}
					if (header->x_max < header->x_min) {
						return MUTT_INVALID_GLYF_HEADER_X_MIN_MAX;
					}
					// yMax
					header->y_max = MU_RBES16(header->data+8);
					if (header->y_max > font->head->y_max) {
						return MUTT_INVALID_GLYF_HEADER_Y_MAX;
					}
					if (header->y_max < header->y_min) {
						return MUTT_INVALID_GLYF_HEADER_Y_MIN_MAX;
					}

					// Subtract header data from length
					header->length -= 10;
					// ...and move data up past header
					header->data += 10;

					return MUTT_SUCCESS;
				}

				// Fills in (or calculates memory needed for) "muttSimpleGlyph" struct
				MUDEF muttResult mutt_simple_glyph(muttFont* font, muttGlyphHeader* header, muttSimpleGlyph* glyph, muByte* data, uint32_m* written) {
					muttResult res = MUTT_SUCCESS;

					// Verify length for endPtsOfContours and instructionLength
					// (req is u64 to avoid possible overflow)
					uint64_m req = (((uint32_m)header->number_of_contours)*2) + 2;
					if (header->length < req) {
						return MUTT_INVALID_GLYF_SIMPLE_LENGTH;
					}

					// Glyph data:
					muByte* gdata = header->data;

					// Memory size calculation:
					if (!data) {
						// Add endPtsOfContours to memory
						uint32_m min_size = header->number_of_contours*2;

						// To calculate amount of points:
						uint16_m points = 0;
						if (header->number_of_contours != 0) {
							// Skip to last element of endPtsOfContours and read
							gdata += (uint32_m)(header->number_of_contours-1)*2;
							points = MU_RBEU16(gdata);

							// Verify last element of endPtsOfContours
							if (points == 0xFFFF) {
								return MUTT_INVALID_GLYF_SIMPLE_END_PTS_OF_CONTOURS;
							}

							// Skip past last element
							++points;
							gdata += 2;
						}

						// Verify point count
						if (points > font->maxp->max_points) {
							return MUTT_INVALID_GLYF_SIMPLE_POINT_COUNT;
						}
						// Add points to memory
						min_size += ((uint32_m)points)*sizeof(muttGlyphPoint);

						// Add instructions to memory
						uint16_m instruction_length = MU_RBEU16(gdata);
						if (instruction_length > font->maxp->max_size_of_instructions) {
							return MUTT_INVALID_GLYF_SIMPLE_INSTRUCTION_LENGTH;
						}
						min_size += instruction_length;

						// Check for size fitting
						// This should not need to be checked, as it is calculated via maxp,
						// to which all of these values are also checked to not exceed.
						/*if (min_size > mutt_simple_glyph_max_size(font)) {
							return MUTT_INVALID_GLYF_SIMPLE_;
						}*/

						// Set memory req
						*written = min_size;
						return MUTT_SUCCESS;
					}

					// Store pointer to original data
					muByte* orig_data = data;

					// Allocate endPtsOfContours
					glyph->end_pts_of_contours = (uint16_m*)data;
					data += ((uint32_m)header->number_of_contours)*2;

					// Loop through each contour
					uint16_m points = 0;
					for (uint16_m c = 0; c < header->number_of_contours; ++c) {
						// Copy value for endPtsOfContour
						glyph->end_pts_of_contours[c] = MU_RBEU16(gdata);

						// Verify increasing order
						if (c != 0) {
							if (glyph->end_pts_of_contours[c] <= glyph->end_pts_of_contours[c-1]) {
								return MUTT_INVALID_GLYF_SIMPLE_END_PTS_OF_CONTOURS;
							}
						}

						// Get point count if we're on the last index
						if (c == header->number_of_contours-1) {
							points = glyph->end_pts_of_contours[c];
							// Verify last element
							if (points == 0xFFFF) {
								return MUTT_INVALID_GLYF_SIMPLE_END_PTS_OF_CONTOURS;
							}
							// Increment points by one to get count (since last value is an index)
							++points;
						}

						// Move to next entry in endPtsOfContours
						gdata += 2;
					}

					// Verify point count
					if (points > font->maxp->max_points) {
						return MUTT_INVALID_GLYF_SIMPLE_POINT_COUNT;
					}

					// instructionLength
					glyph->instruction_length = MU_RBEU16(gdata);
					gdata += 2;
					// + Verify instructionLength
					if (glyph->instruction_length > font->maxp->max_size_of_instructions) {
						return MUTT_INVALID_GLYF_SIMPLE_INSTRUCTION_LENGTH;
					}

					// For instructions:
					if (glyph->instruction_length != 0) {
						// Verify length for instructions
						req += glyph->instruction_length;
						if (header->length < req) {
							return MUTT_INVALID_GLYF_SIMPLE_LENGTH;
						}
						// Allocate
						glyph->instructions = (uint8_m*)data;
						// Copy over
						mu_memcpy(glyph->instructions, gdata, glyph->instruction_length);
						// Move past instructions
						data += glyph->instruction_length;
						gdata += glyph->instruction_length;
					}

					// Allocate points
					glyph->points = (muttGlyphPoint*)data;
					data += points*sizeof(muttGlyphPoint);

					// Loop through each flag
					uint16_m pi = 0; // (point index)
					uint16_m contour_id = 0;
					while (pi < points) {
						// Increment contour ID if we've gone over the end point of the current contour
						if (pi > glyph->end_pts_of_contours[contour_id]) {
							++contour_id;
						}
						// Verify length for this flag
						if (header->length < ++req) {
							return MUTT_INVALID_GLYF_SIMPLE_LENGTH;
						}

						// Get flag
						uint8_m flags = *gdata;
						glyph->points[pi].flags = flags;

						// Verify that flag doesn't rely on prior values if this is the first flag
						// (Some common fonts get this messed up; we're assuming 0 in these cases)
						// This is brought up in the documentation.
						// ...

						// Verify that first point is on curve
						// (Not checking for similar reasons above, also brought up in doc)
						// ...

						// Get implied length of x- and y-coordinates based on flag
						uint8_m coord_length = 0;
						// - x
						if (flags & MUTT_X_SHORT_VECTOR) {
							// 1-byte x-coordinate
							coord_length += 1;
						} else if (!(flags & MUTT_X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR)) {
							// 2-byte x-coordinate
							coord_length += 2;
						}
						// -y
						if (flags & MUTT_Y_SHORT_VECTOR) {
							// 1-byte y-coordinate
							coord_length += 1;
						} else if (!(flags & MUTT_Y_IS_SAME_OR_POSITIVE_Y_SHORT_VECTOR)) {
							// 2-byte y-coordinate
							coord_length += 2;
						}

						// Verify length based on coord length
						req += coord_length;
						if (header->length < req) {
							return MUTT_INVALID_GLYF_SIMPLE_LENGTH;
						}

						// Increment past this point/flag
						++pi;
						++gdata;

						// Handle repeating flags:
						if (flags & MUTT_REPEAT_FLAG) {
							// Verify length for next flag
							if (header->length < ++req) {
								return MUTT_INVALID_GLYF_SIMPLE_LENGTH;
							}

							// Get next flag as length
							uint8_m next = *gdata++;
							// (required length is checked next loop)
							// - If next is 0, we don't repeat at all, and pretend
							// like this never happened
							if (next == 0) {
								continue;
							}

							// Verify length for repeated coordinate flags
							req += ((uint32_m)coord_length) * ((uint32_m)next-1);
							if (header->length < req) {
								return MUTT_INVALID_GLYF_SIMPLE_LENGTH;
							}

							// Verify repeat count
							// Not doing this since we can easily pretend like it didn't
							// happen to no consequence.
							// This is mentioned in documentation
							/*if (pi+next > points) {
								return MUTT_INVALID_GLYF_SIMPLE_FLAG_REPEAT_COUNT;
							}*/

							// Go through each repeat flag and copy it down
							for (uint8_m r = 0; r < next && pi < points; ++r) {
								glyph->points[pi++].flags = flags;
							}
						}
					}

					// Loop through each x-coordinate
					int16_m x_min = 0;
					int16_m x_max = 0;
					pi = 0;
					while (pi < points) {
						// Get flags
						uint8_m flags = glyph->points[pi].flags;

						// Get x-coordinate
						if (flags & MUTT_X_SHORT_VECTOR) {
							// 1-byte
							if (flags & MUTT_X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR) {
								// Positive
								glyph->points[pi].x = (int16_m)*gdata++;
							} else {
								// Negative
								glyph->points[pi].x = -((int16_m)*gdata++);
							}
						} else {
							if (flags & MUTT_X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR) {
								// Repeated x-coordinate
								glyph->points[pi].x = (pi!=0) ?(glyph->points[pi-1].x) :(0);
								++pi;
								// (Continue because no vector stuff is applied)
								continue;
							} else {
								// 2-bytes, signed
								glyph->points[pi].x = MU_RBES16(gdata);
								gdata += 2;
							}
						}

						// Add as vector to prior value
						// - Get previous x value
						int16_m prev_x = (pi!=0) ?(glyph->points[pi-1].x) :(0);
						// - Compute the current coordinate based on previous
						int32_m test_val = prev_x + glyph->points[pi].x;
						// - Verify that the point is within range
						if (test_val < header->x_min || test_val > header->x_max) {
							res = MUTT_INVALID_GLYF_SIMPLE_X_COORD;
						}
						// - Verify that the point is within FUnit range
						if (test_val < -16384 || test_val > 16383) {
							return MUTT_INVALID_GLYF_SIMPLE_X_COORD_FUNITS;
						}
						// - Consider it with currently calculated x min/max
						if ((test_val < x_min) || (pi == 0)) {
							x_min = test_val;
						}
						if ((test_val > x_max) || (pi == 0)) {
							x_max = test_val;
						}
						// - Assign
						glyph->points[pi++].x = (int16_m)test_val;
					}

					// Loop through each y-coordinate
					int16_m y_min = 0;
					int16_m y_max = 0;
					pi = 0;
					while (pi < points) {
						// Get flags
						uint8_m flags = glyph->points[pi].flags;

						// Get y-coordinate
						if (flags & MUTT_Y_SHORT_VECTOR) {
							// 1-byte
							if (flags & MUTT_Y_IS_SAME_OR_POSITIVE_Y_SHORT_VECTOR) {
								// Positive
								glyph->points[pi].y = (int16_m)*gdata++;
							} else {
								// Negative
								glyph->points[pi].y = -((int16_m)*gdata++);
							}
						} else {
							if (flags & MUTT_Y_IS_SAME_OR_POSITIVE_Y_SHORT_VECTOR) {
								// Repeated y-coordinate
								glyph->points[pi].y = (pi!=0) ?(glyph->points[pi-1].y) :(0);
								++pi;
								// (Continue because no vector stuff is applied)
								continue;
							} else {
								// 2-bytes, signed
								glyph->points[pi].y = MU_RBES16(gdata);
								gdata += 2;
							}
						}

						// Add as vector to prior value
						// - Get previous y value
						int16_m prev_y = (pi!=0) ?(glyph->points[pi-1].y) :(0);
						// - Compute the current coordinate based on previous
						int32_m test_val = prev_y + glyph->points[pi].y;
						// - Verify that the point is within range
						if (test_val < header->y_min || test_val > header->y_max) {
							res = MUTT_INVALID_GLYF_SIMPLE_Y_COORD;
						}
						// - Verify that the point is within FUnit range
						if (test_val < -16384 || test_val > 16383) {
							return MUTT_INVALID_GLYF_SIMPLE_Y_COORD_FUNITS;
						}
						// - Consider it with currently calculated y min/max values
						if ((test_val < y_min) || (pi == 0)) {
							y_min = test_val;
						}
						if ((test_val > y_max) || (pi == 0)) {
							y_max = test_val;
						}
						// - Assign
						glyph->points[pi++].y = (int16_m)test_val;
					}

					// Write calculated x/y min/max values
					header->x_min = x_min;
					header->y_min = y_min;
					header->x_max = x_max;
					header->y_max = y_max;
					// Write how many bytes of data were used
					if (written) {
						*written = data-orig_data;
					}
					return res;
				}

				// Calculates amount of points within a simple glyph
				MUDEF muttResult mutt_simple_glyph_points(muttFont* font, muttGlyphHeader* header, uint16_m* num_points) {
					// Give 0 for no contours
					if (header->number_of_contours == 0) {
						*num_points = 0;
						return MUTT_SUCCESS;
					}

					// Verify length for endPtsOfContours
					uint32_m req = ((uint32_m)header->number_of_contours) * 2;
					if (header->length < req) {
						return MUTT_INVALID_GLYF_SIMPLE_LENGTH;
					}

					// Skip to last element of endPtsOfContours and read
					uint16_m rnum_points = MU_RBEU16(header->data + (((uint32_m)(header->number_of_contours-1)) * 2));
					// Verify number of points won't overflow
					if (rnum_points == 0xFFFF) {
						return MUTT_INVALID_GLYF_SIMPLE_END_PTS_OF_CONTOURS;
					}
					// See if point count is good for maxp
					if (++rnum_points > font->maxp->max_points) {
						return MUTT_INVALID_GLYF_SIMPLE_POINT_COUNT;
					}

					// Write point count and give success
					*num_points = rnum_points;
					return MUTT_SUCCESS;
				}

				// Simple glyph memory maximum
				MUDEF uint32_m mutt_simple_glyph_max_size(muttFont* font) {
					return
						// endPtsOfContours
						(font->maxp->max_contours*2)
						// instructions
						+(font->maxp->max_size_of_instructions)
						// maxPoints
						+(font->maxp->max_points*sizeof(muttGlyphPoint))
					;
				}

				// Calculates the x/y min/max for the given simple glyph
				MUDEF muttResult mutt_simple_glyph_min_max(muttFont* font, muttGlyphHeader* header) {
					// Verify length for endPtsOfContours and instructionLength
					uint64_m req = (((uint32_m)header->number_of_contours)*2) + 2;
					if (header->length < req) {
						return MUTT_INVALID_GLYF_SIMPLE_LENGTH;
					}

					// Progressive glyph data:
					// (Move to endPtsOfContours[numContours-1])
					muByte* data = header->data + (req-4);
					// Read last element of endPtsOfContours
					uint16_m num_points = MU_RBEU16(data);
					data += 2;
					// Verify it for number of points
					if (num_points++ == 0xFFFF) {
						return MUTT_INVALID_GLYF_SIMPLE_END_PTS_OF_CONTOURS;
					}
					if (num_points > font->maxp->max_points) {
						return MUTT_INVALID_GLYF_SIMPLE_POINT_COUNT;
					}

					// Read instructionLength
					uint16_m instruction_len = MU_RBEU16(data);
					data += 2;
					// Verify instructions
					req += instruction_len;
					if (header->length < req) {
						return MUTT_INVALID_GLYF_SIMPLE_LENGTH;
					}
					// Move past instructions
					data += instruction_len;

					// Allocate flags
					uint8_m* flags = (uint8_m*)mu_malloc(num_points);
					if (!flags) {
						return MUTT_FAILED_MALLOC;
					}

					// Loop through each flag
					uint16_m pi = 0;
					while (pi < num_points) {
						// Verify length for this flag
						if (header->length < ++req) {
							mu_free(flags);
							return MUTT_INVALID_GLYF_SIMPLE_LENGTH;
						}

						// Get flag
						uint8_m flag = flags[pi] = *data;

						// Get implied length of x- and y-coordinates based on flag
						uint8_m coord_length = 0;
						// - x
						if (flag & MUTT_X_SHORT_VECTOR) {
							// 1-byte x-coordinate
							coord_length += 1;
						} else if (!(flag & MUTT_X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR)) {
							// 2-byte x-coordinate
							coord_length += 2;
						}
						// - y
						if (flag & MUTT_Y_SHORT_VECTOR) {
							// 1-byte y-coordinate
							coord_length += 1;
						} else if (!(flag & MUTT_Y_IS_SAME_OR_POSITIVE_Y_SHORT_VECTOR)) {
							// 2-byte y-coordinate
							coord_length += 2;
						}

						// Verify length based on coord length
						req += coord_length;
						if (header->length < req) {
							mu_free(flags);
							return MUTT_INVALID_GLYF_SIMPLE_LENGTH;
						}

						// Increment past this point/flag
						++pi;
						++data;

						// Handle repeating flags:
						if (flag & MUTT_REPEAT_FLAG) {
							// Verify length for next flag
							if (header->length < ++req) {
								mu_free(flags);
								return MUTT_INVALID_GLYF_SIMPLE_LENGTH;
							}

							// Get next flag as length
							uint8_m next = *data++;
							// If next is 0, we don't repeat at all, and pretend
							// like this never happened
							if (next == 0) {
								continue;
							}

							// Verify length for repeated coordinate flags
							req += ((uint32_m)coord_length) * ((uint32_m)(next-1));
							if (header->length < req) {
								mu_free(flags);
								return MUTT_INVALID_GLYF_SIMPLE_LENGTH;
							}

							// Go through each repeat flag and copy it down
							for (uint8_m r = 0; r < next && pi < num_points; ++r) {
								flags[pi++] = flag;
							}
						}
					}

					// Loop through each x-coordinate
					pi = 0;
					int16_m prev_x = 0;
					while (pi < num_points) {
						// Get x-coordinate
						int16_m x;
						if (flags[pi] & MUTT_X_SHORT_VECTOR) {
							// 1-byte
							if (flags[pi] & MUTT_X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR) {
								// Positive
								x = (int16_m)*data++;
							} else {
								// Negative
								x = -((int16_m)*data++);
							}
						} else {
							if (flags[pi] & MUTT_X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR) {
								// Repeated x-coordinate
								++pi;
								continue;
							} else {
								// 2-bytes, signed
								x = MU_RBES16(data);
								data += 2;
							}
						}

						// Add as vector to prior value
						int32_m test_val = prev_x + x;
						// Verify FUnit range
						if (test_val < -16384 || test_val > 16383) {
							mu_free(flags);
							return MUTT_INVALID_GLYF_SIMPLE_X_COORD_FUNITS;
						}
						// Consider it with currently calculated x min/max
						if ((test_val < header->x_min) || (pi == 0)) {
							header->x_min = test_val;
						}
						if ((test_val > header->x_max) || (pi == 0)) {
							header->x_max = test_val;
						}
						// Set previous x as current x
						prev_x = test_val;
						++pi;
					}

					// Loop through each y-coordinate
					pi = 0;
					int16_m prev_y = 0;
					while (pi < num_points) {
						// Get y-coordinate
						int16_m y;
						if (flags[pi] & MUTT_Y_SHORT_VECTOR) {
							// 1-byte
							if (flags[pi] & MUTT_Y_IS_SAME_OR_POSITIVE_Y_SHORT_VECTOR) {
								// Positive
								y = (int16_m)*data++;
							} else {
								// Negative
								y = -((int16_m)*data++);
							}
						} else {
							if (flags[pi] & MUTT_Y_IS_SAME_OR_POSITIVE_Y_SHORT_VECTOR) {
								// Repeated y-coordinate
								++pi;
								continue;
							} else {
								// 2 bytes, signed
								y = MU_RBES16(data);
								data += 2;
							}
						}

						// Add as vector to prior value
						int32_m test_val = prev_y + y;
						// Verify FUnit range
						if (test_val < -16384 || test_val > 16383) {
							mu_free(flags);
							return MUTT_INVALID_GLYF_SIMPLE_Y_COORD_FUNITS;
						}
						// Consider it with currently calculated y min/max
						if ((test_val < header->y_min) || (pi == 0)) {
							header->y_min = test_val;
						}
						if ((test_val > header->y_max) || (pi == 0)) {
							header->y_max = test_val;
						}
						// Set previous y as current y
						prev_y = test_val;
						++pi;
					}

					// Deallocate flags
					mu_free(flags);
					return MUTT_SUCCESS; if (font) {}
				}

				// Fills in (or calculates memory needed for) "muttCompositeGlyph" struct
				MUDEF muttResult mutt_composite_glyph(muttFont* font, muttGlyphHeader* header, muttCompositeGlyph* glyph, muByte* data, uint32_m* written) {
					// Placeholder variables
					uint32_m req = 0; // Required length
					muByte* gdata = header->data; // Progressive data pointer

					// For memory calculations:
					if (!data) {
						// Loop through each record
						uint16_m flags = (header->length != 0) ?(MUTT_MORE_COMPONENTS) :(0);
						uint32_m written_amt = 0;
						muBool instructions = MU_FALSE;
						uint32_m component_count = 0;

						while (flags & MUTT_MORE_COMPONENTS) {
							// Verify component count
							if (++component_count > font->maxp->max_component_elements) {
								return MUTT_INVALID_GLYF_COMPOSITE_COMPONENT_COUNT;
							}
							// Add component to mem req
							written_amt += sizeof(muttComponentGlyph);

							// Verify length for flags and glyphIndex
							req += 4;
							if (header->length < req) {
								return MUTT_INVALID_GLYF_COMPOSITE_LENGTH;
							}
							// Read flags for if we have more components and instructions
							uint16_m this_flags = MU_RBEU16(gdata);
							flags = this_flags & MUTT_MORE_COMPONENTS;
							if (this_flags & MUTT_WE_HAVE_INSTRUCTIONS) {
								instructions = MU_TRUE;
							}
							// Skip past flags and glyphIndex
							gdata += 4;

							// Add arguments to req length and skip past
							if (this_flags & MUTT_ARG_1_AND_2_ARE_WORDS) {
								// 2 bytes per
								req += 4;
								gdata += 4;
							}
							else {
								// 1 byte per
								req += 2;
								gdata += 2;
							}

							// Add transform to req length and skip past
							// We're not checking for mutual exclusivity here, as
							// it's not relevant to mem req
							if (this_flags & MUTT_WE_HAVE_A_SCALE) {
								// One F2DOT14
								req += 2;
								gdata += 2;
							}
							else if (this_flags & MUTT_WE_HAVE_AN_X_AND_Y_SCALE) {
								// Two F2DOT14s
								req += 4;
								gdata += 4;
							}
							else if (this_flags & MUTT_WE_HAVE_A_TWO_BY_TWO) {
								// Four F2DOT14s
								req += 8;
								gdata += 8;
							}

							// Verify length for arguments and transform
							if (header->length < req) {
								return MUTT_INVALID_GLYF_COMPOSITE_LENGTH;
							}
						}

						// Instruction handling
						if (instructions) {
							// Verify length for instruction length
							req += 2;
							if (header->length < req) {
								return MUTT_INVALID_GLYF_COMPOSITE_LENGTH;
							}

							// Read instruction length
							uint16_m instruction_length = MU_RBEU16(gdata);
							// Verify with maxp
							if (instruction_length > font->maxp->max_size_of_instructions) {
								return MUTT_INVALID_GLYF_COMPOSITE_INSTRUCTION_LENGTH;
							}
							// Verify length for instructions
							// Unneeded here since we're just calculating mem req
							// ...

							// Add instructions to mem req
							written_amt += instruction_length;
						}

						// Give mem req and exit
						*written = written_amt;
						return MUTT_SUCCESS;
					}

					// Placeholder variables:
					muByte* orig_data = data; // (Used for memory usage at the end)
					uint32_m component_count = 0; // (Used for counting components; u32 to avoid overflow)
					muBool instructions = MU_FALSE; // (To see if we've found instructions)
					uint16_m flags = (header->length != 0) ?(MUTT_MORE_COMPONENTS) :(0);

					// Point components array to current data (what we're filling in)
					glyph->components = (muttComponentGlyph*)data;
					muttComponentGlyph* c = glyph->components;

					// Loop through each component:
					while (flags & MUTT_MORE_COMPONENTS) {
						// Verify component count
						if (component_count > font->maxp->max_component_elements) {
							return MUTT_INVALID_GLYF_COMPOSITE_COMPONENT_COUNT;
						}

						// Verify length for flags and glyphIndex
						req += 4;
						if (header->length < req) {
							return MUTT_INVALID_GLYF_COMPOSITE_LENGTH;
						}

						// Read flags
						c->flags = MU_RBEU16(gdata);
						gdata += 2;
						// Mark next component
						flags = c->flags & MUTT_MORE_COMPONENTS;
						// Mark instructions flag
						if (c->flags & MUTT_WE_HAVE_INSTRUCTIONS) {
							instructions = MU_TRUE;
						}

						// Read glyphIndex
						c->glyph_index = MU_RBEU16(gdata);
						gdata += 2;
						// Verify index range
						if (c->glyph_index >= font->maxp->num_glyphs) {
							return MUTT_INVALID_GLYF_COMPOSITE_GLYPH_INDEX;
						}

						// Arguments handling:
						if (c->flags & MUTT_ARG_1_AND_2_ARE_WORDS) {
							// 2 bytes per argument

							// Verify length
							req += 4;
							if (header->length < req) {
								return MUTT_INVALID_GLYF_COMPOSITE_LENGTH;
							}

							// Read values
							if (c->flags & MUTT_ARGS_ARE_XY_VALUES) {
								// int16 xy values
								c->argument1 = MU_RBES16(gdata);
								gdata += 2;
								c->argument2 = MU_RBES16(gdata);
								gdata += 2;
							} else {
								// uint16 point numbers
								c->argument1 = MU_RBEU16(gdata);
								gdata += 2;
								c->argument2 = MU_RBEU16(gdata);
								gdata += 2;
							}
						} else {
							// 1 byte per argument

							// Verify length
							req += 2;
							if (header->length < req) {
								return MUTT_INVALID_GLYF_COMPOSITE_LENGTH;
							}

							// Read values
							if (c->flags & MUTT_ARGS_ARE_XY_VALUES) {
								// int8 xy values
								c->argument1 = MU_RBES8(gdata++);
								c->argument2 = MU_RBES8(gdata++);
							} else {
								// uint8 point numbers
								c->argument1 = *gdata++;
								c->argument2 = *gdata++;
							}
						}

						// Transform data handling
						if (c->flags & MUTT_WE_HAVE_A_SCALE) {
							// One F2DOT14

							// Ensure flag exclusivity
							if ((c->flags & MUTT_WE_HAVE_AN_X_AND_Y_SCALE) || (c->flags & MUTT_WE_HAVE_A_TWO_BY_TWO)) {
								return MUTT_INVALID_GLYF_COMPOSITE_FLAGS;
							}

							// Verify length
							req += 2;
							if (header->length < req) {
								return MUTT_INVALID_GLYF_COMPOSITE_LENGTH;
							}

							// Read
							c->scales[0] = MUTT_F2DOT14(gdata);
							gdata += 2;
						}
						else if (c->flags & MUTT_WE_HAVE_AN_X_AND_Y_SCALE) {
							// Two F2DOT14s

							// Ensure flag exclusivity
							if ((c->flags & MUTT_WE_HAVE_A_SCALE) || (c->flags & MUTT_WE_HAVE_A_TWO_BY_TWO)) {
								return MUTT_INVALID_GLYF_COMPOSITE_FLAGS;
							}

							// Verify length
							req += 4;
							if (header->length < req) {
								return MUTT_INVALID_GLYF_COMPOSITE_LENGTH;
							}

							// Read
							c->scales[0] = MUTT_F2DOT14(gdata);
							gdata += 2;
							c->scales[1] = MUTT_F2DOT14(gdata);
							gdata += 2;
						}
						else if (c->flags & MUTT_WE_HAVE_A_TWO_BY_TWO) {
							// Four F2DOT14s

							// Ensure flag exclusivity
							if ((c->flags & MUTT_WE_HAVE_A_SCALE) || (c->flags & MUTT_WE_HAVE_AN_X_AND_Y_SCALE)) {
								return MUTT_INVALID_GLYF_COMPOSITE_FLAGS;
							}

							// Verify length
							req += 8;
							if (header->length < req) {
								return MUTT_INVALID_GLYF_COMPOSITE_LENGTH;
							}

							// Read
							c->scales[0] = MUTT_F2DOT14(gdata);
							gdata += 2;
							c->scales[1] = MUTT_F2DOT14(gdata);
							gdata += 2;
							c->scales[2] = MUTT_F2DOT14(gdata);
							gdata += 2;
							c->scales[3] = MUTT_F2DOT14(gdata);
							gdata += 2;
						}

						// Increment handle for component and component count
						++c;
						++component_count;
					}
					// Verify component count
					if (component_count > font->maxp->max_component_elements) {
						return MUTT_INVALID_GLYF_COMPOSITE_COMPONENT_COUNT;
					}
					// Move past component array
					data += sizeof(muttComponentGlyph) * component_count;

					// Instruction handling
					if (instructions) {
						// Point to data
						glyph->instructions = data;

						// Verify length for instruction length
						req += 2;
						if (header->length < req) {
							return MUTT_INVALID_GLYF_COMPOSITE_LENGTH;
						}

						// Read instruction length
						glyph->instruction_length = MU_RBEU16(gdata);
						gdata += 2;
						// Verify with maxp
						if (glyph->instruction_length > font->maxp->max_size_of_instructions) {
							return MUTT_INVALID_GLYF_COMPOSITE_INSTRUCTION_LENGTH;
						}

						if (glyph->instruction_length > 0) {
							// Verify length for instructions
							req += glyph->instruction_length;
							if (header->length < req) {
								return MUTT_INVALID_GLYF_COMPOSITE_LENGTH;
							}

							// Memcpy instructions
							mu_memcpy(glyph->instructions, gdata, glyph->instruction_length);
							// Move data past instructions
							data += glyph->instruction_length;
						}
					} else {
						glyph->instruction_length = 0;
						glyph->instructions = 0;
					}

					// Copy over component count
					glyph->component_count = (uint16_m)component_count;
					// Write data written
					if (written) {
						*written = data-orig_data;
					}
					return MUTT_SUCCESS;
				}

				// Gets a component of a composite glyph
				// no_more is set to true when no more components exist
				MUDEF muttResult mutt_composite_component(muttFont* font, muttGlyphHeader* header, muByte** prog, muttComponentGlyph* component, muBool* no_more) {
					// Much of this code is very similar to the code in mutt_composite_glyph

					// So-far component length:
					uint32_m req = (*prog) - header->data;

					// Verify length for flags and glyphIndex
					req += 4;
					if (header->length < req) {
						return MUTT_INVALID_GLYF_COMPOSITE_LENGTH;
					}

					// Read flags
					component->flags = MU_RBEU16(*prog);
					*prog += 2;
					// Mark next component
					if (!(component->flags & MUTT_MORE_COMPONENTS)) {
						*no_more = MU_TRUE;
					}

					// Read glyphIndex
					component->glyph_index = MU_RBEU16(*prog);
					*prog += 2;
					// Verify index range
					if (component->glyph_index >= font->maxp->num_glyphs) {
						return MUTT_INVALID_GLYF_COMPOSITE_GLYPH_INDEX;
					}

					// Argument handling:
					if (component->flags & MUTT_ARG_1_AND_2_ARE_WORDS) {
						// 2 bytes per arg.
						req += 4;
						if (header->length < req) {
							return MUTT_INVALID_GLYF_COMPOSITE_LENGTH;
						}

						// Read values
						if (component->flags & MUTT_ARGS_ARE_XY_VALUES) {
							// int16 xy values
							component->argument1 = MU_RBES16(*prog);
							*prog += 2;
							component->argument2 = MU_RBES16(*prog);
							*prog += 2;
						} else {
							// uint16 point numbers
							component->argument1 = MU_RBEU16(*prog);
							*prog += 2;
							component->argument2 = MU_RBEU16(*prog);
							*prog += 2;
						}
					}
					else {
						// 1 byte per arg.
						req += 2;
						if (header->length < req) {
							return MUTT_INVALID_GLYF_COMPOSITE_LENGTH;
						}

						// Read values
						if (component->flags & MUTT_ARGS_ARE_XY_VALUES) {
							// int8 xy values
							component->argument1 = MU_RBES8((*prog)++);
							component->argument2 = MU_RBES8((*prog)++);
						} else {
							// uint8 point numbers
							component->argument1 = MU_RBEU8((*prog)++);
							component->argument2 = MU_RBEU8((*prog)++);
						}
					}

					// Transform data handling
					if (component->flags & MUTT_WE_HAVE_A_SCALE) {
						// One F2DOT14
						req += 2;
						if (header->length < req) {
							return MUTT_INVALID_GLYF_COMPOSITE_LENGTH;
						}

						// Flag exclusivity
						if ((component->flags & MUTT_WE_HAVE_AN_X_AND_Y_SCALE) || (component->flags & MUTT_WE_HAVE_A_TWO_BY_TWO)) {
							return MUTT_INVALID_GLYF_COMPOSITE_FLAGS;
						}

						// Read value
						component->scales[0] = MUTT_F2DOT14(*prog);
						*prog += 2;
					}
					else if (component->flags & MUTT_WE_HAVE_AN_X_AND_Y_SCALE) {
						// Two F2DOT14s
						req += 4;
						if (header->length < req) {
							return MUTT_INVALID_GLYF_COMPOSITE_LENGTH;
						}

						// Flag exclusivity
						if ((component->flags & MUTT_WE_HAVE_A_SCALE) || (component->flags & MUTT_WE_HAVE_A_TWO_BY_TWO)) {
							return MUTT_INVALID_GLYF_COMPOSITE_FLAGS;
						}

						// Read values
						component->scales[0] = MUTT_F2DOT14(*prog);
						*prog += 2;
						component->scales[1] = MUTT_F2DOT14(*prog);
						*prog += 2;
					}
					else if (component->flags & MUTT_WE_HAVE_A_TWO_BY_TWO) {
						// Four F2DOT14s
						req += 8;
						if (header->length < req) {
							return MUTT_INVALID_GLYF_COMPOSITE_LENGTH;
						}

						// Flag exclusivity
						if ((component->flags & MUTT_WE_HAVE_A_SCALE) || (component->flags & MUTT_WE_HAVE_AN_X_AND_Y_SCALE)) {
							return MUTT_INVALID_GLYF_COMPOSITE_FLAGS;
						}

						// Read values
						component->scales[0] = MUTT_F2DOT14(*prog);
						*prog += 2;
						component->scales[1] = MUTT_F2DOT14(*prog);
						*prog += 2;
						component->scales[2] = MUTT_F2DOT14(*prog);
						*prog += 2;
						component->scales[3] = MUTT_F2DOT14(*prog);
						*prog += 2;
					}

					return MUTT_SUCCESS;
				}

				// Gets the glyph index component by component within a composite glyph
				// no_more is set to true when no more components exist
				// ^ There will always be at least one component from what I'm aware
				MUDEF muttResult mutt_composite_component_glyph(muttFont* font, muttGlyphHeader* header, muByte** prog, uint16_m* glyph_index, muBool* no_more) {
					// prog is always at the beginning of a component record

					// Ensure length for flags + glyphIndex
					if (((*prog+4)-header->data) > header->length) {
						return MUTT_INVALID_GLYF_COMPOSITE_LENGTH;
					}

					// Read flags to see if there's any more after this
					uint16_m flags = MU_RBEU16(*prog);
					*prog += 2;
					if (!(flags & MUTT_MORE_COMPONENTS)) {
						*no_more = MU_TRUE;
					}
					// Read glyphIndex
					uint16_m read_glyph_index = MU_RBEU16(*prog);
					*prog += 2;
					if (read_glyph_index >= font->maxp->num_glyphs) {
						return MUTT_INVALID_GLYF_COMPOSITE_GLYPH_INDEX;
					}

					// Increment prog past argument1 and argument2
					if (flags & MUTT_ARG_1_AND_2_ARE_WORDS) {
						*prog += 4;
					} else {
						*prog += 2;
					}

					// Increment prog past transform data
					// (Not checking for flag exclusivity)
					if (flags & MUTT_WE_HAVE_A_SCALE) {
						*prog += 2;
					} else if (flags & MUTT_WE_HAVE_AN_X_AND_Y_SCALE) {
						*prog += 4;
					} else if (flags & MUTT_WE_HAVE_A_TWO_BY_TWO) {
						*prog += 8;
					}

					// (Not checking for length after argument1, argument2, and transform
					// since we're not reading data from there)

					// Write gylphIndex and give success
					*glyph_index = read_glyph_index;
					return MUTT_SUCCESS;
				}

				// Composite glyph memory maximum
				MUDEF uint32_m mutt_composite_glyph_max_size(muttFont* font) {
					return
						// components
						(font->maxp->max_component_elements*sizeof(muttComponentGlyph))
						// instructions
						+(font->maxp->max_size_of_instructions)
					;
				}

				// Glyph memory maximum
				MUDEF uint32_m mutt_glyph_max_size(muttFont* font) {
					// Simple:
					uint32_m s = mutt_simple_glyph_max_size(font);
					// Composite:
					uint32_m c = mutt_composite_glyph_max_size(font);
					// Return greatest
					return (s>c) ?(s) :(c);
				}

			/* Misc. */

				// Performs idDelta logic
				MUDEF uint16_m mutt_id_delta(uint16_m character_code, int16_m delta) {
					// Just return if idDelta is 0
					if (delta == 0) {
						return character_code;
					}

					// Do modulo 65536 on a larger type to add without overflow
					int32_m big_id = (int32_m)(character_code + delta) % 65536;
					// Prevent negative cases by adding 65536 for cases under 0
					if (big_id < 0) {
						big_id += 65536;
					}
					// Return that but now u16, which (I think) big_id is guaranteed to fit in
					return (uint16_m)big_id;
				}

		/* Raster API */

			/* Math */

				// Float epsilon macro used for countering floating point imprecision in line calculations
				// This value is unabashedly stolen from stb_truetype, but it works real well
				#define MUTTR_LINE_EPSILON32 (1.f/1024.f)

				// Double epsilon macro used for same purpose as MUTTR_LINE_EPSILON32; not used as of right now
				// This value should be tested and vetted more thoroughly if it's going to be used
				#define MUTTR_LINE_EPSILON64 0.00000001

				// Calculates IF a ray intersects a line
				static inline muBool muttR_LineRay(float ry, float y0, float y1) {
					// Any of these conditions must be true for intersection:
					return
						// Ray y is between y0 and y1 with forgiving epsilons:
						(ry >= y0-MUTTR_LINE_EPSILON32 && ry <= y1+MUTTR_LINE_EPSILON32) ||
						(ry >= y1-MUTTR_LINE_EPSILON32 && ry <= y0+MUTTR_LINE_EPSILON32)
						// Line is horizontal:
						|| (y1-y0 == 0.f)
					;
				}

				// Calculates the x-value intersection of a ray and a line
				static inline float muttR_LineRayHit(float ry, float x0, float y0, float x1, float y1) {
					// (https://math.stackexchange.com/a/2297532)
					return x0 + (
						((ry-y0) * (x1-x0))
						/
						(y1-y0)
					);
				}

				// Calculates the x- and y-value of a point on a Bezier curve (on, off, on) given t.
				static inline void muttR_Bezier(float t,
					float x0, float y0, float x1, float y1, float x2, float y2,
					float* x, float* y
				) {
					// I think this can be optimized, but idk...
					*x = ((1.f-t)*(1.f-t)*x0) + (2.f*(1.f-t)*t*x1) + (t*t*x2);
					*y = ((1.f-t)*(1.f-t)*y0) + (2.f*(1.f-t)*t*y1) + (t*t*y2);
				}

			/* Raster shape setup */

				/* Definitions */

					// A raster shape is made up of lines, a line is defined by two points
					struct muttR_Line {
						// 0 is bottom, 1 is top (y0 <= y1)
						float x0;
						float y0;
						float x1;
						float y1;
						// vec = y1-y0 (for direction)
						float vec;
					};
					typedef struct muttR_Line muttR_Line;

					// Used to sort the lines in increasing order of bottom point
					int muttR_CompareLines(const void* p, const void* q) {
						// Get lines
						muttR_Line* l0 = (muttR_Line*)p, * l1 = (muttR_Line*)q;
						// Return value to indicate greatest y1 value
						if (l0->y0 < l1->y0) {
							return -1;
						} else if (l0->y0 > l1->y0) {
							return 1;
						}
						return 0;
					}

					// Calculates the winding for a given line
					static inline int8_m muttR_LineWinding(float ry, muttR_Line* line) {
						// Don't consider winding if we're directly intersecting the high point of line
						if (mu_fabsf(ry - line->y0) <= MUTTR_LINE_EPSILON32) {
							return 0;
						}

						// Return winding order based on vector; down is +1, up is -1, neither is 0
						if (line->vec < 0.f) {
							return 1;
						} else if (line->vec > 0.f) {
							return -1;
						}
						return 0;
					}

					// A raster shape is just a collection of lines representing a glyph
					struct muttR_Shape {
						uint32_m num_lines;
						muttR_Line* lines;
						float x_max;
						float y_max;
					};
					typedef struct muttR_Shape muttR_Shape;

					// Sorts the lines of a shape in increasing order of bottom point
					// This does nothing currently; not sorting because it's usually
					// used for the active line list, but I failed to implement it
					void muttR_ShapeSort(muttR_Shape* shape) {
						// Just call qsort for the job
						//mu_qsort(shape->lines, shape->num_lines, sizeof(muttR_Line), muttR_CompareLines);
						return; if (shape) {}
					}

				/* Conversions */

					// Converts two points to a line
					void muttR_GlyphLine(muttR_Line* line, float x0, float y0, float x1, float y1) {
						// Determine lowest y0 value
						if (y0 < y1) {
							line->x0 = x0;
							line->y0 = y0;
							line->x1 = x1;
							line->y1 = y1;
						}
						else {
							line->x0 = x1;
							line->y0 = y1;
							line->x1 = x0;
							line->y1 = y0;
						}
						// Calculate vec
						line->vec = y1-y0;
					}

					// Converts three-point Bezier (on, off, on) to lines
					// - Constants:
					#define MUTTR_LINES_PER_BEZIER 25
					//#define MUTTR_LINES_PER_BEZIER 1
					static const float MUTTR_LINES_PER_BEZIER_F = ((float)MUTTR_LINES_PER_BEZIER);
					static const float MUTTR_LINES_PER_BEZIER_INV = 1.f / ((float)MUTTR_LINES_PER_BEZIER);
					// - Function:
					static inline void muttR_GlyphCurve(muttR_Line* line, float x0, float y0, float x1, float y1, float x2, float y2) {
						// Loop through each line per Bezier
						for (uint32_m l = 0; l < MUTTR_LINES_PER_BEZIER; ++l) {
							// Calculate Bezier for the first point
							float t = ((float)l) / MUTTR_LINES_PER_BEZIER_F;
							float bx0, by0;
							muttR_Bezier(t, x0, y0, x1, y1, x2, y2, &bx0, &by0);
							
							// + Second point
							float bx1, by1;
							muttR_Bezier(t+MUTTR_LINES_PER_BEZIER_INV, x0, y0, x1, y1, x2, y2, &bx1, &by1);

							// Make line based on this strip of the Bezier
							muttR_GlyphLine(line++, bx0, by0, bx1, by1);
						}
					}

					// Gets the next point of a glyph based on the contour ends
					static inline muttRPoint* muttR_GlyphNextPoint(muttRGlyph* glyph, uint32_m p, uint32_m i, uint32_m c) {
						// The next point is p incremented
						muttRPoint* pn = &glyph->points[p+i];
						// ...unless this goes over the contour's end...
						if (p+i > glyph->contour_ends[c]) {
							// ..in which case the next point wraps back around to the first
							// point of the contour
							pn -= glyph->contour_ends[c]+1;
							if (c > 0) {
								pn += glyph->contour_ends[c-1]+1;
							}
						}
						return pn;
					}

					// Calculates the amount of lines needed to represent a glyph
					uint32_m muttR_GlyphLineCount(muttRGlyph* glyph) {
						// Keep count of lines:
						uint32_m count = 0;
						// Keep count of contours:
						uint16_m c = 0;

						// Loop through each point
						for (uint32_m p = 0; p < glyph->num_points;) {
							// Increment contour ID if necessary
							if (p > glyph->contour_ends[c]) {
								// Go to beginning of contour to avoid over-counting sometimes
								p = glyph->contour_ends[c]+1;
								++c;
							}

							// Get current point
							muttRPoint* p0 = &glyph->points[p];

							// If current point is ON curve (ON...):
							if (p0->flags & MUTTR_ON_CURVE) {
								// Get next point
								muttRPoint* p1 = muttR_GlyphNextPoint(glyph, p, 1, c);
								// If next point is ON curve (ON, ON):
								if (p1->flags & MUTTR_ON_CURVE) {
									// One more line, go to next point
									++count;
									++p;
									continue;
								}

								// If we're here, the next point is OFF curve (ON, OFF...)
								// It must be a Bezier, and we must go forward two points
								count += MUTTR_LINES_PER_BEZIER;
								p += 2;
								continue;
							}

							// Get previous point
							muttRPoint* pn1 = &glyph->points[p-1];
							muttRPoint n1;
							// Edge case for point being 0:
							if (p == 0) {
								// Wrap to end of this contour
								pn1 = &n1;
								n1.flags = glyph->points[glyph->contour_ends[0]].flags;
							}
							// Edge case for point being first point of this contour:
							else if (c > 0 && (uint16_m)(glyph->contour_ends[c-1]+1) == p) {
								// Wrap to end of this contour
								pn1 = &n1;
								n1.flags = glyph->points[glyph->contour_ends[c]].flags;
							}

							// Skip is previous point is ON curve;
							// this avoids duplicate lines from what I'm aware
							if (pn1->flags & MUTTR_ON_CURVE) {
								++p;
								continue;
							}

							// If we're here, we started on an OFF point
							// It must be a Bezier, and we must go forward one point
							count += MUTTR_LINES_PER_BEZIER;
							++p;
							//continue;
						}

						// Return final count
						return count;
					}

					// Converts an rglyph to a shape
					muttResult muttR_ShapeCreate(muttRGlyph* glyph, muttR_Shape* shape) {
						// Calculate number of lines needed
						shape->num_lines = muttR_GlyphLineCount(glyph);
						// Allocate lines
						shape->lines = (muttR_Line*)mu_malloc(sizeof(muttR_Line)*shape->num_lines);
						if (!shape->lines) {
							return MUTT_FAILED_MALLOC;
						}

						// Loop through each point
						uint32_m c = 0; // (contour tracker)
						muttR_Line* l = shape->lines; // (shape tracker)
						for (uint32_m p = 0; p < glyph->num_points;) {
							// Increment contour ID if necessary
							if (p > glyph->contour_ends[c]) {
								// Go to beginning of contour to avoid over-counting sometimes
								p = glyph->contour_ends[c]+1;
								++c;
							}

							// Get current point
							muttRPoint* p0 = &glyph->points[p];

							// If current point is ON curve (ON...):
							if (p0->flags & MUTTR_ON_CURVE) {
								// Get next point
								muttRPoint* p1 = muttR_GlyphNextPoint(glyph, p, 1, c);
								// If next point is ON curve (ON, ON):
								if (p1->flags & MUTTR_ON_CURVE) {
									// Form line between two points and move on by one
									muttR_GlyphLine(l++, p0->x, p0->y, p1->x, p1->y);
									++p;
									continue;
								}

								// If we're here, the next point is OFF curve (ON, OFF...)
								// Get next-next point
								muttRPoint* p2 = muttR_GlyphNextPoint(glyph, p, 2, c);
								// If next-next point is ON the curve (ON, OFF, ON):
								if (p2->flags & MUTTR_ON_CURVE) {
									// Form basic Bezier and move on
									muttR_GlyphCurve(l, p0->x, p0->y, p1->x, p1->y, p2->x, p2->y);
									l += MUTTR_LINES_PER_BEZIER;
									p += 2;
									continue;
								}

								// If we're here, the next-next point is OFF the curve: (ON, OFF, OFF):
								else {
									// Bezier, with last point needing to be mid-pointed
									muttR_GlyphCurve(l, p0->x, p0->y, p1->x, p1->y,
										(p1->x + p2->x) / 2.f, (p1->y + p2->y) / 2.f
									);

									l += MUTTR_LINES_PER_BEZIER;
									p += 2;
									continue;
								}
							}

							// If we're here, the current point is OFF curve (OFF...)
							// Get previous point:
							muttRPoint* pn1 = &glyph->points[p-1];
							muttRPoint n1;
							// Edge case for point being 0:
							if (p == 0) {
								// Wrap to last point of this contour
								pn1 = &n1;
								n1.x = glyph->points[glyph->contour_ends[0]].x;
								n1.y = glyph->points[glyph->contour_ends[0]].y;
								n1.flags = glyph->points[glyph->contour_ends[0]].flags;
							}
							// Edge case for point being first of contour:
							else if (c > 0 && (uint16_m)(glyph->contour_ends[c-1]+1) == p) {
								// Wrap to last point of this contour
								pn1 = &n1;
								n1.x = glyph->points[glyph->contour_ends[c]].x;
								n1.y = glyph->points[glyph->contour_ends[c]].y;
								n1.flags = glyph->points[glyph->contour_ends[c]].flags;
							}

							// Leave if previous point is ON;
							// this avoids duplicate lines from what I'm aware
							if (pn1->flags & MUTTR_ON_CURVE) {
								++p;
								continue;
							}

							// Get next point:
							muttRPoint* p1 = muttR_GlyphNextPoint(glyph, p, 1, c);

							// x0 and y0 must be midpointed between pn1 and p0 since
							// both are OFF curve:
							float x0 = (pn1->x + p0->x) / 2.f;
							float y0 = (pn1->y + p0->y) / 2.f;
							// x1 and y1 are just p0 since it's OFF curve:
							float x1 = p0->x;
							float y1 = p0->y;
							// x2 and y2 are just p1:
							float x2 = p1->x;
							float y2 = p1->y;
							// ...unless it's OFF curve, in which case it must be
							// midpointed:
							if (!(p1->flags & MUTTR_ON_CURVE)) {
								x2 = (x1 + x2) / 2.f;
								y2 = (y1 + y2) / 2.f;
							}

							// Bezier and move on:
							muttR_GlyphCurve(l, x0, y0, x1, y1, x2, y2);
							l += MUTTR_LINES_PER_BEZIER;
							++p;
							//continue;
						}

						// Sort all lines
						muttR_ShapeSort(shape);
						// Set max x/y
						shape->x_max = glyph->x_max;
						shape->y_max = glyph->y_max;

						return MUTT_SUCCESS;
					}

					// Frees all memory used by a shape
					void muttR_ShapeDestroy(muttR_Shape* shape) {
						if (shape->lines) {
							mu_free(shape->lines);
						}
					}

			/* Intersection/Hit logic */

				// A hit:
				struct muttR_Hit {
					// The x-value of the intersection
					float x;
					// The line which it intersected with
					uint32_m l;
				};
				typedef struct muttR_Hit muttR_Hit;

				// Used to sort intersections in order of increasing x-value
				int muttR_HitCompare(const void* p, const void* q) {
					// Get hits
					muttR_Hit* h0 = (muttR_Hit*)p, * h1 = (muttR_Hit*)q;
					// Indicate which hit's x-value is greater
					if (h0->x < h1->x) {
						return -1;
					} else if (h0->x > h1->x) {
						return 1;
					}
					return 0;
				}

				// Updates the active line list based on the ray
				// This function should totally work from what I'm aware, but it
				// seems to not work properly. Don't know why
				static inline void muttR_ActiveLines(muttR_Line* lines, uint32_m num_lines, uint32_m* first, uint32_m* len, float ry) {
					// Increase length to include possible new in-range lines
					while (*first + *len < num_lines) {
						if (ry >= lines[*first + *len].y0 - MUTTR_LINE_EPSILON32) {
							++*len;
						} else {
							break;
						}
					}

					// Move up the first active line until we're in range of the next nearest line
					while (*len != 0) {
						if (ry > lines[*first].y1 + MUTTR_LINE_EPSILON32) {
							++*first;
							--*len;
						} else {
							break;
						}
					}
				}

				// Calculates all hits for a given ray and its active lines
				// Returns amount of hits, and gives winding as well
				static inline uint32_m muttR_Hits(muttR_Line* lines, uint32_m num_lines, float ry, muttR_Hit* hits, int32_m* winding) {
					uint32_m num_hits = 0;

					// Loop through each line
					for (uint32_m l = 0; l < num_lines; ++l) {
						// If ray intersects with line:
						if (muttR_LineRay(ry, lines[l].y0, lines[l].y1)) {
							// Add intersection values
							float x = muttR_LineRayHit(ry, lines[l].x0, lines[l].y0, lines[l].x1, lines[l].y1);
							// Add if intersection is valid
							if (x >= 0.f) {
								hits[num_hits  ].x = x;
								hits[num_hits++].l = l;
							}
						}
					}

					// Sort hits
					mu_qsort(hits, num_hits, sizeof(muttR_Hit), muttR_HitCompare);

					// Calculate total winding order
					*winding = 0;
					for (uint32_m hitw = 0; hitw < num_hits; ++hitw) {
						winding += muttR_LineWinding(ry, &lines[hits[hitw].l]);
					}

					return num_hits;
				}

			/* Rasterization per method */

				// MUTTR_FULL_PIXEL_BI_LEVEL
				muttResult muttR_FullPixelBiLevel(muttR_Shape* shape, muttRBitmap* bitmap, uint8_m adv, uint8_m in, uint8_m out) {
					// Allocate hit tracker
					muttR_Hit* hits = (muttR_Hit*)mu_malloc(shape->num_lines*sizeof(muttR_Hit));
					if (!hits) {
						return MUTT_FAILED_MALLOC;
					}

					// Initialize active lines
					//uint32_m first_line = 0;
					//uint32_m line_len = 0;

					// Loop through each horizontal strip from bottom to top
					for (uint32_m h = 0; h < bitmap->height; ++h) {
						// Calculate horizontal pixel offset
						uint64_m hpix_offset = bitmap->stride*((bitmap->height-h)-1);

						// Just fill all x-values with out if the height is now outside of the glyph range
						// (+ double-pixel extra for bleeding and ceiling)
						if (h > shape->y_max+2) {
							mu_memset(&bitmap->pixels[hpix_offset], out, bitmap->width*adv);
							continue;
						}

						// Calculate y-value of ray (middle of pixel)
						float ray_y = ((float)h) + .5f;

						// Update active line list
						//muttR_ActiveLines(shape->lines, shape->num_lines, &first_line, &line_len, ray_y);
						// ^ Not doing this because it doesn't work for some reason...
						// Calculate all hits with active lines (+ winding)
						int32_m winding;
						uint32_m num_hits = muttR_Hits(shape->lines, shape->num_lines, ray_y, hits, &winding);

						// Loop through each x-value
						uint32_m ih = 0; // (Upcoming hit)
						for (uint32_m w = 0; w < bitmap->width; ++w) {
							// Just fill all remaining x-values with out if width is now outside of glyph range
							// (+ double-pixel extra for bleeding and ceiling)
							if (w > shape->x_max+2) {
								mu_memset(&bitmap->pixels[hpix_offset+(w*adv)], out, (bitmap->width-w)*adv);
								continue;
							}

							// Calculate x-coordinate (middle of pixel)
							float ray_x = ((float)w) + .5f;

							// Skip over every hit we've passed, and remove their windings
							// "ray_x > ..." ensures rule 2 of scan converting:
							// "If a contour falls exactly on a pixel’s center, that pixel is turned on."
							while (ih < num_hits && ray_x > hits[ih].x) {
								winding -= muttR_LineWinding(ray_y, &shape->lines[hits[ih++].l]);
							}

							// Set pixel to whether or not we're in glyph
							// Winding == 0 means in the glyph, and vice versa
							if (winding == 0) {
								mu_memset(&bitmap->pixels[hpix_offset+(w*adv)], out, adv);
							} else {
								mu_memset(&bitmap->pixels[hpix_offset+(w*adv)], in, adv);
							}
						}
					}

					mu_free(hits);
					return MUTT_SUCCESS;
				}

				// A ray
				struct muttR_Ray {
					// Its y-value
					float y;
					// Each line its hit
					muttR_Hit* hits;
					// Number of hits
					uint32_m num_hits;
					// Its winding
					int32_m winding;
					// Upcoming hit
					uint32_m ih;
				};
				typedef struct muttR_Ray muttR_Ray;

				// Fills information about a ray
				static inline void muttR_PixelRayCalc(muttR_Shape* shape, muttR_Ray* ray, float ray_y) {
					// Fill y-value
					ray->y = ray_y;
					// Calculate hit and winding
					ray->num_hits = muttR_Hits(shape->lines, shape->num_lines, ray_y, ray->hits, &ray->winding);
					// Set upcoming hit to 0
					ray->ih = 0;
				}

				// MUTTR_FULL_PIXEL_AANXN inner handling
				void muttR_FullPixelAANXNInner(muttR_Shape* shape, muttRBitmap* bitmap, uint8_m adv, float in, float out, uint8_m vs, uint8_m hs, muttR_Ray* rays) {
					// Weight of each sample:
					float weight = 1.f / ((float)(vs*hs));

					// Loop through each horizontal strip from bottom to top
					for (uint32_m h = 0; h < bitmap->height; ++h) {
						// Calculate horizontal pixel offset
						uint64_m hpix_offset = bitmap->stride*((bitmap->height-h)-1);

						// Just fill all x-values with out if the height is now outside of the glyph range
						// (+ double-pixel extra for bleeding and ceiling)
						if (h > shape->y_max+2) {
							mu_memset(&bitmap->pixels[hpix_offset], out, bitmap->width*adv);
							continue;
						}

						// Calculate each ray
						for (uint8_m r = 0; r < hs; ++r) {
							// a=\left[\frac{1}{n+1},\frac{2}{n+1}...\frac{n}{n+1}\right]
							muttR_PixelRayCalc(shape, rays+r, ((float)h) + (((float)(r+1)) / ((float)(hs+1))));
						}

						// Loop through each x-value
						for (uint32_m w = 0; w < bitmap->width; ++w) {
							// Just fill all remaining x-values with out if width is now outside of glyph range
							// (+ double-pixel extra for bleeding and ceiling)
							if (w > shape->x_max+2) {
								mu_memset(&bitmap->pixels[hpix_offset+(w*adv)], out, (bitmap->width-w)*adv);
								continue;
							}

							// Percentage amount the pixel is in
							float in_per = 0.f;

							// Loop through each vertical sample
							for (uint8_m x = 0; x < vs; ++x) {
								// Calculate x-coordinate
								// (Same math as y-ray)
								float ray_x = ((float)w) + (((float)(x+1)) / ((float)(vs+1)));

								// Loop through each horizontal sample
								for (uint8_m y = 0; y < hs; ++y) {
									// Skip over each hit, removing windings
									while (rays[y].ih < rays[y].num_hits && ray_x > rays[y].hits[rays[y].ih].x) {
										// winding -= muttR_LineWinding(ray_y, &shape->lines[hits[ih++].l]);
										rays[y].winding -= muttR_LineWinding(rays[y].y, &shape->lines[rays[y].hits[rays[y].ih++].l]);
									}
									// Add if sample is in
									in_per += (rays[y].winding==0) ?(0.f) :(weight);
								}
							}

							// Calculate pixel color based on how much the pixel is in
							// \left(a\right)\left(m_{1}-m_{0}\right)+m_{0}
							for (uint8_m a = 0; a < adv-1; ++a) {
								bitmap->pixels[hpix_offset+(w*adv)+a] = 255;
							}
							bitmap->pixels[hpix_offset+(w*adv)+(adv-1)] = (in_per * (in-out)) + out;
						}
					}
				}

				// MUTTR_FULL_PIXEL_AANXN
				muttResult muttR_FullPixelAANXN(muttR_Shape* shape, muttRBitmap* bitmap, uint8_m adv, uint8_m in, uint8_m out, uint8_m vs, uint8_m hs) {
					// Allocate hits
					muttR_Hit* hits = (muttR_Hit*)mu_malloc(shape->num_lines * hs * sizeof(muttR_Hit));
					if (!hits) {
						return MUTT_FAILED_MALLOC;
					}
					// Allocate rays
					muttR_Ray* rays = (muttR_Ray*)mu_malloc(hs * sizeof(muttR_Ray));
					if (!rays) {
						mu_free(hits);
						return MUTT_FAILED_MALLOC;
					}

					// Fill all rays with their hit pointer
					for (uint8_m r = 0; r < hs; ++r) {
						rays[r].hits = hits + (shape->num_lines * r);
					}

					// Rasterize
					muttR_FullPixelAANXNInner(shape, bitmap, adv, (float)in, (float)out, vs, hs, rays);

					// Deallocate and return
					mu_free(rays);
					mu_free(hits);
					return MUTT_SUCCESS;
				}

			/* Rasterization */

				// Converts channels to advance
				uint8_m muttR_ChannelsAdv(muttRChannels channels) {
					switch (channels) {
						default: return 1; break;
						case MUTTR_R: return 1; break;
						case MUTTR_RGB: return 3; break;
						case MUTTR_RGBA: return 4; break;
					}
				}

				// Rasterizes a glyph
				MUDEF muttResult mutt_raster_glyph(muttRGlyph* glyph, muttRBitmap* bitmap, muttRMethod method) {
					// Convert rglyph to shape
					muttR_Shape shape;
					muttResult res = muttR_ShapeCreate(glyph, &shape);
					if (mutt_result_is_fatal(res)) {
						return res;
					}

					// Per-pixel advance based on channels:
					uint8_m adv = muttR_ChannelsAdv(bitmap->channels);
					// In/Out value based on io_color:
					uint8_m in  = (bitmap->io_color == MUTTR_BW) ?(255) :(0);
					uint8_m out = ~in;

					// Render shape based on method
					switch (method) {
						// Unrecognized method:
						default: {
							res = MUTT_UNKNOWN_RASTER_METHOD;
						} break;

						// Full-pixel bi-level
						case MUTTR_FULL_PIXEL_BI_LEVEL: {
							res = muttR_FullPixelBiLevel(&shape, bitmap, adv, in, out);
						} break;

						// Full-pixel AA 2x2
						case MUTTR_FULL_PIXEL_AA2X2: {
							res = muttR_FullPixelAANXN(&shape, bitmap, adv, in, out, 2, 2);
						} break;
						// Full-pixel AA 4x4
						case MUTTR_FULL_PIXEL_AA4X4: {
							res = muttR_FullPixelAANXN(&shape, bitmap, adv, in, out, 4, 4);
						} break;
						// Full-pixel AA 8x8
						case MUTTR_FULL_PIXEL_AA8X8: {
							res = muttR_FullPixelAANXN(&shape, bitmap, adv, in, out, 8, 8);
						} break;
					}

					// Free resources and return latest non-fatal result
					muttR_ShapeDestroy(&shape);
					return res;
				}

			/* Conversion */

				// FUnits to pixel-units
				MUDEF float mutt_funits_to_punits(muttFont* font, float funits, float point_size, float ppi) {
					return point_size * funits * ppi / (72.f * font->head->units_per_em);
				}

				// Gets the ascent/descent/lsb/advance-width of an rglyph for a glyph ID
				MUDEF void mutt_rglyph_metrics(muttFont* font, muttGlyphHeader* header, uint16_m glyph_id, muttRGlyph* rglyph, float point_size, float ppi) {
					// Offsets
					float py = -mutt_funits_to_punits(font, header->y_min, point_size, ppi) + 1.f;

					// Ascender + Descender
					rglyph->ascender  = py + mutt_funits_to_punits(font, font->hhea->ascender , point_size, ppi);
					rglyph->descender = py + mutt_funits_to_punits(font, font->hhea->descender, point_size, ppi);

					// Left-side bearing + Advance width
					if (glyph_id >= font->hhea->number_of_hmetrics) {
						// lsb is in lsb array:
						rglyph->lsb = mutt_funits_to_punits(font, font->hmtx->left_side_bearings[glyph_id - font->hhea->number_of_hmetrics], point_size, ppi);
						// Advance width is last advance width
						if (font->hhea->number_of_hmetrics > 0) {
							rglyph->advance_width = mutt_funits_to_punits(font, font->hmtx->hmetrics[font->hhea->number_of_hmetrics - 1].advance_width, point_size, ppi);
						} else {
							// Defaulting on 0 for advance width with no hmetrics specified
							rglyph->advance_width = 0;
						}
					} else {
						// lsb is in hmetrics array:
						rglyph->lsb = mutt_funits_to_punits(font, font->hmtx->hmetrics[glyph_id].lsb, point_size, ppi);
						// As well as advance width:
						rglyph->advance_width = mutt_funits_to_punits(font, font->hmtx->hmetrics[glyph_id].advance_width, point_size, ppi);
					}
				}

				/* Simple */

					// Simple glyph -> raster glyph
					MUDEF muttResult mutt_simple_rglyph(muttFont* font, muttGlyphHeader* header, muttSimpleGlyph* glyph, muttRGlyph* rglyph, float point_size, float ppi, muByte* data, uint32_m* written) {
						// For written calculations:
						if (!data) {
							uint32_m write = 0;
							// points
							write += sizeof(muttRPoint) * (glyph->end_pts_of_contours[header->number_of_contours-1] + 1);
							// contour_ends
							write += sizeof(uint16_m) * header->number_of_contours;
							// Write data needed
							*written = write;
							return MUTT_SUCCESS;
						}

						// Get data for arrays
						muByte* orig_data = data;
						// - points
						rglyph->num_points = glyph->end_pts_of_contours[header->number_of_contours-1] + 1;
						rglyph->points = (muttRPoint*)data;
						data += sizeof(muttRPoint) * ((uint32_m)rglyph->num_points);
						// - contour_ends
						rglyph->num_contours = header->number_of_contours;
						rglyph->contour_ends = (uint16_m*)data;
						data += sizeof(uint16_m) * ((uint32_m)rglyph->num_contours);
						// Write written data amount
						if (written) {
							*written = data-orig_data;
						}

						// Calculate point offsets based on glyph's min/max values
						float px = -mutt_funits_to_punits(font, header->x_min, point_size, ppi) + 1.f;
						float py = -mutt_funits_to_punits(font, header->y_min, point_size, ppi) + 1.f;

						// Loop through each point
						for (uint16_m p = 0; p < rglyph->num_points; ++p) {
							// X and Y
							rglyph->points[p].x = px + mutt_funits_to_punits(font, glyph->points[p].x, point_size, ppi);
							rglyph->points[p].y = py + mutt_funits_to_punits(font, glyph->points[p].y, point_size, ppi);
							// Flags
							rglyph->points[p].flags = (glyph->points[p].flags & MUTT_ON_CURVE_POINT) ?(MUTTR_ON_CURVE) :(0);
						}

						// Copy over contour ends
						mu_memcpy(rglyph->contour_ends, glyph->end_pts_of_contours, sizeof(uint16_m) * ((uint32_m)rglyph->num_contours));

						// Caclculate x_max and y_max
						rglyph->x_max = px + mutt_funits_to_punits(font, header->x_max, point_size, ppi);
						rglyph->y_max = py + mutt_funits_to_punits(font, header->y_max, point_size, ppi);

						return MUTT_SUCCESS;
					}

					// Memory maximum
					MUDEF uint32_m mutt_simple_rglyph_max(muttFont* font) {
						return
							// points
							(sizeof(muttRPoint) * font->maxp->max_points)
							// contour_ends
							+ (sizeof(uint16_m) * font->maxp->max_contours)
						;
					}

				/* Composite */

					// Used to pass data between function calls within composite glyph processing
					struct muttR_CompProg {
						// Progressives (move up between rglyphs):
						uint32_m num_contours;
						uint32_m num_points;
						uint16_m* contour_ends;
						muttRPoint* points;
						float x_min;
						float y_min;
						float x_max;
						float y_max;
						// Actual rglyph being filled in:
						muttRGlyph* rglyph;
						// Temp simple glyph mem:
						muByte* temp_simple_max;
						// Max values:
						uint16_m max_contours;
						uint16_m max_points;
					};
					typedef struct muttR_CompProg muttR_CompProg;

					// Initializes a CompProg struct
					void muttR_CompProg_init(muttR_CompProg* prog, muttRGlyph* rglyph, muByte* temp_simple_max, muttFont* font) {
						// Set numerical progressives to 0
						prog->num_contours = 0;
						prog->num_points = 0;
						prog->x_min = 0.f;
						prog->y_min = 0.f;
						prog->x_max = 0.f;
						prog->y_max = 0.f;
						// Set initial progressive pointers
						prog->contour_ends = rglyph->contour_ends;
						prog->points = rglyph->points;
						// Rglyph
						prog->rglyph = rglyph;
						rglyph->num_points = 0;
						rglyph->num_contours = 0;
						// Temp simple max
						prog->temp_simple_max = temp_simple_max;
						// Max values
						prog->max_contours = font->maxp->max_composite_contours;
						prog->max_points = font->maxp->max_composite_points;
					}

					// Converts simple glyph component to rglyph within the CompProg
					// Glyph must have at least 1 contour
					// This process gives the raw TrueType coordinates in float form;
					// point and PPI are accounted for afterwards to separate that
					// logic from the point alignment and such.
					muttResult mutt_composite_simple_rglyph(muttR_CompProg* prog, muttGlyphHeader* header, muttSimpleGlyph* glyph, muttComponentGlyph* comp) {
						// Account for number of contours
						prog->num_contours += header->number_of_contours;
						if (prog->num_contours > prog->max_contours) {
							return MUTT_INVALID_RGLYPH_COMPOSITE_CONTOUR_COUNT;
						}
						// Account for number of points
						prog->num_points += glyph->end_pts_of_contours[header->number_of_contours-1]+1;
						if (prog->num_points > prog->max_points) {
							return MUTT_INVALID_RGLYPH_COMPOSITE_POINT_COUNT;
						}
						uint16_m num_points = glyph->end_pts_of_contours[header->number_of_contours-1]+1;

						// Calculate scales
						float xscale=1.f, scale01=0.f, scale10=0.f, yscale=1.f;

						if (comp->flags & MUTT_WE_HAVE_A_SCALE) {
							// One scale applied to x and y
							xscale = yscale = comp->scales[0];
						}
						else if (comp->flags & MUTT_WE_HAVE_AN_X_AND_Y_SCALE) {
							// Individual x and y scale
							xscale = comp->scales[0];
							yscale = comp->scales[1];
						}
						else if (comp->flags & MUTT_WE_HAVE_A_TWO_BY_TWO) {
							// Full 2x2 affine transformation
							xscale  = comp->scales[0];
							scale01 = comp->scales[1];
							scale10 = comp->scales[2];
							yscale  = comp->scales[3];
						}


						// Calculate argument1 & argument2 offsets
						float argument1, argument2;
						if (comp->flags & MUTT_ARGS_ARE_XY_VALUES) {
							// Just read the offsets
							argument1 = comp->argument1;
							argument2 = comp->argument2;
							// Possible scaling:
							if (comp->flags & MUTT_SCALED_COMPONENT_OFFSET) {
								float n_argument1 = (xscale  * argument1) + (scale10 * argument2);
								float n_argument2 = (scale01 * argument1) + (yscale  * argument2);
								argument1 = n_argument1;
								argument2 = n_argument2;
								// Possible rounding:
								if (comp->flags & MUTT_ROUND_XY_TO_GRID) {
									argument1 = mu_roundf(argument1);
									argument2 = mu_roundf(argument2);
								}
							}
						}
						else {
							// argument1 is parent (prog->rglyph) point number
							// argument2 is child (glyph) point number
							// Offset should align these two points

							// NOTE: phantom points can be used in this process, but
							// mutt v1.0.0 doesn't have support for gvar (which, as
							// far as I can tell, is needed to process the phantom
							// points); composite glyphs that depend on phantom
							// points will, for now, render incorrectly or possibly
							// not render successfully at all.

							// - Parent point -
							// Ensure point number is in range
							if (comp->argument1 >= prog->rglyph->num_points) {
								return MUTT_INVALID_RGLYPH_COMPOSITE_COMPONENT_ARGUMENT1;
							}
							// Get parent x and y
							float parent_x = prog->rglyph->points[comp->argument1].x;
							float parent_y = prog->rglyph->points[comp->argument1].y;

							// - Child point -
							// Ensure point number is in range
							if (comp->argument2 >= num_points) {
								return MUTT_INVALID_RGLYPH_COMPOSITE_COMPONENT_ARGUMENT2;
							}
							// Get child x and y
							float t_child_x = glyph->points[comp->argument2].x;
							float t_child_y = glyph->points[comp->argument2].y;
							// Calculate it with scales applied so that point is still
							// aligned with scales
							float child_x = (xscale  * t_child_x) + (scale10 * t_child_y);
							float child_y = (scale01 * t_child_x) + (yscale  * t_child_y);

							// Calcaulte argument1 & argument2 to be offsets that align
							// the child point to the parent point
							argument1 = parent_x - child_x;
							argument2 = parent_y - child_y;
						}

						// Loop through each point
						for (uint16_m p = 0; p < num_points; ++p) {
							// Calculate scaled point
							float point_x = glyph->points[p].x;
							float point_y = glyph->points[p].y;
							prog->points[p].x = (xscale  * point_x) + (scale10 * point_y);
							prog->points[p].y = (scale01 * point_x) + (yscale  * point_y);
							// Add offsets
							prog->points[p].x += argument1;
							prog->points[p].y += argument2;

							// Flags of this point
							prog->points[p].flags = (glyph->points[p].flags & MUTT_ON_CURVE_POINT) ?(MUTTR_ON_CURVE) :(0);
						}
						// Push progressive points pointer
						prog->points += num_points;

						// Loop through each contour end
						for (uint16_m c = 0; c < header->number_of_contours; ++c) {
							// Each contour end is the glyph-relative contour end
							// with the offset of all the previous points
							// I'm bad at explaining numbers
							prog->contour_ends[c] = prog->rglyph->num_points + glyph->end_pts_of_contours[c];
						}
						// Push progressive contour ends pointer
						prog->contour_ends += header->number_of_contours;

						// Add to rglyph
						prog->rglyph->num_points += num_points;
						prog->rglyph->num_contours += header->number_of_contours;
						return MUTT_SUCCESS;
					}

					// Processes composite component for CompProg
					muttResult mutt_component_rglyph(muttFont* font, muttR_CompProg* prog, muttComponentGlyph* comp, uint32_m depth) {
						muttResult res = MUTT_SUCCESS;

						// Store point stuff BEFORE all points for this component are processed
						uint16_m prev_num_points = prog->num_points;
						muttRPoint* prev_points = prog->points;

						// Get header of glyph index
						muttGlyphHeader header;
						res = mutt_glyph_header(font, comp->glyph_index, &header);
						if (mutt_result_is_fatal(res)) {
							return res;
						}

						// Simple glyph with contours:
						if (header.number_of_contours > 0) {
							// Get simple glyph
							muttSimpleGlyph glyph;
							res = mutt_simple_glyph(font, &header, &glyph, prog->temp_simple_max, 0);
							if (mutt_result_is_fatal(res)) {
								return res;
							}

							// Process simple glyph
							res = mutt_composite_simple_rglyph(prog, &header, &glyph, comp);
							if (mutt_result_is_fatal(res)) {
								return res;
							}

							return res;
						}

						// Simple glyph with no contours:
						else if (header.number_of_contours == 0) {
							// Do nothing for this guy
							return res;
						}

						// Composite glyph
						else {
							// Increment depth tracker
							if (++depth > font->maxp->max_component_depth) {
								return MUTT_INVALID_RGLYPH_COMPOSITE_DEPTH;
							}

							// Loop through each component
							uint32_m component_count = 0;
							muBool no_more = MU_FALSE;
							muByte* bprog = header.data;
							while (!no_more) {
								// Verify incremented component count
								if (++component_count > font->maxp->max_component_elements) {
									return MUTT_INVALID_RGLYPH_COMPOSITE_COMPONENT_COUNT;
								}

								// Get individual component
								muttComponentGlyph this_component;
								res = mutt_composite_component(font, &header, &bprog, &this_component, &no_more);
								if (mutt_result_is_fatal(res)) {
									return res;
								}
								// Process component with this current function
								// (Scary recursion)
								res = mutt_component_rglyph(font, prog, &this_component, depth);
								if (mutt_result_is_fatal(res)) {
									return res;
								}
							}
						}

						// This code is considerably similar to mutt_composite_simple_rglyph

						// Calculate this component's values
						uint16_m num_points = prog->num_points - prev_num_points;

						// Calculate scales
						float xscale=1.f, scale01=0.f, scale10=0.f, yscale=1.f;

						if (comp->flags & MUTT_WE_HAVE_A_SCALE) {
							// One scale applied to x and y
							xscale = yscale = comp->scales[0];
						}
						else if (comp->flags & MUTT_WE_HAVE_AN_X_AND_Y_SCALE) {
							// Individual x and y scale
							xscale = comp->scales[0];
							yscale = comp->scales[1];
						}
						else if (comp->flags & MUTT_WE_HAVE_A_TWO_BY_TWO) {
							// Full 2x2 affine transformation
							xscale  = comp->scales[0];
							scale01 = comp->scales[1];
							scale10 = comp->scales[2];
							yscale  = comp->scales[3];
						}

						// Calculate argument1 and argument2 offsets
						float argument1, argument2;
						if (comp->flags & MUTT_ARGS_ARE_XY_VALUES) {
							// Just read the offsets
							argument1 = comp->argument1;
							argument2 = comp->argument2;
							// Possible scaling:
							if (comp->flags & MUTT_SCALED_COMPONENT_OFFSET) {
								float n_argument1 = (xscale  * argument1) + (scale10 * argument2);
								float n_argument2 = (scale01 * argument1) + (yscale  * argument2);
								argument1 = n_argument1;
								argument2 = n_argument2;
								// Possible rounding:
								if (comp->flags & MUTT_ROUND_XY_TO_GRID) {
									argument1 = mu_roundf(argument1);
									argument2 = mu_roundf(argument2);
								}
							}
						}
						else {
							// argument1 is parent (rglyph->points, prev_num_points)
							// argument2 is child (prev_points, num_points)

							// - Parent point -
							// Ensure point range
							if (comp->argument1 >= prev_num_points) {
								return MUTT_INVALID_RGLYPH_COMPOSITE_COMPONENT_ARGUMENT1;
							}
							// Get parent x and y
							float parent_x = prog->rglyph->points[comp->argument1].x;
							float parent_y = prog->rglyph->points[comp->argument1].y;

							// - Child point -
							// Ensure point range
							if (comp->argument2 >= num_points) {
								return MUTT_INVALID_RGLYPH_COMPOSITE_COMPONENT_ARGUMENT2;
							}
							// Get child x and y
							float t_child_x = prev_points[comp->argument2].x;
							float t_child_y = prev_points[comp->argument2].y;
							// Calculate it with scales applied so that point is still
							// aligned with scales
							float child_x = (xscale  * t_child_x) + (scale10 * t_child_y);
							float child_y = (scale01 * t_child_x) + (yscale  * t_child_y);

							// Calculate argument1 & argument2 to be offsets that align
							// the child point to the parent point
							argument1 = parent_x - child_x;
							argument2 = parent_y - child_y;
						}

						// Loop through each point
						for (uint16_m p = 0; p < num_points; ++p) {
							// Calculate scaled point
							float point_x = prev_points[p].x;
							float point_y = prev_points[p].y;
							prev_points[p].x = (xscale  * point_x) + (scale10 * point_y);
							prev_points[p].y = (scale01 * point_x) + (yscale  * point_y);
							// Add offsets
							prev_points[p].x += argument1;
							prev_points[p].y += argument2;
						}

						return res;
					}

					// Converts all TrueType-calculated coordinates from a composite-converted rglyph
					// to pixel coordinates
					// OR, if convert_to_punits is false, it just calculates the x/y min/max within
					// prog
					muttResult mutt_composite_rglyph_coords(muttFont* font, muttR_CompProg* prog, muttRGlyph* rglyph, float point_size, float ppi, muBool convert_to_punits) {
						// Calculate x/y min/max values
						prog->x_min = prog->x_max = rglyph->points[0].x;
						prog->y_min = prog->y_max = rglyph->points[0].y;
						for (uint16_m p = 1; p < rglyph->num_points; ++p) {
							// x min/max
							if (rglyph->points[p].x < prog->x_min) {
								prog->x_min = rglyph->points[p].x;
							}
							if (rglyph->points[p].x > prog->x_max) {
								prog->x_max = rglyph->points[p].x;
							}
							// y min/max
							if (rglyph->points[p].y < prog->y_min) {
								prog->y_min = rglyph->points[p].y;
							}
							if (rglyph->points[p].y > prog->y_max) {
								prog->y_max = rglyph->points[p].y;
							}
						}

						// Ensure FUnit range
						if (prog->x_min < -16384.f || prog->x_min > 16383.f ||
							prog->x_max < -16384.f || prog->x_max > 16383.f) {
							return MUTT_INVALID_GLYF_COMPOSITE_X_COORD_FUNITS;
						}
						if (prog->y_min < -16384.f || prog->y_min > 16383.f ||
							prog->y_max < -16384.f || prog->y_max > 16383.f) {
							return MUTT_INVALID_GLYF_COMPOSITE_Y_COORD_FUNITS;
						}

						// Finish here if conversion to pixel-units is not being performed
						if (!convert_to_punits) {
							return MUTT_SUCCESS;
						}

						// Much of this code is considerably similar to mutt_simple_rglyph

						// Calculate point offsets based on glyph's min/max values
						float px = -mutt_funits_to_punits(font, prog->x_min, point_size, ppi) + 1.f;
						float py = -mutt_funits_to_punits(font, prog->y_min, point_size, ppi) + 1.f;

						// Loop through each point
						for (uint16_m p = 0; p < rglyph->num_points; ++p) {
							// X and Y
							rglyph->points[p].x = px + mutt_funits_to_punits(font, rglyph->points[p].x, point_size, ppi);
							rglyph->points[p].y = py + mutt_funits_to_punits(font, rglyph->points[p].y, point_size, ppi);
						}

						// Calculate x/y max
						rglyph->x_max = px + mutt_funits_to_punits(font, prog->x_max, point_size, ppi);
						rglyph->y_max = py + mutt_funits_to_punits(font, prog->y_max, point_size, ppi);

						return MUTT_SUCCESS;
					}

					// Composite glyph -> raster glyph
					// NO mem req abilities unfortunately
					MUDEF muttResult mutt_composite_rglyph(muttFont* font, muttGlyphHeader* header, muttCompositeGlyph* glyph, muttRGlyph* rglyph, float point_size, float ppi, muByte* data) {
						muttResult res = MUTT_SUCCESS;

						// Allocate temp simple max memory
						// This cannot be allocated within font, as it could lead to
						// very unpredictable multi-threaded behavior
						muByte* temp_simple_max = (muByte*)mu_malloc(mutt_simple_glyph_max_size(font));
						if (!temp_simple_max) {
							return MUTT_FAILED_MALLOC;
						}

						// Set rglyph data
						rglyph->contour_ends = (uint16_m*)data;
						rglyph->points = (muttRPoint*)((data + (((uint32_m)font->maxp->max_composite_contours)*2)));

						// Initialize CompProg
						muttR_CompProg prog;
						muttR_CompProg_init(&prog, rglyph, temp_simple_max, font);

						// Loop through each component
						for (uint16_m c = 0; c < glyph->component_count; ++c) {
							// Process component
							res = mutt_component_rglyph(font, &prog, &glyph->components[c], 1);
							if (mutt_result_is_fatal(res)) {
								mu_free(temp_simple_max);
								return res;
							}
						}

						// Convert collected TrueType coordinates to pixel coordinates
						res = mutt_composite_rglyph_coords(font, &prog, rglyph, point_size, ppi, MU_TRUE);
						if (mutt_result_is_fatal(res)) {
							mu_free(temp_simple_max);
							return res;
						}

						// Free temp simple max memory
						mu_free(temp_simple_max);
						return res; if (header) {}
					}

					// X/Y min/max composite calculator
					MUDEF muttResult mutt_composite_glyph_min_max(muttFont* font, muttGlyphHeader* header) {
						muttResult res = MUTT_SUCCESS;

						// Unfortunately, there's no realistic way to do this without
						// doing the usual allocation as far as I'm aware.

						// Much of this code is considerably similar to mutt_composite_rglyph

						// Allocate composite memory
						muByte* composite_mem = (muByte*)mu_malloc(mutt_composite_rglyph_max(font));
						if (!composite_mem) {
							return MUTT_FAILED_MALLOC;
						}
						// Allocate temp simple max memory
						muByte* temp_simple_max = (muByte*)mu_malloc(mutt_simple_glyph_max_size(font));
						if (!temp_simple_max) {
							mu_free(composite_mem);
							return MUTT_FAILED_MALLOC;
						}

						// Set rglyph data
						muttRGlyph rglyph;
						rglyph.contour_ends = (uint16_m*)composite_mem;
						rglyph.points = (muttRPoint*)((composite_mem + (((uint32_m)font->maxp->max_composite_contours)*2)));

						// Initialize CompProg
						muttR_CompProg prog;
						muttR_CompProg_init(&prog, &rglyph, temp_simple_max, font);

						// Loop through each component
						uint32_m component_count = 0;
						muBool no_more = MU_FALSE;
						muByte* bprog = header->data;
						while (!no_more) {
							// Verify incremented component count
							if (++component_count > font->maxp->max_component_elements) {
								return MUTT_INVALID_RGLYPH_COMPOSITE_COMPONENT_COUNT;
							}

							// Get individual component
							muttComponentGlyph this_component;
							res = mutt_composite_component(font, header, &bprog, &this_component, &no_more);
							if (mutt_result_is_fatal(res)) {
								mu_free(temp_simple_max);
								mu_free(composite_mem);
								return res;
							}

							// Process component
							res = mutt_component_rglyph(font, &prog, &this_component, 1);
							if (mutt_result_is_fatal(res)) {
								mu_free(temp_simple_max);
								mu_free(composite_mem);
								return res;
							}
						}

						// Calculate x/y min/max
						res = mutt_composite_rglyph_coords(font, &prog, &rglyph, 0.f, 0.f, MU_FALSE);
						if (mutt_result_is_fatal(res)) {
							mu_free(temp_simple_max);
							mu_free(composite_mem);
							return res;
						}

						// Set x/y min/max values
						header->x_min = mu_ceilf(prog.x_min);
						header->y_min = mu_ceilf(prog.y_min);
						header->x_max = mu_ceilf(prog.x_max);
						header->y_max = mu_ceilf(prog.y_max);

						// Free and return
						mu_free(temp_simple_max);
						mu_free(composite_mem);
						return res;
					}

					// Memory maximum
					MUDEF uint32_m mutt_composite_rglyph_max(muttFont* font) {
						return
							// contour_ends
							(sizeof(uint16_m) * font->maxp->max_composite_contours)
							// points
							+ (sizeof(muttRPoint) * font->maxp->max_composite_points)
						;
					}

				/* Header */

					// Glyph header -> raster glyph
					MUDEF muttResult mutt_header_rglyph(muttFont* font, muttGlyphHeader* header, muttRGlyph* rglyph, float point_size, float ppi, muByte* data, uint32_m* written) {
						muttResult res = MUTT_SUCCESS;
						uint32_m write0 = 0, write1 = 0;

						// Just memory calculations:
						if (!data) {
							// Simple:
							if (header->number_of_contours >= 0) {
								// Glyph data:
								res = mutt_simple_glyph(font, header, 0, 0, &write0);
								if (mutt_result_is_fatal(res)) {
									return res;
								}
								// Rglyph data:
								res = mutt_simple_rglyph(font, header, 0, 0, point_size, ppi, 0, &write1);
								if (mutt_result_is_fatal(res)) {
									return res;
								}
							}
							// Composite:
							else {
								// Glyph data:
								res = mutt_composite_glyph(font, header, 0, 0, &write0);
								if (mutt_result_is_fatal(res)) {
									return res;
								}
								// Rglyph data:
								write1 = mutt_composite_rglyph_max(font);
							}

							// Write sum of memory needed
							*written = write0 + write1;
							return res;
						}

						// Simple:
						if (header->number_of_contours >= 0) {
							// Load simple glyph
							muttSimpleGlyph glyph;
							res = mutt_simple_glyph(font, header, &glyph, data, &write0);
							if (mutt_result_is_fatal(res)) {
								return res;
							}
							data += write0;

							// Convert to rglyph
							res = mutt_simple_rglyph(font, header, &glyph, rglyph, point_size, ppi, data, &write1);
							if (mutt_result_is_fatal(res)) {
								return res;
							}

							//data += write1;
						}
						// Composite:
						else {
							// Load composite glyph
							muttCompositeGlyph glyph;
							res = mutt_composite_glyph(font, header, &glyph, data, &write0);
							if (mutt_result_is_fatal(res)) {
								return res;
							}
							data += write0;

							// Convert to rglyph
							// MUDEF muttResult mutt_composite_rglyph(muttFont* font, muttGlyphHeader* header, muttCompositeGlyph* glyph, muttRGlyph* rglyph, float point_size, float ppi, muByte* data)
							res = mutt_composite_rglyph(font, header, &glyph, rglyph, point_size, ppi, data);
							if (mutt_result_is_fatal(res)) {
								return res;
							}
							write1 = mutt_composite_rglyph_max(font);
							//data += write1;
						}

						// Write written
						if (written) {
							*written = write0 + write1;
						}
						return res;
					}

					// Glyph header x/y min/max -> raster x/y max
					MUDEF void mutt_funits_punits_min_max(muttFont* font, muttGlyphHeader* header, muttRGlyph* rglyph, float point_size, float ppi) {
						rglyph->x_max = (-mutt_funits_to_punits(font, header->x_min, point_size, ppi) + 1.f)
							+ mutt_funits_to_punits(font, header->x_max, point_size, ppi);
						rglyph->y_max = (-mutt_funits_to_punits(font, header->y_min, point_size, ppi) + 1.f)
							+ mutt_funits_to_punits(font, header->y_max, point_size, ppi);
					}

					MUDEF uint32_m mutt_header_rglyph_max(muttFont* font) {
						// Simple:
						uint32_m sim = mutt_simple_glyph_max_size(font) + mutt_simple_rglyph_max(font);
						// Composite:
						uint32_m com = mutt_composite_glyph_max_size(font) + mutt_composite_rglyph_max(font);
						// Return greater
						return (sim > com) ?(sim) :(com);
					}

		/* Result */

			MUDEF muBool mutt_result_is_fatal(muttResult result) {
				switch (result) {
					default: return MU_TRUE; break;
					case MUTT_SUCCESS:
					case MUTT_INVALID_GLYF_SIMPLE_X_COORD: case MUTT_INVALID_GLYF_SIMPLE_Y_COORD:
						return MU_FALSE; break;
				}
			}

		/* Names */

			#ifdef MUTT_NAMES

			MUDEF const char* mutt_result_get_name(muttResult result) {
				switch (result) {
					default: return "MU_UNKNOWN"; break;
					case MUTT_SUCCESS: return "MUTT_SUCCESS"; break;
					case MUTT_FAILED_MALLOC: return "MUTT_FAILED_MALLOC"; break;
					case MUTT_FAILED_REALLOC: return "MUTT_FAILED_REALLOC"; break;
					case MUTT_FAILED_FIND_TABLE: return "MUTT_FAILED_FIND_TABLE"; break;
					case MUTT_INVALID_DIRECTORY_LENGTH: return "MUTT_INVALID_DIRECTORY_LENGTH"; break;
					case MUTT_INVALID_DIRECTORY_SFNT_VERSION: return "MUTT_INVALID_DIRECTORY_SFNT_VERSION"; break;
					case MUTT_INVALID_DIRECTORY_NUM_TABLES: return "MUTT_INVALID_DIRECTORY_NUM_TABLES"; break;
					case MUTT_INVALID_DIRECTORY_RECORD_OFFSET: return "MUTT_INVALID_DIRECTORY_RECORD_OFFSET"; break;
					case MUTT_INVALID_DIRECTORY_RECORD_LENGTH: return "MUTT_INVALID_DIRECTORY_RECORD_LENGTH"; break;
					case MUTT_INVALID_DIRECTORY_RECORD_CHECKSUM: return "MUTT_INVALID_DIRECTORY_RECORD_CHECKSUM"; break;
					case MUTT_INVALID_DIRECTORY_RECORD_TABLE_TAG: return "MUTT_INVALID_DIRECTORY_RECORD_TABLE_TAG"; break;
					case MUTT_MISSING_DIRECTORY_RECORD_TABLE_TAGS: return "MUTT_MISSING_DIRECTORY_RECORD_TABLE_TAGS"; break;
					case MUTT_INVALID_MAXP_LENGTH: return "MUTT_INVALID_MAXP_LENGTH"; break;
					case MUTT_INVALID_MAXP_VERSION: return "MUTT_INVALID_MAXP_VERSION"; break;
					case MUTT_INVALID_MAXP_NUM_GLYPHS: return "MUTT_INVALID_MAXP_NUM_GLYPHS"; break;
					case MUTT_INVALID_MAXP_MAX_ZONES: return "MUTT_INVALID_MAXP_MAX_ZONES"; break;
					case MUTT_INVALID_HEAD_LENGTH: return "MUTT_INVALID_HEAD_LENGTH"; break;
					case MUTT_INVALID_HEAD_VERSION: return "MUTT_INVALID_HEAD_VERSION"; break;
					case MUTT_INVALID_HEAD_MAGIC_NUMBER: return "MUTT_INVALID_HEAD_MAGIC_NUMBER"; break;
					case MUTT_INVALID_HEAD_UNITS_PER_EM: return "MUTT_INVALID_HEAD_UNITS_PER_EM"; break;
					case MUTT_INVALID_HEAD_X_MIN_COORDINATES: return "MUTT_INVALID_HEAD_X_MIN_COORDINATES"; break;
					case MUTT_INVALID_HEAD_Y_MIN_COORDINATES: return "MUTT_INVALID_HEAD_Y_MIN_COORDINATES"; break;
					case MUTT_INVALID_HEAD_X_MAX_COORDINATES: return "MUTT_INVALID_HEAD_X_MAX_COORDINATES"; break;
					case MUTT_INVALID_HEAD_Y_MAX_COORDINATES: return "MUTT_INVALID_HEAD_Y_MAX_COORDINATES"; break;
					case MUTT_INVALID_HEAD_X_MIN_MAX: return "MUTT_INVALID_HEAD_X_MIN_MAX"; break;
					case MUTT_INVALID_HEAD_Y_MIN_MAX: return "MUTT_INVALID_HEAD_Y_MIN_MAX"; break;
					case MUTT_INVALID_HEAD_INDEX_TO_LOC_FORMAT: return "MUTT_INVALID_HEAD_INDEX_TO_LOC_FORMAT"; break;
					case MUTT_INVALID_HEAD_GLYPH_DATA_FORMAT: return "MUTT_INVALID_HEAD_GLYPH_DATA_FORMAT"; break;
					case MUTT_INVALID_HHEA_LENGTH: return "MUTT_INVALID_HHEA_LENGTH"; break;
					case MUTT_INVALID_HHEA_VERSION: return "MUTT_INVALID_HHEA_VERSION"; break;
					case MUTT_INVALID_HHEA_METRIC_DATA_FORMAT: return "MUTT_INVALID_HHEA_METRIC_DATA_FORMAT"; break;
					case MUTT_INVALID_HHEA_NUMBER_OF_HMETRICS: return "MUTT_INVALID_HHEA_NUMBER_OF_HMETRICS"; break;
					case MUTT_HHEA_REQUIRES_MAXP: return "MUTT_HHEA_REQUIRES_MAXP"; break;
					case MUTT_INVALID_HMTX_LENGTH: return "MUTT_INVALID_HMTX_LENGTH"; break;
					case MUTT_HMTX_REQUIRES_MAXP: return "MUTT_HMTX_REQUIRES_MAXP";
					case MUTT_HMTX_REQUIRES_HHEA: return "MUTT_HMTX_REQUIRES_HHEA";
					case MUTT_INVALID_LOCA_LENGTH: return "MUTT_INVALID_LOCA_LENGTH"; break;
					case MUTT_INVALID_LOCA_OFFSET: return "MUTT_INVALID_LOCA_OFFSET"; break;
					case MUTT_LOCA_REQUIRES_MAXP: return "MUTT_LOCA_REQUIRES_MAXP"; break;
					case MUTT_LOCA_REQUIRES_HEAD: return "MUTT_LOCA_REQUIRES_HEAD"; break;
					case MUTT_LOCA_REQUIRES_GLYF: return "MUTT_LOCA_REQUIRES_GLYF"; break;
					case MUTT_INVALID_NAME_LENGTH: return "MUTT_INVALID_NAME_LENGTH"; break;
					case MUTT_INVALID_NAME_VERSION: return "MUTT_INVALID_NAME_VERSION"; break;
					case MUTT_INVALID_NAME_STORAGE_OFFSET: return "MUTT_INVALID_NAME_STORAGE_OFFSET"; break;
					case MUTT_INVALID_NAME_LENGTH_OFFSET: return "MUTT_INVALID_NAME_LENGTH_OFFSET"; break;
					case MUTT_INVALID_GLYF_HEADER_LENGTH: return "MUTT_INVALID_GLYF_HEADER_LENGTH"; break;
					case MUTT_INVALID_GLYF_HEADER_NUMBER_OF_CONTOURS: return "MUTT_INVALID_GLYF_HEADER_NUMBER_OF_CONTOURS"; break;
					case MUTT_INVALID_GLYF_HEADER_X_MIN: return "MUTT_INVALID_GLYF_HEADER_X_MIN"; break;
					case MUTT_INVALID_GLYF_HEADER_Y_MIN: return "MUTT_INVALID_GLYF_HEADER_Y_MIN"; break;
					case MUTT_INVALID_GLYF_HEADER_X_MAX: return "MUTT_INVALID_GLYF_HEADER_X_MAX"; break;
					case MUTT_INVALID_GLYF_HEADER_Y_MAX: return "MUTT_INVALID_GLYF_HEADER_Y_MAX"; break;
					case MUTT_INVALID_GLYF_HEADER_X_MIN_MAX: return "MUTT_INVALID_GLYF_HEADER_X_MIN_MAX"; break;
					case MUTT_INVALID_GLYF_HEADER_Y_MIN_MAX: return "MUTT_INVALID_GLYF_HEADER_Y_MIN_MAX"; break;
					case MUTT_INVALID_GLYF_SIMPLE_LENGTH: return "MUTT_INVALID_GLYF_SIMPLE_LENGTH"; break;
					case MUTT_INVALID_GLYF_SIMPLE_END_PTS_OF_CONTOURS: return "MUTT_INVALID_GLYF_SIMPLE_END_PTS_OF_CONTOURS"; break;
					case MUTT_INVALID_GLYF_SIMPLE_POINT_COUNT: return "MUTT_INVALID_GLYF_SIMPLE_POINT_COUNT"; break;
					case MUTT_INVALID_GLYF_SIMPLE_INSTRUCTION_LENGTH: return "MUTT_INVALID_GLYF_SIMPLE_INSTRUCTION_LENGTH"; break;
					case MUTT_INVALID_GLYF_SIMPLE_X_COORD: return "MUTT_INVALID_GLYF_SIMPLE_X_COORD"; break;
					case MUTT_INVALID_GLYF_SIMPLE_Y_COORD: return "MUTT_INVALID_GLYF_SIMPLE_Y_COORD"; break;
					case MUTT_INVALID_GLYF_COMPOSITE_LENGTH: return "MUTT_INVALID_GLYF_COMPOSITE_LENGTH"; break;
					case MUTT_INVALID_GLYF_COMPOSITE_INSTRUCTION_LENGTH: return "MUTT_INVALID_GLYF_COMPOSITE_INSTRUCTION_LENGTH"; break;
					case MUTT_INVALID_GLYF_COMPOSITE_COMPONENT_COUNT: return "MUTT_INVALID_GLYF_COMPOSITE_COMPONENT_COUNT"; break;
					case MUTT_INVALID_GLYF_COMPOSITE_GLYPH_INDEX: return "MUTT_INVALID_GLYF_COMPOSITE_GLYPH_INDEX"; break;
					case MUTT_INVALID_GLYF_COMPOSITE_FLAGS: return "MUTT_INVALID_GLYF_COMPOSITE_FLAGS"; break;
					case MUTT_INVALID_GLYF_SIMPLE_X_COORD_FUNITS: return "MUTT_INVALID_GLYF_SIMPLE_X_COORD_FUNITS"; break;
					case MUTT_INVALID_GLYF_SIMPLE_Y_COORD_FUNITS: return "MUTT_INVALID_GLYF_SIMPLE_Y_COORD_FUNITS"; break;
					case MUTT_INVALID_GLYF_COMPOSITE_X_COORD_FUNITS: return "MUTT_INVALID_GLYF_COMPOSITE_X_COORD_FUNITS"; break;
					case MUTT_INVALID_GLYF_COMPOSITE_Y_COORD_FUNITS: return "MUTT_INVALID_GLYF_COMPOSITE_Y_COORD_FUNITS"; break;
					case MUTT_INVALID_CMAP_LENGTH: return "MUTT_INVALID_CMAP_LENGTH"; break;
					case MUTT_INVALID_CMAP_VERSION: return "MUTT_INVALID_CMAP_VERSION"; break;
					case MUTT_INVALID_CMAP_ENCODING_RECORD_OFFSET: return "MUTT_INVALID_CMAP_ENCODING_RECORD_OFFSET"; break;
					case MUTT_INVALID_CMAP_ENCODING_RECORD_LENGTH: return "MUTT_INVALID_CMAP_ENCODING_RECORD_LENGTH"; break;
					case MUTT_INVALID_CMAP_ENCODING_RECORD_FORMAT: return "MUTT_INVALID_CMAP_ENCODING_RECORD_FORMAT"; break;
					case MUTT_INVALID_CMAP0_LENGTH: return "MUTT_INVALID_CMAP0_LENGTH"; break;
					case MUTT_INVALID_CMAP4_LENGTH: return "MUTT_INVALID_CMAP4_LENGTH"; break;
					case MUTT_INVALID_CMAP4_SEG_COUNT_X2: return "MUTT_INVALID_CMAP4_SEG_COUNT_X2"; break;
					case MUTT_INVALID_CMAP4_END_CODE: return "MUTT_INVALID_CMAP4_END_CODE"; break;
					case MUTT_INVALID_CMAP4_LAST_END_CODE: return "MUTT_INVALID_CMAP4_LAST_END_CODE"; break;
					case MUTT_INVALID_CMAP4_START_CODE: return "MUTT_INVALID_CMAP4_START_CODE"; break;
					case MUTT_INVALID_CMAP4_ID_RANGE_OFFSET: return "MUTT_INVALID_CMAP4_ID_RANGE_OFFSET"; break;
					case MUTT_INVALID_CMAP12_LENGTH: return "MUTT_INVALID_CMAP12_LENGTH"; break;
					case MUTT_INVALID_CMAP12_START_CHAR_CODE: return "MUTT_INVALID_CMAP12_START_CHAR_CODE"; break;
					case MUTT_INVALID_CMAP12_END_CHAR_CODE: return "MUTT_INVALID_CMAP12_END_CHAR_CODE"; break;
					case MUTT_CMAP_REQUIRES_MAXP: return "MUTT_CMAP_REQUIRES_MAXP"; break;
					case MUTT_UNKNOWN_RASTER_METHOD: return "MUTT_UNKNOWN_RASTER_METHOD"; break;
					case MUTT_INVALID_RGLYPH_COMPOSITE_CONTOUR_COUNT: return "MUTT_INVALID_RGLYPH_COMPOSITE_CONTOUR_COUNT"; break;
					case MUTT_INVALID_RGLYPH_COMPOSITE_POINT_COUNT: return "MUTT_INVALID_RGLYPH_COMPOSITE_POINT_COUNT"; break;
					case MUTT_INVALID_RGLYPH_COMPOSITE_DEPTH: return "MUTT_INVALID_RGLYPH_COMPOSITE_DEPTH"; break;
					case MUTT_INVALID_RGLYPH_COMPOSITE_COMPONENT_COUNT: return "MUTT_INVALID_RGLYPH_COMPOSITE_COMPONENT_COUNT"; break;
					case MUTT_INVALID_RGLYPH_COMPOSITE_COMPONENT_ARGUMENT1: return "MUTT_INVALID_RGLYPH_COMPOSITE_COMPONENT_ARGUMENT1"; break;
					case MUTT_INVALID_RGLYPH_COMPOSITE_COMPONENT_ARGUMENT2: return "MUTT_INVALID_RGLYPH_COMPOSITE_COMPONENT_ARGUMENT2"; break;
				}
			}

			MUDEF const char* mutt_platform_get_name(uint16_m platform_id) {
				switch (platform_id) {
					default: return "MU_UNKNOWN"; break;
					case MUTT_PLATFORM_UNICODE: return "MUTT_PLATFORM_UNICODE"; break;
					case MUTT_PLATFORM_MACINTOSH: return "MUTT_PLATFORM_MACINTOSH"; break;
					case MUTT_PLATFORM_ISO: return "MUTT_PLATFORM_ISO"; break;
					case MUTT_PLATFORM_WINDOWS: return "MUTT_PLATFORM_WINDOWS"; break;
					case MUTT_PLATFORM_CUSTOM: return "MUTT_PLATFORM_CUSTOM"; break;
				}
			}
			MUDEF const char* mutt_platform_get_nice_name(uint16_m platform_id) {
				switch (platform_id) {
					default: return "Unknown"; break;
					case MUTT_PLATFORM_UNICODE: return "Unicode"; break;
					case MUTT_PLATFORM_MACINTOSH: return "Macintosh"; break;
					case MUTT_PLATFORM_ISO: return "ISO"; break;
					case MUTT_PLATFORM_WINDOWS: return "Windows"; break;
					case MUTT_PLATFORM_CUSTOM: return "Custom"; break;
				}
			}

			MUDEF const char* mutt_unicode_encoding_get_name(uint16_m encoding_id) {
				switch (encoding_id) {
					default: return "MU_UNKNOWN"; break;
					case MUTT_UNICODE_1_0: return "MUTT_UNICODE_1_0"; break;
					case MUTT_UNICODE_1_1: return "MUTT_UNICODE_1_1"; break;
					case MUTT_UNICODE_ISO_IEC_10646: return "MUTT_UNICODE_ISO_IEC_10646"; break;
					case MUTT_UNICODE_2_0_BMP: return "MUTT_UNICODE_2_0_BMP"; break;
					case MUTT_UNICODE_2_0: return "MUTT_UNICODE_2_0"; break;
					case MUTT_UNICODE_VARIATION: return "MUTT_UNICODE_VARIATION"; break;
					case MUTT_UNICODE_FULL: return "MUTT_UNICODE_FULL"; break;
				}
			}
			MUDEF const char* mutt_unicode_encoding_get_nice_name(uint16_m encoding_id) {
				switch (encoding_id) {
					default: return "Unknown"; break;
					case MUTT_UNICODE_1_0: return "Unicode 1.0"; break;
					case MUTT_UNICODE_1_1: return "Unicode 1.1"; break;
					case MUTT_UNICODE_ISO_IEC_10646: return "ISO/IEC 10646"; break;
					case MUTT_UNICODE_2_0_BMP: return "Unicode 2.0 (BMP only)"; break;
					case MUTT_UNICODE_2_0: return "Unicode 2.0 (full repertoire)"; break;
					case MUTT_UNICODE_VARIATION: return "Unicode variation sequences"; break;
					case MUTT_UNICODE_FULL: return "Unicode full repertoire"; break;
				}
			}

			MUDEF const char* mutt_macintosh_encoding_get_name(uint16_m encoding_id) {
				switch (encoding_id) {
					default: return "MU_UNKNOWN"; break;
					case MUTT_MACINTOSH_ROMAN: return "MUTT_MACINTOSH_ROMAN"; break;
					case MUTT_MACINTOSH_JAPANESE: return "MUTT_MACINTOSH_JAPANESE"; break;
					case MUTT_MACINTOSH_CHINESE_TRADITIONAL: return "MUTT_MACINTOSH_CHINESE_TRADITIONAL"; break;
					case MUTT_MACINTOSH_KOREAN: return "MUTT_MACINTOSH_KOREAN"; break;
					case MUTT_MACINTOSH_ARABIC: return "MUTT_MACINTOSH_ARABIC"; break;
					case MUTT_MACINTOSH_HEBREW: return "MUTT_MACINTOSH_HEBREW"; break;
					case MUTT_MACINTOSH_GREEK: return "MUTT_MACINTOSH_GREEK"; break;
					case MUTT_MACINTOSH_RUSSIAN: return "MUTT_MACINTOSH_RUSSIAN"; break;
					case MUTT_MACINTOSH_RSYMBOL: return "MUTT_MACINTOSH_RSYMBOL"; break;
					case MUTT_MACINTOSH_DEVANAGARI: return "MUTT_MACINTOSH_DEVANAGARI"; break;
					case MUTT_MACINTOSH_GURMUKHI: return "MUTT_MACINTOSH_GURMUKHI"; break;
					case MUTT_MACINTOSH_GUJARATI: return "MUTT_MACINTOSH_GUJARATI"; break;
					case MUTT_MACINTOSH_ODIA: return "MUTT_MACINTOSH_ODIA"; break;
					case MUTT_MACINTOSH_BANGLA: return "MUTT_MACINTOSH_BANGLA"; break;
					case MUTT_MACINTOSH_TAMIL: return "MUTT_MACINTOSH_TAMIL"; break;
					case MUTT_MACINTOSH_TELUGU: return "MUTT_MACINTOSH_TELUGU"; break;
					case MUTT_MACINTOSH_KANNADA: return "MUTT_MACINTOSH_KANNADA"; break;
					case MUTT_MACINTOSH_MALAYALAM: return "MUTT_MACINTOSH_MALAYALAM"; break;
					case MUTT_MACINTOSH_SINHALESE: return "MUTT_MACINTOSH_SINHALESE"; break;
					case MUTT_MACINTOSH_BURMESE: return "MUTT_MACINTOSH_BURMESE"; break;
					case MUTT_MACINTOSH_KHMER: return "MUTT_MACINTOSH_KHMER"; break;
					case MUTT_MACINTOSH_THAI: return "MUTT_MACINTOSH_THAI"; break;
					case MUTT_MACINTOSH_LAOTIAN: return "MUTT_MACINTOSH_LAOTIAN"; break;
					case MUTT_MACINTOSH_GEORGIAN: return "MUTT_MACINTOSH_GEORGIAN"; break;
					case MUTT_MACINTOSH_ARMENIAN: return "MUTT_MACINTOSH_ARMENIAN"; break;
					case MUTT_MACINTOSH_CHINESE_SIMPLIFIED: return "MUTT_MACINTOSH_CHINESE_SIMPLIFIED"; break;
					case MUTT_MACINTOSH_TIBETAN: return "MUTT_MACINTOSH_TIBETAN"; break;
					case MUTT_MACINTOSH_MONGOLIAN: return "MUTT_MACINTOSH_MONGOLIAN"; break;
					case MUTT_MACINTOSH_GEEZ: return "MUTT_MACINTOSH_GEEZ"; break;
					case MUTT_MACINTOSH_SLAVIC: return "MUTT_MACINTOSH_SLAVIC"; break;
					case MUTT_MACINTOSH_VIETNAMESE: return "MUTT_MACINTOSH_VIETNAMESE"; break;
					case MUTT_MACINTOSH_SINDHI: return "MUTT_MACINTOSH_SINDHI"; break;
					case MUTT_MACINTOSH_UNINTERPRETED: return "MUTT_MACINTOSH_UNINTERPRETED"; break;
				}
			}
			MUDEF const char* mutt_macintosh_encoding_get_nice_name(uint16_m encoding_id) {
				switch (encoding_id) {
					default: return "Unknown"; break;
					case MUTT_MACINTOSH_ROMAN: return "Roman"; break;
					case MUTT_MACINTOSH_JAPANESE: return "Japanese"; break;
					case MUTT_MACINTOSH_CHINESE_TRADITIONAL: return "Chinese (Traditional)"; break;
					case MUTT_MACINTOSH_KOREAN: return "Korean"; break;
					case MUTT_MACINTOSH_ARABIC: return "Arabic"; break;
					case MUTT_MACINTOSH_HEBREW: return "Hebrew"; break;
					case MUTT_MACINTOSH_GREEK: return "Greek"; break;
					case MUTT_MACINTOSH_RUSSIAN: return "Russian"; break;
					case MUTT_MACINTOSH_RSYMBOL: return "RSymbol"; break;
					case MUTT_MACINTOSH_DEVANAGARI: return "Devanagari"; break;
					case MUTT_MACINTOSH_GURMUKHI: return "Gurmukhi"; break;
					case MUTT_MACINTOSH_GUJARATI: return "Gujarati"; break;
					case MUTT_MACINTOSH_ODIA: return "Odia"; break;
					case MUTT_MACINTOSH_BANGLA: return "Bangla"; break;
					case MUTT_MACINTOSH_TAMIL: return "Tamil"; break;
					case MUTT_MACINTOSH_TELUGU: return "Telugu"; break;
					case MUTT_MACINTOSH_KANNADA: return "Kannada"; break;
					case MUTT_MACINTOSH_MALAYALAM: return "Malayalam"; break;
					case MUTT_MACINTOSH_SINHALESE: return "Sinhalese"; break;
					case MUTT_MACINTOSH_BURMESE: return "Burmese"; break;
					case MUTT_MACINTOSH_KHMER: return "Khmer"; break;
					case MUTT_MACINTOSH_THAI: return "Thai"; break;
					case MUTT_MACINTOSH_LAOTIAN: return "Laotian"; break;
					case MUTT_MACINTOSH_GEORGIAN: return "Georgian"; break;
					case MUTT_MACINTOSH_ARMENIAN: return "Armenian"; break;
					case MUTT_MACINTOSH_CHINESE_SIMPLIFIED: return "Chinese (Simplified)"; break;
					case MUTT_MACINTOSH_TIBETAN: return "Tibetan"; break;
					case MUTT_MACINTOSH_MONGOLIAN: return "Mongolian"; break;
					case MUTT_MACINTOSH_GEEZ: return "Geez"; break;
					case MUTT_MACINTOSH_SLAVIC: return "Slavic"; break;
					case MUTT_MACINTOSH_VIETNAMESE: return "Vietnamese"; break;
					case MUTT_MACINTOSH_SINDHI: return "Sindhi"; break;
					case MUTT_MACINTOSH_UNINTERPRETED: return "Uninterpreted"; break;
				}
			}

			MUDEF const char* mutt_windows_encoding_get_name(uint16_m encoding_id) {
				switch (encoding_id) {
					default: return "MU_UNKNOWN"; break;
					case MUTT_WINDOWS_SYMBOL: return "MUTT_WINDOWS_SYMBOL"; break;
					case MUTT_WINDOWS_UNICODE_BMP: return "MUTT_WINDOWS_UNICODE_BMP"; break;
					case MUTT_WINDOWS_SHIFTJIS: return "MUTT_WINDOWS_SHIFTJIS"; break;
					case MUTT_WINDOWS_PRC: return "MUTT_WINDOWS_PRC"; break;
					case MUTT_WINDOWS_BIG5: return "MUTT_WINDOWS_BIG5"; break;
					case MUTT_WINDOWS_WANSUNG: return "MUTT_WINDOWS_WANSUNG"; break;
					case MUTT_WINDOWS_JOHAB: return "MUTT_WINDOWS_JOHAB"; break;
					case MUTT_WINDOWS_UNICODE: return "MUTT_WINDOWS_UNICODE"; break;
				}
			}
			MUDEF const char* mutt_windows_encoding_get_nice_name(uint16_m encoding_id) {
				switch (encoding_id) {
					default: return "Unknown"; break;
					case MUTT_WINDOWS_SYMBOL: return "Symbol"; break;
					case MUTT_WINDOWS_UNICODE_BMP: return "Unicode (BMP only)"; break;
					case MUTT_WINDOWS_SHIFTJIS: return "ShiftJIS"; break;
					case MUTT_WINDOWS_PRC: return "PRC"; break;
					case MUTT_WINDOWS_BIG5: return "Big5"; break;
					case MUTT_WINDOWS_WANSUNG: return "Wansung"; break;
					case MUTT_WINDOWS_JOHAB: return "Johab"; break;
					case MUTT_WINDOWS_UNICODE: return "Unicode (full repertoire)"; break;
				}
			}

			MUDEF const char* mutt_name_id_get_name(uint16_m name_id) {
				switch (name_id) {
					default: return "MU_UNKNOWN"; break;
					case MUTT_NAME_COPYRIGHT_NOTICE: return "MUTT_NAME_COPYRIGHT_NOTICE"; break;
					case MUTT_NAME_FONT_FAMILY: return "MUTT_NAME_FONT_FAMILY"; break;
					case MUTT_NAME_FONT_SUBFAMILY: return "MUTT_NAME_FONT_SUBFAMILY"; break;
					case MUTT_NAME_FONT_IDENTIFIER: return "MUTT_NAME_FONT_IDENTIFIER"; break;
					case MUTT_NAME_FONT_FULL: return "MUTT_NAME_FONT_FULL"; break;
					case MUTT_NAME_VERSION: return "MUTT_NAME_VERSION"; break;
					case MUTT_NAME_POSTSCRIPT: return "MUTT_NAME_POSTSCRIPT"; break;
					case MUTT_NAME_TRADEMARK: return "MUTT_NAME_TRADEMARK"; break;
					case MUTT_NAME_MANUFACTURER: return "MUTT_NAME_MANUFACTURER"; break;
					case MUTT_NAME_DESIGNER: return "MUTT_NAME_DESIGNER"; break;
					case MUTT_NAME_DESCRIPTION: return "MUTT_NAME_DESCRIPTION"; break;
					case MUTT_NAME_VENDOR_URL: return "MUTT_NAME_VENDOR_URL"; break;
					case MUTT_NAME_DESIGNER_URL: return "MUTT_NAME_DESIGNER_URL"; break;
					case MUTT_NAME_LICENSE: return "MUTT_NAME_LICENSE"; break;
					case MUTT_NAME_LICENSE_URL: return "MUTT_NAME_LICENSE_URL"; break;
					case MUTT_NAME_TYPOGRAPHIC_FAMILY: return "MUTT_NAME_TYPOGRAPHIC_FAMILY"; break;
					case MUTT_NAME_TYPOGRAPHIC_SUBFAMILY: return "MUTT_NAME_TYPOGRAPHIC_SUBFAMILY"; break;
					case MUTT_NAME_COMPATIBLE_FULL: return "MUTT_NAME_COMPATIBLE_FULL"; break;
					case MUTT_NAME_SAMPLE_TEXT: return "MUTT_NAME_SAMPLE_TEXT"; break;
					case MUTT_NAME_POSTSCRIPT_CID_FINDFONT: return "MUTT_NAME_POSTSCRIPT_CID_FINDFONT"; break;
					case MUTT_NAME_WWS_FAMILY: return "MUTT_NAME_WWS_FAMILY"; break;
					case MUTT_NAME_WWS_SUBFAMILY: return "MUTT_NAME_WWS_SUBFAMILY"; break;
					case MUTT_NAME_LIGHT_BACKGROUND_PALETTE: return "MUTT_NAME_LIGHT_BACKGROUND_PALETTE"; break;
					case MUTT_NAME_DARK_BACKGROUND_PALETTE: return "MUTT_NAME_DARK_BACKGROUND_PALETTE"; break;
				}
			}
			MUDEF const char* mutt_name_id_get_nice_name(uint16_m name_id) {
				switch (name_id) {
					default: return "Unknown"; break;
					case MUTT_NAME_COPYRIGHT_NOTICE: return "Copyright notice"; break;
					case MUTT_NAME_FONT_FAMILY: return "Font family"; break;
					case MUTT_NAME_FONT_SUBFAMILY: return "Font subfamily"; break;
					case MUTT_NAME_FONT_IDENTIFIER: return "Unique font identifier"; break;
					case MUTT_NAME_FONT_FULL: return "Full font name"; break;
					case MUTT_NAME_VERSION: return "Version"; break;
					case MUTT_NAME_POSTSCRIPT: return "Postscript name"; break;
					case MUTT_NAME_TRADEMARK: return "Trademark"; break;
					case MUTT_NAME_MANUFACTURER: return "Manufacturer"; break;
					case MUTT_NAME_DESIGNER: return "Designer"; break;
					case MUTT_NAME_DESCRIPTION: return "Description"; break;
					case MUTT_NAME_VENDOR_URL: return "Vendor URL"; break;
					case MUTT_NAME_DESIGNER_URL: return "Designer URL"; break;
					case MUTT_NAME_LICENSE: return "License"; break;
					case MUTT_NAME_LICENSE_URL: return "License URL"; break;
					case MUTT_NAME_TYPOGRAPHIC_FAMILY: return "Typographic family"; break;
					case MUTT_NAME_TYPOGRAPHIC_SUBFAMILY: return "Typographic subfamily"; break;
					case MUTT_NAME_COMPATIBLE_FULL: return "Compatible full"; break;
					case MUTT_NAME_SAMPLE_TEXT: return "Sample text"; break;
					case MUTT_NAME_POSTSCRIPT_CID_FINDFONT: return "PostScript CID findfont"; break;
					case MUTT_NAME_WWS_FAMILY: return "WWS family"; break;
					case MUTT_NAME_WWS_SUBFAMILY: return "WWS subfamily"; break;
					case MUTT_NAME_LIGHT_BACKGROUND_PALETTE: return "Light background palette"; break;
					case MUTT_NAME_DARK_BACKGROUND_PALETTE: return "Dark background palette"; break;
				}
			}

			#endif /* MUTT_NAMES */

		MU_CPP_EXTERN_END
	#endif /* MUTT_IMPLEMENTATION */
	#endif

	#endif /* MUSDK_MUTT */

#endif /* MUSDK_IMPLEMENTATION */
// @ATTENTION

/*
For provided source code:
	------------------------------------------------------------------------------
	This software is available under 2 licenses -- choose whichever you prefer.
	------------------------------------------------------------------------------
	ALTERNATIVE A - MIT License
	Copyright (c) 2024 Hum
	Permission is hereby granted, free of charge, to any person obtaining a copy of
	this software and associated documentation files (the "Software"), to deal in
	the Software without restriction, including without limitation the rights to
	use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
	of the Software, and to permit persons to whom the Software is furnished to do
	so, subject to the following conditions:
	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
	------------------------------------------------------------------------------
	ALTERNATIVE B - Public Domain (www.unlicense.org)
	This is free and unencumbered software released into the public domain.
	Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
	software, either in source code form or as a compiled binary, for any purpose,
	commercial or non-commercial, and by any means.
	In jurisdictions that recognize copyright laws, the author or authors of this
	software dedicate any and all copyright interest in the software to the public
	domain. We make this dedication for the benefit of the public at large and to
	the detriment of our heirs and successors. We intend this dedication to be an
	overt act of relinquishment in perpetuity of all present and future rights to
	this software under copyright law.
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
	ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
	WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
	------------------------------------------------------------------------------

For Khronos specifications:
	Copyright (c) 2013-2020 The Khronos Group Inc.

	Licensed under the Apache License, Version 2.0 (the "License"); you may not
	use this file except in compliance with the License. You may obtain a copy of
	the License at

	http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software distributed
	under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
	CONDITIONS OF ANY KIND, either express or implied. See the License for the
	specific language governing permissions and limitations under the License.
*/

