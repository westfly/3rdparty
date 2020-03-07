#!/usr/bin/env bash
function run_test() {
    ls test_*
    for file in $(ls test_*)
    do
        ./$file
    done
}
if [[ ! -d build ]]; then
   mkdir build
fi
cd build
cmake -G Ninja .. -DCMAKE_BUILD_TYPE=Release
if [[ $? -eq 0 ]]; then
   ninja 
fi
if [[ $? -eq 0 ]]; then
   run_test 
fi
cd -
