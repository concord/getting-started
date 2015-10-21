#!/bin/bash

python fetch_artifacts.py

# -r to uninstall requirements
sudo pip uninstall -r concord concord-py
sudo pip install concord concord-py

sudo gem2.0 uninstall -a concord-ruby
sudo gem2.0 install concord-ruby

