#! /usr/bin/env python

# Import the os module, for the os.walk function
import os
#from time import sleep
import argparse


"""Make a hexdump"""
import re, sys
from binascii import hexlify
from functools import partial

def hexdump(filename, chunk_size=1<<15):
  add_spaces = partial(re.compile(b'(..)').sub, br'\1 ')
  write = getattr(sys.stdout, 'buffer', sys.stdout).write
  with open(filename, 'rb') as file:
    for chunk in iter(partial(file.read, chunk_size), b''):
      write(add_spaces(hexlify(chunk)))


class cd:
  """Context manager for changing the current working directory"""
  def __init__(self, newPath):
    self.newPath = os.path.expanduser(newPath)

  def __enter__(self):
    self.savedPath = os.getcwd()
    os.chdir(self.newPath)

  def __exit__(self, etype, value, traceback):
    os.chdir(self.savedPath)


parser = argparse.ArgumentParser(description='Test producer for Tap')
parser.add_argument('rootdir', nargs='?', help='Root directory to start traversal from')
args = parser.parse_args()

# Set the directory you want to start from
rootDir = '.'
if(args.rootdir):
  rootDir = args.rootdir


for dirName, subdirList, fileList in os.walk(rootDir):
  print('Found directory: %s' % dirName)
  with cd (dirName):
    for fname in fileList:
      print('\t%s' % fname)
      hexdump(fname)
