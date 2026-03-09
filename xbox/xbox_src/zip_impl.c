// libzip-compatible implementation using miniz for Xbox/nxdk
// Only read operations are functional. Write operations are stubs.
// Uses one-shot extraction instead of streaming iterators for reliability.
#ifdef NXDK

#include "miniz.h"
#include "zip.h"
#include <stdlib.h>
#include <string.h>
#include "xbox_debug.h"

static int s_zip_dbg = 0;

// Internal zip archive structure
struct zip {
    mz_zip_archive mz;
    int is_open;
    zip_error_t error;
    // Cache for zip_get_name - store filenames
    char **filenames;
    mz_uint num_files;
};

// Internal zip file (entry being read) structure
// Uses pre-extracted buffer instead of streaming iterator
struct zip_file {
    void *data;        // extracted file data (malloc'd)
    size_t size;       // total size of data
    size_t read_pos;   // current read position
};

zip_t *zip_open(const char *path, int flags, int *errorp) {
    (void)flags;

    zip_t *z = (zip_t *)calloc(1, sizeof(zip_t));
    if (!z) {
        if (errorp) *errorp = -1;
        return NULL;
    }

    memset(&z->mz, 0, sizeof(z->mz));

    if (!mz_zip_reader_init_file(&z->mz, path, 0)) {
        if (errorp) *errorp = -1;
        free(z);
        return NULL;
    }

    z->is_open = 1;
    z->num_files = mz_zip_reader_get_num_files(&z->mz);

    // Pre-cache all filenames
    z->filenames = (char **)calloc(z->num_files, sizeof(char *));
    if (z->filenames) {
        for (mz_uint i = 0; i < z->num_files; i++) {
            char buf[MZ_ZIP_MAX_ARCHIVE_FILENAME_SIZE];
            if (mz_zip_reader_get_filename(&z->mz, i, buf, sizeof(buf))) {
                z->filenames[i] = strdup(buf);
            }
        }
    }

    if (errorp) *errorp = 0;
    return z;
}

static void zip_free_internal(zip_t *archive) {
    if (!archive) return;

    if (archive->filenames) {
        for (mz_uint i = 0; i < archive->num_files; i++) {
            free(archive->filenames[i]);
        }
        free(archive->filenames);
    }

    if (archive->is_open) {
        mz_zip_reader_end(&archive->mz);
        archive->is_open = 0;
    }

    free(archive);
}

int zip_close(zip_t *archive) {
    if (!archive) return -1;
    zip_free_internal(archive);
    return 0;
}

void zip_discard(zip_t *archive) {
    zip_free_internal(archive);
}

zip_int64_t zip_get_num_entries(zip_t *archive, int flags) {
    (void)flags;
    if (!archive || !archive->is_open) return 0;
    return (zip_int64_t)archive->num_files;
}

const char *zip_get_name(zip_t *archive, zip_uint64_t index, int flags) {
    (void)flags;
    if (!archive || !archive->filenames || index >= archive->num_files) return NULL;
    return archive->filenames[index];
}

zip_int64_t zip_name_locate(zip_t *archive, const char *fname, int flags) {
    (void)flags;
    if (!archive || !archive->is_open || !fname) return -1;

    // Linear search through cached filenames (mz_zip_reader_locate_file is unreliable)
    if (archive->filenames) {
        for (mz_uint i = 0; i < archive->num_files; i++) {
            if (archive->filenames[i] && strcmp(archive->filenames[i], fname) == 0) {
                return (zip_int64_t)i;
            }
        }
    }
    return -1;
}

void zip_stat_init(zip_stat_t *sb) {
    if (sb) memset(sb, 0, sizeof(zip_stat_t));
}

int zip_stat_index(zip_t *archive, zip_uint64_t index, int flags, zip_stat_t *sb) {
    (void)flags;
    if (!archive || !archive->is_open || !sb) return -1;

    mz_zip_archive_file_stat mz_stat;
    if (!mz_zip_reader_file_stat(&archive->mz, (mz_uint)index, &mz_stat)) {
        return -1;
    }

    sb->valid = ZIP_STAT_SIZE | ZIP_STAT_NAME;
    sb->size = mz_stat.m_uncomp_size;
    sb->index = index;
    sb->comp_size = mz_stat.m_comp_size;
    sb->name = archive->filenames ? archive->filenames[index] : NULL;

    s_zip_dbg++;
    if (s_zip_dbg <= 5) {
        xbox_log("zip_stat[%d]: uncomp=%u sb->size=%u sizeof(zip_stat)=%u\n",
                 (int)index, (unsigned)mz_stat.m_uncomp_size,
                 (unsigned)sb->size, (unsigned)sizeof(zip_stat_t));
    }

    return 0;
}

// Extract the entire file to memory in one shot (no streaming)
zip_file_t *zip_fopen_index(zip_t *archive, zip_uint64_t index, int flags) {
    (void)flags;
    if (!archive || !archive->is_open) return NULL;

    size_t uncomp_size = 0;
    void *data = mz_zip_reader_extract_to_heap(&archive->mz, (mz_uint)index, &uncomp_size, 0);
    if (!data) {
        if (s_zip_dbg <= 5) {
            xbox_log("zip_fopen_index[%d]: extract_to_heap FAILED\n", (int)index);
        }
        return NULL;
    }

    zip_file_t *f = (zip_file_t *)calloc(1, sizeof(zip_file_t));
    if (!f) {
        free(data);
        return NULL;
    }

    f->data = data;
    f->size = uncomp_size;
    f->read_pos = 0;

    if (s_zip_dbg <= 5) {
        xbox_log("zip_fopen_index[%d]: extracted %u bytes OK\n", (int)index, (unsigned)uncomp_size);
    }

    return f;
}

zip_int64_t zip_fread(zip_file_t *file, void *buf, zip_uint64_t nbytes) {
    if (!file || !file->data || !buf) return -1;

    size_t remaining = file->size - file->read_pos;
    size_t to_read = (size_t)nbytes;
    if (to_read > remaining) to_read = remaining;

    memcpy(buf, (char *)file->data + file->read_pos, to_read);
    file->read_pos += to_read;

    return (zip_int64_t)to_read;
}

int zip_fclose(zip_file_t *file) {
    if (!file) return 0;
    if (file->data) {
        free(file->data);
    }
    free(file);
    return 0;
}

// Write operation stubs - not supported on Xbox
zip_source_t *zip_source_buffer(zip_t *archive, const void *data, zip_uint64_t len, int freep) {
    (void)archive; (void)data; (void)len; (void)freep;
    return NULL;
}

void zip_source_free(zip_source_t *source) { (void)source; }

int zip_file_add(zip_t *archive, const char *name, zip_source_t *source, int flags) {
    (void)archive; (void)name; (void)source; (void)flags;
    return -1;
}

static zip_error_t s_dummy_error = { -1, 0 };

zip_error_t *zip_get_error(zip_t *archive) {
    if (!archive) return &s_dummy_error;
    return &archive->error;
}

const char *zip_error_strerror(zip_error_t *error) {
    (void)error;
    return "zip write not supported on Xbox";
}

int zip_error_code_zip(zip_error_t *error) {
    if (!error) return -1;
    return error->zip_err;
}

#endif // NXDK
