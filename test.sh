#!/bin/sh
echo 'Testing library ...'
echo -n "-> "
lua -e 'print(require("luacv"))'
echo 'Trying to run sample script ...'
echo -n "-> "
./samples/drawing.lua
if [ "$?" == 0 ]; then 
  echo "Seems to running fine."
  echo "You can go to $PWD to see more of demo scripts."
else
  echo "Some errors occurred."
  echo "If you see this message send me email eplease at jiri<dot>prymus<at>gmail<dot>com with description/output."
fi
