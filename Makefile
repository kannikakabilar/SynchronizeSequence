all : syncseq sub_program

syncseq : syncseq.c
	gcc -Wall -g -o syncseq syncseq.c

sub_program : sub_program.c
	gcc -Wall -g -o sub_program sub_program.c

clean :
	rm syncseq sub_program