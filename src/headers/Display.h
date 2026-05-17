#pragma once

#include <vector>
#include <string>

void DisplayHandResults(Hand& player, Hand& dealer, Shoe& shoe, int& totalBalance, int& bet);
void DisplayPlayerOrDealerCards(Hand& hand, std::string name);