#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
from time import time
from shutil import copy

print("************* START *************")
start = time()

#create temporary directory
print("### Creating temporary directory")
os.system('mkdir ../out/source_files')

#run python script to copy all source files to out/source_files
print("### Copying source files")
os.system('python3 copy_files.py')

#compile all files
print("### Compiling in progres")
os.system('cd ../out/source_files && make')

#move binary file to /out
print("### Moving binary file")
os.system('mv ../out/source_files/start ../out')

#remove temporary directory
print("### Deleting temporary directory")
os.system('rm -rf ../out/source_files')

end = time()
time_elapsed = round((end - start), 3)
print("\nTime: {}s".format(time_elapsed))
print("************* FINISH *************")