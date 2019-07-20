from distutils.core import setup, Extension
import numpy
from cnpy import __version__

extension_mod = Extension("cnpy.cnpy_c",
                          sources=["cnpy//cnpy_c//cnpycmodule.c",],
                          include_dirs=[numpy.get_include()],
                          libraries=[],
                          library_dirs=[],
                          )

setup(name='cnpy',
      packages=['cnpy'],
      ext_modules=[extension_mod],
      version=__version__,
      author='Max-Heinrich Laves',
      author_email='laves@imes.uni-hannover.de',
      )
