#include "LrItem.h"


LrItem::LrItem(Production production, Symbol lookahead, int position) :
Production(production),
lookahead(lookahead),
position(position)
{

}

LrItem::LrItem(Symbol lhs, Sequence rhs, Symbol lookahead, int position) :
Production(lhs, rhs),
lookahead(lookahead),
position(position)
{

}

LrItem::~LrItem()
{
}
