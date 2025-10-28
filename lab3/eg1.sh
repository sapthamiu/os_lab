declare -a teams
teams[0]="India"
teams[1]="England"
teams[2]="Nepal"
echo "There are ${#teams[*]} teams"
echo "They are: ${teams[*]}"
unset teams[1]

