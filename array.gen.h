typedef struct {
	T *items;
	int len, cap;
} Array(T);

extern struct {
	void (*push)(Array(T) *arr, T item);
	void (*delete)(Array(T) *arr);
} array(T);
