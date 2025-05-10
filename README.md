# Run_Python_in_cpp
Run a python sccript using cpp


## Intro

🔐 Options to Hide or Protect Python Source Code

### 1. ✅ Compile to .pyc (Bytecode)

  This is the simplest option:
  
  ```
    python -m compileall your_script.py
  ```

  - This creates .pyc files in a __pycache__ folder.

  - Still not fully secure — .pyc files can be decompiled.
  
  You can then run:

  ```
    python __pycache__/your_script.cpython-311.pyc
  ```

### 2. ✅ Use cython to Compile to C, Then to Binary

  You can convert Python code to C, then compile that into a native binary:
  
      pip install cython
      cython --embed -o myprog.c your_script.py
      gcc -Os -o myprog myprog.c $(python3-config --includes --ldflags)
  
  - Now myprog is a standalone executable.

  - Source is much harder to reverse-engineer.

### 3. ✅ Use Nuitka to Compile to Executable

This is one of the best tools for compiling Python to binaries.

      pip install nuitka
      nuitka --standalone --follow-imports your_script.py

- Generates a folder with a compiled binary and all needed files.

- Cross-platform support.

- Much harder to reverse than .pyc.

### 4. 🔒 Use Obfuscation (Not Real Security)

Use tools like:

 - pyarmor

 - pyminifier (for variable renaming and compression)

They make the code harder to read, but don’t offer real protection.


### 🧠 Summary

| Method         | Hides Source | Security Level | Can Be Reversed |
| -------------- | ------------ | -------------- | --------------- |
| `.pyc`         | ❌ Partial    | 🔓 Low         | ✅ Easy          |
| `Cython + gcc` | ✅ Yes        | 🔐 High        | ❌ Hard          |
| `Nuitka`       | ✅ Yes        | 🔐 High        | ❌ Hard          |
| Obfuscators    | ❌ No         | 🔓 Low         | ✅ Very Easy     |


You're asking two things here — both important, and they touch on performance, integration, and protection. Let's address each clearly:
### ✅ 1. Does compiling Python with Cython turn it into a "real" C program?

**Yes and no.**

    Yes, in the sense that Cython translates Python code into C code, which is then compiled with gcc (or clang) into a binary executable or a shared library.

    No, because the resulting binary is still tightly tied to the Python runtime — it embeds or links to Python internally.

So while the final binary looks like a C/C++ program and hides your source code, it's still running your Python logic under the hood via the Python C-API.

    ✅ Key Point: Cython helps hide your code and improve performance, but it doesn't eliminate the Python runtime dependency unless carefully managed.

### ✅ 2. I have a C++ program — should I run a Python script directly, or embed Python via CPython?

Here are your two options side-by-side:

| Option                                                       | Description                                                  | Pros                                                                            | Cons                                                                                                    |
| ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------- |
| 🧪 **Call Python script using `std::system()` or `popen()`** | Run as external process: `system("python3 myscript.py")`     | ✅ Simple, no deep integration<br>✅ Easy to debug<br>✅ Isolated                  | ❌ Slower due to process overhead<br>❌ Can't exchange data easily<br>❌ Can't tightly integrate           |
| 🧬 **Use CPython API (embed Python in C++)**                 | Use `#include <Python.h>` and call Python functions directly | ✅ Fine-grained control<br>✅ Share data between C++/Python<br>✅ No extra process | ❌ Complex setup<br>❌ Memory and error handling are tricky<br>❌ Requires Python to be present at runtime |


### 🚀 Recommendation:

  **Use system call (std::system or popen) if:**

      - You want quick integration

      - No tight data sharing is needed

      - You don’t care about performance overhead

   **Use CPython embedding if:**

      - You want high performance

      - You need to call Python functions with input/output

      - You're building a plugin system or hybrid app


## Writing python script - use of Cython

### Requirement
We'll use Cython to compile the Python script into a shared library.
Install Cython (if not installed yet):
```
pip install cython


yum install python3-devel -y
```

#### 🔄 Project Structure (Minimal + Clean)

```

    project/
    ├── pycode/
    │   ├── your_module.pyx         # Your Python/Cython interface
    │   ├── setup.py                # Builds your `.so`
    │   └── requirements.txt        # sentence-transformers, etc.
    ├── bundle.sh                   # 🔧 Creates deployable bundle

```

**These lines in bundle.sh creates venv. Be careful**
    # Step 1: Create clean environment
    rm -rf bundle
    python3 -m venv bundle/venv
    source bundle/venv/bin/activate

#### To use

```
  cd project
  bash bundle.sh
  cd bundle
  ./run.sh
```

✅ You now have a portable production-ready bundle:

      No .py source code
  
      No need to install Python or packages
  
      Just copy **bundle** directory and run
    


### Below information gives you how to do manually 


### Steps to Compile Python Script into .so (Shared Object) and Use in C++:

In **python_scripts** dirctory there are 2 script
1. multiply_script -> contains multiplication code
2. setup -> This creates **.so** file


**Compile the Python Script to .so:**

Run the following command in your terminal:

```
python setup.py build_ext --inplace
```

This will create a shared object file script.cpython-<version>-<platform>.so (e.g., multiply_script.cpython-38-x86_64-linux-gnu.so) in the same directory.

Now copy the .so file in **compiled_python** ( or any other directry, just change path in main.cpp ) 


**main.cpp** :- Uses compile .so file

**Compile and Run the C++ Program**

Make sure that you have the Python.h headers installed (python3-dev package on Linux). Then compile the C++ program:

```
g++ main.cpp -o runme -I/usr/include/python3.9 -lpython3.9
```

Ensure the .so file is in the same folder as the C++ program or adjust the Python path to locate it.

```
./runme
```

**This will output:**

```
Result: 42
```


## How to use

1. Run bundle.sh using
```
bash bundle.sh
```
2. This will create a directory called **bundle**. This directory contains **lib** directory with .so file(s) and a **venv** python virtual environemt will all dependecies.
3. Copy this **bundle** directory to the directory where you want to use this
