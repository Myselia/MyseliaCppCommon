#include <cppcommon/Generic.h>

using namespace com::myselia::cppcommon;

int main(int argc, char** argv) {
	if(argc<2)
		return -1;

	string filename=argv[1];

	Sha1Hash hash=GenericUtil::getSha1Hash(filename);

	cout << "SHA1: " << GenericUtil::getHexString(hash, hash.getSizeInBytes()) << endl;
}
