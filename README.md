# Hash it out!

## Names: Hrishee and Prasun

# File Overview

`cache.hh`, `cache_lib.cc` - Interface and implementation of the cache class.

`evictor.hh` -Abstract base class of the evictor class, which details the general interface all eviction policies adhere to.

`fifo_evictor.hh`, `fifo_evictor.cc` - Interface and implementation of the fifo eviction policy.

`test_cache_lib.cc` - Our test cases. Also the entry point into the main function.

`Makefile` - Makefile!


# How to run
To compile and run, just type `make` and then `./test`. You may have to change the compiler version in the CXX variable to match the compiler on your computer.
**We highly suggest running it on the Linux Virtual Machine.**
We checked our program for memory leaks using `vagrant`, and none were found.

# Basic Cache Operations

We are implementing our cache using the `std::unordered_map` data structure which is a built in hash table inside the C++ STL. To understand the utility of different variables and functions, we refer you to the source file inside the `cache_lib.cc` file. Everything has been explained in detail.

# Testing

 Refer to the source file  `test_cache_lib.cc` for additional comments. We are using the assert statement for testing purposes. 
 
## Naive Test

Naive test simply tests the functionality of `get`, `set`, `del`,`space_used` and `reset` methods which assumes the default eviction policy i.e. `nullptr`. Refer to the source file to see what tests we came up with. 

## Hash Test

Tests the functionality of passing custom hash functions into the cache. We tested 3 different hash functions. A **constant hasher** which just returns 1 regardless of what key we pass in. A **mod** hasher which takes a string representation of an integer and returns mod 10 of the `stoi(key)`. Finally, the vanilla hash function i.e. the default `std::hash_function`. Refer to the source file to see what tests we came up with. 

We cannot use asserts in this test because the unordered map is a private data member of the cache. So, in order to check that the hash functions are working properly we print out the bucket each key is in using the `umap.bucket(key)` method and the total no. of buckets using `umap.bucket_count()`. If they are working correctly we should get the screenshot below which we are. For further comments as to how the tests work, refer to the source file. 



# Performance


# Collision Resolution

# Dynamic Resizing

# Eviction Policy (FIFO)
