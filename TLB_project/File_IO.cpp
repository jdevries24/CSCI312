#include "File_IO.h"
   
void File_IO::open_output_file(const char* fname){
   this->output_file = ofstream(fname);
}

int* File_IO::read_address_file(const char* fname){
   //method to read in a file with a bunch of strings of
   //intergers and convert them into regular intergers
   int* addresses = (int*) malloc(sizeof(int) * 1000);
   ifstream address_file = ifstream(fname);
   int working_addr = 0;
   for(int i = 0;i < 1000;i++){
      unsigned char next_char = address_file.get();
      while(next_char != 0xa){
         if(next_char == 0xd){
            next_char = address_file.get();
            continue;
         }
         working_addr *= 10;
         working_addr += (int)(next_char - 0x30);
         next_char = address_file.get();
      }
      addresses[i] = working_addr;
      working_addr = 0;
   }
   address_file.close();
   return addresses;
}

void File_IO::write_solution(int vert_addr,int physical,char value){
   //just a method to abstract away writing the log file.
   this->output_file << "Virtual address: " << to_string(vert_addr);
   this->output_file << "  Physical address: " << to_string(physical);
   this->output_file << "  Value: " << to_string((int) value) << "\n";
}

void File_IO::close_output_file(int page_falts,int hits){
   this->output_file << "Number of Translated Addresses = 1000\n";
   this->output_file << "Page Faults = " << page_falts << "\n";
   this->output_file << "TLB Hits = " << hits << "\n";
   this->output_file.close();
}