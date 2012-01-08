#!/bin/bash

################################################
#
# Runs client and server to test converations 
# between the two. 
#
################################################

function usage() {
    echo "Script failed. Run sudo test_setup.sh first." 
    exit 1
}

if [ -z ${DUMMY_ACCOUNT} ]; then
    usage
    exit 1
fi


CURRENT_USER=$(whoami)

echo "Current users ${CURRENT_USER}"

sudo su - ${DUMMY_ACCOUNT}



 




