localhost = localhost:8888
javaCMD = java -jar pythieofconcept.jar
gccww = gcc -Wall -Wextra -std=c99

mycodefile = christophe_Pythie_Of_Concept.c


# ---

all : compile main

init : 
	$(javaCMD) -b2 -b3

main : jars html
	$(javaCMD) C:code/main -b2 -b3

help : jars html
	$(javaCMD) -h

php : html
	php -S $(localhost) &
	firefox $(localhost)/html/index.php

compile: code
	$(gccww) -o code/main code/$(mycodefile)

git :
	git add .
	git commit -am "update"
	git push origin master
	git status