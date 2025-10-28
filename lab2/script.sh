echo the name of this script is $0
echo the first argument is $1
echo a list of all the arguments is $*
echo this script places the date into a temporary file called $1.$$
date > $1.$$
ls $1.$$
echo contents of this file: 
cat $1.$$
rm $1.$$
