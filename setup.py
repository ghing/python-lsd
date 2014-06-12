import os.path
import subprocess

from setuptools import setup

with open(os.path.join(os.path.dirname(__file__), 'README.rst')) as f:
    readme = f.read()

subprocess.call(['make', '-C', 'src'])

setup(name='LSD',
      version='0.0.1',
      description='Python bindings for the LSD line segment detector.',
      long_description=readme,
      author='Geoff Hing',
      author_email='geoffhing@gmail.com',
      url='https://github.com/ghing/python-lsd',
      packages=['lsd'],
      package_data={'lsd': ['liblsd.so']},
      include_package_data=True,
      classifiers=[
          'Development Status :: 1 - Planning',
          'Intended Audience :: Developers',
          'License :: OSI Approved :: GNU Affero General Public License v3 or later (AGPLv3+)',
          'Operating System :: OS Independent',
          'Programming Language :: Python',
          'Programming Language :: C',
          ],
     )
