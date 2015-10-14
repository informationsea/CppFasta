#include "filereader.hpp"

#include <cstring>
#include <sstream>

#define DEBUG(fmt,...) fprintf(stderr, __FILE__ ": %3d: " fmt "\n" ,__LINE__, ## __VA_ARGS__)

namespace cppfasta {
    using namespace std;

    BasicReader *openFile(const char *filepath)
    {
        BasicReader *reader;
        const char *extension = strrchr(filepath, '.');
        
        if (extension && strcmp(".gz", extension) == 0) {
            reader = new GzipReader;
        } else if (extension && strcmp(".bz2", extension) == 0) {
            reader = new Bzip2Reader;
        } else {
            reader = new FileReader;
        }

        if (reader->open(filepath))
            return reader;
        delete reader;
        return 0;
    }

    std::string BasicReader::readLine()
    {
        stringstream stream(std::ios_base::out);
        do {
            char buf[200];
            if (fgets(buf, sizeof(buf)) != buf)
                break;
            stream.write(buf, strlen(buf));
            if (strchr(buf, '\n') != NULL)
                break;
        } while(1);
        
        return stream.str();
    }
    

    FileReader::~FileReader()
    {
        fclose(m_file);
    }

    bool FileReader::open(const char *filepath)
    {
        if (m_file) return false;
        m_file = std::fopen(filepath, "r");
        if (m_file == NULL)
            return false;
        return true;
    }
    
    bool FileReader::open(FILE *file)
    {
        if (m_file) return false;
        m_file = file;
        if (m_file == NULL) return false;
        return true;
    }

    char *FileReader::fgets(char * str, int size)
    {
        return ::fgets(str, size, m_file);
    }


    GzipReader::~GzipReader()
    {
        gzclose(m_gzfile);
    }

    bool GzipReader::open(const char *filepath)
    {
        if (m_gzfile) return false;
        
        m_gzfile = gzopen(filepath, "r");
        if (m_gzfile == NULL)
            return false;
        return true;
    }

    char *GzipReader::fgets(char * str, int size)
    {
        return gzgets(m_gzfile, str, size);
    }

    Bzip2Reader::Bzip2Reader() :
        m_bzFile(0), m_bufSize(0), m_position(0)
    {
        bzero(m_buf, sizeof(m_buf));
    }

    Bzip2Reader::~Bzip2Reader() {
        BZ2_bzclose(m_bzFile);
    }

    bool Bzip2Reader::open(const char *filepath)
    {
        if (m_bzFile) return false;
        
        m_bzFile = BZ2_bzopen(filepath, "r");
        if (m_bzFile == NULL)
            return false;
        m_bufSize = BZ2_bzread(m_bzFile, m_buf, FILEREADER_BUFFER_SIZE);
        return true;
    }

    char *Bzip2Reader::fgets(char * str, int size)
    {
        if (m_bufSize != FILEREADER_BUFFER_SIZE && m_bufSize == m_position)
            return NULL;
        
        char *original = str;
        size -= 1; // terminate
        do {
            char *pos = strchr(m_buf + m_position, '\n');
            if (pos != NULL) {
                int len = pos - (m_buf + m_position) + 1;
                if (len < size) {
                    strncpy(str, m_buf + m_position, len);
                    m_position += len;
                    str[len] = '\0';
                    //DEBUG("Normal?");
                    return original;
                }
                
                strncpy(str, m_buf + m_position, size);
                m_position += size;
                //DEBUG("Full buffer?");
                str[size] = '\0';
                return original;
                
            }
            
            int remainlen = m_bufSize - m_position;
            if (remainlen >= size) {
                strncpy(str, m_buf + m_position, size);
                m_position += size;
                str[size] = '\0';
                //DEBUG("Full Buffer2");
                return original;
            }

            strncpy(str, m_buf + m_position, remainlen);
            str += remainlen;
            size -= remainlen;

            //DEBUG("Buffer rotated");
            
            m_bufSize = BZ2_bzread(m_bzFile, m_buf, FILEREADER_BUFFER_SIZE);
            if (m_bufSize == 0) return NULL;
            m_position = 0;
        } while(1);
    }

}
