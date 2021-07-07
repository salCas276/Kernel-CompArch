#ifndef CLOCK_DRIVER_H
#define CLOCK_DRIVER_H

// Implementado en libasm.o
int getFromRTC(int value);
int getRtcStatus();
void setRtcBinary();

#endif