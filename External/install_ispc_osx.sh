#!/bin/bash
if [ "$(uname)" == "Darwin" ]; then
  echo "Clean up local tmp files"
  # remove old tmp files
  rm -rf External/tmp/ispc-*.tar.gz
  rm -rf External/Darwin/ispc-release
  # download prebuilt ispc
  echo "Download start"
  wget https://github.com/ispc/ispc/releases/download/v1.16.0/ispc-v1.16.0-macOS.tar.gz -P External/tmp
  mkdir -p External/tmp
  tar zxvf External/ispc-v1.16.0-macOS.tar.gz -C External/tmp
  mv External/tmp/ispc-v1.16.0-macOS External/Darwin/ispc-release
  # remove tmp files
  echo "Clean up"
  rm -rf External/tmp/ispc-*.tar.gz
  echo "Installation Succeed."
else
  echo "Installation Failed."
  echo "Current Platform's installation with this script is not supported yet."
fi
