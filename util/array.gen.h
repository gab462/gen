#ifdef GEN_INSTANTIATION

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
array_free(Array(T) *self)
{
    free(self->items);
}

#elifdef GEN_DECLARATION

GenModule
gen_array(void)
{
    static char *member_functions[] = {
        "array_push",
        "array_free",
        NULL,
    };

    return (GenModule){
        .source = __FILE__,
        .name = "Array",
        .typename = "T",
        .member_functions = member_functions,
    };
}

#else

#error "Cannot include generic header"

#endif
