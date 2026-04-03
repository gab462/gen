typedef struct {
    T *items;
    int len, cap;
} Array(T);

void
array_push(T)(Array(T) *self, T item)
{
    if (self->len + 1 > self->cap) {
        self->cap = (self->cap + 1) * 2;
        self->items = realloc(self->items, sizeof(T) * self->cap);
    }
    self->items[self->len++] = item;
}

void
array_delete(T)(Array(T) *self)
{
    free(self->items);
}
