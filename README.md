From any folder in terminal
1. Generate build tree with `cmake -B build -S /path/to/source/root`
2. Build target calculator with `make all -C build`
3. Create a file for input
4. Fill input file with expressions. You can write multiple expressions, each on one a separate line.
5. Create a file for database settings, similar to db.conf in project root
6. Call calculator by passing paths to input and database config files as argument `./build/calculator /path/to/input.txt /path/to/db.conf`
7. Check results from output.txt in current directory

Prerequisites
1. Cmake
2. Mysql installed
3. Given mysql user (in db.conf) should have permissions to CRUD databases and tables. 
