// coord.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "coord.h"

// Adds a new coordinate to the coords pointer
int ec_add_coord(Coordinate **coords, int *size, char *name, int x, int y, int z) {
    Coordinate *newCoords = realloc(*coords, (*size + 1) * sizeof(Coordinate));
    if (newCoords == NULL) {
        printf("Memory allocation failed\n");
        return 1; // Failure due to memory allocation error
    }

    *coords = newCoords;
    strncpy((*coords)[*size].name, name, NAME_MAX - 1);
    (*coords)[*size].name[NAME_MAX - 1] = '\0'; // Ensure the string is null-terminated
    (*coords)[*size].x = x;
    (*coords)[*size].y = y;
    (*coords)[*size].z = z;
    (*size)++;

    return 0; // Success
}

// Deletes the coord by name
int ec_del_coord(Coordinate **coords, int *size, char *name) {
    for (int i = 0; i < *size; i++) {
        if (strcmp((*coords)[i].name, name) == 0) {
            for (int j = i; j < *size - 1; j++) {
                (*coords)[j] = (*coords)[j + 1]; // Shift elements down
            }
            (*size)--;
            *coords = realloc(*coords, (*size) * sizeof(Coordinate));
            if (*coords == NULL && *size > 0) { // Check for realloc failure
                printf("Memory allocation failed\n");
                return 1; // Failure due to memory allocation error
            }
            return 0; // Success
        }
    }
    return 2; // Failure due to not finding the name
}

// Changes the location of the named coord
int ec_edit_coord(Coordinate *coords, int size, char *name, int newX, int newY, int newZ) {
    for (int i = 0; i < size; i++) {
        if (strcmp(coords[i].name, name) == 0) {
            coords[i].x = newX;
            coords[i].y = newY;
            coords[i].z = newZ;
            return 0; // Success
        }
    }
    return 1; // Failure due to not finding the name
}

// Lists the coordinates
int ec_list_coords(Coordinate *coords, int size) {
    printf("+------------------------------+\n");
    for (int i = 0; i < size; i++) {
        printf("%s: %d %d %d\n", coords[i].name, coords[i].x, coords[i].y, coords[i].z);
    }
    printf("+------------------------------+\n");

    return 0; // Success
}

int ec_save_coords(Coordinate *coords, int size, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 1; // Failure due to file opening error
    }

    for (int i = 0; i < size; i++) {
        fprintf(file, "%s | %d | %d | %d\n", coords[i].name, coords[i].x, coords[i].y, coords[i].z);
    }

    fclose(file); // Ensure file is closed before returning
    return 0; // Success
}

int ec_load_coords(Coordinate **coords, int *size, const char* filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 1; // Failure due to file opening error
    }

    char name[NAME_MAX];
    int x, y, z;

    while(fscanf(file, "%99s | %d | %d | %d", name, &x, &y, &z) == 4) {
        if (ec_add_coord(coords, size, name, x, y, z) != 0) {
            fclose(file); // Close file on failure
            return 2; // Failure due to memory allocation error
        }
    }

    fclose(file);
    return 0; // Success
}
