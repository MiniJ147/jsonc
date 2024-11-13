files = parsers/parser.c parsers/lexer.c main.c
output = ../bin/a.out

run:
	gcc $(files) -o $(output) && ./$(output) && rm -f $(output)

build:
	gcc $(files) -o $(output)

check-leak:
	valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         ./$(output)
