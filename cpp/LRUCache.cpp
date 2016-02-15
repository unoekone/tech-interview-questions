// LRU Cache

#include <iostream>

#include <list>
#include <unordered_map>
#include <string>

namespace unoekone { namespace interview {

template <typename T>
class LRUCache {

public:
    LRUCache(unsigned int size) 
        : mCacheMaxSize(size) {
    }

    void add(std::string const& key, T const& val) {

        CacheIter_t it = mCache.find(key);
        
        if (it != mCache.end()) {

            mLruList.erase(it->second);
            mCache.erase(it);
        }

        if (mLruList.size() >= mCacheMaxSize) {
            LruListElem_t const& tail = mLruList.back();

            mCache.erase(tail.first);

            mLruList.pop_back();
        }

        mLruList.push_front(LruListElem_t(key, val));

        mCache.insert(std::make_pair(key, mLruList.begin()));
    }

    void get(std::string const& key, T &out) {

        CacheIter_t it = mCache.find(key);

        if (it != mCache.end()) {
            ListIter_t listIt = it->second;

            out = listIt->second;

            mLruList.splice(mLruList.begin(), mLruList, listIt);

            it->second = mLruList.begin();
        }
    }

    bool remove(std::string const& key) {
        bool removed = false;

        CacheIter_t it = mCache.find(key);

        if (it != mCache.end()) {
            ListIter_t listIt = it->second;

            mCache.erase(it);

            mLruList.erase(listIt);

            removed = true;
        }

        return removed;
    }

    void print() {

        for (auto const& e : mLruList) {
            std::cout << "key = " << e.first << " val = " << e.second << std::endl;
        }
    }

private:

    int mCacheMaxSize;

    typedef std::pair<std::string, T> LruListElem_t;
    typedef typename std::list<LruListElem_t>::iterator ListIter_t;
    typedef typename std::unordered_map<std::string, ListIter_t>::iterator CacheIter_t;
	
    std::list<LruListElem_t> mLruList;

    std::unordered_map<std::string, ListIter_t> mCache;
};

} }

int main(int argc, char **argv) {

    unoekone::interview::LRUCache<int> lruCache(5);

    lruCache.add("hello", 1);
    lruCache.add("jello", 2);
    lruCache.add("mello", 3);
    lruCache.print();

    int out = -1;

    lruCache.get("jello", out);
    std::cout << "jello = " << out << std::endl;
    lruCache.print();

    lruCache.get("hello", out);
    std::cout << "hello = " << out << std::endl;
    lruCache.print();

    lruCache.add("fello", 4);
    lruCache.print();

    std::cout << "remove(\"fello\") = " << lruCache.remove("fello") << std::endl;
    lruCache.print();

    return 0;
}
