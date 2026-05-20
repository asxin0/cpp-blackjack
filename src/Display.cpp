#include "headers/System.h"
#include "headers/Blackjack.h"
#include "headers/Cards.h"

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <cctype>

void DisplayHandResults(Hand& player, Hand& dealer, Shoe& shoe, int& totalBalance) {
	CalculatePlayerOrDealerTotal(player);
	CalculatePlayerOrDealerTotal(dealer);

	if (player.total > 21) {
		std::cout << "\n\nYou bust!";
	}
	else if (player.total == 21 && dealer.total != 21 && player.cards.size() == 2) {
		std::cout << "\n\nBlackjack!";
		totalBalance += player.bet * 5 / 2;
	}
	else if (dealer.total > 21) {
		std::cout << "\n\nDealer busts!";
		totalBalance += (player.bet * 2);
	}
	else if (player.total == dealer.total) {
		std::cout << "\n\nPush!";
		totalBalance += player.bet;
	}
	else if (player.total > dealer.total) {
		std::cout << "\n\nYou win!";
		totalBalance += (player.bet * 2);
	}
	else {
		std::cout << "\n\nYou lose!";
	}
}

void DisplayBalanceResults(Shoe& shoe, Hand& player, int& totalBalance) {
	std::cout << "\nYour new balance is $" << totalBalance << ".\n";
	SaveBlackjackFile(totalBalance, CurrentTime());
}

void DisplayPlayerOrDealerCards(Hand& hand, std::string name, bool hideSecond) {
	CalculatePlayerOrDealerTotal(hand);

	if (hideSecond) {
		hand.total -= hand.cards[1];
	}

	std::cout << "\n| Total: " << std::setw(2) << hand.total << " | ";
	std::cout << name << " cards: ";
	
	if (hideSecond) {
		hand.total += hand.cards[1];
	}

	for (int i = 0; i < hand.cards.size(); i++) {
		if (hideSecond && i == 1) {
			std::cout << "?";
		}
		else {
			std::cout << hand.cards[i];

			if (hand.cards[i] == 11 || hand.cards[i] == 1) {
				std::cout << " (Ace)";
			}
		}

		if (i < hand.cards.size() - 1) {
			std::cout << ", ";
		}
	}
}