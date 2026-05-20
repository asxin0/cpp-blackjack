#include "headers/System.h"
#include "headers/Blackjack.h"
#include "headers/Cards.h"

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

void DisplayHandResults(Hand& player, Hand& dealer, Shoe& shoe, int& totalBalance) {
	CalculatePlayerOrDealerTotal(player);
	CalculatePlayerOrDealerTotal(dealer);

	if (player.total > 21) {
		std::cout << "\n\nYou bust!";
	}
	else if (player.total == 21 && dealer.total != 21) {
		std::cout << "\n\nBlackjack!";
		totalBalance += (player.bet * 2) + (player.bet * 0.5);
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

	if (shoe.cardsDrawn >= 117) {
		std::cout << "\nYou've hit the cut card, reshuffling shoe.";
		BlackjackMenu();
	}
}

void DisplayPlayerOrDealerCards(Hand& hand, std::string name) {
	CalculatePlayerOrDealerTotal(hand);

	std::cout << "\n| Total: " << std::setw(2) << hand.total << " | ";
	std::cout << name << " cards: ";
	int x = 1;

	for (int i : hand.cards) {
		if (i == 11) {
			if (x == hand.cards.size()) {
				std::cout << i << " (Ace)";
			}
			else {
				std::cout << i << " (Ace), ";
			}
		}
		else {
			if (x == hand.cards.size()) {
				std::cout << i;
			}
			else {
				std::cout << i << ", ";
			}
		}
		x++;
	}
}