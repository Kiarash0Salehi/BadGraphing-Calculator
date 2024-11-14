#include "header/global.h"

#define VECTOR_INIT_CAPACITY 4

void vector_init(vector *v) {
    v->capacity = VECTOR_INIT_CAPACITY;
    v->total = 0;
    v->data = malloc(sizeof(void *) * v->capacity);
}

int vector_total(vector *v)
{
    return v->total;
}

static void vector_resize(vector *v, int capacity) 
{
    void **data = realloc(v->data, sizeof(void *) * capacity);
    if (data) {
        v->data = data;
        v->capacity = capacity;
    }
}

void vector_add(vector *v, void *item) 
{
    if (v->capacity == v->total)
        vector_resize(v, v->capacity * 2);
    v->data[v->total++] = item;
}

void vector_set(vector *v, int index, void *item) 
{
    if (index >= 0 && index < v->total)
        v->data[index] = item;
}

void *vector_get(vector *v, int index) 
{

    if (index >= 0 && index < v->total)
        return v->data[index];
    return NULL;
}

void vector_delete(vector *v, int index) 
{
    if (index < 0 || index >= v->total)
        return;
    v->data[index] = NULL;
    int i;
    for (i = 0; i < v->total - 1; i++) 
	{
        v->data[i] = v->data[i + 1];
        v->data[i + 1] = NULL;
    }
    v->total--;
    if (v->total > 0 && v->total == v->capacity / 4)

        vector_resize(v, v->capacity / 2);
}

void vector_free(vector *v) 
{
    free(v->data);
}