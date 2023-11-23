#pragma once

#include <thread>

#include "parameters.hpp"



namespace server {

   int run( const base::Parameters::tSptr parameters );

}

class Server
{
   public:
      Server( base::Parameters::tSptr parameters );
      ~Server( );

      bool create( );
      bool bind( );
      bool listen( );
      int accept( );
      bool select( );
      bool send( int client_socket, const char* buffer, const size_t size );
      bool recv( int client_socket, char* buffer, const size_t size );

      bool run( );

   private:
      static void thread_loop( int client_socket );
      bool build_message( std::string& message );

   private:
      base::Parameters::tSptr m_parameters;
      int m_socket_family     = -1;
      int m_socket_type       = -1;
      int m_socket_protocole  = -1;

      size_t m_count          = 0;
      uint64_t m_send_time    = 0;
      uint64_t m_recv_time    = 0;

      int m_error             = -1;

      int m_master_socket     = -1;

      int m_max_socket        = -1;
      lisot::base::fds        m_fds;

      std::set< int >            m_client_sockets;
      std::vector< std::thread > m_client_threads;
};
