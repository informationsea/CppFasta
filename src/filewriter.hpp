#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <cstdio>
#include <string>
#include <zlib.h>
#include <bzlib.h>

namespace cppfasta {

    class BasicWriter
    {
    public:
        BasicWriter() {};
        virtual ~BasicWriter(){};

        virtual bool open(const char *filepath) = 0;
        virtual bool write(const void* data, size_t length) = 0;
        virtual bool printf(const char *format, ...);
    };

    class FileWriter : public BasicWriter
    {
    public:
        FileWriter() : m_file(0) {};
        virtual ~FileWriter();

        virtual bool open(const char *filepath);
        virtual bool write(const void* data, size_t length);
    private:
        std::FILE *m_file;
    };

    class GzipWriter : public BasicWriter
    {
    public:
        GzipWriter() : m_file(0) {};
        virtual ~GzipWriter();
        virtual bool open(const char *filepath);
        virtual bool write(const void* data, size_t length);
    private:
        gzFile m_file;
    };

    class Bzip2Writer : public BasicWriter
    {
    public:
        Bzip2Writer() : m_file(0) {};
        virtual ~Bzip2Writer();
        virtual bool open(const char *filepath);
        virtual bool write(const void* data, size_t length);
    private:
        BZFILE* m_file;
    };

    BasicWriter *openFileForWrite(const char *filepath);
}

#endif /* FILEWRITER_H */
