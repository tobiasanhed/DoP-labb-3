/*
* File: cmdTable.h
* ------------
* Interface för commandTable
*/

#ifndef _cmdTable_h
#define _cmdTable_h

#include "env.h"

void ExecuteCommand(string cmd, environmentADT env);
void InitCommandTable(void);

#endif