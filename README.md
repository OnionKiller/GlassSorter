# GlassSorter
This is a pet project to practice cpp&amp;python interops
It has not yet docuumneted well.

# Run & build with Visual Studio
Just open the solution file, and everything should be set up already. 
Testing is available trough the Test-Explorer.

# Run & build with CMake
I assume that `bash` is available, but most of the commansd have aliases on PS as well.
Start a terminal from the root direcotry of the repository.
```bash
#init cmake
cmake -S . -B build

#build
cmake --build build

#test
cd build
ctest
```

