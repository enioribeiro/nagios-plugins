#!/usr/bin/python
# Enio B Ribeiro
# 08/08/2018

import glob,os,sys
from datetime import date

os.chdir('/root/ldap-backup/')
today = date.today().strftime('%Y%m%d')

list = []

file1 = "ldap-backup-"+ today  +"*.ldif.bz2"
file2 = "ldap-config-backup-"+ today  +"*.ldif.bz2"

for file in glob.glob(file1):
   if(file):
      list.append(file)

for file in glob.glob(file2):
   if(file):
      list.append(file)


if len(list) == 2:
   print('OK - Existem 2 arquivos de backup | %s' % str(list))
   sys.exit(0)
elif len(list) == 1:
   print('WARNING - Existe apenas 1 arquivo de backup | %s' % str(list))
   sys.exit(1)
else:
   print('CRITICAL - Nao existem arquivo de backup')
   sys.exit(2)
