#include "HashTable.h"

namespace Hash
{
	// basic barebones hashing
	uint32 AddUpHash(const uint8* key, const uint32 keyLen)
	{
		uint32 hash = 0;
		uint32 keyIndex;

		for (keyIndex = 0; keyIndex < keyLen; ++keyIndex)
		{
			hash += key[keyIndex];
		}

		return hash;
	}

	// jenkins one-at-a-time hashing
	uint32 JenkSinglesHash(const uint8* key, const uint32 keyLen)
	{
		uint32 hash = 0;
		uint32 keyIndex;

		for (keyIndex = 0; keyIndex < keyLen; ++keyIndex)
		{
			hash += key[keyIndex];
			hash += (hash << 10);
			hash ^= (hash >> 6);
		}
		hash += (hash << 3);
		hash ^= (hash >> 11);
		hash += (hash << 15);
		return hash;
	}
}