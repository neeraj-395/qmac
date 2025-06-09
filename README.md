# 🧮 Qmac: Quine-McCluskey Minimizer

An implementation of the Quine-McCluskey Algorithm for Boolean function minimization, written in C.

## 📦 Features

* Supports up to **16 input variables**
* Handles both **minterms** and **don’t-care terms**
* Compatible with Linux/macOS
* Lightweight and dependency-free

## 🛠️ Requirements

* GCC or Clang (tested with `gcc 11.3.0+`)
* Make (for building targets)
* Unix-like environment (Linux/macOS)

## 🚧 Build Instructions

### 🔧 Compile

```bash
make        # To build the main and all test

make all    # To build only the main binary

make test   # To build and run all test binaries

make clean  # To clean the build artifacts
```

## ▶️ Usage

```bash
./bin/qmac -v <var_count> -m <m-terms> -d <d-terms>
```

### Arguments:

* `-v`: Number of variables (e.g., 3 for A, B, C) **\[Required]**
* `-m`: Comma-separated minterms (e.g., `1,3,5`) **\[Optional]**
* `-d`: Comma-separated don’t-care terms (e.g., `0,2,7`) **\[Optional]**

> 🔸 **Note:** At least one of `-m` or `-d` **must** be provided.

## 📌 Example

```bash
./bin/qmac -v 3 -m 1,3,5 -d 0,2,7
```

## 🗂️ Project Structure

```
qmac/
├── bin/           # Compiled binaries (main and test executables)
├── build/         # Object files (.o) for all sources and tests
├── include/       # Header files for all modules
├── src/           # Source files (core logic and main entry point)
├── tests/         # Unit tests for various modules
├── Makefile       # Build system
├── README.md      # Project documentation
└── LICENSE        # License file (MIT)
```

## ✅ Test Suite

Test binaries are automatically generated in the `bin/` directory and cover:

* Coverage table logic
* Essential prime implicants
* Dominance checks
* Reduction logic

To run them all:

```bash
make test
```
