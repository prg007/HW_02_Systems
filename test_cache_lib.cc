

#include "cache.hh"
#include <assert.h>
#include <iostream>
#include "fifo_evictor.hh"
#include "LRU_evictor.hh"

using namespace std;

// Tests basic functionality of Cache methods
void naive_test()
{ 
  //////////////////////////////////////
  // Create cache with capacity 30 bytes.
  // Add 3 key-value pairs whose total value size is 12
  Cache cacheobj(30);
  cacheobj.set("s","Sun",3);
  cacheobj.set("m","Moon",4);
  cacheobj.set("e","Earth",5);

  // checks space used method
  assert(cacheobj.space_used() == 12); 

  ///////////////////////////////////
  // Adds another k-v pair, with total value size in bytes now 19
  cacheobj.set("n","Mercury",7);
  assert(cacheobj.space_used() == 19);

  ////////////////////////////////////////
  // Check if get method updates variable passed by reference correctly. 
  // It should update to the length of the value, which is 3 and not 10.
  Cache::size_type ref_len = 10;
  std::string s1 = "Sun";
  // std::string::compare returns 0 if two strings are equal. 
  assert(s1.compare(cacheobj.get("s",ref_len)) == 0);
  assert(ref_len == 3);

  ///////////////////////////////////////
  s1 = "Saturn";
  // Updating key "s" to "Saturn" from "Sun" to see if it is accurately reflected in cache
  // The reference variable should also be updated to 6 from 3, to reflect the length of "Saturn" from "Sun".
  cacheobj.set("s","Saturn",6);
  assert(s1.compare(cacheobj.get("s",ref_len)) == 0);
  assert(ref_len == 6);

  ////////////////////////////////////
  // Checks if key not in cache returns nullptr, which it should.
  assert(cacheobj.get("z",ref_len) == nullptr);

  //////////////////////////////////
  // check del functionality. "m" should be in cache, "z" should not.
  assert(cacheobj.del("m") == true);
  assert(cacheobj.del("z") ==  false);
  // see if space_used accurately reflects deletes. Should be 22-4 = 18.
  assert(cacheobj.space_used() == 18);

  ////////////////////////////////
  // check reset 
  cacheobj.reset();
  assert(cacheobj.space_used() == 0);
}


Cache::size_type cons(key_type key)
{
  // so compiler doesn't complain
  auto side_effect = key;

  return 1;
}

Cache::size_type mod(key_type key)
{
  return stoi(key) % 10;
}


void hash_test()
{
  // No asserts here because the unordered map is a private data member, so we can't directly check.
  // We instead check by printing out the bucket counts/ number of buckets in the set method and make sure it
  // is what we expected (spoiler: it is. ) See ReadMe for more details.

  // To verify hash_test is working, comment out the other two tests and uncomment the two lines at the end of the set method.

  //////////////////////////
  // tests whether nullptr gets replaced
  Cache cacheobj_null(30, 0.75 ,nullptr, nullptr); // should default to std::hash
  cacheobj_null.set("a","apple",5);
  cacheobj_null.set("b","ball",4);
  // In this case, the hash should behave exactly the same as the std::hash behavior directly below. 
  // Spoiler: it does :)

  ////////////////////////////
  // default parameter std::hash function
  Cache cacheobj1(30, 0.75 ,nullptr);

  // In this case, std::hash should put "a" and "b" into two different buckets, and increase the total number of buckets
  // in the umap. This is because it would exceed the maximum load factor 0.75 otherwise.
  cacheobj1.set("a","apple",5);
  cacheobj1.set("b","ball",4);

  ////////////////////////////////////
  // constant hash function
  Cache::hash_func cons_hasher = cons;
  Cache cacheobj2(30,0.75, nullptr, cons_hasher);

  // In this case, cons_hasher should put "a" and "b" into the same bucket, because they both hash to 1. 
  // The total number of buckets in the umap should be increased because otherwise it would exceed max load factor 0.75.
  cacheobj2.set("a","apple",5);
  cacheobj2.set("b","ball",4);

  //////////////////////////////////
  // mod hash function with load factor 1.1
  Cache::hash_func mod_hasher = mod;
  Cache cacheobj3(30,1.1, nullptr, mod_hasher);

  // In this case:
  cacheobj3.set("10","apple",5);
  // 10 gets hashed to bucket 0, and there are two buckets in the umap to start with, so load factor is 0.5 < 1.1

  cacheobj3.set("25","ball",4);
  // 25 gets hashed to bucket 1, since there are only two buckets (0 and 1). The load factor is now 1 < 1.1

  cacheobj3.set("32","cat",3);
  // 32 gets hashed to bucket 2 after the umap increases the number of buckets to 5 since the load factor would otherwise be 1.5 > 1.1.
}

void fifo_evictor_test()
{
  ////////////////////////////////////
  FifoEvictor fifo;
  Cache cacheobj(10, 0.75, &fifo); // default hasher

  // Testing whether existing key gets updated with eviction policy (touching key twice).
  cacheobj.set("s", "sun", 3);
  cacheobj.set("s", "solar", 5);

  assert(cacheobj.space_used() == 5); 

  // tests fifo eviction test 
  FifoEvictor fifo2;
  Cache cacheobj2(11, 0.75, &fifo2); 
  cacheobj2.set("a", "ant", 3);
  cacheobj2.set("b", "ball", 4);
  assert(cacheobj2.space_used() == 7);

  // Making sure that "a" gets evicted to make room for "c". If this is happening,
  // total space used should be 7 - 3 + 6 = 10.
  cacheobj2.set("c", "campus", 6);
  assert(cacheobj2.space_used() == 10);

  /////////////////////////////////
  // tests whether get method is touching key properly. 
  FifoEvictor fifo3;
  Cache cacheobj3(14, 0.75, &fifo3); 
  cacheobj3.set("a", "ant", 3);
  cacheobj3.set("b", "ball", 4);

  Cache::size_type ref_len = 10;
  string s1 = "ant";
  // touch a again in get method, so it gets added to fifo queue again. 
  assert(s1.compare(cacheobj3.get("a", ref_len)) == 0);
  assert(ref_len == 3);

  /////////////////////////////////////
  cacheobj3.set("c", "campus", 6);
  assert(cacheobj3.space_used() == 13);

  // "a" and "b" should be evicted to make room for "d", so 
  // new space used should be 13 - 3 - 4 + 7 = 13.
  cacheobj3.set("d", "dungeon", 7);
  assert(cacheobj3.space_used() == 13);

  // add new "a" into cache.
  cacheobj3.set("a", "a", 1);
  assert(cacheobj3.space_used() == 14);

  // evictor should evict "a" since we touched it a while back in the get method, even though we just added it. 
  cacheobj3.set("f", "f", 1); 
  assert(cacheobj3.space_used() == 14);

  // "a" should not exist in the cache anymore.
  assert(cacheobj3.del("a") == false);
}

void LRU_evictor_test()
{
  /////////////////////////////
  LRUEvictor lru;
  Cache cacheobj(10, 0.75, &lru, nullptr);
  cacheobj.set("a", "ant", 3);
  cacheobj.set("b", "bobby", 5);
  cacheobj.set("c", "ca", 2);
  Cache::size_type ref_len = 10;
  string s1 = "ant";

  // Getting "a" makes it go to the front of the list, since it was most recently used
  assert(s1.compare(cacheobj.get("a", ref_len)) == 0);
  assert(ref_len == 3);

  cacheobj.set("d", "d", 1);
  // b should be evicted, since a was more recently used
  assert(cacheobj.space_used() == 6);
}

int main()
{
  naive_test();
  hash_test();
  fifo_evictor_test();
  LRU_evictor_test();
  cout << "All tests passed!" << endl;
  return 0;
}
