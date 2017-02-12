#ifndef __MCSCANNER_HPP__
#define __MCSCANNER_HPP__ 1

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include <parser.tab.hh>
#include <location.hh>

namespace Parser{

class VMT_Scanner : public yyFlexLexer{
public:
   
   VMT_Scanner(std::istream *in) : yyFlexLexer(in)
   {
      loc = new Parser::VMT_Parser::location_type();
   };
   virtual ~VMT_Scanner() {
      delete loc;
   };

   //get rid of override virtual function warning
   using FlexLexer::yylex;

   virtual
   int yylex( Parser::VMT_Parser::semantic_type * const lval, 
              Parser::VMT_Parser::location_type *location );
   // YY_DECL defined in mc_lexer.l
   // Method body created by flex in mc_lexer.yy.cc


private:
   /* yyval ptr */
   Parser::VMT_Parser::semantic_type *yylval = nullptr;
   /* location ptr */
   Parser::VMT_Parser::location_type *loc    = nullptr;
};

} /* end namespace MC */

#endif /* END __MCSCANNER_HPP__ */
