# CppND-System-Monitor

Follow along with the classroom lesson to complete the project!

![System Monitor](images/monitor.png)

## Make
This project uses [Make](https://www.gnu.org/software/make/). The Makefile has four targets:
* `build` compiles the source code and generates an executable
* `format` applies [ClangFormat](https://clang.llvm.org/docs/ClangFormat.html) to style the source code
* `debug` compiles the source code and generates an executable, including debugging symbols
* `clean` deletes the `build/` directory, including all of the build artifacts

## Instructions

1. Build the project: `make build`

2. Run the resulting executable: `./build/monitor`
![Starting System Monitor](images/starting_monitor.png)