#pragma once
//#include "stdafx.h"
#include "iostream"
#include <vector>
#include "fstream"
#include "list"
#include "string"
#include "map"

#include "integer.h"

using namespace std;

class fsm_lexer;
typedef void(fsm_lexer::*fsm_Fun)();

enum symbols { s_letter, s_digit, s_operation, s_ratio, s_space, s_semicolon, s_endline, s_bracket, s_error };
enum key_words { kw_declare, kw_as, kw_while, kw_if, kw_else, kw_fi, kw_input, kw_print, kw_goto, kw_int, kw_longint, kw_do, kw_od, kw_raise };
enum states {
	start_state, continue_read_kw_state, error_state, finish_read_kw_as_state, finish_read_kw_declare_state, finish_read_kw_while_state,
	finish_read_kw_if_state, finish_read_kw_fi_state, finish_read_kw_print_state, finish_read_kw_input_state, finish_read_kw_goto_state,
	finish_read_kw_int_state, finish_read_kw_longint_state, finish_read_kw_do_state, finish_read_kw_od_state, finish_read_kw_else_state,
	one_semicolon_state, one_less_state, two_less_state, less_or_equal_state, read_label_state, finish2_read_label_state, finish3_read_label_state,
	read_comment_state, finish1_read_comment_state, finish2_read_comment_state, finish3_read_comment_state, one_equal_state,
	finish_read_two_equals, finish_read_operation_state, exclamation_mark_state, finish_read_exclamation_mark_state,
	one_greater_state, finish_read_goq_state, finish_read_kw_raise_state, read_const_state, read_var_state, last_state
};
enum lexemes { l_key_word, l_semicolon, l_bracket, l_operation, l_ratio, l_const, l_variable, l_label, l_comment, l_error, l_assignment };

struct token
{
	int token_type;
	int token_value;
};

struct constant
{
	int int_value;
	longint longint_value;
};

struct lexeme
{
	int lexeme_type;
	int value;
	string it; //для переменных
	string label_name;//для меток
	int * const_value;//для констант

	lexeme() : label_name("")
	{
		lexeme_type = 0;
		value = 0;
		//it = 0;
		const_value = 0;
	}
};

struct ident_table_row
{
	int number;
	char letter;
	int alt;
	fsm_Fun fn;
	ident_table_row(int n, char c, int a, fsm_Fun f)
	{
		number = n;
		letter = c;
		alt = a;
		fn = f;
	}
};

class fsm_lexer
{
	int current_state;
	vector<vector<fsm_Fun>> table;
	token t;
	lexeme temp_lexeme;
	int reg_const;
	string reg_varName;
	int reg_ident;
	string reg_label;

	vector<int> start_char_table;//начальный вектор
	vector<ident_table_row> ident_table;//вектор переходов

	
	//--------------------------funtions------------------------
	void create_error_lexeme();
	void error_fun();
	void stay_in_start_state();
	void start_read_kw_fun();
	void continue_read_kw_fun();
	void reg_ident_increase();
	void read_semicolons();
	void read_ratios();
	void read_two_less_ratios();
	void read_label();
	void finish_read_label();
	void finish2_read_label();
	void read_three_less_ratios();
	void read_comment();
	void finish1_read_comment();
	void finish2_read_comment();
	void finish3_read_comment();
	void read_brackets();
	void read_two_equal_ratios();
	void create_equal_lexeme();
	void create_assignment_lexeme();
	void read_operation();
	void create_operation_lexeme();
	void read_not_equal();
	void create_ratio_greater_lexeme();
	void read_goq();
	void read_const();
	void create_lexeme_const();
	void read_var();


	void finish_read_kw_as();
	void finish_read_kw_declare();
	void finish_read_kw_while();
	void finish_read_kw_if();
	void finish_read_kw_else();
	void finish_read_kw_fi();
	void finish_read_kw_input();
	void finish_read_kw_print();
	void finish_read_kw_goto();
	void finish_read_kw_int();
	void finish_read_kw_longint();
	void finish_read_kw_do();
	void finish_read_kw_od();
	void finish_read_kw_raise();

	void create_kw_as_lexeme();
	void create_kw_declare_lexeme();
	void create_kw_do_lexeme();
	void create_kw_else_lexeme();
	void create_kw_fi_lexeme();
	void create_kw_goto_lexeme();
	void create_kw_if_lexeme();
	void create_kw_int_lexeme();
	void create_kw_input_lexeme();
	void create_kw_longint_lexeme();
	void create_kw_od_lexeme();
	void create_kw_print_lexeme();
	void create_kw_while_lexeme();
	void create_kw_raise_lexeme();

	void create_ratio_goq_lexeme();
	void create_comment_lexeme();
	void create_ratio_loq_lexeme();
	void create_ratio_less_lexeme();
	void create_semicolon_lexeme();
	void create_label_lexeme();
	void create_ratio_not_equal_lexeme();
	void create_var_lexeme();

public:
	fsm_lexer();
	~fsm_lexer();
	token Transliterator(char c);
	void Run(string str);
	void showLexemes();
	map <string, int> table_of_variables;
	list<int> table_of_constants;
	list<lexeme> table_of_lexemes;
};



