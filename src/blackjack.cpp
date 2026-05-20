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
#include <chrono>
#include <thread>

char playerMoveCheck(Hand& player, int& totalBalance) {
	char playerMove;

	while (true) {
		std::cout << "\n\nHit - Stand - Double (H / S / D): ";
		std::cin >> playerMove;

		if (std::toupper(playerMove) != 'H' && std::toupper(playerMove) != 'S' && std::toupper(playerMove) != 'D') {
			std::cout << "\nInvalid move, please enter either H (hit), S (stand) or D (double)";

			std::cin.clear();
			std::cin.ignore(1000, '\n');
		}
		else {
			if (std::toupper(playerMove) == 'D' && totalBalance < player.bet) {
				std::cout << "Not enough chips to double.";

				std::cin.clear();
				std::cin.ignore(1000, '\n');
			}
			else {
				break;
			}
		}
	}
	return std::toupper(playerMove);
}

void SplitCards(int splitCard, int dealerCard, Shoe& shoe, Hand& player, Hand& dealer, int& totalBalance) {
	std::vector<Hand> playerHands = {};
	int baseBet = player.bet;

	for (int i = 1; i < 3; i++) {
		player.cards = { splitCard };
		player.isDoubled = false;

		player.bet = baseBet;

		std::cout << "\nHand " << i << ".\n";
		BlackjackDrawCard(player, shoe);

		DisplayPlayerOrDealerCards(player, "player", false);
		DisplayPlayerOrDealerCards(dealer, "dealer", true);

		char playerMove = playerMoveCheck(player, totalBalance);

		if (playerMove == 'H') {
			playerHit(player, dealer, shoe, totalBalance);
		}
		else if (playerMove == 'D') {
			std::cout << "\n- " << player.bet << " chips.\n";

			playerDouble(player, dealer, shoe, totalBalance);
			player.isDoubled = true;

			totalBalance -= player.bet;
			player.bet *= 2;

			std::cout << "\n";
		} 

		CalculatePlayerOrDealerTotal(player);
		playerHands.push_back(player);
	}

	playerStand(player, dealer, shoe);

	std::cout << "\n";

	for (int i = 0; i < 2; i++) {
		player = playerHands[i];

		std::cout << "\nHand " << i + 1 << " (Your Total: " << player.total << ")";

		DisplayHandResults(player, dealer, shoe, totalBalance);
		DisplayBalanceResults(shoe, player, totalBalance);
	}
}

void BlackjackGame(int& totalBalance, Shoe& shoe, Hand& player, Hand& dealer) {
	bool runOnce = true;

	for (int i = 1; i < 3; i++) {
		BlackjackDrawCard(player, shoe);
		BlackjackDrawCard(dealer, shoe);
	}

	CalculatePlayerOrDealerTotal(player);
	CalculatePlayerOrDealerTotal(dealer);

	DisplayPlayerOrDealerCards(player, "Player", false);
	DisplayPlayerOrDealerCards(dealer, "Dealer", true);

	if (player.cards[0] == player.cards[1]) {
		char splitChoice;
		bool splitErrorCheck = true;

		do {
			std::cout << "\n\nWould you like to split your " << player.cards[0] << "s? (y / n): ";
			std::cin >> splitChoice;

			switch (std::tolower(splitChoice)) {
			case 'y':
				if (totalBalance < player.bet) {
					std::cout << "You don't have enough chips to split.";
					splitErrorCheck = false;

					break;
				}

				totalBalance -= player.bet;
				std::cout << "\n- " << player.bet << " chips.\n";
				SplitCards(player.cards[0], dealer.cards[0], shoe, player, dealer, totalBalance);
				splitErrorCheck = false;

				std::cin.clear();
				std::cin.ignore(1000, '\n');

				return;
			case 'n':
				DisplayPlayerOrDealerCards(player, "Player", false);
				DisplayPlayerOrDealerCards(dealer, "Dealer", true);

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
		char playerMove = playerMoveCheck(player, totalBalance);

		if (playerMove == 'H') {
			playerHit(player, dealer, shoe, totalBalance);
		}
		else if (playerMove == 'D' && totalBalance >= player.bet) {
			std::cout << "\n\n- " << player.bet << " chips.\n";
			playerDouble(player, dealer, shoe, totalBalance);
			AceOneOrEleven(player);

			totalBalance -= player.bet;
			player.bet *= 2;
		}

		if (player.total <= 21) {
			playerStand(player, dealer, shoe);
		}
	}

	DisplayHandResults(player, dealer, shoe, totalBalance);
	DisplayBalanceResults(shoe, player, totalBalance);
}

void BlackjackTable(int table, int& chips, Shoe& shoe) {
	int lastDailyCheck;

	while (true) {
		Hand player;
		Hand dealer;

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

		std::cout << "\nTable " << table << "\n\n";
		std::cout << "Available balance: $" << chips << "\n";
		std::cout << "Min bet: $" << minBets << "\n\n";
		std::cout << "How much would you like to bet (Enter a negative number to leave this table): ";
		std::cin >> player.bet;

		if (player.bet < 0) {
			return;
		}
		else if (player.bet < minBets) {
			std::cout << "Bet does not meet the minimum bet.\n";
			std::cin.clear();
			std::cin.ignore(1000, '\n');
		}
		else if (player.bet > chips) {
			std::cout << "Bet execeds your balance.\n";
			std::cin.clear();
			std::cin.ignore(1000, '\n');
		}
		else {
			std::cin.ignore(1000, '\n');
			chips -= player.bet;
			BlackjackGame(chips, shoe, player, dealer);
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

		std::cout << "\n\n+------------------------------------------+\n";
		std::cout << "|          BLACKJACK - 3 DECK SHOE         |\n";
		std::cout << "+------------------------------------------+\n\n";
		DailyBlackjackReward(chips, lastDailyCheck);
		LoadBlackjackFile(chips, lastDailyCheck);

		std::cout << "+------------------------------------------+\n";
		std::cout << "| Your Chips: $" << std::setw(27) << chips << " |\n";
		std::cout << "+--------+---------------------------------+\n";
		std::cout << "| Table  | Entry Requirement               |\n";
		std::cout << "+--------+---------------------------------+\n";

		for (int i = 1; i <= blackjack.maxTables; i++) {
			std::cout << "| " << std::setw(3) << i << "    | $"
				<< std::setw(30) << requirements[i - 1] << " |\n";
		}

		std::cout << "+--------+---------------------------------+\n\n";
		std::cout << "Choose a table number: ";
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

