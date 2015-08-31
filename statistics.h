#pragma once

#include   "stdafx.h"
#include <vector>
using namespace std;
#include "windows.h"
#define DBOUT( s )            \
{                             \
   std::wostringstream os_;    \
   os_ << s;                   \
   OutputDebugStringW( os_.str().c_str() );  \
}
namespace statistics
{
	float cal_variance(const double *data, int size);
	float cal_avg_p2p(vector<double> &_x, vector<double> &_y, float thres );
	double cal_RMS(vector<double> &data);
	pair<double, double> find_peak(const vector<double> &data, 
		unsigned int low_bd =1, unsigned int high_bd = 0);
	pair<double, double> find_neg_peak(const vector<double> &data,
		unsigned int low_bd = 0, unsigned int high_bd = 0);

}