// libzip-compatible API backed by miniz for Xbox/nxdk
// Provides the subset of libzip used by libultraship's O2rArchive.
#pragma once

#include <stdint.h>
#include <stddef.h>

typedef int64_t zip_int64_t;
typedef uint64_t zip_uint64_t;
typedef int32_t zip_int32_t;
typedef uint32_t zip_uint32_t;
typedef uint16_t zip_uint16_t;
typedef int8_t zip_int8_t;
typedef uint8_t zip_uint8_t;

// Opaque types
typedef struct zip zip_t;
typedef struct zip_file zip_file_t;
typedef struct zip_source zip_source_t;

typedef struct zip_stat {
    zip_uint64_t valid;
    const char *name;
    zip_uint64_t index;
    zip_uint64_t size;
    zip_uint64_t comp_size;
} zip_stat_t;

typedef struct zip_error {
    int zip_err;
    int sys_err;
} zip_error_t;

// Flag constants
#define ZIP_FL_UNCHANGED 0
#define ZIP_FL_NOCASE 1
#define ZIP_FL_NODIR 2
#define ZIP_FL_COMPRESSED 4
#define ZIP_FL_ENC_GUESS 0
#define ZIP_RDONLY 16
#define ZIP_CREATE 1
#define ZIP_STAT_SIZE 0x0002
#define ZIP_STAT_NAME 0x0001
#define ZIP_FL_ENC_UTF_8 0x0800
#define ZIP_FL_OVERWRITE 0x2000

#ifdef __cplusplus
extern "C" {
#endif

// Archive operations
zip_t *zip_open(const char *path, int flags, int *errorp);
int zip_close(zip_t *archive);
void zip_discard(zip_t *archive);

// Entry enumeration
zip_int64_t zip_get_num_entries(zip_t *archive, int flags);
const char *zip_get_name(zip_t *archive, zip_uint64_t index, int flags);
zip_int64_t zip_name_locate(zip_t *archive, const char *fname, int flags);

// Entry info
void zip_stat_init(zip_stat_t *sb);
int zip_stat_index(zip_t *archive, zip_uint64_t index, int flags, zip_stat_t *sb);

// File reading
zip_file_t *zip_fopen_index(zip_t *archive, zip_uint64_t index, int flags);
zip_int64_t zip_fread(zip_file_t *file, void *buf, zip_uint64_t nbytes);
int zip_fclose(zip_file_t *file);

// Write operations (stubs - not supported on Xbox)
zip_source_t *zip_source_buffer(zip_t *archive, const void *data, zip_uint64_t len, int freep);
void zip_source_free(zip_source_t *source);
int zip_file_add(zip_t *archive, const char *name, zip_source_t *source, int flags);
zip_error_t *zip_get_error(zip_t *archive);
const char *zip_error_strerror(zip_error_t *error);
int zip_error_code_zip(zip_error_t *error);

#ifdef __cplusplus
}
#endif
