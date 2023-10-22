# lisot
Linux socket test tool

Client-server applications for testing Linux socket communication.
Server is the echo-server.

USAGE:
   lisot --server|--client|--info [PARAMETER...]

PARAMETER:

   --family=FAMILY         -  Socket communication domain.
                              Default value: 'AF_VSOCK'.
                              Supported values: 'AF_UNIX', 'AF_INET', 'AF_VSOCK'.

   --type=TYPE             -  Socket communication semantics.
                              Default value: 'SOCK_STREAM'.
                              Supported values: 'SOCK_STREAM', 'SOCK_DGRAM'.

   --protocole=PROTOCOLE   -  Socket protocol.
                              Default value: '0'.
                              Supported values: '0'

   --address=ADDRESS       -  Connection address to be bind or connect and depends on the address family.
                              Default value:
                                 -  '/tmp/unix.socket' - path to file for AF_UNIX socket family;
                                 -  '127.0.0.1' - IP address for AF_INET socket family;
                                 -  '1' - CID for AF_VSOCK socket family;

   --port=PORT             -  Connection port to be bind or connect.
                              Not relevant for AF_UNIX socket family.
                              Default value:
                                 -  '5000' - for AF_INET and AF_VSOCK socket family;

   --mode=MODE             -  Client work mode defines what messages client will send to server.
                              Default value: count.
                              Supported values:
                                 -  'count' - client will send to server message with count number
                                    what will be started from 0 and incremented in each next message;
                                 -  'message' - client will send to server single message what user
                                    will write in the console;
                                 -  'random' - client will send to server random text message what
                                    with length defined in '--length' parameter;

   --timeout=TIMEOUT       -  Timeout in miliseconds between messages what client will send to server
                              in 'count' and 'random' mode.
                              Default value: '1000'.

   --length=LENGTH         -  Message length what client will send to server in 'random' mode.
                              Default value: '0'.
                              Supported values:
                                 -  '0' - client each time will generate message with random length
                                    between 1 and 4095.
                                 -  '1'..'4095' - client each time will generate message with defined length

   --interface=NAME        -  Name of the interface what will be used by lisot tool in client mode to connect
                              to the server.
