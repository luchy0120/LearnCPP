Note that the C++ grammer is written so that the `{}` notation can only be used for initializers and on the right-hand side of an assignment:

	void g(complex a, complex b) {
		a = {1, 2};		// OK, right hand side of assignment
		a += {1, 2};	// OK, right hand side of assignment
		b = a + {1, 2};	// syntax error
		b = a + complex {1,2};	// OK
		g(a, {1, 2});	// OK: a function argument is considered an initializer
		{a, b} = {b, a};	// syntax error
	}

###Binary and Unary Operators

A binary operator can be defined by either a non-static member function taking one argument or a nonmember function taking two arguments. For any binary operator `@`, `aa@bb` can be interpreted as either `aa.operator @(bb)` or `operator@(aa, bb)`. If both are defined, overload resolution determines which, if any, interpretation is used.

	class X {
	public:
		void operator +(int);
		X(int);
	};

	void operator+(X, X);
	void operator+(X, double)

	void f(X a){
		a+1;		// a.operator+(1)
		1+a;		// ::operator+(X(1), a)
		a+1.0;		// ::operator(a, 1.0)
	}

A unary operator, whether prefix or postfix, can be defined by either a non-static member function taking no arguments or a nonmember function taking one argument. For any prefix unary operator `@`, `@aa` can be interpreted as either `aa.operator@()` or `operator@(a)`. If both are defined, overload resolution determines which, if any, interpretation is used. For any postfix unary operator `@`, `aa@` can be interpreted as either `aa.operator@(int)` or `operator@(aa, int)`.

The default meaning of `&&` `||` and `,` involves sequencing: the first operand is evaluated before the second(and for `&&` and `||` the second operand is not always evaluated). This special rule does not hold for user-defined versions of `&&`, `||` and `,`; instead these operators are treated exactly like other binary operators.

###Predefined Meanings for Operators

A compiler will not generate a definition of `Z::operator+=()` from the definitions of `Z::operator+()` and `Z::operator=()`

###Operators and User-Defined Types.

An operator function must either be a member or take at least one argument of a user-defined type(functions redefining the new and delete operators need not).

It is not possible to define an operator function that operates exclusively on pointers. This ensures that C++ is extensible but not mutable(with the exception of operators =, &, and , for class objects)

###Passing Objects

For arguments, we have two main choices:

*	Pass by value
*	Pass by reference

###Operators in Namespaces

Operators defined in namespaces can be found based on their operand types just as functions can be found based on their argument types. 

Consider a binary operator @, If x is of type X and y is of type Y, x@y is resolved like this:

*	If X is aa class, look for operator@ as a member of X or as a member of a base of X; and
*	look for declarations of operators@ in the context surrounding x@y; and
*	if X is defined in namespace N, look for declarations of operator@ in N, and
*	if Y is defined in namespace M, look for declarations of operator@ in M

Note that in operator lookup no preference is given to members over nonmembers. This differs from lookup of named functions. The lack of hiding of operators ensures that built-in operators are never inaccessible and that users can supply new meanings for an operator without modifying existing class declarations.

	X operator!(X);

	struct Z {
		Z operator!();					// does not hide ::operator!()
		X f(X x) {/* */ return !X;}		// invoke ::operator!(X)
		int f(int x) {/* */ return !x}	// invoke the built-in ! for ints
	};

In particular, the standard iostream defines << member function to output built-in types, and a user can define << to output user-defined types without modifying class ostream

##A Complex Number Type

###Literals

We have literals of built-in types. For example, 1.2 and 12e3 are literals of type double. For complex, we can some pretty close to that by declaring constructors constexpr.

	class Complex {
	public:
		constexpr Complex(double r=0, double i=0):re{r},im{i} {}
	};

Given that, a complex can be constructed from its constituent parts at compile time just like a literal from a built-in type.

It is possible to go further and introduce a user-defined literal in support of out complex type. In particular, we could define i to be a suffix meaning "imaginary". For example:

	constexpr complex<double> operator ""i(long double d)	// imaginary literal
	{
		return {0, d};
	}

This would allow us to write:

	complex z1 {1.2 + 12e3i};
	complex f(double d) {
		auto x{2.3i};
		return x+sqrt(d+12e3i) + 12e3i;
	} 

###Conversion Operators

A member function X::operator T() where T is a type name, defines a conversion from X to T.

Note that the type being converted to is part of the name of the operator and can not be repeated as the return value of the conversion function.

	Tiny::operator int() const {return v;}		// right
	int Tiny::operator int() const {return v;}	// error

If both user-defined conversions and user-defined operators are defined, it is possible to get ambiguities between the user-defined operators and the built-in operators. For example:

	int operator+(Tiny, Tiny);

	void f(Tiny t, int i) {
		t + i;		//error, ambiguous: "operator+(t, Tiny(i)) or int(t)+i"
	}

###Ambiguities

In some cases, a value of the desired type can be constructed by repeated use of constructors or conversion operators. This must be handled by explicit conversions; Only one level of user-defined implicit conversion is legal. In some cases, a value of the desired type can be constructed in more than one way, such cases are illegal.

User-defined conversions are considered only if a call can not be resolved without them, for example:

	class XX {XX(int)};

	void h(double);
	void h(XX);

	void k2() {
		h(1);	// h(double{1}) or h(XX{1})
	}

The call h(1) means h(double{1}) because the alternative uses only a standard conversion rather than a user-defined conversion.




