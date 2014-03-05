###Subscripting

An operator[] function can be used to give subscripts a meaning for class objects. The second argument(the subscript) of an operator[] function may be of any type. This makes it possible to define vectors, associative arrays, etc.

###Dereferencing

The dereferencing operator, `->`(also known as the arrow operator), can be defined as a unary post-fix operator. For example:

	class Ptr {
		X* operator->();
	};

Operator `->` must be a non-static member function, If used, its return type must be a pointer or an object of a class to which you can apply `->`.The body of a template class member function is only checked if the function is used, so we can define `operator->()` without worrying about types, such as Ptr<int> for which -> does not make sense.

Despite the similarity between `->` and `.`(dot), there is no way of overloading operator .(dot)

###Increment and Decrement

Member operator new()s and operator delete()s are implicitly static members. Consequently, they do not have a this pointer and do not modify an object. They provide storage that a constructor can initialize and a destructor can clean up.

###User-defined Literals

The name of a literal operator is `operator""` followed by the suffix.

There are four kinds of literals that can be suffixed to make a user-defined literal:

*	An integer literal: accepted by a literal operator taking an `unsigned long long` or a `const char*` argument or by a template literal operator, for example, `123m` or `12345678901234567890X`
*	A floating-point literal: accepted by a literal operator taking a `long double` or a `const char*` argument or by a template literal operator, for example: `12345678901234567890.976543210x` or `3.99s`
*	A string literal: accepted by a literal operator taking a (`const char*`, `size_t`) pair of arguments, for example, "string"s and R"(Foo\bar)"_path
*	A character literal: accepted by a literal operator taking a character argument of type char, wchar_t, char16_t or char32_t, for example, 'f'_runic or u'BEEF'_w

A template literal operator is a literal operator that takes its argument as a template parameter pack, rather than as a function argument. For example:

	template<char...>
	constexpr int operator""_b3();

Given that, we get:

	201_b3		// means operator""_b3<'2','0','1'>(); so 9*2+0*3+1 == 19
	241_b3		// means operator""_b3<'2','4','1'>(); so error: 4 is not a ternary digit

The variadic template techniques can be disconverting, but it is the only way of assigning nonstandard meanings to digits at compile time.

The standard library reserves all suffixes not starting with an initial underscore, so define your suffixes starting with an underscore or risk your code breaking in the future.

##Friends

An ordinary member function declaration specifies three logically distinct things:

*	The function can access the private part of the class declaration
*	The function is in the scope of the class
*	The function must be invoked on an object(has a this pointer)

By declaring a member function static, we can give it the first two properties only. By declaring a nonmember function a friend, we can give it the first property only. That is, a function declared frient is granted access to the implementation of a class just-like a member function but is otherwise independent of that class.

A friend declaration can be placed in either the private or the public part of a class declaration; it does not matter where. Like a member function, a friend function is explicitly declared in the declaration of the class of which it is a friend. It is therefore as much a part of that interface as is a member function.

A member function of one class can be the friend of another:

	class List_iterator {
		// ...
		int *next();
	};

	class List {
		frient int* List_iterator::next();
	};

There is a shorthand for making all functions of one class friends of another.

	class List {
		frient class List_iterator;
		// ...
	};

The friend declaration makes all of `List_iterator`'s member functions friends of List

It is possible to make a template argument a friend

	template<typename T>
	class X {
		friend T;
		friend class T;	// redundant "class"
	};

Often, there is a choice between making a class a member(a nested class) or a nonmember friend.

###Finding Friends

A friend must be previously declared in an enclosing scope or defined in the non-class scope immediately enclosing the class that is declaring it to be a friend. Scopes outside the innermost enclosing namespace scope are not considered for a name first declared as a friend.

	class C1 {};		// will become friend of N::C
	void f1();			// will become friend of N::C

	namespace N {
		class C2 {};	// will become friend of C
		void f2();		// will become friend of C

		class C {
			int x;

		public:
			friend class C1;	// OK(previously defined)
			friend void f1();

			friend class C3;	// OK(defined in enclosing namespace)
			friend void f3();

			friend class C4;	// First declared in N and assumed to be in N
			friend void f4();

			class C3 {};	// friend of C
			void f3() { C x; x.x = 1;}
		}	// namespace N

		class C4 {};	// not friend of N::C
		void f4() {N::C x; x.x=1;}	// error: x is private and f4() is not a friend of N::C
	}

A friend function can be found through its arguments even if it was not declared in the immediately enclosing scope.

Thus, a friend function should be explicitly declared in an enclosing scope or take an argument of its class or a class derived from that. If not, the friend can not be called.

	// no f() in this scope
	class X {
		friend void f();		// useless
		friend void h(const X&);	// can be found through its argument
	};

	void g(const X& x) {
		f();		// no f() in scope
		h(x);		// X's friend h()
	}


