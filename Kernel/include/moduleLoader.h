#ifndef MODULELOADER_H
#define MODULELOADER_H

void loadModules(void * payloadStart, void ** moduleTargetAddress);
long backupRSP();
void resetRSP(long value);

#endif