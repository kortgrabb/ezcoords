#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "coord.h" 

#define DEFAULT_FILE_NAME "saved_coords.ec"
#define DOCUMENTS_FOLDER_ENV_VAR "USERPROFILE"

void clear_screen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

char *get_documents_folder()
{
    char *documents_folder = getenv(DOCUMENTS_FOLDER_ENV_VAR);
    if (documents_folder == NULL)
    {
        fprintf(stderr, "Failed to get the Documents folder path.\n");
        exit(1);
    }
    return documents_folder;
}

int main()
{
    Coordinate *coords = NULL;
    int size = 0;
    int choice;
    char name[256];
    int x, y, z;
    char input[256];
    char file_path[512];

    char *documents_folder = get_documents_folder();
    snprintf(file_path, sizeof(file_path), "%s/%s", documents_folder, DEFAULT_FILE_NAME);

    if (ec_load_coords(&coords, &size, file_path) != 0)
    {
        printf("Failed to load coordinates.\n");
    }

    clear_screen();
    while (1)
    {
        printf("\nEzCoords Dashboard\n");
        printf("1. Add Coordinate\n");
        printf("2. Delete Coordinate\n");
        printf("3. Edit Coordinate\n");
        printf("4. List Coordinates\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");

        // Read the entire line input
        fgets(input, sizeof(input), stdin); 
        if (sscanf(input, "%d", &choice) != 1)
        {
            printf("Invalid input. Please enter a number.\n");
            continue; 
        }

        switch (choice)
        {
        case 1:
            printf("Enter name (no space) and coordinates (x y z): ");
            fgets(input, sizeof(input), stdin);
            // Extract the data from the input
            if (sscanf(input, "%s %d %d %d", name, &x, &y, &z) == 4)
            {
                if (ec_add_coord(&coords, &size, name, x, y, z) != 0)
                {
                    printf("Failed to add coordinate.\n");
                }
            }
            else
            {
                printf("Invalid input format.\n");
            }
            break;
        case 2:
            printf("Enter name of the coordinate to delete: ");
            fgets(name, NAME_MAX, stdin);
            name[strcspn(name, "\n")] = 0;
            if (ec_del_coord(&coords, &size, name) != 0)
            {
                printf("Failed to delete coordinate.\n");
            }
            break;
        case 3:
            printf("Enter name and new coordinates (x y z): ");
            fgets(input, sizeof(input), stdin);
            if (sscanf(input, "%s %d %d %d", name, &x, &y, &z) == 4)
            {
                if (ec_edit_coord(coords, size, name, x, y, z) != 0)
                {
                    printf("Failed to edit coordinate.\n");
                }
            }
            else
            {
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

        if (ec_save_coords(coords, size, file_path) != 0)
        {
            printf("Failed to save coordinates.\n");
        }

        printf("\nPress enter to continue...");
        // wait for input and clear screen
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
        {
        }
        clear_screen();
    }

    return 0;
}
