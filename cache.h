#ifndef CACHE_H
#define CAHCE_H
#include <string>
#include <vector>
#include <map>
using namespace std;
class Cache{
        public:
		Cache();
		Cache( vector<char>instruction,vector< long long> address);
		long directMapped(int);
		long setAssociative(int);
	
	private:
		vector<char>instruction;
		vector< long long> address;
 		
};

#endif
