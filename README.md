# CIRI - C++ INIH Runtime Improvements

**Ciri** stands for **C++ INIH Runtime Improvements**.  
It is a lightweight C++ wrapper around [inih](https://github.com/benhoyt/inih), providing additional runtime features for INI configuration parsing.

---

## Features

Ciri extends the functionality of `INIReader` from `inih` with the following enhancements:

- **Argument Substitution**: Supports string substitution using:
  - `$(0)` ... `$(N)` for positional argument replacement.
  - `${section=key}` for cross-reference resolution within the INI.
- **Modification Methods**: Allows modifying the loaded INI and save it into a file.
  - `Add`
  - `Remove`
  - `Save`
  - `File`
- **Trimming**:
  - Automatically trims leading/trailing spaces from list elements.
  - Removes enclosing double quotes (`"`) from list elements if both start and end quotes are present.
- **List Parsing**:
  - Parses lists written as `{a, b, c}` into `std::vector<std::string>`.
  - Supports repetition lists using `(N, value)` syntax, expanded to `N` repeated `value`s.
- **Type-Safe List Access**:
  - `GetStringList`
  - `GetIntegerList`
  - `GetUnsignedList`
  - `GetInteger64List`
  - `GetUnsigned64List`
  - `GetRealList`
  - `GetBooleanList`
- **Graceful fallback** to default values on type mismatch or parse errors.

---

## Build Instructions

Before building, clone the repository with submodules:

```bash
git clone --recurse-submodules https://github.com/marc24force/ciri.git
````

If you already cloned without submodules, initialize them with:

```bash
git submodule update --init --recursive
```

To build the library:

```bash
make
````

This compiles `Ciri` and produces a static/shared library in the `build/` directory without polluting the `inih/` source tree.

To build and run the included test:

```bash
make test
```

### BOSC

Ciri is used for the project bulding tool [bosc](https://github.com/marc24force/bosc). 
It also supports being managed by it for building other projects.

---

## Dependencies

* Requires `inih` (included as a git submodule in the `inih/` directory.)
* A C++11 compatible compiler (e.g., `g++ 4.8+`, `clang++ 3.3+`)

---

## License

Ciri builds upon the [inih](https://github.com/benhoyt/inih) library, which is licensed under the [BSD license](https://opensource.org/licenses/BSD-3-Clause).

This extension is distributed under the same BSD 3-Clause license.
