/* Copyright (c) 2020-2022, ellie/@ell1e & Spew3D Team (see AUTHORS.md).

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

Alternatively, at your option, this file is offered under the Apache 2
license, see accompanied LICENSE.md.
*/

#ifndef SPEW3D_GLOB_H_
#define SPEW3D_GLOB_H_

#include <stdint.h>


/// The following setting controls the maximum amount of '*' or "**"
/// allowed per pattern for spew3d_globmatch(). This prevents recursion
/// crashes or excessive hangs when facing too complex, bad patterns.
#ifndef SPEW3D_MAX_GLOBS
#define SPEW3D_MAX_GLOBS 5
#endif

/// spew3d_globmatch() matches a glob pattern, like e.g. "abc*" against
/// "abctest".
///
/// Special characters:
/// In the pattern, '*', "**", '^', and '?' have special meanings:
/// '*' and "**" match longer substrings while '*' may not match
/// path separators (see parameters below), '?' matches an arbitrary
/// single character. If you prefix a special character with a '^'
/// including '^' itself, it will be treated as normal character.
///
/// @returns 1 if the value matches the pattern, otherwise 0.
///
/// @param double_glob_for_paths
/// If parameter doubleglob_for_paths is set to 0, then a '*' character
/// will match any sub expression. If it is set to 1, then it will
/// match everything except for path separators, and only a "**" double
/// star/double glob will match any sub expression including separators.
///
/// @param is_winpath
/// If parameter is_winpath is set to 0, only '/' is treated as
/// path separator. If set to 1, '/' and '\\' are treated as separators.
S3DEXP int spew3d_globmatch(
    const char *pattern, const char *value,
    int doubleglob_for_paths, int is_winpath
);


/// Similar as spew3d_globmatch(), but you can specify patterns and
/// values as byte buffers that may contain null bytes. Null bytes
/// are treated like just any other normal character.
S3DEXP int spew3d_globmatchbuf(
    const uint8_t *pattern, size_t patternlen,
    const uint8_t *value, size_t valuelen,
    int doubleglob_for_paths, int is_winpath
);

#endif  // SPEW3D_GLOB_H_

