#pragma once
//Include
#include <pch.h>

class Expression
{
public:
	Expression(const std::string& exp, const std::string & var = "x"); //Expression constructor
	~Expression(); //Expression destructor

	void SetExpression(const std::string& exp, const std::string& varName = "x"); //Change the current mathematical expression

	const std::string& GetStrExpression() const; //Return the current mathematical expression in string format (m_StrExpression)
	bool IsCompiled() const; //Return the current state of the compilation (m_Compiled)

	Expression& operator=(const Expression& rhs); // = Operator Overload

	std::vector<double> Calculate(const std::pair<double, double>& range, double step); //Return a vector of X and Y values based on the given range and step
private:
	void Compile(); //Compile the mathematical expression

	//Member variables
	bool m_Compiled; //State of the compilation

	std::string m_StrExpression, m_VarName; //Mathematical expression in string format (m_StrExpression) and the name of the variable (m_VarName) (Usually X)

	double m_ValueX; //This variable is passed by reference to the m_VarTable, so if changed the mathematical expression evaluation also changes to new value of m_ValueX

	exprtk::symbol_table<double> m_VarTable; //Table that hold the variables reference and the their names
	exprtk::expression<double> m_Expression; //Register of the m_VarTable
};