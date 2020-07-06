//Includes
#include <pch.h>
#include "Expression.h"

extern exprtk::parser<double> Parser; //Global variable (defined at main.cpp)

//Constructor
Expression::Expression(const std::string& exp, const std::string& var) : 
	m_StrExpression(exp), //m_StrExpression = exp
	m_VarName(var), //m_VarName = var
	m_Compiled(false) //m_Compiled = false (when created the compilation status it's always false)
{
	Compile(); //Call the Compile function
}

//Desdructor
Expression::~Expression()
{
}

void Expression::SetExpression(const std::string & exp, const std::string & varName)
{
	m_StrExpression = exp; //Changes the m_StrExpression to the new mathematical expression
	m_VarName = varName; //Changes the m_VarName to the new variable name (Usually X)
	m_Compiled = false; //Changes the compilation status to false, because the mathematical expression needs to be recompiled

	Compile(); //Call the Compile function
}

const std::string& Expression::GetStrExpression() const
{
	return m_StrExpression; //Return the current mathematical expression in string format (m_StrExpression)
}

bool Expression::IsCompiled() const
{
	return m_Compiled; //Return the current state of the compilation (m_Compiled)
}

Expression & Expression::operator=(const Expression & rhs)
{
	//Make all member variables equals to the rhs member variables

	m_Compiled = rhs.m_Compiled;
	m_StrExpression = rhs.m_StrExpression;
	m_VarName = rhs.m_VarName;
	m_ValueX = rhs.m_ValueX;
	m_VarTable = rhs. m_VarTable;
	m_Expression = rhs.m_Expression;

	return *this; //Return a dereferenced pointer
}

std::vector<double> Expression::Calculate(const std::pair<double, double>& range, double step)
{
	//Checks if the expression is compiled
	if (m_Compiled)
	{
		unsigned int totalPoints = (unsigned int)round((abs(range.first) + abs(range.second)) / step); //Total points in the xy vector
		std::vector<double> xy; //Creation of the xy vector
		xy.reserve(2 * totalPoints); //Reservation of space

		//Calculates y point for each x in the m_Range
		for (unsigned int i = 0; i < totalPoints; i++)
		{
			m_ValueX = range.first + (i * step); //Alters the m_ValueX (passed by reference to m_VarTable)
			double y = m_Expression.value(); //Gets the Y value corresponding to the new m_ValueX

			xy.push_back(m_ValueX); //Push X
			xy.push_back(y); //Push Y
		}
		return std::move(xy); //Returns xy vector
	}
	else
	{
		std::cout << "The expression: " << m_StrExpression << " it's not compiled!!" << "\n"; //Error message
		return {}; //Returns a empty vector
	}

}

void Expression::Compile()
{
	m_Compiled = false; //Sets the m_Compiled to false for safety

	//Compile the expression mathematical expression
	m_VarTable.add_variable(m_VarName, m_ValueX); //Adds the variable name (m_VarName) and a reference to their value (m_ValueX)
	m_VarTable.add_pi(); //Adds "pi" as symbol to the compiler
	m_VarTable.add_constant("e", exprtk::details::numeric::constant::e); //Adds "e" as symbol to the compiler
	m_Expression.register_symbol_table(m_VarTable); //Register the m_VarTable

	//Compile and checks if it failed
	if (!Parser.compile(m_StrExpression, m_Expression))
	{
		std::cout << "Error: " << Parser.error().c_str() << "\n"; //Error message
	}
	else
	{
		m_Compiled = true; //Sets the compilation status (m_Compiled) to true
	}
}
