
CONFIG=Debug # or Release
set CONFIG=DEBUG

# Setup
cmake -B out/x64-release -DCMAKE_BUILD_TYPE=Release -S .    
cmake -B out/x64-debug   -DCMAKE_BUILD_TYPE=Debug   -S .    

# Build 
cmake --build out/x64-release --target all    
cmake --build out/x64-debug   --target all    

# Test
ctest --test-dir out/x64-release/    
ctest --test-dir out/x64-debug/    
