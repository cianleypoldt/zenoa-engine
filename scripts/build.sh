mkdir build
cd build
#-DCMAKE_BUILD_TYPE=Release
cmake  ..
if [ $? -ne 0 ]; then
    echo "cmake failed"
    exit 1
fi

make
if [ $? -ne 0 ]; then
    echo "make failed"
    exit 1
fi

echo "Build succeeded"

./Zenoa
