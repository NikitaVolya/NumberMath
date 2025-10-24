main : vector2i.o field_cell.o game_field.o custom_output.o serializer.o game.o game_menu.o main.o
	gcc -o main vector2i.o field_cell.o game_field.o custom_output.o serializer.o game.o game_menu.o main.o
main.o : main.c game_menu.h
	gcc -o main.o  -W -Wall -std=c89 -O2 -pedantic -c main.c
game_menu.o : game_menu.c game_menu.h game.h
	gcc -o game_menu.o  -W -Wall -std=c89 -O2 -pedantic -c game_menu.c
game.o : game.c game.h game_field.h custom_output.h serializer.h
	gcc -o game.o  -W -Wall -std=c89 -O2 -pedantic -c game.c
serializer.o : serializer.c serializer.h game_field.h field_cell.h
	gcc -o serializer.o -W -Wall -std=c89 -O2 -pedantic -c serializer.c
custom_output.o : custom_output.c custom_output.h vector2i.h
	gcc -o custom_output.o  -W -Wall -std=c89 -O2 -pedantic -c custom_output.c
game_field.o : game_field.c game_field.h field_cell.h vector2i.h
	gcc -o game_field.o  -W -Wall -std=c89 -O2 -pedantic -c game_field.c
field_cell.o : field_cell.c field_cell.h
	gcc -o field_cell.o  -W -Wall -std=c89 -O2 -pedantic -c field_cell.c
vector2i.o : vector2i.c vector2i.h
	gcc -o vector2i.o  -W -Wall -std=c89 -O2 -pedantic -c vector2i.c
clean :
	rm -rf *.o *~ main