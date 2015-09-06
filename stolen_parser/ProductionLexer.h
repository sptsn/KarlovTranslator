#pragma once
#include <unordered_set>
#include <vector>
#include "Symbol.h"
#include "Production.h"
class ProductionLexer;
struct ProductionLexerError
{
	int row;
	int col;
};
enum TransliteratorClass
{
	symbol, quote, less, more, colon, equals, whitespace, newline, end, LAST_TRANSLITERATOR_CLASS
};
struct TransliteratorToken
{
	TransliteratorClass type;
	char value;
};
enum State
{
	A1, B1, B2, C1, C2, C3, D1, E1, E2, F1, F2, G1, Z1, EXIT, LAST_STATE
};
enum SymbolClass
{
	nonterminal, terminal
};

typedef void(ProductionLexer::*lexer_method)(TransliteratorToken);
class ProductionLexer
{
	std::vector<std::string> terminal_symbols_list;
	std::vector<std::string> nonterminal_symbols_list;

	std::vector<Production> productions_list;
	std::vector<ProductionLexerError> error_list;
	int current_row;
	int current_column;
	State current_state;
	std::string string_register;
	SymbolClass class_register;
	int value_register;
	Production production_register;

	int terminal_symbols_size;
	int nonterminal_symbols_size;

	TransliteratorToken transliterator(char c);

	int find(std::vector<std::string>, std::string);

	int result;



	std::vector<std::vector<lexer_method>> fsm_table;
	//fsm methods
	void A1a(TransliteratorToken value);
	void A1b(TransliteratorToken value);
	void B1a(TransliteratorToken value);
	void B2a(TransliteratorToken value);
	void B2b(TransliteratorToken value);
	void C1a(TransliteratorToken value);
	void C2a(TransliteratorToken value);
	void C3a(TransliteratorToken value);
	void D1a(TransliteratorToken value);
	void E1a(TransliteratorToken value);
	void E2a(TransliteratorToken value);
	void E2b(TransliteratorToken value);
	void F1a(TransliteratorToken value);
	void F2a(TransliteratorToken value);
	void F2b(TransliteratorToken value);
	void G1a(TransliteratorToken value);
	int add_symbol();
	void add_production();
	void error_method(TransliteratorToken value);
	void stay(TransliteratorToken value){}
	void exit_good(TransliteratorToken value);
	void exit_bad(TransliteratorToken value);
public:
	ProductionLexer();
	~ProductionLexer();
	int terminal_size() const { return terminal_symbols_size; }
	int nonterminal_size() const { return nonterminal_symbols_size; }
	std::string get_nonterminal_symbol(int index)const{ return nonterminal_symbols_list[index]; }
	std::string get_terminal_symbol(int index)const{ return terminal_symbols_list[index]; }

	const std::vector<Production>& get_productions_list() const { return productions_list; }
	std::string productions_list_to_string() const;
	int parse_productions(std::string filename);

};

