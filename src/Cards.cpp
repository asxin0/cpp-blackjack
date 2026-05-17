#include "headers/Cards.h"
#include "headers/Blackjack.h"
#include "headers/Display.h"

#include <iostream>

void BlackjackDrawCard(char playerOrDealer, Hand& hand, Shoe& shoe) {

	if (playerOrDealer == 'd') {
		int card = shoe.dist(shoe.gen);
		int result = shoe.cards[card];

		BlackjackAceCheck(result, hand);

		shoe.cardsWeighted[card]--;
		shoe.cardsDrawn++;

		shoe.dist = std::discrete_distribution<>(shoe.cardsWeighted, shoe.cardsWeighted + 10);
	}
	else {
		int card = shoe.dist(shoe.gen);
		int result = shoe.cards[card];

		BlackjackAceCheck(result, hand);

		shoe.cardsWeighted[card]--;
		shoe.cardsDrawn++;

		shoe.dist = std::discrete_distribution<>(shoe.cardsWeighted, shoe.cardsWeighted + 10);
	}
}

void hitOrStand(char playerChoice, Hand& player, Hand& dealer, Shoe& shoe) {

	CalculatePlayerOrDealerTotal(player);
	CalculatePlayerOrDealerTotal(dealer);

	switch (std::toupper(playerChoice)) {
	case 'H':
		while (true) {
			BlackjackDrawCard('p', player, shoe);;

			AceOneOrEleven(player);

			DisplayPlayerOrDealerCards(player, "Player");
			DisplayPlayerOrDealerCards(dealer, "Dealer");

			CalculatePlayerOrDealerTotal(player);

			if (player.total >= 21) {
				break;
			}

			if (hitOrStandCheck() == 'S') {
				break;
			}
		}
	case 'S':
		while (dealer.total < 17) {
			BlackjackDrawCard('d', dealer, shoe);

			AceOneOrEleven(dealer);

			DisplayPlayerOrDealerCards(player, "Player");
			DisplayPlayerOrDealerCards(dealer, "Dealer");

			CalculatePlayerOrDealerTotal(dealer);

			std::cout << "\n";
		}
	}
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