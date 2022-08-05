

#ifdef SPEW3D_IMPLEMENTATION

#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <unistd.h>

static inline uint16_t spew3d_simplehash(const char *k);

uint64_t _internal_spew3d_texlist_count;
spew3d_texture_info *_internal_spew3d_texlist;

#define SPEW3D_TEXLIST_IDHASHMAP_SIZE 2048
typedef struct spew3d_texlist_idhashmap_bucket
    spew3d_texlist_idhashmap_bucket;
typedef struct spew3d_texlist_idhashmap_bucket {
    uint64_t texlist_slot_idx;
    spew3d_texlist_idhashmap_bucket *next;
} spew3d_texlist_idhashmap_bucket;
spew3d_texlist_idhashmap_bucket *_internal_spew3d_texlist_hashmap = NULL;


static void __attribute__((constructor)) _internal_spew3d_ensure_texhash() {
    if (_internal_spew3d_texlist_hashmap != NULL)
        return;

    _internal_spew3d_texlist_hashmap = malloc(
        sizeof(*_internal_spew3d_texlist_hashmap) *
        SPEW3D_TEXLIST_IDHASHMAP_SIZE
    );
    if (!_internal_spew3d_texlist_hashmap) {
        fprintf(stderr, "spew3d_texture.c: error: "
            "failed to allocate _internal_spew3d_texlist_hashmap");
        _exit(1);
    }
}


void _internal_normpath(char *p) {
    uint32_t plen = strlen(p);
    uint32_t i = 0;
    while (i < plen) {
        if (p[i] == '\\') {
            p[i] = '/';
        }
        if (p[i] == '/' && i > 0 && p[i - 1] == '/') {
            if (i + 1 < plen)
                memcpy(&p[i], &p[i + 1],
                    (plen - i - 1));  // Ignores null terminator!
            plen -= 1;
            p[plen] = '\0';  // Re-add null terminator.
            // Don't do i++!
            continue; 
        } else if (p[i] == '/' && i > 0 && p[i - 1] == '.' &&
                (i <= 1 || p[i - 2] == '/')) {
            if (i + 1 < plen)
                memcpy(&p[i - 1], &p[i + 1],
                    (plen - i));  // Ignores null terminator!
            plen -= 2;
            p[plen] = '\0';  // Re-add null terminator.
            i--;  // Intentional.
            continue;
        } else if (p[i] == '/' && i + 1 >= plen) {
            plen--;
            p[plen] = '\0';
            break;
        }
        i++;
    }
}


char *_internal_tex_get_buf = NULL;
uint32_t _internal_tex_get_buf_size = 0;


spew3d_texture_t _internal_spew3d_texture_ByFileOrName(
        const char *name, const char *path, int fromfile
        ) {
    uint32_t idlen = (fromfile ? strlen(path) : strlen(name));
    const char *id = (fromfile ? path : name);
    if (idlen >= _internal_tex_get_buf_size) {
        uint32_t newsize = (
            idlen + 20
        );
        char *_internal_tex_get_buf_new = malloc(
            newsize);
        if (!_internal_tex_get_buf_new)
            return 0;
        _internal_tex_get_buf =
            _internal_tex_get_buf_new;
    }
    memcpy(_internal_tex_get_buf, id, idlen + 1);
    if (fromfile) {
        _internal_normpath(_internal_tex_get_buf);
        #if defined(_WIN32) || defined(_WIN64)
        if (strlen(_internal_tex_get_buf) >= 2 &&
                _internal_tex_get_buf[1] == ':')
            return 0;
        #endif
        if (strlen(_internal_tex_get_buf) >= 1 &&
                _internal_tex_get_buf[0] == '/')
            return 0;
        if (strlen(_internal_tex_get_buf) >= 2 &&
                _internal_tex_get_buf[0] == '.' &&
                _internal_tex_get_buf[1] == '.' && (
                strlen(_internal_tex_get_buf) == 2 ||
                _internal_tex_get_buf[1] == '/'))
            return 0;
    }
    if (strlen(_internal_tex_get_buf) == 0)
        return 0;

    int32_t i = 1;
    while (i <= _internal_spew3d_texlist_count) {
        if (_internal_spew3d_texlist[i - 1].idstring != NULL &&
                _internal_spew3d_texlist[i - 1].fromfile ==
                (fromfile != 0) &&
                strcmp(_internal_spew3d_texlist[i - 1].idstring,
                   _internal_tex_get_buf) == 0) {
            return i;
        }
        i++;
    }
    int64_t newcount = _internal_spew3d_texlist_count + 1;
    spew3d_texture_info *new_texlist = realloc(
        _internal_spew3d_texlist,
        sizeof(*new_texlist) * newcount
    );
    if (!new_texlist)
        return 0;
    _internal_spew3d_texlist = new_texlist;
    char *iddup = strdup(_internal_tex_get_buf);
    if (!iddup)
        return 0;
    spew3d_texture_info *newinfo = &_internal_spew3d_texlist[
        _internal_spew3d_texlist_count
    ];
    _internal_spew3d_texlist_count++;
    memset(newinfo, 0, sizeof(*newinfo));
    newinfo->idstring = iddup;
    newinfo->fromfile = (fromfile != 0);
    newinfo->loaded = 0;
    return _internal_spew3d_texlist_count;
}


spew3d_texture_t spew3d_texture_ByFile(
        const char *path
        ) {
    return _internal_spew3d_texture_ByFileOrName(
        NULL, path, 1
    );
}


spew3d_texture_t spew3d_texture_ByName(
        const char *name, uint32_t w, uint32_t h
        ) {
    spew3d_texture_t tex = (
        _internal_spew3d_texture_ByFileOrName(name, NULL, 0)
    );
    if (tex == 0)
        return 0;
    spew3d_texture_info *tinfo = spew3d_texinfo(tex);
    assert(tinfo != NULL); 
    assert(!tinfo->fromfile);
    if (!tinfo->loaded) {
        assert(tex == _internal_spew3d_texlist_count - 1);
        tinfo->w = w;
        tinfo->h = h;
        int64_t pixelcount = ((int64_t)w) * ((int64_t)h);
        if (pixelcount <= 0) pixelcount = 1;
        tinfo->pixels = malloc(4 * pixelcount);
        if (!tinfo->pixels) {
            free(tinfo->idstring);
            _internal_spew3d_texlist_count--;
            return 0;
        }
        tinfo->loaded = 1;
    }
    return tex;
}


void spew3d_texture_Destroy(spew3d_texture_t tid) {
    assert(tid >= 0 && tid < _internal_spew3d_texlist_count);
    if (tid == 0)
        return;
    spew3d_texture_info *tinfo = spew3d_texinfo(tid);
    assert(tinfo != NULL);
    assert(!tinfo->fromfile);
    if (!tinfo->loaded)
        return;
    free(tinfo->pixels);
    free(tinfo->idstring);
    tinfo->idstring = NULL;
    tinfo->pixels = NULL;
    tinfo->loaded = 0;
}


spew3d_texture_t spew3d_texture_ByNameAsWritableCopy(
        const char *name, const char *original_path
        ) {

}

#endif  // SPEW3D_IMPLEMENTATION
