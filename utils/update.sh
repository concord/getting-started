#!/bin/bash

python fetch_artifacts.py

sudo pip uninstall concord concord-py
sudo pip install concord concord-py

sudo gem2.0 uninstall concord-ruby
sudo gem2.0 install concord-ruby

