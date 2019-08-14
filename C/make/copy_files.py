#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
from shutil import copy

#path section
os.chdir("../../C")
mainPath = os.getcwd()
copyPath = mainPath + "/out/source_files"

#source files lists
cList = []
hList = []

if __name__ == "__main__":

   #x contains:
   #x[0] - path
   #x[1] - list with directories
   #x[2] - list with files

   #iterates on every directory
   for x in os.walk('.'):
      path = x[0][1:]

      if('/out' in path):
         pass

      else:
         #check if there are any files in directory
         if(len(x[2]) > 0):
            print("Entering path: {}".format(path))
            print("Files: {}".format(x[2]))
            os.chdir(mainPath + path)
            print(os.getcwd())

            #copy only .c and .h files
            for file in x[2]:
               if(file.endswith('.c')):
                  cList.append(file)
                  copy(file, copyPath)
                  print(file)
               elif(file.endswith('.h')):
                  hList.append(file)
                  copy(file, copyPath)
                  print(file)
            os.chdir(mainPath)
         print("\n")

   for h in hList:
      print(h)

   for c in cList:
      print(c)
