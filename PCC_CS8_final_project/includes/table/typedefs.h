#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <vector>
#include <queue>
#include "../queue/MyQueue.h"
#include "../bplustree/map.h"
#include "../bplustree/multimap.h"
using namespace std;

typedef Map<string, string> map_ss;
typedef MMap<string, string> mmap_ss;

typedef Map<string, long> map_sl;
typedef MMap<string, long> mmap_sl;

typedef vector<string> vectorstr;
typedef vector<long> vectorlong;
typedef Queue<string> queuestr;

#endif