//
//  Copyright (c), JoaoAJMatos, 2020
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define FILEPATH_SIZE BUFFER_SIZE

char* get_real_path(char*);                     // Converts a relative path to an absolute path
int   get_file_bytes(char*);                    // Returns the number of bytes in a file
int   load_file_contents(char*, char**, int);   // Loads the contents of a file into a buffer
int   write_file_contents(char*, char*, int);   // Writes the contents of a buffer to a file
int   delete_random_bytes(char*, int);          // Deletes random bytes from a buffer
int   random_int(int, int);                     // Returns a random integer between two values
char* get_filename(char*);                      // Returns the filename from a path
int   delete_file(char*);                       // Deletes a file


int main(int argc, char *argv[])
{
    char* filepath, *buffer;
    int   file_bytes, bytes_deleted;

    // Seed the random number generator
    srand(time(NULL));

    // Ensure the correct number of arguments were passed
    if (argc < 2) {
        printf("Usage: %s <filepath>", argv[0]);
        return 1;
    }

    // Get the absolute path of the file
    filepath = get_real_path(argv[1]);
    if (filepath == NULL) {
        printf("Error: File not found\n");
        return 1;
    }

    // Get the number of bytes in the file
    file_bytes = get_file_bytes(filepath);

    // Load the file contents into a buffer
    buffer = malloc(file_bytes);
    if (load_file_contents(filepath, &buffer, file_bytes) < 0) {
        printf("Error: Could not load file contents\n");
        return 1;
    }

    // Delete random bytes from the buffer
    bytes_deleted = delete_random_bytes(buffer, file_bytes);
    if (bytes_deleted < file_bytes / 2) {
        printf("Error: Could not randomly delete 50%% of the bytes in the file. Balanced could not be established\n");
        return 1;
    }

    // Delete the input file
    if (delete_file(filepath) < 0) {
        printf("Error: Could not delete file\n");
        return 1;
    }

    // Write the buffer contents to the file
    if (write_file_contents(filepath, buffer, file_bytes) < 0) {
        printf("Error: Could not write file contents\n");
        return 1;
    }

    // Free the buffer
    free(buffer);

    printf("The file %s is now perfectly balanced, as all things should be.\n", get_filename(filepath));
    printf("%d bytes have been sacrificed for the sake of balance in the Multiverse\n", bytes_deleted);

    return 0;
}

/**
 * @brief Converts a given relative path into an absolute path
 * @param path
 * @return
 */
char* get_real_path(char* path)
{
    char* real_path = (char*) malloc(FILEPATH_SIZE);
    if (real_path == NULL) {
        return NULL;
    }

    if (realpath(path, real_path) == NULL) {
        return NULL;
    }

    return real_path;
}

/**
 * @brief Returns the number of bytes in a file
 * @param filename
 * @return byte count
 */
int get_file_bytes(char* filename)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return -1;
    }

    fseek(file, 0, SEEK_END);
    int file_size = ftell(file);
    fclose(file);

    return file_size;
}


/**
 * @brief Loads the contents of a file into a buffer
 * @param filename
 * @param output_buffer
 * @param buffer_size
 * @return number of bytes read
 */
int load_file_contents(char* filename, char** output_buffer, int size)
{
    int file_size = get_file_bytes(filename);

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return -1;
    }

    *output_buffer = (char*) malloc(size);
    if (*output_buffer == NULL) {
        return -1;
    }

    int read_bytes = fread(*output_buffer, 1, size, file);
    if (read_bytes == 0) {
        return -1;
    }

    fclose(file);
    return read_bytes;
}

/**
 * @brief Writes the contents of a buffer to a file
 * @param filename
 * @param buffer
 * @param size
 * @return number of bytes written
 */
int write_file_contents(char* filename, char* buffer, int size)
{
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        return -1;
    }

    int written_bytes = fwrite(buffer, 1, size, file);
    if (written_bytes == 0) {
        return -1;
    }

    fclose(file);
    return written_bytes;
}

/**
 * @brief Randomly deletes bytes from a buffer
 * @param buffer
 * @param size
 * @return number of bytes deleted
 */
int delete_random_bytes(char* buffer, int size)
{
    int bytes_deleted = 0;

    do {
        int index = random_int(0, size);
        if (buffer[index] != 0) {
            buffer[index] = 0;
            bytes_deleted++;
        }
    } while (bytes_deleted < size / 2);

    return bytes_deleted;
}

/**
 * @brief Returns a random integer between two values
 * @param min
 * @param max
 * @return random integer
 */
int random_int(int min, int max)
{
    return (rand() % (max - min + 1)) + min;
}

char* get_filename(char* path)
{
    char* filename = strrchr(path, '/');
    if (filename == NULL) {
        return path;
    }

    return filename + 1;
}

int delete_file(char* filename)
{
    return remove(filename);
}