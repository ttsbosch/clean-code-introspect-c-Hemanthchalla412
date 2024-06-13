char** SplitString(const char* InputString, char delimiter) {
    int numofdelimiter = 0;
    const char* ptr = InputString;
    while (*ptr != '\0') {
        if (*ptr++ == delimiter) {
            numofdelimiter++;
        }
    }

    char** tokens = (char**)malloc(sizeof(char*) * (numofdelimiter + 2));
    int Index = 0;
    ptr = InputString;
    char* token = (char*)malloc(strlen(InputString) + 1);
    int TokenIndex = 0;
    while (*ptr != '\0') {
        if (*ptr == delimiter) {
            token[TokenIndex] = '\0';
            tokens[Index++] = strdup(token);
            TokenIndex = 0;
        } else {
            token[TokenIndex++] = *ptr;
        }
        ptr++;
    }
    token[TokenIndex] = '\0';
    tokens[Index++] = strdup(token);
    tokens[Index] = NULL;
    free(token);
    return tokens;
}
