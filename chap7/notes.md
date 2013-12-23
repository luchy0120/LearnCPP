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

###void*

The primary use for `void*` is for passing pointers to functions that are not allowed to make assumptions about the type of the object and for returnning untyped objects from functions. To use such an object, we must use explicit type conversion.

Functions using `void*` pointers typically exist at the very low level of the system, where real hardware resources are manipulated. For example.

	void* my_alloc(size_t n);

Pointers to functions and pointers to members can not be assigned to `void*`s.

###nullptr

The literal `nullptr` represents the null pointer, that is, a pointer that does not point to an object. It can be assigned to any pointer type, but not to other built-in types.

	int* pi = nullptr;
	double* pd = nullptr;
	int i = nullptr;		// error: i is not a pointer

There are differences in the definition of `NULL` in different implementations; for example, NULL might be 0 or 0L, In C, NULL is typically `(void*)0`, which makes it illegal in C++.

	// I don't know how to raise this error
	int *p = NULL;			// error: can not assign a void* to an int*

Using `nullptr` makes code more readable than alternatives and avoids potential confusion when a function is overloaded to accpet either a pointer or an integer.

##Arrays

An array can be allocated statically, on the stack, and on the free store.

	int a1[10];			// 10 ints in static storage

	void f() {
		int a2[20];		// 20 ints on the stack
		int *p = new int[40];	// 40 ints on the free store
	}

###Array Initializers

An array can be initialized by a list of values.

	int v1[] = {1, 2, 3, 4};
	char v2[] = {'a', 'b', 'c', 0};

There is no built-in copy operation for arrays. You can not initialize one array with another (not even of exactly the same type), and there is no array assignment:

	int v6[8] = v5;		// error, can not copy an array
	v6 = v5;			// error, no array assignment

You can not pass arrays by value

The type of a string literal is "array of the appropriate number of const characters", so "abcd" is of type `const char[5]`

In C and in older C++ code, you could assign a string literal to a non-const char*

	void f() {
		char *p = "Plato";		// error, but accepted in pre-c++11 standard code
		p[4] = 'e';				// error, assignment to const
	}

If we want a string that we are guaranteed to be able to modify, we must place the characters in a non-const array:

	void f() {
		char p[] = "Zeno";		// p is an array of 5 char
		p[0] = 'R';				// OK
	}

A string literal is statically allocated so that it is safe to return one from a function. For example:

	const char* error_message(int i) {
		// ...
		return "range error";
	}

The memory holding "range error" will not go away after a call of error_message()

Whether two identical string literals are allocated as one array or as two is implementaton defined:

	const char* p = "Heraclitus";
	const char* q = "Heraclitus";

	void g() {
		if (p == q) cout << "one" << endl;
		// ...
	}

test in gcc 4.8.2 it is true;

The empty string is written as a pair of adjacent double quotes, `""`, and has the type `const char[1]`. The one character of the empty string is the terminating `\0`

Long strings can be broken by whitespace to make the program text neater. For example:

	char alpha[] = "abcdefghijklmnopqrstuvwxyz"
					"ABCDEFGHIJKLMNOPQRSTUVWXYZ";

The compiler will concatenate adjacent strings.

	string s = "\\w\\\\w";

A raw string literal is a string literal where a backslash is just a backslash(and a double quote is just a double quote) so that our example becomes:

	string s = R"(\w\\w)";

Raw string literals use the `R("ccc")` notation for a sequence of characters `ccc`. The initial R is there to distinguish raw string literals from ordinary string literals. The parentheses are there to allow ("unescapted") double quotes. For example:

	R"("quoted string")"

So, how do we get the character sequence `)` into a raw string literal? Fortunately, that's rare problem, but `"(` and `)"` is only the default delimiter pair. we can add delimiters before the `(` and after the `)` in `(...)`. For example:
	
	R"***("quoted string containing the usual terminator("))")***"

In contrast nonraw string literals, a raw string literal can contain a newline. For example:

	string counts {R"(1
		22
		333)"};

is equivalent to:

	string x {"1\n22\n333"};

A string with the prefix `L` such as `L"angst"` is a string of wide characters. Its type is `const wchar_t[]`. Similarly, a string with the prefix `LR` is a raw string.

	"folder\\file";
	R"(folder\file)";
	u8"folder\\file";
	u8R"(folder\file)";
	u"folder\\file";
	uR"(folder\file)";
	U"folder\\file";
	UR"(folder\file)";

##Pointers into Arrays

the integer value of `p+1` will be `sizeof(T)` larger than the integer value of `p`. For example:

	template<typename T>
	int byte_diff(T* p, T* q) {
		return reinterprete_cast<char*>(q) - 
			reinterprete_cast<char*>(p);
	}

###Pointers and const

Many objects do not have their values changed after initialization:

*	Symbolic constants lead to more maintainable code than using literals directly in code.
*	Many pointers are often read through but never written through
*	Most functions parameters are read but not written to.

Note that `const` modifies a type: it restricts the ways in which an object can be used, rather than specifying how the constant is to be allocated.

You can assign the address of a `non-cast` variable to be a pointer to constant because no harm can come from that. However , the address of a constant can not be assigned to an unrestricted pointer, because this would allow the object's value to be changed.

	void f() {
		int a = 1;
		const int c = 2;
		const int *p1 = &c;		// OK
		const int *p2 = &a;		// OK
		int *p3 = &c;			// error
		*p3 = 7;				// try to change the value of c
	}

It is possible, but typically unwise, to explicitly remove the restrictions on a pointer to `const` by explicit type conversion.

##Pointers and Ownership




