/*************************************************************************
 * Copyright (C) 2017 by Rohit Dureja                                    *
 *                                                                       *
 * This file is part of SimplePDR.                                       *
 *                                                                       *
 *  SimplePDR is free software: you can redistribute it and/or modify    *
 *  it under the terms of the GNU General Public License as published by *
 *  the Free Software Foundation, either version 3 of the License, or    *
 *  (at your option) any later version.                                  *
 *                                                                       *
 *  SimplePDR is distributed in the hope that it will be useful,         *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 *  GNU General Public License for more details.                         *
 *                                                                       *
 *  You should have received a copy of the GNU General Public License    *
 *  along with SimplePDR.  If not, see <http://www.gnu.org/licenses/>.   *
 *************************************************************************/

#include <memory>

#include <model/model.h>
#include <pdr/IC3.h>
#include <parser/parser.h>

void generate_string(Parser::ast a) {
    if(a->get_ast_type() == Parser::AND) {
//        std::cout << "and" << std::endl;
//        std::cout << a->get_operands().size() << std::endl;
        std::cout << " (and";
        for(unsigned int i = 0 ; i < a->get_operands().size() ; ++i) {
            generate_string(a->get_operands()[i]);
        }
        std::cout << ")";
    }
    else if(a->get_ast_type() == Parser::OR) {
        std::cout << " (or";
        for(unsigned int i = 0 ; i < a->get_operands().size() ; ++i) {
                    generate_string(a->get_operands()[i]);
                }
        }
    else if(a->get_ast_type() == Parser::NOT) {
//        std::cout << "not" << std::endl;
//                std::cout << a->get_operands().size() << std::endl;
        std::cout << " (not";
        for(unsigned int i = 0 ; i < a->get_operands().size() ; ++i) {
                    generate_string(a->get_operands()[i]);
                }
        std::cout << ")";
        }
    else if(a->get_ast_type() == Parser::EQ) {
        std::cout << " (=";
            for(unsigned int i = 0 ; i < a->get_operands().size() ; ++i) {
                        generate_string(a->get_operands()[i]);
                    }
            std::cout << ")";
            }
    else if (a->get_ast_type() == Parser::SYM) {
            std::cout << " " << a->get_ast_string() << "";
        }
    return;
}

int
main( const int argc, const char **argv ) {


    // Specify transition system
//    std::shared_ptr<Model::Model> M(new Model::Model());

//    /******Model 1******/
//    // state variables
//    M->add_variable("a");
//    M->add_variable("b");
//    M->add_variable("na");
//    M->add_variable("nb");
//
//    // add current and next state variable relations
//    M->add_variable_relation("a", "na");
//    M->add_variable_relation("b", "nb");
//
//    // initial states
//    M->add_clause(Model::I, "!a");
//    M->add_clause(Model::I, "!b");
//
//    // transition relations
//    M->add_clause(Model::T, "a||!b||nb");
//    M->add_clause(Model::T, "a||b||!na");
//    M->add_clause(Model::T, "!a||na");
//    M->add_clause(Model::T, "!a||!nb");
//    M->add_clause(Model::T, "b||!nb");
//
//    // safety property
//    M->add_clause(Model::P, "!a||b");

//    /******Model 2******/
//    // state variables
//    M->add_variable("a");
//    M->add_variable("b");
//    M->add_variable("c");
//    M->add_variable("na");
//    M->add_variable("nb");
//    M->add_variable("nc");
//
//    // add current and next state variable relations
//    M->add_variable_relation("a", "na");
//    M->add_variable_relation("b", "nb");
//    M->add_variable_relation("c", "nc");
//
//    // initial states
//    M->add_clause(Model::I, "!a");
//    M->add_clause(Model::I, "!b");
//    M->add_clause(Model::I, "!c");
//
//    // transition relations
//    M->add_clause(Model::T,  "a||!nb");
//    M->add_clause(Model::T, "!a||nb");
//    M->add_clause(Model::T,  "b||!nc");
//    M->add_clause(Model::T, "!b||nc");
//
//    // safety property
//    M->add_clause(Model::P, "!a||!b||!c");

//    /******Model 3******/
//    // state variables
//    M->add_variable("a");
//    M->add_variable("b");
//    M->add_variable("c");
//    M->add_variable("na");
//    M->add_variable("nb");
//    M->add_variable("nc");
//
//    // add current and next state variable relations
//    M->add_variable_relation("a", "na");
//    M->add_variable_relation("b", "nb");
//    M->add_variable_relation("c", "nc");
//
//    // initial states
//    M->add_clause(Model::I, "!a");
//    M->add_clause(Model::I, "!b");
//    M->add_clause(Model::I, "!c");
//
//    // transition relations
//    M->add_clause(Model::T,  "!a||!b||nb");
//    M->add_clause(Model::T,  "!a||b||!nb");
//    M->add_clause(Model::T,  "a||b||!c||nb");
//    M->add_clause(Model::T,  "!b||!na||nb");
//    M->add_clause(Model::T,  "!b||nb||nc");
//    M->add_clause(Model::T,  "b||c||!nb");
//    M->add_clause(Model::T,  "b||na");
//    M->add_clause(Model::T,  "na||!nb");
//
//    // safety property
//    M->add_clause(Model::P, "!a||!b");
//
//// Create IC3 instance
//    std::shared_ptr<IC3::IC3> ic3_instance(new IC3::IC3(M));
//
//    // Run the IC3 algorithm
//    bool res = ic3_instance->prove();
//    if (res == true)
//        std::cout << "UNSAT" << std::endl;
//    else
//        std::cout << "SAT" << std::endl;

// Clean up

//    return 0;
    /** check for the right # of arguments **/
       if( argc == 2 )
       {
          Parser::VMT_Driver driver;
          /** example for piping input from terminal, i.e., using cat **/
          if( std::strncmp( argv[ 1 ], "-o", 2 ) == 0 )
          {
             driver.parse( std::cin );
          }
          /** simple help menu **/
          else if( std::strncmp( argv[ 1 ], "-h", 2 ) == 0 )
          {
             std::cout << "use -o for pipe to std::cin\n";
             std::cout << "just give a filename to count from a file\n";
             std::cout << "use -h to get this menu\n";
             return( EXIT_SUCCESS );
          }
          /** example reading input from a file **/
          else
          {
             /** assume file, prod code, use stat to check **/
             driver.parse( argv[1] );
             /* file parsed successfully  */
//             std::cout << driver.get_var_map().size() << std::endl;
////             for(unsigned int i = 0 ; i < driver.get_def_map().size() ; ++i)
////             {
//
////                 std::map<std::string, Parser::ast> m = driver.get_def_map();
//////                 for(std::map<std::string,Parser::ast>::iterator it = m.begin(); it != m.end(); ++it) {
//////                     std::cout << "here";
//////                     std::cout << it->first << "\n";
//////                 }
////                 std::string ex = ".def_27";
////
////                 Parser::ast a = m[ex];
////                 generate_string(a);
//
                 std::map<Parser::ast, Parser::ast> cn = driver.get_curr_next();
                 for(std::map<Parser::ast,Parser::ast>::iterator it = cn.begin(); it != cn.end(); ++it) {
                                      Parser::ast curr = it->first;
                                      Parser::ast next = cn[it->first];
                                      generate_string(curr);
                                      generate_string(next);
                  }

                 std::cout << "\n\n";
                 generate_string(driver.trans);

                 std::cout << "\n\n";
                 generate_string(driver.init);



//                 std::cout << a->get_ast_string() << std::endl;

//             }
//             std::cout << driver.get_def_map().size() << std::endl;
          }

       }
       else
       {
          /** exit with failure condition **/
          return ( EXIT_FAILURE );
       }


       return( EXIT_SUCCESS );
}

// 2017-01-31
