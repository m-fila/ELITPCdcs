#!/bin/bash
cmake -H. -Bbuild -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=$HOME/$CI_PROJECT_NAME-$CI_COMMIT_REF_NAME
cmake --build build
rm -rf $HOME/$CI_PROJECT_NAME-$CI_COMMIT_REF_NAME
cmake --build build --target install
