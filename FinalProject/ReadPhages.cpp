#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

#include "ReadPhages.hpp"

/*******************************************************************************
The constructor
*******************************************************************************/
ProPhages::ProPhages()
{
    // nothing needed yet
    return;
}

/*******************************************************************************
stringstream is used to parse the wanted pieces out of the line
make a string act like stream and does the conversion for you

The second parameter is optional.  If no parameter is passed a value of 3 is used.
The value represents the number of lines from the beginning of the file which will
be skipped before looking of line with start and end values to extract.
*******************************************************************************/
bool ProPhages::LoadPhageFile(string strFileName, unsigned int NunberOfLinesToSkip)
{
    string line;                    // the line as read from the file
    stringstream ssline;            // the line as a stringstream

    ProPhageLocation *myLoc;
    unsigned long startPos = 0l;    // temp variable to hold the start pos
    unsigned long stopPos = 0l;     // temp variable to hold the stop pos
    unsigned long length = 0l;
    unsigned int lineCounter = 0;   // line counter too allow the ignoring of non-data lines
    size_t found = 0;
    string strTmp;
    string proPhageDescr;

    ifstream in;
    in.open(strFileName);
    if(!in.is_open())
    {
        cout << "The read file " << strFileName << " could not be opened";
        return false;
    }

    while(in.peek()!=EOF)
    {
        getline( in, line);
        lineCounter++;

        /*  if the lineCount is greater NunberOfLinesToSkip and the line has .. then we want to parse it
         *  beginning of line to ".." is startpos
         *  after .. to tab is endpos
         *  tab to to end of line is description
         */
        found = line.find("..",0);                  // find returns ULONG_MAX if not found
        if (lineCounter > NunberOfLinesToSkip && found != ULONG_MAX)
        {
            //The debugger makes looking at vectors a pain so store in local var first then transfer
            ssline = stringstream(line);
            ssline >> startPos ;
            ssline.width(2);
            ssline >> strTmp;
            ssline >> stopPos;
            getline(ssline, proPhageDescr);
            length = stopPos - startPos +1 ;
            /*
            // for debug
            cout << "start ->" << startPos << endl;
            cout << "tmp   ->" << strTmp << endl;
            cout << "stop  ->" << stopPos << endl;
            cout << "descr ->" << proPhageDescr << endl;
            */

            // add new location and set values
            myLoc = new ProPhageLocation;
            myLoc->startPos = startPos;
            myLoc->stopPos = stopPos;
            myLoc->length = length;
            myLoc->proPhageDescr = proPhageDescr;
            this->myLocations.push_back(*myLoc);
        }
    }
    in.clear();
    in.close();

    // The file should be in ascending start order
    // if it is not a backward move out of the buffer will cause the file to
    // reset to the beginning and move to the desired spot.  This will slow things
    // down but it works
    return true;
}

/*******************************************************************************
Prints the vector to the screen for DEBUG and testing
*******************************************************************************/
bool ProPhages::DEBUG_printVector()
{
    for (size_t i = 0; i < this->myLocations.size(); i++)
    {
        cout << "Index ->" << i << "\t\t";
        cout << "Start ->" << this->myLocations[i].startPos << "\t\t";
        cout << "Stop  ->" << this->myLocations[i].stopPos << "\t\t";
        cout << "Length->" << this->myLocations[i].length << endl;
    }
    return true;
}

/*******************************************************************************
Reads trough the vector and determines the max over between consecutive phages
This can be used to determine the number of buffer lines for reading the gene
file.
*******************************************************************************/
unsigned int ProPhages::DEBUG_maxOverlap()
{
    unsigned int ltemp = 0;
    unsigned int maxltemp = 0;
    unsigned long startOverlap = 0;
    unsigned long stopOverlap = 0;

    for (size_t i = 0; (i < this->myLocations.size() -1); i++)
    {
        // ltemp = myLocations[i-1].stopPos - myLocations[i].startPos;
        ltemp = this->getOverlap( i, startOverlap, stopOverlap);
        if (ltemp > 0l)
        {
            if (ltemp > maxltemp)
            {
                maxltemp = ltemp;
            }
        }
    }
    return maxltemp;
}

/*******************************************************************************
Reads through the vector and checks that the entries are in ascending order

*******************************************************************************/
unsigned int ProPhages::CheckAssendingOrder()
{
    for (size_t i = 0; i < this->myLocations.size() -1; i++) // don't go past the limit
    {
        if (this->myLocations[i+1].startPos < this->myLocations[i].startPos)
        {
            cout << "block out of order at index ->" << i << endl;
            cout << "Start ->" << this->myLocations[i].startPos << "\t\t";
            cout << "Stop  ->" << this->myLocations[i].stopPos << endl;
            cout << "Start ->" << this->myLocations[i+1].startPos << "\t\t";
            cout << "Stop  ->" << this->myLocations[i+1].stopPos << endl;
            return i;
        }
    }
    return 0;
}

/*******************************************************************************
Gets the overlag between the current phage and the next

*******************************************************************************/
unsigned int ProPhages::getOverlap ( size_t i, unsigned long &startOverlap, unsigned long &stopOverlap)
{
    // if not last vector and overlap return data else return 0
    if (this->myLocations[i+1].startPos <= this->myLocations[i].stopPos && i < (this->myLocations.size() -1) )
    {
        startOverlap = this->myLocations[i+1].startPos;
        stopOverlap = this->myLocations[i].stopPos;
        return (this->myLocations[i].stopPos - this->myLocations[i+1].startPos) +1 ;
    }

    startOverlap = 0;
    stopOverlap = 0;
    return 0;
}

/*******************************************************************************
This was the original main used to test this class

*******************************************************************************/
int ProPhages::TEST_ReadPhages_Main()
{
    ProPhages *myPhages = new ProPhages();
    //myPositions->LoadPhageFile("Test01.txt", 3);
    myPhages->LoadPhageFile("NC_018080.ptt");
    myPhages->DEBUG_printVector();
    cout << "Max overlap ->" << myPhages->DEBUG_maxOverlap() << endl;
    //myPhages->CheckAssendingOrder();
    return 0;
}

