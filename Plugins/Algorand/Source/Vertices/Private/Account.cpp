#include "Account.h"

#include <mutex>

Account::Account(std::string address)
	: address(algorand::vertices::Address(address)) {
}

Account::Account(algorand::vertices::Address address)
	: address(address) {
}

Account::Account(bytes public_key, bytes secret_key)
	: address(algorand::vertices::Address(public_key)), secret_key(secret_key) {
	assert(public_key.size() == crypto_sign_ed25519_PUBLICKEYBYTES);
	assert(secret_key.size() == crypto_sign_ed25519_SECRETKEYBYTES);
}

Account::Account(std::pair<bytes, bytes> key_pair) :
	Account(key_pair.first, key_pair.second) {
}

Account& Account::operator=(const Account& other)
{
	if (this != &other) // check for self-assignment
	{
		secret_key.assign(other.secret_key.begin(), other.secret_key.end());
		address = other.address;
	}
	return *this;
}
