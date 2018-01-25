X=$(screen -wi | grep -oh "[0-9].*MBP")

screen -S $X -X quit
