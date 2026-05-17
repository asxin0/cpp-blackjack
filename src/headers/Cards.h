#pragma once

#include <random>
#include <vector>

struct Shoe {
	int cards[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	int cardsWeighted[10] = { 12, 12, 12, 500000, 12, 12, 12, 12, 12, 48 };

	int cardsDrawn = 0;

	std::random_device rd;
	std::mt19937 gen{rd()};

	std::discrete_distribution<> dist{ cardsWeighted, cardsWeighted + 10 };
};

struct Hand {
	std::vector<int> cards = {};

	int total = 0;
};

void BlackjackDrawCard(Hand& hand, Shoe& shoe);
void playerHit(Hand& hand, Hand& dealer, Shoe& shoe);
void playerStand(Hand& hand, Hand& dealer, Shoe& shoe);
void AceOneOrEleven(Hand& hand);
void CalculatePlayerOrDealerTotal(Hand& hand);
void BlackjackAceCheck(int result, Hand& hand);