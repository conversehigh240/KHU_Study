// Test driver
#include <iostream>
#include <fstream>

#include "QueType.h"

using namespace std;

template <class ItemType>
void ReplaceItem(QueType<ItemType> &queue, ItemType oldItem, ItemType newItem);

int main()
{

  ifstream inFile;       // file containing operations
  ofstream outFile;      // file containing output
  string inFileName;     // input file external name
  string outFileName;    // output file external name
  string outputLabel;     
  string command;        // operation to be executed
  
  int item;
  QueType<int> queue;
  int numCommands;


  // Prompt for file names, read file names, and prepare files
  cout << "Enter name of input command file; press return." << endl;
  cin  >> inFileName;
  inFile.open(inFileName.c_str());

  cout << "Enter name of output file; press return." << endl;
  cin  >> outFileName;
  outFile.open(outFileName.c_str());

  cout << "Enter name of test run; press return." << endl;
  cin  >> outputLabel;
  outFile << outputLabel << endl;

  inFile >> command;


  numCommands = 0;
  while (command != "Quit")
  { 
    try 
    {
        if (command == "Enqueue")
        {
            inFile >> item;
            queue.Enqueue(item);
            outFile << item << " is enqueued." << endl;
        }
        else if (command == "Dequeue")
        {
            queue.Dequeue(item);
            outFile << item << " is dequeued. " << endl;
        }
        else if (command == "IsEmpty")
            if (queue.IsEmpty())
                outFile << "Queue is empty." << endl;
            else
                outFile << "Queue is not empty." << endl;

        else if (command == "IsFull")
            if (queue.IsFull())
                outFile << "Queue is full." << endl;
            else outFile << "Queue is not full." << endl;

        else if (command == "ReplaceItem")
            queue.ReplaceItem(3, 8);

    }
    catch (FullQueue)
    {
      outFile << "FullQueue exception thrown." << endl;
    }
    
    catch (EmptyQueue)
    {
      outFile << "EmtpyQueue exception thrown." << endl;
    }    
    numCommands++;
    cout <<  " Command number " << numCommands << " completed." 
         << endl;
    inFile >> command;
 
  };
 
  cout << "Testing completed."  << endl;
  inFile.close();
  outFile.close();
  return 0;
}

template <class ItemType>
void ReplaceItem(QueType<ItemType>& queue, ItemType oldItem, ItemType newItem)
{
    QueType<ItemType> temp_que;
    ItemType temp_item;

    while (!queue.IsEmpty())
    {
        queue.Dequeue(temp_item);
        if (temp_item == oldItem)
            temp_que.Enqueue(newItem);
        else
            temp_que.Enqueue(temp_item);
    }

    while (!temp_que.IsEmpty())
    {
        temp_que.Dequeue(temp_item);
        queue.Enqueue(temp_item);
    }
}

