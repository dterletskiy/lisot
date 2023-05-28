#include "common.hpp"
#include "server.hpp"



namespace server {

   void thread_loop( int client_socket )
   {
      printf( "client thread enter: %d\n", client_socket );

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
               printf( "'recv' error: %d\n", error );
               continue;
            }
            else
            {
               printf( "client disconnected: %d\n", client_socket );
               break;
            }
         }
         buffer[ recv_size ] = 0;
         printf( "'recv' success: %ld bytes / '%s'\n", recv_size, buffer );

         // Send message to client
         ssize_t send_size = send( client_socket, buffer, recv_size, 0 );
         if( -1 == send_size )
         {
            int error = errno;
            printf( "'send' error: %d\n", error );
            break;
         }
         printf( "'send' success: %ld bytes / '%s'\n", send_size, buffer );
      }

      close( client_socket );

      printf( "client thread exit: %d\n", client_socket );
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
         printf( "'socket' error: %d\n", error );
         return error;
      }
      printf( "'socket' success: %d\n", master_socket );

      int result_bind = -1;
      if( AF_UNIX == socket_family )
      {
         const char* address = parameters->value_or< std::string >(
               "address", common::default_values::address_unix
            ).first.c_str( );

         struct sockaddr_un addr_un;
         memset( &addr_un, 0, sizeof( sockaddr_un ) );
         addr_un.sun_family = socket_family;
         strncpy( addr_un.sun_path, address, sizeof(addr_un.sun_path) - 1 );
         unlink( address );
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
         printf( "'bind' error: %d\n", error );
         return error;
      }
      printf( "'bind' success\n" );

      if( -1 == listen( master_socket, 0 ) )
      {
         int error = errno;
         printf( "'listen' error: %d\n", error );
         return error;
      }
      printf( "'listen' success\n" );

      struct sockaddr_vm client_addr;
      socklen_t client_addr_size = sizeof( struct sockaddr_vm );



      std::vector< std::thread > client_threads;
      while( true )
      {
         int client_socket = accept( master_socket, (sockaddr*)&client_addr, &client_addr_size );
         if( -1 == client_socket )
         {
            int error = errno;
            printf( "'accept' error: %d\n", error );
            continue;
         }
         printf( "'accept' success: %d\n", client_socket );

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
