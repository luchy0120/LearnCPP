Using a class as a base is equivalent to defining an (unnamed) object of that class. Consequently, a class must be defined in order to be used as a base:
	class Employee;		// declaration only, no definition

	class Manager : public Employee {	// error, Employee not defined
		// ...
	};

Typically, the cleanest solution is for the derived class to used only the public members of its base class.

	void Manager::print() const {
		Employee::print();
		cout << level;
	}

###Constructors and Destructors

As usual, constructors and destructors are as essential:

*	Objects are constructed from the bottom-up (base before member and member before derievd) and destroyed top-down (derived before member and member before base)
*	Each class can intialize its members and bases(but not directly members or bases or bases of its bases)
*	Typically, destructors in a hierarchy need to be virtual
*	Copy constructors of classes in a hierarchy should be used with care(if at all) to avoid slicing
*	The resolution of a virtual function call, a dynamic_cast or a typeid() in a constructor or destructor reflects the stage of construction and destruction (rather than the type of the yet-to-be-completed object)

###Type Fields

Given a pointer of type `Base*`, to which derived type does the object pointed to really belong? There are four fundamental solutions:

*	Ensure that only objects of a single type are pointed to
*	Place a type field in the base class for the functions to inspect
*	Use dynamic_cast
*	Use virtual functions

###Virtual Functions

To allow a virtual function declaration to act as an interface to functions defined in derived classes, the argument types specified for a function in a derived class can not differ from the argument types declared in the base, and only very slight changes are allowed for the return type. A virtual member function is sometimes called a method.

A virtual function can be used even if no class is derievd from its class, and a derived class that does not need its own version of a virtual function need not provide one. When deriving a class, simply provide an approviate function if it is needed.

A function from a derived class with the same name and the same set of argument types as a virtual function in a base is said to override the base class version of the virtual function. Furthermore, it is possible to override a virtual function from a base with a more derived return type.

Note that this will work even if print_list() was written and compiled before the specific derived class Manager was even conceived of!

By default, a function that overrides a virtual function itself becomes virtual. We can, but do not have to repeat `virtual` in a derived class. I do not recommend repeating virtual. If you want to be explicit, use `override`.

In a typical implementation, the space taken is just enough to hold a pointer: the usual implementation technique is for the compiler to convert the name of a virtual function into an index into a table of pointers to functions.

A virtual function invoked from a constructor or a destructor reflects that the object is partially constructed or partially destroyed. It is therefore typically a bad ideal to call a virtual function from a constructor or a destructor.

###Explicit Qualification:

The use of a qualified name has another desirable effect. That is, if a virtual function is also inline(as is not uncommon), then inline substitution can be used for calls specified using `::`, this provides the programmer with an effecient way to handle some important cases in which one virtual function calls another for the same object.

In the absence of any of these controls, a non-static member function is virtual if and only if it overrides a virtual function in a base class.

####override

	struct D: B5 {
		void f(int) const override;
	};

The override specifier comes last in a declaration, after all other parts:

	void f(int) const noexcept override;

An override specifier is not part of the type of a function and can not be repeated in an out-of-class definition.

Curiously, override is not a keyword, it is what is called a contextual keyword. That is, override has a special meaning in a few contexts but can be used as an identifier elsewhere.

The other contextual keyword is final.

####final

We can make every virtual member function of a class final; just add final after the class name.

For good and bad, adding final to the class not only prevents overriding, it also prevents further derivation from a class.

A final specifier is not part of the type if a function and can not be repeated in an out of class definition.

###using Base Members

Functions do not overload across scopes. 

	struct Base {
		void f(int);
	};

	struct Derived: Base {
		void f(double);
	};

	void use(Derived d) {
		d.f(1);		// call Derived::f(double);
		Base& br = d;
		br.f(1);	// call Base::f(1)
	};

Sometimes, we want overloading to ensure that the base matching member function is used. As for namespaces, using-declarations can be used to add a function to a scope.

	struct D2: Base {
		using Base::f;		// bring all fs from Base into D2
		void f(double);
	};

	void use2(D2 d) {
		d.f(1);		// call D2::f(int), that is, Base::f(int)
		Base& br = d;
		br.f(1);	// call Base::f(int)
	}

This is a simple consequence of a class also being consiered a namespace.

We can bring constructors into a derived class scope. A name brought into a derived class scope by a using-declaration has its access determined by the placement of the using-declaration. We can not using-directives to bring all members of a base class into a derived class.

####Inheriting Constructors

Here is a example to show that how to inherit constructors:

	template<class T>
	struct Vector: std::vector<T> {
		using vector<T>::vector;		// inherit constructors

		// ...
	};

Most often it is best to avoid being clever and restrict the use of inheriting constructors to the simple cases where no data members are added.

###Return Type Relaxation

If the original return type was `B*`, then the return type of the overriding function maybe `D*`, provided B is a public base of D. Similarly, a return type of `B&` may be relaxed to `D&`.

This relaxation applies only to return types that are pointers or references, and not to "smart pointers" such as `unique_ptr`. In particular, there is not a similar relaxation of the rules for argument types because that would lead to type violations.

##Abstract classes

A class with one or more pure virtual functions is an abstract class, and no objects of that abstract class can be created.

##Access to Base Classes

Like a member, a base class can be declared private, protected, or public.

The different access specifiers serve different design needs

*	public derivation make the derived class a subtype of its  base.
*	private bases are most useful when defining a class by restricting the interface to a base so that stronger guarantees can be provided.
*	protected bases are useful in class hierarchies in which further derivation is the norm. Like the private derivation, protected derivation is used to represent implementation details.

The access specifier for a base class can be left out. In that case, the base defaults to a private base for a classand a public base for a struct.

	class XX:B {/* */};		// B is a private base
	struct YY:B {/* */};	// B is a public base

Consider a class D derived from a base B:

*	If B is a private base, its public and protected members can be used only by member functions and friends of D. Only friends and members of D can convert a D* to a B*.
*	If B is a protected base, its public and protected members can be used only member functions and friends of D and by member functions and friends of classes derived from D. Only friends and members of D and friends and members of classes derived from D can convert a D* to a B*
*	If B is a public base, its public members can be used by any function. In addition, its protected members can bse used by members and friends of D and members and friends of classes derived from D, Any function can convert a D* to a B*.

####Multiple Inheritance and Access Control

When a using-declaration is combined with private or protected derivation, it can be used to specify interfaces to some, but not all, of the facilities usually offered by a class.

####Pointer to Member function

Because a pointer to a virtual member(s in this example) is a kind of offset, it does not depend on an object's location in memory. A pointer to a virtual member can therefore be passed between different address spaces as long as the same object layout is used in both. Like poitner to ordinary functions, pointers to non-virtual member functions can not  be exchanged between address spaces.

Note that the function invoked through the pointer to function can be virtual. 

A static member is not associated with a particalar object, so a pointer to a static member is simply an ordinary pointer.

	class Task {
		// ...
		static void schedule();
	};

	void (*p)() = &Task::schedule;		// OK
	void (Task::*pm) = &Task::schedule;	// error

###Pointers to Data Members

example:

	using Pmfi = void (C::*)(int);		// pointer to member function of C taking an int
	using Pm = const char* C::*;		// pointer to char* data member of C

###Base and Derived members

A derived class has at least the members that it inherits from its base classes. Often it has more. This implies that we can safely assign a pointer to a member of a base class to a pointer to a member of a derived class, but not the other way around. This property is often called contravariance.

