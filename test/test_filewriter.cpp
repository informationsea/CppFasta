#include "filewriter.hpp"
#include "filereader.hpp"
#include <stdlib.h>
#include <limits.h>
#include "gtest/gtest.h"

using namespace cppfasta;

void common_test(BasicWriter *writer, BasicReader *reader)
{
    ASSERT_EQ(0, system("mkdir -p tmp"));

    char tempfilepath[PATH_MAX];
    sprintf(tempfilepath, "tmp/normal-XXXXXX");
    ASSERT_NE(-1, mkstemp(tempfilepath));

    
    ASSERT_TRUE(writer->open(tempfilepath));
    writer->printf("Hello %s\n", "World");
    writer->printf("%04x\n", 0xa7);
    delete writer;

    ASSERT_TRUE(reader->open(tempfilepath));
    ASSERT_EQ(0, reader->readLine().compare("Hello World\n"));
    ASSERT_EQ(0, reader->readLine().compare("00a7\n"));

    unlink(tempfilepath);
    rmdir("tmp");
}

TEST(FILEWRITER, NORMAL) {
    FileWriter *fileWriter = new FileWriter;
    FileReader *fileReader = new FileReader;
    common_test(fileWriter, fileReader);
}

TEST(FILEWRITER, GZIP) {
    BasicWriter *writer = new GzipWriter;
    BasicReader *reader = new GzipReader;
    common_test(writer, reader);
}

TEST(FILEWRITER, BZIP2) {
    BasicWriter *writer = new Bzip2Writer;
    BasicReader *reader = new Bzip2Reader;
    common_test(writer, reader);
}

void common_test(const char *suffix)
{
    ASSERT_EQ(0, system("mkdir -p tmp"));

    char tempfilepath[PATH_MAX];
    sprintf(tempfilepath, "tmp/normal-XXXXXX%s", suffix);
    ASSERT_NE(-1, mkstemps(tempfilepath, strlen(suffix)));

    BasicWriter *writer = openFileForWrite(tempfilepath);
    writer->printf("Hello %s\n", "World");
    writer->printf("%04x\n", 0xa7);
    delete writer;

    BasicReader *reader = openFile(tempfilepath);
    ASSERT_EQ(0, reader->readLine().compare("Hello World\n"));
    ASSERT_EQ(0, reader->readLine().compare("00a7\n"));

    unlink(tempfilepath);
    rmdir("tmp");
}

TEST(FILEWRITER, ADAPTIVE) {
    common_test(".txt");
    common_test(".gz");
    common_test(".bz2");
}
