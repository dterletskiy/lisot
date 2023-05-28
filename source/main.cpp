#include "server.hpp"
#include "client.hpp"
#include "common.hpp"



base::Parameters::tMap default_parameters
{
   { "family", common::default_values::socket_family },
   { "type", common::default_values::socket_type },
   { "protocole", std::to_string( common::default_values::socket_protocole ) },
};



int main( int argc, char* argv[ ] )
{
   base::Parameters::tSptr parameters = base::Parameters::create( argc, argv, default_parameters );
   if( not parameters )
   {
      printf( "%s\n", common::info );
      return 255;
   }
   parameters->dump( );


   const bool is_server = parameters->is_exist( "server" );
   const bool is_client = parameters->is_exist( "client" );

   if( is_server and is_client )
   {
      printf( "Only one of parameters must be defined: 'clent' or 'server'\n" );
      printf( "%s\n", common::info );
   }
   else if( not is_server and not is_client )
   {
      printf( "One of parameters must be defined: 'clent' or 'server'\n" );
      printf( "%s\n", common::info );
   }
   else if( is_server )
   {
      server::run( parameters );
   }
   else if( is_client )
   {
      client::run( parameters );
   }

   return 0;
}
