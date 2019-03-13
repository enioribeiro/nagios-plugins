#!/usr/bin/python
# Enio Borges Ribeiro - 01/08/2018
# TRT-SP
# Executar com SUDO
#

import sys,sh,os,subprocess,time,signal
from datetime import datetime


def log(text):
        fileName = "./proad.log"

        if(os.path.isfile(fileName)):
                with open(fileName, "a+") as logFile:
                        logFile.write("[" + datetime.now().strftime("%Y-%m-%d %H:%M:%S") +"] - "+ text + "\n")
        else:
                f = open(fileName, "w")
                f.write("---------------------")
                f.close()



def isAlive():
        base       =  os.path.basename(sys.argv[0])
        scriptName =  os.path.splitext(base)[0]

        pid = os.getpid()

        query = "ps aux | grep " + scriptName +" | grep -v grep | grep sudo | grep -v %s" % pid
        for line in os.popen(query):
                otherPid = line.split()[1]
                if((pid-1) != int(otherPid)):
                        print("OK - Em execucao")
                        exit(0)

def killProad():
        #log("\nIniciando processo")
        #DEBUG
        #time.sleep(30)
        #exit(0)
        ps = subprocess.Popen(('ps', 'acx'), shell=False, stdout=subprocess.PIPE)
        output = ps.communicate()[0]
        for line in output.split('\n'):
                if 'java' in line:
                        pid = line.split()[0]
                        try:
                                print("\nMatando processo %s" % pid)
                                os.kill(int(pid), signal.SIGKILL)
                        except OSError:
                                print("Processos PROAD mortos")
                                line.pop()



if __name__ ==  "__main__":
        #exit(0)
        isAlive()
        killProad()
        print("\nIniciando PROAD\n")
        os.system("service proad start");
        time.sleep(300)
        exit(0)
