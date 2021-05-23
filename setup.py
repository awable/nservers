#!/usr/bin/python

from distutils.core import setup, Extension

__version__ = "1.0.0"

macros = [('MODULE_VERSION', '"%s"' % __version__)]

setup(name         = "python-nservers",
      version      = __version__,
      author       = "Akhil Wable",
      author_email = "awable@gmail.com",
      description  = "Consistent hashing for multiple servers",
      ext_modules  = [
        Extension(
            name='nservers',
            sources=['nservers.c'],
            define_macros=macros,
            extra_compile_args=['-std=c99'])
      ]
)
