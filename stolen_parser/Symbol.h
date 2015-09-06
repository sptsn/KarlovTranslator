#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
class Symbol
{
	bool terminal;
	int index;
public:
	Symbol() : terminal(true), index(-1){}
	Symbol(bool terminal, int index) : terminal(terminal), index(index){}
	~Symbol();
	bool operator<(const Symbol& other) const
	{
		if (terminal < other.terminal)
		{
			return true;
		}
		else if (terminal > other.terminal)
		{
			return false;
		}
		if (index < other.index)
		{
			return true;
		}
		return false;
	}
	bool operator>(const Symbol& other) const
	{
		if (terminal > other.terminal)
		{
			return true;
		}
		else if (terminal < other.terminal)
		{
			return false;
		}
		if (index > other.index)
		{
			return true;
		}
		return false;
	}

	bool operator<=(const Symbol& other) const
	{
		if (terminal <= other.terminal)
		{
			return true;
		}
		if (index <= other.index)
		{
			return true;
		}
		return false;
	}

	bool operator== (const Symbol& other) const
	{
		return (terminal == other.terminal) && (index == other.index);
	}
	friend std::ostream& operator<< (std::ostream& out, Symbol symbol)
	{
		out << '(' << int(symbol.is_terminal()) << ", " << symbol.get_value() << ')';
		return out;
	}
	bool is_terminal() const{ return terminal; }
	int get_value() const{ return index; }
	bool is_empty() const{ return terminal && index == -1; }
};

typedef std::vector<Symbol> Sequence;

namespace std {

	template <>
	struct hash<Symbol>
	{
		std::size_t operator()(const Symbol& k) const
		{
			using std::size_t;
			using std::hash;
			using std::string;

			return ((hash<int>()(k.is_terminal() + 1)
				^ (hash<int>()(k.get_value()) << 1)) >> 1);
		}
	};



}

