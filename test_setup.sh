#!/bin/bash

#################################################
# One time setup for tests.  
#
# This script will set up a dummy account called
# 'dummy' and set the current users id_rsa.pub
# key in the dummy users .ssh/known_hosts.  The
# script will also set up a directory in dummy
# called ~/.ucp that will contain AES session 
# keys used in tests
#
################################################



if [ -z $DUMMY_ACCOUNT ]; then
   
    DUMMY_ACCOUNT=dummy   
fi

echo "Dummy account ${DUMMY_ACCOUNT}"


grep -q DUMMY_ACCOUNT= ~/.bashrc || echo "export DUMMY_ACCOUNT=${DUMMY_ACCOUNT}" >> ~/.bashrc


( cat /etc/passwd | grep -q ${DUMMY_ACCOUNT} ) || userdel --remove ${DUMMY_ACCOUNT}

useradd --create-home ${DUMMY_ACCOUNT}

mkdir /home/${DUMMY_ACCOUNT}/.ssh 
mkdir /home/${DUMMY_ACCOUNT}/.ucp

chown -R ${DUMMY_ACCOUNT}:${DUMMY_ACCOUNT} /home/${DUMMY_ACCOUNT}

cat ~/.ssh/id_rsa.pub >> /home/${DUMMY_ACCOUNT}/.ssh/authorized_keys 


