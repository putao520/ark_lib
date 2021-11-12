#include "PdbInfo.h"
#include "file_until.h"
#include "guid_until.h"
#include <pe-parse/parse.h>

typedef struct CV_INFO_PDB70 {
	DWORD			CvSignature;
	GUID			Signature;
	DWORD			Age;
	unsigned char   PdbFileName[1];
}CV_INFO_PDB70, * PCV_INFO_PDB70;
using namespace peparse;
VOID PdbInfo::findPdbInfo() {
	std::vector<uint8_t> DebugEntry;
	parsed_pe* pe = ParsePEFromFile(path.c_str());
	if (!pe)
		return;
	if (!GetDataDirectoryEntry(pe, DIR_DEBUG, DebugEntry))
		return;
	PIMAGE_DEBUG_DIRECTORY pDebugDir = (PIMAGE_DEBUG_DIRECTORY)DebugEntry.data();
	if (pDebugDir->Type != IMAGE_DEBUG_TYPE_CODEVIEW)
		return;
	PCV_INFO_PDB70 cv = (PCV_INFO_PDB70)(pe->fileBuffer->buf + pDebugDir->PointerToRawData);
	peSigned = guid_until::Instance((_GUID* )&cv->Signature).toString();
	peSigned = std::format("{}{}", peSigned, cv->Age);
}

PdbInfo::PdbInfo(const char* path_): path(path_){
	findPdbInfo();
}

PdbInfo::~PdbInfo(){
}

string PdbInfo::getPdbName() {
	return pdbName;
}
string PdbInfo::getSigned() {
	return peSigned;
}