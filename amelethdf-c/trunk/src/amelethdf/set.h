#ifndef SET_H
#define SET_H

typedef struct
{
        char** array;
        int number_of_element;
} set_t;

int in(set_t aset, char* aelement);
set_t add(set_t aset, char* aelement);
char* get(set_t aset, int id);
int get_index(set_t aset, char* aelement);
set_t clean(set_t aset);

#endif // SET_H
