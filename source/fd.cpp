#include "common.hpp"
#include "server.hpp"

#include "trace/Trace.hpp"
#define CLASS_ABBR "SERVER"



using namespace lisot::base;




fds::fds( )
{
   mp_read = new fd_set;
   mp_write = new fd_set;
   mp_except = new fd_set;
}

fds::~fds( )
{
   delete mp_read;
   delete mp_write;
   delete mp_except;
}

void fds::reset( )
{
   FD_ZERO( mp_read );
   FD_ZERO( mp_write );
   FD_ZERO( mp_except );
}

void fds::set( const tSocket _socket, const eType type )
{
   fd_set* p_fd_set = convert( type );
   FD_SET( _socket, p_fd_set );
}

void fds::set( const std::set< tSocket > socket_set, const std::set< eType > type_set )
{
   std::set< fd_set* > p_fd_set = convert( type_set );
   for( const auto& _socket : socket_set )
      set( _socket, p_fd_set );
}

void fds::clear( const tSocket _socket, const eType type )
{
   fd_set* p_fd_set = convert( type );
   FD_CLR( _socket, p_fd_set );
}

void fds::clear( const std::set< tSocket > socket_set, const std::set< eType > type_set )
{
   std::set< fd_set* > p_fd_set = convert( type_set );
   for( const auto& _socket : socket_set )
      clear( _socket, p_fd_set );
}

bool fds::is_set( const tSocket _socket, const eType type )
{
   fd_set* p_fd_set = convert( type );

   return p_fd_set ? FD_ISSET( _socket, p_fd_set ) : false;
}

void fds::set( tSocket _socket, std::set< fd_set* > p_fd_set )
{
   for( const auto& item : p_fd_set )
      FD_SET( _socket, item );
}

void fds::clear( tSocket _socket, std::set< fd_set* > p_fd_set )
{
   for( auto& item : p_fd_set )
      FD_CLR( _socket, item );
}

fd_set* fds::convert( const eType type ) const
{
   switch( type )
   {
      case eType::READ:       return mp_read;
      case eType::WRITE:      return mp_write;
      case eType::EXCEPT:     return mp_except;
      default:                return nullptr;
   }
   return nullptr;
}

std::set< fd_set* > fds::convert( const std::set< eType > type_set ) const
{
   std::set< fd_set* > fds;
   for( auto& type : type_set )
   {
      switch( type )
      {
         case eType::READ:       fds.insert( mp_read );     break;
         case eType::WRITE:      fds.insert( mp_write );    break;
         case eType::EXCEPT:     fds.insert( mp_except );   break;
         default:                                           break;
      }
   }
   return fds;
}

fd_set* const fds::read( ) const
{
   return mp_read;
}

fd_set* const fds::write( ) const
{
   return mp_write;
}

fd_set* const fds::except( ) const
{
   return mp_except;
}
