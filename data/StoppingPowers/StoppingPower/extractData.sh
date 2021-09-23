ls *html | sed s/".html"//g | while read file; do
    echo FILE $file
    grep "td al" "${file}"".html" | sed s/'<td align="center">'//g | sed s/'<\/td>'/' '/g | sed s/'<\/tr>'//g > StoppingPower."${file}".txt
done
