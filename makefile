
sudoku.x: sudoku.c
	gcc sudoku.c -o sudoku.x -lpthread

clean:
	rm -f *.x *.o *~

run: sudoku.x
	clear
	@./sudoku.x

test: sudoku.x
	@echo
	@echo
	@echo
	@echo
	@echo Testing valid sudoku:
	@echo
	@./sudoku.x < validInput
	@echo
	@echo
	@echo Testing invalid sudoku:
	@echo
	@./sudoku.x < invalidInput

testValid: sudoku.x
	./sudoku.x < validInput

testInvalid: sudoku.x
	@./sudoku.x <invalidInput

push: clean
	@echo
	@echo
	git add .
	git commit
	git push origin HEAD

