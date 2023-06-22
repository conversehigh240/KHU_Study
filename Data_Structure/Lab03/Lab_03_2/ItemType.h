// ItemType.h  contains the specifications for the data for the real 
//  estate manipulation program.
const int MAX_ITEMS = 25; 
#include <fstream> 
#include "StrTypeAugmented.h"   // Gain access to String ADT.
enum RelationType {LESS, EQUAL, GREATER};
class HouseType
{ 
public:
  void GetFromFile(std::ifstream&); 
  void WriteToFile(std::ofstream&); 
  void GetFromUser(); 
  void PrintHouseToScreen(); 
  void GetNameFromUser(); 
  void PrintNameToScreen(); 
  bool operator<(const HouseType &other);
  bool operator==(const HouseType &other);
  RelationType ComparedTo(HouseType); 

private:
  StrType lastName; 
  StrType firstName; 
  StrType address; 
  float price; 
  int squareFeet; 
  int bedRooms; 
  int bathRooms;
  char letters[MAX_CHARS + 1];
}; 

// Make ItemType an alias for HouseType. 
typedef HouseType ItemType;
