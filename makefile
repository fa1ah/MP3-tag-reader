main: main.o tag_editor.o
	gcc main.o tag_editor.o -o main

main.o: main.c tag_editor.h
	gcc -c main.c

tag_editor.o: tag_editor.c tag_editor.h
	gcc -c tag_editor.c

clean:
	rm -f main main.o tag_editor.o
