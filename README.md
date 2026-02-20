# cpp_multi

A small multi-directory C++ project demonstrating libraries and a main application.

## Structure

```
cpp_multi/
├── CMakeLists.txt          # root – no source files here
├── geometry/               # static library: 3-D geometry primitives
│   ├── CMakeLists.txt
│   ├── include/geometry/
│   │   ├── point.h
│   │   ├── shape.h
│   │   └── transform.h
│   └── src/
│       ├── point.cpp
│       ├── shape.cpp
│       └── transform.cpp
├── io/                     # static library: logger + file writer
│   ├── CMakeLists.txt
│   ├── include/io/
│   │   ├── logger.h
│   │   └── file_writer.h
│   └── src/
│       ├── logger.cpp
│       └── file_writer.cpp
└── app/                    # executable – consumes both libraries
    ├── CMakeLists.txt
    └── main.cpp
```

## Requirements

| Tool  | Minimum version |
|-------|----------------|
| GCC   | 7 (C++17)      |
| CMake | 3.14           |

On RHEL / CentOS / Fedora, install with:

```bash
sudo dnf install gcc-c++ cmake        # RHEL 8 / 9 / Fedora
# or
sudo yum install gcc-c++ cmake3       # RHEL 7
```

## Build

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)
```

## Run

```bash
./build/bin/app
```

The app prints shape properties to the terminal and writes a summary to `output.txt`
in the directory from which it is invoked.
