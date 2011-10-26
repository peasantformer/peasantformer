all:
	cd mouse && make
	javac Main.java mouse/MouseParser.java mouse/MouseParserSymantics.java -cp mouse/Mouse-1.4.jar

clean:
	find . -name '*.class' -delete
	cd mouse && make clean
