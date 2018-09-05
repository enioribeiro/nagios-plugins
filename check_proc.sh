# Enio B Ribeiro ( 08/08/2018 )
#!/bin/bash

OK=0
CRIT=2

ps_out=`ps -ef | grep $1 | grep -v 'grep' | grep -v $0`
result=$(echo $ps_out | grep "$1")
if [[ "$result" != "" ]];then
    echo "OK | Running"
        exit $OK
else
    echo "ERROR | Not Running"
        exit $CRIT
fi
