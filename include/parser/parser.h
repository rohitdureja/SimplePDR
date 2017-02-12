#ifndef __MCDRIVER_HPP__
#define __MCDRIVER_HPP__ 1

#include <string>
#include <cstddef>
#include <istream>

#include <parser.tab.hh>
#include "scanner.h"

namespace Parser{

class VMT_Driver{
public:
   VMT_Driver() {
    
   }

   virtual ~VMT_Driver();
   
   /** 
    * parse - parse from a file
    * @param filename - valid string with input file
    */
   void parse( const char * const filename );
   /** 
    * parse - parse from a c++ input stream
    * @param is - std::istream&, valid input stream
    */
   void parse( std::istream &iss );

private:

   void parse_helper( std::istream &stream );

   Parser::VMT_Parser  *parser  = nullptr;
   Parser::VMT_Scanner *scanner = nullptr;

};
} /* end namespace MC */
#endif /* END __MCDRIVER_HPP__ */
