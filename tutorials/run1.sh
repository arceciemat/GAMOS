#!/bin/bash
source $GAMOSINSTALL/config/confgamos.sh
cd $1 
echo @%@%@%@%@%@ $1/$2.in
gamos $2.in 2>&1 | tee out.$2
mv gamos.log ../gamos.$2.log
mv gamos_error.log ../gamos_error.$2.log
cd -
