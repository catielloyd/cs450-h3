branch = master

sudoku.x: sudoku.c
	gcc -c -o $@ $<

clean:
	rm -f *.x *.o *~

run: sudoku.x
	clear
	./sudoku.x < testInput

push: clean
	clear
	git add .
	git commit -m $(m)
	git push origin $(branch)
