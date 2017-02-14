#include <parser/parser.h>

#include <cctype>
#include <fstream>
#include <cassert>

namespace Parser {
   
VMT_Driver::~VMT_Driver()
{
   delete(scanner);
   scanner = nullptr;
   delete(parser);
   parser = nullptr;
}

void 
VMT_Driver::parse( const char * const filename )
{
   assert( filename != nullptr );
   std::ifstream in_file( filename );
   if( ! in_file.good() )
   {
       exit( EXIT_FAILURE );
   }
   parse_helper( in_file );
   return;
}

void
VMT_Driver::parse( std::istream &stream )
{
   if( ! stream.good()  && stream.eof() )
   {
       return;
   }
   //else
   parse_helper( stream ); 
   return;
}


void 
VMT_Driver::parse_helper( std::istream &stream )
{
   
   delete(scanner);
   try
   {
      scanner = new VMT_Scanner( &stream );
   }
   catch( std::bad_alloc &ba )
   {
      std::cerr << "Failed to allocate scanner: (" <<
         ba.what() << "), exiting!!\n";
      exit( EXIT_FAILURE );
   }
   
   delete(parser); 
   try
   {
      parser = new VMT_Parser( (*scanner) /* scanner */, 
                                  (*this) /* driver */ );
   }
   catch( std::bad_alloc &ba )
   {
      std::cerr << "Failed to allocate parser: (" << 
         ba.what() << "), exiting!!\n";
      exit( EXIT_FAILURE );
   }
   const int accept( 0 );
   if( parser->parse() != accept )
   {
      std::cerr << "Parse failed!!\n";
   }
   return;
}

ast
VMT_Driver::mk_var(std::string symbol) {

    if(var_map.find(symbol) == var_map.end()) {
        ast node = new ast_node;
        node->add_name(symbol); node->node_type(SYM);
        var_map[symbol] = node;
        return node;
    }
    else {
        return var_map[symbol];
    }

}

ast
VMT_Driver::mk_or(std::vector<ast> operands) {

    ast node = new ast_node;
    node->node_type(OR);
    for(unsigned int i = 0 ; i < operands.size() ; i++)
        node->add_operand(operands[i]);
    return node;
}

ast
VMT_Driver::mk_and(std::vector<ast> operands) {

    ast node = new ast_node;
    node->node_type(AND);
    for(unsigned int i = 0 ; i < operands.size() ; i++)
        node->add_operand(operands[i]);
    return node;
}

ast
VMT_Driver::mk_not(std::vector<ast> operands) {

    ast node = new ast_node;
    node->node_type(NOT);
    for(unsigned int i = 0 ; i < operands.size() ; i++)
        node->add_operand(operands[i]);
    return node;
}

ast
VMT_Driver::mk_eq(std::vector<ast> operands) {

    ast node = new ast_node;
    node->node_type(EQ);
    for(unsigned int i = 0 ; i < operands.size() ; i++)
        node->add_operand(operands[i]);
    return node;
}

ast
VMT_Driver::get_ast(std::string str) {
    return ast_map[str];
}

void
VMT_Driver::add_definition(std::string str, ast a) {
    ast_map[str] = a;
}

std::map<std::string, ast>
VMT_Driver::get_var_map() {
    return var_map;
}

std::map<std::string, ast>
VMT_Driver::get_def_map() {
    return ast_map;
}

}
