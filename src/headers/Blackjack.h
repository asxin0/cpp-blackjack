#pragma once

#include "Cards.h"
#include <random>

struct BlackjackTableValues {
	int table1 = 50;
	int table2 = 500;
	int table3 = 5000;
	int maxTables = 3;
};

char playerMoveCheck(Hand& player, int& totalBalance);
void SplitCards(int splitCard, int dealerCard, Shoe& shoe, Hand& player, Hand& dealer, int& totalBalance);
void BlackjackGame(int& totalBalance, Shoe& shoe, Hand& player, Hand& dealer);
void BlackjackTable(int table, int& chips, Shoe& shoe);
void BlackjackMenu();