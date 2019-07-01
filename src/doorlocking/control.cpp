#include "control.h"

String readFile(String fileName){
  File rFile = SPIFFS.open(fileName,"r");
  String sFile = rFile.readString();
  rFile.close();
  return sFile;
}
