Objects of structure types can be assigned, passed as function arguments, and returned as the result from a function.

Other plausible operations, such as comparison(`==` and `!=`), are not available by default.

###`struct` Layout

An object of a struct holds its members in the order they are declared. 

	struct Readout {
		char hour;
		int value;
		char seq;
	};

Members are allocated in memory in declaration order, so the address of `hour` must be less than the address of `value`.

However, the size of an object of a `struct` is not necessary the sum of the sizes of its members. This is because many machines require objects of certain types to be allocated on architecture-dependent boundaries or handle such objects much more efficiently if they are.

So you can minimize wasted space by simply ordering members by size(largest member first), for example:

	struct Readout {
		int value;
		char hour;
		char seq;
	}

It is usually best to order members for readability and sort them by size only if there is a demonstrated to optimize.

Use of multiple access specifiers(`public`, `private` or `protected`) can affect layout.

###`struct` Names

The name of a `struct` can be used before the type is defined as long as that use does not require the name of a member or the size of the structure to be known. However, until the completion of the declaration of a `struct`, that `struct` is an incomplete type.

For reasons that reach into the prehistory of C, it is possible to declare a `struct` and a `non-struct` with the same name in the same scope. For example:

	struct stat {/* */};
	int stat(char* name, struct stat* buf);

the keywords, `class`, `union` and `enum` can be used as prefixes for disambiguation. However, it is best not to overload names to make such explicit disambiguation necessary.

###Structures and Classes

A `struct` is simply a `class` where the members are `public` by default. So, a `struct` can have member functions, In particular, a `struct` can have constructors.

###Structures and Arrays

The main advantages of 	`std::array` over a built-in array are that it is a proper object type(has assignment, etc.) and does not implicitly convert to a pointer to an individual element:
	
The disadvantage of `std::array` compared to a built-in array is that we can not deduce the number of elements from the length of the initializer.

	array<Point> point3 = {{1, 2}, {3, 4}, {5, 6}};	// error: number of elements not given

###Type Equivalence

The structs are different types even when they have the same members.

	struct S1 {int a; };
	struct S2 {int a; };
	S1 x;
	S2 y = x;		// error: type mismatch

A struct is also a different type from a type used as a member.

###Plain Old Data

a POD(plain old data) is an object that can be manipulated as "just data" without worrying about complications of class layouts or user-defined semantics for construction, copy, and move.

For us to manipulate an object as "just data", the object must:

*	not have a complicated layout(eg. with a vptr)
*	not have nonstandard(user-defined) copy semantics, and
*	have a trivial default constructor.

a pod object must be of:

*	a standard layout type, and
*	a trivially copyable type,(trivial type is a type with a trivial default constructor and a trivial copy and move operations)
*	a type with a trivial default constructor

Standard layout:

*	has a non-static member or a base that is not standard layout.
*	has a virtual function
*	has a virtual base
*	has a member that is a reference
*	has multiple access specifiers for non-static data members or
*	prevents important layout optimizations(by having non-static data members in more than one base class or in both the derived class and reference, or by having a base class of the same type as the first non-static data member)

The `is_pod` is a standard-library property predicate defined in `<type_traits>` allowing us to ask the question "Is T a POD" in our code. The base thing about `is_pod<T>` is that it saves us from remembering the exact rules for what a POD is.

	template<typename T>
	void mycopy(T* to, const T* from, int count)
	{
		if (is_pod<T>::value) {
			memcpy(to, from, count * sizeof(T));
		} else {
			for (int i = 0; i != count; ++i) {
				to[i] = from[i];
			}
		}
	}

Note that adding or subtracting non-default constructors does not affect layout or performance(that was not true in C++98)

###Fields

	struct PPN {
		unsigned int PEN : 22;
		int: 3;								// unnamed fields
		unsigned int CCA : 3;
		bool nonreachable : 1;
		boo dirty : 1;
		bool valid : 1;
		bool global : 1;
	};

A field is often called a bit-field, a member is defined to be a field by specifying the number of bits it is to occupy. Unnamed fields are allowed.

A field muse be of an integral or enumeration type. It is not possible to take the address of a field. Apart from that, however, it can be used exactly like other variables. Note that a bool field really can be represented by a single bit.

Using fields to pack several variables into a single byte does not necessarily save space, it saves data space. but the size of the code needed to manipulated these variables increases on most machines.

##Unions

a union is a struct in which all members are allocated at the same address so that the union occupies only as mush space as its largest member. Naturally, a union can hold a value for only one member at a time. 

Use of unions can be essential for compactness of data and through that for performance. However, most programs do not improve much from the use of `unions` and `unions` are rather error-prone. Consequently, I consider unions an overused feature; avoid them when you can.

###Unions and Classes

*	A union can not have virtual functions
*	A union can not have members of reference type
*	A union can not have base classes
*	If a union has a member with a user-defined constructor, a copy operation, a move operation, or a destructor, then the special function is deleted for that union; that is, it can not be used for an object of the union type.
*	At most one member of a union can have an in-class initializer
*	A union can not be used as a base class

It is possible to specify an in-class initializer for at most one member. If so, this initializer will be used for default initialization.

	union U2 {
		int a;
		const char* p {""};
	};

###Anonymous unions


