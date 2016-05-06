#include <iostream>
#include <fstream>
#include <string>


using namespace std;

#include "ReadGeneFile.hpp"

/*******************************************************************************
The constructor
*******************************************************************************/
GeneFileClass::GeneFileClass()
{
    geneDescription = "";
    currentGenePos = 1;
    bufferStartPos = 1;
    posInBuffer = 0;
    return;
}

/*******************************************************************************
Open the file
get the description
add bufferRows lines to the buffer
return true if no problem
The number of bufferRows loaded is set in the header file
*******************************************************************************/
bool GeneFileClass::openFile (string strFileName)
{
    string line;                    // the line as read from the file
    unsigned int lineCounter = 0;

    // using the this notation lets the intelesense work.
    this->geneDescription = "";
    this->currentGenePos = 1;
    this->bufferStartPos = 1;
    this->posInBuffer = 0;
    this->strBuffer = "";

    this->GeneFileName = strFileName;      // saves the name of the file so we can reopen the file if needed

    this->in.open(strFileName);
    if(!this->in.is_open())
    {
        cout << "The read file " << strFileName << " could not be opened";
        return false;
    }

    while(this->in.peek()!=EOF)
    {
        getline( this->in, line);
        lineCounter++;

        if (lineCounter == 1)
        {
            this->geneDescription = line;
        }
        if (lineCounter > 1)
        {
            this->strBuffer = this->strBuffer + line;
            this->bufferStopPos = this->strBuffer.size();
        }
        if (lineCounter == (this->bufferRows +1))
        {
            return true;
        }
    }
    return false;
}

/*******************************************************************************
closes the gene file
*******************************************************************************/
void GeneFileClass::closeFile()
{
    // TODO test end of file conditions
    this->in.clear();
    this->in.close();
}

/*******************************************************************************
returns the next char or null if end if file. Increments currentGenPos to the next char
*******************************************************************************/
char GeneFileClass::getChar()
{
    string line;
    // protect against indexing past the end of the buffer
    if (this->posInBuffer > this->strBuffer.size())
    {
        return '\0';
    }
    char c = strBuffer[posInBuffer];
    posInBuffer++;
    currentGenePos++;
    if (this->posInBuffer == this->strBuffer.size())
    {
        // the next char is the end of the buffer
        // get the next line and  append to buffer
        if (this->in.peek()!=EOF)
        {
            getline( this->in, line);
            this->strBuffer = this->strBuffer + line;
            // delete from the beginning of the buffer
            this->strBuffer.erase(0,(line.size()));
            // the first position in the buffer needs to be set
            this->bufferStartPos = this->bufferStartPos + line.size();
            // the last position in the buffer needs to be set
            this->bufferStopPos = this->bufferStopPos + line.size();
            // the read position in the buffer needs to be set
            this->posInBuffer = this->posInBuffer - line.size();
        }
        else
        {
            this->in.clear();
            this->in.close();
        }
    }
    return c;
}


/*******************************************************************************
If the position is "before" the buffer return false
If the position requested is in the buffer move to it; set currentGenePos and posInBuffer
If the position is still in the file advance so it is in the newest read part
    of the buffer, return true
    If you get to the end of the file before the requested position return false
*******************************************************************************/
bool GeneFileClass::moveTo(unsigned long newPosition)
{

    string line;

    // If the position is "before" the buffer
    // force the file to the beginning by close then open
    // set the position as if just opened
    // this should only happen if the buffer was not sized correctly
    if (newPosition < this->bufferStartPos)
    {
        GeneFileClass::closeFile();
        if (!GeneFileClass::openFile(GeneFileName)) return false;
    }

    // advance to a position outside the buffer
    // if you get to the end of the file first return false
    if (newPosition >= this->bufferStopPos)
    {
        // while not in the buffer
        // move to the end of the buffer
        // add another line from the file
        // adjust the pointers
        while (newPosition >= this->bufferStopPos)
        {
            this->currentGenePos = this->bufferStopPos + 1;
            // get the next line and append to buffer
            if (this->in.peek()!=EOF)
            {
                getline( in, line);
                this->strBuffer = this->strBuffer + line;
                // delete from the beginning of the buffer
                this->strBuffer.erase(0,(line.size()));
                // the first position in the buffer needs to be set
                this->bufferStartPos = this->bufferStartPos + line.size();
                // the last position in the buffer needs to be set
                this->bufferStopPos = this->bufferStopPos + line.size();
                // the read position in the buffer needs to be set
                this->posInBuffer = this->strBuffer.size() - line.size();
            }
            else
            {
                this->in.clear();
                this->in.close();
            }
        }
    }

    // If the position requested is in the buffer move to it; set currentGenePos and posInBuffer
    // It should always be in the buffer is you get here
    if (newPosition >= this->bufferStartPos && newPosition < this->bufferStopPos)
    {
        if (newPosition >= this->currentGenePos)
        {
            this->posInBuffer = this->posInBuffer + (newPosition - this->currentGenePos);
        }
        else
        {
            this->posInBuffer = this->posInBuffer - (this->currentGenePos - newPosition);
        }
        this->currentGenePos = newPosition;
        return true;
    }

    return false;
}

/*******************************************************************************
Used to test the class
The class was tested with a 1 line buffer, 2 line buffer and 10 line buffer
*******************************************************************************/
int GeneFileClass::TEST_ReadGeneFile_Main()
{
    char c = '\0';
    GeneFileClass *myGeneFile = new GeneFileClass();

    myGeneFile->openFile("NC_018080.fna");
    myGeneFile->moveTo(150);
    for (size_t i = 1; i <= 200; i++)
    {
        c=myGeneFile->getChar();
        cout << "i ->" << i << "\t" << (myGeneFile->currentGenePos -1) << "\t" << c << endl;
    }
    myGeneFile->moveTo(150);
    for (size_t i = 1; i <= 25; i++)
    {
        c=myGeneFile->getChar();
        cout << "i ->" << i << "\t" << (myGeneFile->currentGenePos -1) << "\t" << c << endl;
    }
    myGeneFile->moveTo(145);
    for (size_t i = 1; i <= 15; i++)
    {
        c=myGeneFile->getChar();
        cout << "i ->" << i << "\t" << (myGeneFile->currentGenePos -1) << "\t" << c << endl;
    }
    myGeneFile->moveTo(170);
    for (size_t i = 1; i <= 15; i++)
    {
        c=myGeneFile->getChar();
        cout << "i ->" << i << "\t" << (myGeneFile->currentGenePos -1) << "\t" << c << endl;
    }

    // Test end of file conditions
    // in the test file position 640620 is the last char in the 2nd to last line
    myGeneFile->moveTo(6402620);
    for (size_t i = 1; i <= 150; i++)
    {
        c=myGeneFile->getChar();
        cout << "i ->" << i << "\t" << (myGeneFile->currentGenePos -1) << "\t" << c << endl;
    }
    myGeneFile->closeFile();
    return 0;
}
