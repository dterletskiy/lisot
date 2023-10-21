#include "server.hpp"
#include "client.hpp"
#include "common.hpp"

#include "trace/Trace.hpp"
#define CLASS_ABBR "MAIN"



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


   const trace::eLogStrategy trace_strategy = trace::log_strategy_from_string(
         parameters->value_or( "trace_log", std::string{ "CONSOLE" } ).first.c_str( )
      );
   const std::size_t trace_buffer = parameters->value_or< std::size_t >( "trace_buffer", 4096 ).first;
   const std::string trace_app_name = parameters->value_or( "trace_app_name", std::string{ "LISOT" } ).first;
   const trace::eLogLevel trace_level = trace::log_level_from_number(
         parameters->value_or< size_t >( "trace_level", 6 ).first
      );
   trace::Logger::init( trace_strategy, trace_app_name, trace_buffer, trace_level );



   if( parameters->is_exist( "info" ) )
   {
      // http://web.archive.org/web/20090812134009/http://tuxology.net/tag/interface

      struct ifaddrs *addrs;
      getifaddrs( &addrs );

      for( struct ifaddrs *addr = addrs; nullptr != addr; addr = addr->ifa_next )
      {
         if( addr->ifa_addr && AF_PACKET == addr->ifa_addr->sa_family )
         {
            MSG_DBG( "addr->ifa_name: %s", addr->ifa_name );
            if( addr->ifa_addr )
            {
               MSG_VRB( "addr->ifa_addr->sa_family: %u", addr->ifa_addr->sa_family );
               MSG_VRB( "addr->ifa_addr->sa_data: %s", addr->ifa_addr->sa_data );
            }
            MSG_VRB( );
         }
      }

      freeifaddrs( addrs );

      return 0;
   }

   const bool is_server = parameters->is_exist( "server" );
   const bool is_client = parameters->is_exist( "client" );

   if( is_server and is_client )
   {
      MSG_ERR( "Only one of parameters must be defined: 'clent' or 'server'" );
      printf( "%s\n", common::info );
   }
   else if( not is_server and not is_client )
   {
      MSG_ERR( "One of parameters must be defined: 'clent' or 'server'" );
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
