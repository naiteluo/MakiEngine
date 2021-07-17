#!/bin/bash
if [ "$(uname)" == "Darwin" ]; then
  echo "Clean up local tmp files"
  # remove old tmp files
  rm -rf ./ispc-*.tar.gz
  rm -rf ./ispc-release-*
  # download prebuilt ispc
  echo "Download start"
  wget https://github.com/ispc/ispc/releases/download/v1.16.0/ispc-v1.16.0-macOS.tar.gz
  tar zxvf ./ispc-v1.16.0-macOS.tar.gz
  mv ispc-v1.16.0-macOS ispc-release-darwin
  # remove tmp files
  echo "Clean up"
#  rm -rf ./ispc-*.tar.gz
  echo "Installation Succeed."
else
  echo "Installation Failed."
  echo "Current Platform's installation with this script is not supported yet."
fi