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
			std::cout << "\nInvalid move, please enter either H (hit) or S (stand)";
			std::cin.clear();
			std::cin.ignore(1000, '\n');
		}
		else {
			break;
		}
	}
	return std::toupper(hitOrStand);
}

void SplitCards(int splitCard, int dealerCard, Shoe& shoe, Hand& player, Hand& dealer, int& totalBalance, int& bet) {
	dealer.cards = { dealerCard };
	std::vector<Hand> playerHands = {};

	for (int i = 1; i < 3; i++) {
		if (i == 2) {
			totalBalance -= bet;
		}
		player.cards = { splitCard };

		std::cout << "\nHand " << i << ".\n";
		BlackjackDrawCard(player, shoe);

		DisplayPlayerOrDealerCards(player, "player");
		DisplayPlayerOrDealerCards(dealer, "dealer");

		if (hitOrStandCheck() == 'H') {
			playerHit(player, dealer, shoe);
		}

		CalculatePlayerOrDealerTotal(player);
		playerHands.push_back(player);
	}
	
	std::cout << "\nDealer Draws:\n";
	playerStand(player, dealer, shoe);

	std::cout << "\n";

	for (int i = 0; i < 2; i++) {
		player = playerHands[i];

		std::cout << "\nHand " << i + 1 << " (Your Total: " << player.total << ")";

		DisplayHandResults(player, dealer, shoe, totalBalance, bet);
		DisplayBalanceResults(shoe, totalBalance, bet);
	}
}

void BlackjackGame(int& bet, int& totalBalance, Shoe& shoe) {

	Hand player;
	Hand dealer;

	bool runOnce = true;

	int i = 1;

	while (i <= 3) {
		if (i == 2) {
			BlackjackDrawCard(dealer, shoe);
		}
		else if (i == 1 || i == 3) {
			BlackjackDrawCard(player, shoe);
		}
		else {
		}
		i++;
	}

	CalculatePlayerOrDealerTotal(player);
	CalculatePlayerOrDealerTotal(dealer);

	DisplayPlayerOrDealerCards(player, "Player");
	DisplayPlayerOrDealerCards(dealer, "Dealer");

	if (player.cards[0] == player.cards[1]) {
		char splitChoice;
		bool splitErrorCheck = true;

		do {
			std::cout << "\n\nWould you like to split your " << player.cards[0] << "s? (y / n): ";
			std::cin >> splitChoice;

			switch (splitChoice) {
			case 'y':
				if (totalBalance < bet * 2) {
					std::cout << "You don't have enough chips to split.";
					splitErrorCheck = false;
					break;
				}

				SplitCards(player.cards[0], dealer.cards[0], shoe, player, dealer, totalBalance, bet);
				splitErrorCheck = false;

				std::cin.clear();
				std::cin.ignore(1000, '\n');
				return;
			case 'n':
				DisplayPlayerOrDealerCards(player, "Player");
				DisplayPlayerOrDealerCards(dealer, "Dealer");
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
	}

	if (player.total == 21) {
		runOnce = false;
	}

	AceOneOrEleven(player);

	if (player.total >= 21) {
		runOnce = false;
	}

	if (runOnce) {
		if (hitOrStandCheck() == 'H') {
			playerHit(player, dealer, shoe);
		}

		if (player.total <= 21) {
			playerStand(player, dealer, shoe);
		}
	}

	DisplayHandResults(player, dealer, shoe, totalBalance, bet);
	DisplayBalanceResults(shoe, totalBalance, bet);
}

void BlackjackTable(int table, int& chips, Shoe& shoe) {

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
			return;
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
			BlackjackGame(bet, chips, shoe);
		}
	}
}

void BlackjackMenu() {
	while (true) {

		int chips;
		int lastDailyCheck;
		int table;
		BlackjackTableValues blackjack;
		int requirements[3] = { 1000, 10000, 100000 };

		Shoe shoe;

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
			BlackjackTable(1, chips, shoe);
			break;
		case 2:
			if (chips < requirements[table - 1]) {
				std::cout << "Not enough chips to enter this table.";
				std::cin.clear();
				std::cin.ignore(1000, '\n');
				break;
			}
			BlackjackTable(2, chips, shoe);
			break;
		case 3:
			if (chips < requirements[table - 1]) {
				std::cout << "Not enough chips to enter this table.";
				std::cin.clear();
				std::cin.ignore(1000, '\n');
				break;
			}
			BlackjackTable(3, chips, shoe);
			break;
		default:
			std::cout << "\nNot a valid table number";
			std::cin.clear();
			std::cin.ignore(1000, '\n');
		}
	}
}

