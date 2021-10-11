#pragma once
#include <string>
#include <Windows.h>

using namespace std;
class PdbInfo{
public:
	PdbInfo(const char*);
	~PdbInfo();

	string getPdbName();
	string getSigned();
private:

	VOID findPdbInfo();
	string path;
	string pdbName;
	string peSigned;
};

