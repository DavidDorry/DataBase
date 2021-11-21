#include <list>

#include "hash/extendible_hash.h"
#include "page/page.h"

namespace scudb {

/*
 * constructor
 * array_size: fixed array size for each bucket
 */
template <typename K, typename V>
ExtendibleHash<K, V>::ExtendibleHash(size_t size) {
    globalDepth = 0;
    bucketSize = size;
    bucketNum = 1;
    directories.push_back(make_shared<Bucket>(0));
}

/*
 * helper function to calculate the hashing address of input key
 */
template <typename K, typename V>
size_t ExtendibleHash<K, V>::HashKey(const K &key) {
    return hash<K>{}(key);
}

/*
 * helper function to return global depth of hash table
 * NOTE: you must implement this function in order to pass test
 */
template <typename K, typename V>
int ExtendibleHash<K, V>::GetGlobalDepth() const {
    lock_guard<mutex> lock(latch);
    return globalDepth;
}

/*
 * helper function to return local depth of one specific bucket
 * NOTE: you must implement this function in order to pass test
 */
template <typename K, typename V>
int ExtendibleHash<K, V>::GetLocalDepth(int bucket_id) const {
    lock_guard<mutex> lock(latch);
    return directories[bucket_id]->localDepth;
}

/*
 * helper function to return current number of bucket in hash table
 */
template <typename K, typename V>
int ExtendibleHash<K, V>::GetNumBuckets() const {
    lock_guard<mutex> lock(latch);
    return bucketNum;
}

/*
 * lookup function to find value associate with input key
 */
template <typename K, typename V>
bool ExtendibleHash<K, V>::Find(const K &key, V &value) {
    int idx = getIdx(key);
    lock_guard<mutex> lck(directories[idx]->latch);
    if(directories[idx]->kmap.find(key) != directories[idx]->kmap.end())
    {
        value = directories[idx]->kmap[key];
        return true;
    }
    return false;
}

/*
 * delete <key,value> entry in hash table
 * Shrink & Combination is not required for this project
 */
template <typename K, typename V>
bool ExtendibleHash<K, V>::Remove(const K &key) {
    int idx = getIdx(key);
    lock_guard<mutex> lck(directories[idx]->latch);
    shared_ptr<Bucket> cur = directories[idx];
    if(cur->kmap.find(key) == cur->kmap.end())
    {
        return false;
    }

    cur->kmap.erase(key);
    return true;
}

/*
 * insert <key,value> entry in hash table
 * Split & Redistribute bucket when there is overflow and if necessary increase
 * global depth
 */
template <typename K, typename V>
void ExtendibleHash<K, V>::Insert(const K &key, const V &value) {
    int idx = getIdx(key);
    shared_ptr<Bucket> cur = directories[idx];
    while(true)
    {
        lock_guard<mutex> lck(cur->latch);
        if(cur->kmap.find(key) != cur->kmap.end() || cur->kmap.size() < bucketSize)
        {
            cur->kmap[key] = value;
            break;
        }

        int mask = (int)pow(2, cur->localDepth);
        cur->localDepth++;

        lock_guard<mutex> lock(latch);
        if(cur->localDepth > globalDepth)
        {
            size_t length = directories.size();
            for(size_t i = 0; i < length; i++)
            {
                directories.push_back(directories[i]);
            }
            globalDepth++;
        }
        bucketNum++;
        auto newBuc = make_shared<Bucket>(cur->localDepth);

        typename map<K, V>::iterator iter;
        for(iter = cur->kmap.begin(); iter != cur->kmap.end();)
        {
            if(HashKey(iter->first) & mask)
            {
                newBuc->kmap[iter->first] = iter->second;
                iter = cur->kmap.erase(iter);
            }
            else
            {
                iter++;
            }
        }

        for(size_t i = 0; i < directories.size(); i++)
        {
            if(directories[i] == cur && (i & mask))
            {
                directories[i] = newBuc;
            }
        }
        idx = getIdx(key);
        cur = directories[idx];
    }
}


template<typename K, typename V>
int ExtendibleHash<K, V>::getIdx(const K &key) {
    return HashKey(key) & ((int)pow(2, globalDepth) - 1);
}

template class ExtendibleHash<page_id_t, Page *>;
template class ExtendibleHash<Page *, std::list<Page *>::iterator>;
// test purpose
template class ExtendibleHash<int, std::string>;
template class ExtendibleHash<int, std::list<int>::iterator>;
template class ExtendibleHash<int, int>;
} // namespace scudb
