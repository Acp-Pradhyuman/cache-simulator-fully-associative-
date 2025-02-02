# Cache Simulation using Fully Associative Mapping

## Problem Statement

The problem involves simulating a cache system with the following specifications:

- **Main Memory size**: 64K words  
- **Cache size**: 2K words  
- **Block size**: 16 words  
- **Word size**: 32 bits  
- **Processor address**: 16 bits  
- **Cache mapping**: Fully associative  
- **Access pattern to simulate**: Spatial access, Temporal access, and Mixed access (read and write)

### The objective is to estimate:

- Cache misses
- Number of cache searches
- Cache hit/miss statistics

## Assumptions

- The main memory always contains the required data, and there is never a need to access the hard disk.
- The access is performed with a fully associative cache mapping scheme.
- The cache uses the Least Recently Used (LRU) policy for cache replacement.
- The memory accesses are simulated for both read and write operations.

## C++ Code

```cpp
#include <iostream>
#include <vector>
#include <climits>

// CacheBlock class represents a single cache block in the cache
class CacheBlock {
public:
    bool valid;               // Valid bit, indicates if the block contains valid data
    bool dirty;               // Dirty bit, indicates if the block has been modified
    int tag;                  // Tag used for identifying the block in the cache
    int lastAccessTime;       // Time of last access for LRU replacement policy
    std::vector<int> data;    // Data stored in the cache block

    // Constructor to initialize a cache block
    CacheBlock(int blockSize) {
        valid = false;         // Initially, the block is invalid
        dirty = false;         // Initially, the block is not dirty
        tag = -1;              // Initially, no tag
        lastAccessTime = 0;    // Initially, no access time
        data.resize(blockSize); // Resize the data vector to hold the block's data
    }
};
```

# Cache Simulator

This project simulates a cache memory system with basic operations like memory access, read/write misses, and cache replacement using the Least Recently Used (LRU) policy.

## Classes

### `CacheBlock` Class

The `CacheBlock` class represents a single block in the cache. It has the following attributes:

- **valid**: A boolean indicating whether the block contains valid data.
- **dirty**: A boolean indicating whether the block has been modified.
- **tag**: The tag that identifies the block’s memory location.
- **lastAccessTime**: The timestamp of the last access to the block, used for the LRU replacement policy.
- **data**: A vector holding the actual data stored in the block.

The constructor initializes the attributes:
- `valid` is set to `false`
- `dirty` is set to `false`
- `tag` is set to `-1`
- `data` is resized based on the block size.

### `Cache` Class

The `Cache` class represents the entire cache system and handles caching operations. It includes the following attributes:

- **cache**: A vector of `CacheBlock` objects representing the cache blocks.
- **numBlocks**: The total number of blocks in the cache.
- **blockSize**: The number of words per block.
- **currentTime**: The current time used to track the least recently used block.
- **cacheMisses**: A counter for the total number of cache misses.
- **readMisses**: A counter for the number of read misses.
- **writeMisses**: A counter for the number of write misses.
- **cacheSearches**: A counter for the number of cache searches.

#### Methods

- **access()**: This method performs a memory access (either read or write) and updates the cache state accordingly. If a miss occurs, it updates the cache using the LRU policy.
  
- **printStats()**: This method prints cache statistics, including the number of misses, hits, and hit rate.

## How It Works

- The `CacheBlock` class models individual cache blocks, where each block contains a tag, a flag to indicate if the block is dirty or valid, the data stored, and the last access time for LRU replacement.
  
- The `Cache` class handles memory accesses and keeps track of cache hits, misses, and their respective statistics. It uses the LRU policy to replace blocks in case of cache misses.