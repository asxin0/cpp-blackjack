#include <iostream>
#include <string>

void BlackjackMenu();

void MainMenu() {
	while (true) {
		std::string menuAction;
		std::cout << "MENU\n\n";
		std::cout << "Blackjack\n\n";
		std::cout << "What do you want to play: ";
		std::getline(std::cin, menuAction);

		for (char& c : menuAction) {
			c = std::tolower(c);
		}

		if (menuAction == "blackjack") {
			BlackjackMenu();
		}
		else {
			std::cout << "Invalid game.\n\n";
		}
	}
}

int main() {
	MainMenu();
}
