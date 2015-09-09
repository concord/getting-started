#!/bin/bash --login

root=$(git rev-parse --show-toplevel)
cd $root
vagrant package
hash=$(git log --pretty=format:"%h" | head -n1)
latest="concord_${hash}.box"
mv package.box $latest
echo "Latest release is now: $latest"
gsutil -m cp -a public-read $latest gs://concord-release/$latest
