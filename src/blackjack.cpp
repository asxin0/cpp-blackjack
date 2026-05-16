#include "headers/Blackjack.h"
#include "headers/Cards.h"
#include "headers/Display.h"
#include "headers/System.h"

#include <iostream>
#include <string>
#include <format>
#include <random>
#include <cctype>
#include <vector>
#include <fstream>
#include <ctime>
#include <iomanip>


char hitOrStandCheck() {
	char hitOrStand;

	while (true) {
		std::cout << "\n\nHit or Stand (H/S): ";
		std::cin >> hitOrStand;

		if (std::toupper(hitOrStand) != 'H' && std::toupper(hitOrStand) != 'S') {
			std::cout << "Invalid move, please enter either H (hit) or S (stand)\n\n";
			std::cin.clear();
			std::cin.ignore(1000, '\n');
		}
		else {
			break;
		}
	}
	return std::toupper(hitOrStand);
}


void SplitCards(int splitCard, int dealerCard, int(&cards)[10], int(&cardsWeighted)[10], int& cardsDrawn, int& totalBalance, int& bet, std::discrete_distribution<>& dist, std::mt19937& gen) {
	for (int i = 1; i < 3; i++) {
		if (i == 2) {
			totalBalance -= bet;
		}
		std::vector<int> playerCards = { splitCard };
		std::vector<int> dealerCards = { dealerCard };

		std::cout << "\nHand " << i << ".\n";
		DisplayPlayerOrDealerCards(0, playerCards, "player");
		DisplayPlayerOrDealerCards(0, dealerCards, "dealer");

		hitOrStand(hitOrStandCheck(), playerCards, dealerCards, cards, cardsWeighted, cardsDrawn, dist, gen);
		DisplayHandResults(playerCards, dealerCards, cardsDrawn, totalBalance, bet);
	}
}

void BlackjackGame(int& bet, int& totalBalance, int& cardsDrawn, int(&cards)[10], int(&cardsWeighted)[10]) {

	std::vector<int> playerCards = {};
	std::vector<int> dealerCards = {};
	std::random_device rd;
	std::mt19937 gen(rd());

	int playerTotal = 0;
	int dealerTotal = 0;

	bool runOnce = true;

	std::discrete_distribution<> dist(cardsWeighted, cardsWeighted + 10);
	int i = 1;

	while (i <= 3) {
		if (i == 2) {
			BlackjackDrawCard('d', playerCards, dealerCards, cards, cardsWeighted, cardsDrawn, dist, gen);
		}
		else if (i == 1 || i == 3) {
			BlackjackDrawCard('p', playerCards, dealerCards, cards, cardsWeighted, cardsDrawn, dist, gen);
		}
		else {
		}
		i++;
	}

	CalculatePlayerOrDealerTotal(playerCards, playerTotal);
	CalculatePlayerOrDealerTotal(dealerCards, dealerTotal);

	DisplayPlayerOrDealerCards(playerTotal, playerCards, "Player");
	DisplayPlayerOrDealerCards(dealerTotal, dealerCards, "Dealer");

	if (playerCards[0] == playerCards[1]) {
		char splitChoice;
		bool splitErrorCheck = true;

		do {
			std::cout << "\n\nWould you like to split your " << playerCards[0] << "s? (y / n): ";
			std::cin >> splitChoice;

			switch (splitChoice) {
			case 'y':
				SplitCards(playerCards[0], dealerCards[0], cards, cardsWeighted, cardsDrawn, totalBalance, bet, dist, gen);
				splitErrorCheck = false;

				std::cin.clear();
				std::cin.ignore(1000, '\n');
				break;
			case 'n':
				DisplayPlayerOrDealerCards(playerTotal, playerCards, "Player");
				DisplayPlayerOrDealerCards(dealerTotal, dealerCards, "Dealer");
				splitErrorCheck = false;

				std::cin.clear();
				std::cin.ignore(1000, '\n');
				break;
			default:
				std::cout << "\nInvalid Option, (y or n).";
				std::cin.clear();
				std::cin.ignore(1000, '\n');
			} 
		} while (splitErrorCheck);
		return;
	}

	if (playerTotal == 21) {
		runOnce = false;
	}

	AceOneOrEleven(playerCards, playerTotal);

	if (playerTotal >= 21) {
		runOnce = false;
	}

	if (runOnce) {
		hitOrStand(hitOrStandCheck(), playerCards, dealerCards, cards, cardsWeighted, cardsDrawn, dist, gen);
	}

	DisplayHandResults(playerCards, dealerCards, cardsDrawn, totalBalance, bet);
}


void BlackjackTable(int table, int& chips, int& cardsDrawn, int(&cards)[10], int(&cardsWeighted)[10]) {

	int lastDailyCheck;
	while (true) {

		LoadBlackjackFile(chips, lastDailyCheck);
		BlackjackTableValues bets;
		int minBets = 0;

		switch (table) {
		case 1:
			minBets = bets.table1;
			break;
		case 2:
			minBets = bets.table2;
			break;
		case 3:
			minBets = bets.table3;
		}

		int bet = 0;
		std::cout << "\nTable " << table << "\n\n";
		std::cout << "Available balance: $" << chips << "\n";
		std::cout << "Min bet: $" << minBets << "\n\n";
		std::cout << "How much would you like to bet (Enter a negative number to leave this table): ";
		std::cin >> bet;

		if (bet < 0) {
			BlackjackMenu();
		}
		else if (bet < minBets) {
			std::cout << "Bet does not meet the minimum bet.\n";
			std::cin.clear();
			std::cin.ignore(1000, '\n');
		}
		else if (bet > chips) {
			std::cout << "Bet execeds your balance.\n";
			std::cin.clear();
			std::cin.ignore(1000, '\n');
		}
		else {
			std::cin.ignore(1000, '\n');
			chips -= bet;
			BlackjackGame(bet, chips, cardsDrawn, cards, cardsWeighted);
		}
	}
}

void BlackjackMenu() {

	int chips;
	int lastDailyCheck;
	int table;
	BlackjackTableValues blackjack;
	int requirements[3] = { 1000, 10000, 100000 };

	int cards[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	int cardsWeighted[10] = { 12, 12, 12, 12, 12, 12, 12, 12, 12, 48 };
	int cardsDrawn = 0;

	std::ifstream file("Blackjack.txt");
	if (file.is_open()) {
		LoadBlackjackFile(chips, lastDailyCheck);
		file.close();
	}
	else {
		std::ofstream file("Blackjack.txt");
		chips = 0;
		lastDailyCheck = 0;
		SaveBlackjackFile(chips, lastDailyCheck);
		file.close();
	}

	while (true) {

		std::cout << "\n\nWelcome to Blackjack - 3 deck shoe\n\n";
		DailyBlackjackReward(chips, lastDailyCheck);
		LoadBlackjackFile(chips, lastDailyCheck);
		for (int i = 1; i <= blackjack.maxTables; i++) {
			std::cout << "Table " << i << " - " << requirements[i - 1] << " chips required for entry.\n";
		}
		std::cout << "Your Chips: " << chips << "\n";
		std::cout << "\nWhich table would you like to join? (Please enter a table #): ";
		std::cin >> table;

		switch (table) {
		case 1:
			if (chips < requirements[table - 1]) {
				std::cout << "Not enough chips to enter this table.";
				std::cin.clear();
				std::cin.ignore(1000, '\n');
				break;
			}
			BlackjackTable(1, chips, cardsDrawn, cards, cardsWeighted);
			break;
		case 2:
			if (chips < requirements[table - 1]) {
				std::cout << "Not enough chips to enter this table.";
				std::cin.clear();
				std::cin.ignore(1000, '\n');
				break;
			}
			BlackjackTable(2, chips, cardsDrawn, cards, cardsWeighted);
			break;
		case 3:
			if (chips < requirements[table - 1]) {
				std::cout << "Not enough chips to enter this table.";
				std::cin.clear();
				std::cin.ignore(1000, '\n');
				break;
			}
			BlackjackTable(3, chips, cardsDrawn, cards, cardsWeighted);
			break;
		default:
			std::cout << "\nNot a valid table number";
			std::cin.clear();
			std::cin.ignore(1000, '\n');
		}
	}
}

