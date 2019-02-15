from setuptools import setup, Extension


try:
    from wheel.bdist_wheel import bdist_wheel as _bdist_wheel
    class bdist_wheel(_bdist_wheel):
        def finalize_options(self):
            _bdist_wheel.finalize_options(self)
            self.root_is_pure = False
except ImportError:
    bdist_wheel = None


def readme():
    with open('../../README.rst') as f:
        return f.read()


setup(name='pyDBoW3',
      version='0.1',
      description='Python wrapper for DBoW3 library',
      long_description=readme(),
      url='http://github.com/foxis/pyDBoW3',
      author='Andrius Mikonis',
      author_email='andrius.mikonis@gmail.com',
      classifiers=[
          'Development Status :: 3 - Alpha',
          'License :: OSI Approved :: MIT License',
          'Programming Language :: Python :: 2.7',
          'Programming Language :: Python :: 3.5',
          'Programming Language :: Python :: 3.6',
          'Programming Language :: Python :: 3.7',
          'Topic :: Text Processing :: Linguistic',
      ],
      keywords='bag of words bow dbow3 dbow slam orb odometry visual',
      license='MIT',
      packages=['pyDBoW3'],
      include_package_data=True,
      zip_safe=False,
      install_requires=[
          'numpy'
      ],
      cmdclass={'bdist_wheel': bdist_wheel},
)