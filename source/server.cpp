#include "common.hpp"
#include "server.hpp"

#include "trace/Trace.hpp"
#define CLASS_ABBR "SERVER"



namespace server {

   void thread_loop( int client_socket )
   {
      MSG_INF( "client thread enter: %d", client_socket );

      const size_t buffer_size = common::default_values::message_max_size;
      char buffer[ buffer_size ];
      while( true )
      {
         // Receive message from client
         ssize_t recv_size = recv( client_socket, &buffer, buffer_size - 1, 0 );
         if( 0 >= recv_size )
         {
            int error = errno;
            if( EAGAIN == error )
            {
               MSG_ERR( "recv(%d) error: %d", client_socket, error );
               continue;
            }
            else
            {
               MSG_INF( "client disconnected: %d", client_socket );
               break;
            }
         }
         buffer[ recv_size ] = 0;
         MSG_DBG( "recv(%d): %zd bytes", client_socket, recv_size );
         MSG_VRB( "received message: '%s'", buffer );

         // Send message to client
         ssize_t send_size = send( client_socket, buffer, recv_size, 0 );
         if( -1 == send_size )
         {
            int error = errno;
            MSG_ERR( "send(%d) error: %d", client_socket, error );
            break;
         }
         MSG_DBG( "send(%d) success: %zd bytes", client_socket, send_size, buffer );
         MSG_VRB( "sent message: '%s'", buffer );
      }

      close( client_socket );

      MSG_INF( "client thread exit: %d", client_socket );
   }

   int run( const base::Parameters::tSptr parameters )
   {
      const int socket_family = common::string_to_socket_family(
            parameters->value_or< std::string >( "family", common::default_values::socket_family ).first
         );
      const int socket_type = common::string_to_socket_type(
            parameters->value_or< std::string >( "type", common::default_values::socket_type ).first
         );
      const int socket_protocole = parameters->value_or< int >(
            "protocole", common::default_values::socket_protocole
         ).first;

      int master_socket = socket( socket_family, socket_type, socket_protocole );
      if( -1 == master_socket )
      {
         int error = errno;
         MSG_ERR( "socket(%d, %d, %d) error: %d", socket_family, socket_type, socket_protocole, error );
         return error;
      }
      MSG_DBG( "socket(%d) success", master_socket );

      int result_bind = -1;
      if( AF_UNIX == socket_family )
      {
         // Here "address" must be a string not "char*" to avoid pointer to destroyed temporary object
         std::string address = parameters->value_or< std::string >(
               "address", common::default_values::address_unix
            ).first;

         struct sockaddr_un addr_un;
         memset( &addr_un, 0, sizeof( sockaddr_un ) );
         addr_un.sun_family = socket_family;
         strncpy( addr_un.sun_path, address.c_str( ), sizeof(addr_un.sun_path) - 1 );
         unlink( address.c_str( ) );
         int addr_un_size = sizeof( addr_un.sun_family ) + strlen( addr_un.sun_path );

         result_bind = bind( master_socket, (sockaddr*)(&addr_un), addr_un_size );
      }
      else if( AF_INET == socket_family )
      {
         uint32_t address = htonl( INADDR_ANY );
         const std::string address_string = parameters->value_or< std::string >( "address", "" ).first;
         if( !address_string.empty( ) )
         {
            address = inet_addr( address_string.c_str( ) );
         }
         const int port = parameters->value_or< int >( "port", common::default_values::port ).first;

         struct sockaddr_in addr_in;
         memset( &addr_in, 0, sizeof( sockaddr_in ) );
         addr_in.sin_family = socket_family;
         addr_in.sin_addr.s_addr = address;
         addr_in.sin_port = htons( port );
         int addr_in_size = sizeof( addr_in );

         result_bind = bind( master_socket, (sockaddr*)(&addr_in), addr_in_size );
      }
      else if( AF_VSOCK == socket_family )
      {
         const int address = parameters->value_or< int >( "address", common::default_values::address_vsock ).first;
         const int port = parameters->value_or< int >( "port", common::default_values::port ).first;

         struct sockaddr_vm addr_vm;
         memset( &addr_vm, 0, sizeof( sockaddr_vm ) );
         addr_vm.svm_family = socket_family;
         addr_vm.svm_cid = address;
         addr_vm.svm_port = port;
         int addr_vm_size = sizeof( addr_vm );

         result_bind = bind( master_socket, (sockaddr*)&addr_vm, addr_vm_size );
      }



      if( -1 == result_bind )
      {
         int error = errno;
         MSG_ERR( "bind(%d) error: %d", master_socket, error );
         return error;
      }
      MSG_DBG( "bind(%d) success", master_socket );

      if( -1 == listen( master_socket, 0 ) )
      {
         int error = errno;
         MSG_ERR( "listen(%d) error: %d", master_socket, error );
         return error;
      }
      MSG_DBG( "listen(%d) success", master_socket );

      struct sockaddr_vm client_addr;
      socklen_t client_addr_size = sizeof( struct sockaddr_vm );



      std::vector< std::thread > client_threads;
      while( true )
      {
         int client_socket = accept( master_socket, (sockaddr*)&client_addr, &client_addr_size );
         if( -1 == client_socket )
         {
            int error = errno;
            MSG_ERR( "accept(%d) error: %d", master_socket, error );
            continue;
         }
         MSG_DBG( "accept(%d) success: %d", master_socket, client_socket );

         client_threads.push_back( std::thread{ thread_loop, client_socket } );
      }

      for( auto& item: client_threads )
      {
         item.join( );
      }

      close( master_socket );

      return 0;
   }

}
