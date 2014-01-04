`&&`, `||`, `!` take operands of *arithmetic* and *pointer* types.

The `&&` and `||` operators evaluate their second argument only if necessary.

The bitwise logical operator `&`(and), `|`(or), `^`(exclusive or), `~`(complement), `>>`(right shift), `<<`(left shift) are applied to objects of integral types - that is, `char`, `short`, `int`, `long`, `long long` and their unsigned counterparts and `bool`, `wchar_t`, `char16_t`, `char32_t`. a plain enum(not enum class).

`&` (intersection), `|` (union), `^` (symmetric difference) `~` (complement)

`&` has higher precendence than `|`

A named object has its lifetime determined  by its scope.

The `delete` operator may be applied only to a pointer returned by `new` or to the `nullptr`. Applying `delete` to `nullptr` has no effect.

The main problems with free store are:

*	Leaked objects: People use `new` and then forget to `delete` the allocated object.
*	Premature deletion: People `delete` an object that they have some other pointer to and later use that other pointer.
*	Double deletion: An object is deleted twice, invoking its destructor twice.

general approaches to resource management that avoid such problems:

*	Do not put objects on the free store if you do not have to;
*	When you construct an object on the free store, place its pointer into a manager object(somtimes called a handle), with a destructor that will destroy it. `string`, `vector` and all other standard-library containers, `unique_ptr` and `shared_ptr`. Whenver possible, have that manager object be a scoped variable.

"no leaked news"; that is, `new` belongs in constructors and similar operations, `delete` belongs in destructors, and together they provide a coherent memory management strategy. In addition, `new` is often used in arguments to resource handles.

The plain operator `delete` is used to delete individual objects; `delete []` is used to delete arrays.

Note that a `vector` is a proper object and can therefore be allocated and deallocated using plain `new` and `delete`

	void f(int n) {
		vector<int> *p = new vector<int>(n);
		int *q = new int[n];

		// ..
		delete p;
		delete [] q;
	}

Do not use `new` to create local objects.

The standard implementation of `operator new()` and `operator new[]()` do not initialize the memory returned.

In addition to the functions defined in `<new>`, a user can define `operator new()` for a specific class. Class members `operator new()` are found and used in preference to the ones from `<new>` according to the usual scope rules

The placement delete operators do nothing except possibly inform a garbage collector that the `deleted` pointer is no longer safely derived.

The placement new construct can also be used to allocated memory from a specific arena.

Note that explicit calls of destructors should be avoided except in the implementation of resource management classes.

In programs where exceptions must be avoided, we can use `nothrow` versions of `new` and `delete`.

	void f(int n) {
		int *p = new(nothrow)int[n];
		if (p == nullptr) {
			
		}
		// ...
		operator delete(nothrow, p);
	}

That `nothrow` is the name of an object of the standard-library type `nothrow_t` that is used for disambiguation; nothrow and nothrow_t are declared in `<new>`

`{}`-list can appear in two forms:

*	Qualified by a type, `T{}` meaning "create an object of type T intialized by T{..}"
*	Unqualified `{...}`, for which the type must be determined from the context of use.

The underlying array is immutable, so there is no way (within the standard's rules) that the meaning of a `{}-list` can change between two users.

	void f() {
		initializer_list<int> lst {1, 2, 3};
		cout << *lst.begin() << '\n';
		*lst.begin() = 2;		// error, lst is immutable
		cout << *lst.begin() << '\n';
	}

In partiular, have a `{}-list` be immutable that a container taking elements from it must use a copy operation, rather than a move operation.

One way of looking at a qualified initializer list with one element is as a conversion from one type to another.

	template<class T>
	T square(T x) {
		return x * x;
	}

	void f(int i) {
		double d = square(double{i});
		complex<double> z = square(complex<double>{i});
	}

A unqualified list is used where an expected type is unambiguously known, it can be used as an expression only as:

*	A function argument
*	A return type
*	The right-hand operand of an assignment operator(`=`, `+=`, `*=`, etc.)
*	A subscript

	int f(double d, Matrix& m) {
		int v{7};				// initializer(direct initialization)
		int v2 = {7};			// initializer(copy initialization)
		int v3 = m[{2, 3}];		// assume m takes value pairs as subscripts

		v = {8};				// right-hand operand of assignment
		v += {88};				// right-hand operand of assignment
		{v} = 9;				// error: not left-hand operand of assignment
		v = 7 + {10};			// error: not an operand of a non-assignment operator
		f({10.0});				// function argument
		return {11};			// return type
	}

The type of a `{}`-list can be deduced only if all elements are of the same type.

We do not deduce the type of an unqualified list for a plain template argument.

	template<typename T>
	void f(T);

	f({});				// error
	f({1});				// error
	f({1, 2});			// error
	f({1, 2, 3});		// error

We do not deduce the element type of a container represented as a template.

	template<class T>
	void f2(const vector<T>&);

	f2({1, 2, 3});			// error
	f2({"Kona", "Sidney"});	// error

##Lambda Expressions

a lambda expression consists of a sequence of parts:

*	A possibly empty *capture* list, specifying what names from the definition environment can be used in the lambda expression's body, and whether those are copied or accessed by reference. The capture list is delimited by `[]`.
*	An optional parameter list, specifying what arguments the lambda expression requires. The parameter list is delimited by `()`
*	An optional mutable specifier, indicating that the lambda expression's body may modify the state of the lambda
*	An optional `noexcept` specifier
*	An optional return type declaration of the form `->` type
*	A body, specifying the code to be executed. The body is delimited by `{}`

By default,	`operator()()` is const. so that the lambda body does not modify the captured variables. That's by far the most common case. Should you want to modify the state of a lambda for its body, the lambda can be declared `mutable`. This corresponds to an `operator()()` not being declared const.

If you need to capture a variadic template argument, use `...`, for example:

	template<typename... Var>
	void algo(int s, Var... v) {
		auto helper = [&s, &v...]{return s*(h1(v...) + h2(v...));};
		// ...
	}

We do not need to "capture" namespace variables(including global variables) because they are always accessible (provided they are in scope).

We can include class members in the set of names potentially captured by adding `this` to the capture list.

members are always captured by reference. That is, `[this]` implies that members are accessed through this rather copied into the lambda. Unfortunately, `[this]` and `[=]` are incompatible. This implies that incautious use can lead to race conditions in multi-threaded programs.

If a lambda expression does not take any arguments, the argument list can be omitted, Thus, the minimal lambda expression is `[]{}`.

A lambda expression's return type can be deduced from its body. Unfortunately, that is not also done for a function.

	void g(double y) {
		[&]{f(y);};		// return type is void
		auto z1 = [=](int x) {return x+y;};	// return type is double
		auto z2 = [=, y](){if(y) return 1; else return 2;};	// eror, body too complicated for return type deduction
		auto z3 = [y]() {return 1: 2;}		// return type is int; may be error
		auto z4 = [=, y]()->int {if(y) return 1; else return 2;};// OK, explicit return type
	}

To allow for optimized versions of lambda expressions, the type of a lambda expression is not defined. this type, called the *closure type*, is unique to the lambda, so no two lambdas have the same type. In addition to using a lambda as an argument, we can use it to initialize a variable declared `auto` or `std::function<R(AL)>` where R is the lambda's return type and AL is its argument list of types.

For example:

	auto rev = [&rev](char* b, char* e) {
		if (1 < e-b) {swap(*b, *--e); rev(++b, e);}
	};	// error

However, that's not possible because I can not use an `auto` variable before its type has been deduced, Instead, I can introduce a name and then use it:

	void f(string& s1, string& s2) {
		function<void(char* b char* e)> rev = [&](char* b, char* e) {
			if (1< e-b) {swap(*b, *--e); rev(++b, e);}
		};
		rev(&s1[0], &s1[0] + s1.size());
		rev(&s2[0], &s2[0] + s2.size());
	}

A lambda that captures nothing can be assigned to a pointer to function of an approriate.

	double (*p1)(double) = [](double a) {return sqrt(a);};

##Explicit Type Conversion

C++ offers explicit type conversion operations of varying convenience and safety:

*	Construction, using the `{}` notation, providing type-safe construction of new values.
*	Names conversions, providing conversions of various degrees of nastiness:`const_caast`(get write access to something declared const), `static_cast`(reversing a well-defined implicit conversion), `reinterpre_cast`(changing the meaning of bit patterns), `dynamic_cast`(dynamically checked class hierarchy navigation)
*	C-style cast.
*	Functional notation.

The construction of a value of type T from a value e can be expressed by the notation `T{e}`.

	auto d1 = double{2};		// d1 == 2.0
	double d2 {double{2} / 4};	// d2 == 0.5

Part of the attraction of the `T{v}` notation is that it will only perform only `well-behaved` conversions.

	void f(int);
	void f(double);

	void g(int i, double d) {
		f(i);
		f(double{i});					// error: {} does not do int to floating conversion

		f(d);
		f(int{d});						// errir: {} does not truncate
		f(static_cast<int>(d));

		f(round(d));
		f(static_cast<int>(lround(d)));
	}

Explicitly constructed unnamed objects are temporary objects, and(unless bound to a reference), their lifetime is limited do the full expression in which they are used. In this, they differ from unnamed objects created using `new`.

`static_cast` converts between related types such as one pointer type to another in the same class hierarchy, an integral type to an enumeration, or a floating-point type to an integral type. It also does conversions defined by constructors and conversion operators.

`reinterpret_cast` handles conveersions between unrelated types such as an integer to a pointeer or a pointer to an unrelated pointer type.

`const_cast` converts between types that differ only in const and volatile qualifiers.

`dynamic_cast` does run-time checked conversion of pointers and references into a class hierarchy.

The result of a `reinterpret_cast` is guaranteed to be usable only of its result is converted back to the exact original type.

Note that `reinterpret_cast` is the kind of conversion that must be used for pointers to functions.

The C-style cast can also cast from a pointer to a class to a pointer to a private base of that class.

The construction of a value of type T from a value e can be expressed by the functional notation `T(e)`:

	void f(double d) {
		int i = int(d);			// truncate d
		complex z = complex(d);	// make a complex from d
	}


