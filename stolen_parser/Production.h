#pragma once
#include "Symbol.h"
#include <vector>
#include <iostream>
class Production
{
protected:
	Symbol lhs;
	Sequence rhs;
public:
	Production();
	Production(Symbol lhs, Sequence rhs) :lhs(lhs), rhs(rhs){}
	~Production();

	void set_lhs(Symbol symb){ lhs = symb; }
	void append_rhs(Symbol symb){ rhs.push_back(symb); }
	Symbol get_lhs() const{ return lhs; }
	const Sequence& get_rhs() const{ return rhs; }
	friend std::ostream& operator<< (std::ostream& out, Production production)
	{
		out << production.get_lhs() << " ::= ";
		for each (Symbol symbol in production.get_rhs())
		{
			out << symbol;
		}
		return out;
	}
	bool operator==(const Production& production) const
	{
		return production.lhs == lhs && rhs == production.rhs;
	}
	void clear(){ rhs.clear(); }

};

