#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "coord.h"

#define BUFFER_SIZE 256
#define DEFAULT_FILENAME "coords"

void clear_screen() {
    // This is platform dependent. For Unix/Linux, use "clear"; for Windows, use "cls".
    system("clear"); // or system("cls") on Windows
}

void print_menu() {
    clear_screen(); // Clear screen before printing the menu to reduce clutter
    printf("\n======= Dashboard =======\n");
    printf("1. Add a coordinate\n");
    printf("2. Delete a coordinate\n");
    printf("3. Edit a coordinate\n");
    printf("4. List all coordinates\n");
    printf("5. Save coordinates to file\n");
    printf("6. Load coordinates from file\n");
    printf("7. Exit\n");
    printf("=========================\n");
    printf("Enter your choice: ");
}

int main() {
    Coordinate *coords = NULL;
    int size = 0;
    int choice;
    char buffer[BUFFER_SIZE];
    char name[NAME_MAX];
    int x, y, z;

	ec_load_coords(&coords, &size, DEFAULT_FILENAME);

    while (1) {
        print_menu();
        fgets(buffer, BUFFER_SIZE, stdin);
        if (sscanf(buffer, "%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }

        switch (choice) {
            case 1:
                printf("Enter coordinate name: ");
                fgets(name, NAME_MAX, stdin);
                name[strcspn(name, "\n")] = '\0'; // Remove newline character
                printf("Enter x, y, z coordinates: ");
                fgets(buffer, BUFFER_SIZE, stdin);
                if (sscanf(buffer, "%d %d %d", &x, &y, &z) == 3) {
                    ec_add_coord(&coords, &size, name, x, y, z);
                    printf("Coordinate added successfully.\n");
                    ec_save_coords(coords, size, DEFAULT_FILENAME);
                } else {
                    printf("Invalid input for coordinates.\n");
                }
                break;
            case 2:
                printf("Enter the name of the coordinate to delete: ");
                fgets(name, NAME_MAX, stdin);
                if (ec_del_coord(&coords, &size, name)) {
                    printf("Coordinate deleted successfully.\n");
                    ec_save_coords(coords, size, DEFAULT_FILENAME);
                } else {
                    printf("Coordinate not found.\n");
                }
                break;
            case 3:
                printf("Enter the name of the coordinate to edit: ");
                fgets(name, NAME_MAX, stdin);
                printf("Enter new x, y, z coordinates: ");
                fgets(buffer, BUFFER_SIZE, stdin);
                if (sscanf(buffer, "%d %d %d", &x, &y, &z) == 3) {
                    if (ec_edit_coord(coords, size, name, x, y, z)) {
                        printf("Coordinate updated successfully.\n");
                        ec_save_coords(coords, size, DEFAULT_FILENAME);
                    } else {
                        printf("Coordinate not found.\n");
                    }
                } else {
                    printf("Invalid input for new coordinates.\n");
                }
                break;
            case 4:
                ec_list_coords(coords, size);
                break;
            case 5:
                ec_save_coords(coords, size, DEFAULT_FILENAME);
                break;
            case 6:
                printf("Enter the filename to load coordinates: ");
                fgets(buffer, BUFFER_SIZE, stdin);
                // Remove newline character from fgets input
                buffer[strcspn(buffer, "\n")] = 0;
                if (ec_load_coords(&coords, &size, buffer) == 0) {
                    printf("Coordinates loaded from \"%s\" successfully.\n", buffer);
                } else {
                    printf("Failed to load coordinates from \"%s\".\n", buffer);
                }
                break;
            case 7:
                printf("Exiting...\n");
                free(coords);
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
        printf("Press Enter to continue...");
        fgets(buffer, BUFFER_SIZE, stdin); // Pause for user to see the message
    }

    return 0;
}
