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


sudo su --login -c "./ucp --verbose -d -L --pid-directory=/home/${DUMMY_ACCOUNT} > trace.log &"  $DUMMY_ACCOUNT



 




