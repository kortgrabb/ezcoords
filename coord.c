// coord.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "coord.h"

// Adds a new coordinate to the coords pointer
int ec_add_coord(Coordinate **coords, int *size, char *name, int x, int y, int z) {

    // Check for dupes
    for (int i = 0; i < *size; i++) {
        if (strcmp((*coords)[i].name, name) == 0) {
            printf("coord with same name exists!\n");
            return 1; 
        }
    }

    // Add 1 to the size of (coords) but keep the data of
    Coordinate *newCoords = realloc(*coords, (*size + 1) * sizeof(Coordinate));
    if (newCoords == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    *coords = newCoords;

    strncpy((*coords)[*size].name, name, NAME_MAX - 1);

    // Ensure the string is null-terminated
    (*coords)[*size].name[NAME_MAX - 1] = '\0'; 
    (*coords)[*size].x = x;
    (*coords)[*size].y = y;
    (*coords)[*size].z = z;
    (*size)++;

    return 0; 
}

// Deletes the coord by name
int ec_del_coord(Coordinate **coords, int *size, char *name) {
    for (int i = 0; i < *size; i++) {
        if (strcmp((*coords)[i].name, name) == 0) {
            for (int j = i; j < *size - 1; j++) {
                // Shift elements down
                (*coords)[j] = (*coords)[j + 1]; 
            }
            (*size)--;
            *coords = realloc(*coords, (*size) * sizeof(Coordinate));
            if (*coords == NULL && *size > 0) { 
                printf("Memory allocation failed\n");
                return 1; 
            }
            return 0; 
        }
    }
    return 2; 
}

// Changes the location of the named coord
int ec_edit_coord(Coordinate *coords, int size, char *name, int newX, int newY, int newZ) {
    for (int i = 0; i < size; i++) {
        if (strcmp(coords[i].name, name) == 0) {
            coords[i].x = newX;
            coords[i].y = newY;
            coords[i].z = newZ;
            return 0; 
        }
    }
    return 1; 
}

// Lists the coordinates
int ec_list_coords(Coordinate *coords, int size) {
    printf("+------------------------------+\n");
    for (int i = 0; i < size; i++) {
        printf("%s: %d %d %d\n", coords[i].name, coords[i].x, coords[i].y, coords[i].z);
    }
    printf("+------------------------------+\n");

    return 0; 
}


// Saves the coords to a file
int ec_save_coords(Coordinate *coords, int size, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 1; 
    }

    for (int i = 0; i < size; i++) {
        fprintf(file, "%s | %d | %d | %d\n", coords[i].name, coords[i].x, coords[i].y, coords[i].z);
    }

    fclose(file); 
    return 0; 
}

// Loads the coords from a file
int ec_load_coords(Coordinate **coords, int *size, const char* filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 1; 
    }

    char name[NAME_MAX];
    int x, y, z;

    while(fscanf(file, "%s | %d | %d | %d", name, &x, &y, &z) == 4) {
        if (ec_add_coord(coords, size, name, x, y, z) != 0) {
            fclose(file);
            return 2; 
        }
    }

    fclose(file);
    return 0; 
}
