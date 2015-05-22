/*
* File: fileHandler.h
* -----------
* Hanterar filer :)
*/


//Sparar en environment
void saveToFile(string fileName, environmentADT environment, int NBuckets);

//Laddar in definitios från fil
void loadFromFile(string fileName, int NBuckets);