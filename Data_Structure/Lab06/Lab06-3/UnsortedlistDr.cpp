// Test driver
/*#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <cstring>

#include "UnsortedType-1.h"

using namespace std;
void PrintList(ofstream& outFile, UnsortedType<int>& list);
template <class ItemType>
void MergeLists(UnsortedType<ItemType>& l_a, UnsortedType<ItemType>& l_b, UnsortedType<ItemType>& result);

int main()
{

    ifstream inFile;       // file containing operations
    ofstream outFile;      // file containing output
    string inFileName;     // input file external name
    string outFileName;    // output file external name
    string outputLabel;
    string command;        // operation to be executed

    int item;
    UnsortedType<int> list;
    UnsortedType<int> temp_list;
    UnsortedType<int> result;
    bool found;
    int numCommands;

    int elem[6] = { 4, 2, 6, 10, 8, 12 };
    for (int i : elem)
        temp_list.InsertItem(i);

    // Prompt for file names, read file names, and prepare files
    cout << "Enter name of input command file; press return." << endl;
    cin >> inFileName;
    inFile.open(inFileName.c_str());

    cout << "Enter name of output file; press return." << endl;
    cin >> outFileName;
    outFile.open(outFileName.c_str());

    cout << "Enter name of test run; press return." << endl;
    cin >> outputLabel;
    outFile << outputLabel << endl;

    inFile >> command;

    numCommands = 0;
    while (command != "Quit")
    {
        if (command == "InsertItem")
        {
            inFile >> item;
            list.InsertItem(item);
            outFile << item;
            outFile << " is inserted" << endl;
        }
        else if (command == "DeleteItem")
        {
            inFile >> item;
            list.DeleteItem(item);
            outFile << item;
            outFile << " is deleted" << endl;
        }
        else if (command == "RetrieveItem")
        {
            inFile >> item;

            list.RetrieveItem(item, found);
            if (found)
                outFile << item << " found in list." << endl;
            else outFile << item << " not in list." << endl;
        }
        else if (command == "LengthIs")
            outFile << "Length is " << list.LengthIs() << endl;

        else if (command == "IsFull")
            if (list.IsFull())
                outFile << "List is full." << endl;
            else outFile << "List is not full." << endl;

        else if (command == "MergeLists")
        {
            MergeLists(list, temp_list, result);
            PrintList(outFile, result);
        }

        else PrintList(outFile, list);
        numCommands++;
        cout << " Command " << command << " completed."
            << endl;
        inFile >> command;
    }

    cout << "Testing completed." << endl;
    inFile.close();
    outFile.close();
    return 0;
}


void PrintList(ofstream& dataFile, UnsortedType<int>& list)
// Pre:  list has been initialized.      
//       dataFile is open for writing.   
// Post: Each component in list has been written to dataFile.
//       dataFile is still open.         
{
    int length;
    int item;
    list.ResetList();
    length = list.LengthIs();
    dataFile << "List Items: " << endl;
    for (int counter = 1; counter <= length; counter++)
    {
        list.GetNextItem(item);
        dataFile << item << endl;
    }
    dataFile << endl;
}*/
