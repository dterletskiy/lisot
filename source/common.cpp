#include "common.hpp"



namespace common {

   int string_to_socket_family( const std::string& family )
   {
      if( "AF_UNIX" == family )           return AF_UNIX;
      if( "AF_INET" == family )           return AF_INET;
      if( "AF_VSOCK" == family )          return AF_VSOCK;

      return -1;
   }

   int string_to_socket_type( const std::string& family )
   {
      if( "SOCK_STREAM" == family )       return SOCK_STREAM;
      if( "SOCK_SEQPACKET" == family )    return SOCK_SEQPACKET;

      return -1;
   }

}
