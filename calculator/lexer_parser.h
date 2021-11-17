#include <string>
#include <list>
using namespace std;
enum token_type{number,operation,END_SEQ,ans};
enum lex_parse_error{No_prev_oprand_for_op,No_next_oprand_for_op,bad_char,oprand_not_found};

struct syntax_node{
   token_type type;
   string value;
   syntax_node *left_node;
   syntax_node *right_node;
};

class lexer_parser{
   
   list<syntax_node*> token_list;
   list<syntax_node*> tree_stack;
   string current_val;
   
   public:
   lexer_parser();
   syntax_node *parse(string input);
   string get_tokens();
   string get_tree();
   ~lexer_parser();
   private:
   void push_char(char a_char);
   void lex(string input);
   string visit_tree(syntax_node *snode,string loc,string acm);
   token_type get_type(char a_char);
   syntax_node *new_node(token_type tok_type,string val);
   void free_tree(syntax_node *f_node);
};
