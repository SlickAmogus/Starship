// Stub vorbis/vorbisfile.h for Xbox build
// OGG Vorbis playback is not supported on Xbox
#pragma once

#include <stdint.h>
#include <stddef.h>

typedef int64_t ogg_int64_t;

typedef struct {
    long rate;
    int channels;
} vorbis_info;

typedef struct {
    void* datasource;
} OggVorbis_File;

typedef struct {
    void* data;
    size_t pos;
    size_t size;
} OggFileData;

typedef struct {
    size_t (*read_func)(void*, size_t, size_t, void*);
    int (*seek_func)(void*, ogg_int64_t, int);
    int (*close_func)(void*);
    long (*tell_func)(void*);
} ov_callbacks;

#ifdef __cplusplus
extern "C" {
#endif

static inline int ov_open_callbacks(void* d, OggVorbis_File* vf, const char* i, long ib, ov_callbacks c) {
    (void)d; (void)vf; (void)i; (void)ib; (void)c;
    return -1; // always fail - not supported
}
static inline vorbis_info* ov_info(OggVorbis_File* vf, int l) {
    (void)vf; (void)l;
    static vorbis_info dummy = {32000, 1};
    return &dummy;
}
static inline ogg_int64_t ov_pcm_total(OggVorbis_File* vf, int l) { (void)vf; (void)l; return 0; }
static inline long ov_read(OggVorbis_File* vf, char* b, int len, int be, int w, int s, int* bs) {
    (void)vf; (void)b; (void)len; (void)be; (void)w; (void)s; (void)bs;
    return 0;
}
static inline int ov_clear(OggVorbis_File* vf) { (void)vf; return 0; }

#ifdef __cplusplus
}
#endif
