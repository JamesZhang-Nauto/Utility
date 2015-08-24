#ifndef FILE_OPEARTOR_H
#define FILE_OPEARTOR_H
#include   "stdafx.h"

class File_op_class
{
private:
	wifstream hfile;
public:
	File_op_class(wstring _path);
	int load_file(wstring &path, vector<double> &x, vector<double> &y);
	void save_file(wstring &save_to_fname, vector<double> &x, vector<double> &y);
	void save_to(vector<double> &x, vector<double> &y, const int &fft_size);
	bool isvalid();
};

#endif