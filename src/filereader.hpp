#ifndef FILEREADER_H
#define FILEREADER_H

#include <cstdio>
#include <string>
#include <zlib.h>
#include <bzlib.h>

#define FILEREADER_BUFFER_SIZE 1000

namespace cppfasta {

    class BasicReader
    {
    public:
        BasicReader(){};
        virtual ~BasicReader(){};

        virtual bool open(const char *filepath) = 0;
        virtual char *fgets(char * str, int size) = 0;
        virtual std::string readLine();
    };

    class FileReader : public BasicReader
    {
    public:
        FileReader() : m_file(0), autoclose(true){};
        virtual ~FileReader();

        virtual bool open(const char *filepath);
        virtual bool open(FILE *file);
        virtual char *fgets(char * str, int size);
    
    private:
        std::FILE *m_file;
        bool autoclose;
    };


    class GzipReader : public BasicReader
    {
    public:
        GzipReader() : m_gzfile(0){};
        virtual ~GzipReader();
    
        virtual bool open(const char *filepath);
        virtual char *fgets(char * str, int size);

    private:
        gzFile m_gzfile;
    };

    class Bzip2Reader : public BasicReader
    {
    public:
        Bzip2Reader();
        virtual ~Bzip2Reader();

        virtual bool open(const char *filepath);
        virtual char *fgets(char * str, int size);

    private:
        BZFILE *m_bzFile;
        char m_buf[FILEREADER_BUFFER_SIZE+1];
        int m_bufSize;
        int m_position;
    };


    BasicReader *openFile(const char *filepath);
}

#endif /* FILEREADER_H */


