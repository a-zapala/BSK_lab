#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <dirent.h>
#include <security/pam_appl.h>
#include <security/pam_misc.h>
#include <libgen.h>


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
    
    return 0;
}
