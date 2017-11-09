build: hangman.c
	gcc hangman.c -o hangman -lncurses -lmenu -Wall 

hangman: hangman.c
	gcc hangman.c -o hangman -lncurses -lmenu -Wall

clean:
	rm -f hangman
