#include "filewriter.hpp"

#include <stdarg.h>
#include <stdlib.h>

namespace cppfasta {

    bool BasicWriter::printf(const char *format, ...)
    {
        va_list ap;
        va_start(ap, format);

        char *temp;
        int len = vasprintf(&temp, format, ap);
        bool success = write(temp, len);
        free(temp);
        
        va_end(ap);
        return success;
    }

    FileWriter::~FileWriter() {
        if (m_file)
            fclose(m_file);
    }

    
    bool FileWriter::open(const char *filepath)
    {
        m_file = std::fopen(filepath, "w");
        if (m_file == NULL) return false;
        return true;
    }

    bool FileWriter::write(const void* data, size_t length)
    {
        return fwrite(data, length, 1, m_file) == 1;
    }

    GzipWriter::~GzipWriter()
    {
        gzclose(m_file);
    }

    bool GzipWriter::open(const char *filepath)
    {
        m_file = gzopen(filepath, "w");
        if (m_file == NULL) return false;
        return true;
    }

    bool GzipWriter::write(const void* data, size_t length)
    {
        return gzwrite(m_file, data, length) == (int)length;
    }

    Bzip2Writer::~Bzip2Writer()
    {
        BZ2_bzclose(m_file);
    }

    bool Bzip2Writer::open(const char *filepath)
    {
        m_file = BZ2_bzopen(filepath, "w");
        if (m_file == NULL) return false;
        return true;
    }

    bool Bzip2Writer::write(const void* data, size_t length)
    {
        char *temp = new char[length];
        memcpy(temp, data, length);
        bool success = BZ2_bzwrite(m_file, temp, length) == (int)length;
        delete temp;
        return success;
    }

    BasicWriter *openFileForWrite(const char *filepath)
    {
        BasicWriter *writer;
        const char *extension = strrchr(filepath, '.');
        
        if (strcmp(".gz", extension) == 0) {
            writer = new GzipWriter;
        } else if (strcmp(".bz2", extension) == 0) {
            writer = new Bzip2Writer;
        } else {
            writer = new FileWriter;
        }

        if (writer->open(filepath))
            return writer;
        delete writer;
        return 0;

    }
}
