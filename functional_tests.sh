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

if [ -e /var/run/ucp.pid ]; then
    echo "ucp listener already running"
else
  `sudo  src/ucp -d -L`  
fi

sudo rm -f /home/${DUMMY_ACCOUNT}/testfile

src/ucp data/testfile $DUMMY_ACCOUNT@localhost:testfile



sleep 2

sudo kill -SIGINT `cat /var/run/ucp.pid`


diff -r data/testfile /home/${DUMMY_ACCOUNT}/testfile 

RESULT=$?

if [[ $RESULT == "0" ]]
then echo "....Test Passed!"
else echo "....Test Failed!"
fi  




