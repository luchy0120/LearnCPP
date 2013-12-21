###Names

*	名称中含有`$`可能造成不可移植的代码
*	以`_`开头的非局部名称或者(以`_`或`__`开头但是后面跟上大写字母，比如`_Foo`)的变量是被保留的
*	不要使用匈牙利命名方法，比如：`char *pcname`和`int icount`，因为在动态语言或者弱类型系统中可能会自动做掉

以下内容摘自原书：

*	Encoding types in names lowers the abstraction level of the program; in particular, it prevents generic programming(which relies on a name being able to refer to entities of different types)
*	The compiler is better at keeping track of types than you are
*	If you want to change the type of a name(e.g. use a `std::string` to hold the name), you will have to change every use of the name(or the type encoding becomes a lie)
*	Any system of type abbreviations you can come up with will become overelaborate and cryptic system of types you use increases.

新的C++关键字

*	`alignas`:
*	`alignof`:

###Scope

	int x;
	void f2() {
		int x = 1;		// hide global x
		::x = 2;		// assign to global x
		x = 2;			// assign to local x
		// ...
	}

Another snippet

	int x = 97;

	void f3() {
		int x = x;		// preverse: initialize x with its own(uninitialize) value
	}

Another snippet

	// the names of function arguments are considered declared in the outmost block of a function
	void f5(int x) {
		int x;			// error,
	}

A declaration is not allowed as the only statement on the branch of an if-statement

###Initialization

An initializer can use one of four syntactic styles:

	X a1 {x};
	X a2 = {v};
	X a3 = v;
	X a4(v);

Of these, only the first can be used in every context.

list initialization does not allow narrowing.

*	An integer can not be converted to another integer that can not hold its value. For example, `char` to `int` is allowed, but not `int` to `char`
*	A floating-point value can not be converted to another float-point type that can not hold its value. For example, `float` to `double` is allowed, but not `double` to `float`
*	A floating-point value can not be converted to an integer type.
*	An integer value can not be converted to a floating-point type.

	auto z1 {99};		// z1 is an initializer_list<int>
	auto z2 = 99;		// z2 is an int

So prefer `=` when using auto.

	vector<string> v1{"hello!"};	// v1 is a vector of 1 element with the value "hello!"
	vector<string> v2("Hello!");	// error: no vector constructor takes a string literal

So prefer {} initialization over alternatives unless you have a strong reason not to.

The empty initializer list {} is used to indicate that a default value is desired.

The only really good case for an uninitialized variable is a large input buffer. 

	constexpr int max = 1024 * 1024;
	char buf[max];
	some_stream.get(buf, max);

We would easily have initialied buf:

	char buf[max]{};		// initialize every char to 0

If no initializer is specified, a `global`, `namespace`, local `static` or `static` member is initialized to {} of the appropriate type.

Local variable and objects created on the free store (sometimes called dynamic objects or heap objects) are not initialized by default unless they are of user-defined types with a default constructor.

    void f() {
        int x;                      // x does not have a well-defined value
        char buf[1024];             // buf[i] does not have a well-defined value
        int *p {new int};           // *p does not have a well-defined value
        char *q{new char[1024]};    // q[i] does not have a well-defined value
        
        string s;                   // a == "" because of string's default constructor
        vector<char> v;             // v = {} because of vector's default constructor
        
        string *ps {new string};    // *ps is "" because of string's default constructor
        // ...
    }
    
###Deducing a Type

`auto` and `decltype`

`auto` for deducing a type of an object from its initializer; the type can be the type of a variable, a `const`, or `constexpr`

`decltype(expr)` for deducing the type of something that is not a simple initializer, such as the return type for a function or the type of a class member.

They simply report the type of an expression already known to the compiler.

Note that the type of an expression is never a reference because references are implicity dereferenced in expressions. For example:

	void g(int& v) {
		auto x = v;		// x is an int, not an int&
		auto& y = v;	// y is an int&
	}

In face, it can be an advantage to use the `=` syntax, because the `{}`-list might surprise someone:

	auto v1 {12345};	// v1 is a list of int
	auto v2 {'c'};		// v2 is a list of char
	auto v3 {f()};		// v3 is a list of some appropriate type

This is logical:

	auto x0 {};			// error: can not deduce a type
	auto x1 {1};		// list of int with one element
	auto x2 {1, 2};		// list of int with two elements
	auto x3 {1, 2, 3};	// list of int with three elements

Consequently, using `=` rather than `{}` for objects specified `auto` whenever we do not mean "list"

Example of using `decltype` specifier

	template<class T, class U>
	auto operator+ (const Matrix<T>& a, const Matrix<U>& b)->Matrix<decltype(T{} + U{})>;

##Object and Values

an `object` is a contiguous region of storage; an `lvalue` is an expression that refers to an object. The word `lavlue` was originally coined to mean "something that can be on the left-hand side of an assignment". However, not every lvalue may be used on the left-hand side of an assignment; an lvalue can refer to a constant. An lvalue that has not been declared `const` is often called a modifiable lvalue. This simple and low-level notion of an object should not be confused with the notions of class object and object of polymorphic type.

###Lvalues and Rvalues

Roughly, rvalue means "a value that is not an lvalue", such as a temporary value(e.g., the value returned by a function)

There are two properties that matter for an object when it comes to addressing, copying, and moving:

*	`Has identity`: The program has the name of, pointer to, or reference to the object so that it is possible to determine if two objects are the same, whether the value of the object has changed, etc.
*	`Movable`: The object may be moved from(i.e., we are allowed to move its value to another location and leave the object in a valid but unspecified state, rather than copying);

It turns out that three of the four possible combinations of those two properties are needed to precisely describe the C++ language rules(we have no need for object that do not have identity and can not be moved). Using `m` for movable and `i` for has identity. We can represent this classfication of expressions graphically.

	lvalue: (has identity but can not be moved)(i & !m)
	xvalue:	(has identity and can be moved)(i & m)
	prvalue: (has no identity but can be moved)(!i & m)
	glvalue: (has identity)(i)
	rvalue: (can be moved)(m)

So `glvalue` contains `xvalue` and `lvalue`, and `rvalue` contains `xvalue` and `prvalue`

	void f(vector<string>& vs) {
		vector<string>& v2 = std::move(vs);
	}

Note that every expression is either an lvalue or an rvalue, but not both.

###Lifetimes of Objects

We can classfy objects based on their lifetimes:

*	Automatic: Unless the programmer specifies otherwise, an object declared in a function is created when its definition is encountered and destroyed when its name goes out of scope. Such objects are sometimes called automatic objects. In a typical implementation, automatic objects are allocated on the stack; each call of the function gets its own stack frame to hold its automatic objects.
*	Static: Objects declared in global or namespace scope and `static` declared in functions or classes are created and initialized once(only), and "live" until the program terminates. Such objects are called static objects. A static object has the same address throughout the life of a program execution. static objects can cause serious problems in a multii-threaded program because they are shared among all threads and typically require locking to avoid data races.
*	Free store: Using the `new` and `delete` operators, we can create objects whose lifetimes are controlled directly.
*	Temporary objects: intermediate results in a computation or an object used to hold a value for a reference to const argument: their lifetime is that of the reference; otherwise, they "live" until the end of the full expression of which they are part. A full expression is an expression that is not part of another expression, typically, temporary objects are automatic.
*	Thread-local objects: that is, objects declared `thread_local`: such objects are created when their thread is and destroyed when their thread is.

Static and automatic are traditionally refered to as storage classes.

Array elements and nonstatic class members have their lifetimes determined by the object of which they are part.

##Type Aliases

Cases when we would need a new name for a type.

*	The original name is too long, complicated or ugly(in some programmer's eyes)
*	A programming technique requires different types to have the same name in a context.
*	A specific type is mentioned in one place only to simplify maintaince.

	using Pchar = char*;		// pointer to character
	using PF = int(*)(double);	// pointer to function taking a double and returning an int.

Similar types can define the same name as a member alias:

	template<class T>
	class vector {
		using value_type = T;
	}

Another example

	Pchar p1 = nullptr;		// p1 is a char*
	char* p3 = p1;			// fine

We can not apply type specifiers, such as `unsigned` to an alias.

	using Char = char;
	using Uchar = unsigned Char;	// error
	using Uchar = unsigned char;	// OK

