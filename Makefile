main : game_objects/vector2i.o game_objects/field_cell.o game_objects/game_field.o custom_output.o serializer.o console_game_strategy.o game.o  main.o
	gcc -o main game_objects/vector2i.o game_objects/field_cell.o game_objects/game_field.o custom_output.o serializer.o console_game_strategy.o game.o main.o
main.o : main.c console_game_strategy.h
	gcc -o main.o  -W -Wall -std=c89 -O2 -pedantic -c main.c
game.o : game.c game.h game_objects/game_field.h console_game_strategy.h serializer.h
	gcc -o game.o  -W -Wall -std=c89 -O2 -pedantic -c game.c
console_game_strategy.o : console_game_strategy.c console_game_strategy.h custom_output.h game_objects/field_cell.h serializer.h game.h
	gcc -o console_game_strategy.o -W -Wall -std=c89 -O2 -pedantic -c console_game_strategy.c
serializer.o : serializer.c serializer.h game_objects/game_field.h game_objects/field_cell.h
	gcc -o serializer.o -W -Wall -std=c89 -O2 -pedantic -c serializer.c
custom_output.o : custom_output.c custom_output.h game_objects/vector2i.h
	gcc -o custom_output.o  -W -Wall -std=c89 -O2 -pedantic -c custom_output.c
game_field.o : game_objects/game_field.c game_objects/game_field.h game_objects/field_cell.h game_objects/vector2i.h
	gcc -o game_objects/game_field.o  -W -Wall -std=c89 -O2 -pedantic -c game_objects/game_field.c
field_cell.o : game_objects/field_cell.c game_objects/field_cell.h
	gcc -o game_objects/field_cell.o  -W -Wall -std=c89 -O2 -pedantic -c game_objects/field_cell.c
vector2i.o : game_objects/vector2i.c game_objects/vector2i.h
	gcc -o game_objects/vector2i.o  -W -Wall -std=c89 -O2 -pedantic -c game_objects/vector2i.c
clean :
	rm -rf *.o main
	rm -rf game_objects/*.o