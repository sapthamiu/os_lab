echo "Enter a, b, c: "
read a
read b
read c
d=$(( b * b - 4 * a * c ))
sqrt(){
echo "scale=3; sqrt($1)" | bc -l
}
sign=$(echo "$d" | awk '{if ($1 > 0) print 1; else if ($1 == 0) print 0; else print -1}'

case $sign in
1) 
	r1=$(echo "scale=3; (-$b + $(sqrt "$d")) / (2 * $a)" | bc -l)
	r2=$(echo "scale=3; (-$b - $(sqrt "$d")) / (2 * $a)" | bc -l)
	echo "Real and distinct roots: $r1, $r2"
	;;
0) 
	r=$(echo "scale=3; (-$b) / (2 * $a)" | bc -l)
	echo "Real and equal roots: $r, $r"
	;;
-1) 
	re=$(echo "scale=3; (-$b) / (2 * $a)" | bc -l)
	im=$(echo "scale=3; $(sqrt -$d) / (2 * $a)" | bc -l)
	echo "Complex and distinct roots: $re + ${im}i, $re - ${im}i"
	;;
esac
