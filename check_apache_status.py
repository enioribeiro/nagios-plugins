#!/usr/bin/python3.6
#
# Enio B. Ribeiro
# 2018-08-20	
# TRT-SP
#

import subprocess
import sys
import urllib.request
import tempfile
import shutil


from optparse import OptionParser, OptionGroup


# Resultados Nagios
OK = 0
WARNING = 1
CRITICAL = 2
UNKNOWN = 3


usage = "%prog -[p] -[w] -[c]\nExemplo: %prog -s Idls -w 200 -c 250"
parser = OptionParser(usage, version="%prog 0.1")

parser.add_option( "-H",
		   type="string",
		   dest="hostname",
		   default="localhost"
		  )

parser.add_option( "-P",
		   type="int",
		   dest="port",
		   default=80
		)
		  
parser.add_option("-p",
		  type="string",
		  dest="param",
		  default="ServerUptime",
		  help="IdleWorkers, BusyWorkers, 'Total Accesses', ServerUptime, RestartTime, ServerVersion, etc"
		)	

parser.add_option("-w",
		  type="int",
		  dest="warning",
		  default="0"
		)
		  
parser.add_option("-c",
		  type="int",
		  dest="critical",
		  default="0"
		)

(options, args) = parser.parse_args()

hostname = options.hostname;
port	= options.port
param = options.param;
warning = options.warning;
critical = options.critical;



def get_server_status():
	url = "http://{}:{}/server-status?auto".format(hostname, port)

	with urllib.request.urlopen(url) as response:
		with tempfile.NamedTemporaryFile(delete=False) as tmp_file:
			shutil.copyfileobj(response, tmp_file)
	
	return tmp_file




def file_parser(fileName):
	dicStatus = {}


	with open(fileName.name, 'r') as f:
		for line in f:
			l  = line.strip().split(':')
			if len(l) == 2 :
				dicStatus[l[0]] = l[1]
	return dicStatus



def get_status(status, param, value = None):
   
   if status == OK:
      exitCode = 0
      statusCode = "OK"
   elif status == WARNING:
      exitCode = 1
      statusCode = "WARNING"
   elif status == CRITICAL:
      exitCode = 2
      statusCode = "CRITICAL"
   else:
      print("UNKNOWN: %s" % param)
      sys.exit(3)
	
   if warning > 0 or critical > 0:
      print("{0}: {1}:{2} | {1}={2};{3};{4}".format(statusCode, param, value, warning, critical))
   else:
      print("{0}: {1}:{2}".format(statusCode, param, value))

   sys.exit(exitCode)





if __name__ == "__main__":
	fileTemp  = get_server_status()
	content = file_parser(fileTemp)
	
	if warning > 0 or critical > 0:
		value = int(content[param])
		if value < warning:	
			get_status(OK, param, value)
		elif value >= warning and value < critical:
			get_status(WARNING, param, value)
		elif value >= critical:
			get_status(CRITICAL, param, value)
		else:
			get_status(UNKNOWN, 'no result')
	
	else:
		get_status(OK, param, content[param])



