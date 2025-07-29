for file in $(find src -name '*.cpp' -o -name '*.h'); do
    clang-tidy "$file" -p build -fix
done
