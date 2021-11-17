#include "Calculator.h"
using namespace std;
/* Program Name: Calculator
* Programer Name: Jeffrey Devries
* Program date:11/15/2021
* Program Discription: This is the raw calculator class
* for the Calcuator with undo assignment. All this calculator
* Does is take in strings with resovabal equations. Then the calc
* Parses the string and solves. It will also put the solution onto 
* a stack
*/

calculator::calculator(){
   //Just a Default constructor
   this->ans_stack = list<float>();
}

string calculator::get_ans(string equ){
   /* this method is the interface of the calculator. 
    * it takes in a string of a equation then solves it.
    * it will send back a string with an answer or information
    * about some error. */
   try{
      return to_string(this->resolve(equ));
   }
   catch(const calculator_error error){
      switch(error){
         case No_ans:
            return "No answer to delete or use";
         case div_by_zero:
            return "Divide by zero";
      }
   }
   catch(const lex_parse_error error){
      switch(error){
         case No_prev_oprand_for_op:
            return "possible double operation";
         case No_next_oprand_for_op:
            return "possible double operation";
         case bad_char:
            return "Unknown char in input";
      }
   }
   return "Unknown error... HOW?";
}

void calculator::UNDO(){
   //for the UNDO function
   if(this->ans_stack.size() == 0){ //error handling
      throw No_ans;
   }
   this->ans_stack.pop_back();
}

void calculator::CLEAR(){
   //this still should work even with an empty stack so 
   //no error handeling needed. 
   this->ans_stack.erase(this->ans_stack.begin(),this->ans_stack.end());
}

float calculator::resolve(string equ){
   //convert a equation string into a float
   lexer_parser* lp = new lexer_parser(); //parse out the string 
   syntax_node *root = lp->parse(equ);
   float result = this->solve(root);//then recursivly solve the parse tree
   this->ans_stack.push_back(result);//and puth the answer onto the stack
   delete lp;//and delete the parse tree
   return result;
}

float calculator::solve(syntax_node *r_node){
   //for recursivly solving a tree 
   if(r_node->type == number){return stof(r_node->value);}//if our node is a number simply convert the string to a float
   if(r_node->type == operation){//operation ie +,-,/,*
      float a = this->solve(r_node->left_node);//get the left and right value
      float b = this->solve(r_node->right_node);
      return this->op(a,b,r_node->value[0]);// and do the binary op
   }
   if(r_node->type == ans){//for grabing an answer
      if(this->ans_stack.size() == 0){
         throw No_ans;
      }
      return this->ans_stack.back();
   }
   throw "unknown operation";
}

float calculator::op(float a,float b,char op){
   //takes in two floats and an op then return the 
   //binary result
   if(op == '+'){return a + b;}
   if(op == '-'){return a - b;}
   if(op == '*'){return a * b;}
   if(op == '/'){
      if(b == 0){throw div_by_zero;}
      return a / b; // and error checking
   }
   return 0;
}

int main(){
   try{
      string test_str;
      cout << "equ: " << flush;
      cin >> test_str;
      calculator calc = calculator();
      cout << calc.get_ans(test_str) << endl;
   }
   catch(const char* e){
      cout << e << '\n' << flush;
   }
}