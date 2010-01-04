from setuptools import setup, find_packages
import sys, os

version = '0.1'

setup(name='amelethdf',
      version=version,
      description="Amelet-HDF python tools",
      long_description="""\
Amelet-HDF python tools""",
      classifiers=[], # Get strings from http://pypi.python.org/pypi?%3Aaction=list_classifiers
      keywords='Amelet-HDF python electromagnetism simulation',
      author='Cyril Giraudon',
      author_email='cyril.giraudon@axessim.fr',
      url='',
      license='LGPL',
      packages=find_packages(exclude=['ez_setup', 'examples', 'tests']),
      include_package_data=True,
      zip_safe=False,
      install_requires=["tables", "traits"],
      entry_points="""
      # -*- Entry points: -*-
      """,
      )
