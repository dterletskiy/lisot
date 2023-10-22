#pragma once

#include <sys/socket.h>
#include <linux/vm_sockets.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>

#include <iostream>
#include <thread>
#include <ctime>



namespace common {

   namespace default_values {

      const std::string socket_family   = "AF_VSOCK";
      const std::string socket_type     = "SOCK_STREAM";
      const int socket_protocole        = 0;

      // const int address_vsock           = VMADDR_CID_HYPERVISOR;
      const int address_vsock           = VMADDR_CID_LOCAL;
      // const int address_vsock           = VMADDR_CID_HOST;
      const char* const address_inet    = "127.0.0.1";
      const char* const address_unix    = "/tmp/unix.socket";

      const int port                    = 5000;

      const int timeout                 = 1000;
      const std::string mode            = "count";
      const size_t message_length       = 0;
      const size_t count                = std::numeric_limits< size_t >::max( );

      const size_t message_max_size     = 4096;

   }

   int string_to_socket_family( const std::string& family );
   int string_to_socket_type( const std::string& family );



   class Parameters
   {
      public:
      private:
         int socket_family;
         int socket_type;
         int socket_protocole;
   };



   const char* const info = " \n\
Client-server applications for testing Linux socket communication. \n\
Server is the echo-server. \n\
 \n\
USAGE: \n\
   lisot --server|--client [PARAMETER...] \n\
 \n\
PARAMETER: \n\
 \n\
   --family=FAMILY         - Socket communication domain. \n\
                             Default value: 'AF_VSOCK'. \n\
                             Supported values: 'AF_UNIX', 'AF_INET', 'AF_VSOCK'. \n\
 \n\
   --type=TYPE             - Socket communication semantics. \n\
                             Default value: 'SOCK_STREAM'. \n\
                             Supported values: 'SOCK_STREAM', 'SOCK_DGRAM'. \n\
 \n\
   --protocole=PROTOCOLE   - Socket protocol. \n\
                             Default value: '0'. \n\
                             Supported values: '0' \n\
 \n\
   --address=ADDRESS       - Connection address to be bind or connect and depends on \n\
                             the address family. \n\
                             Default value: \n\
                                 - '/tmp/unix.socket' - path to file for AF_UNIX socket family; \n\
                                 - '127.0.0.1' - IP address for AF_INET socket family; \n\
                                 - '1' - CID for AF_VSOCK socket family; \n\
 \n\
   --port=PORT             - Connection port to be bind or connect. \n\
                             Not relevant for AF_UNIX socket family. \n\
                             Default value: \n\
                                 - '5000' - for AF_INET and AF_VSOCK socket family; \n\
 \n\
   --mode=MODE             - Client work mode defines what messages client will send to server. \n\
                             Default value: count. \n\
                             Supported values: \n\
                                 - 'count' - client will send to server message with count number \n\
                                   what will be started from 0 and incremented in each next message; \n\
                                 - 'message' - client will send to server single message what user \n\
                                   will write in the console; \n\
                                 - 'random' - client will send to server random text message what \n\
                                   with length defined in '--length' parameter; \n\
 \n\
   --timeout=TIMEOUT       - Timeout in miliseconds between messages what client will send to server \n\
                             in 'count' and 'random' mode. \n\
                             Default value: '1000'. \n\
 \n\
   --length=LENGTH         - Message length what client will send to server in 'random' mode. \n\
                             Default value: '0'. \n\
                             Supported values: \n\
                                 - '0' - client each time will generate message with random length \n\
                                    between 1 and 4095. \n\
                                 - '1'..'4096' - client each time will generate message with defined length \n\
 \n\
   ";

}



// #define MESSAGE( FORMAT, ... ) base::write( FORMAT "\n", ##__VA_ARGS__ );
