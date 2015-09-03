#include   "stdafx.h"

#include <string>
#include <deque>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>


#ifdef _WIN32_WINNT
// Define the location for saving a logging file.
   #define LOG_LOC "C:\\log.txt"
#else _WIN32_WCE
   #define LOG_LOC "\\HardDisk2\\data\\cgn_files\\log.txt"
#endif

void Log(char*);
void Log(int);
void Log(unsigned long);
void Log(float);
void Log(std::vector<double> &a, const char* loc);
void Log(float a, const float &interval, std::vector<double> &b, const char* loc);
float to_kph(float mm_per_sec);
double round(double d);

#include <time.h>

#ifndef SYSOUT_F
#define SYSOUT_F(f, ...)      _RPT1( 0, f, __VA_ARGS__ ) // For Visual studio
#endif

#ifndef speedtest__             
#define speedtest__(data)   for (long blockTime = NULL; (blockTime == NULL ? (blockTime = clock()) != NULL : false); SYSOUT_F(data "%dms\n",  (clock() - blockTime) ))
#endif