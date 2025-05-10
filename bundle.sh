#!/bin/bash
set -e

# Clean and prep
rm -rf bundle
python3 -m venv bundle/venv
source bundle/venv/bin/activate

# Install deps and build .so
cd python_scripts
pip install -r requirements.txt
python setup.py build_ext 

# Bundle build output
cd ..
mkdir -p bundle/lib
cp python_scripts/so_files/*.so bundle/lib/

# Optional cleanup
#rm -rf bundle/venv/share bundle/venv/include

# Step 6: Create run script
cat << 'EOF' > bundle/run.sh
#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source "$DIR/venv/bin/activate"
EOF

# Create usage note
cat << EOF > bundle/README.txt
To use in C++:
- Initialize Python using your C++ app (Py_Initialize)
- Set PYTHONPATH to 'bundle/lib'
- Import 'multiply' from C++ and call exposed functions
- Execute 'bash bundle/run.sh'
- Execute 'g++ main.cpp -o runme -I/usr/include/python3.9 -lpython3.9'
- Then ./runme
EOF

echo "✅ Your Python logic is now bundled as a hidden .so in ./bundle"
