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

#ifndef SPEW3D_WINDOW_H_
#define SPEW3D_WINDOW_H_

#include <stdint.h>
#include <SDL2/SDL.h>


typedef struct spew3d_point spew3d_point;

/// Convert coordinates from a mouse event supplied
/// by SDL2 into the 2d canvas draw units.
/// The resulting coordinates match what you'd supply to
/// something like SDL_RenderCopy, SDL_RenderFillRect, or
/// spew3d_texture_Draw to draw at the clicked spot.
/// This conversion is needed e.g. with a High-DPI window.
spew3d_point spew3d_window_EventPointToCanvasDrawPoint(
    int x, int y
);


/// Helper function for how wide the 2d canvas is (that may
/// or may not correspond to screen pixels) for the output
/// window. This unit is also used for SDL_RenderCopy,
/// SDL_RenderFillRect, or spew3d_texture_Draw.
/// This might differ from SDL_GetWindowSize e.g. for a
/// High-DPI window.
int32_t spew3d_window_CanvasDrawWidth();


/// Helper function for how tall the 2d canvas is (that may
/// or may not correspond to screen pixels) for the output
/// window. This unit is also used for SDL_RenderCopy,
/// SDL_RenderFillRect, or spew3d_texture_Draw.
/// This might differ from SDL_GetWindowSize e.g. for a
/// High-DPI window.
int32_t spew3d_window_CanvasDrawHeight();

#endif  // SPEW3D_WINDOW_H_
