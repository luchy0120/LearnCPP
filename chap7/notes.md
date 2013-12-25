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

A resource is something that has to be acquired and later released. Memory acquired by `new` and released by `delete` and files opened by `fopen` and closed by `fclose`.

	void confused(int *p) {
		// delete p?
	}
	
	int global {7};

	void f() {
		X *pn = new int {7};
		int i{7};
		int q = &i;
		confused(pn);
		confused(q);
		confused(&global);a
	}

If `confused` deletes p, the Program will seriously misbehave for the second two calls because we may not `delete` objects not allocated by `new`. If `confused` does not delete p, the program leaks.

It is usually a good idea to immediately place a pointer that represents ownership in a resource handle class, such as `vector`, `string` and `unique_ptr`. That way, we can assume that every pointer that resource management in greater detail.

##References

Using a pointer differs from using the name of an object in a few ways:

*	We use a different syntax, for example, `*p` instead of `obj` and `p->m` rather than `obj.m`.
*	We can make a pointer point to different objects at different times.
*	We must be more careful when using pointers than when using an object directly: a pointer may be a `nullptr` or point to an object that wasn't the one we expected.

Like a pointer, a `reference` is an alias for an object, it usually implemented to hold a machine address of an object, and does not impose performance overhead compared to pointers, but it differs from a pointer in that:

*	You access a reference with exactly the same syntax as the name of an object.
*	A reference always refers to the object to which it was initialized.
*	There is no "null reference", and we may assume that a reference refers to an object.

The main use of reference is for specifying arguments and return values for functions in general and for overloaded operators in particular.

	template<class T>
	class Vector {
		T* elem;
		// ...
	public:
		T& operator[](int i) {return elem[i];}
		const T& operator[](int i) const {return elem[i];}
		void push_back(const T& a);
	};

	void f(const Vector<double>& v) {
		double d1 = v[1];
		v[2] = 7;

		v.push_back(d1);
	}

To reflect the lvalue/rvalue and const/non-const distinctions, there are three kinds of references:

*	`lvalue references`: to refer to objects whose value we want to change
*	`const references`: to refer to objects whose value we do not want to change
*	`rvalue references`: to refer to objects whose value we do not need to preserve after we have used it.

Collectively, they are called references, the first two are both called lvalue references.

###Lvalue References

In a type name, the notation `X&` means "reference to X", It is used for references to lvalues. so it is often called an `lvalue references`.

Initialization of a reference is something quite different from assignment to it. Despite appearances, no operator operates on a reference.

	void g() {
		int var = 0;
		int& rr{var};
		++rr;
		int *pp = &rr;
	}

Here, `++rr` does not increment the reference `rr`, rather, `++` is applied to the `int` to which `rr` refers, that is, to `var`. Consequently, the value of a reference can not be changed after initialization; it always refers to the object it was initialized to denote. To get a pointer to the object denoted by a reference `rr`, we can write `&rr`. Thus, we can not have a pointer to a reference. Furthermore, we can not define an array of referenecs.

The obvious implementation of a reference is as a (constant) pointer that is dereferenced each time it is used.

Initialization of a reference is trivial when the initializer is an lvalue(an object whose address you can take). The initializer for a plain `T&` must be a lvaue of type T.

The initializer for a `const T&` need not be an lvalue or even of type `T`. In such cases:

1.	First, implicit type conversion to `T` is applied if necessary.
2.	Then, the resulting value is placed in a temporary variable of type `T`
3.	Finally, this temporary variable is used as the value of the initializer.

Consider:

	double& dr = 1;			// error, lvalue needed. 1 is prvalue
	const double& cdr{1};	// OK

The interpretation of this last initialization might be:
	
	double temp = double{1};
	const double& cdr {temp};

References to variables and references to constant are distinguished because introducing a temporary for a variable would have been highly error-prone; an assignment to the variable would become an assignment to the - soon-to-disappear - temporary. No such problem exist for references to constants, and references to constants are often important as function arguments.

To keep program readaable, it is often best to avoid functions that modify their arguments. Instead, you can return a value from the function explicitly.

References can also be used as return types. This is mostly used to define functions that can be used on both left-hand and right-hand sides of an assignment. 

	map<int, char> m;
	m[96] = 'a';		// left hand side
	char c = m[96];		// right hand side

###Rvalue References

The basic idea of having more than one kind of reference is to support different uses of objects:

*	A non-const lvalue reference refers to an object, to which the user of the reference can write.
*	A const lvalue reference refers to a constant, which is immutable from the point of view of the user of the reference.
*	An rvalue reference refers to a temporary object, which the user of the reference can (and typically will) modify, assuming that the object will never be used again.

An rvalue reference can bind to an rvalue, but not to an lvalue. In that, an rvalue reference is exactly opposite to an lvalue reference.

	string var{"Cambrige"};
	string f();

	string& r1 {var};			// lvalue reference, bind r1 to var (an lvalue)
	string& r2 {f()};			// lvalue reference. error: f() is an rvalue
	string& r3 {"Princeton";}	// lvalue reference, error: can not bind to temporary
	
	string&& rr1 {f()};			// rvalue reference, fine: bind rr1 to rvalue(a temporary)
	string&& rr2 {var};			// rvalue reference, error: var is an lvalue
	string&& rr3 {"Oxford"};	// rr3 refers to a temporary holding "Oxford";

We do not use `const` rvalue references; Both a const lvalue reference and an rvalue reference can bind to an rvalue.

*	We use rvalue reference to implement a `destructive read` for optimization of what would otherwise have required a copy.
*	We use const lvalue reference to prevent modification of an argument.

Sometimes, a programmer knowns that an object won't be used again, even though the compiler does not. Consider:

	template<class T>
	swap(T& a, T& b) {
		T tmp {a};
		a = b;
		b = tmp;
	}

Can be changed to(only swap lvalues):

	template<class T>
	void swap(T& a, T& b) {
		T tmp {static_cast<T&&>(a)};
		a = static_cast<T&&>(b);
		b = static_cast<T&&>(tmp)
	}

`vector` has move constructor:

	vector<string> s;
	vector<string> s2 {s};				// s is an lvalue, so use copy constructor
	vector<string> s3 {s + "tail"};		// s + "tail" is an rvalue, so pick move constructor

`move(x)` does not move x(it simply produces an rvalue reference to x), it would have been bettern if `move(x)` had been called `rval()`, but by now `move()` has been used for years.

###References to References

If you take a reference to a reference to a type, you get a reference to that type, rather than some kind of special reference to reference type.

	using rr_i = int&&;
	using lt_i = int&;
	using rr_rr_i = rr_i&&;			// int&& && is an int&&
	using lr_rr_i = rr_i&;			// int&& & is an int&
	using rr_lr_i = lr_i&&;			// int& && is an int&
	using lr_lr_i = lr_i&;			// int& & is an int&

In other words, lvalue reference always wins. This makes sense: nothing we can do with types can change the fact that an lvalue reference refers to an lvalue. This is sometimes known as reference collapse.

The syntax does not allow: `int && & r = i;

###Pointers and References

You can not create null reference:

	char* ident(char* p) {return p;}
	char& r {*ident(nullptr)};		// invalid code
