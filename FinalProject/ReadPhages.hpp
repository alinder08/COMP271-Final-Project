#ifndef READPHAGES_HPP_INCLUDED
#define READPHAGES_HPP_INCLUDED

/*******************************************************************************
Class to store the prophage start and stop positions and description.
This will be read them from a file and store then in a vector for later processing
*******************************************************************************/
class ProPhageLocation
{
public:
    unsigned long startPos = 0l;    // the startPosition from the line
    unsigned long stopPos = 0l;     // the stopPosition from the line
    unsigned long length = 0l;      // the number of characters to extract
    string proPhageDescr = "";      // the rest of the line if we ever want to expand this class later
};

class ProPhages
{
public:
    vector<ProPhageLocation> myLocations;   // Vector to hold the extraction data

    ProPhages();                            // Constructor

    // Read the file and add the entries to the vector
    // The parser for the file uses "::" to determine if a line is a data line
    // NumberOfLinesToSkip is the number of lines the parser will ignore before it starts looking for data
    // It defaults to 3 but can easily be overridden by the calling function
    bool LoadPhageFile(string strFileName, unsigned int NunberOfLinesToSkip = {3});

    // Used for development and debug
    bool DEBUG_printVector();
    unsigned int DEBUG_maxOverlap();
    int TEST_ReadPhages_Main();

private:
    unsigned int getOverlap ( size_t i, unsigned long &startOverlap, unsigned long &stopOverlap);
    unsigned int CheckAssendingOrder();
};


#endif // READPHAGES_HPP_INCLUDED
