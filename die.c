#include "die.h"

void DieWithError(char *errorMessage)
{
    perror(errorMessage);
    exit(1);
}
