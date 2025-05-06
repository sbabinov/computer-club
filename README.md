> [!IMPORTANT]
> ## Important notes
> 1. The program implies that an event with ```ID 4``` cannot be triggered for a client who has not occupied a table, but is in a waiting queue.

# Build
> [!NOTE]
> The project requires support of ```C++14``` standart.

For a successful build create directory ```/build``` in the project root. Then use following commands:
```cmd
cd build
cmake ..
make
```
You can choose whether to build the tests or not using the option in CMakeLists.txt:
```cmake
option(COMPUTER_CLUB_BUILD_TESTS "Build tests" ON)
```
By default, it's ```ON```.

Now, you can run the program:
```cmd
./main <file_name>
```
...and tests:
```cmd
./tests/tests
```


