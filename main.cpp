#include  <iostream>
#include <vector>
#include<fstream>
#include<string>
#include "cache.h"
using namespace std;

int main(int argc, char*argv[]){

	vector<long long>address;
	vector<char>instruction;
         unsigned long long addr;
 	 char instr;
	 ifstream infile(argv[1]);
	while(infile >>instr >> std::hex>> addr) {
		instruction.push_back(instr);
		address.push_back(addr);
        }
	Cache cache(instruction, address);
	cout<<cache.directMapped(1024)<<endl;
	cout<<cache.setAssociative(16)<<endl;
	cout<<cache.setAssociative(512)<<endl;
}
