from setuptools import setup
from Cython.Build import cythonize
import numpy

setup(
    name="multiply",
    ext_modules=cythonize("multiply.py", build_dir="build", language_level=3),
    include_dirs=[numpy.get_include()]
)
