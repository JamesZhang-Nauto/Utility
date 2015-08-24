#include "stdafx.h"
#include <iostream>

#include <cmath>
#include <fstream>
#include <sstream>
#include "assert.h"
#include "statistics.h"
#include <map>

namespace statistics
{
	/*-------------------------------------------------------------------------*/
	//
	//
	//   variance = sum[1-n][(xn - mean)^2]
	//
	float cal_variance(double *data, int size)
	{
		double sum = 0;
		for (int i = 0; i < size; i++)
		{
			sum += data[i];
		}
		double mean = sum / size;
		sum = 0;
		for (int i = 0; i < size; i++)
		{
			sum += (data[i] - mean)*(data[i] - mean);
		}
		float variance = (float)sum / size;
		return variance;
	}
	pair<double, double> find_peak(const vector<double> &data, uint low_bd, uint high_bd)
	{
		if (high_bd == 0)
			high_bd = data.size();

		//Please don't include dc component
		assert(low_bd > 0);

		pair<double, double> _max = make_pair(-1, -1);
		for (uint i = low_bd; i < high_bd; i++)
		{
			if (i == low_bd)
			{
				_max = make_pair(i, data[i]);
			}
			else
			{
				if (_max.second < data[i])
				{
					_max.first = i;
					_max.second = data[i];
				}
			}
		}
		return _max;
	}
	pair<double, double> find_neg_peak(const vector<double> &data, uint low_bd, uint high_bd)
	{
		if (high_bd == 0)
			high_bd = data.size();

		pair<double, double> _min = make_pair(-1, -1);
		for (uint i = low_bd; i < high_bd; i++)
		{
			if (i == low_bd)
			{
				_min = make_pair(i, data[i]);
			}
			else
			{
				if (_min.second > data[i])
				{
					_min.first = i;
					_min.second = data[i];
				}
			}
		}
		return _min;
	}
	float cal_avg_p2p(vector<double> &_x, vector<double> &_y, float thres)
	{
		assert(_x.size());
		assert(_y.size());

		float val_thres = thres;
		double avg_ampliltude_mm = 0, avg_wavelength_mm = 0;
		double *accl_data = &(_y[0]);

		pair<long, double> loc_min = make_pair(0, accl_data[0]);

		pair<long, double> loc_max = loc_min;
		map<long, double> maxpts;
		map<long, double> minpts;
		vector<double> wavelength;
		vector<double> amp;

		for (uint i = 0; i < _y.size(); i++)
		{
			//
			//   when the wave is in negative side, we find min
			//
			//
			if (accl_data[i] < 0)
			{
				uint cross_pt = i + 1;
				loc_min = make_pair(i, accl_data[i]);

				while (cross_pt < _y.size())
				{
					if (accl_data[cross_pt] >= 0)
						break;
					else
					{
						if (accl_data[cross_pt] <= loc_min.second)
						{
							loc_min.first = cross_pt;
							loc_min.second = accl_data[cross_pt];
						}

						cross_pt++;
					}
				};
				if (-1 * loc_min.second > val_thres)
					minpts.insert(loc_min);
				if (cross_pt - i > 1)
					i = cross_pt--;
			}
			else if (accl_data[i] > 0)
			{
				uint cross_pt = i + 1;
				loc_max = make_pair(i, accl_data[i]);

				while (cross_pt < _y.size())
				{
					if (accl_data[cross_pt] <= 0)
						break;
					else
					{
						if (accl_data[cross_pt] >= loc_max.second)
						{
							loc_max = make_pair(cross_pt, accl_data[cross_pt]);
						}

						cross_pt++;
					}
				};
				if (loc_max.second > val_thres)
					maxpts.insert(loc_max);
				if (cross_pt - i > 1)
					i = cross_pt--;
			}
		}
		// if no zero cross, report for now. Should do a dc offset remove.
		if (minpts.size() == 0 || maxpts.size() == 0)
		{
			DBOUT("no peaks are found...max = " << loc_max.second << "; min = " << loc_min.second << endl);
			return 0;
		}

		// calculate amplitude and wavelength

		for (map<long, double>::iterator maxit = maxpts.begin(), minit = minpts.begin();
			maxit != maxpts.end() && minit != minpts.end();
			++maxit, ++minit)
		{
			amp.push_back(maxit->second - minit->second);
			wavelength.push_back(abs(_x[maxit->first] - _x[minit->first]));
		}

		double sum_wl = 0;
		for (uint i = 0; i < wavelength.size(); i++)
			sum_wl += wavelength[i];

		double sum_amp = 0;
		for (uint i = 0; i < amp.size(); i++)
			sum_amp += amp[i];

		if (amp.size() > 0 && wavelength.size() > 0)
		{
			avg_ampliltude_mm = fabs(sum_amp / amp.size());
			avg_wavelength_mm = fabs(sum_wl / wavelength.size());
		}
		else
		{
			avg_ampliltude_mm = 0;
			avg_wavelength_mm = 0;
		}

		//
		//   deallocate memory
		//
		//
		vector<double>().swap(wavelength);
		vector<double>().swap(amp);

		return (float)avg_ampliltude_mm;
	}

	double cal_RMS(vector<double> &data)
	{
		assert(data.size());
		double ret = 0;
		for (uint i = 0; i < data.size(); i++)
		{
			ret += data[i] * data[i];
		}
		ret = ret / data.size();
		ret = sqrt(ret);
		DBOUT("RMS = " << ret << endl);

		return ret;
	}
}