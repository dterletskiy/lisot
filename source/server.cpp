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
      switch( socket_family )
      {
         case AF_UNIX:
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

            break;
         }
         case AF_INET:
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

            break;
         }
         case AF_VSOCK:
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

            break;
         }
         default:
         {
            MSG_ERR( "Unsuported family pretocole" );
         }
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



Server::Server( base::Parameters::tSptr parameters )
   : m_parameters( parameters )
{
   m_socket_family = common::string_to_socket_family(
         m_parameters->value_or< std::string >( "family", common::default_values::socket_family ).first
      );
   m_socket_type = common::string_to_socket_type(
         m_parameters->value_or< std::string >( "type", common::default_values::socket_type ).first
      );
   m_socket_protocole = m_parameters->value_or< int >(
         "protocole", common::default_values::socket_protocole
      ).first;
}

Server::~Server( )
{
   shutdown( m_master_socket, SHUT_RDWR );
   close( m_master_socket );
}

bool Server::create( )
{
   int master_socket = ::socket( m_socket_family, m_socket_type, m_socket_protocole );
   if( -1 == master_socket )
   {
      m_error = errno;
      MSG_ERR( "socket(%d, %d, %d) error: %d", m_socket_family, m_socket_type, m_socket_protocole, m_error );
      return false;
   }
   MSG_DBG( "socket(%d) success", master_socket );

   return true;
}

bool Server::bind( )
{
   int result_bind = -1;
   switch( m_socket_family )
   {
      case AF_UNIX:
      {
         // Here "address" must be a string not "char*" to avoid pointer to destroyed temporary object
         std::string address = m_parameters->value_or< std::string >(
               "address", common::default_values::address_unix
            ).first;

         struct sockaddr_un addr_un;
         memset( &addr_un, 0, sizeof( sockaddr_un ) );
         addr_un.sun_family = m_socket_family;
         strncpy( addr_un.sun_path, address.c_str( ), sizeof(addr_un.sun_path) - 1 );
         unlink( address.c_str( ) );
         int addr_un_size = sizeof( addr_un.sun_family ) + strlen( addr_un.sun_path );

         result_bind = ::bind( m_master_socket, (sockaddr*)(&addr_un), addr_un_size );

         break;
      }
      case AF_INET:
      {
         uint32_t address = htonl( INADDR_ANY );
         const std::string address_string = m_parameters->value_or< std::string >( "address", "" ).first;
         if( !address_string.empty( ) )
         {
            address = inet_addr( address_string.c_str( ) );
         }
         const int port = m_parameters->value_or< int >( "port", common::default_values::port ).first;

         struct sockaddr_in addr_in;
         memset( &addr_in, 0, sizeof( sockaddr_in ) );
         addr_in.sin_family = m_socket_family;
         addr_in.sin_addr.s_addr = address;
         addr_in.sin_port = htons( port );
         int addr_in_size = sizeof( addr_in );

         // To reuse current address for bind in case of previously terminated application
         // https://youtu.be/EuSlYET_U0Y?list=PLT1RFoTVtIAzvUWekDdl_e_rX4pnVluG9&t=2664
         int optval = 1;
         if( -1 == ::setsockopt( m_master_socket, SOL_SOCKET, SO_REUSEADDR, (void *)&optval, sizeof( optval ) ) )
         {
            m_error = errno;
            MSG_ERR( "setsockopt(%d) error: %d", m_master_socket, m_error );
            return false;
         }
         MSG_DBG( "setsockopt(%d) success", m_master_socket );

         result_bind = ::bind( m_master_socket, (sockaddr*)(&addr_in), addr_in_size );

         break;
      }
      case AF_VSOCK:
      {
         const int address = m_parameters->value_or< int >( "address", common::default_values::address_vsock ).first;
         const int port = m_parameters->value_or< int >( "port", common::default_values::port ).first;

         struct sockaddr_vm addr_vm;
         memset( &addr_vm, 0, sizeof( sockaddr_vm ) );
         addr_vm.svm_family = m_socket_family;
         addr_vm.svm_cid = address;
         addr_vm.svm_port = port;
         int addr_vm_size = sizeof( addr_vm );

         result_bind = ::bind( m_master_socket, (sockaddr*)&addr_vm, addr_vm_size );

         break;
      }
      default:
      {
         MSG_ERR( "Unsuported family pretocole" );
      }
   }

   if( -1 == result_bind )
   {
      m_error = errno;
      MSG_ERR( "bind(%d) error: %d", m_master_socket, m_error );
      return false;
   }
   MSG_DBG( "bind(%d) success", m_master_socket );

   return true;
}

bool Server::listen( )
{
   if( -1 == ::listen( m_master_socket, SOMAXCONN ) )
   {
      m_error = errno;
      MSG_ERR( "listen(%d) error: %d", m_master_socket, m_error );
      return false;
   }
   MSG_DBG( "listen(%d) success", m_master_socket );

   return true;
}

int Server::accept( )
{
   struct sockaddr_vm client_addr;
   socklen_t client_addr_size = sizeof( struct sockaddr_vm );

   int client_socket = ::accept( m_master_socket, (sockaddr*)&client_addr, &client_addr_size );
   if( -1 == client_socket )
   {
      m_error = errno;
      MSG_ERR( "accept(%d) error: %d", m_master_socket, m_error );
      return -1;
   }
   MSG_DBG( "accept(%d) success: %d", m_master_socket, client_socket );

   return client_socket;
}

bool Server::select( )
{
   if( -1 == ::select( m_max_socket, m_fds.read( ), m_fds.write( ), m_fds.except( ) ) )
   {
      m_error = errno;
      MSG_ERR( "select(%d) error: %d", m_max_socket, m_error );
      return false;
   }
   MSG_DBG( "select(%d) success", m_max_socket );

   return true;
}

bool Server::send( int client_socket, const char* buffer, const size_t size )
{
   ssize_t send_size = ::send( client_socket, buffer, size, 0 );
   m_send_time = time( trace::eGranularity::microseconds );
   if( -1 == send_size )
   {
      m_error = errno;
      MSG_ERR( "send(%d) error: %d", client_socket, m_error );
      return false;
   }
   MSG_DBG( "send(%d) success: %zd bytes", client_socket, send_size );

   return true;
}

bool Server::recv( int client_socket, char* buffer, const size_t size )
{
   ssize_t recv_size = ::recv( client_socket, buffer, size, 0 );
   m_recv_time = time( trace::eGranularity::microseconds );
   if( 0 >= recv_size )
   {
      m_error = errno;
      if( EAGAIN == m_error )
      {
         MSG_ERR( "recv(%d) error: %d", client_socket, m_error );
         // @TDA: ToDo
         return true;
      }
      else
      {
         MSG_INF( "server disconnected" );
         return false;
      }
   }
   MSG_DBG( "recv(%d): %zd bytes", client_socket, recv_size );

   return true;
}

bool Server::run( )
{
   cretae( );
   bind( );
   listen( );

   while( true )
   {
      fd_init( );
      if( false == select( ) )
         break;
   }


   return true;
}

bool Server::build_message( std::string& message )
{
   return true;
}

void Server::thread_loop( int client_socket )
{
   MSG_INF( "client thread enter: %d", client_socket );

   const size_t buffer_size = common::default_values::message_max_size;
   char buffer[ buffer_size ];
   while( true )
   {
      // Receive message from client
      ssize_t recv_size = recv( client_socket, &buffer, buffer_size - 1 );
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
      ssize_t send_size = send( client_socket, buffer, recv_size );
      if( -1 == send_size )
      {
         int error = errno;
         MSG_ERR( "send(%d) error: %d", client_socket, error );
         break;
      }
      MSG_DBG( "send(%d) success: %zd bytes", client_socket, send_size, buffer );
      MSG_VRB( "sent message: '%s'", buffer );
   }

   shutdown( client_socket, SHUT_RDWR );
   close( client_socket );

   MSG_INF( "client thread exit: %d", client_socket );
}

void SocketServer::fd_init( )
{
   m_fds.reset( );

   m_fds.set( m_master_socket, lisot::base::fds::eType::READ );
   m_max_socket = m_master_socket;
   for( const auto& client_sockets : m_client_sockets )
   {
      m_fds.set( client_sockets, lisot::base::fds::eType::READ );
      if( client_sockets > m_max_socket )
         m_max_socket = client_sockets;
   }
}

void SocketServer::fd_process( )
{
   for( auto iterator = m_client_sockets.begin( ); iterator != m_client_sockets.end( ); ++iterator )
   {
      if( false == m_fd.is_set( (*iterator)->socket( ), os_linux::socket::fds::eType::READ ) )
         continue;

      const eResult result = (*iterator)->recv( );
      if( eResult::DISCONNECTED == result )
      {
         (*iterator)->info( "Host disconnected" );
         m_consumer.disconnected( *iterator );
         iterator = m_client_sockets.erase( iterator );
         if( m_client_sockets.end( ) == iterator )
            break;
      }
      else if( eResult::OK == result )
      {
         m_consumer.read_slave( *iterator );
      }
   }

   if( true == m_fd.is_set( socket( ), os_linux::socket::fds::eType::READ ) )
   {
      if( auto client_socket = accept( ) )
      {
         m_client_sockets.push_back( p_socket );
         m_client_threads.push_back( std::thread{ Server::thread_loop, client_socket } );
         p_socket->info( "Host connected" );
         p_socket->unblock( );
         m_consumer.connected( p_socket );
      }
   }
}
