#include<fstream>
using namespace std;

class File_IO{
   ofstream output_file;
   public:
      void open_output_file(const char* fname);
      int* read_address_file(const char* fname);
      void write_solution(int vert_addr,int physical,char value);
      void close_output_file(int page_falts,int hits);
};