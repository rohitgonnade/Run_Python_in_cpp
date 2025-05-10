from setuptools import setup, Extension
from Cython.Build import cythonize
import numpy
import glob
import os

# Create target directory if it doesn't exist
output_dir = "so_files"
os.makedirs(output_dir, exist_ok=True)

# Gather all .py files except setup.py
source_files = [f for f in glob.glob("*.py") if f != "setup.py"]

# Define extensions, overriding output paths
extensions = []
for src in source_files:
    module_name = os.path.splitext(os.path.basename(src))[0]
    ext = Extension(
        name=module_name,
        sources=[src],
        include_dirs=[numpy.get_include()],
    )
    extensions.append(ext)

setup(
    name="cython_modules",
    ext_modules=cythonize(
        extensions,
        build_dir="build",
        language_level=3
    ),
    script_args=["build_ext"],
    options={"build_ext": {"build_lib": output_dir}},  # <- this moves .so files to so_files/
)
