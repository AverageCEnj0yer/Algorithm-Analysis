# How to Run Python and C++ Code

## Running the Python Script

1. Open a terminal in the project directory.
2. Navigate to the folder containing the Python script (e.g., `Lab 1/`).
3. Run the script using:
   
   ```bash
   python3 script_name.py
   ```
   
   Replace `script_name.py` with the actual filename (e.g., `plotting.py`).

---

## Building and Running C++ Code with CMake

1. Open a terminal in the project directory (e.g., `Lab 2/`).
2. Run CMake to generate the build files and create the build directory automatically:
   
   ```bash
   cmake -B build
   ```
3. Build the project using Make:
   
   ```bash
   cmake --build build
   ```
4. Run the compiled executable (named `test`) from the build directory:
   
   ```bash
   ./build/test
   ```

---

**Note:** Ensure you have Python 3, CMake, and a C++ compiler installed on your system.
