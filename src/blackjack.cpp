#include <iostream>
#include <string>
#include <format>
#include <random>
#include <cctype>
#include <vector>
#include <fstream>
#include <ctime>
#include <iomanip>

void BlackjackMenu();

struct BlackjackTableValues {
	int table1 = 50;
	int table2 = 500;
	int table3 = 5000;
	int maxTables = 3;
};

void LoadBlackjackFile(int& chips, int& lastDailyCheck) {
	std::string tempChips;
	std::string tempDailyCheck;

	std::ifstream file("Blackjack.txt");
	std::getline(file, tempChips);
	std::getline(file, tempDailyCheck);

	chips = std::stoi(tempChips);
	lastDailyCheck = std::stoi(tempDailyCheck);
	file.close();
}

void SaveBlackjackFile(int chips, int lastDailyCheck) {
	std::ofstream file("Blackjack.txt");
	file << chips << "\n";
	file << lastDailyCheck;
	file.close();
}

int CurrentTime() {
	time_t currentTime = time(0);
	return currentTime;
}

void DailyBlackjackReward(int chips, int lastDailyCheck) {
	time_t currentTime = time(0);
	int timeFromDay = currentTime % 86400;
	int currentDailyCheck = currentTime - timeFromDay;

	if (currentDailyCheck >= lastDailyCheck + 86400) {
		lastDailyCheck = currentDailyCheck;
		chips = chips + 1500;
		SaveBlackjackFile(chips, lastDailyCheck);

		std::cout << "Thank you for logging in today! Your daily reward of 1500 chips has been depositied.";
		std::cout << "Login again in " << 86400 - timeFromDay << " seconds for your next reward.\n\n";
	}
	else {
		std::cout << "Login again in " << 86400 - timeFromDay << " seconds for your next reward.\n\n";
	}
}

void CalculatePlayerOrDealerTotal(std::vector<int> playerOrDealerCards, int& playerOrDealerTotal) {
	playerOrDealerTotal = 0;
	for (int i : playerOrDealerCards) {
		playerOrDealerTotal = playerOrDealerTotal + i;
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

void BlackjackGame(int& bet, int& totalBalance, int& cardsDrawn, int(&cards)[10], int(&cardsWeighted)[10]) {

	std::vector<int> playerCards = {};
	std::vector<int> dealerCards = {};
	std::random_device rd;
	std::mt19937 gen(rd());
	int playerTotal = 0;
	int dealerTotal = 0;

	bool runOnce = true;

	while (true) {
		std::discrete_distribution<> dist(cardsWeighted, cardsWeighted + 10);
		int i = 1;

		while (i <= 3) {
			if (i == 2) {
				BlackjackDrawCard('d', playerCards, dealerCards, cards, cardsWeighted, cardsDrawn, dist, gen);
			}
			else if (i == 1 || i == 3) {
				BlackjackDrawCard('p', playerCards, dealerCards, cards, cardsWeighted, cardsDrawn, dist, gen);
			}
			else {
			}
			i++;
		}

		while (true) {
			CalculatePlayerOrDealerTotal(playerCards, playerTotal);
			CalculatePlayerOrDealerTotal(dealerCards, dealerTotal);

			while (runOnce) {
				DisplayPlayerOrDealerCards(playerTotal, playerCards, "Player");
				DisplayPlayerOrDealerCards(dealerTotal, dealerCards, "Dealer");
				runOnce = false;
			}

			if (playerTotal == 21) {
				break;
			}

			AceOneOrEleven(playerCards, playerTotal);

			if (playerTotal >= 21) {
				break;
			}

			char hitOrStand;
			std::cout << "\n\nHit or Stand (H/S): ";
			std::cin >> hitOrStand;
			std::cout << "\n";

			if (std::toupper(hitOrStand) == 'H') {
				BlackjackDrawCard('p', playerCards, dealerCards, cards, cardsWeighted, cardsDrawn, dist, gen);;

				AceOneOrEleven(playerCards, playerTotal);

				DisplayPlayerOrDealerCards(playerTotal, playerCards, "Player");
				DisplayPlayerOrDealerCards(dealerTotal, dealerCards, "Dealer");
			}
			else if (std::toupper(hitOrStand) == 'S') {
				while (dealerTotal < 17) {
					BlackjackDrawCard('d', playerCards, dealerCards, cards, cardsWeighted, cardsDrawn, dist, gen);
					CalculatePlayerOrDealerTotal(dealerCards, dealerTotal);

					AceOneOrEleven(dealerCards, dealerTotal);

					DisplayPlayerOrDealerCards(playerTotal, playerCards, "Player");
					DisplayPlayerOrDealerCards(dealerTotal, dealerCards, "Dealer");
					std::cout << "\n";
				}
				break;
			}
			else {
				std::cout << "Invalid move, please enter either H (hit) or S (stand)\n\n";
			}
		}

		CalculatePlayerOrDealerTotal(playerCards, playerTotal);
		CalculatePlayerOrDealerTotal(dealerCards, dealerTotal);

		if (playerTotal > 21) {
			std::cout << "\n\nYou bust!";
		}
		else if (playerTotal == 21 && dealerTotal != 21) {
			std::cout << "\n\nBlackjack!";
			totalBalance = totalBalance + (bet * 2) + (bet * 0.5);
		}
		else if (dealerTotal > 21) {
			std::cout << "\n\nDealer busts!";
			totalBalance = totalBalance + (bet * 2);
		}
		else if (playerTotal == dealerTotal) {
			std::cout << "\n\nPush!";
			totalBalance = totalBalance + bet;
		}
		else if (playerTotal > dealerTotal) {
			std::cout << "\n\nYou win!";
			totalBalance = totalBalance + (bet * 2);
		}
		else {
			std::cout << "\n\nYou lose!";
		}
		break;
	}
	std::cout << "\nYour new balance is $" << totalBalance << ".\n";
	SaveBlackjackFile(totalBalance, CurrentTime());

	if (cardsDrawn >= 117) {
		std::cout << "\nYou've hit the cut card, reshuffling shoe.";
		BlackjackMenu();
	}
}


void BlackjackTable(int table, int& chips, int& cardsDrawn, int(&cards)[10], int(&cardsWeighted)[10]) {

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
			BlackjackMenu();
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
			chips = chips - bet;
			BlackjackGame(bet, chips, cardsDrawn, cards, cardsWeighted);
		}
	}
}

void BlackjackMenu() {

	int chips;
	int lastDailyCheck;
	int table;
	BlackjackTableValues blackjack;
	int requirements[3] = { 1000, 10000, 100000 };

	int cards[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	int cardsWeighted[10] = { 48, 12, 12, 12, 12, 12, 12, 12, 12, 48 };
	int cardsDrawn = 0;

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

	while (true) {

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
			BlackjackTable(1, chips, cardsDrawn, cards, cardsWeighted);
			break;
		case 2:
			if (chips < requirements[table - 1]) {
				std::cout << "Not enough chips to enter this table.";
				std::cin.clear();
				std::cin.ignore(1000, '\n');
				break;
			}
			BlackjackTable(2, chips, cardsDrawn, cards, cardsWeighted);
			break;
		case 3:
			if (chips < requirements[table - 1]) {
				std::cout << "Not enough chips to enter this table.";
				std::cin.clear();
				std::cin.ignore(1000, '\n');
				break;
			}
			BlackjackTable(3, chips, cardsDrawn, cards, cardsWeighted);
			break;
		default:
			std::cout << "\nNot a valid table number";
			std::cin.clear();
			std::cin.ignore(1000, '\n');
		}
	}
}

