#!/bin/bash

################################################
#
# Runs client and server to test converations 
# between the two. 
#
################################################



if [ -z "${DUMMY_ACCOUNT}" ]; then
    echo "Script failed. Run sudo test_setup.sh first." 
    exit 1
fi

if [ -e /home/${DUMMY_ACCOUNT}/ucp.pid ]; then
    echo "ucp listener already running"
else
  sudo su --login -c "./ucp --verbose -d -L --pid-directory=/home/${DUMMY_ACCOUNT} > trace.log &"  $DUMMY_ACCOUNT
fi

src/ucp data/testfile $DUMMY_ACCOUNT@localhost:testfile

#sleep 2

#sudo kill -SIGINT `cat /home/${DUMMY_ACCOUNT}/ucp.pid`
 




