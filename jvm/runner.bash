#! /bin/bash --login
dir=$(pwd)
jar_file=$(find $dir -iname "getting_started-assembly-*.jar");
if [[ ! -f $jar_file ]]; then
    echo "Cannot find getting_started assembly file";
    exit 1
fi
echo "Found java executable at: $jar_file"
exec java -cp $jar_file $@
