# Qmac
An implementation of the Quine-McCluskey Algorithm for Boolean function minimization, written in C.

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
make or gcc -o build/qmac ./src/*.c
````

### ▶️ Usage

```bash
./build/qmac -v <var_count> -m <m-terms> -d <d-terms>
```

* `-v`: Number of variables (e.g. 3 for A, B, C) **\[Required]**
* `-m`: Comma-separated minterms (e.g. `1,3,5`) **\[Optional]**
* `-d`: Comma-separated don't-care terms (e.g. `0,2,7`) **\[Optional]**

> 🔸 **Note:** At least one of `-m` or `-d` **must be provided**. You can also use both.


### 📌 Example

```bash
./build/qmac -v 3 -m 1,3,5 -d 0,2,7
```

## 🗂️ Project Structure

```
qmac
├── build
│   └── qmac
├── include
│   ├── group.h
│   ├── helper.h
│   ├── minterm.h
│   ├── parser.h
│   └── utils.h
├── LICENSE
├── Makefile
├── README.md
└── src
    ├── group.c
    ├── main.c
    ├── minterm.c
    ├── parser.c
    └── utils.c
````