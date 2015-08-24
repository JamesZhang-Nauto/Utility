#include   "stdafx.h"
#include   "debug.h"
#include   <stdio.h>
#include   <cmath>

using namespace std;



double round(double d)
{
   return floor(d + 0.5);
}


float to_kph(float mm_per_sec)
{
   return(mm_per_sec/1000000*3600.f);
}
/*------------------------------------------------------------------------*/
//
//  For debug, log information into a log file on the usb stick
//
//
static   struct tm ts;
void Log(char* a )
{ 
   ofstream txt;
  
  
   txt.open (LOG_LOC, ios::app) ;
   txt <<  " " << a << endl;
   txt.close();
}
void Log(float a )
{
   ofstream txt;
   txt.open (LOG_LOC, ios::app) ;
   txt << a << endl;
   txt.close();
}

void Log(int text)
{
   ofstream txt;
   txt.open (LOG_LOC, ios::app) ;
   txt << text << endl;
   txt.close();
}
void Log(unsigned long text)
{
   ofstream txt;
   txt.open (LOG_LOC, ios::app) ;
   txt << text <<endl;
   txt.close();
}

#if 0 //high precision timing
LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds;
LARGE_INTEGER Frequency;

QueryPerformanceFrequency(&Frequency);
QueryPerformanceCounter(&StartingTime);

// Activity to be timed

QueryPerformanceCounter(&EndingTime);
ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
DBOUT("execution time: " << ElapsedMicroseconds.QuadPart * 1000000 / Frequency.QuadPart << " us" << endl);
#endif