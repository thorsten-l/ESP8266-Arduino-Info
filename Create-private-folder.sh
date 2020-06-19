#!/bin/bash

if [ ! -d private ]; then
  echo "Creating 'private' folder from 'config' directory."
  mkdir private
  cp config/* private
else
  echo "'private' folder already exists."
fi
