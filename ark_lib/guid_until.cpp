#include "guid_until.h"
#include <iostream>
#include <sstream>	// ostringstream ÀàĞÍ

guid_until::guid_until(_GUID* guid_) : guid({ 0 }) {
	if (guid_) {
		memcpy(&guid, guid_, sizeof(_GUID));
	}
}

guid_until::guid_until(_GUID guid_) {
	guid = guid_;
}

string guid_until::toString() {
    std::stringstream os;
    os.fill('0');
    os << std::uppercase;
    os.width(8);
    // os << std::hex << guid.Data1 << '-';
    os << std::hex << guid.Data1;

    os.width(4);
    // os << std::hex << guid.Data2 << '-';
    os << std::hex << guid.Data2;

    os.width(4);
    // os << std::hex << guid.Data3 << '-';
    os << std::hex << guid.Data3;

    /*
    os.width(2);
    os << std::hex
        << static_cast<short>(guid.Data4[0])
        << static_cast<short>(guid.Data4[1])
        << '-'
        << static_cast<short>(guid.Data4[2])
        << static_cast<short>(guid.Data4[3])
        << static_cast<short>(guid.Data4[4])
        << static_cast<short>(guid.Data4[5])
        << static_cast<short>(guid.Data4[6])
        << static_cast<short>(guid.Data4[7]);
        */
    for (auto i = 0; i < 8; i++) {
		os.width(2);
        os << std::hex
           << static_cast<short>(guid.Data4[i]);
    }
    os << std::nouppercase;

    return os.str();
}

guid_until* guid_until::New(_GUID* guid_) {
    return new guid_until(guid_);
}

guid_until guid_until::Instance(_GUID* guid_) {
    guid_until self(guid_);
    return self;
}