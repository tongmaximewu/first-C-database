#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include "database.h"

// function to validate a handle
int validate_handle(char const *handle) {
    if (handle == NULL) {
        return 0;
    }
    if (strlen(handle) > MAX_HANDLE) {
        return 0;
    }
    if (strchr(handle, ',') != NULL || strchr(handle, '\n') != NULL) {
        return 0;
    }
    if (!(strlen(handle) >= 5 && strlen(handle) <= 32)) {
        return 0;
    }
    if (!(handle[0] == '@')) {
        return 0;
    }
    return 1;
}

// function to validate a comment
int validate_comment(char const *comment) {
    if (comment == NULL) {
        return 0;
    }
    if (strlen(comment) > MAX_COMMENT) {
        return 0;
    }
    if (strchr(comment, ',') != NULL || strchr(comment, '\n') != NULL) {
        return 0;
    }
    return 1;
}

// main loop of the program
int main_loop(Database *db) {
    char *input_line = NULL;
    size_t line_length = 0;
    ssize_t num_chars;
    char *command_token;
    int changes_made = 0;

    printf("loaded %d records.\n", db->size);

    while (1) {
        printf("> ");
        num_chars = getline(&input_line, &line_length, stdin);
        if (num_chars == -1) {
            printf("exit");
            break;
        }

        if (input_line[num_chars - 1] == '\n') {
            input_line[num_chars - 1] = '\0';
        }

        command_token = strtok(input_line, " ");

        if (command_token == NULL) {
            printf("please input a valid command.\n");
            continue;
        }

        if (strcmp(command_token, "list") == 0) {
            // print records
            printf("%-20s | %-10s | %-20s | %s\n", "handle", "followers", "last modified", "comment");
            printf("%-20s | %-10s | %-20s | %s\n", "--------------------", "----------",
                                                   "--------------------", "----------------------");

            for (int i = 0; i < db->size; i++) {
                const Record *current_record = db_index(db, i);
                char time_format[21]; // buffer to store formatted time
                time_t timestamp = (time_t)current_record->timestamp;
                struct tm *tm_info = localtime(&timestamp);
                strftime(time_format, sizeof(time_format), "%Y-%m-%d %H:%M", tm_info);
                printf("%-20.20s | %-10lu | %-20.20s | %.40s\n", current_record->handle,
                                current_record->followers, time_format, current_record->comment);
            }

        } else if (strcmp(command_token, "add") == 0) {
            // add new record
            char *new_handle = strtok(NULL, " ");
            char *followers_str = strtok(NULL, " ");
            long unsigned int new_followers;

            if (!new_handle || !followers_str) {
                printf("input handle AND followers\n");
                continue;
            }

            if (validate_handle(new_handle) == 0) {
                printf("invalid handle\n");
                continue;
            }
            if (db_lookup(db, new_handle)) {
                printf("%s already exists.\n", new_handle);
                continue;
            }

            char *extra_arg = strtok(NULL, " ");
            if (extra_arg) {
                printf("too many arguments.\n");
                continue;
            }

            // convert followers to long unsigned int 
            // and check if it is a valid number by checking if the endptr is null
            char *endptr;
            new_followers = strtol(followers_str, &endptr, 10);
            if (*endptr != '\0') {
                printf("error: follower count must be an integer\n");
                continue;
            }

            printf("comment > ");
            char *new_comment = NULL;
            size_t comment_length = 0;
            getline(&new_comment, &comment_length, stdin);

            // remove the newline character from the comment
            size_t comment_size = strlen(new_comment);
            if (comment_size > 0 && new_comment[comment_size - 1] == '\n') {
                new_comment[comment_size - 1] = '\0';
            }
            if (validate_comment(new_comment) == 0) {
                printf("invalid comment\n");
                continue;
            }

            Record new_record;

            strncpy(new_record.handle, new_handle, sizeof(new_record.handle) - 1);
            new_record.followers = new_followers;
            strncpy(new_record.comment, new_comment, sizeof(new_record.comment) - 1);
            new_record.timestamp = time(NULL);

            db_append(db, &new_record);
            printf("record added succesfully!\n");

            free(new_comment);
            changes_made = 1;

        } else if (strcmp(command_token, "update") == 0) {
            // update existing record
            char *handle_to_update = strtok(NULL, " ");
            char *updated_followers_str = strtok(NULL, " ");
            long unsigned int updated_followers = 0;

            if (!handle_to_update || !updated_followers_str) {
                printf("error: update handle followers\n");
                continue;
            }

            char *extra_arg = strtok(NULL, " ");
            if (extra_arg) {
                printf("error: too many arguments.\n");
                continue;
            }

            Record *record_to_update = db_lookup(db, handle_to_update);
            if (record_to_update == NULL) {
                printf("error: no entry with handle %s\n", handle_to_update);
                continue;
            }

            char *endptr;
            updated_followers = strtol(updated_followers_str, &endptr, 10);
            if (*endptr != '\0') {
                printf("error: follower count must be an integer\n");
                continue;
            }

            printf("comment > ");
            size_t comment_length = 0;
            char *updated_comment = NULL;
            getline(&updated_comment, &comment_length, stdin);

            size_t updated_comment_size = strlen(updated_comment);
            if (updated_comment_size > 0 && updated_comment[updated_comment_size - 1] == '\n') {
                updated_comment[updated_comment_size - 1] = '\0';
            }

            record_to_update->followers = updated_followers;
            strncpy(record_to_update->comment, updated_comment, sizeof(record_to_update->comment) - 1);
            record_to_update->timestamp = time(NULL);

            printf("record updated!\n");
            free(updated_comment);
            changes_made = 1;

        } else if (strcmp(command_token, "save") == 0) {
            // save the database to a csv file
            char *extra_arg = strtok(NULL, " ");
            if (extra_arg) {
                printf("error: too many arguments.\n");
                continue;
            }

            db_write_csv(db, "database.csv");
            printf("database saved.\n");
            changes_made = 0;

        } else if (strcmp(command_token, "exit") == 0) {
            // exit the program
            command_token = strtok(NULL, " ");
            if (command_token && strcmp(command_token, "fr") == 0) {
                db_free(db);
                exit(0);
            } else {
                if (changes_made > 0) {
                    printf("did not save your changes. use 'exit fr' to force exiting anyway.\n");
                } else {
                    db_free(db);
                    exit(0);
                }
            }

        } else {
            printf("invalid command\n");
        }
    }

    free(input_line);
    return 0;
}

// main function
int main() {
    Database db = db_create();
    db_load_csv(&db, "database.csv");
    return main_loop(&db);
}

~                                                                                                                                              
~                                                                                                                                              
~     
