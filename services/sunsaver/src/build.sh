#!/bin/sh
PREFIX="/opt/outpostserver"
CLFAGS="-g -I$PREFIX/include"
LDFLAGS="-L$PREFIX/lib -Wl,-rpath=$PREFIX/lib -lmodbus"
    if [ ! -d $PREFIX/bin ]
    then
        echo "sudo mkdir $PREFIX/bin"
        sudo mkdir -p $PREFIX/bin
    fi
    for i in sunsaver sunsaverEEPROM sunsaverlog 
    do
        echo "cc $CFLAGS -o $i $i.c $LDFLAGS"
        cc $CFLAGS -o $i $i.c $LDFLAGS
        echo "sudo install $i $PREFIX/bin/"
        sudo install $i $PREFIX/bin/
    done
