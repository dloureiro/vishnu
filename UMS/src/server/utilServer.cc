#include "utilServer.hh"
#include "UMSVishnuException.hh"


int convertToInt(std::string val) {
  int intValue;
  std::istringstream str(val); 
  str >> intValue;
  return static_cast<int> (intValue);
}

int checkId(int id) {
  if (id < 0) {
    UMSVishnuException e (4, "The id of the object is incorrect");
    throw e;
  }
  return id;
}
