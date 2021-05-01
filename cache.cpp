#include "cache.h"
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <cmath>
#include <climits>
using namespace std;

Cache::Cache(){
}

struct entry{
	long long tag;
	long long valid; 
	long long access;
};

Cache::Cache(vector<char>_instruction,vector<long long>_address):instruction(_instruction), address(_address){}

long Cache:: directMapped(int sizes){

	long hits = 0;
	//2^5 = 32 
       
	//Block address mode number of block in cache
	//Since 32 bits, 3
	//Cache size = Number of sets * Number of ways * Block size 
	//need to store tag and data in table
        //calculate number of blocks
	long long DMcache [sizes][2]; //one entry to store valid bit , one entry to store tag
	for(int i = 0; i<sizes;i++){
		for(int j=0;j<2;j++){
			DMcache[i][j] = 0;
		}
	}
        int offset = (int)(log2(sizes)+5);
        for(int i =0;i<address.size();i++){
		//figure out if its a hit or not
		//find address and indeex
		int block =floor( address[i]/32);
		int index = block%sizes;	
		//cache line size 32 = 5 bits
		//log2(sizes)
               // int offset = (int)log2(sizes)+5;
        	int tag = address[i]>>offset;
		if(DMcache[index][0]==1 && DMcache[index][1] == tag){
			hits++;

		}else{
	
			DMcache[index][0] =1;
			DMcache[index][1] = tag;
		}
	}


	return hits;
}
long Cache:: setAssociative(int ways){
	long hits = 0;
        //figure out number of sets
	int sets = 512/ways;	
  //      cout<<"SETs: "<<sets;
	entry SAcache[sets][ways]; 
//	long long lru [sets][ways]; //keep track of the count
	for(int i=0;i<sets;i++){
		for(int j=0;j<ways;j++){
			SAcache[i][j].tag = -1;
			SAcache[i][j].valid = 0;
			SAcache[i][j].access = 0;
		//	lru[i][j] = 0;
		}
	}
//        int debug = 0; 
        int offset = (int)(log2(sets) + 5);
 	for(int i =0;i<address.size();i++){
		int block = floor(address[i]/32);

		int index = block%sets
;
		int tag = address[i]>>offset;
		bool found = false;
		//traverse through each way in the in the set
		for(int j = 0;j<ways;j++){
			if(SAcache[index][j].tag == tag && SAcache[index][j].valid == 1){
				//found a hit
				hits++;
				SAcache[index][j].access++;
				found = true;
				break;	
			}	
		}
		//bring data in cache	
		bool empty = false;
		if(!found){
			//go through the ways and if there is a an empty entry
			for(int k = 0;k<ways;k++){
				if(SAcache[index][k].valid ==0){
					//found indvalid page, can write
					SAcache[index][k].valid = 1;
					SAcache[index][k].tag = tag;
					SAcache[index][k].access = 0;
					empty = true;
					break;
				}
			}
		}
		if(!empty){
			//need to find the Least recently used block
			int minIndex = -1;
			int minAccess = INT_MAX;
			for(int l = 0; l<ways; l++){
				if(SAcache[index][l].access<minAccess){
					minAccess = SAcache[index][l].access;
					minIndex = l;
				}
			}
			SAcache[index][minIndex].valid = 1;
			SAcache[index][minIndex].tag = tag;
			SAcache[index][minIndex].access = 0;

		}
	}   	
	
	return hits;
}
