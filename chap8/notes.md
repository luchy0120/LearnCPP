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


