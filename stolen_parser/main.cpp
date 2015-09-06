#include <iostream>
#include "Production.h"
#include "LrItem.h"
#include "Oracle.h"
#include "ProductionLexer.h"
#include "OracleBuilder.h"
#include "SyntaxAnalyser.h"
#include "Lexer.h"
#include "Integer.h"
#include <string>
#include <set>

int main()
{
	/*OracleBuilder oracle_builder;
	oracle_builder.parse_productions("data.txt");
	std::cout << oracle_builder.production_list_to_string() << std::endl;
	oracle_builder.build_first_sets();
	std::cout << oracle_builder.first_sets_to_string() << std::endl;


	Production goal_production = oracle_builder.get_production_list()[0];
	LrItem goal_item = LrItem(goal_production);
	std::set<LrItem> initial_set = { goal_item };
	std::set<LrItem> initial_state = oracle_builder.closure(initial_set);

	for each (LrItem lritem in initial_state)
	{
	std::cout << lritem << ", ";
	}*/

	Synt analyser("program.txt", "data.txt");
	//std::cout << analyser.builder;
	//ofstream file("out.txt");
	//file << analyser.builder << endl;
	//file.close();
	analyser.Run();
	
	analyser.lexer.showLexemes();

	/*OracleBuilder oracle_builder2;
	oracle_builder2.build_oracle("data.txt");
	fsm_lexer lexer;
	lexer.Run("program.txt");
	lexer.showLexemes();*/
	
	//std::cout << oracle_builder2 << std::endl;

	/*OracleBuilder oracle_builder;
	oracle_builder.build_oracle("data.txt");
	std::cout << oracle_builder;*/
	return 0;
}