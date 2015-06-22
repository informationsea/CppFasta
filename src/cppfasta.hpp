#ifndef CPPFASTA_H
#define CPPFASTA_H

#include <string>

namespace cppfasta {

    class BasicReader;

    class SequenceRecord
    {
    public:
        SequenceRecord(const std::string &name, const std::string &sequence, const std::string &quality) :
            m_name(name), m_sequence(sequence), m_hasQuality(true), m_quality(quality) {}
        SequenceRecord(const std::string &name, const std::string &sequence) :
            m_name(name), m_sequence(sequence), m_hasQuality(false), m_quality() {}
        virtual ~SequenceRecord(){}

        const std::string& name() const {return m_name;}
        const std::string& sequence() const {return m_sequence;}
        const std::string& quality() const {return m_quality;}
        bool hasQuality() const {return m_hasQuality;}
        
    private:
        std::string m_name;
        std::string m_sequence;
        bool m_hasQuality;
        std::string m_quality;
    };
    
    
    class FastxReader
    {
    public:
        FastxReader() : m_reader(0) {};
        virtual ~FastxReader();

        virtual bool open(const char *filepath);
        virtual bool hasNext() = 0;
        virtual SequenceRecord* nextRecord() = 0;

    protected:
        BasicReader *m_reader;
    };

    /**
     * Do not use this class directly. This class is prepared to implement FastaReader and FastqReader simply
     */
    class FastxReaderWithNextLoad : public FastxReader
    {
    public:
        FastxReaderWithNextLoad(){};
        virtual ~FastxReaderWithNextLoad(){};
        
        virtual bool open(const char *filepath);
        virtual bool hasNext();
        virtual SequenceRecord* nextRecord();

    protected:
        SequenceRecord *m_nextRecord;
        virtual void loadNext() = 0;
    };

    
    class FastaReader : public FastxReaderWithNextLoad
    {
    public:
        FastaReader(){};
        virtual ~FastaReader(){};

    protected:
        virtual void loadNext();
    private:
        std::string unprocessedLine;
    };

    class FastqReader : public FastxReaderWithNextLoad
    {
    public:
        FastqReader(){};
        virtual ~FastqReader(){};

    protected:
        virtual void loadNext();
    };

}

#endif /* CPPFASTA_H */
