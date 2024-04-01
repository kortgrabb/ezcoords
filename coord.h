// coord.h
#ifndef COORD_H
#define COORD_H

#define NAME_MAX 21

typedef struct {
	char name[NAME_MAX];
	int x, y, z;
} Coordinate;

int ec_add_coord(Coordinate **coords, int *size, char *name, int x, int y, int z);
int ec_del_coord(Coordinate **coords, int *size, char *name);
int ec_edit_coord(Coordinate *coords, int size, char *name, int newX, int newY, int newZ);
int ec_list_coords(Coordinate *coords, int size);
int ec_save_coords(Coordinate *coords, int size, const char *filename);
int ec_load_coords(Coordinate **cords, int *size, const char *filename);

#endif
