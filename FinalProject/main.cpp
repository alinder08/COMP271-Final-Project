#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

#include "ReadPhages.hpp"
#include "ReadGeneFile.hpp"

int main()
{
    string strPhageFileName = "";
    string strOutputFileName = "";
    cout << "Enter Phage File Name (NC_018080.ptt)\t=\t";
    cin >> strPhageFileName;
    string strGeneFileName = "";

    cout << "Enter Gene File Name  (NC_018080.fna)\t=\t";
    cin >> strGeneFileName;

    cout << "Enter Output File Name(NC_018080.out)\t=\t";
    cin >> strOutputFileName;

    cout << endl;
    cout << "=========================================================" << endl;
    cout << "Your files are:" << endl;
    cout << "Phage File Name  =\t" << strPhageFileName << endl;
    cout << "Gene File Name   =\t" << strGeneFileName << endl;
    cout << "Output File Name =\t" << strOutputFileName << endl;
    cout << "=========================================================" << endl;
    cout << "Processing - Loading Phage File ..." << endl;
    cout << "=========================================================" << endl;
    ProPhages *myPhages = new ProPhages();
    if (!myPhages->LoadPhageFile(strPhageFileName)) return 1;
    //myPhages->DEBUG_printVector();

    cout << "Processing - Opening Gene File ..." << endl;
    cout << "=========================================================" << endl;
    GeneFileClass *myGeneFile = new GeneFileClass();
    if (!myGeneFile->openFile(strGeneFileName)) return 1;


    cout << "Processing - Writing Output File ..." << endl;
    cout << "=========================================================" << endl;
    ofstream out;
    out.open(strOutputFileName);
    // for each vector in the Phage file
        // write the first line of the gene file to the output + "|" + start + "|" + stop + "|" + sequencedata + endl;
    for ( size_t i = 0; i < myPhages->myLocations.size(); i++)
    {
        // output the first line
        out << myGeneFile->geneDescription << "|";
        out << myPhages->myLocations[i].startPos << "|";
        out << myPhages->myLocations[i].stopPos << "|";
        // position the gene file
        if (!myGeneFile->moveTo(myPhages->myLocations[i].startPos))
        {
            cout << "Error reading Gene file, exiting" << endl;
            return 1;
        }
        // write the sequence
        for ( size_t j = 0; j < myPhages->myLocations[i].length; j++ )
        {
            out << myGeneFile->getChar();
        }
        // end the line
        out << endl;
    }
    out.clear();
    out.close();

    // close the gene file
    myGeneFile->closeFile();
    cout << "Processing - Complete" << endl;
    cout << "=========================================================" << endl;

    return 0;
}
