cd $1 
echo @%@%@%@%@@%@%@%@%@%@ $1/$2.in
gamos $2.in |& tee out.$2
mv gamos.log ../gamos.$2.log
mv gamos_error.log ../gamos_error.$2.log
cd -
