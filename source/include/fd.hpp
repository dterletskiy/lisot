#pragma once

#include <thread>

#include "parameters.hpp"



namespace lisot::base {

   class fds
   {
      public:
         enum class eType { READ, WRITE, EXCEPT };

      public:
         fds( );
         ~fds( );

         void set( const tSocket _socket, const eType type );
         void set( const std::set< tSocket > socket_set, const std::set< eType > type_set );
         void clear( const tSocket _socket, const eType type );
         void clear( const std::set< tSocket > socket_set, const std::set< eType > type_set );
         bool is_set( const tSocket _socket, const eType type );
         void reset( );

      private:
         void set( tSocket _socket, std::set< fd_set* > p_fd_set );
         void clear( tSocket _socket, std::set< fd_set* > p_fd_set );
         fd_set* convert( const eType type ) const;
         std::set< fd_set* > convert( const std::set< eType > type_set ) const;

      public:
         fd_set* const read( ) const;
         fd_set* const write( ) const;
         fd_set* const except( ) const;
      private:
         fd_set* mp_read = nullptr;
         fd_set* mp_write = nullptr;
         fd_set* mp_except = nullptr;
   };

}
