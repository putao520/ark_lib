#include <filesystem>
#include <fstream>
#include "file_until.h"
#include "memory_until.h"

using namespace std;
using namespace std::filesystem;
const size_t MAX_BUFFER = 0x10000;
FileUntil::FileUntil(const char* file) {
	init(file);
}

FileUntil::FileUntil(const std::string& file) {
	init(file.c_str());
}

void FileUntil::init(const char* file) {
	path _path(file);
	if (!_path.is_absolute()) {
		try {
			_path = canonical(_path);
		}
		catch (...) {
		}
	}

	this_path = _path;
}

std::string FileUntil::toAbsolute(const char* file) {
	path _path(file);
	if (!_path.is_absolute()) {
		try {
			_path = canonical(_path);
		}
		catch (...) {

		}
	}
	return _path.string();
}

bool FileUntil::existing() {
	return exists(this_path);
}

void* FileUntil::toMemory() {
	if (!exists(this_path))
		return nullptr;

	ifstream fileIo;
	fileIo.open(this_path, ios::binary);
    if (!fileIo.is_open())
        return nullptr;

	filebuf* pbuf;
	pbuf = fileIo.rdbuf();
    size_t size = static_cast<long>(pbuf->pubseekoff(0, ios::end, ios::in));
	pbuf->pubseekpos(0, ios::in);

	buffer = new char[size + 1];

	memset(buffer, 0, size + 1);

	pbuf->sgetn(buffer, size);
	
	fileIo.close();

	return buffer;
}

void FileUntil::write(const char* data, size_t length) {
	ofstream file(this_path, ios::binary| ios::trunc);
	if (!file.is_open())
		return;
	if (length == 0)
		length = strlen(data);
	size_t len = MAX_BUFFER;
	size_t end = memory_until::ngx_align(MAX_BUFFER, length);
	for (size_t postion = 0; postion < end; postion+=MAX_BUFFER) {
		if ((postion + MAX_BUFFER) > length)
			len = length % MAX_BUFFER;
		file.write(&data[postion], len);
		file.flush();
	}
	file.close();
	return;
}

void FileUntil::writeString(const std::string& data) {
	ofstream file(this_path, ios::out | ios::trunc);
	if (!file.is_open())
		return;
	file << data;
	file.close();
	return;
}