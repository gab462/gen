typedef struct {
    T *items;
    size_t len, cap;
} Array(T);

void
array_push(Array(T) *self, T item)
{
    if (self->len + 1 > self->cap) {
        self->cap = (self->cap + 1) * 2;
        self->items = realloc(self->items, sizeof(T) * self->cap);
    }

    self->items[self->len++] = item;
}

void
array_swapdel(Array(T) *self, size_t idx)
{
    self->items[idx] = self->items[--self->len];
}

T
array_pop(Array(T) *self)
{
    return self->items[--self->len];
}

void
array_resize(Array(T) *self, size_t new_cap)
{
    self->cap = new_cap;
    self->items = realloc(self->items, sizeof(T) * self->cap);

    if (self->cap < self->len)
        self->len = self->cap;
}

void
array_free(Array(T) *self)
{
    free(self->items);
}
