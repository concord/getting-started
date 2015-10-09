#!/bin/bash --login
set -e # fail fast
git_root=$(git rev-parse --show-toplevel)
target_dir=$git_root/jvm/target/concord
getting_started=$git_root/jvm
mkdir -p $target_dir

cd $getting_started
sbt assembly

# copy all the artifacts
rm -rf $target_dir/*
cd $target_dir
cp $getting_started/*.json $target_dir
cp $getting_started/runner.bash $target_dir
for f in $(find $git_root -iname "getting_started-assembly-*.jar"); do
    cp $f $target_dir
done
