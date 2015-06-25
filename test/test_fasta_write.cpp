#include "gtest/gtest.h"
#include "cppfasta.hpp"
#include <limits.h>

using namespace cppfasta;

namespace {
    SequenceRecord records[] = {
        SequenceRecord("TEST1", "ATCG", "????"),
        SequenceRecord("TEST2", "ATTGGG", "??????"),
        SequenceRecord("TEST3", "GATTCCA", "???????")
    };
    
    SequenceRecord records2[] = {
        SequenceRecord("TEST1", "ATCG"),
        SequenceRecord("TEST2", "ATTGGG"),
        SequenceRecord("TEST3", "GATTCCA"),
        SequenceRecord("TEST4",
                       "AGCAAAAGCAGGGTGACAAAGACATAATGGATTCCAACACTGTGTCAAGTTTCCAGGTAGATTGCTTTCTTTGGCATATC"
                       "CGGAAACAAGTTGTAGACCAAGAACTGAGTGATGCCCCATTCCTTGATCGGCTTCGCCGAGATCAGAGGTCCCTAAGGGG"
                       "AAGAGGCAATACTCTCGGTCTAGACATCAAAGCAGCCACCCATGTTGGAAAGCAAATTGTAGAAAAGATTCTGAAAGAAG"
                       "AATCTGATGAGGCACTTAAAATGACCATGGTCTCCACACCTGCTTCGCGATACATAACTGACATGACTATTGAGGAATTG"
                       "TCAAGAAACTGGTTCATGCTAATGCCCAAGCAGAAAGTGGAAGGACCTCTTTGCATCAGAATGGACCAGGCAATCATGGA"
                       "GAAAAACATCATGTTGAAAGCGAATTTCAGTGTGATTTTTGACCGACTAGAGACCATAGTATTACTAAGGGCTTTCACCG"
                       "AAGAGGGAGCAATTGTTGGCGAAATCTCACCATTGCCTTCTTTTCCAGGACATACTATTGAGGATGTCAAAAATGCAATT"
                       "GGGGTCCTCATCGGAGGACTTGAATGGAATGATAACACAGTTCGAGTCTCTAAAAATCTACAGAGATTCGCTTGGAGAAG"
                       "CAGTAATGAGAATGGGGGACCTCCACTTACTCCAAAACAGAAACGGAAAATGGCGAGAACAGCTAGGTCAAAAGTTTGAA"
                       "GAGATAAGATGGCTGATTGAAGAAGTGAGACACAGACTAAAAACAACTGAAAATAGCTTTGAACAAATAACATTCATGCA"
                       "AGCATTACAACTGCTGTTTGAAGTGGAACAGGAGATAAGAACTTTCTCATTTCAGCTTATTTAATGATAAAAAACACCCT"
                       "TGTTTCTACT"),
    };

}

TEST(FASTX_WRITE, FASTQ)
{
    ASSERT_EQ(0, system("mkdir -p tmp"));

    char tempfilepath[PATH_MAX];
    sprintf(tempfilepath, "tmp/fastq-writer-XXXXXX");
    ASSERT_NE(-1, mkstemp(tempfilepath));

    FastqWriter *writer = new FastqWriter;
    ASSERT_TRUE(writer->open(tempfilepath));
    for (size_t i = 0; i < sizeof(records)/sizeof(records[0]); i++) {
        ASSERT_TRUE(writer->write(records[i]));
    }
    delete writer;

    FastqReader *reader = new FastqReader;
    ASSERT_TRUE(reader->open(tempfilepath));
    for (size_t i = 0; i < sizeof(records)/sizeof(records[0]); i++) {
        SequenceRecord *record = reader->nextRecord();
        ASSERT_EQ(records[i], *record);
        delete record;
    };
    delete reader;
    
    unlink(tempfilepath);
    rmdir("tmp");
}


TEST(FASTX_WRITE, FASTA)
{
    ASSERT_EQ(0, system("mkdir -p tmp"));

    char tempfilepath[PATH_MAX];
    sprintf(tempfilepath, "tmp/fasta-writer-XXXXXX");
    ASSERT_NE(-1, mkstemp(tempfilepath));

    FastaWriter *writer = new FastaWriter;
    ASSERT_TRUE(writer->open(tempfilepath));
    for (size_t i = 0; i < sizeof(records2)/sizeof(records2[0]); i++) {
        ASSERT_TRUE(writer->write(records2[i]));
    }
    delete writer;

    FastaReader *reader = new FastaReader;
    ASSERT_TRUE(reader->open(tempfilepath));
    for (size_t i = 0; i < sizeof(records2)/sizeof(records2[0]); i++) {
        SequenceRecord *record = reader->nextRecord();
        ASSERT_EQ(records2[i], *record);
        delete record;
    };
    delete reader;
    
    unlink(tempfilepath);
    rmdir("tmp");
}
