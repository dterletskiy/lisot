#include <unistd.h>

#include "random.hpp"



namespace lisot::random {

   void init( )
   {
      static bool initialized = false;
      if( not initialized )
      {
         srand( (unsigned)time( nullptr ) * getpid( ) );
         initialized = true;
      }
   }

   std::string text( size_t length )
   {
      init( );

      static const char alphanum[ ] =
         "0123456789"
         "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
         "abcdefghijklmnopqrstuvwxyz";
      std::string tmp_s;
      tmp_s.reserve( length );

      for( size_t i = 0; i < length; ++i )
      {
         tmp_s += alphanum[ rand( ) % ( sizeof( alphanum ) - 1 ) ];
      }

      return tmp_s;
   }

   size_t number( size_t begin, size_t end )
   {
      init( );

      return begin + ( rand( ) % ( end - begin + 1 ) );
   }

}