#include "gtest/gtest.h"
#include "cppfasta2.hpp"

using namespace cppfasta;

void test_fastqEntry2(FastqReader2 *reader, const char *name, const char *seq, const char *quality) {
    SequenceRecord2 record;
    ASSERT_TRUE(reader->next(&record));
    ASSERT_STREQ(name, record.name);
    ASSERT_STREQ(seq, record.sequence);
    ASSERT_STREQ(quality, record.quality);
}

#define test_fastqEntry(reader, name, seq, quality) \
    do { \
        test_fastqEntry2(reader, name, seq, quality);\
        if (HasFatalFailure())\
            return;\
    } while(0);

TEST(FASTX_READER, FASTQ) {
    //system("gzip -dk ./data/DRR000035_head100.fastq.gz");
    FILE *file = fopen("./data/DRR000035_head100.fastq", "r");
    FastqReader2 reader(file);

    test_fastqEntry(&reader,
                    "DRR000035.1 20E09AAXX:1:1:107:276 length=36",
                    "TCACAATGTATACTCTCGAGATCCTAATTATCTGAA",
                    "IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII");
    test_fastqEntry(&reader,
                    "DRR000035.2 20E09AAXX:1:1:106:201 length=36",
                    "TATTTTTATTTAAGAGAATTTTTTAAAGCACTCTGG",
                    "IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII");
    test_fastqEntry(&reader,
                    "DRR000035.3 20E09AAXX:1:1:105:335 length=36",
                    "CTGAACTAAAGGCTGCTTAAGGAACTTGACTGAGAT",
                    "IIIIIIEIIIIIIIIIIIIIIIIII)II&IIIIIII");

    SequenceRecord2 record;
    for (int i = 0; i < 100/4-3; i++) {
        ASSERT_TRUE(reader.next(&record));
    }

    ASSERT_FALSE(reader.next(&record));
}

TEST(FASTX_READER, FASTQ2) {
    //system("gzip -dk ./data/DRR000035_head100.fastq.gz");
    FILE *file = fopen("./data/DRR000035_head100.fastq", "r");
    FILE *file2 = fopen("./data/DRR000035_101-200.fastq", "r");
    FastqReader2 reader(file, file2);

    test_fastqEntry(&reader,
                    "DRR000035.1 20E09AAXX:1:1:107:276 length=36",
                    "TCACAATGTATACTCTCGAGATCCTAATTATCTGAA",
                    "IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII");
    test_fastqEntry(&reader,
                    "DRR000035.27 20E09AAXX:1:1:88:355 length=36",
                    "CACAATTAAAGCTAAAATAATATAAAAATAATTCGA",
                    "IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII");
    test_fastqEntry(&reader,
                    "DRR000035.2 20E09AAXX:1:1:106:201 length=36",
                    "TATTTTTATTTAAGAGAATTTTTTAAAGCACTCTGG",
                    "IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII");

    SequenceRecord2 record;
    for (int i = 0; i < 200/4-3; i++) {
        ASSERT_TRUE(reader.next(&record));
    }

    ASSERT_FALSE(reader.next(&record));
}
