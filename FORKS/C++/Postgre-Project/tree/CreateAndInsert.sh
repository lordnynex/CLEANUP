#!/bin/bash
#This executes all C++ executables necessary to create and insert tables into the database "postgres"
./createTable.out
wc -l HHV2PUB.CSV PERV2PUB.CSV | hpinsert.out
wc -l VEH2PUB.CSV | vinsert.out
wc -l DAYV2PUB.CSV | dinsert.out
./EIA.out
echo -en "\007"
echo -en "\007"
echo -en "\007"
