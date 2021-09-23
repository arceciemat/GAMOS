ls |grep Tutorial | while read tuto; do
    cat $tuto/runAllQA | while read line; do
	#	run1qa.sh HSTutorial exercise2 exercise2
	dir=`echo $line | awk '{print $2}'`
	subdir=`echo $line | awk '{print $3}'`
	exer=`echo $line | awk '{print $4}'`
	echo    $dir/$subdir/qatest.summary.$exer
	#	emacs $dir/$subdir/qatest.summary.$exer
	evince  $dir/$subdir/qatest*${exer}*pdf
	
    done
done
			 
