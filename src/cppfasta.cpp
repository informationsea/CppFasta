#include "cppfasta.hpp"
#include "filereader.hpp"
#include <string>
#include <sstream>

namespace cppfasta {

    FastxReader::~FastxReader() {
        delete m_reader;
    }
    
    bool FastxReader::open(const char *filepath)
    {
        m_reader = cppfasta::openFile(filepath);
        if (m_reader) {
            return true;
        }
        return false;
    }

    // ---- FastxReaderWithNextLoad implementation ---

    bool FastxReaderWithNextLoad::hasNext()
    {
        if (m_reader == NULL) return false;
        return m_nextRecord != NULL;
    }

    SequenceRecord* FastxReaderWithNextLoad::nextRecord()
    {
        SequenceRecord* value = m_nextRecord;
        loadNext();
        return value;
    }

    bool FastxReaderWithNextLoad::open(const char *filepath)
    {
        bool ret = FastxReader::open(filepath);
        if (ret)
            loadNext();
        return ret;
    }

    void FastaReader::loadNext()
    {
        {
            if (unprocessedLine.size() == 0)
                unprocessedLine = m_reader->readLine();
            if (unprocessedLine.size() == 0) goto finish;

            std::string name = unprocessedLine.substr(1, unprocessedLine.size() - 2);
            std::string line;
            std::stringstream sequence(std::ios_base::out);
            while (1) {
                line = m_reader->readLine();
                if (line.size() == 0) {
                    unprocessedLine = "";
                    break;
                }
                
                if (line[0] == '>') {
                    unprocessedLine = line;
                    break;
                }
                sequence << line.substr(0, line.size()-1);
            }
            m_nextRecord = new SequenceRecord(name, sequence.str());
        }
        return;
    finish:
        m_nextRecord = NULL;
    }


    void FastqReader::loadNext()
    {
        {
            std::string name = m_reader->readLine();
            if (name.length() == 0) goto finish;
            std::string sequence = m_reader->readLine();
            if (sequence.length() == 0) goto finish;
            std::string name2 = m_reader->readLine();
            if (name2.length() == 0) goto finish;
            std::string quality = m_reader->readLine();
            if (quality.length() == 0) goto finish;
            m_nextRecord = new SequenceRecord(name.substr(1, name.size()-2),
                                              sequence.substr(0, sequence.size()-1),
                                              quality.substr(0, quality.size()-1));
        }
        return;
    finish:
        m_nextRecord = NULL;
    }
}
