#include "PdbInfo.h"
#include "file_until.h"
#include "guid_until.h"
#include <LIEF/LIEF.hpp>

#ifdef _DEBUG
#pragma comment(lib, "../../LIEF/Debug/LIEF.lib")
#else
#pragma comment(lib, "../../LIEF/Debug/Release.lib")
#endif

using namespace LIEF;
VOID PdbInfo::findPdbInfo() {
	auto pe = PE::Parser::parse(path.c_str());

	auto debugArray = pe->debug();
	for (PE::Debug& debugInfo : debugArray) {
		if (debugInfo.has_code_view() ) {
			const PE::CodeViewPDB& cv = static_cast<const PE::CodeViewPDB&>(debugInfo.code_view());
			pdbName = cv.filename();
			_GUID* pGuid = (_GUID*)cv.signature().data();
			peSigned = guid_until::Instance(pGuid).toString();
			break;
		}
	}

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