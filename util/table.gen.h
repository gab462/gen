#ifdef GEN_INSTANTIATION

#ifndef TABLE_ONCE
#define TABLE_ONCE

#define table_make_type(K, V) struct Table_Type_##K##_##V { K key; V value; }
#define Table(K, V) Table_Impl(table_make_type(K, V))

typedef size_t (*Table_Hasheq)(void *a, void *b, size_t len);

#endif

#define K typeof((KV){0}.key)
#define V typeof((KV){0}.value)

typedef struct {
    K *keys;
    V *values;
    bool *active;
    size_t count, cap;
    Table_Hasheq hasheq;
} Table_Impl(KV);

size_t
table_hasheq(Table_Impl(KV) *self, K *k1, K *k2)
{
    if (self->hasheq) {
        return self->hasheq(k1, k2, sizeof(K));
    } else {
        if (k2 == NULL) {
            // hash - fnv-1a
            uint8_t *bytes = (void *) k1;
            uint64_t hash = 0xcbf29ce484222325;

            for (size_t i = 0; i < sizeof(K); ++i) {
                hash ^= (uint64_t) bytes[i];
                hash *= 0x100000001b3;
            }

            return hash;
        } else {
            // equality - compare bytes
            return memcmp(k1, k2, sizeof(K)) == 0;
        }
    }
}

void table_set(Table_Impl(KV) *self, K key, V value);

void
table_resize(Table_Impl(KV) *self, size_t new_cap)
{
    Table_Impl(KV) old = *self;

    self->keys = malloc(new_cap * sizeof(K));
    self->values = malloc(new_cap * sizeof(V));
    self->active = calloc(new_cap, sizeof(bool));
    self->cap = new_cap;
    self->count = 0;

    for (size_t i = 0; i < old.cap; ++i) {
        if (old.active[i])
            table_set(self, old.keys[i], old.values[i]);
    }

    free(old.keys);
    free(old.values);
    free(old.active);
}

size_t
table_index(Table_Impl(KV) *self, K key)
{
    if (self->cap == 0 || (float) self->count / self->cap > 0.75f)
        table_resize(self, self->cap > 0 ? self->cap * 2 : 16);

    size_t idx = table_hasheq(self, &key, NULL) % self->cap;

    while (self->active[idx]) {
        if (table_hasheq(self, &key, &self->keys[idx]))
            return idx;
        else
            idx = (idx + 1) % self->cap;
    }

    // Key not found, create
    self->keys[idx] = key;
    self->values[idx] = (V){0};
    self->active[idx] = true;
    ++self->count;

    return idx;
}

V *
table_get(Table_Impl(KV) *self, K key)
{
    size_t idx = table_index(self, key);

    return &self->values[idx];
}

void
table_set(Table_Impl(KV) *self, K key, V value)
{
    size_t idx = table_index(self, key);

    self->values[idx] = value;
}

void
table_del(Table_Impl(KV) *self, K key)
{
    size_t idx = table_index(self, key);

    self->active[idx] = false;
    --self->count;

    // Backshift cluster

    size_t prev = idx;
    size_t curr = (idx + 1) % self->cap;

    while (self->active[curr]) {
        // Find preferred place for curr key
        size_t home = table_hasheq(self, &self->keys[curr], NULL) % self->cap;

        // Calculate forward distance from home (currently and if shifted)
        size_t curr_distance = (curr + self->cap - home) % self->cap;
        size_t prev_distance = (prev + self->cap - home) % self->cap;

        // Shift if distance would decrease
        if (prev_distance < curr_distance) {
            self->keys[prev] = self->keys[curr];
            self->values[prev] = self->values[curr];

            self->active[prev] = true;
            self->active[curr] = false;

            prev = curr;
        }

        curr = (curr + 1) % self->cap;
    }
}

void
table_free(Table_Impl(KV) *self)
{
    free(self->keys);
    free(self->values);
    free(self->active);
}

#undef K
#undef V

#elifdef GEN_DECLARATION

GenModule
gen_table(void)
{
    static char *member_functions[] = {
        "table_hasheq",
        "table_resize",
        "table_index",
        "table_get",
        "table_set",
        "table_del",
        "table_free",
        NULL,
    };

    return (GenModule){
        .source = __FILE__,
        .name = "Table_Impl",
        .typename = "KV",
        .member_functions = member_functions,
    };
}

#else

#error "Cannot include generic header"

#endif
