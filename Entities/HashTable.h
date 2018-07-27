#pragma once

// hash table structure and functions adapted from outside source

#include <math.h>
#include <iostream>
#include <list>
#include "TypeDefines.h"
using namespace std;

namespace Hash
{
	typedef unsigned char    uint8;
	typedef uint32(*HashFunction)(const uint8* key, const uint32 keyLen);

	uint32 AddUpHash(const uint8* key, const uint32 keyLen);			// basic barebones hashing
	uint32 JenkSinglesHash(const uint8* key, const uint32 keyLen);	// jenkins one-at-a-time hashing


	template <class KeyType, class ValueType>
	class cHashTable
	{
	public:
		cHashTable(
			const uint32 initialSize,
			HashFunction hashingFuncPtr,
			const float32 maxLoad = 0.7f
		) : m_MaxLoad(maxLoad), m_HashingFuncPtr(hashingFuncPtr)
		{	
			m_Size = initialSize;
			m_Buckets = new sBucket[m_Size];

			m_NumEntries = 0;
		}

		~cHashTable()
		{
			delete[] m_Buckets;
		}

		// interface
		bool LookUpKey(const KeyType& key, ValueType* value)
		{
			uint32 bucket = FindBucket(key);	// get index of bucket of key

			// 
			pairIter thisKeyValuePair = FindKeyValuePair(bucket, key);

			// if not found
			if(thisKeyValuePair == m_Buckets[bucket].end())
			{
				return false;
			}

			*value = thisKeyValuePair->value;
			return true;
		}

		void SetKeyValue(const KeyType& key, const ValueType& value)
		{
			uint32 bucket = FindBucket(key);	// get index of bucket of key

												// 
			pairIter thisKeyValuePair = FindKeyValuePair(bucket, key);

			if (thisKeyValuePair != m_Buckets[bucket].end())
			{
				thisKeyValuePair->value = value;
			}
			else
			{
				// check if table is full, resize if necessary
				if (m_NumEntries > m_Size * m_MaxLoad)
				{
					Resize(m_Size * 2);
					bucket = FindBucket(key);
				}
			}

			KeyValuePair newPair;
			newPair.key = key;
			newPair.value = value;

			m_Buckets[bucket].push_back(newPair);

			++m_NumEntries;
		}

		// return false if key not found
		bool RemoveKey(const KeyType& key)
		{
			uint32 bucket = FindBucket(key);	// get index of bucket of key

												// 
			pairIter thisKeyValuePair = FindKeyValuePair(bucket, key);

			if (thisKeyValuePair == m_Buckets[bucket].end())
			{
				return false;
			}

			m_Buckets[bucket].erase(thisKeyValuePair);

			--m_NumEntries;
			return true;
		}

		void RemoveAllEntries()
		{
			for (uint32 bucket = 0; bucket < m_Size; ++bucket)
			{
				m_Buckets[bucket].clear();
			}
		}

	private:
		// prevent copy and assignment
		cHashTable(const cHashTable&);
		cHashTable& operator=(const cHashTable&);

		// types
		struct KeyValuePair
		{
			KeyType key;
			ValueType value;
		};

		typedef list<KeyValuePair> sBucket;
		typedef typename sBucket::iterator pairIter;

		// internal support functions

		// find index of bucket containing given key
		uint32 FindBucket(const KeyType& key) const
		{
			const uint8* keyData = reinterpret_cast<const uint8*>(&key);

			uint32 index = m_HashingFuncPtr(keyData, sizeof(KeyType));

			index %= m_Size;

			return index;
		}

		pairIter FindKeyValuePair(const int bucket, const KeyType& key) const
		{
			pairIter thisKeyValuePair = m_Buckets[bucket].begin();

			// loop through until matching key is found
			while (thisKeyValuePair != m_Buckets[bucket].end())
			{
				if (key == thisKeyValuePair->key)
				{
					break;
				}
				++thisKeyValuePair;
			}

			return thisKeyValuePair;
		}

		void Resize(const uint32 newSize)
		{
			// store old values
			uint32 oldSize = m_Size;
			sBucket* oldBuckets = m_Buckets;

			// new buckets array
			m_Size = newSize;
			m_Buckets = new sBucket[m_Size];

			// insert old buckets into new
			m_NumEntries = 0;

			for (uint32 bucket = 0; bucket < oldSize; ++bucket)
			{
				while (oldBuckets[bucket].size())
				{
					SetKeyValue(oldBuckets[bucket].front().key, oldBuckets[bucket].front().value);
					oldBuckets[bucket].pop_front();
				}
			}

			delete[] oldBuckets;
		}


		// data
		sBucket* m_Buckets;
		uint32 m_Size;
		uint32 m_NumEntries;

		const HashFunction m_HashingFuncPtr;
		const float32 m_MaxLoad;

	};

}