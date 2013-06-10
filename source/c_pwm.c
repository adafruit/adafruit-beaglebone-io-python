#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include "c_pwm.h"

#define KEYLEN 7

char pwm_ctrl_dir[30];
char ocp_dir[22];
int pwm_initialized = 0;

// pwm exports
struct pwm_exp
{
    char key[KEYLEN];
    int fd;
    struct pwm_exp *next;
};
struct pwm_exp *exported_pwms = NULL;

int load_device_tree(const char *name);

int build_path(const char *partial_path, const char *prefix, char *full_path, size_t full_path_len)
{
    DIR *dp;
    struct dirent *ep;

    dp = opendir (partial_path);
    if (dp != NULL) {
        while ((ep = readdir (dp))) {
            if (strstr(ep->d_name, prefix)) {
                snprintf(full_path, full_path_len, "%s%s", partial_path, ep->d_name);
                (void) closedir (dp);
                return 1;
            }
        }
        (void) closedir (dp);
    } else {
        return 0;
    }

    return 0;
}

int initialize_pwm(void)
{
    if  (build_path("/sys/devices/", "bone_capemgr", pwm_ctrl_dir, sizeof(pwm_ctrl_dir)) && 
         build_path("/sys/devices/", "ocp", ocp_dir, sizeof(ocp_dir))) {
        pwm_initialized = 1;

        load_device_tree("am33xx_pwm");
        return 1;
    }
    return 0;   
}

int load_device_tree(const char *name)
{
    FILE *file = NULL;
    char slots[40];
    char line[256];

    if(!pwm_initialized) {
        initialize_pwm();
    }

    snprintf(slots, sizeof(slots), "%s/slots", pwm_ctrl_dir);

    file = fopen(slots, "r+");
    if (!file) {
        return -1;
    }

    while (fgets(line, sizeof(line), file)) {
        //the device is already loaded, return 1
        if (strstr(line, name)) {
            fclose(file);
            return 1;
        }
    }

    //if the device isn't already loaded, load it, and return
    fprintf(file, name);
    fclose(file);

    return 1;
}

int unload_device_tree(const char *name)
{
    FILE *file = NULL;
    char slots[40];
    char line[256];
    char *slot_line;

    snprintf(slots, sizeof(slots), "%s/slots", pwm_ctrl_dir);

    file = fopen(slots, "r+");
    if (!file) {
        return -1;
    }

    while (fgets(line, sizeof(line), file)) {
        //the device is loaded, let's unload it
        if (strstr(line, name)) {
            slot_line = strtok(line, ":");
            //remove leading spaces
            while(*slot_line == ' ')
                slot_line++;

            fprintf(file, "-%s", slot_line);
            fclose(file);
            return 1;
        }
    }

    //not loaded, close file
    fclose(file);

    return 1;
}



int pwm_enable(const char *key)
{
    char fragment[18];
    int fd;
    struct pwm_exp *new_pwm, *pwm;

    snprintf(fragment, sizeof(fragment), "bone_pwm_%s", key);
    

    if (!load_device_tree(fragment)) {
        //error enabling pin for pwm
        return -1;
    }

    //TODO add fd to list
    //fd = 

    // add to list
    new_pwm = malloc(sizeof(struct pwm_exp));
    if (new_pwm == 0) {
        return -1; // out of memory
    }

    strncpy(new_pwm->key, key, KEYLEN);
    //new_pwm->fd = fd;
    new_pwm->next = NULL;

    if (exported_pwms == NULL)
    {
        // create new list
        exported_pwms = new_pwm;
    } else {
        // add to end of existing list
        pwm = exported_pwms;
        while (pwm->next != NULL)
            pwm = pwm->next;
        pwm->next = new_pwm;
    }

    return 0;
}

int pwm_disable(const char *key)
{
    struct pwm_exp *pwm, *temp, *prev_pwm = NULL;
    char fragment[18];

    snprintf(fragment, sizeof(fragment), "bone_pwm_%s", key);
    fprintf(stderr, "pwm_disable fragment: %s\n", fragment);
    unload_device_tree(fragment);

    // remove from list
    pwm = exported_pwms;
    while (pwm != NULL)
    {
        if (strcmp(pwm->key, key) == 0)
        {
            //close the fd
            close(pwm->fd);
            if (prev_pwm == NULL)
                exported_pwms = pwm->next;
            else
                prev_pwm->next = pwm->next;
            temp = pwm;
            pwm = pwm->next;
            free(temp);
        } else {
            prev_pwm = pwm;
            pwm = pwm->next;
        }
    }
    return 0;    
}

void pwm_cleanup(void)
{
    while (exported_pwms != NULL) {
        fprintf(stderr, "pwm_clenaup key: %s\n", exported_pwms->key);
        pwm_disable(exported_pwms->key);
    }
}