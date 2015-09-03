#include "stdafx.h"
#include "File_operator.h"
#include "windows.h"
#include <tchar.h>

//save points into a file with a comma as the splitter.
void File_op_class::save_to_file(wstring &save_to_fname, vector<double> &x, vector<double> &y)
{
	ofstream txt;
	txt.open(save_to_fname);
	for (uint i = 0; i < x.size() && i < y.size(); i++)
		txt << x[i] << "," << y[i] << endl;
	txt.close();
}
//close the opened file.
void File_op_class::close()
{
	hfile.close();
}
// Save loaded points into argument vectors.
void File_op_class::save_to(vector<double> &x, vector<double> &y, const int &fft_size)
{
	if (!hfile)
		return;
	int temp_loc = 0;
	wchar_t temp_str[256];
	wstring temp_string = temp_str;
	size_t found;
	x.clear();
	y.clear();
	x.reserve(fft_size);
	y.reserve(fft_size);

	for (int i = 0; i < fft_size; i++)
	{
		if (hfile.eof() == FALSE || hfile.fail() == false)
		{
			hfile.getline(temp_str, 255);
			temp_string = temp_str;
			if (temp_string.size() > 0)
			{
				found = temp_string.find(L"\t");
				if (found == string::npos)
				{
					found = temp_string.find(L",");
					assert(found != string::npos);
				}

				x.push_back(stod(temp_string.substr(0, found + 1)));
				y.push_back(stod(temp_string.substr(found + 1)));
			}
		}
		else
		{
			hfile.close();
			break;
		}
	}
}
//check if the opened file is still valid
bool File_op_class::isvalid()
{
	return (hfile.is_open());
}

File_op_class::File_op_class(wstring _path)
{
	if (_path.size() > 0)
	{
		hfile.open(_path, ifstream::in);
		assert(hfile.is_open());
	}
}
//load all the y points in the file into vectors.
int File_op_class::load_file(wstring &path, vector<double> &y)
{
	DBOUT("Loading " << path <<endl);
	y.clear();
	y.reserve(600000);

	wifstream file(path, ifstream::in);
	int temp_loc = 0;
	wchar_t temp_str[256];
	wstring temp_string = temp_str;
	size_t found;
	while (file.eof() == FALSE || file.fail() == false)
	{
		file.getline(temp_str, 255);
		temp_string = temp_str;
		if (temp_string.size() > 0)
		{
			y.push_back(stod(temp_string));
		}
	}

	file.close();
	DBOUT("End of Loading " << path << endl);

	return 0;
}
//load all the x and y points in the file into vectors.
int File_op_class::load_file(wstring &path, vector<double> &x, vector<double> &y)
{
	DBOUT("Loading " << path << endl);

	x.clear();
	y.clear();
	x.reserve(600000);
	y.reserve(600000);
#if 1
	wifstream file(path, ifstream::in);
	int temp_loc = 0;
	wchar_t temp_str[256];
	wstring temp_string = temp_str;
	size_t found;
	while (file.eof() == FALSE || file.fail() == false)
	{
		file.getline(temp_str, 255);
		temp_string = temp_str;
		if (temp_string.size() > 0)
		{
			found = temp_string.find(L"\t");
			if (found == string::npos)
			{
				found = temp_string.find(L",");
				assert(found != string::npos);
			}

			x.push_back(stod(temp_string.substr(0, found + 1)));
			y.push_back(stod(temp_string.substr(found + 1)));
		}
	}

	file.close();
	DBOUT("End of Loading " << path << endl);

	return 0;
#else

#define BUFFSIZE 1024 // size of the memory to examine at any one time

#define FILE_MAP_START 138240 // starting point within the file of
	// the data to examine (135K)
	HANDLE hMapFile;      // handle for the file's memory-mapped region
	HANDLE hFile;         // the file handle
	BOOL bFlag;           // a result holder
	DWORD dBytesWritten;  // number of bytes written
	DWORD dwFileSize;     // temporary storage for file sizes
	DWORD dwFileMapSize;  // size of the file mapping
	DWORD dwMapViewSize;  // the size of the view
	DWORD dwFileMapStart; // where to start the file map view
	DWORD dwSysGran;      // system allocation granularity
	SYSTEM_INFO SysInfo;  // system information; used to get granularity
	LPVOID lpMapAddress;  // pointer to the base address of the
	// memory-mapped region
	char * pData;         // pointer to the data
	int i;                // loop counter
	int iData;            // on success contains the first int of data
	int iViewDelta;       // the offset into the view where the data
	//shows up

	// Create the test file. Open it "Create Always" to overwrite any
	// existing file. The data is re-created below
	hFile = CreateFile(path.c_str(),
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		_tprintf(TEXT("hFile is NULL\n"));
		_tprintf(TEXT("Target file is %s\n"),
			path.c_str());
		return 4;
	}

	// Get the system allocation granularity.
	GetSystemInfo(&SysInfo);
	dwSysGran = SysInfo.dwAllocationGranularity;

	// Now calculate a few variables. Calculate the file offsets as
	// 64-bit values, and then get the low-order 32 bits for the
	// function calls.

	// To calculate where to start the file mapping, round down the
	// offset of the data into the file to the nearest multiple of the
	// system allocation granularity.
	dwFileMapStart = (FILE_MAP_START / dwSysGran) * dwSysGran;
	_tprintf(TEXT("The file map view starts at %ld bytes into the file.\n"),
		dwFileMapStart);

	// Calculate the size of the file mapping view.
	dwMapViewSize = (FILE_MAP_START % dwSysGran) + BUFFSIZE;
	_tprintf(TEXT("The file map view is %ld bytes large.\n"),
		dwMapViewSize);

	// How large will the file mapping object be?
	dwFileMapSize = FILE_MAP_START + BUFFSIZE;
	_tprintf(TEXT("The file mapping object is %ld bytes large.\n"),
		dwFileMapSize);

	// The data of interest isn't at the beginning of the
	// view, so determine how far into the view to set the pointer.
	iViewDelta = FILE_MAP_START - dwFileMapStart;
	_tprintf(TEXT("The data is %d bytes into the view.\n"),
		iViewDelta);

	// Now write a file with data suitable for experimentation. This
	// provides unique int (4-byte) offsets in the file for easy visual
	// inspection. Note that this code does not check for storage
	// medium overflow or other errors, which production code should
	// do. Because an int is 4 bytes, the value at the pointer to the
	// data should be one quarter of the desired offset into the file

	// Verify that the correct file size was written.
	dwFileSize = GetFileSize(hFile, NULL);
	_tprintf(TEXT("hFile size: %10d\n"), dwFileSize);

	// Create a file mapping object for the file
	// Note that it is a good idea to ensure the file size is not zero
	hMapFile = CreateFileMapping(hFile,          // current file handle
		NULL,           // default security
		PAGE_READONLY, // read/write permission
		0,              // size of mapping object, high
		dwFileMapSize,  // size of mapping object, low
		NULL);          // name of mapping object

	if (hMapFile == NULL)
	{
		_tprintf(TEXT("hMapFile is NULL: last error: %d\n"), GetLastError());
		return (2);
	}

	// Map the view and test the results.

	lpMapAddress = MapViewOfFile(hMapFile,            // handle to
		// mapping object
		FILE_MAP_READ, // read/write
		0,                   // high-order 32
		// bits of file
		// offset
		dwFileMapStart,      // low-order 32
		// bits of file
		// offset
		dwMapViewSize);      // number of bytes
	// to map
	if (lpMapAddress == NULL)
	{
		_tprintf(TEXT("lpMapAddress is NULL: last error: %d\n"), GetLastError());
		return 3;
	}

	// Calculate the pointer to the data.
	pData = (char *)lpMapAddress + iViewDelta;

	while (pData != NULL)
	{
		// Extract the data, an int. Cast the pointer pData from a "pointer
		// to char" to a "pointer to int" to get the whole thing
		iData = *(float *)pData;
		pData += sizeof(float);
	}

	_tprintf(TEXT("The value at the pointer is %d,\nwhich %s one quarter of the desired file offset.\n"),
		iData,
		iData * 4 == FILE_MAP_START ? TEXT("is") : TEXT("is not"));

	// Close the file mapping object and the open file

	bFlag = UnmapViewOfFile(lpMapAddress);
	bFlag = CloseHandle(hMapFile); // close the file mapping object

	if (!bFlag)
	{
		_tprintf(TEXT("\nError %ld occurred closing the mapping object!"),
			GetLastError());
	}

	bFlag = CloseHandle(hFile);   // close the file itself

	if (!bFlag)
	{
		_tprintf(TEXT("\nError %ld occurred closing the file!"),
			GetLastError());
	}
#endif
}

#define PI 3.14159265358979323846
void File_op_class::generate_sinwave(const float sampling_freq, float freq, float amp, float number, vector<double> &y)
{
	float delta_t = 1 / sampling_freq;
	y.clear();
	y.resize(0);
	for (int i = 0; i < (int)number; ++i)
	{
		y.push_back
			(
			amp * sin(freq * 2 * PI * i / sampling_freq)
			);
	}

}