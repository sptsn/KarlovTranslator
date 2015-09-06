#include "Oracle.h"


Oracle::Oracle()
{
}

Oracle::Oracle(int terminals, int non_terminals, int states) :
sr_table(states, std::vector<OracleAction>(terminals + 1, { OracleActionType::error, 0 })),
goto_table(states, std::vector<OracleAction>(non_terminals, { OracleActionType::error, 0 }))
{

}

Oracle::~Oracle()
{
}
