import setuptools

if __name__ == '__main__':
    setuptools.setup(
        name='codegen',
        version='0.1.0.0',
        packages=[
            'src'
        ],
        url='https://github.com/milosob/milo',
        license='MIT',
        author='milosob',
        author_email='git@milosob.com',
        description='milo source code generation tool.',
        install_requires=[],
        entry_points={
            'console_scripts': [
                'milo-codegen=main:main'
            ]
        }
    )
