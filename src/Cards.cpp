#include "headers/Cards.h"
#include "headers/Blackjack.h"
#include "headers/Display.h"

#include <iostream>

void BlackjackDrawCard(char playerOrDealer, std::vector<int>& playerCards, std::vector<int>& dealerCards, int(&cards)[10], int(&cardsWeighted)[10], int& cardsDrawn, std::discrete_distribution<>& dist, std::mt19937& gen) {
	if (playerOrDealer == 'd') {
		int card = dist(gen);
		int result = cards[card];

		BlackjackAceCheck(result, dealerCards);

		cardsWeighted[card]--;
		cardsDrawn++;

		dist = std::discrete_distribution<>(cardsWeighted, cardsWeighted + 10);
	}
	else {
		int card = dist(gen);
		int result = cards[card];

		BlackjackAceCheck(result, playerCards);

		cardsWeighted[card]--;
		cardsDrawn++;

		dist = std::discrete_distribution<>(cardsWeighted, cardsWeighted + 10);
	}
}

void hitOrStand(char playerChoice, std::vector<int>& playerCards, std::vector<int>& dealerCards, int(&cards)[10], int(&cardsWeighted)[10], int& cardsDrawn, std::discrete_distribution<>& dist, std::mt19937& gen) {
	int playerTotal;
	int dealerTotal;

	CalculatePlayerOrDealerTotal(playerCards, playerTotal);
	CalculatePlayerOrDealerTotal(dealerCards, dealerTotal);

	switch (std::toupper(playerChoice)) {
	case 'H':
		while (true) {
			BlackjackDrawCard('p', playerCards, dealerCards, cards, cardsWeighted, cardsDrawn, dist, gen);;

			AceOneOrEleven(playerCards, playerTotal);

			DisplayPlayerOrDealerCards(playerTotal, playerCards, "Player");
			DisplayPlayerOrDealerCards(dealerTotal, dealerCards, "Dealer");

			CalculatePlayerOrDealerTotal(playerCards, playerTotal);

			if (playerTotal >= 21) {
				break;
			}

			if (hitOrStandCheck() == 'S') {
				break;
			}
		}
	case 'S':
		while (dealerTotal < 17) {
			BlackjackDrawCard('d', playerCards, dealerCards, cards, cardsWeighted, cardsDrawn, dist, gen);

			AceOneOrEleven(dealerCards, dealerTotal);

			DisplayPlayerOrDealerCards(playerTotal, playerCards, "Player");
			DisplayPlayerOrDealerCards(dealerTotal, dealerCards, "Dealer");

			CalculatePlayerOrDealerTotal(dealerCards, dealerTotal);

			std::cout << "\n";
		}
	}
}

void AceOneOrEleven(std::vector<int>& playerOrDealerCards, int& playerOrDealerTotal) {
	int x = 0;
	for (int i : playerOrDealerCards) {
		CalculatePlayerOrDealerTotal(playerOrDealerCards, playerOrDealerTotal);

		if (i == 11 && playerOrDealerTotal > 21) {
			playerOrDealerCards[x] = 1;
		}
		x++;
	}
}

void CalculatePlayerOrDealerTotal(std::vector<int> playerOrDealerCards, int& playerOrDealerTotal) {
	playerOrDealerTotal = 0;
	for (int i : playerOrDealerCards) {
		playerOrDealerTotal += i;
	}
}

void BlackjackAceCheck(int result, std::vector<int>& dealerOrPlayerCards) {
	if (result != 1) {
		dealerOrPlayerCards.push_back(result);
	}
	else {
		dealerOrPlayerCards.push_back(11);
	}
}