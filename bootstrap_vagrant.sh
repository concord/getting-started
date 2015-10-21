#!/bin/bash --login
git_root=$(git rev-parse --show-toplevel)
if [[ ! -d .env ]]; then
    virtualenv $git_root/.env
    source $git_root/.env/bin/activate
    pip install ansible
else
    source $git_root/.env/bin/activate
fi

vagrant halt
vagrant up --provision
