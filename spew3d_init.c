

#ifdef SPEW3D_IMPLEMENTATION

#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <unistd.h>


SDL_Window *_internal_spew3d_outputwindow = NULL;
SDL_Renderer *_internal_spew3d_outputrenderer = NULL;


int spew3d_Init(SDL_Window *window, SDL_Renderer *renderer) {
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

#endif  // SPEW3D_IMPLEMENTATION

