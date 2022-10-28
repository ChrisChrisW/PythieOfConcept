localhost = localhost:8888
javaCMD = java -jar pythieofconcept.jar
gccww = gcc -Wall -Wextra -std=c99

all : compile main php

init : 
	$(java CMD) -b3 -b2 

main : jars html
	$(javaCMD) -b3 -b2  C:code/main

php :
	php -S $(localhost) &
	firefox $(localhost)/html/index.php

compile: code
	$(gccww) -o code/main code/main.c