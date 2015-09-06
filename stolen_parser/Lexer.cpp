//#include "stdafx.h"
#include "iostream"
#include <vector>
#include "fstream"
#include "list"
#include "string"
#include "map"

#include "lexer.h"


fsm_lexer::fsm_lexer() :table(last_state + 1, vector<fsm_Fun>(l_error + 1, &fsm_lexer::error_fun))// по умолчанию все заполняется ошибочными состояниями
{
	current_state = 0;
	reg_const = 0;
	reg_label = "";
	reg_varName = "";
	start_char_table = { 0, -1, -1, 1, 8, 11, 12, -1, 15, -1, -1, 21, -1, -1, 27, 28, -1, 37, -1, -1, -1, -1, 32, -1, -1, -1 };
	//----------------------------------------------------------Заполнение таблицы переходов----------------------------------------
	ident_table.push_back(ident_table_row(1, 's', 0, &fsm_lexer::finish_read_kw_as));
	ident_table.push_back(ident_table_row(2, 'o', 3, &fsm_lexer::finish_read_kw_do));
	ident_table.push_back(ident_table_row(3, 'e', 0, &fsm_lexer::reg_ident_increase));
	ident_table.push_back(ident_table_row(4, 'c', 0, &fsm_lexer::reg_ident_increase));
	ident_table.push_back(ident_table_row(5, 'l', 0, &fsm_lexer::reg_ident_increase));
	ident_table.push_back(ident_table_row(6, 'a', 0, &fsm_lexer::reg_ident_increase));
	ident_table.push_back(ident_table_row(7, 'r', 0, &fsm_lexer::reg_ident_increase));
	ident_table.push_back(ident_table_row(8, 'e', 0, &fsm_lexer::finish_read_kw_declare));
	ident_table.push_back(ident_table_row(9, 'l', 0, &fsm_lexer::reg_ident_increase));
	ident_table.push_back(ident_table_row(10, 's', 0, &fsm_lexer::reg_ident_increase));
	ident_table.push_back(ident_table_row(11, 'e', 0, &fsm_lexer::finish_read_kw_else));
	ident_table.push_back(ident_table_row(12, 'i', 0, &fsm_lexer::finish_read_kw_fi));
	ident_table.push_back(ident_table_row(13, 'o', 0, &fsm_lexer::reg_ident_increase));
	ident_table.push_back(ident_table_row(14, 't', 0, &fsm_lexer::reg_ident_increase));
	ident_table.push_back(ident_table_row(15, 'o', 0, &fsm_lexer::finish_read_kw_goto));
	ident_table.push_back(ident_table_row(16, 'f', 17, &fsm_lexer::finish_read_kw_if));
	ident_table.push_back(ident_table_row(17, 'n', 0, &fsm_lexer::reg_ident_increase));
	ident_table.push_back(ident_table_row(18, 't', 19, &fsm_lexer::finish_read_kw_int));
	ident_table.push_back(ident_table_row(19, 'p', 0, &fsm_lexer::reg_ident_increase));
	ident_table.push_back(ident_table_row(20, 'u', 0, &fsm_lexer::reg_ident_increase));
	ident_table.push_back(ident_table_row(21, 't', 0, &fsm_lexer::finish_read_kw_input));
	ident_table.push_back(ident_table_row(22, 'o', 0, &fsm_lexer::reg_ident_increase));
	ident_table.push_back(ident_table_row(23, 'n', 0, &fsm_lexer::reg_ident_increase));
	ident_table.push_back(ident_table_row(24, 'g', 0, &fsm_lexer::reg_ident_increase));
	ident_table.push_back(ident_table_row(25, 'i', 0, &fsm_lexer::reg_ident_increase));
	ident_table.push_back(ident_table_row(26, 'n', 0, &fsm_lexer::reg_ident_increase));
	ident_table.push_back(ident_table_row(27, 't', 0, &fsm_lexer::finish_read_kw_longint));
	ident_table.push_back(ident_table_row(28, 'd', 0, &fsm_lexer::finish_read_kw_od));
	ident_table.push_back(ident_table_row(29, 'r', 0, &fsm_lexer::reg_ident_increase));
	ident_table.push_back(ident_table_row(30, 'i', 0, &fsm_lexer::reg_ident_increase));
	ident_table.push_back(ident_table_row(31, 'n', 0, &fsm_lexer::reg_ident_increase));
	ident_table.push_back(ident_table_row(32, 't', 0, &fsm_lexer::finish_read_kw_print));
	ident_table.push_back(ident_table_row(33, 'h', 0, &fsm_lexer::reg_ident_increase));
	ident_table.push_back(ident_table_row(34, 'i', 0, &fsm_lexer::reg_ident_increase));
	ident_table.push_back(ident_table_row(35, 'l', 0, &fsm_lexer::reg_ident_increase));
	ident_table.push_back(ident_table_row(36, 'e', 0, &fsm_lexer::finish_read_kw_while));
	ident_table.push_back(ident_table_row(37, 'a', 0, &fsm_lexer::reg_ident_increase));
	ident_table.push_back(ident_table_row(38, 'i', 0, &fsm_lexer::reg_ident_increase));
	ident_table.push_back(ident_table_row(39, 's', 0, &fsm_lexer::reg_ident_increase));
	ident_table.push_back(ident_table_row(40, 'e', 0, &fsm_lexer::finish_read_kw_raise));

	//--------------------------------------------------------Заполнение таблицы автомата--------------------------------------------------

	table[start_state][s_letter] = &fsm_lexer::start_read_kw_fun;
	table[start_state][s_space] = &fsm_lexer::stay_in_start_state;
	table[start_state][s_endline] = &fsm_lexer::stay_in_start_state;
	table[start_state][s_semicolon] = &fsm_lexer::read_semicolons;
	table[start_state][s_ratio] = &fsm_lexer::read_ratios;
	table[start_state][s_bracket] = &fsm_lexer::read_brackets;
	table[start_state][s_operation] = &fsm_lexer::read_operation;
	table[start_state][s_digit] = &fsm_lexer::read_const;

	table[continue_read_kw_state][s_letter] = &fsm_lexer::continue_read_kw_fun;
	table[continue_read_kw_state][s_digit] = &fsm_lexer::read_var;
	table[continue_read_kw_state][s_space] = &fsm_lexer::create_var_lexeme;

	table[read_var_state][s_letter] = &fsm_lexer::read_var;
	table[read_var_state][s_digit] = &fsm_lexer::read_var;
	table[read_var_state][s_space] = &fsm_lexer::create_var_lexeme;

	table[read_const_state][s_digit] = &fsm_lexer::read_const;
	table[read_const_state][s_space] = &fsm_lexer::create_lexeme_const;


	table[one_semicolon_state][s_space] = &fsm_lexer::create_semicolon_lexeme;
	table[one_semicolon_state][s_endline] = &fsm_lexer::create_semicolon_lexeme;

	table[one_less_state][s_space] = &fsm_lexer::create_ratio_less_lexeme;
	table[one_less_state][s_ratio] = &fsm_lexer::read_two_less_ratios;

	table[two_less_state][s_letter] = &fsm_lexer::read_label;
	table[two_less_state][s_ratio] = &fsm_lexer::read_three_less_ratios;

	table[less_or_equal_state][s_space] = &fsm_lexer::create_ratio_loq_lexeme;

	table[one_equal_state][s_ratio] = &fsm_lexer::read_two_equal_ratios;
	table[one_equal_state][s_space] = &fsm_lexer::create_assignment_lexeme;

	table[finish_read_two_equals][s_space] = &fsm_lexer::create_equal_lexeme;

	table[one_greater_state][s_space] = &fsm_lexer::create_ratio_greater_lexeme;
	table[one_greater_state][s_ratio] = &fsm_lexer::read_goq;

	table[finish_read_goq_state][s_space] = &fsm_lexer::create_ratio_goq_lexeme;

	table[finish_read_operation_state][s_space] = &fsm_lexer::create_operation_lexeme;

	table[exclamation_mark_state][s_ratio] = &fsm_lexer::read_not_equal;
	table[finish_read_exclamation_mark_state][s_space] = &fsm_lexer::create_ratio_not_equal_lexeme;



	table[read_comment_state][s_letter] = &fsm_lexer::read_comment;
	table[read_comment_state][s_space] = &fsm_lexer::read_comment;
	table[read_comment_state][s_digit] = &fsm_lexer::read_comment;
	table[read_comment_state][s_operation] = &fsm_lexer::read_comment;
	table[read_comment_state][s_error] = &fsm_lexer::read_comment;
	table[read_comment_state][s_endline] = &fsm_lexer::read_comment;
	table[read_comment_state][s_ratio] = &fsm_lexer::finish1_read_comment;

	table[finish1_read_comment_state][s_ratio] = &fsm_lexer::finish2_read_comment;
	table[finish1_read_comment_state][s_letter] = &fsm_lexer::read_comment;
	table[finish1_read_comment_state][s_space] = &fsm_lexer::read_comment;
	table[finish1_read_comment_state][s_digit] = &fsm_lexer::read_comment;
	table[finish1_read_comment_state][s_error] = &fsm_lexer::read_comment;
	table[finish1_read_comment_state][s_operation] = &fsm_lexer::read_comment;

	table[finish2_read_comment_state][s_ratio] = &fsm_lexer::finish3_read_comment;
	table[finish2_read_comment_state][s_letter] = &fsm_lexer::read_comment;
	table[finish2_read_comment_state][s_space] = &fsm_lexer::read_comment;
	table[finish2_read_comment_state][s_digit] = &fsm_lexer::read_comment;
	table[finish2_read_comment_state][s_error] = &fsm_lexer::read_comment;
	table[finish2_read_comment_state][s_operation] = &fsm_lexer::read_comment;


	table[finish3_read_comment_state][s_ratio] = &fsm_lexer::finish3_read_comment;
	table[finish3_read_comment_state][s_space] = &fsm_lexer::create_comment_lexeme;
	table[finish3_read_comment_state][s_endline] = &fsm_lexer::create_comment_lexeme;

	table[read_label_state][s_letter] = &fsm_lexer::read_label;
	table[read_label_state][s_digit] = &fsm_lexer::read_label;
	table[read_label_state][s_ratio] = &fsm_lexer::finish_read_label;
	table[finish2_read_label_state][s_ratio] = &fsm_lexer::finish2_read_label;
	table[finish3_read_label_state][s_space] = &fsm_lexer::create_label_lexeme;
	table[finish3_read_label_state][s_endline] = &fsm_lexer::create_label_lexeme;


	table[finish_read_kw_as_state][s_endline] = &fsm_lexer::create_kw_as_lexeme;
	table[finish_read_kw_as_state][s_space] = &fsm_lexer::create_kw_as_lexeme;
	table[finish_read_kw_declare_state][s_endline] = &fsm_lexer::create_kw_declare_lexeme;
	table[finish_read_kw_declare_state][s_space] = &fsm_lexer::create_kw_declare_lexeme;
	table[finish_read_kw_while_state][s_endline] = &fsm_lexer::create_kw_while_lexeme;
	table[finish_read_kw_while_state][s_space] = &fsm_lexer::create_kw_while_lexeme;
	table[finish_read_kw_if_state][s_endline] = &fsm_lexer::create_kw_if_lexeme;
	table[finish_read_kw_if_state][s_space] = &fsm_lexer::create_kw_if_lexeme;
	table[finish_read_kw_else_state][s_endline] = &fsm_lexer::create_kw_else_lexeme;
	table[finish_read_kw_else_state][s_space] = &fsm_lexer::create_kw_else_lexeme;
	table[finish_read_kw_fi_state][s_endline] = &fsm_lexer::create_kw_fi_lexeme;
	table[finish_read_kw_fi_state][s_space] = &fsm_lexer::create_kw_fi_lexeme;
	table[finish_read_kw_input_state][s_endline] = &fsm_lexer::create_kw_input_lexeme;
	table[finish_read_kw_input_state][s_space] = &fsm_lexer::create_kw_input_lexeme;
	table[finish_read_kw_print_state][s_endline] = &fsm_lexer::create_kw_print_lexeme;
	table[finish_read_kw_print_state][s_space] = &fsm_lexer::create_kw_print_lexeme;
	table[finish_read_kw_goto_state][s_endline] = &fsm_lexer::create_kw_goto_lexeme;
	table[finish_read_kw_goto_state][s_space] = &fsm_lexer::create_kw_goto_lexeme;
	table[finish_read_kw_int_state][s_endline] = &fsm_lexer::create_kw_int_lexeme;
	table[finish_read_kw_int_state][s_space] = &fsm_lexer::create_kw_int_lexeme;
	table[finish_read_kw_longint_state][s_endline] = &fsm_lexer::create_kw_longint_lexeme;
	table[finish_read_kw_longint_state][s_space] = &fsm_lexer::create_kw_longint_lexeme;
	table[finish_read_kw_do_state][s_endline] = &fsm_lexer::create_kw_do_lexeme;
	table[finish_read_kw_do_state][s_space] = &fsm_lexer::create_kw_do_lexeme;
	table[finish_read_kw_od_state][s_endline] = &fsm_lexer::create_kw_od_lexeme;
	table[finish_read_kw_od_state][s_space] = &fsm_lexer::create_kw_od_lexeme;
	table[finish_read_kw_raise_state][s_space] = &fsm_lexer::create_kw_raise_lexeme;
	table[finish_read_kw_raise_state][s_endline] = &fsm_lexer::create_kw_raise_lexeme;


	table[error_state][s_endline] = &fsm_lexer::create_error_lexeme;
	table[error_state][s_space] = &fsm_lexer::create_error_lexeme;
}

fsm_lexer::~fsm_lexer(){}

void fsm_lexer::Run(string str)
{
	fstream file(str);
	char symbol;
	current_state = 0;
	while (file.get(symbol))
	{
		t = Transliterator(symbol);
		(this->*table[current_state][t.token_type])();
	}
}

void fsm_lexer::showLexemes()
{
	for (list<lexeme>::iterator it = table_of_lexemes.begin(); it != table_of_lexemes.end(); it++)
	{
		switch (it->lexeme_type)
		{
		case l_key_word:
			cout << "key_word ";
			switch (it->value)
			{
			case kw_as: cout << "as" << endl; break;
			case kw_declare: cout << "declare" << endl; break;
			case kw_while: cout << "while" << endl; break;
			case kw_if: cout << "if" << endl; break;
			case kw_else: cout << "else" << endl; break;
			case kw_fi: cout << "fi" << endl; break;
			case kw_input: cout << "input" << endl; break;
			case kw_print: cout << "print" << endl; break;
			case kw_goto: cout << "goto" << endl; break;
			case kw_int: cout << "int" << endl; break;
			case kw_longint: cout << "longint" << endl; break;
			case kw_do: cout << "do" << endl; break;
			case kw_od: cout << "od" << endl; break;
			case kw_raise: cout << "raise" << endl; break;
			}
			break;
		case l_error:
			cout << "error" << endl; break;
		case l_semicolon:
			cout << "semicolon" << endl; break;
		case l_ratio:
			cout << "ratio ";
			switch (it->value)
			{
			case 1: cout << "equal" << endl; break;
			case 2: cout << "greater" << endl; break;
			case 3: cout << "less" << endl; break;
			case 4: cout << "greater_or_equal" << endl; break;
			case 5: cout << "less_or_equal" << endl; break;
			case 6: cout << "not_equal" << endl; break;
			}
			break;
		case l_label:
			cout << "label " << it->label_name << endl; break;
		case l_comment:
			cout << "comment" << endl; break;
		case l_bracket:
			cout << "bracket ";
			if (it->value == 0) cout << "left_bracket" << endl;
			else cout << "right_bracket" << endl;
			break;
		case l_assignment:
			cout << "assignment" << endl; break;
		case l_operation:
			cout << "operation ";
			switch (it->value)
			{
			case 1: cout << "plus" << endl; break;
			case 2: cout << "minus" << endl; break;
			case 3: cout << "mult" << endl; break;
			case 4: cout << "div" << endl; break;
			case 5: cout << "modulo" << endl; break;
			}
			break;
		case l_const:
			cout << "const " << *(it->const_value) << endl; break;
		case l_variable:
			cout << "variable " << it->it << endl; break;
		}
	}
	cout << endl;
}

token fsm_lexer::Transliterator(char c)
{
	token res;
	if ('0' <= c && c <= '9')
	{
		res.token_type = s_digit;
		res.token_value = c - '0';
	}
	else if ('a' <= c && c <= 'z')
	{
		res.token_type = s_letter;
		res.token_value = c - 'a';
	}
	else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%')
	{
		res.token_type = s_operation;
		if (c == '+') res.token_value = 1;
		else if (c == '-') res.token_value = 2;
		else if (c == '*') res.token_value = 3;
		else if (c == '/') res.token_value = 4;
		else if (c == '%') res.token_value = 5;
	}
	else if (c == '<' || c == '>' || c == '=' || c == '!')
	{
		res.token_type = s_ratio;
		if (c == '=') res.token_value = 1;
		else if (c == '!') res.token_value = 2;
		else if (c == '<') res.token_value = 3;
		else if (c == '>') res.token_value = 4;
	}
	else if (c == ' ')
	{
		res.token_type = s_space;
		res.token_value = 0;
	}
	else if (c == '\n' || c == '\0')
	{
		res.token_type = s_endline;
		res.token_value = 0;
	}
	else if (c == ';')
	{
		res.token_type = s_semicolon;
		res.token_value = 0;
	}
	else if (c == '(' || c == ')')
	{
		res.token_type = s_bracket;
		if (c == '(') res.token_value = 0;
		if (c == ')') res.token_value = 1;
	}
	else
	{
		res.token_type = s_error;
		res.token_value = 0;
	}
	return res;
}


//------------------------------------Создание лексем-------------------------------------------------

void fsm_lexer::create_kw_as_lexeme()
{
	lexeme l;
	l.lexeme_type = l_key_word;
	l.value = kw_as;
	table_of_lexemes.push_back(l);
	current_state = start_state;
	reg_varName = "";
}
void fsm_lexer::create_kw_declare_lexeme()
{
	lexeme l;
	l.lexeme_type = l_key_word;
	l.value = kw_declare;
	table_of_lexemes.push_back(l);
	current_state = start_state;
	reg_varName = "";
}
void fsm_lexer::create_kw_do_lexeme()
{
	lexeme l;
	l.lexeme_type = l_key_word;
	l.value = kw_do;
	table_of_lexemes.push_back(l);
	current_state = start_state;
	reg_varName = "";
}
void fsm_lexer::create_kw_else_lexeme()
{
	lexeme l;
	l.lexeme_type = l_key_word;
	l.value = kw_else;
	table_of_lexemes.push_back(l);
	current_state = start_state;
	reg_varName = "";
}

void fsm_lexer::create_kw_raise_lexeme()
{
	lexeme l;
	l.lexeme_type = l_key_word;
	l.value = kw_raise;
	table_of_lexemes.push_back(l);
	current_state = start_state;
	reg_varName = "";
}
void fsm_lexer::create_kw_fi_lexeme()
{
	lexeme l;
	l.lexeme_type = l_key_word;
	l.value = kw_fi;
	table_of_lexemes.push_back(l);
	current_state = start_state;
	reg_varName = "";
}
void fsm_lexer::create_kw_goto_lexeme()
{
	lexeme l;
	l.lexeme_type = l_key_word;
	l.value = kw_goto;
	table_of_lexemes.push_back(l);
	current_state = start_state;
	reg_varName = "";
}
void fsm_lexer::create_kw_if_lexeme()
{
	lexeme l;
	l.lexeme_type = l_key_word;
	l.value = kw_if;
	table_of_lexemes.push_back(l);
	current_state = start_state;
	reg_varName = "";
}
void fsm_lexer::create_kw_int_lexeme()
{
	lexeme l;
	l.lexeme_type = l_key_word;
	l.value = kw_int;
	table_of_lexemes.push_back(l);
	current_state = start_state;
	reg_varName = "";
}
void fsm_lexer::create_kw_input_lexeme()
{
	lexeme l;
	l.lexeme_type = l_key_word;
	l.value = kw_input;
	table_of_lexemes.push_back(l);
	current_state = start_state;
	reg_varName = "";
}
void fsm_lexer::create_kw_longint_lexeme()
{
	lexeme l;
	l.lexeme_type = l_key_word;
	l.value = kw_longint;
	table_of_lexemes.push_back(l);
	current_state = start_state;
	reg_varName = "";
}
void fsm_lexer::create_kw_od_lexeme()
{
	lexeme l;
	l.lexeme_type = l_key_word;
	l.value = kw_od;
	table_of_lexemes.push_back(l);
	current_state = start_state;
	reg_varName = "";
}
void fsm_lexer::create_kw_print_lexeme()
{
	lexeme l;
	l.lexeme_type = l_key_word;
	l.value = kw_print;
	table_of_lexemes.push_back(l);
	current_state = start_state;
	reg_varName = "";
}
void fsm_lexer::create_kw_while_lexeme()
{
	lexeme l;
	l.lexeme_type = l_key_word;
	l.value = kw_while;
	table_of_lexemes.push_back(l);
	current_state = start_state;
	reg_varName = "";
}
void fsm_lexer::create_comment_lexeme()
{
	lexeme l;
	l.lexeme_type = l_comment;
	table_of_lexemes.push_back(l);
	current_state = start_state;
}
void fsm_lexer::create_label_lexeme()
{
	lexeme l;
	l.lexeme_type = l_label;
	l.label_name = reg_label;
	table_of_lexemes.push_back(l);
	current_state = start_state;
	reg_label = "";
}
void fsm_lexer::create_ratio_less_lexeme()
{
	lexeme l;
	l.lexeme_type = l_ratio;
	l.value = 3;
	table_of_lexemes.push_back(l);
	current_state = start_state;
}
void fsm_lexer::create_ratio_loq_lexeme()
{
	lexeme l;
	l.lexeme_type = l_ratio;
	l.value = 5;
	table_of_lexemes.push_back(l);
	current_state = start_state;
}
void fsm_lexer::create_semicolon_lexeme()
{
	lexeme l;
	l.lexeme_type = l_semicolon;
	table_of_lexemes.push_back(l);
	current_state = start_state;
}
void fsm_lexer::create_error_lexeme()
{
	lexeme l;
	l.lexeme_type = l_error;
	table_of_lexemes.push_back(l);
	current_state = start_state;
	reg_varName = "";
}
void fsm_lexer::create_equal_lexeme()
{
	lexeme l;
	l.lexeme_type = l_ratio;
	l.value = 1;
	table_of_lexemes.push_back(l);
	current_state = start_state;
}
void fsm_lexer::create_assignment_lexeme()
{
	lexeme l;
	l.lexeme_type = l_assignment;
	table_of_lexemes.push_back(l);
	current_state = start_state;
}
void fsm_lexer::create_operation_lexeme()
{
	table_of_lexemes.push_back(temp_lexeme);
	current_state = start_state;
}
void fsm_lexer::create_ratio_not_equal_lexeme()
{
	lexeme l;
	l.lexeme_type = l_ratio;
	l.value = 6;
	table_of_lexemes.push_back(l);
	current_state = start_state;
}
void fsm_lexer::create_ratio_greater_lexeme()
{
	lexeme l;
	l.lexeme_type = l_ratio;
	l.value = 2;
	table_of_lexemes.push_back(l);
	current_state = start_state;
}
void fsm_lexer::create_ratio_goq_lexeme()
{
	lexeme l;
	l.lexeme_type = l_ratio;
	l.value = 4;
	table_of_lexemes.push_back(l);
	current_state = start_state;
}
void fsm_lexer::create_lexeme_const()
{
	lexeme l;
	l.lexeme_type = l_const;
	table_of_constants.push_back(reg_const);
	l.const_value = &table_of_constants.back();
	table_of_lexemes.push_back(l);
	current_state = start_state;
	reg_const = 0;
}
void fsm_lexer::create_var_lexeme()
{
	lexeme l;
	l.lexeme_type = l_variable;
	table_of_variables.insert(make_pair(reg_varName, 0));
	l.it = reg_varName;
	table_of_lexemes.push_back(l);
	reg_varName = "";
	current_state = start_state;
}
//-----------------------------------Функции автомата--------------------------------------------------------

void fsm_lexer::stay_in_start_state()
{
	current_state = start_state;
}

void fsm_lexer::reg_ident_increase()
{
	reg_ident++;
}

void fsm_lexer::error_fun()
{
	current_state = error_state;
}

void fsm_lexer::start_read_kw_fun()
{
	reg_varName += t.token_value + 'a';
	reg_ident = start_char_table[t.token_value];
	if (reg_ident == -1) current_state = read_var_state;
	else current_state = continue_read_kw_state;
}

void fsm_lexer::continue_read_kw_fun()
{
	reg_varName += t.token_value + 'a';
	if (t.token_value == (ident_table[reg_ident].letter - 'a'))
	{
		cout << "ok" << endl;
		(this->*ident_table.at(reg_ident).fn)();
	}
	else if (ident_table[reg_ident].alt != 0)
	{
		reg_ident = ident_table[reg_ident].alt;
	}
	else current_state = read_var_state;
}

void fsm_lexer::finish_read_kw_as()
{
	current_state = finish_read_kw_as_state;
}
void fsm_lexer::finish_read_kw_declare()
{
	current_state = finish_read_kw_declare_state;
}
void fsm_lexer::finish_read_kw_while()
{
	current_state = finish_read_kw_while_state;
}
void fsm_lexer::finish_read_kw_if()
{
	current_state = finish_read_kw_if_state;
}
void fsm_lexer::finish_read_kw_fi()
{
	current_state = finish_read_kw_fi_state;
}
void fsm_lexer::finish_read_kw_input()
{
	current_state = finish_read_kw_input_state;
}
void fsm_lexer::finish_read_kw_goto()
{
	current_state = finish_read_kw_goto_state;
}
void fsm_lexer::finish_read_kw_int()
{
	current_state = finish_read_kw_int_state;
}
void fsm_lexer::finish_read_kw_longint()
{
	current_state = finish_read_kw_longint_state;
}
void fsm_lexer::finish_read_kw_do()
{
	current_state = finish_read_kw_do_state;
}
void fsm_lexer::finish_read_kw_od()
{
	current_state = finish_read_kw_od_state;
}
void fsm_lexer::finish_read_kw_print()
{
	current_state = finish_read_kw_print_state;
}
void fsm_lexer::finish_read_kw_else()
{
	current_state = finish_read_kw_else_state;
}
void fsm_lexer::finish_read_kw_raise()
{
	current_state = finish_read_kw_raise_state;
}
void fsm_lexer::read_semicolons()
{
	current_state = one_semicolon_state;
}
void fsm_lexer::read_ratios()
{
	switch (t.token_value)
	{
	case 1: current_state = one_equal_state; break;
	case 2: current_state = exclamation_mark_state; break;
	case 3:	current_state = one_less_state; break;
	case 4: current_state = one_greater_state; break;
	}
}
void fsm_lexer::read_two_less_ratios()
{
	switch (t.token_value)
	{
	case 1: current_state = less_or_equal_state; break;
	case 3: current_state = two_less_state; break;
	default: current_state = error_state; break;
	}
}
void fsm_lexer::read_label()
{
	if (t.token_type == s_digit) reg_label += t.token_value + '0';
	else reg_label += t.token_value + 'a';
	current_state = read_label_state;
}
void fsm_lexer::finish_read_label()
{
	if (t.token_value == 4) current_state = finish2_read_label_state;
	else current_state = error_state;
}
void fsm_lexer::finish2_read_label()
{
	if (t.token_value == 4) current_state = finish3_read_label_state;
	else current_state = error_state;
}
void fsm_lexer::read_three_less_ratios()
{
	if (t.token_value == 3) current_state = read_comment_state;
	else current_state = error_state;
}
void fsm_lexer::read_comment()
{
	current_state = read_comment_state;
}
void fsm_lexer::finish1_read_comment()
{
	if (t.token_value == 4) current_state = finish1_read_comment_state;
	else current_state = read_comment_state;
}
void fsm_lexer::finish2_read_comment()
{
	if (t.token_value == 4) current_state = finish2_read_comment_state;
	else current_state = read_comment_state;
}
void fsm_lexer::finish3_read_comment()
{
	if (t.token_value == 4) current_state = finish3_read_comment_state;
	else current_state = read_comment_state;
}
void fsm_lexer::read_brackets()
{
	lexeme l;
	l.lexeme_type = l_bracket;
	l.value = t.token_value;
	table_of_lexemes.push_back(l);
	current_state = start_state;
}
void fsm_lexer::read_two_equal_ratios()
{
	if (t.token_value == 1) current_state = finish_read_two_equals;
	else current_state = error_state;
}
void fsm_lexer::read_operation()
{
	temp_lexeme.lexeme_type = l_operation;
	temp_lexeme.value = t.token_value;
	current_state = finish_read_operation_state;
}
void fsm_lexer::read_not_equal()
{
	if (t.token_value == 1) current_state = finish_read_exclamation_mark_state;
	else current_state = error_state;
}
void fsm_lexer::read_goq()
{
	if (t.token_value == 1) current_state = finish_read_goq_state;
	else current_state = error_state;
}
void fsm_lexer::read_const()
{
	reg_const = reg_const * 10 + t.token_value;
	current_state = read_const_state;
}
void fsm_lexer::read_var()
{
	if (t.token_type == s_digit) reg_varName += t.token_value + '0';
	else	reg_varName += t.token_value + 'a';
	current_state = read_var_state;
}

