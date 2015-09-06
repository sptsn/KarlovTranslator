#pragma once
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>
enum OracleActionType
{
	shift, reduce, transfer, accept, error
};
struct OracleAction
{
	OracleActionType type;
	int value;


	bool is_empty() { if (type == OracleActionType::error) return true; else return false; }

	friend std::ostream& operator<< (std::ostream& out, OracleAction oracle_action)
	{
		out << "(" << int(oracle_action.type) << ", " << oracle_action.value << ')';
		return out;
	}
	std::string to_string()
	{
		std::stringstream ss;
		ss << *this;
		return ss.str();
	}
};

typedef std::vector<std::vector<OracleAction>> OracleTable;

class Oracle
{
public:
	static const int cell_width = 7;
	OracleTable sr_table;
	OracleTable goto_table;

	OracleTable& get_sr_table() { return sr_table; }
	OracleTable& get_goto_table()  { return goto_table; }

	Oracle();
	Oracle(int terminals, int non_terminals, int states);
	~Oracle();

	friend std::ostream& operator<< (std::ostream& out, Oracle oracle)
	{
		using namespace std;
		out << std::setw(cell_width) << " ";
		for (int i = 0; i < oracle.sr_table[0].size(); i++)
		{
			out << left << std::setw(oracle.cell_width) << i;
		}
		out << '|';
		for (int i = 0; i < oracle.goto_table[0].size(); i++)
		{
			out << left << std::setw(oracle.cell_width) << i;
		}
		out << std::endl;

		for (size_t i = 0; i < oracle.get_sr_table().size(); i++)
		{
			out << left << std::setw(oracle.cell_width) << setfill(' ') << i;
			for (int j = 0; j < oracle.sr_table[0].size(); j++)
			{
				out << left << std::setw(oracle.cell_width) << setfill(' ') << oracle.sr_table[i][j].to_string();
			}
			out << '|';
			for (int j = 0; j < oracle.goto_table[0].size(); j++)
			{
				out << left << std::setw(oracle.cell_width) << oracle.goto_table[i][j].to_string();
			}
			out << std::endl;
		}
		return out;
	}
};

