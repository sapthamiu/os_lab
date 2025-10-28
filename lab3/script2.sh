echo "Enter n: "
read n
i=1
until [ $i -gt $((2 * n)) ] 
do
echo $i
i=`expr $i + 2`
done
