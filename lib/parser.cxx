#include <model.hpp>

#include <boost/spirit/include/classic.hpp>
#include <boost/config/warning_disable.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/include/support_istream_iterator.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

namespace model {

namespace parser
{
    namespace x3 = boost::spirit::x3;
    namespace ascii = boost::spirit::x3::ascii;

    template <typename Iterator>
    int parse_it(model::Input & i, 
        Iterator first, Iterator last)
    {
      using x3::lit;
      using x3::char_;
      using x3::uint_;
      using x3::_attr;
      using x3::string;
      using x3::phrase_parse;
      using ascii::space;
      int layers, n_shapes, n_vias, n_obst;

      auto viacost_lit = lit("ViaCost");
      auto spacing_lit = lit("Spacing");
      auto boundary_lit = lit("Boundary");
      auto metal_n_lit = lit("#MetalLayers");
      auto rs_n_lit = lit("#RoutedShapes");
      auto rv_n_lit = lit("#RoutedVias");
      auto obs_n_lit = lit("#Obstacles");
      auto rs_lit = lit("RoutedShape");
      auto rv_lit = lit("RoutedVia");
      auto obs_lit = lit("Obstacle");

      R2D temp_rect;
      P2D temp_pt;

      auto set_vc = [&](auto & c){ i.viaCost = _attr(c); };
      auto viacost_decl = viacost_lit >> '=' >> uint_[set_vc] >> x3::eol;

      auto set_sp = [&](auto & c){ i.spacing = _attr(c); };
      auto spacing_decl = spacing_lit >> '=' >> uint_[set_vc] >> x3::eol;

      auto pt_decl
        = x3::rule<class pt_decl, P2D >{}
	= char_('(') >> uint_ >> char(',') >> uint_ >> char_(')');
     

      auto rect_decl
	      =

          /*
      auto set_bd = [&](auto & c){ i.viaCost = _attr(c); };
      auto boundary_decl = 
        boundary_lit >> 
        '=' 
        >> char_('(')
        >> char_(')')
        >> uint_[set_vc] >> x3::eol;

      auto set_m_n = [&](auto & c){ i.viaCost = _attr(c); };
      auto viacost_decl = viacost_lit >> '=' >> uint_[set_vc] >> x3::eol;

      auto set_rs_n = [&](auto & c){ i.viaCost = _attr(c); };
      auto viacost_decl = viacost_lit >> '=' >> uint_[set_vc] >> x3::eol;

      auto set_rv_n = [&](auto & c){ i.viaCost = _attr(c); };
      auto viacost_decl = viacost_lit >> '=' >> uint_[set_vc] >> x3::eol;

      auto set_obs_n = [&](auto & c){ i.viaCost = _attr(c); };
      auto viacost_decl = viacost_lit >> '=' >> uint_[set_vc] >> x3::eol;

      */
      /*
      auto set_function_op = [&](auto &c){ op = _attr(c); };
      auto function_name 
        = x3::rule<class function_name, std::string>{}
        = string("and")   
        | string("nand") 
        | string("or")   
        | string("nor")  
        | string("xor")  
        | string("xnor") 
        | string("not")  
        | string("buf");
      auto function_name_op = function_name[set_function_op];

      auto add_function = [&](auto &c){ v.add_function(op, _attr(c)); };
      auto function_decl 
        = function_name_op > 
        '(' > identifier_list[add_function] > ')' > ';';

      auto module_body 
        = +(input_decl 
          | output_decl 
          | wire_decl 
          | function_decl
          );

      
      auto add_ports = [&](auto &c){ v.add_ports(_attr(c)); };
      
      auto module 
        = module_lit 
          > identifier 
	>> -( '(' >> -identifier_list[add_ports] >> ')' ) > ';'
          >> module_body 
          >> endmodule_lit;
	
	  
      phrase_parse(first, last,
          module >> x3::eoi, comment);
    */
      phrase_parse(first, last,
          viacost_decl >> x3::eoi, space);

      if (first != last) {
        std::cout << "ERROR: PARSE FAILED" << std::endl;
        return -1;
      }

      return 0;
    }

    int parse_file(model::Input &i, char* filename) {
      std::ifstream input(filename);
      input.unsetf(std::ios::skipws);
      boost::spirit::istream_iterator begin(input);
      boost::spirit::istream_iterator end;
      return parse_it(i, begin, end);
    }



}

}
