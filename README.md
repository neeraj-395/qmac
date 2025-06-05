# Qmac
An implementation of the Quine-McCluskey algorithm for Boolean function minimization, written in C.

## 📦 Features

- Supports up to 16 input variables
- Handles both **minterms** and **don't-care terms**
- Lightweight and dependency-free

## 🚀 Build & Run

### 🛠️ Requirements

* **GCC compiler** (e.g. `gcc (GCC) 11.3.0` or higher)
* Compatible with most Unix-like systems (Linux/macOS)

### 🛠️ Compile

```bash
gcc -o qmac ./src/*.c
````

### ▶️ Usage

```bash
./qmac -v <var_count> -m <m-terms> -d <d-terms>
```

* `-v`: Number of variables (e.g. 3 for A, B, C) **\[Required]**
* `-m`: Comma-separated minterms (e.g. `1,3,5`) **\[Optional]**
* `-d`: Comma-separated don't-care terms (e.g. `0,2,7`) **\[Optional]**

> 🔸 **Note:** At least one of `-m` or `-d` **must be provided**. You can also use both.


### 📌 Example

```bash
./qmac -v 3 -m 1,3,5 -d 0,2,7
```

## 🗂️ Project Structure

```
qmac/
├── LICENSE
├── README.md
└── src
    ├── group.h              # Grouping and simplification logic
    ├── helper.h             # Common macros (e.g., SAFE_ALLOC)
    ├── logic_parser.h       # CLI argument parsing and struct setup
    ├── main.c               # Entry point: combines everything
    ├── minterm.h            # Minterm structure and manipulation
    └── utils.h              # Utility functions
````