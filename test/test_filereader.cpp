#include "filereader.hpp"
#include "gtest/gtest.h"
#include <stdio.h>

using namespace cppfasta;

void common_test(BasicReader *reader) {
    char buf2[15];
    ASSERT_EQ(buf2, reader->fgets(buf2, sizeof(buf2)));
    ASSERT_STREQ("@DRR000035.1 2", buf2);
    
    char buf[1024];
    ASSERT_EQ(buf, reader->fgets(buf, sizeof(buf)));
    ASSERT_STREQ("0E09AAXX:1:1:107:276 length=36\n", buf);
    ASSERT_EQ(buf, reader->fgets(buf, sizeof(buf)));
    ASSERT_STREQ("TCACAATGTATACTCTCGAGATCCTAATTATCTGAA\n", buf);
    ASSERT_EQ(buf, reader->fgets(buf, sizeof(buf)));
    ASSERT_STREQ("+DRR000035.1 20E09AAXX:1:1:107:276 length=36\n", buf);
    ASSERT_EQ(buf, reader->fgets(buf, sizeof(buf)));
    ASSERT_STREQ("IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII\n", buf);

    for (int i = 0; i < 100-4; ++i) {
        ASSERT_EQ(buf, reader->fgets(buf, sizeof(buf)));
    }

    //ASSERT_EQ(NULL, reader->fgets(buf, sizeof(buf)));
}

void common_test2(BasicReader *reader) {
    char buf[201];
    for (int i = 0; i < 5; i++) {
        ASSERT_EQ(buf, reader->fgets(buf, sizeof(buf))) << "Loop " << i;
        ASSERT_STREQ("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", buf) << "Loop " << i;
        ASSERT_EQ(buf, reader->fgets(buf, sizeof(buf))) << "Loop " << i;
        ASSERT_STREQ("bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb", buf) << "Loop " << i;
        ASSERT_EQ(buf, reader->fgets(buf, sizeof(buf))) << "Loop " << i;
        ASSERT_STREQ("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc", buf) << "Loop " << i;
        ASSERT_EQ(buf, reader->fgets(buf, sizeof(buf))) << "Loop " << i;
        ASSERT_STREQ("dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd", buf) << "Loop " << i;
        ASSERT_EQ(buf, reader->fgets(buf, sizeof(buf))) << "Loop " << i;
        ASSERT_STREQ("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee", buf) << "Loop " << i;
        ASSERT_EQ(buf, reader->fgets(buf, sizeof(buf))) << "Loop " << i;
        ASSERT_STREQ("fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff\n", buf) << "Loop " << i;
    }

    ASSERT_EQ(NULL, reader->fgets(buf, sizeof(buf)));
}

TEST(FILEREADER, FILE) {
    FileReader reader;
    ASSERT_TRUE(reader.open("data/DRR000035_head100.fastq"));

    {
        SCOPED_TRACE("read data");
        common_test(&reader);
    }

    ASSERT_FALSE(reader.open("data/DRR000035_head100.fastq"));

    FileReader reader2;
    ASSERT_TRUE(reader2.open("data/linedata.txt"));
    common_test2(&reader2);
}

TEST(FILEREADER, GZIP) {
    GzipReader reader;
    ASSERT_TRUE(reader.open("data/DRR000035_head100.fastq.gz"));

    {
        SCOPED_TRACE("read data");
        common_test(&reader);
    }

    ASSERT_FALSE(reader.open("data/DRR000035_head100.fastq.gz"));

    GzipReader reader2;
    ASSERT_TRUE(reader2.open("data/linedata.txt.gz"));
    common_test2(&reader2);

}

TEST(FILEREADER, BZIP2) {
    Bzip2Reader reader;
    ASSERT_TRUE(reader.open("data/DRR000035_head100.fastq.bz2"));
    common_test(&reader);

    Bzip2Reader reader2;
    ASSERT_TRUE(reader2.open("data/linedata.txt.bz2"));
    common_test2(&reader2);

    ASSERT_FALSE(reader2.open("data/DRR000035_head100.fastq.gz"));
}

void common_fna(BasicReader *reader) {
    char buf[1024];
    ASSERT_EQ(buf, reader->fgets(buf, sizeof(buf)));
    ASSERT_STREQ(">NC_007373.1 Influenza A virus (A/New York/392/2004(H3N2)) segment 1, complete sequence\n", buf);
    ASSERT_EQ(buf, reader->fgets(buf, sizeof(buf)));
    ASSERT_STREQ("AGCAAAAGCAGGTCAATTATATTCAGTATGGAAAGAATAAAAGAACTACGGAACCTGATGTCGCAGTCTCGCACTCGCGA\n", buf);
    for (int i = 0; i < 181; i++) {
        ASSERT_EQ(buf, reader->fgets(buf, sizeof(buf)));
    }
    ASSERT_EQ(NULL, reader->fgets(buf, sizeof(buf))) << buf;
}

TEST(FILEREADER, OPENFILE1) {
    {
        SCOPED_TRACE("Normal File");
        BasicReader *reader = cppfasta::openFile("data/GCF_000865085.1_ViralMultiSegProj15622_genomic.fna");
        common_fna(reader);
        delete reader;
    }

    {
        SCOPED_TRACE("Gzip File");
        BasicReader *reader = cppfasta::openFile("data/GCF_000865085.1_ViralMultiSegProj15622_genomic.fna.gz");
        common_fna(reader);
        delete reader;
    }

    {
        SCOPED_TRACE("Bzip2 File");
        BasicReader *reader = cppfasta::openFile("data/GCF_000865085.1_ViralMultiSegProj15622_genomic.fna.bz2");
        common_fna(reader);
        delete reader;
    }
}

void common_readLine(BasicReader *reader)
{
    for (int i = 0; i < 5; i++) {
        std::string line = reader->readLine();
        ASSERT_STREQ("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeefffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff\n", line.c_str()) << "Error on line " << i;
    }

    ASSERT_STREQ("", reader->readLine().c_str());
}

TEST(FILEREADER, READLINE) {
    {
        SCOPED_TRACE("Normal File");
        BasicReader *reader = cppfasta::openFile("data/linedata.txt");
        common_readLine(reader);
        delete reader;
    }

    {
        SCOPED_TRACE("Gzip File");
        BasicReader *reader = cppfasta::openFile("data/linedata.txt.gz");
        common_readLine(reader);
        delete reader;
    }

    {
        SCOPED_TRACE("Bzip2 File");
        BasicReader *reader = cppfasta::openFile("data/linedata.txt.bz2");
        common_readLine(reader);
        delete reader;
    }
}


void common_fasta(BasicReader *reader)
{
    ASSERT_STREQ(">NC_007373.1 Influenza A virus (A/New York/392/2004(H3N2)) segment 1, complete sequence\n",
                 reader->readLine().c_str());
    ASSERT_STREQ("AGCAAAAGCAGGTCAATTATATTCAGTATGGAAAGAATAAAAGAACTACGGAACCTGATGTCGCAGTCTCGCACTCGCGA\n",
                 reader->readLine().c_str());
    ASSERT_STREQ("GATACTGACAAAAACCACAGTGGACCATATGGCCATAATTAAGAAGTACACATCGGGGAGACAGGAAAAGAACCCGTCAC\n",
                 reader->readLine().c_str());
    ASSERT_STREQ("TTAGGATGAAATGGATGATGGCAATGAAATACCCAATCACTGCTGACAAAAGGATAACAGAAATGGTTCCGGAGAGAAAT\n",
                 reader->readLine().c_str());
}

TEST(FILEREADER, READLINE2) {
    {
        SCOPED_TRACE("Normal File");
        BasicReader *reader = cppfasta::openFile("data/GCF_000865085.1_ViralMultiSegProj15622_genomic.fna");
        common_fasta(reader);
        delete reader;
    }

    {
        SCOPED_TRACE("Gzip File");
        BasicReader *reader = cppfasta::openFile("data/GCF_000865085.1_ViralMultiSegProj15622_genomic.fna.gz");
        common_fasta(reader);
        delete reader;
    }

    {
        SCOPED_TRACE("Bzip2 File");
        BasicReader *reader = cppfasta::openFile("data/GCF_000865085.1_ViralMultiSegProj15622_genomic.fna.bz2");
        common_fasta(reader);
        delete reader;
    }
}
