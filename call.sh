./exe/lisot --server --family=AF_UNIX --type=SOCK_STREAM --address=/tmp/unix.socket
./exe/lisot --client --family=AF_UNIX --type=SOCK_STREAM --address=/tmp/unix.socket --mode=random --timeout=1000 --length=256



FAMILY=AF_INET
TYPE=SOCK_STREAM
IP="192.168.0.102"
PORT=5000
INTERFACE="enxd46e0e0895d1"

IP="127.0.0.1"
INTERFACE="wlp0s20f3"
INTERFACE="lo"

IP="8.8.8.8"
PORT=8080

./exe/lisot --server --family=${FAMILY} --type=${TYPE} --address=${IP} --port=${PORT}

./exe/lisot --client --family=${FAMILY} --type=${TYPE} --address=${IP} --port=${PORT} --interface=${INTERFACE} --mode=random --timeout=1000 --length=256
./exe/lisot --client --family=${FAMILY} --type=${TYPE} --address=${IP} --port=${PORT} --mode=random --timeout=1000 --length=256
