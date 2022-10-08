#include "auth.h"
#include "routes.h"
#include "proyecto.h"

#define USERS_FILE "users.db"

#include <stdio.h>
#include <errno.h>

static LList* users;

void freeUsers(){
    while(LList_size(users) > 0){
        User* user = LList_remove_at(users, 0);
        // free(user->name);
        // free(user->pass);

        if(user->type == PASSANGER){
            while(LList_size(user->queued_routes) > 0){
                Route* route = LList_remove_at(user->queued_routes, 0);
                free(route->name);
                free(route->destination);
                free(route->scheduled_times);
                free(route);
            }
        }

        free(user);
    }
    free(users);
}

// TODO: #1 Save users to file
Result loadAllUsers() {
    Result result;
    result.Error_state = OK;

    users = LList_new();
    if (users == 0) {
        result.Error_state = MALLOC_FAULT;
        return result;
    }

    FILE* file = fopen(USERS_FILE, "r, ccs=UTF-8");
    if (file == 0) {
        // check for errors produced by fopen
        if (errno == ENOENT) {
            // if the file was not found, create it
            file = fopen(USERS_FILE, "a+, ccs=UTF-8");
            if (file == 0) {
                if(errno == EACCES) {
                    result.Error_state = FILE_PERMISSION_DENIED;
                } else {
                    result.Error_state = FOPEN_FAULT;
                }
            } else {
                result.Error_state = OK;
                // Because the file was created, we need to set the number of users to 0
                fwprintf(file, L"%d\n", 0);
            }
        } else if(errno == EACCES){
            result.Error_state = FILE_PERMISSION_DENIED;
        } else {
            result.Error_state = FILE_OPEN_ERROR;
        }
        
        // We were able to open the file, but there was an error.
        if(result.Error_state != OK){
            freeUsers();
            fclose(file);
            return result;
        }
    }

    { //Define the awlays avaiable admin account
        User* Admin = malloc(sizeof(User));
        Admin->name = L"admin";
        Admin->pass = L"admin";
        Admin->state = ENABLED;
        Admin->type = ADMIN;
        LList_add(users, Admin);
    }

    int number_of_users = 0;
    if( fwscanf(file, L" %d ", &number_of_users) != 1){
        result.Error_state = FILE_READ_ERROR;
        freeUsers();
        fclose(file);
        return result;
    }
    if( fwscanf(file, L"\n") != 0){
        result.Error_state = FILE_READ_ERROR;
        freeUsers();
        return result;
    }
    for (int i = 0; i < number_of_users; i++) {
        User* user = malloc(sizeof(User));
        if (user == 0) {
            result.Error_state = MALLOC_FAULT;
            freeUsers();
            fclose(file);
            return result;
        }

        if(fwscanf(file, L"%ls %ls %d %d", user->name, user->pass, &user->state, &user->type) != 4){
            result.Error_state = FILE_READ_ERROR;
            freeUsers();
            fclose(file);
            return result;
        }
        
        // if the user isn't an admin, we read all the queued rouutes
        if(user->type == PASSANGER){
            int number_of_routes = 0;
            if(fwscanf(file, L" %d", &number_of_routes) != 1){
                result.Error_state = FILE_READ_ERROR;
                freeUsers();
                fclose(file);
                return result;
            }
            
            user->queued_routes = LList_new();
            if(user->queued_routes == 0){
                result.Error_state = MALLOC_FAULT;
                freeUsers();
                fclose(file);
                return result;
            }

            for(int j = 0; j < number_of_routes; j++){
                // Read route id
                wchar_t* route_id = malloc(sizeof(wchar_t) * 10);
                if(route_id == 0){
                    result.Error_state = MALLOC_FAULT;
                    freeUsers();
                    fclose(file);
                    return result;
                }

                if(fwscanf(file, L" %ls", route_id) != 1){
                    result.Error_state = FILE_READ_ERROR;
                    freeUsers();
                    fclose(file);
                    return result;
                }

                LList_add(user->queued_routes, route_id);
            }
        }
        
        LList_add(users, user);

        if(fwscanf(file, L"\n") != 0){
            result.Error_state = FILE_READ_ERROR;
            freeUsers();
            fclose(file);
            return result;
        }
    }

    fclose(file);
    return result;
}

Result login(const wchar_t* name, const wchar_t* pass){
    Result result;

    for(int i = 0; i < LList_size(users); i++){
        User* user = LList_get(users, i);
        if(wcscmp(user->name, name) == 0){
            if(wcscmp(user->pass, pass) == 0){
                if(user->state == ENABLED){
                    result.Error_state = OK;
                    result.result = user;
                    return result;
                } else {
                    result.Error_state = USER_DISABLED;
                    return result;
                }
            } else {
                result.Error_state = INCORRECT_PASSWORD;
                return result;
            }
        }
    }

    result.Error_state = USER_NOT_FOUND;
    return result;
}

Result add_user(const User Requester,const wchar_t* NewUserName, const wchar_t* NewUserPass, const Type NewUserType){
    Result result;

    if(Requester.type != ADMIN){
        result.Error_state = USER_NOT_ALLOWED;
        return result;
    }

    for(int i = 0; i < LList_size(users); i++){
        User* user = LList_get(users, i);
        if(wcscmp(user->name, NewUserName) == 0){
            result.Error_state = USER_ALREADY_EXISTS;
            return result;
        }
    }

    User* user = malloc(sizeof(User));
    if(user == 0){
        result.Error_state = MALLOC_FAULT;
        return result;
    }

    user->name = malloc(sizeof(wchar_t) * (wcslen(NewUserName) + 1));
    if(user->name == 0){
        result.Error_state = MALLOC_FAULT;
        return result;
    }
    wcscpy(user->name, NewUserName);

    user->pass = malloc(sizeof(wchar_t) * (wcslen(NewUserPass) + 1));
    if(user->pass == 0){
        result.Error_state = MALLOC_FAULT;
        return result;
    }
    wcscpy(user->pass, NewUserPass);

    user->state = ENABLED;
    user->type = NewUserType;

    if(user->type == PASSANGER){
        user->queued_routes = LList_new();
        if(user->queued_routes == 0){
            result.Error_state = MALLOC_FAULT;
            return result;
        }
    }

    LList_add(users, user);

    result.Error_state = OK;
    return result;
}

Result modify_user(const User Requester, const wchar_t* UserName, const wchar_t* NewUserPass, Type NewUserType){
    Result result;

    if(Requester.type != ADMIN){
        result.Error_state = USER_NOT_ALLOWED;
        return result;
    }

    for(int i = 0; i < LList_size(users); i++){
        User* user = LList_get(users, i);
        if(wcscmp(user->name, UserName) == 0){

            free(user->pass);
            user->pass = malloc(sizeof(wchar_t) * (wcslen(NewUserPass) + 1));
            if(user->pass == 0){
                result.Error_state = MALLOC_FAULT;
                return result;
            }
            wcscpy(user->pass, NewUserPass);

            user->type = NewUserType;

            if(user->type == ADMIN){
                freeUserRoutes(query_user(Requester, UserName).result);
            }

            result.Error_state = OK;
            return result;
        }
    }

    result.Error_state = USER_NOT_FOUND;
    return result;
}

Result remove_user(const User Requester, const wchar_t* UserName){
    Result result;

    if(Requester.type != ADMIN){
        result.Error_state = USER_NOT_ALLOWED;
        return result;
    }

    for(int i = 0; i < LList_size(users); i++){
        User* user = LList_get(users, i);
        if(wcscmp(user->name, UserName) == 0){

            user->state = DISABLED;

            result.Error_state = OK;
            return result;
        }
    }

    result.Error_state = USER_NOT_FOUND;
    return result;
}

Result query_user(const User Requester, const wchar_t* UserName){
    Result result;

    if(Requester.type != ADMIN){
        result.Error_state = USER_NOT_ALLOWED;
        return result;
    }

    for(int i = 0; i < LList_size(users); i++){
        User* user = LList_get(users, i);
        if(wcscmp(user->name, UserName) == 0){
            result.Error_state = OK;
            result.result = user;
            return result;
        }
    }

    result.Error_state = USER_NOT_FOUND;
    return result;
}