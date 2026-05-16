#pragma once

#include <random>

void BlackjackDrawCard(char playerOrDealer, std::vector<int>& playerCards, std::vector<int>& dealerCards, int(&cards)[10], int(&cardsWeighted)[10], int& cardsDrawn, std::discrete_distribution<>& dist, std::mt19937& gen);
void hitOrStand(char playerChoice, std::vector<int>& playerCards, std::vector<int>& dealerCards, int(&cards)[10], int(&cardsWeighted)[10], int& cardsDrawn, std::discrete_distribution<>& dist, std::mt19937& gen);
void AceOneOrEleven(std::vector<int>& playerOrDealerCards, int& playerOrDealerTotal);
void CalculatePlayerOrDealerTotal(std::vector<int> playerOrDealerCards, int& playerOrDealerTotal);
void BlackjackAceCheck(int result, std::vector<int>& dealerOrPlayerCards);