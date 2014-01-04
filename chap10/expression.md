In C++, an assignment is an expression, a function-call is an expression, the construction of an object is an expression, and so are many other operations that go beyond conventional arithmetic expression evaluation.

##Case Study: A Desk Calculator

*Requirement*: Provides the four standard arithmetic operations as infix operators on floating-point numbers. The user can also define variables.

The calculator consists of four main parts: a parser(syntactic analysis), an input function(handles input and lexical analysis), a symbol table(holds permanent information), and a driver(handles initialization, output, and errors).

###The parser

	program:
		end
		expr_list end

	expr_list:
		expression print
		expression print expr_list

	expression:
		expression + term
		expression - term
		term
	
	term:
		term / primary
		term * primary
		primary

	primary:
		number
		name
		name = expression
		- primary
		(expression)

The syntax analysis called *recursive descent*. 


