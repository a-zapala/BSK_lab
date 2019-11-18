#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <dirent.h>
#include <security/pam_appl.h>
#include <security/pam_misc.h>
#include <libgen.h>

char *buffer = NULL;
const char *SUBJECT_DIR = "./katalog_przedmiotowy";
const char *ALPHABETICAL_DIR = "./katalog_alfabetyczny";
const int BUFFER_SIZE = 256;
int is_manager;

void exit_fail(const char *msg) {
    fputs(msg, stderr);
    if (buffer != NULL) free(buffer);
    exit(1);
}

void print_heading() {
    printf("###########################################\n");
}

int read_number(int min, int max) {
    long a;
    char buf[1024]; // use 1KiB just to be sure
    int success; // flag for successful conversion
    
    do {
        printf("Wybierz liczbę:\n");
        if (!fgets(buf, 1024, stdin)) {
            // reading input failed:
            return 1;
        }
        
        // have some input, convert it to integer:
        char *endptr;
        
        errno = 0; // reset error number
        a = strtol(buf, &endptr, 10);
        if (errno == ERANGE || a > (long int) max || a < (long int) min) {
            printf("Ta liczba jest za duża, bądź za mała.\n");
            success = 0;
        } else if (endptr == buf) {
            // no character was read
            success = 0;
        } else if (*endptr && *endptr != '\n') {
            // *endptr is neither end of string nor newline,
            // so we didn't convert the *whole* input
            success = 0;
        } else {
            success = 1;
        }
    } while (!success); // repeat until we got a valid number
    
    return (int) a;
}

char *create_path_to_subject_dir(int number) {
    char main_directory = number / 100 + '0';
    char sub_directory = (number % 100 / 10) + '0';
    unsigned long size = strlen(SUBJECT_DIR) + 5;
    char *result = malloc(size);
    snprintf(result, size, "%s/%c/%c", SUBJECT_DIR, main_directory, sub_directory);
    return result;
}

void print_directory(const char *path) {
    DIR *fd;
    struct dirent *dir;
    fd = opendir(path);
    
    if (fd == NULL) {
        exit_fail("Can't open subject directory\n");
    }
    print_heading();
    while ((dir = readdir(fd)) != NULL) {
        if (dir->d_type == DT_LNK) {
            printf("%s\n", dir->d_name);
        }
    }
}

char *get_path_to_classification_directory() {
    printf("Wybierz numer klasyfikacji:\n");
    int number = read_number(0, 999);
    return create_path_to_subject_dir(number);
}

void print_classification_directory() {
    char *path = get_path_to_classification_directory();
    print_directory(path);
    free(path);
}

char *find_file_in_dir(const char *dir_path, const char *name_of_file) {
    DIR *dir;
    struct dirent *entry;
    char *result = NULL;
    
    if (!(dir = opendir(dir_path)))
        return result;
    
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            char path[1024];
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            snprintf(path, sizeof(path), "%s/%s", dir_path, entry->d_name);
            char *tmp = find_file_in_dir(path, name_of_file);
            if (tmp != NULL) {
                result = tmp;
            }
        } else {
            if (strcmp(entry->d_name, name_of_file) == 0) {
                result = malloc(strlen(dir_path) + strlen(entry->d_name) + 2);
                snprintf(result, strlen(dir_path) + strlen(entry->d_name) + 2, "%s/%s", dir_path,
                         entry->d_name);
            }
        }
    }
    closedir(dir);
    return result;
}

int number_retrieve_classification_number(const char *path_subject_dir) {
    char *dirc = strdup(path_subject_dir);
    char *dname = dirname(dirc);
    char c = dname[strlen(dname) - 1];
    char d = dname[strlen(dname) - 3];
    free(dirc);
    return (d - '0') * 100 + (c - '0') * 10;
}

void delete_book_info(const char *path_alph_dir) {
    remove(path_alph_dir);
}

char *create_path(const char *dir, const char *name) {
    unsigned long size = strlen(dir) + strlen(name) + 2;
    char *result = malloc(size);
    snprintf(result, size, "%s/%s", dir, name);
    return result;
}

void create_classification(const char *path_alph_dir, const char *path_subject_dir) {
    char *dirc = strdup(path_alph_dir);
    char *bname = basename(dirc);
    char resolved_path[PATH_MAX];
    realpath(path_alph_dir, resolved_path);
    char *new_link = create_path(path_subject_dir, bname);
    symlink(resolved_path, new_link);
    free(new_link);
    free(dirc);
}

void add_classification(const char *path_alph_dir) {
    char *new_subject_dir = get_path_to_classification_directory();
    create_classification(path_alph_dir, new_subject_dir);
    free(new_subject_dir);
}

void change_classification(const char *path_alph_dir, const char *path_subject_dir) {
    remove(path_subject_dir);
    add_classification(path_alph_dir);
}

void book_info_without_classification(const char *path_alph_dir) {
    printf("1. Usuń opis\n");
    printf("2. Dodaj klasyfikację\n");
    printf("3. Powrót\n");
    int option = read_number(1, 3);
    if (option == 1) {
        delete_book_info(path_alph_dir);
    } else if (option == 2) {
        add_classification(path_alph_dir);
    }
}

void book_info_with_classification(const char *path_alph_dir, const char *path_subject_dir) {
    int classification = number_retrieve_classification_number(path_subject_dir);
    printf("Klasyfikacja: %03d\n", classification);
    printf("1. Usuń opis\n");
    printf("2. Zmień klasyfikację\n");
    printf("3. Powrót\n");
    int option = read_number(1, 3);
    if (option == 1) {
        delete_book_info(path_alph_dir);
    } else if (option == 2) {
        change_classification(path_alph_dir, path_subject_dir);
    }
}

void create_info() {
    char *path = create_path(ALPHABETICAL_DIR, buffer);
    printf("Wpisz autora książki\n");
    if (fgets(buffer, BUFFER_SIZE, stdin)) {
        FILE *f;
        f = fopen(path, "w");
        fprintf(f, "%s", buffer);
        fclose(f);
    }
    free(path);
}

void not_existing_book_info() {
    printf("Nie istnieje\n");
    printf("1. Utwórz opis\n");
    printf("2. Powrót\n");
    int option = read_number(1, 2);
    if (option == 1) {
        create_info();
    }
}

void print_book_info(const char *path_alph_dir, const char *path_subject_dir, char *name) {
    print_heading();
    printf("Książka: %s\n", buffer);
    if (path_alph_dir) {
        if (path_subject_dir) {
            book_info_with_classification(path_alph_dir, path_subject_dir);
        } else {
            book_info_without_classification(path_alph_dir);
        }
    } else {
        not_existing_book_info();
    }
}

void find_book() {
    printf("Wpisz nazwę książki\n");
    if (fgets(buffer, BUFFER_SIZE, stdin)) {
        buffer[strcspn(buffer, "\n")] = 0;
        char *path_subject_dir = find_file_in_dir(SUBJECT_DIR, buffer);
        char *path_alph_dir = find_file_in_dir(ALPHABETICAL_DIR, buffer);
        print_book_info(path_alph_dir, path_subject_dir, buffer);
        if (path_alph_dir) free(path_alph_dir);
        if (path_subject_dir) free(path_subject_dir);
    }
}

void manager_main_menu() {
    while (1) {
        print_heading();
        printf("1. Przegladaj katalog\n");
        printf("2. Znajdź książkę po nazwie\n");
        printf("3. Wyjscie\n");
        int option = read_number(1, 3);
        
        if (option == 1) {
            print_classification_directory();
        } else if (option == 3) {
            break;
        } else if (option == 2) {
            find_book();
        }
    }
}

void sellers_main_menu() {
    while (1) {
        print_heading();
        printf("1. Przegladaj katalog\n");
        printf("2. Wyjscie\n");
        int option = read_number(1, 2);
        
        if (option == 1) {
            print_classification_directory();
        } else if (option == 2) {
            break;
        }
    }
}

void main_menu() {
    if (is_manager) {
        manager_main_menu();
    } else {
        sellers_main_menu();
    }
}

int check_manager_login(const char *user) {
    pam_handle_t *pamh = NULL;
    struct pam_conv pam_conversation = {misc_conv, NULL};
    int retval;
    
    retval = pam_start("manager_radegast", user, &pam_conversation, &pamh);
    if (pamh == NULL || retval != PAM_SUCCESS) exit_fail("Failed to perform pam_start\n");
    
    retval = pam_acct_mgmt(pamh, 0);
    pam_end(pamh, PAM_SUCCESS);
    
    if (retval != PAM_SUCCESS) {
        return 0;
    } else {
        return 1;
    }
}

int main() {
//     handle user login
    int retval;
    pam_handle_t *pamh = NULL;
    static struct pam_conv login_conv = {misc_conv, NULL};
    
    retval = pam_start("user_radegast", NULL, &login_conv, &pamh);
    if (pamh == NULL || retval != PAM_SUCCESS) exit_fail("Failed to perform pam_start\n");
    
    
    retval = pam_authenticate(pamh, 0);
    if (retval != PAM_SUCCESS) {
        exit(2);
    }
    
    retval = pam_acct_mgmt(pamh, 0);
    if (retval != PAM_SUCCESS) {
        exit(2);
    }
    
    const char *user;
    
    retval = pam_get_item(pamh, PAM_USER, (const void **) &user);
    if (retval != PAM_SUCCESS) {
        exit_fail("Failed pam_get_item");
    }
    
    is_manager = check_manager_login(user);
    if (is_manager == 1) {
        printf("manager has logged\n");
    } else {
        printf("sprzedawca has logged\n");
    }
    pam_end(pamh, PAM_SUCCESS);
    
    buffer = malloc(BUFFER_SIZE * sizeof(char));
    main_menu();
    free(buffer);
    return 0;
}
