#pragma once

#include "Cards.h"

#include <vector>
#include <string>

void DisplayHandResults(Hand& player, Hand& dealer, Shoe& shoe, int& totalBalance);
void DisplayBalanceResults(Shoe& shoe, Hand& player, int& totalBalance);
void DisplayPlayerOrDealerCards(Hand& hand, std::string name);