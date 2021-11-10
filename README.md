From any folder in terminal
1. Generate build tree with `cmake -B build -S /path/to/source/root`
2. Build target calculator with `make all -C build`
3. Create a file for input
4. FIll input file with expressions. You can write multiple expressions, each on one a separate line.
5. Call calculator by passing input file path as argument `./build/calculator /path/to/created/file`
6. Check results from output.txt in current directory