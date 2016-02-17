#!/bin/bash

set -e
git_root=$(git rev-parse --show-toplevel)
proj_dir=$git_root/examples/click_rate_demo

thrift -v --gen py:json,utf8strings -o $proj_dir/python $proj_dir/thrift/click.thrift
mv $proj_dir/python/gen-py/click $proj_dir/python
rm -rf $proj_dir/python/gen-py
