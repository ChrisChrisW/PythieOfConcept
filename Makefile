localhost = localhost:8888
javaCMD = java -jar pythieofconcept.jar
gccww = gcc -Wall -Wextra -std=c99

all : compile main php

init : 
	$(java CMD) -b3 -b2 

main : jars html
	$(javaCMD) C:code/main -b3 -b2

php : html
	php -S $(localhost) &
	firefox $(localhost)/html/index.php

compile: code
	$(gccww) -o code/main code/main.c

git :
	git add .
	git commit -am "update"
	git push origin master
	git status