#ifndef READGENE_HPP_INCLUDED
#define READGENE_HPP_INCLUDED


class GeneFileClass
{
public:
    string geneDescription;             // the first line of the gene file
    unsigned long currentGenePos = 1;   // the position of the next character to be returned by getChar()

    GeneFileClass();                    // constructor
    bool openFile (string strFileName); // opens the file, reads the first line into the geneDescription, loads the buffer
                                        // and initializes all the position values
    void closeFile();                   // closes the gene file
    char getChar();                     // returns the next char or null if end if file. Increments currentGenPos to the next char
    bool moveTo(unsigned long newPosition);
    int  TEST_ReadGeneFile_Main();      // Used to validate the class

private:
    ifstream in;
    string GeneFileName;                // saves the name of the file so we can reopen the file if needed

    // To keep from constantly rewinding the file by opening and closing it
    // we keep the last number of rows from the gene file in memory.
    // The number of 70 character rows to keep as a buffer is set here.
    unsigned int bufferRows = 10;       // 10 * 70 = 700
    string strBuffer;

    unsigned long bufferStartPos = 1;   // The Gene position of the character in strBuffer[0]
    unsigned long bufferStopPos = 1;    // The Gene position of the character in the last position of strBuffer
    unsigned long posInBuffer = 0;      // The position in the strbuffer of the currentGenePos
};
#endif // READGENE_HPP_INCLUDED
