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

#ifdef SPEW3D_IMPLEMENTATION

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


//#define _SPEW3D_GLOBDEBUG

S3DEXP int spew3d_globmatchbuf(
        const uint8_t *pattern, size_t patternlen,
        const uint8_t *value, size_t valuelen,
        int doubleglob_for_paths, int is_winpath
        ) {
    #ifdef _SPEW3D_GLOBDEBUG
    char _patternstrbuf[64];
    char _valuestrbuf[64];
    {
        int i = 0;
        while (i < patternlen && i < 63) {
            _patternstrbuf[i] = (
                (pattern[i] != '\0' &&
                pattern[i] >= 32) ? pattern[i] : '?'
            );
            i += 1;
        }
        _patternstrbuf[i] = '\0';
        i = 0;
        while (i < valuelen && i < 63) {
            _valuestrbuf[i] = (
                (value[i] != '\0' &&
                value[i] >= 32) ? value[i] : '?'
            );
            i += 1;
        }
        _valuestrbuf[i] = '\0';
        printf("spew3d_glob.c: debug: "
            "spew3d_globmatchbuf(\"%s\", %d, "
            "\"%s\", %d, ...) called\n",
            _patternstrbuf, (int)patternlen,
            _valuestrbuf, (int)valuelen);
    }
    #endif
    // First, enforce glob limit:
    int globcount = 0;
    int is_escaped = 0;
    int i = 0;
    while (i < patternlen) {
        if (pattern[i] == '^' && !is_escaped) {
            is_escaped = 1;
            i++;
            continue;
        }
        if (!is_escaped && pattern[i] == '*') {
            globcount++;
            i++;
            while (i < patternlen &&
                    pattern[i] == '*')
                i++;
            continue;
        }
        is_escaped = 0;
        i++;
    }
    if (globcount > SPEW3D_MAX_GLOBS) {
        #ifdef _SPEW3D_GLOBDEBUG
        printf("spew3d_glob.c: debug: "
            "spew3d_globmatchbuf() mismatch, "
            "pattern exceeds glob limit\n");
        #endif
        return 0;
    }

    // Now do matching:
    is_escaped = 0;
    int i2 = 0;
    i = 0;
    while (i < patternlen && i2 < valuelen) {
        #ifdef _SPEW3D_GLOBDEBUG
        printf("spew3d_glob.c: debug: "
            "spew3d_globmatchbuf() comparing "
            "'%c' and '%c'\n",
            pattern[i], value[i2]);
        #endif
        if (pattern[i] == '^' && !is_escaped) {
            is_escaped = 1;
            i++;
            continue;
        }
        if (!is_escaped && pattern[i] == '?' &&
                (!doubleglob_for_paths ||
                (value[i2] != '/' &&
                (!is_winpath || value[i2] != '\\')))) {
            i++;
            i2++;
            continue;
        }
        if (!is_escaped && pattern[i] == '*') {
            int atdoubleglob = 0;
            while (i + 1 < patternlen && pattern[i + 1] == '*') {
                atdoubleglob = 1;
                i++;
            }
            const uint8_t *remainingpattern = NULL;
            size_t remainingpatternlen = 0;
            if (i + 1 >= patternlen) {  // Trailing last '*':
                if (!doubleglob_for_paths)  // Matches everything, done!
                    return 1;
                // Make sure remaining value has no path separator:
                while (i2 < valuelen) {
                    if (value[i2] == '/' ||
                            (is_winpath && value[i2] == '\\')) {
                        #ifdef _SPEW3D_GLOBDEBUG
                        printf("spew3d_glob.c: debug: "
                            "spew3d_globmatchbuf() mismatch at "
                            "pattern position %d\n", (int)i);
                        #endif
                        return 0;
                    }
                    i2++;
                }
                #ifdef _SPEW3D_GLOBDEBUG
                printf("spew3d_glob.c: debug: "
                    "spew3d_globmatchbuf() matched!\n");
                #endif
                return 1;
            }
            // We got still some stuff left in our pattern, match it:
            remainingpattern = &pattern[i + 1];
            remainingpatternlen = patternlen - (i + 1);
            while (i2 < valuelen) {
                if (doubleglob_for_paths && !atdoubleglob &&
                        (value[i2] == '/' ||
                        (is_winpath && value[i2] == '\\'))) {
                    // Encountered a path sep in our pattern.
                    // This is a hard boundary where we need to stop
                    // trying, unless our pattern also has a path sep
                    // right next:
                    if (remainingpatternlen > 0 && (
                            remainingpattern[0] == '/' ||
                            (is_winpath &&
                            remainingpattern[0] == '\\'))) {
                        remainingpattern++;
                        remainingpatternlen--;
                        i2++;
                        continue;
                    }
                    if (remainingpatternlen > 1 &&
                            remainingpattern[0] == '^' && (
                            remainingpattern[1] == '/' ||
                            (is_winpath &&
                            remainingpattern[1] == '\\'))) {
                        remainingpattern++;
                        remainingpatternlen--;
                        i2++;
                        continue;
                    }
                    #ifdef _SPEW3D_GLOBDEBUG
                    printf("spew3d_glob.c: debug: "
                        "spew3d_globmatchbuf() mismatch at "
                        "pattern position %d\n", (int)i);
                    #endif
                    return 0;
                }
                // Try to match remaining substring:
                int result = spew3d_globmatchbuf(
                    remainingpattern, remainingpatternlen,
                    &value[i2], valuelen - i2,
                    doubleglob_for_paths, is_winpath
                );
                if (result) {
                    #ifdef _SPEW3D_GLOBDEBUG
                    printf("spew3d_glob.c: debug: "
                        "spew3d_globmatchbuf() matched via "
                        "recursion!\n");
                    #endif
                    return 1;
                }
                i2++;
            }
            #ifdef _SPEW3D_GLOBDEBUG
            printf("spew3d_glob.c: debug: "
                "spew3d_globmatchbuf() mismatch at "
                "pattern position %d\n", (int)i);
            #endif
            return 0;
        }
        if (pattern[i] != value[i2]) {
            #ifdef _SPEW3D_GLOBDEBUG
            printf("spew3d_glob.c: debug: "
                "spew3d_globmatchbuf() mismatch at "
                "pattern position %d\n", (int)i);
            #endif
            return 0;
        }
        is_escaped = 0;
        i++;
        i2++;
    }
    if (i >= patternlen && i2 >= valuelen) {
        #ifdef _SPEW3D_GLOBDEBUG
        printf("spew3d_glob.c: debug: "
            "spew3d_globmatchbuf() matched!\n");
        #endif
        return 1;
    }
    #ifdef _SPEW3D_GLOBDEBUG
    printf("spew3d_glob.c: debug: "
        "spew3d_globmatchbuf() mismatch at "
        "pattern position %d\n", (int)i);
    #endif
    return 0;
}


S3DEXP int spew3d_globmatch(
        const char *pattern, const char *value,
        int doubleglob_for_paths, int is_winpath
        ) {
    return spew3d_globmatchbuf(
        pattern, strlen(pattern),
        value, strlen(value),
        doubleglob_for_paths, is_winpath
    );
}

#endif  // SPEW3D_IMPLEMENTATION

