ls *html | sed s/".html"//g | while read file; do
    echo FILE $file
    echo "// Stopping powers form https://physics.nist.gov/PhysRefData/Star/Text/PSTAR.html" > StoppingPower."${file}".txt
    echo "// SP: MeV cm2 / g" >> StoppingPower."${file}".txt
    echo "// Range: g/cm2" >> StoppingPower."${file}".txt
    echo "// Kinetic Energy / SP Electronic / SP Nuclear / SP  Total / Range CSDA / Range Projected / Range Detour Factor = Projected / CSDA " >> StoppingPower."${file}".txt
    grep "td al" "${file}"".html" | sed s/'<td align="center">'//g | sed s/'<\/td>'/' '/g | sed s/'<\/tr>'//g >> StoppingPower."${file}".txt
done
