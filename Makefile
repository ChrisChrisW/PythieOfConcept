localhost = localhost:8888
javaCMD = java -jar pythieofconcept.jar
gccww = gcc -Wall -Wextra -std=c99

all : compile main

init : 
	$(javaCMD) -b3 -b4

main : jars html
	$(javaCMD) C:code/main -b2 -b3

help : jars html
	$(javaCMD) -h

php : html
	php -S $(localhost) &
	firefox $(localhost)/html/index.php

compile: code
	$(gccww) -o code/main code/code.c

git :
	git add .
	git commit -am "update"
	git push origin master
	git status