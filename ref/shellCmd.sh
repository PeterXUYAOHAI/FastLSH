hadoop classpath --glob


export CLASSPATH=...


head -300000 dataset.csv

//cut columns
cut -f1,2,3,4,5,7 file

//check how many columns
head -1 myfile.csv | sed 's/[^,]//g' | wc -c

//check how many lines
wc -l ..
