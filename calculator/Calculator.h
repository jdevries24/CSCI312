#include <regex>
#include <string>
#include <iostream>
#include <list>
#include "Lexer_Parser.cpp"
using namespace std;

enum calculator_error{No_ans,div_by_zero};
class calculator{
   
   list<float> ans_stack;
   public:
   calculator();
   void pop_char();
   string get_input();
   string get_ans(string equ);
   void UNDO();
   void CLEAR();
   private:
   float resolve(string equ);
   float solve(syntax_node *r_node);
   float op(float a,float b,char oprand);
};

//vhat is component composition
//define service oriented architecture