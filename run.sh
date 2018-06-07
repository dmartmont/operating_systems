#!/bin/bash
./examples/rmmem memoria ;
./examples/rmmem config ;
make controlewe ;
make interewe ;
./build/bin/controlewe memoria memoria.mew ;
./build/bin/interewe memoria instructions.bew