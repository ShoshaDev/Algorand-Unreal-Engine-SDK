#pragma once

#include "Address.h"
#include "../Private/mnemonic.h"
#include <cassert>
#include <cstring>

#include "../Private/include/sodium.h"

using namespace std;

typedef std::vector<unsigned char> bytes;


class Account {
public:
	Account() {}
	Account& operator=(const Account& other);
	Account(std::string address);
	Account(algorand::vertices::Address address);
	Account(bytes public_key, bytes secret_key);
	Account(std::pair<bytes, bytes> key_pair);

	const bytes public_key() const { return address.public_key; }
	algorand::vertices::Address address;
	bytes secret_key;       // empty() if created from an address, not key
	
};
