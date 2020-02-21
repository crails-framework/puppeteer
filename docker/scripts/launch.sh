#!/bin/bash -ex

current_dir=`dirname $0`
cd $current_dir

./launch-sidekic.sh
./launch-faye.sh
./launch-crailsapp.sh $1

if [[ $1 == "--wait" ]] ; then
  ./stop.sh
fi
