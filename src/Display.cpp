#include "headers/System.h"
#include "headers/Blackjack.h"
#include "headers/Cards.h"

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

void DisplayHandResults(std::vector<int>playerCards, std::vector<int>dealerCards, int& cardsDrawn, int& totalBalance, int& bet) {
	int playerTotal;
	int dealerTotal;

	CalculatePlayerOrDealerTotal(playerCards, playerTotal);
	CalculatePlayerOrDealerTotal(dealerCards, dealerTotal);

	if (playerTotal > 21) {
		std::cout << "\n\nYou bust!";
	}
	else if (playerTotal == 21 && dealerTotal != 21) {
		std::cout << "\n\nBlackjack!";
		totalBalance += (bet * 2) + (bet * 0.5);
	}
	else if (dealerTotal > 21) {
		std::cout << "\n\nDealer busts!";
		totalBalance += (bet * 2);
	}
	else if (playerTotal == dealerTotal) {
		std::cout << "\n\nPush!";
		totalBalance += bet;
	}
	else if (playerTotal > dealerTotal) {
		std::cout << "\n\nYou win!";
		totalBalance += (bet * 2);
	}
	else {
		std::cout << "\n\nYou lose!";
	}

	std::cout << "\nYour new balance is $" << totalBalance << ".\n";
	SaveBlackjackFile(totalBalance, CurrentTime());

	if (cardsDrawn >= 117) {
		std::cout << "\nYou've hit the cut card, reshuffling shoe.";
		BlackjackMenu();
	}
}

void DisplayPlayerOrDealerCards(int playerOrDealerTotal, std::vector<int>& playerOrDealerCards, std::string name) {
	CalculatePlayerOrDealerTotal(playerOrDealerCards, playerOrDealerTotal);

	std::cout << "\n| Total: " << std::setw(2) << playerOrDealerTotal << " | ";
	std::cout << name << " cards: ";
	int x = 1;

	for (int i : playerOrDealerCards) {
		if (i == 11) {
			if (x == playerOrDealerCards.size()) {
				std::cout << i << " (1 or 11)";
			}
			else {
				std::cout << i << " (1 or 11), ";
			}
		}
		else {
			if (x == playerOrDealerCards.size()) {
				std::cout << i;
			}
			else {
				std::cout << i << ", ";
			}
		}
		x++;
	}
}