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

#include <assert.h>
#include <check.h>

#define SPEW3D_OPTION_DISABLE_SDL
#define SPEW3D_IMPLEMENTATION
#include "spew3d.h"

#include "testmain.h"

START_TEST (test_glob)
{
    int result;
    result = spew3d_globmatch(
        "abc", "abc", 0, 0
    );
    assert(result == 1);
    result = spew3d_globmatch(
        "abc", "def", 0, 0
    );
    assert(result == 0);
    result = spew3d_globmatch(
        "abc*", "def", 0, 0
    );
    assert(result == 0);
    result = spew3d_globmatch(
        "abc*", "abdef", 0, 0
    );
    assert(result == 0);
    result = spew3d_globmatch(
        "abc*", "abcdef", 0, 0
    );
    assert(result == 1);
    result = spew3d_globmatch(
        "abc*ef", "abcdef", 0, 0
    );
    assert(result == 1);
    result = spew3d_globmatch(
        "abc*de", "abcdef", 0, 0
    );
    assert(result == 0);
    result = spew3d_globmatch(
        "abc*de*", "abcdef", 0, 0
    );
    assert(result == 1);
    result = spew3d_globmatch(
        "abc*de?", "abcdef", 0, 0
    );
    assert(result == 1);
    result = spew3d_globmatch(
        "abc*de?", "abcdefg", 0, 0
    );
    assert(result == 0);
    result = spew3d_globmatch(
        "abc*de*", "abcdefg", 0, 0
    );
    assert(result == 1);

    // Test escaping examples:

    result = spew3d_globmatch(
        "abc^*de*", "abcdefg", 0, 0
    );
    assert(result == 0);
    result = spew3d_globmatch(
        "abc^*de*", "abc*deefg", 0, 0
    );
    assert(result == 1);
    result = spew3d_globmatch(
        "abc^^*de*", "abc^deefg", 0, 0
    );
    assert(result == 1);
    result = spew3d_globmatch(
        "abc^^x*de*", "abc^deefg", 0, 0
    );
    assert(result == 0);

    // Test file path examples:

    result = spew3d_globmatch(
        "abc*de*", "abc/defg", 1, 0
    );
    assert(result == 0);
    result = spew3d_globmatch(
        "abc*/de*", "abc/defg", 1, 0
    );
    assert(result == 1);
    result = spew3d_globmatch(
        "abc**de*", "abc/defg", 1, 0
    );
    assert(result == 1);
    result = spew3d_globmatch(
        "abc?de", "abc/de", 1, 0
    );
    assert(result == 0);
    result = spew3d_globmatch(
        "abc?de", "abc\\de", 1, 0
    );
    assert(result == 1);
    result = spew3d_globmatch(
        "abc?de", "abc\\de", 1, 1
    );
    assert(result == 0);
    result = spew3d_globmatch(
        "abc*x/de", "abcx/de", 1, 0
    );
    assert(result == 1);
}
END_TEST

TESTS_MAIN(test_glob)
