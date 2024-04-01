#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "coord.h" // Ensure this header defines Coordinate structure and NAME_MAX

#define DEFAULT_FILE_NAME "saved_coords.ec"

void clear_screen() {
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
}

int main() {
    Coordinate *coords = NULL;
    int size = 0;
    int choice;
    char name[256];
    int x, y, z;
    char input[256]; // Buffer to store user input

    if (ec_load_coords(&coords, &size, DEFAULT_FILE_NAME) != 0) {
        printf("Failed to load coordinates.\n");
    }

    while (1) {
        printf("\nEzCoords Dashboard\n");
        printf("1. Add Coordinate\n");
        printf("2. Delete Coordinate\n");
        printf("3. Edit Coordinate\n");
        printf("4. List Coordinates\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        fgets(input, sizeof(input), stdin); // Read the entire line
        if (sscanf(input, "%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue; // Skip the rest of the loop if input is not a number
        }

        switch (choice) {
            case 1:
                printf("Enter name (no space) and coordinates (x y z): ");
                fgets(input, sizeof(input), stdin); 
                // Extract the data from the input
                if (sscanf(input, "%255s %d %d %d", name, &x, &y, &z) == 4) { 
                    if (ec_add_coord(&coords, &size, name, x, y, z) != 0) {
                        printf("Failed to add coordinate.\n");
                    }
                } else {
                    printf("Invalid input format.\n");
                }
                break;
            case 2:
                printf("Enter name of the coordinate to delete: ");
                fgets(name, NAME_MAX, stdin);
                name[strcspn(name, "\n")] = 0; 
                if (ec_del_coord(&coords, &size, name) != 0) {
                    printf("Failed to delete coordinate.\n");
                }
                break;
            case 3:
                printf("Enter name and new coordinates (x y z): ");
                fgets(input, sizeof(input), stdin);
                if (sscanf(input, "%255s %d %d %d", name, &x, &y, &z) == 4) {
                    if (ec_edit_coord(coords, size, name, x, y, z) != 0) {
                        printf("Failed to edit coordinate.\n");
                    }
                } else {
                    printf("Invalid input format.\n");
                }
                break;
            case 4:
                ec_list_coords(coords, size);
                break;
            case 5:
                free(coords);
                printf("Exiting...\n");
                return 0;
            default:
                printf("Invalid choice. Please enter a number between 1 and 7.\n");
        }

        if (ec_save_coords(coords, size, DEFAULT_FILE_NAME) != 0) {
            printf("Failed to save coordinates.\n");
        }
    }

    return 0;
}
