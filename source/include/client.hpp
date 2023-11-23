#pragma once

#include "parameters.hpp"



namespace client {

   int run( const base::Parameters::tSptr parameters );

}

namespace result {

   struct Send
   {
      bool     result = false;
      uint64_t timestamp = 0;
      ssize_t  size = 0;
      int      error = -1;
   };

}

class Client
{
   public:
      Client( base::Parameters::tSptr parameters );
      ~Client( );

      bool create( );
      bool connect( );
      bool send( const char* buffer, const size_t size );
      bool recv( char* buffer, const size_t size );

      bool run( );

   private:
      bool build_message( std::string& message );

   private:
      base::Parameters::tSptr m_parameters;
      int m_error = -1;
      int m_socket = -1;
      int m_socket_family = -1;
      int m_socket_type = -1;
      int m_socket_protocole = -1;

      size_t m_count       = 0;
      uint64_t m_send_time = 0;
      uint64_t m_recv_time = 0;
};
