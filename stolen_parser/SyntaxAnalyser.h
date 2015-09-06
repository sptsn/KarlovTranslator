#pragma once
#include "OracleBuilder.h"
#include "lexer.h"

#include <stack>
#include <string>
#include <vector>
#include <map>

using namespace std;

enum nonterminals
{
	S, program, construct, label, multideclare, expression, test, elseoperator, expression1, expression2
};

enum terminals
{
	_$, _label, _semicolon, _declare, _ident, _eq, _while, _do, _od, _if, _lbr, _rbr, _fi, _input, _print, _goto, _comment, _raise, _else,
	_as, _type, _ratio, _3prior, _2prior, _const, _1prior
};

struct rule
{
	int nonterminal;
	int number;
	rule(int s, int n)
	{
		nonterminal = s;
		number = n;
	}
};

class Synt
{
	list<lexeme>::iterator it;
	vector<rule> rules;
	
	int syntTranslit(lexeme l)
	{

		switch (l.lexeme_type)
		{
		//case l_raise:
		case l_key_word:
			switch (l.value)
			{
			case kw_raise: return _raise; break;
			case kw_declare: return _declare; break;
			case kw_as: return _as; break;
			case kw_do: return _do; break;
			case kw_else: return _else; break;
			case kw_fi: return _fi; break;
			case kw_goto: return _goto; break;
			case kw_if: return _if; break;
			case kw_input: return _input; break;
			case kw_int: return _type; break;
			case kw_longint: return _type; break;
			case kw_od: return _od; break;
			case kw_print: return _print; break;
			case kw_while: return _while; break;
			}
			break;
		case l_ratio: return _ratio; break;
		case l_label: return _label; break;
		case l_assignment: return _eq; break;
		case l_comment: return _comment; break;
		case l_semicolon: return _semicolon; break;
		case 228: return _$; break;
		case l_variable: return _ident; break;
		case l_const: return _const; break;
		case l_bracket:
			if (l.value == 0) return _lbr;
			else return _rbr;
			break;
		case l_operation:
			switch (l.value)
			{
			case 1: return _3prior; break;
			case 2: return _3prior; break;
			case 3: return _2prior; break;
			case 4: return _2prior; break;
			case 5: return _2prior; break;
			}
			break;
		}
	}

	void shift(int n)
	{
		actions.push(n);
		symbols.push(syntTranslit(*it));
		it++;
	}
	void reduce(int n)
	{
		if (n == 0)
		{
			isAccepted = 1;
			return;
		}
		if (rules[n].number >= actions.size() || rules[n].number >= symbols.size())
		{
			isAccepted = 0;
			return;
		}
		for (int i = 0; i < rules[n].number; i++)
		{
			actions.pop();
			symbols.pop();
		}
		symbols.push(rules[n].nonterminal);
		if (builder.oracle.goto_table[actions.top()][symbols.top()].type == 2)
			actions.push(builder.oracle.goto_table[actions.top()][symbols.top()].value);
		//else if (builder.oracle.goto_table[actions.top()][symbols.top()].type == 4) isAccepted = 0;
	}
	int isAccepted;
public:
	OracleBuilder builder;
	fsm_lexer lexer;

	Synt();
	Synt(string programfile, string grammar) : lexer(), builder()
	{
		//nonterminals = { "S", "program", "construct", "label", "multideclare", "expression", "test", "elseoperator", "expression1", "expression2" };
		rules.push_back(rule(S, 1));
		rules.push_back(rule(program, 1));
		rules.push_back(rule(program, 2));
		rules.push_back(rule(label, 0));
		rules.push_back(rule(label, 1));
		rules.push_back(rule(construct, 1));
		rules.push_back(rule(construct, 4));
		rules.push_back(rule(construct, 5));
		rules.push_back(rule(construct, 7));
		rules.push_back(rule(construct, 9));
		rules.push_back(rule(construct, 4));
		rules.push_back(rule(construct, 4));
		rules.push_back(rule(construct, 4));
		rules.push_back(rule(construct, 2));
		rules.push_back(rule(construct, 2));
		rules.push_back(rule(elseoperator, 0));
		rules.push_back(rule(elseoperator, 2));
		rules.push_back(rule(multideclare, 3));
		rules.push_back(rule(multideclare, 4));
		rules.push_back(rule(test, 3));
		rules.push_back(rule(expression, 3));
		rules.push_back(rule(expression, 1));
		rules.push_back(rule(expression1, 3));
		rules.push_back(rule(expression1, 1));
		rules.push_back(rule(expression2, 3));
		rules.push_back(rule(expression2, 1));
		rules.push_back(rule(expression2, 1));
		rules.push_back(rule(expression2, 4));

		lexer.Run(programfile);
		builder.build_oracle(grammar);
	}
	void Run()
	{
		isAccepted = 2;
		actions.push(0);
		symbols.push(0);
		lexeme l;
		l.lexeme_type = 228;
		lexer.table_of_lexemes.push_back(l);//концевой маркер - $
		it = lexer.table_of_lexemes.begin();
		while (isAccepted == 2)//!(actions.top() == 0 && symbols.top() == )
		{
			cout << actions.top() << " " << symbols.top() << " (" << builder.oracle.sr_table[actions.top()][syntTranslit(*it)].type << ", " << builder.oracle.sr_table[actions.top()][syntTranslit(*it)].value << ")" << endl;
	/*		if (it->lexeme_type == l_error)
			{
				it++;
				continue;
			}*/
			switch (builder.oracle.sr_table[actions.top()][syntTranslit(*it)].type)
			{
			case 0:
				shift(builder.oracle.sr_table[actions.top()][syntTranslit(*it)].value);
				break;
			case 1:
				reduce(builder.oracle.sr_table[actions.top()][syntTranslit(*it)].value);
				break;
			case 2:
				break;
			case 3:
				isAccepted = 1;
				break;
			case 4:
				isAccepted = 0;
				break;
			}
		}
		if (isAccepted == 1) cout << "Accept" << endl;
		else cout << "Reject" << endl;
	}
	stack <int> actions;
	stack <int> symbols;
};