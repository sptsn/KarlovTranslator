#pragma once
#include "ProductionLexer.h"
#include "LrItem.h"
#include "Oracle.h"
#include "Symbol.h"
#include <set>
class OracleBuilder
{
public:
	std::unordered_map<Symbol, std::set<Symbol>> first_sets;
	ProductionLexer production_lexer;
	Oracle oracle;
	std::vector<std::set<LrItem>> states;
	std::vector<std::unordered_map<Symbol, int>> goto_dict;

	//void build_first_sets();
	std::set<LrItem> goto_next(std::set<LrItem> lr_itemset, Symbol symb);
	std::set<LrItem> closure(std::set<LrItem>& lr_itemset);
	std::set<Symbol> first(Sequence sequence);


	std::set<Symbol> set_union(std::set<Symbol> first, std::set<Symbol> second);
	std::set<Symbol> remove_empty_character(std::set<Symbol> symbols_set);
	std::set<Symbol> add_empty_character(std::set<Symbol> symbols_set);
	bool contains_empty(std::set<Symbol> symbols_set);
	bool change_set(std::set<Symbol>& left, const std::set<Symbol>& right);

	int match_state(std::set<LrItem> state_to_match);
	int match_production(const Production production_to_match) const;

	OracleBuilder();
	~OracleBuilder();

	int parse_productions(std::string filename){ return production_lexer.parse_productions(filename); }


	void build_oracle(std::string);
	const Oracle& get_oracle() const;
	const std::vector<Production>& get_production_list() const{ return production_lexer.get_productions_list(); }
	const std::vector<std::string>& get_terminals() const;
	const std::vector<std::string>& get_nonterminals() const;

	size_t terminals_size() const { return production_lexer.terminal_size(); }
	size_t nonterminals_size() const { return production_lexer.nonterminal_size(); }

	std::string production_list_to_string() const{ return production_lexer.productions_list_to_string(); }
	std::string first_sets_to_string() const;
	std::string states_to_string() const;
	std::string symbol_to_string(Symbol symbol) const;

	friend std::ostream& operator<<(std::ostream& out, const OracleBuilder& oracle_builder);

	bool isLRgrammar;

	//degunning
	void build_first_sets();
};

