#include "File_IO.cpp"
#include "Dict.h"
using namespace std;

class TLB_demo{
   File_IO IO;
   ifstream disk;
   char* physical_memory;
   int* addresses;
   Dict TLB;
   Dict Page_table;
   int TLB_max;
   int Page_max;
   int Page_falts;
   int TLB_hits;
   
   public:
      TLB_demo(string address_fname,string disk_fname,string output_fname);
      void run();
   private:
      int get_physical_addr(int v_addr);
      void update_TLB(int page_number);
      int get_page_addr(int page_number);
      void update_page_table(int page_number);
      void load_frame(int memory_frame_number,int disk_frame_number);
};