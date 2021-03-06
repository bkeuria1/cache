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
	float  access;
};



Cache::Cache(vector<char>_instruction,vector< long long>_address):instruction(_instruction), address(_address){

}

long Cache:: directMapped(int sizes){

	long hits = 0;
	//2^5 = 32 
       
	//Block address mode number of block in cache
	//Since 32 bits, 3
	//Cache size = Number of sets * Num
	long long DMcache [sizes][2];
	for(int i = 0; i<sizes;i++){
		for(int j=0;j<2;j++){
			DMcache[i][j] = 0;
		}
	}
       	
        int offset = (int)(log2(sizes)+5);
//        cout<<offset<<endl;
	for(int i =0;i<address.size();i++){
		//figure out if its a hit or not

		int block =floor( address[i]/32);
		int index = block%sizes;	
		
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
	int sets = 16384/(32*ways);	
  //      cout<<"SETs: "<<sets;
	entry SAcache[sets][ways];  
//	t = clock()
	for(int i=0;i<sets;i++){
		for(int j=0;j<ways;j++){
			SAcache[i][j].tag = -1;
			SAcache[i][j].valid = 0;
			SAcache[i][j].access = 0;
		//	lru[i][j] = 0;
		}
	}
//        int debug = 0;
//	cout<<ways<<" associative cache and 16KB size cache, 32 byte line"<<endl;

        int  offset = (int)(log2(sets)+5); //offset to get tag value
//	cout<<"Offset: "<<offset<<endl; 
 	for(int i =0;i<address.size();i++){
		int block = floor(address[i]/32);

		int index = block%sets; //get the set number
		long tag = address[i]>>offset;  //get the tag
		bool found = false;
		//traverse through each way in the in the set
		for(int j = 0;j<ways;j++){
			if(SAcache[index][j].tag == tag && SAcache[index][j].valid == 1){
			      
				hits++;
				SAcache[index][j].access = i;
				found = true;
				break;	
			}	
		}
		//bring data in cache	
		bool empty = false;

		if(!found){
			for(int k=0;k<ways;k++){
				if(SAcache[index][k].valid==0){
					SAcache[index][k].valid = 1;
					SAcache[index][k].tag = tag;
					SAcache[index][k].access = i;
					empty = true;
					break;
				}
			}	

		if(!empty){
			int minIndex = -1;
			int least = INT_MAX;
			for(int l = 0; l<ways; l++){
				if(SAcache[index][l].access<least){
					least = SAcache[index][l].access;
					minIndex = l;
				}
			}
		
			SAcache[index][minIndex].valid = 1;
			SAcache[index][minIndex].tag = tag;
			SAcache[index][minIndex].access = i;
		}
	
		}
	}   	
	
	return hits;
}

long Cache:: hotColdLRU(){
	long hits = 0;

	int ways = 512; 
	vector<unsigned long long>c(512,0);
	
	vector<int>hotCold(511,0); //everybit is set to 0
	int offset = 5;
	bool found = false;
	//loop through every address
 	for(int i = 0;i<address.size();i++){
		found = false;
		unsigned long long tag = address[i]>>5;
		for(int k = 0;k<512;k++){
			if(c[k] == tag){
				hits++;
				int treeIndex = k+511;
				while(treeIndex!=0){
					if(treeIndex%2==0){
						treeIndex = (treeIndex-2)/2;
						hotCold[treeIndex] = 0;
					}else{
						treeIndex = (treeIndex-1)/2;
                                                hotCold[treeIndex] = 1;
					}
				}
				found = true;
				break;
			}
		}
		if(!found){
			int cold = 0;
			int level = 0;
			while(level<9){
				if(hotCold[cold] == 0){
					hotCold[cold] =1;
					cold = cold*2+1;
				}else{
					hotCold[cold] =0;
                                        cold = cold*2+2;
				
				}
				level++;
			}
			cold = cold-511;
			c[cold] = tag;
		}
	}
	return hits;

}


long Cache:: noAlloc(int ways){

	long hits = 0;
    
	int sets = 16384/(32*ways);	
 
	entry SAcache[sets][ways];  
	for(int i=0;i<sets;i++){
		for(int j=0;j<ways;j++){
			SAcache[i][j].tag = -1;
			SAcache[i][j].valid = 0;
			SAcache[i][j].access = 0;
		}
	}


        int  offset = (int)(log2(sets)+5); //offset to get tag value
//	cout<<"Offset: "<<offset<<endl; 
 	for(int i =0;i<address.size();i++){
		int block = floor(address[i]/32);

		int index = block%sets; //get the set number
		long tag = address[i]>>offset;  //get the tag
		bool found = false;
		//traverse through each way in the in the set
		for(int j = 0;j<ways;j++){
			if(SAcache[index][j].tag == tag && SAcache[index][j].valid == 1){
			  
				hits++;
				SAcache[index][j].access = i;
				found = true;
				break;	
			}	
		}
		//bring data in cache	
		bool empty = false;

		if(!found && instruction[i]!='S'){
			//go through the ways and if there is a an empty entry
			for(int k = 0;k<ways;k++){
				if(SAcache[index][k].valid ==0){
					//found indvalid page, can write
					SAcache[index][k].valid = 1;
					SAcache[index][k].tag = tag;
					SAcache[index][k].access = i;
					empty = true;
					break;
				}
			
		}
		if(!empty){
			int minIndex = -1;
			int least = INT_MAX;
			for(int l = 0; l<ways; l++){
				if(SAcache[index][l].access<least){
					least = SAcache[index][l].access;
					minIndex = l;
				}
			}
		
			SAcache[index][minIndex].valid = 1;
			SAcache[index][minIndex].tag = tag;
			SAcache[index][minIndex].access = i;
		
		}
		}
	}   	
	
	return hits;
}
long Cache:: nextLine(int ways){
	long hits = 0;
        
	int sets = 16384/(32*ways);	
  
	entry SAcache[sets][ways];  

	for(int i=0;i<sets;i++){
		for(int j=0;j<ways;j++){
			SAcache[i][j].tag = -1;
			SAcache[i][j].valid = 0;
			SAcache[i][j].access = 0;
	
		}
	}
        int  offset = (int)(log2(sets)+5); //offset to get tag value

 	for(int i =0;i<address.size();i++){
		int block = floor(address[i]/32);

		int index = block%sets; //get the set number
		int nextIndex = (block+1)%sets;
		long tag = address[i]>>offset;  //get the tag
	        long nextTag =( address[i]+32)>>offset;
		bool found = false;
		//traverse through each way in the in the set
		for(int j = 0;j<ways;j++){
			if(SAcache[index][j].tag == tag && SAcache[index][j].valid == 1){
				hits++;
				SAcache[index][j].access = i;
				found = true;
				break;	
			}	
		}
		bool nextFound = false;
		for(int j = 0;j<ways;j++){
                        if(SAcache[nextIndex][j].tag ==nextTag && SAcache[nextIndex][j].valid == 1){
                               
                                SAcache[nextIndex][j].access = i;
                               	nextFound = true;
                                break;
                        }
                }
		
		bool empty = false;

		if(!found){
		 for(int k=0;k<ways;k++){
                                if(SAcache[index][k].valid==0){
                                        SAcache[index][k].valid = 1;
                                        SAcache[index][k].tag = tag;
                                        SAcache[index][k].access = i;
                                        empty = true;
                                        break;
                                }
                        }	
                if(!empty){
                       
			int minIndex = -1;
                        int least = INT_MAX;
                        for(int l = 0; l<ways; l++){
                                if(SAcache[index][l].access<least){
                                        least = SAcache[index][l].access;
                                        minIndex = l;
                                }
                        }

                        SAcache[index][minIndex].valid = 1;
                        SAcache[index][minIndex].tag = tag;
                        SAcache[index][minIndex].access = i;
			
		}
	
		}
		empty = false;
		if(!nextFound){
			  for(int k=0;k<ways;k++){
                                if(SAcache[nextIndex][k].valid==0){
                                        SAcache[nextIndex][k].valid = 1;
                                        SAcache[nextIndex][k].tag = nextTag;
                                        SAcache[nextIndex][k].access = i;
                                        empty = true;
                                        break;
                                }
                        }
			if(!empty){
			//need to find the Least recently used block
			int minIndex = -1;
			int least = INT_MAX;
			for(int l = 0; l<ways; l++){
				if(SAcache[nextIndex][l].access<least){
					least = SAcache[nextIndex][l].access;
					minIndex = l;
				}
			}
		
			SAcache[nextIndex][minIndex].valid = 1;
			SAcache[nextIndex][minIndex].tag = nextTag;
			SAcache[nextIndex][minIndex].access = i;
		
		
		}
		}
		
	}   	
	
	return hits;


}

long Cache::  preFetchMiss(int ways){

		long hits = 0;
        
	int sets = 16384/(32*ways);	
  
	entry SAcache[sets][ways];  

	for(int i=0;i<sets;i++){
		for(int j=0;j<ways;j++){
			SAcache[i][j].tag = -1;
			SAcache[i][j].valid = 0;
			SAcache[i][j].access = 0;
	
		}
	}
        int  offset = (int)(log2(sets)+5); //offset to get tag value

 	for(int i =0;i<address.size();i++){
		int block = floor(address[i]/32);

		int index = block%sets; //get the set number
		int nextIndex = (block+1)%sets;
		long tag = address[i]>>offset;  //get the tag
	        long nextTag =( address[i]+32)>>offset;
		bool found = false;
		bool empty = false;
		bool nextFound = false;
		//traverse through each way in the in the set
		for(int j = 0;j<ways;j++){
			if(SAcache[index][j].tag == tag && SAcache[index][j].valid == 1){
				hits++;
				SAcache[index][j].access = i;
				found = true;
				break;	
			}	
		}
		if(!found){
		//bool nextFound = false;
		for(int j = 0;j<ways;j++){
                        if(SAcache[nextIndex][j].tag ==nextTag && SAcache[nextIndex][j].valid == 1){
                               
                                SAcache[nextIndex][j].access = i;
                               	nextFound = true;
                                break;
                        }
                }
		
		

		
		 for(int k=0;k<ways;k++){
                                if(SAcache[index][k].valid==0){
                                        SAcache[index][k].valid = 1;
                                        SAcache[index][k].tag = tag;
                                        SAcache[index][k].access = i;
                                        empty = true;
                                        break;
                                }
                   }	
                if(!empty){
                       
			int minIndex = -1;
                        int least = INT_MAX;
                        for(int l = 0; l<ways; l++){
                                if(SAcache[index][l].access<least){
                                        least = SAcache[index][l].access;
                                        minIndex = l;
                                }
                        }

                        SAcache[index][minIndex].valid = 1;
                        SAcache[index][minIndex].tag = tag;
                        SAcache[index][minIndex].access = i;
			
		}
	
		
		empty = false;
		if(!nextFound){
			  for(int k=0;k<ways;k++){
                                if(SAcache[nextIndex][k].valid==0){
                                        SAcache[nextIndex][k].valid = 1;
                                        SAcache[nextIndex][k].tag = nextTag;
                                        SAcache[nextIndex][k].access = i;
                                        empty = true;
                                        break;
                                }
                        }
			if(!empty){
			//need to find the Least recently used block
			int minIndex = -1;
			int least = INT_MAX;
			for(int l = 0; l<ways; l++){
				if(SAcache[nextIndex][l].access<least){
					least = SAcache[nextIndex][l].access;
					minIndex = l;
				}
			}
		
			SAcache[nextIndex][minIndex].valid = 1;
			SAcache[nextIndex][minIndex].tag = nextTag;
			SAcache[nextIndex][minIndex].access = i;
		
		
		}
		}
		}
		
	}   	
	
	return hits;
     
}
