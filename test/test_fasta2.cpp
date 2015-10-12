#include "gtest/gtest.h"
#include "cppfasta2.hpp"

using namespace cppfasta;

void test_fastqEntry(FastqReader2 *reader, const char *name, const char *seq, const char* quality) {
    SequenceRecord2 record;
    ASSERT_TRUE(reader->next(&record));
    ASSERT_STREQ(name, record.name);
    ASSERT_STREQ(seq, record.sequence);
    ASSERT_STREQ(quality, record.quality);
}

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
