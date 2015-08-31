#ifndef FILE_OPEARTOR_H
#define FILE_OPEARTOR_H
#include   "stdafx.h"

class File_op_class
{
private:
	wifstream hfile;
public:
	File_op_class(wstring _path);
	static int  load_file(wstring &path, vector<double> &x, vector<double> &y);
	static int  load_file(wstring &path, vector<double> &y);
	static void generate_sinwave(const float sampling_freq, float freq, float amp, float number, vector<double> &y);
	void save_to_file(wstring &save_to_fname, vector<double> &x, vector<double> &y);
	void close();
	void save_to(vector<double> &x, vector<double> &y, const int &fft_size);
	bool isvalid();
};

#endif