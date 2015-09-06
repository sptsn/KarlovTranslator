#include "OracleBuilder.h"
#include <algorithm>
#include <stack>
#include <queue>

OracleBuilder::OracleBuilder() : isLRgrammar(true)
{
}


OracleBuilder::~OracleBuilder()
{
}


std::set<LrItem> OracleBuilder::goto_next(std::set<LrItem> lr_itemset, Symbol symb)
{
	std::set<LrItem> new_itemset;
	for each (LrItem item in lr_itemset)
	{
		if (item.get_symbol_at_position() == symb)
		{
			LrItem new_item = item;
			new_item.shift_position();
			new_itemset.insert(new_item);
		}
	}
	return closure(new_itemset);
}

std::set<LrItem> OracleBuilder::closure(std::set<LrItem>& lr_itemset)
{
	std::queue<LrItem> lr_queue;
	for each (LrItem item in lr_itemset)
	{
		lr_queue.push(item);
	}
	while (!lr_queue.empty())
	{
		LrItem cur = lr_queue.front();
		lr_queue.pop();
		Symbol B = cur.get_symbol_at_position();
		if (!B.is_terminal())
		{
			Sequence b = cur.get_suffix();
			Symbol c = cur.get_lookahead();
			b.push_back(c);
			for each (Production production in get_production_list())
			{
				if (production.get_lhs() == B)
				{
					std::set<Symbol> first_bc = first(b);
					for each (Symbol i in first_bc)
					{
						LrItem new_lr = LrItem(production, i);
						//std::cout << new_lr << std::endl;
						if (lr_itemset.find(new_lr) == lr_itemset.end())
						{
							lr_itemset.insert(new_lr);
							lr_queue.push(new_lr);
						}
					}
				}
			}
		}
	}
	return lr_itemset;
}


std::set<Symbol> OracleBuilder::first(Sequence sequence)
{
	std::set<Symbol> result;
	if (sequence.empty())
	{
		result.insert(Symbol());
		return result;
	}
	for each (Symbol symbol in sequence)
	{
		std::set<Symbol> new_itemset = first_sets[symbol];
		result = set_union(result, new_itemset);
		if (!contains_empty(new_itemset))
		{
			remove_empty_character(new_itemset);
			break;
		}
	}
	return result;
}


void OracleBuilder::build_first_sets()
{
	for (int i = -1; i < production_lexer.terminal_size(); i++)
	{
		Symbol cur(true, i);
		std::set<Symbol> cur_set;
		cur_set.insert(cur);
		first_sets[cur] = cur_set;
	}
	for (int i = 0; i < production_lexer.nonterminal_size(); i++)
	{
		Symbol cur(false, i);
		std::set<Symbol> cur_set;
		first_sets[cur] = cur_set;
	}
	bool has_been_changed = true;
	while (has_been_changed)
	{

		has_been_changed = false;
		for each (Production production in get_production_list())
		{
			Symbol A = production.get_lhs();
			Sequence beta = production.get_rhs();
			int i = 0;
			if (!beta.empty())
			{
				Symbol B1 = beta[0];
				std::set<Symbol> cur_set = set_union(first_sets[A], remove_empty_character(first_sets[B1]));
				if (change_set(first_sets[A], cur_set))
				{
					has_been_changed = true;
				}

				for (i = 0; i < beta.size() - 1; i++)
				{
					Symbol Bi = beta[i];
					if (!contains_empty(first_sets[Bi]))
					{
						break;
					}
					Bi = beta[i + 1];
					std::set<Symbol> cur_set = set_union(first_sets[A], remove_empty_character(first_sets[Bi]));
					if (change_set(first_sets[A], cur_set))
					{
						has_been_changed = true;
					}
				}
				if (i == beta.size() - 1 && contains_empty(first_sets[beta[i]]))
				{
					std::set<Symbol> cur_set = add_empty_character(first_sets[A]);
					if (change_set(first_sets[A], cur_set))
					{
						has_been_changed = true;
					}
				}
			}
			else
			{
				std::set<Symbol> cur_set = add_empty_character(first_sets[A]);
				if (change_set(first_sets[A], cur_set))
				{
					has_been_changed = true;
				}
			}
		}
	}
}

std::set<Symbol> OracleBuilder::set_union(std::set<Symbol> first, std::set<Symbol> second)
{
	std::set<Symbol> temp_set;
	std::set_union(first.begin(), first.end(),
		second.begin(), second.end(),
		std::inserter(temp_set, temp_set.begin()));
	return temp_set;
}
std::set<Symbol> OracleBuilder::remove_empty_character(std::set<Symbol> symbols_set)
{
	std::set<Symbol> temp_set(symbols_set);
	temp_set.erase({ true, -1 });
	return temp_set;
}
std::set<Symbol> OracleBuilder::add_empty_character(std::set<Symbol> symbols_set)
{
	std::set<Symbol> temp_set(symbols_set);
	temp_set.insert({ true, -1 });
	return temp_set;

}
bool OracleBuilder::contains_empty(std::set<Symbol> symbols_set)
{
	if (symbols_set.find(Symbol()) != symbols_set.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool OracleBuilder::change_set(std::set<Symbol>& left, const std::set<Symbol>& right)
{
	if (right != left)
	{
		left = right;
		return true;
	}
	else
	{
		return false;
	}
}

std::string OracleBuilder::first_sets_to_string() const
{
	std::string str;
	for (auto first_set : first_sets)
	{
		str += "FIRST(";
		Symbol left = first_set.first;
		std::set<Symbol> right = first_set.second;
		str += symbol_to_string(left) + ") = {";

		for (auto symbol : right)
		{
			str += symbol_to_string(symbol) + ", ";
		}
		str += "}\n";
	}
	return str;
}

std::string OracleBuilder::symbol_to_string(Symbol symbol) const
{
	if (symbol.is_terminal())
	{
		if (symbol.is_empty())
		{
			return "\'$\'";
		}
		return "\'" + production_lexer.get_terminal_symbol(symbol.get_value()) + "\'";
	}
	else
		return "<" + production_lexer.get_nonterminal_symbol(symbol.get_value()) + ">";
}


void OracleBuilder::build_oracle(std::string filename)
{
	production_lexer.parse_productions(filename);
	build_first_sets();

	std::queue<std::set<LrItem>> states_to_transition;

	Production goal_production = get_production_list()[0];
	LrItem goal_item = LrItem(goal_production);
	std::set<LrItem> initial_set = { goal_item };
	std::set<LrItem> initial_state = closure(initial_set);


	states.push_back(initial_state);
	states_to_transition.push(initial_state);

	int current_state_index = 0;
	while (!states_to_transition.empty())
	{
		std::set<LrItem> current_state = states_to_transition.front();
		states_to_transition.pop();
		goto_dict.push_back(std::unordered_map<Symbol, int>());

		for (size_t i = 0; i < terminals_size(); i++)
		{
			Symbol cur_symbol = Symbol(true, i);
			std::set<LrItem> new_state = goto_next(current_state, cur_symbol);
			if (!new_state.empty())
			{
				int matched_state = match_state(new_state);
				if (matched_state == -1)
				{
					states.push_back(new_state);
					states_to_transition.push(new_state);
					goto_dict.push_back(std::unordered_map<Symbol, int>());
					goto_dict[current_state_index][cur_symbol] = states.size() - 1;
				}
				else
				{
					goto_dict[current_state_index][cur_symbol] = matched_state;
				}
			}
			else
			{
				goto_dict[current_state_index][cur_symbol] = -1;
			}
		}

		for (size_t i = 0; i < nonterminals_size(); i++)
		{
			Symbol cur_symbol = Symbol(false, i);
			std::set<LrItem> new_state = goto_next(current_state, cur_symbol);
			if (!new_state.empty())
			{
				int matched_state = match_state(new_state);
				if (matched_state == -1)
				{
					states.push_back(new_state);
					states_to_transition.push(new_state);
					goto_dict.push_back(std::unordered_map<Symbol, int>());
					goto_dict[current_state_index][cur_symbol] = states.size() - 1;
				}
				else
				{
					goto_dict[current_state_index][cur_symbol] = matched_state;
				}
			}
			else
			{
				goto_dict[current_state_index][cur_symbol] = -1;
			}
		}
		current_state_index++;
	}


	oracle = Oracle(terminals_size(), nonterminals_size(), states.size());
	for (size_t i = 0; i < states.size(); i++)
	{

		for each (auto symb in goto_dict[i])
		{
			const Symbol& cur_symbol = symb.first;
			const int& cur_goto_value = symb.second;
			if (cur_goto_value != -1)
			{
				int symbol_index = cur_symbol.get_value();
				if (cur_symbol.is_terminal())
				{
					OracleAction action = { OracleActionType::shift, cur_goto_value };
					oracle.get_sr_table()[i][symbol_index + 1] = action;
				}
				else
				{
					OracleAction action = { OracleActionType::transfer, cur_goto_value };
					oracle.get_goto_table()[i][symbol_index] = action;
				}
			}
		}

		for each (LrItem lritem in states[i])
		{
			if (lritem.get_position() == lritem.get_rhs().size())
			{
				Production to_match = lritem.to_production();
				int production_index = match_production(to_match);

				Symbol lookahead = lritem.get_lookahead();

				if (oracle.get_sr_table()[i][lookahead.get_value() + 1].is_empty())
				{
					OracleAction action = { OracleActionType::reduce, production_index };
					oracle.get_sr_table()[i][lookahead.get_value() + 1] = action;
				}
				else
				{
					std::cout << i << " " << lritem << std::endl;
					isLRgrammar = false;
					return;
				}

			}
		}
	}
}

int OracleBuilder::match_state(std::set<LrItem> state_to_match)
{
	for (size_t i = 0; i < states.size(); i++)
	{
		if (states[i] == state_to_match)
		{
			return i;
		}
	}
	return -1;
}
int OracleBuilder::match_production(const Production production_to_match) const
{
	for (size_t i = 0; i < get_production_list().size(); i++)
	{
		if (get_production_list()[i] == production_to_match)
		{
			return i;
		}
	}
	return -1;
}

std::ostream& operator<<(std::ostream& out, const OracleBuilder& oracle_builder)
{
	if (oracle_builder.isLRgrammar == false)
	{
		out << "Not an LR(1) grammar" << std::endl;
		return out;
	}
	out << oracle_builder.production_list_to_string() << std::endl;
	out << oracle_builder.first_sets_to_string() << std::endl;
	for (size_t i = 0; i < oracle_builder.states.size(); i++)
	{
		for each (auto symb in oracle_builder.goto_dict[i])
		{
			if (symb.second != -1)
			{
				out << i << "," << symb.first << " -> " << symb.second << std::endl;
			}
		}
		out << i << "= {";
		for each (LrItem lritem in oracle_builder.states[i])
		{
			out << "[" << oracle_builder.symbol_to_string(lritem.get_lhs()) << " ::= ";
			for (int i = 0; i < lritem.get_rhs().size(); i++)
			{
				if (lritem.get_position() == i)
				{
					out << '.';
				}
				out << oracle_builder.symbol_to_string(lritem.get_rhs()[i]);
			}
			if (lritem.get_position() == lritem.get_rhs().size())
			{
				out << '.';
			}
			out << "; " << oracle_builder.symbol_to_string(lritem.get_lookahead()) << "]";
		}
		out << "}" << std::endl;
	}
	out << oracle_builder.oracle << std::endl;
	out << "LR(1) grammar" << std::endl;
	return out;
}