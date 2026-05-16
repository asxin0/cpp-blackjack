#pragma once

#include <random>

struct BlackjackTableValues {
	int table1 = 50;
	int table2 = 500;
	int table3 = 5000;
	int maxTables = 3;
};

char hitOrStandCheck();
void SplitCards(int splitCard, int dealerCard, int(&cards)[10], int(&cardsWeighted)[10], int& cardsDrawn, int& totalBalance, int& bet, std::discrete_distribution<>& dist, std::mt19937& gen);
void BlackjackGame(int& bet, int& totalBalance, int& cardsDrawn, int(&cards)[10], int(&cardsWeighted)[10]);
void BlackjackTable(int table, int& chips, int& cardsDrawn, int(&cards)[10], int(&cardsWeighted)[10]);
void BlackjackMenu();