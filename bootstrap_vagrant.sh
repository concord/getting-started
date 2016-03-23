#!/bin/bash --login
DIR=$(git rev-parse --show-toplevel)
if [[ -e ${DIR}/.env ]]; then
    source ${DIR}/.env/bin/activate
else
    sudo apt-get update
    sudo apt-get install -y \
         software-properties-common \
         python-dev \
         python-pip

    sudo pip install virtualenv
    echo "Creating work directory ${PWD}/.env"
    mkdir -p ${DIR}/.env
    virtualenv ${DIR}/.env
    source ${DIR}/.env/bin/activate
    pip install paramiko PyYAML Jinja2 httplib2
    pip install ansible==1.9.2
    echo "Successfully installed ansible 1.9.2!"
fi

vagrant halt
vagrant up --provision
