#include "Lexer_Parser.h"
using namespace std;

/*this class is a basic lexer and parser.
* for lexing and parsing a resovible equation.
* the parsing doesn't respect order of operation
* but is very simple left to right*/
lexer_parser::lexer_parser(){
   this->token_list = list<syntax_node*>();
}

syntax_node *lexer_parser::parse(string input_string){
   //Lex and parse out a string of an equation
   this->lex(input_string);//Lex out the string
   while(this->token_list.size() != 0){
      syntax_node *next_node = this->token_list.front();
      this->token_list.pop_front();
      if ((next_node->type == number) || (next_node->type == ans))
      {
         //if the token is a unary token simply put it on the tree stack
         this->tree_stack.push_back(next_node);
      }
      else{
         //if its an operation resolve the left and right then create the new node
         if(this->tree_stack.size() == 0){
            throw No_prev_oprand_for_op; //need a left node
         }
         syntax_node *right = this->token_list.front();
         this->token_list.pop_front();
         if (right->type != number){
            throw No_next_oprand_for_op;//need a right node thats a number
         }
         syntax_node *left = this->tree_stack.back();
         this->tree_stack.pop_back();
         next_node->left_node = left;//set a left and right leafs and push it onto the tree stack
         next_node->right_node = right;
         this->tree_stack.push_back(next_node);
      }
   }
   return this->tree_stack.back();
}

string lexer_parser::get_tree(){
   //for dumping out a tree for debuging. Not for general use
   return this->visit_tree(this->tree_stack.back()," "," ");
}

string lexer_parser::visit_tree(syntax_node *snode,string loc,string acm){
   //for recursive dumping of tree
   if((snode->type == number) || (snode->type == ans)){
      return acm + loc + ":" + snode->value + "\n";
   }
   acm += loc + ":"+ snode->value + "\n";
   loc.push_back('1');
   acm = this->visit_tree(snode->left_node,loc,acm);
   loc.pop_back();
   loc.push_back('0');
   acm = this->visit_tree(snode->right_node,loc,acm);
   loc.pop_back();
   return acm;
}
   
void lexer_parser::lex(string input){
   //for lexing out a string turring it from a string
   //to a list of tokens. The types are Number(real_literal),Answer,Operation,END
   char next_char = input[0];
   token_type c_type = this->get_type(next_char);
   this->current_val = "";
   token_type p_type = c_type;
   if(c_type == operation){
      //if the first token is an operation then there is a implicit ans token
      this->token_list.push_back(this->new_node(ans,"ANS"));
   }
   for(int i = 0;i < input.length();i++){
      //move thru the string
      p_type = c_type;
      next_char = input[i];
      c_type = this->get_type(next_char);
      if(c_type != p_type){//if the type has changed then append the token value to 
         //the token list and reset the token value
         this->token_list.push_back(this->new_node(p_type,this->current_val));
         this->current_val = "";
      }
      this->push_char(next_char);//for every char push it to the current val
   }
   this->token_list.push_back(this->new_node(c_type,this->current_val));//and push the last token to list
   this->token_list.push_back(this->new_node(END_SEQ,"END"));//and the END token
}

void lexer_parser::push_char(char a_char){
   string special = "pna";//for special chars ie -,+ signes are 
   //not good for context free gramers therefore the p,n chars are 
   //stand in for that. and ANS if for the answer token
   if(special.find(a_char) == string::npos){
      this->current_val.push_back(a_char);
      return;
   }
   if(a_char == 'n'){
      this->current_val.push_back('-');
   }
   if(a_char == 'a'){
      this->current_val += "ANS";
   }
}

token_type lexer_parser::get_type(char a_char){
   //for determing what type token a char is involved in. 
   string numbers = "0123456789.xnp";
   string operations = "+-/*";
   if(numbers.find(a_char) != string::npos){return number;}//Dont think that CPP regex methods have a search for char method 
   if(operations.find(a_char) != string::npos){return operation;}//so this is a kinda bad middle type
   if(a_char == 'a'){return ans;}
   throw bad_char;
}
  
string lexer_parser::get_tokens(){
   //for dumping out the token list. For debuging purposus.
   string output = "";
   for(list<syntax_node*>::iterator it = this->token_list.begin();it != token_list.end();it++){
      syntax_node *next_node = *it;
      switch(next_node->type){
         case number:
            output += "(number,"+next_node->value+")\n";
            break;
         case ans:
            output += "(ANS,)\n";
            break;
         case operation:
            output += "(OP ,"+next_node->value + ")\n";
         case END_SEQ:
            output += "(END,)\n";
      }
   }
   return output;
}

syntax_node *lexer_parser::new_node(token_type tok_type,string val){
   //creating a new node is kinda verbos so a method for doing so 
   //is usefull
   syntax_node *new_node = (syntax_node*) malloc(sizeof(syntax_node));
   new_node->type = tok_type;
   new_node->value = val;
   new_node->right_node = NULL;
   new_node->left_node = NULL;
   return new_node;
}

void lexer_parser::free_tree(syntax_node *f_node){
   if(f_node->right_node != NULL){this->free_tree(f_node->right_node);}
   if(f_node->left_node != NULL){this->free_tree(f_node->left_node);}
   free(f_node);
}

lexer_parser::~lexer_parser(){
   this->free_tree(this->tree_stack.back());
}