#ifndef CPPFASTA2_H
#define CPPFASTA2_H

#include <stdio.h>
#include <string.h>

#ifndef CPPFASTA_MAXIMUM_SEQUENCE_LENGTH
#define CPPFASTA_MAXIMUM_SEQUENCE_LENGTH 200
#endif

namespace cppfasta {
    struct SequenceRecord2
    {
        char name[CPPFASTA_MAXIMUM_SEQUENCE_LENGTH];
        char sequence[CPPFASTA_MAXIMUM_SEQUENCE_LENGTH];
        char quality[CPPFASTA_MAXIMUM_SEQUENCE_LENGTH];

        SequenceRecord2() {}
        
        SequenceRecord2(const char *name, const char *sequence, const char *quality = "") {
            bzero(this->name, sizeof(this->name));
            bzero(this->sequence, sizeof(this->sequence));
            bzero(this->quality, sizeof(this->quality));
            
            strncpy(this->name, name, CPPFASTA_MAXIMUM_SEQUENCE_LENGTH-1);
            strncpy(this->sequence, sequence, CPPFASTA_MAXIMUM_SEQUENCE_LENGTH-1);
            strncpy(this->quality, quality, CPPFASTA_MAXIMUM_SEQUENCE_LENGTH-1);
        }
    };

    class FastqReader2
    {
    public:
        FastqReader2(FILE *file) {
            m_file = file;
        }
        
        virtual ~FastqReader2() {
            
        }

        bool next(struct SequenceRecord2 *record) {
            char *result;

            result = fgets(dummy, CPPFASTA_MAXIMUM_SEQUENCE_LENGTH-1, m_file);
            if (!clean(result)) goto onerror;
            if (dummy[0] != '@') {
                fprintf(stderr, "Input file is not FASTQ file\n");
                return false;
            }
            strncpy(record->name, result+1, CPPFASTA_MAXIMUM_SEQUENCE_LENGTH-1); // skip @
            record->name[CPPFASTA_MAXIMUM_SEQUENCE_LENGTH-1] = '\0'; //terminate
            
            result = fgets(record->sequence, CPPFASTA_MAXIMUM_SEQUENCE_LENGTH-1, m_file);
            if (!clean(result)) goto onerror;
            
            result = fgets(dummy, CPPFASTA_MAXIMUM_SEQUENCE_LENGTH-1, m_file);
            if (!clean(result)) goto onerror;
            
            result = fgets(record->quality, CPPFASTA_MAXIMUM_SEQUENCE_LENGTH-1, m_file);
            if (!clean(result)) goto onerror;

            return true;
        onerror:
            if (result == NULL) {
                if (!feof(m_file))
                    perror("Cannot read FASTQ file");
            } else {
                fprintf(stderr, "Too long line. Please consider increase CPPFASTA_MAXIMUM_SEQUENCE_LENGTH in cppfasta2.hpp\n");
            }
            return false;
        }
        
    private:
        FILE *m_file;
        char dummy[CPPFASTA_MAXIMUM_SEQUENCE_LENGTH];
        
        inline bool clean(char *line) {
            if (line == NULL)
                return false;
            
            char *pos;
            pos = strchr(line, '\r');
            if (pos != NULL) {
                *pos = '\0';
                return true;
            }
            
            pos = strchr(line, '\n');
            if (pos != NULL) {
                *pos = '\0';
                return true;
            }
            return false;
        }
    };
}

#endif /* CPPFASTA2_H */

