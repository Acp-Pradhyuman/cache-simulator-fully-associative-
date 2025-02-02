#include <iostream>
#include <vector>
#include <climits>

class CacheBlock {
public:
    bool valid;
    bool dirty;
    int tag;
    int lastAccessTime;
    std::vector<int> data;

    CacheBlock(int blockSize) {
        valid = false;
        dirty = false;
        tag = -1;
        lastAccessTime = 0;
        data.resize(blockSize);
    }
};

class Cache {
private:
    std::vector<CacheBlock> cache;
    int numBlocks;
    int blockSize;
    int currentTime;
    int cacheMisses;
    int readMisses;
    int writeMisses;
    int cacheSearches;

public:
    Cache(int numBlocks, int blockSize) : numBlocks(numBlocks), blockSize(blockSize), currentTime(0),
        cacheMisses(0), readMisses(0), writeMisses(0), cacheSearches(0) {
        cache.resize(numBlocks, CacheBlock(blockSize));
    }

    void access(int memoryAddress, bool write) {
        currentTime++;
        cacheSearches++;

        // one word is 32bits (4bytes) hence in one access we get 4byte data
        int blockOffsetBits = 4;  // Since block size is 64 bytes (16 words), 4bits are used for block offset
        // int tagBits = 12;      // The rest of the bits are used for the tag (16 - 4 = 12 bits for the tag)

        // Extract the tag from the memory address
        int tag = memoryAddress >> blockOffsetBits;  // Shift the memory address to get the tag (ignore the block offset bits)

        // for fully asscociative
        // tag, bytes
        // here block size = 64bytes hence bytes = 6bits
        // tag = 16 - 

        // int blockAddress = memoryAddress / blockSize;
        int blockIndex = -1;
        bool hit = false;

        // Check if the block is in cache
        // In full Associative search time is O(N)
        // where N is the capacity of blocks in cache
        for (int i = 0; i < numBlocks; ++i) {
            // if (cache[i].valid && cache[i].tag == blockAddress) {
            if (cache[i].valid && cache[i].tag == tag) {
                blockIndex = i;
                hit = true;
                cache[i].lastAccessTime = currentTime;
                break;
            }
        }

        if (!hit) {
            cacheMisses++;
            if (write) {
                writeMisses++;  // Increment writeMisses only if it's a write miss
            } else {
                readMisses++;
            }

            // Find the block to replace using LRU policy
            int lruIndex = -1;
            int minTime = currentTime;

            // LRU cache replacement policy
            for (int i = 0; i < numBlocks; ++i) {
                if (!cache[i].valid || cache[i].lastAccessTime < minTime) {
                    lruIndex = i;
                    minTime = cache[i].lastAccessTime;
                }
            }

            blockIndex = lruIndex;

            cache[blockIndex].valid = true;
            cache[blockIndex].tag = tag;
            // for (int j = 0; j < blockSize; ++j) {
            //     cache[blockIndex].data[j] = memoryAddress + j;
            // }

            if (write) {
                cache[blockIndex].dirty = true;  // Mark as dirty if it's a write
            }

            cache[blockIndex].lastAccessTime = currentTime;
        } else {
            if (write) {
                cache[blockIndex].dirty = true;  // Mark as dirty on hit if it's a write
            }
        }
    }

    void printStats() {
        std::cout << "Cache Misses: " << cacheMisses << std::endl;
        std::cout << "Cache Searches: " << cacheSearches << std::endl;
        std::cout << "Cache Hit Rate: " << (1.0 - (double)cacheMisses / cacheSearches) * 100 << "%" << std::endl;
        std::cout << "Read Misses: " << readMisses << std::endl;
        std::cout << "Write Misses: " << writeMisses << std::endl;
    }

    void printCacheState() {
        for (int i = 0; i < numBlocks; ++i) {
            std::cout << "Block " << i << ": tag=" << cache[i].tag 
                      << ", valid=" << cache[i].valid 
                      << ", dirty=" << cache[i].dirty 
                      << ", lastAccessTime=" << cache[i].lastAccessTime << std::endl;
        }
    }
};

int main() {
    // Consider Main Memory of 64K words, and Cache size is 2K Words;
    // Consider Block Size is 16 Words; Word Size is 32 Bit; 
    // Processor Address is 16-bits
    // Implement Fully Associate Cache Mapping Scheme for Spatial access 
    // program/data and temporal access program/data Estimate the Cache Misses 
    // and Number of searches in Cache
    int numBlocks = 128;
    int blockSize = 16;

    Cache cache(numBlocks, blockSize);

    // std::cout<<std::endl<<"Before : "<<std::endl;
    // cache.printCacheState();
    // Simulating spatial access patterns (read and write)
    std::cout << "Simulating Spatial Access - Read:" << std::endl;
    for (int i = 0; i < 1000; i++) {
        cache.access(i, false);  // Read access
    }
    // std::cout<<std::endl<<"After : "<<std::endl;
    // cache.printCacheState();

    // std::cout<<std::endl<<"Before : "<<std::endl;
    // cache.printCacheState();
    cache.printStats();
    // std::cout<<std::endl<<"After : "<<std::endl;
    // cache.printCacheState();

    std::cout << "Simulating Spatial Access - Write:" << std::endl;
    for (int i = 0; i < 2000; i++) {
        cache.access(i, true);  // Write access
    }

    // std::cout<<std::endl<<"Before : "<<std::endl;
    // cache.printCacheState();
    cache.printStats();
    // std::cout<<std::endl<<"After : "<<std::endl;
    // cache.printCacheState();

    std::cout << "Simulating Temporal Access - Read:" << std::endl;
    // Access a subset of memory addresses multiple times to simulate temporal locality
    for (int i = 0; i < 1000; i++) {
        cache.access(i, false);  // Read access to addresses 0 to 999
    }

    for (int i = 0; i < 1000; i++) {
        cache.access(i, false);  // Read access again to the same addresses
    }

    for (int i = 1000; i < 2000; i++) {
        cache.access(i, false);  // Read access to addresses 1000 to 1999
    }

    for (int i = 1000; i < 2000; i++) {
        cache.access(i, false);  // Read access again to the same addresses
    }

    // std::cout<<std::endl<<"Before : "<<std::endl;
    // cache.printCacheState();
    cache.printStats();
    // std::cout<<std::endl<<"After : "<<std::endl;
    // cache.printCacheState();

    std::cout << "Simulating Temporal Access - Write:" << std::endl;
    for (int i = 0; i < 4000; i++) {
        cache.access(i, true);  // Write access
    }

    for (int i = 0; i < 4000; i++) {
        cache.access(i, true);  // Write access again to the same addresses
    }

    for (int i = 1000; i < 2000; i++) {
        cache.access(i, true);  // Write access to addresses 1000 to 1999
    }

    for (int i = 1000; i < 2000; i++) {
        cache.access(i, true);  // Write access again to the same addresses
    }

    // std::cout<<std::endl<<"Before : "<<std::endl;
    // cache.printCacheState();
    cache.printStats();
    // std::cout<<std::endl<<"After : "<<std::endl;
    // cache.printCacheState();

    std::cout << "Simulating Mixed Access - Read:" << std::endl;
    for (int i = 0; i < 100; i++) {
        cache.access(i, false);  // Read access
    }

    for (int i = 500; i < 3000; i++) {
        cache.access(i, false);  // Read access
    }

    for (int i = 500; i < 3000; i++) {
        cache.access(i, false);  // Read access (temporal)
    }

    // std::cout<<std::endl<<"Before : "<<std::endl;
    // cache.printCacheState();
    cache.printStats();
    // std::cout<<std::endl<<"After : "<<std::endl;
    // cache.printCacheState();

    std::cout << "Simulating Mixed Access - Write:" << std::endl;
    for (int i = 0; i < 1000; i++) {
        cache.access(i, true);  // Write access
    }

    for (int i = 0; i < 1000; i++) {
        cache.access(i, true);  // Write access (temporal)
    }

    for (int i = 2000; i < 6000; i++) {
        cache.access(i, true);  // Write access
    }

    cache.printStats();

    std::cout << "Simulating Mixed Access - Read & Write:" << std::endl;
    // assumption changing array contents from arr[i] to x*arr[i]
    // and "x" here can be a constant or a variable attained from
    // previous read and write operations (above)
    // Hence first we need to read then write
    for (int i = 0; i < 1000; i++) {
        cache.access(i, false); // Read access
        cache.access(i, true);  // Write access
    }

    for (int i = 0; i < 1000; i++) {
        cache.access(i, false); // Read access (temporal)
        cache.access(i, true);  // Write access (temporal)
    }

    for (int i = 2000; i < 6000; i++) {
        cache.access(i, false); // Read access
        cache.access(i, true);  // Write access
    }

    // std::cout<<std::endl<<"Before : "<<std::endl;
    // cache.printCacheState();
    cache.printStats();
    // std::cout<<std::endl<<"After : "<<std::endl;
    // cache.printCacheState();

    return 0;
}