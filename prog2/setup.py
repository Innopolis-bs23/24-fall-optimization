from distutils.core import setup
from distutils.extension import Extension
from Cython.Build import cythonize

ext_modules = [
    Extension(
        "simplex",
        ["simplex.pyx"],
        extra_link_args=['-undefined', 'dynamic_lookup']  # For macOS
    )
]

setup(
    ext_modules=cythonize(ext_modules)
)