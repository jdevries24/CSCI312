#include <iostream>
#include "TLB_demo.cpp"
using namespace std;

int main(){
   TLB_demo demo = TLB_demo("addresses.txt","BACKING_STORE.bin","my_output.txt");
   demo.run();
}