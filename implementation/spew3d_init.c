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
#include <string.h>
#include <SDL2/SDL.h>
#include <unistd.h>


SDL_Window *_internal_spew3d_outputwindow = NULL;
SDL_Renderer *_internal_spew3d_outputrenderer = NULL;


int spew3d_InitFromManualSDLInit(
        SDL_Window *window, SDL_Renderer *renderer
        ) {
    assert(_internal_spew3d_outputwindow == NULL);
    assert(_internal_spew3d_outputrenderer == NULL);
    if (_internal_spew3d_outputwindow != NULL ||
            _internal_spew3d_outputrenderer != NULL)
        return 0;
    assert(window != NULL && renderer != NULL);
    if (window == NULL || renderer == NULL)
        return 0;
    _internal_spew3d_outputwindow = window;
    _internal_spew3d_outputrenderer = renderer;
    return 1;
}

int spew3d_Init(
        const char *title, int initflags,
        SDL_Window **out_window, SDL_Renderer **out_renderer
        ) {
    SDL_SetHintWithPriority(
        SDL_HINT_FRAMEBUFFER_ACCELERATION, "0",
        SDL_HINT_OVERRIDE);

    SDL_Window *window = SDL_CreateWindow(
        title, SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, 800, 500,
        SDL_WINDOW_RESIZABLE|
        (((initflags & SPEW3D_INITFLAG_FULLSCREEN) != 0) ?
            SDL_WINDOW_FULLSCREEN:0)|
        (((initflags & SPEW3D_INITFLAG_FORCE_SOFTWARE_RENDERED) == 0) ?
            SDL_WINDOW_OPENGL:0)|
        SDL_WINDOW_ALLOW_HIGHDPI);
    if (!window && (((initflags & SPEW3D_INITFLAG_FORCE_OPENGL) != 0)))
        return 0;
    SDL_Renderer *renderer = NULL;
    if (window) {
        renderer = SDL_CreateRenderer(window, -1,
            SDL_RENDERER_PRESENTVSYNC|
            (((initflags & SPEW3D_INITFLAG_FORCE_SOFTWARE_RENDERED) == 0) ?
                SDL_RENDERER_ACCELERATED:SDL_RENDERER_SOFTWARE));
        if (!renderer) {
            SDL_DestroyWindow(window);
            window = NULL;
            if ((initflags & SPEW3D_INITFLAG_FORCE_OPENGL) != 0)
                return 0;
        } else {
            if (!spew3d_InitFromManualSDLInit(window, renderer)) {
                SDL_DestroyWindow(window);
                window = NULL;
                SDL_DestroyRenderer(renderer);
                renderer = NULL;
                return 1;
            }
            *out_window = window;
            *out_renderer = renderer;
            return 1;
        }
    }
    window = SDL_CreateWindow(
        title, SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, 800, 500,
        SDL_WINDOW_RESIZABLE|
        (((initflags & SPEW3D_INITFLAG_FULLSCREEN) != 0) ?
            SDL_WINDOW_FULLSCREEN:0)|
        SDL_WINDOW_ALLOW_HIGHDPI);
    if (!window)
        return 0;
    renderer = SDL_CreateRenderer(window, -1,
        SDL_RENDERER_PRESENTVSYNC|SDL_RENDERER_SOFTWARE);
    if (!renderer) {
        SDL_DestroyWindow(window);
        window = NULL;
        return 0;
    }

    assert(window != NULL && renderer != NULL);
    if (!spew3d_InitFromManualSDLInit(window, renderer)) {
        SDL_DestroyWindow(window);
        window = NULL;
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
        return 1;
    }

    *out_window = window;
    *out_renderer = renderer;
    return 1;
}

#endif  // SPEW3D_IMPLEMENTATION

