#!/bin/sh
    if [ ! -d /opt/sunsaver/bin ]
    then
        echo "sudo mkdir /opt/sunsaver/bin"
        sudo mkdir -p /opt/sunsaver/bin
    fi
    for i in sunsaver sunsaverEEPROM sunsaverlog powerbutton
    do
        echo "cc -g -o $i $i.c -I/opt/sunsaver/include -L/opt/sunsaver/lib -Wl,-rpath=/opt/sunsaver/lib -lmodbus"
        cc -g -o $i $i.c -I/opt/sunsaver/include -L/opt/sunsaver/lib -Wl,-rpath=/opt/sunsaver/lib -lmodbus
        echo "sudo cp $i /opt/sunsaver/bin/"
        sudo cp $i /opt/sunsaver/bin/
    done
