##Introduction

Templates offer:

*	The ability to pass types(as well as values and templates) as arguments without loss of information. This implies excellent opportunities for inlining, of which current implementations take great advantage.
*	Delayed type checking(done at instantiation time). This implies opportunities to weave together information from different contects.
*	The ability to pass constant values as arguments. This implies the ability to do compile-time computation.

In the context of C++, "generic programming" implies an emphasis on the design of general algorithms implemented using templates.

Focusing more on generative techniques(seeing templates as types and function generators) and relying on type functions to express compile-time computation are called template metaprogramming.

##Algorithms and Lifting

A function template is often called an algorithm.

Lifting is a skill that requires knowledge of an application domain and some experience. The most important single guide for designing algorithms is to lift them from concrete examples without adding features(notation or run-time cost) that would impair their use. 

##Concepts

The central concept for "ordinary types" is regular. A regular type is a type that:

*	you can copy(using assignment or initialization) with the proper copy semantics
*	you can default construct
*	does not have problems with various minor technical requirements(such as taking the address of a variable)
*	you can compare for equality(using `==` and `!=`)

###Concepts and Constraints

Concepts is a very general idea that does not inherently have anything to do with templates.

If I can write out a meaningful semantic specification, I have a concept, If not, what I have is a constraint that may be useful but should not be expected to be stable or widely used.

##Making Concepts Concrete

A concept is a predicate; that is, we think of a concept as a compile-time function that looks at a set of template argumetns and returns true if they meet the concept's requirements and false if they don't. So, we implement a concept as a constexpt function.

Checking concepts through constraints-check templates has obvious weaknesses:

*	Constraints checks are placed in definitions, but they really belong in declarations. That is, a concept is part of the interface to an abstraction, but a constraints check can be used only in implementation.
*	The checking of constraints occurs as part of the instantiation of the constraints-check template.Therefore, the checking may occur later than we would like. In particular, we would have prefered for a constraints check to be guaranteed to be done by the compiler at the point of the first call, but that is impossible without language changes.
*	We can forget to insert a constraints check(especially for a function template)
*	The compiler does not check that a template implementation uses only the properties specified in its concepts. Thus, a template implementation may pass the constraints check, yet still fail to type check.
*	We do not specify semantic properties in a way that a compiler can understand.

###Axioms

As in mathematics, an axiom is something we can not prove. It is something we assume to be true.

###Multi-argument Concepts

Multi-argument concepts are particularly common and useful when specifying generic algorithms. 

###Value Concepts

Compared to the fundamental concepts for types, value concepts tend to be small and ad hoc.

###Template Definition Checking

Concepts provide a strong guideline: the implementation should use no property of an argument that is not specified by the concepts, so we should test the implementation with arguments that provide the properties specified by the implementation concepts, and only those.




