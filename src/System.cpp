#include "headers/System.h"

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>


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

	char timeMagnitude = 's';
	int timeRemainder;
	int nextLoginTime;

	nextLoginTime = 86400 - timeFromDay;

	if (nextLoginTime / 60 >= 1) {
		nextLoginTime /= 60;
		timeMagnitude = 'm';

		if (nextLoginTime / 60 > 1) {
			timeRemainder = nextLoginTime % 60;
			nextLoginTime /= 60;
			timeMagnitude = 'h';
		}
	}

	if (currentDailyCheck >= lastDailyCheck + 86400) {
		lastDailyCheck = currentDailyCheck;
		chips += 1500;
		SaveBlackjackFile(chips, lastDailyCheck);

		std::cout << "Thank you for logging in today! Your daily reward of 1500 chips has been depositied.\n";
	}
	switch (timeMagnitude) {
	case 'h':
		std::cout << "Login again in " << nextLoginTime << timeMagnitude << " " << timeRemainder << "m" << " for your next reward.\n\n";
		break;
	case 'm':
		std::cout << "Login again in " << nextLoginTime << timeMagnitude << " for your next reward.\n\n";
		break;
	default:
		std::cout << "Login again in " << 86400 - timeFromDay << timeMagnitude << " for your next reward.\n\n";
	}
}