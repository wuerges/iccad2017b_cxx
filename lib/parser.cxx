#include <model.hpp>

#include <boost/spirit/include/classic.hpp>
#include <boost/config/warning_disable.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/include/support_istream_iterator.hpp>
#include <boost/fusion/include/at.hpp>

#include <utility>
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
      using x3::lexeme;
      using ascii::space;
      int layers, n_shapes, n_vias, n_obst;

      auto set_vc = [&](auto & c){ i.viaCost = _attr(c); };
      auto viacost_decl = lit("ViaCost") >> '=' >> uint_[set_vc];

      auto set_sp = [&](auto & c){ i.spacing = _attr(c); };
      auto spacing_decl = lit("Spacing") >> '=' >> uint_[set_sp];

      auto pt_decl
        = x3::rule<struct _, std::vector<unsigned int> >{}
	= '(' > uint_ > ',' > uint_ > ')';
     
      auto rect_decl
        = x3::rule<struct _, std::vector<unsigned int> >{}
        = pt_decl >> pt_decl;


      auto boundary_decl_set = [&](auto & c) { 
	      i.boundary = _attr(c);
      };
      auto boundary_decl
        = lit("Boundary") >> '=' >> rect_decl[boundary_decl_set];
	

      auto set_metal_n = [&](auto & c){ 
	      layers = _attr(c); 
	      i.shapes.resize(layers);
	      i.vias.resize(layers);
	      i.obstacles.resize(layers);
      };
      auto metal_n_decl = 
	      lit("#MetalLayers") >> '=' >> uint_[set_metal_n];

      auto set_rs_n = [&](auto & c){ n_shapes = _attr(c); };
      auto rs_n_decl = 
	      lit("#RoutedShapes") >> '=' >> uint_[set_rs_n];

      auto set_rv_n = [&](auto & c){ n_vias = _attr(c); };
      auto rv_n_decl = 
	      lit("#RoutedVias") >> '=' >> uint_[set_rv_n];

      auto set_obs_n = [&](auto & c){ n_obst = _attr(c); };
      auto obs_n_decl = 
	      lit("#Obstacles") >> '=' >> uint_[set_obs_n];


      int curr_layer;
      auto set_l_id = [&](auto & c) { curr_layer = _attr(c); };
      auto layer_id = lexeme[(lit('M') | 'V') > uint_[set_l_id]];

      auto set_rs_decl = [&](auto & c){ 
	      i.shapes[curr_layer-1].push_back(_attr(c));
      };
      auto rs_decl = lit("RoutedShape") >> layer_id >> rect_decl[set_rs_decl];

      auto set_rv_decl = [&](auto & c){ 
	      i.vias[curr_layer-1].push_back(_attr(c));
      };
      auto rv_decl = lit("RoutedVia") >> layer_id >> pt_decl[set_rv_decl];

      auto set_obs_decl = [&](auto & c){ 
	      i.obstacles[curr_layer-1].push_back(_attr(c));
      };
      auto obs_decl = lit("Obstacle") >> layer_id >> rect_decl[set_obs_decl];


      auto decl = 
	      viacost_decl |
	      spacing_decl | 
	      boundary_decl | 
	      metal_n_decl | 
	      rs_n_decl | 
	      rv_n_decl | 
	      obs_n_decl |
	      rs_decl | 
	      rv_decl |
	      obs_decl;


      auto input_decl = +decl;

      phrase_parse(first, last,
          input_decl >> x3::eoi, space);

      if (first != last) {
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
