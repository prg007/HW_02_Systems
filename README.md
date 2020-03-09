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


# Performance

We allowed the `std::unordered_map` implementation to accept the hash function from the hash constructor. 

# Collision Resolution

# Dynamic Resizing

# Eviction Policy (FIFO)

# Testing

 Refer to the source file  `test_cache_lib.cc` for additional comments. We are using the assert statement for testing purposes.  
 

 
## Naive Test

Naive test simply tests the functionality of `get`, `set`, `del`,`space_used` and `reset` methods which assumes the default eviction policy i.e. `nullptr`. Refer to the source file to see what tests we came up with. 

## Hash Test

Tests the functionality of passing custom hash functions into the cache. We tested 3 different hash functions.If the hash function is nullptr, then we default to `std::hash`.(First 2 blocks of the screenshot) To check the hash test, uncomment out the last 2 lines of the set method.

The **vanilla hash function** i.e. the default `std::hash_function`(Next 2 blocks) A **constant hasher** which just returns 1 regardless of what key we pass in.(next 2 blocks) A **mod** hasher which takes a string representation of an integer and returns mod 10 of the `stoi(key)` (Last 3 blocks of the screenshot). Finally,. We also test different max load factors. Refer to the source file to see what tests we came up with. 

We cannot use asserts in this test because the unordered map is a private data member of the cache. So, in order to check that the hash functions are working properly we print out the bucket each key is in using the `umap.bucket(key)` method and the total no. of buckets using `umap.bucket_count()`. If they are working correctly we should get the screenshot below which we do. For further comments as to how the tests work and the meaning of the screenshot, refer to the source file. 

<img src = "https://github.com/prg007/HW_02_Systems/blob/master/hashtest_1.PNG" width = "400" height = "500"/>

## FIFO Evictor Test

Tests the functionality of the fifo eviction policy. We use an `std::queue` as our implementation of the `fifo_eviction` policy. For further comments about what tests we are using refer to the source file `test_cache_lib.cc` again. 
