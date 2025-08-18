# Ciri

Ciri (C++ INI Runtime Improvements) is a lightweight, powerful INI configuration parser for C++ projects. It supports advanced parsing features including lists, cross-references, argument substitution, environment variable replacement, and command execution.

## Features

* **INI parsing**: Read sections, keys, and values.
* **Lists**:

  * Lists can be comma-separated, optionally enclosed in `{}`.
  * Concatenated lists are supported: `{a,b}{c,d}` → `{"a","b","c","d"}`.
  * Replication lists are supported: `(N,e)` → repeats element `e` N times.
* **Argument substitution**: `$N` or `$(N(:default)?)` replaces with the Nth argument. Default is used if the argument is missing. Starts with $1, $0 corresponds to the directory where the INI file is located or the current path if parsed from a string.
* **Environment variable substitution**: `$VAR` or `${VAR(:DEFAULT)?}` replaces with environment variables. If `VAR` does not exist, `DEFAULT` is first checked as an environment variable, otherwise its literal value is used.
* **Cross-section references**: `${section#key}` allows referencing values from other sections.
* **Command execution**: `$[cmd]` executes shell commands and replaces with their output.
* **Pass arguments**: Arguments can be passed as `std::vector<std::string>` when constructing `Ciri`.
* **Type support**: Values of any type can be supported by using the template getter functions.

## API

### Constructor

```cpp
Ciri(const std::filesystem::path& file_path, const std::vector<std::string>& args = {});
```

* **Description**: Loads the INI file at `file_path` and stores optional runtime arguments.
* **Parameters**:

  * `file_path`: Path to the INI file.
  * `args`: Vector of runtime arguments used for `$N` substitutions.

```cpp
Ciri(const std::string& content, const std::vector<std::string>& args = {});
```

* **Description**: Parses content and stores optional runtime arguments.
* **Parameters**:

  * `content`: Text corresponding to an INI file.
  * `args`: Vector of runtime arguments used for `$N` substitutions.

### Existence Checks

```cpp
bool exists(const std::string& section) const;
bool exists(const std::string& section, const std::string& key) const;
```

* **Description**: Checks whether a section or key exists.

```cpp
std::vector<std::string> sections() const;
std::vector<std::string> keys(const std::string& section) const;
```

* **Description**: Returns the list of sections or keys in a section.

### Modifiers

```cpp
void add(const std::string& section, const std::string& key, const std::string& value);
void remove(const std::string& section, const std::string& key);
```

* **Description**: Add a new entry (appends to existing values) or remove an entry.

### Save

```cpp
void save(const std::string& filename) const;
void save(const std::filesystem::path& file_path) const;
```

* **Description**: Saves the contents of the runtime data to a a file.

### Getters


```cpp
T get(const std::string& section, const std::string& key, const T& default_value, const std::function<T(const std::string&)>& converter) const;
std::vector<T> getList(const std::string& section, const std::string& key, const std::vector<T>& default_value, F converter) const;
```

* **Description**: Get a value parsed into any type by providing a string to type function.

Ciri already provides:
```cpp
std::string getString(const std::string& section, const std::string& key, const std::string& default_value = "") const;
int getInteger(const std::string& section, const std::string& key, int default_value = 0) const;
unsigned getUnsigned(const std::string& section, const std::string& key, unsigned default_value = 0) const;
bool getBoolean(const std::string& section, const std::string& key, bool default_value = false) const;
float getFloat(const std::string& section, const std::string& key, float default_value = 0.0f) const;
std::vector<std::string> getStringList(const std::string& section, const std::string& key, const std::vector<std::string>& default_value = {}) const;
std::vector<int> getIntegerList(const std::string& section, const std::string& key, const std::vector<int>& default_value = {}) const;
std::vector<unsigned> getUnsignedList(const std::string& section, const std::string& key, const std::vector<unsigned>& default_value = {}) const;
std::vector<bool> getBooleanList(const std::string& section, const std::string& key, const std::vector<bool>& default_value = {}) const;
std::vector<float> getFloatList(const std::string& section, const std::string& key, const std::vector<float>& default_value = {}) const;
```


### Environment & Argument Handling

* `$N` / `$(N(:default)?)`: Replace with the Nth argument or default.
* `$VAR` / `${VAR(:DEFAULT)?}`: Replace with environment variable, check DEFAULT as env first, else use literal.
* `${section#key}`: Cross-reference within INI.
* `$[cmd]`: Executes command and replaces with output.

## Examples

See [example/test.ini](example/test.ini) for full demonstration.

## Building and Testing

```bash
make         # builds the project
make test    # runs a short dummy test
```

## License

[BSD 3-Clause License](LICENSE.txt)

