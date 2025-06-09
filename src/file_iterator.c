#include <stdio.h>
#include <assert.h>

#include "file_iterator.h"

typedef struct _file_it_s
{
    FILE *handle;
    Byte_t cur_char;
} FileIterator_t;

_Static_assert(sizeof(FileIterator_t) <= IT_OPAQUE_DATA_SIZE, "File iterator too large");

void FileIteratorNext(FileIterator_t *it)
{
    it->cur_char = (Byte_t) getc(it->handle);
}

void FileIteratorPrev(FileIterator_t *it)
{
    fseek(it->handle, -2, SEEK_CUR);
    it->cur_char = (Byte_t) getc(it->handle);
}

bool FileIteratorDone(FileIterator_t *it)
{
    Byte_t c = (Byte_t) getc(it->handle);
    if (c != 255)
    {
        ungetc(c, it->handle);
    }

    return c == 255;
}

void *FileIteratorItem(FileIterator_t *it)
{
    return &it->cur_char;
}

void FileIteratorClose(FileIterator_t *it)
{
    fclose(it->handle);
}

Iterator_t NewFileIterator(char *filename)
{
    Iterator_t it = {
        (IteratorMove_t) FileIteratorNext,
        (IteratorMove_t) FileIteratorPrev,
        (IteratorDone_t) FileIteratorDone,
        (IteratorItem_t) FileIteratorItem,
        (IteratorClose_t) FileIteratorClose,
    };

    FileIterator_t *opaque = (FileIterator_t*) it.opaque_data;
    opaque->handle = fopen(filename, "r");
    opaque->cur_char = (Byte_t) getc(opaque->handle);
    assert(opaque->handle != NULL);

    return it;
}