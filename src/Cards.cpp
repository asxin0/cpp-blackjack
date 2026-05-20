#include "headers/Cards.h"
#include "headers/Blackjack.h"
#include "headers/Display.h"

#include <iostream>

void BlackjackDrawCard(Hand& hand, Shoe& shoe) {
	int card = shoe.dist(shoe.gen);
	int result = shoe.cards[card];

	BlackjackAceCheck(result, hand);

	shoe.cardsWeighted[card]--;
	shoe.cardsDrawn++;

	shoe.dist = std::discrete_distribution<>(shoe.cardsWeighted, shoe.cardsWeighted + 10);
}

void playerHit(Hand& player, Hand& dealer, Shoe& shoe, int& totalBalance) {
	CalculatePlayerOrDealerTotal(player);
	CalculatePlayerOrDealerTotal(dealer);
	
	while (true) {
		BlackjackDrawCard(player, shoe);;

		AceOneOrEleven(player);

		DisplayPlayerOrDealerCards(player, "Player");
		DisplayPlayerOrDealerCards(dealer, "Dealer");

		CalculatePlayerOrDealerTotal(player);

		if (player.total >= 21) {
			break;
		}

		char playerNextChoice;

		while (true) {
			playerNextChoice = playerMoveCheck(player, totalBalance);

			if (playerNextChoice == 'D') {
				std::cout << "\nCan only double with a fresh hand.";
			}
			else {
				break;
			}
		}

		if (playerNextChoice == 'S') {
			break;
		}
	}
}

void playerStand(Hand& player, Hand& dealer, Shoe& shoe) {
	CalculatePlayerOrDealerTotal(player);
	CalculatePlayerOrDealerTotal(dealer);

	while (dealer.total < 17) {
		BlackjackDrawCard(dealer, shoe);

		AceOneOrEleven(dealer);

		DisplayPlayerOrDealerCards(player, "Player");
		DisplayPlayerOrDealerCards(dealer, "Dealer");

		CalculatePlayerOrDealerTotal(dealer);

		std::cout << "\n";
	}
}

void playerDouble(Hand& player, Hand& dealer, Shoe& shoe, int& totalBalance) {
	CalculatePlayerOrDealerTotal(player);
	CalculatePlayerOrDealerTotal(dealer);

	BlackjackDrawCard(player, shoe);

	DisplayPlayerOrDealerCards(player, "Player");
	DisplayPlayerOrDealerCards(dealer, "Dealer");
}

void AceOneOrEleven(Hand& hand) {

	int x = 0;
	for (int i : hand.cards) {
		CalculatePlayerOrDealerTotal(hand);

		if (i == 11 && hand.total > 21) {
			hand.cards[x] = 1;
		}
		x++;
	}
}

void CalculatePlayerOrDealerTotal(Hand& hand) {
	hand.total = 0;
	for (int i : hand.cards) {
		hand.total += i;
	}
}

void BlackjackAceCheck(int result, Hand& hand) {
	if (result != 1) {
		hand.cards.push_back(result);
	}
	else {
		hand.cards.push_back(11);
	}
}