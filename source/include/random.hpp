#pragma once

#include <cstdlib>
#include <string>



namespace lisot::random {

   void init( );
   std::string text( size_t length );
   size_t number( size_t begin, size_t end );

}
