make:
	gcc main.c TADs/AVL.c TADs/filaPrioridade.c -o sus

run:
	./sus

clean:
	rm -f sus
