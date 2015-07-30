#!/bin/bash --login

echo "Directory: $(pwd)"
exec bundle exec ruby2.0 "$@"
