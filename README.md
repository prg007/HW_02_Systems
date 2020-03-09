# Hash it out!

## Names: Hrishee and Prasun

# File Overview

`cache.hh`, `cache_lib.cc` - Interface and implementation of the cache class.

`evictor.hh` -Abstract base class of the evictor class, which details the general interface all eviction policies adhere to.

`fifo_evictor.hh`, `fifo_evictor.cc` - Interface and implementation of the fifo eviction policy.

`LRU_evictor.hh`, `LRU_evictor.cc` - Interface and implementation of the LRU eviction policy.

`test_cache_lib.cc` - Our test cases. Also the entry point into the main function.

`Makefile` - Makefile!


# How to run
To compile and run, just type `make` and then `./test`. You may have to change the compiler version in the CXX variable to match the compiler on your computer.
**We highly suggest running it on the Linux Virtual Machine.**

**We checked our program for memory leaks using `valgrind`, and none were found.**
`valgrind --leak-check=yes ./test`


# Basic Cache Operations

We are implementing our cache using the `std::unordered_map` data structure which is a built in hash table inside the C++ STL. To understand the utility of different variables and functions, we refer you to the source file inside the `cache_lib.cc` file. Everything has been explained in detail.


# Performance

We allowed the `std::unordered_map` implementation to accept the hash function from the Cache constructor. If the user passes `nullptr` as the hash function, then we default to `std::hash` just like the default parameter. Since `u_map` is a STL hash table, all operations run in asymptotic constant time. In our test cases we create custom hash functions using `std::function`. Go to the testing section of this README for more details about this.  

# Collision Resolution

To resolve collsions, we rely on `std::unordered_map` default collsion resolution policy. As it turns out, they use **separate chaining** to resolve collisions. More info here: [U_Map Collision Resolution Link](https://stackoverflow.com/questions/21518704/how-does-c-stl-unordered-map-resolve-collisions). 


# Dynamic Resizing

We implemented the max load factor by passing it to the unordered map from the cache constructor. The unordered map automatically resizes the umap/hash_table to accomodate the max load factor constraint when adding new key-value pairs. By default the max_load_factor is 0.75. Refer to the `hash_test()` function inside the file `test_cache_lib.cc` to see how we test this functionality. Also, refer to the **Hash Test** section inside **Testing** part of README for more details.

# Eviction Policy (FIFO)

For our FIFO eviction policy we use the `std::queue` implementation. Note that whenever we get or set a key, we add that key to the  queue. Whenever we evict something from the queue we dequeue it and return to the caller. Both of these operations are constant time. Refer to the **FIFO EVICTOR Test** inside the **Testing** section part of the README for more details. 

# Eviction Policy (LRU)

We are implemeting the LRU evictor test using `std::list` and `std::unordered_map`. The list holds all the keys with the most recently used ones at the front of the list and the least recently used ones at the back of the list (basically acting as a queue). The unordered map is a key-value map where keys map to their corresponding list iterators. This ensures O(1) time for removing a single key from the list, and O(1) search for keys through the hash map. Also, note that space used is O(n) because we are only dealing with n+n = 2n = O(n) keys in the list and hash map, where n is the number of keys in the cache. Our implementation does not affect any of the existing interface/implementation.

Refer to the source file `LRU_evictor.cc` and `LRU_evictor.hh` for more implementation comments. 

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
## LRU Evictor Test
 Tests the functionality of the LRU eviction policy. We refer you to the `test_cache_lib.cc` source file once again for details and comments about our test cases.
