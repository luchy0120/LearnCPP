##Pointers

The fundamental operation on a poiner is `dereferencing`, that is, referring to the object pointed to by the pointer. This operation is also called `indirection`. The dereferencing operator is (prefix) unary `*`.

The smallest object that can be independently allocated and pointed to using a built-in pointer type is a `char`, Note that a `bool` occupies at least as musch space as a `char`. To store smaller values more compactly, you can use the bit-wise logical operations, bit-fields in structures or a `bitset`

	int *pi;			// pointer to int
	char **ppc;			// pointer to pointer to char
	int *ap[15];		// array of 15 pointers to ints
	int (*fp)(char*);	// pointer to function taking a char* argument; returns an int
	int *f(char*);		// function taking a char* argument; returns a pointer to int

`void*` read as "pointer to an object of unknown type"

	void f(int *pi) {
		void *pv = pi;		// OK
		*pv;				// error, can not dereference void*
		++pv;				// error, can not increment void*

		// explicit conversion back to int *
		int *pi2 = static_cast<int*>(pv);

		double *pd1 = pv;	// error
		double *pd2 = pi;	// error
		double *pd3 = static_cast<double*>(pv);	// unsafe
	}

The notation used, `static_cast`, was designed to be ugly and easy to find in code.

The primary use for `void*` is for passing pointers to functions that are not allowed to make assumptions about the type of the object and for returnning untyped objects from functions. To use such an object, we must use explicit type conversion.

Functions using `void*` pointers typically exist at the very low level of the system, where real hardware resources are manipulated. For example.

	void* my_alloc(size_t n);

Pointers to functions and pointers to members can not be assigned to `void*`s.

