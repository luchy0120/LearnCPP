Having more than one public section tends to be messy, though, and might affect the object layout, so does having more than one private section. However, allowing many access specifiers in a class is useful for machine-generated code.

recommend the `{}` notation for initialization because it is explicit about what is being done(initialization), avoids some potential mistakes and can be used consistently. There are cases where `()` notation must be used, but they are rare.

By default, declare a constructor that can be called with a single argument `explicit`.

If a constructor is delcared `explicit` and defined outside the class, the explicit can not be repeated:

	class Date {
		int d, m, y;
	public:
		explicit Date(int dd);
	};

	explicit Date::Date(int dd) {}	// error

Explicit can also be useful for constructor with zero or more than one argument.

	struct X {
		explicit X();
		explicit X(int, int);
	};

	X x1 = {};		// error, implicit
	X x2 = {1, 2};	// error, implicit

	X x3 {};		// OK
	X x4 {1, 2};	// OK

A member function defined within the class definition - rather than simply declared there - is taken to be an inline member function. that is, in-class definition of member functions is for small, rarely modified, frequently used functions.

A member class can refer to types and static members of its enclosing class. It can only refer to non-static members when it is given an object of the enclosing class to refer to.

Member enums are often an alternative to enum classes when it comes to avoiding polluting an enclosing scope with the names of enumerators.




