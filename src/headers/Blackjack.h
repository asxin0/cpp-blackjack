#pragma once

#include "Cards.h"
#include <random>

struct BlackjackTableValues {
	int table1 = 50;
	int table2 = 500;
	int table3 = 5000;
	int maxTables = 3;
};

char playerMoveCheck();
void SplitCards(int splitCard, int dealerCard, Shoe& shoe, Hand& player, Hand& dealer, int& totalBalance, int& bet);
void BlackjackGame(int& bet, int& totalBalance, Shoe& shoe);
void BlackjackTable(int table, int& chips, Shoe& shoe);
void BlackjackMenu();