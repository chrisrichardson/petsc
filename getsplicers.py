#!/usr/bin/env python
import user

import os
import sys
import re
import cPickle

def getSplicersDir(splicedimpls,dir,names):

  reg = re.compile('splicer.begin\(([A-Za-z0-9._]*)\)')

  if 'SCCS' in names: del names[names.index('SCCS')]
  if 'BitKeeper' in names: del names[names.index('BitKeeper')]
  if 'docs' in names: del names[names.index('docs')]
  for f in names:
    if f.endswith('.pyc'): continue
    if f.endswith('.log'): continue
    if f.endswith('.db'): continue
    if f == '__init__.py': continue
    if f.endswith('.sidl'): continue
    if f.endswith('.o'): continue
    if f.endswith('.a'): continue
    if f.endswith('.so'): continue
    if not os.path.isfile(os.path.join(dir,f)): continue
    fd = open(os.path.join(dir,f),'r')
    line = fd.readline()
    while line:
      if not line.find('splicer.begin') == -1:
        fl = reg.search(line)
        name = fl.group(1)

        line = fd.readline()
        body = ''
        while line.find('splicer.end') == -1:
          body = body + line
          line = fd.readline()
        splicedimpls[name] = body

      line = fd.readline()
    fd.close()
  
def getSplicers(directory):
  splicedimpls = {}

  if not directory: directory = os.getcwd()
  os.path.walk(directory,getSplicersDir,splicedimpls)

  f    = open('splicerblocks', 'w')
  cPickle.dump(splicedimpls,f)
  f.close()
    
if __name__ ==  '__main__':
  if len(sys.argv) > 2: sys.exit('Usage: getsplicers.py <directory>')
  sys.argv.append(None)
  getSplicers(sys.argv[1])

