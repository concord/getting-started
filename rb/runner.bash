#!/bin/bash --login

set -ex

echo "Directory: $(pwd)"


gem2.0 install bundler
bundle install
exec bundle exec ruby2.0 "$@"
