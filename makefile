make:
	gcc main.c TADs/AVL.c TADs/filaPrioridade.c TADs/historico.c TADs/IO.c -o sus

run:
	./sus

clean:
	rm -f sus
