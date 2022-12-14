#include "input.h"

Result input(int type, wchar_t *buffer, int buffer_size){
    Result result = noEcho();
    {// init and error handlingq
        //disable echo
        if(result.Error_state != OK){
            return result;
        }

        //enter raw mode
        result = rawMode();
        if(result.Error_state != OK){
            return result;
        }

        if(buffer_size < 1){
            result.Error_state = INPUT_BUFFER_SIZE_TOO_SMALL;
            return result;
        }

        if(buffer == NULL){
            result.Error_state = INPUT_BUFFER_NULL;
            return result;
        }

        if(buffer_size < 1){
            result.Error_state = INPUT_BUFFER_SIZE_TOO_SMALL;
            return result;
        }


        // make buffer
        buffer = calloc(buffer_size, sizeof(wchar_t));
        if(buffer == NULL){
            result.Error_state = MALLOC_FAULT;
            return result;
        }
    }

    // clean any input errors
    clearerr(stdin);

    // get input
    switch (type)
    {
    case INTEGER:
        wscanf(L"%d", buffer);
        break;
    case FLOAT:
        wscanf(L"%f", buffer);
        break;
    case NUMERIC:
        wscanf(L"%d", buffer);
        break;
    case ALPHA:
        wscanf(L"%s", buffer);
        break;
    case ALPHANUMERIC:
        wscanf(L"%s", buffer);
        break;
    case ANY:
        wscanf(L"%s", buffer);
        break;
    default:
        result.Error_state = INPUT_INVALID_TYPE;
        free(buffer);
        return result;
        break;
    }

    return result;
}