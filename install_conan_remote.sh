conan remote add conan-center https://bintray.com/conan/conan-center || echo "conan OK"
conan remote add epitech https://bintray.com/epitech/public-conan || echo "epitech OK"
conan remote add bincrafters https://bintray.com/bincrafters/public-conan || echo "bincrafters OK"
 mkdir build && cd build && conan install .. --build=missing && cmake .. -G “Unix Makefiles” && cmake --build .