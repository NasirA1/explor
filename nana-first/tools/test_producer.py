#! /usr/bin/env python

# Import the os module, for the os.walk function
import os
from time import sleep


# Set the directory you want to start from
rootDir = '.'


for dirName, subdirList, fileList in os.walk(rootDir):
  print('Found directory: %s' % dirName)
  for fname in fileList:
    print('\t%s' % fname)
    with open (fname, "rb") as file:
      print( file.read() )
