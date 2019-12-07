#!/bin/sh
echo $0
PROJECT_DIR=`dirname $0`
echo project directory $PROJECT_DIR

echo deleting build directory
rm -r $PROJECT_DIR/build

echo creating build directory
mkdir $PROJECT_DIR/build

echo entering build directory
pushd $PROJECT_DIR/build

echo building
cmake3 -DBOOST_INCLUDEDIR=/usr/include/boost169/ -DBOOST_LIBRARYDIR=/usr/lib64/boost169/ ../
RESULT=$?

echo returning
popd

if [ $RESULT -eq 0 ]
then
    echo SUCCESS
else 
    echo FAIL
fi

