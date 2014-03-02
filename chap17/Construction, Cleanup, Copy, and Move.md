The difference between *move* and *copy* is that after a copy two objects must have the same value, whereas after a move the source of the move is not required to have its original value. Moves can be used when the source object will not be used again.

There are five situation in which an object is copied or moved:

*	As the source of an assignment
*	As an object initializer
*	As a function argument
*	As a function return type
*	As an exception

The name of a class can not be used for an ordinary member function, data member, member type.

A type that has no destructor declared, such as a built-in type, is considered to have a destructor that does nothing.

A constructor builds a class object "from the bottom up":

1.	First, the constructor invokes its base class constructors,

2.	then, it invokes the member constructors, and

3.	finally, it executes its own body

A destructor "tears down" an object in the reverse order:

1.	First, the destructor executes its own body

2.	Then, it invokes its member destructors, and

3.	Finally, it invokes its base class destructors

Constructors execute member and base constructors in declaration order (not the order of initializers): if two constructors used a different order, the destructor could not (without serious overhead) guarantee to destroy in the reverse order of construction.

If a class is used so that a default constructor is needed, and if the class does not have other constructors, the compiler will try to generate a default constructor.

	struct S1 {
		string s;
	};

	S1 x\;		// OK, x.s is initialized to ""

	struct X 
	{
		X(int);
	};

	S2 x1;		// error
	S2 x2{1};	// OK, x2.x is initialized with 1

We can prevent destruction of an X by declaring its destructor `=delete` or `private`

	class Nonlocal {
	public:
		// ...
		void destroy() {this->~Nonlocal();}
	private:
		// ...
		~Nonlocal();
	};

	void user() {
		Nonlocal x;	// error, can not destroy a nonlocal
		X *p = new Nonlocal;	// OK
		// ...
		delete p;	// error, can not destroy a Nonlocal
		p->destroy();	// OK
	}

The reason we need a virtual destructor is that an object usually manipulated through the interface provided by a base class is often also deleted through that interface.

We can initialize objects of a class for which we have not defined a constructor using:

*	memberwise initialization
*	copy initialization, or
*	default initialization(without an initializer or with an empty initializer list)

Where no constructor requiring arguments is declared, it is also possible to leave out the initializer completely. 

	Work alpha;

	void f() {
		Work beta;
	}

For statically allocated objects, the rule are exactly as if you had used `{}`, so the value of alpha is `{"", "", 0}`, However, for local variables and free-store objects, the default initialization is done only for members of class type, and members of built-in type are left uninitialized, so the value of beta is `{"", "", unknown}`

Note that the default constructor disappears when you define a constructor requiring arguments;

`{}` initialization is sometimes refered to as universal initialization: the notion can be used everywhere. In addition, `{}` initialization is uniform.

	int *p1 = new int;		// uninitialized int
	int *p2 = new int{};	// the int is initialized to 0

Referneces and consts must be initialized.

When shoud a class have a default constructor? A simple minded technical answer is "when you use it as the element type for an array, etc.". However, a better question is "for what types does it make sense to have a default value?" or even "Does this type have a 'special' value we can 'naturally' use as a default".

The mechanism for accepting a {}-list is a function(often a constructor) taking an argument of type std::initializer_list<T>.

For selecting a constructor, default and initializer lists take precedence.

	struct X {
		X(initializer_list<T>);
		X();
		X(int);
	};

	X x0 {};		// empty list: deafult constructor
	X x1 {1};		// the initializer-list constructor

The rules are:

*	If either a default constructor or an initializer-list constructor could be invoked, prefer the default constructor.
*	If both an initializer-list constructor and an "ordinary constructor" could be invoked, prefer the initializer-list constructor

If we really want to invoke the constructor taking one or two integer arguments, we must use the `()` notation.

A function with an `initializer_list<T>` argument can access it as a sequence using the member functions `begin()`, `end()` and `size()`. initializer_list does not provide subscripting.

The elements of an initializer_list are immutable. Do not even think about trying to modify their values.

Because `initializer_list` elements are immutable, we can not apply a move constructor to them.

The distinction between direct initializaton and copy initializaton is maintained for `{}` initialization. For a container, this implies that the distinction is applies to both the container and its elements

The members' constructors are called before the body of the containing class's own constructor is executed. The constructors are called in the order in which the members are declared in the class rather than the order in which the members appear in the initializer list. To avoid confusion, it is best to specify the intializers in the member declaration order. Hope for a compiler warning if you do not get the order right. The member destructors are called in the reverse of construction after the body of the class's own destructor has been executed.

If a member constructor needs no arguments, the member need not be mentioned in the member initializer list. 

"Implicitly initialized" member of a built-in type is left uninitialized

That is, a member-style initializer using the class's own name(its constructor name) calls another constructor as part of the construction. Such a constructor is called a delegating constructor(and occasionally a forwarding constructor)

You can not both delegate and explicitly initialize a member.

We can specify an initializer for a non-static data member in the class declaration. for example:

	class A {
	public:
		int a{7};
		int b = 77;
	};

For pretty obscure technical reasons related to parsing and name lookup, the `{}` and `=` initialzer notations can be used for in-class member initializers, but the `()` notation can not.

If a member is initialized by both an in-class initializer and a constructor, only the constructor's initialization is done(it overrides the default)

It is a bad idea to hide subtle dependencies on global data in member initializers.

However, for a few simple special cases, it is possible to initialize a static member in the class declaration. The static member must be a const of an integral enumeration type, or a constexpr of a literal type, and the initializer must be a constant-expression.

	class Curious {
	public:
		static const int c1 = 7;		// OK
		static int c2 = 11;				// error: not const
		const int c3 = 14;				// OK, but not static
		static const int c4 = sqrt(9);	// error: in class initializer not constant
		static const float c5 = 7.0;	// error: in-class not integral(use constexpr rather than const)
		// ...
	};

Typically, a move can not throw, whereas a copy might(because it may need to acquire a resource), and a move is often more efficient than a copy. When you write a move operation, you shoud leave the source object in a valid but unspecified state because it will eventually be destroyed and the destructor can not destroy an object left in an invalid state. Also, standard library algorithms rely on being able to assign to(using move or copy) a moved-from object. So, design your moves not to throw, and to leave their source objects in a state that allows destruction and assignment.

In particular, move operations typically do not throw exceptions; they do not acquire resources or do complicated operations, so they do not tend to.

How does the compiler know when it can use a move operation rather than a copy operation? In a few cases, such as for a return value, the language rules say that it can(because the next action is defined to destroy the element). However, in general we have to tell it by giving an rvalue reference argument. For example:

	template<class T>
	void swap(T& a, T& b) {
		T tmp = std::move(a);
		a = std::move(b);
		b = std::move(tmp);
	}

The `move` is a standard library function returning an rvalue reference to its argument: move(x) means "give me an rvalue reference to x", that is, std::move(x) does not move anything; instead, it allows a user to move x. It would have been better if move() had been called rval(), but the name move() has been used for this operation for years.

Here are some rules:

*	If the programmer declares any constructor for a class, the default constructor is not generated for that class.
*	If the programmer declares a copy operation, a move operation, or a destructor for a class, no copy operation, move operation, or destructor is generated for that class.

Unfortunately, the second rule is only incompletely enforced: for backward compatibility, copy constructors and copy assignments are generated even if a destructor is defined.

Wherever possible:

*	Establish an invariant in a constructor(including possibly resource acquisition)
*	Maintain the invariant with copy and move operations(with the usual names and types)
*	Do any needed cleanup in the destructor(incl. possibly resource release)

###Resource Invariants

For every class, we should ask:

*	Is a default constructor needed(because the default one is not adequate or has been suppressed by another constructor)
*	Is a destructor needed(because some resource needs to be released)
*	Are copy operations needed(because the default copy semantics is not adequate, e.g, because the class is meant to be a base class or because it contains pointers to objects that must be deleted by the class)
*	Are move operations needed(because the default semantics is not adequate, because an empty object does not make sense)

We can eliminate a specialization from the set of possible specializations of a function template:

	template<class T>
	T* clone(T *p)	{ // return copy of *p 
		return new T{*p};
	}

	Foo* clone(Foo*)=delete;	// do not try to clone a Foo

A further use is to control where a class can be allocated:

	class Not_on_stack {
		~Not_on_stack()=delete;
	};

	class Not_on_free_store {
		void *operator new(size_t)=delete;
	};

Make sure that copy assignments are safe for self-assignment

