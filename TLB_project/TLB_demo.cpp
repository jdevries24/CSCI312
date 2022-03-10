#include "TLB_demo.h"

TLB_demo::TLB_demo(string address_fname,string disk_fname,string output_fname){
   /*so the constructor just has lots of inits in it
   files need to be opened, arrays need to be allocated,
   you know computer busy work */
   this->IO = File_IO();
   this->addresses = this->IO.read_address_file(address_fname.c_str());
   this->IO.open_output_file(output_fname.c_str());
   this->disk = ifstream(disk_fname.c_str());
   this->physical_memory = (char*) calloc(65536,sizeof(char));
   this->TLB_max = 0;
   this->Page_max = 0;
   this->TLB = Dict(16);//our TLB is a 16 entity dictonary 
   this->Page_table = Dict(256);//the Page table needs 256 entites
   this->Page_falts = 0;//Need to cout page falts for later
   this->TLB_hits = 0;//as do TLB_hits
}

void TLB_demo::run(){
   /*the first part is simple simply run through all
   1000 vertual addresses get the pysical addresses 
   and log it all to a file */
   for(int i = 0;i < 1000;i++){
      int v_addr = this->addresses[i];
      int p_addr = this->get_physical_addr(v_addr);
      char value = this->physical_memory[p_addr];
      this->IO.write_solution(v_addr,p_addr,value);
   }
   //and free and close all unneeded arrays and files
   free(this->physical_memory);
   this->TLB.free_lists();
   this->Page_table.free_lists();
   this->IO.close_output_file(this->Page_falts,this->TLB_hits);
}

int TLB_demo::get_physical_addr(int v_addr){
   /*Given vertual address 0x23A2 the below statement
   converts that number into 0x0023. that new number
   will serve as the page number */
   int page_number = (v_addr >> 8) & 0xff;
   //then we see if our page number is in the TLB
   if(!this->TLB.has_key(page_number)){
      //If the page number isnt in the TLB then update the TLB
      this->update_TLB(page_number);
   }
   else{
      //If it is then thats a TLB hit
      this->TLB_hits += 1;
   }
   /*now there is a entity in the TLB for the page number
   if the frame number is 0x005F the next few statements
   merge that with the existing low byte of the vertual
   address to return the physical address to get 0x5fA2*/
   int high_byte = this->TLB.get_by_key(page_number) << 8;
   int low_byte = v_addr & 0xff;
   return high_byte | low_byte;
}

void TLB_demo::update_TLB(int page_number){
   //first off check if there is a entity for our
   //given page number
   if(!this->Page_table.has_key(page_number)){
      //if not we need to update the page_table
      this->update_page_table(page_number);
      //and its a page falt
      this->Page_falts += 1;
   }
   //the next to line update the frame number given a page_number
   int frame_number = this->Page_table.get_by_key(page_number);
   this->TLB.update_entity(this->TLB_max,page_number,frame_number);
   //the TLB max and next line of code allows for a first in last out
   //structure of the TLB
   this->TLB_max = (this->TLB_max + 1) % 16;
}

void TLB_demo::update_page_table(int page_number){
   /*the page_number is also the frame number for data that is stored in the disk
   Page max is a counting varible that can act as the frame pointer for what we store
   in the main memory*/
   this->Page_table.update_entity(this->Page_max,page_number,this->Page_max);
   this->load_frame(this->Page_max,page_number);
   this->Page_max += 1;
}

void TLB_demo::load_frame(int memory_frame_number,int disk_frame_number){
   //loads a frame from disk_frame into main memory
   disk_frame_number << 8;
   memory_frame_number << 8;
   this->disk.seekg(disk_frame_number);
   for(int i = 0;i < 256;i++){
      this->physical_memory[i + memory_frame_number] = disk.get();
   }
}
   