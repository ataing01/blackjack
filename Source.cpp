#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <span>

struct card {
	int val;
	std::string face;
	std::string suit;
	bool red;
	bool hidden = true;

	card(int v, std::string f, std::string s, bool r) {
		val = v;
		face = f;
		suit = s;
		red = r;
	}
};

//initialization of deck
std::vector<card*> og =
{ {new card(11, "ace", "clubs", false)},{new card(11, "ace", "spades", false)},{new card(11, "ace", "hearts", true)},{new card(11, "ace", "diamonds", true)},
{new card(2, "num", "clubs", false)},{new card(2, "num", "spades", false)},{new card(2, "num", "hearts", true)},{new card(2, "num", "diamonds", true)},
{new card(3, "num", "clubs", false)},{new card(3, "num", "spades", false)},{new card(3, "num", "hearts", true)},{new card(3, "num", "diamonds", true)},
{new card(4, "num", "clubs", false)},{new card(4, "num", "spades", false)},{new card(4, "num", "hearts", true)},{new card(4, "num", "diamonds", true)},
{new card(5, "num", "clubs", false)},{new card(5, "num", "spades", false)},{new card(5, "num", "hearts", true)},{new card(5, "num", "diamonds", true)},
{new card(6, "num", "clubs", false)},{new card(6, "num", "spades", false)},{new card(6, "num", "hearts", true)},{new card(6, "num", "diamonds", true)},
{new card(7, "num", "clubs", false)},{new card(7, "num", "spades", false)},{new card(7, "num", "hearts", true)},{new card(7, "num", "diamonds", true)},
{new card(8, "num", "clubs", false)},{new card(8, "num", "spades", false)},{new card(8, "num", "hearts", true)},{new card(8, "num", "diamonds", true)},
{new card(9, "num", "clubs", false)},{new card(9, "num", "spades", false)},{new card(9, "num", "hearts", true)},{new card(9, "num", "diamonds", true)},
{new card(10, "num", "clubs", false)},{new card(10, "num", "spades", false)},{new card(10, "num", "hearts", true)},{new card(10, "num", "diamonds", true)},
{new card(10, "jack", "clubs", false)},{new card(10, "jack", "spades", false)},{new card(10, "jack", "hearts", true)},{new card(10, "jack", "diamonds", true)},
{new card(10, "queen", "clubs", false)},{new card(10, "queen", "spades", false)},{new card(10, "queen", "hearts", true)},{new card(10, "queen", "diamonds", true)},
{new card(10, "king", "clubs", false)},{new card(10, "king", "spades", false)},{new card(10, "king", "hearts", true)},{new card(10, "king", "diamonds", true)} };

//shuffle deck
std::vector<card*> shuffle(std::vector<card*> deck) {
	std::vector<card*> result;
	srand(time(NULL));
	while (!(deck.empty())) {
		int random = rand() % deck.size();
		result.push_back(deck[random]);
		deck.erase(deck.begin() + random);
	}
	return result;
}

//revert value of ace from 1 back to 11 after game
void resetdeck(std::vector<card*> deck) {
	for (int i = 0; i < deck.size(); i++) {
		if (deck[i]->face == "ace" && deck[i]->val == 1) {
			deck[i]->val = 11;
		}
	}
}

//output unhidden cards in hand
void printhand(std::vector<card*> hand) {
	for (int i = 0; i < hand.size(); i++) {
		if (hand[i]->hidden) {
			std::cout << "Hidden card" << std::endl;
		}
		else {
			if (hand[i]->face == "num" || hand[i]->face == "ace") {
				std::cout << hand[i]->val << " of " << hand[i]->suit << std::endl;
			}
			else {
				std::cout << hand[i]->face << " of " << hand[i]->suit << std::endl;
			}
		}
	}
}

//output total value of hand
int gettotalval(std::vector<card*> hand) {
	int result = 0;
	for (int i = 0; i < hand.size(); i++) {
		result += hand[i]->val;
	}
	return result;
}

//check if hand contains card with target face
bool hascard(std::vector<card*> hand, std::string target) {
	for (int i = 0; i < hand.size(); i++) {
		if (hand[i]->face == target) { return true; }
	}
	return false;
}

//find first index of card with val in hand
int findcard(std::vector<card*> hand, int val) {
	for (int i = 0; i < hand.size(); i++) {
		if (hand[i]->val == val) { return i; }
	}
	return -1;
}

//return true if total val of hand accounting for aces exceeds cap
//aces are always 11 for dealer
bool bust(std::vector<card*> hand, int cap, bool dealer) {
	int total = gettotalval(hand);
	if (!dealer) {
		if (total > cap) {
			if (hascard(hand, "ace") && findcard(hand, 11) != -1) {
				int aceidx = findcard(hand, 11);
				hand[aceidx]->val = 1;
				return bust(hand, cap, dealer);
			}
			else { return true; }
		}
		else {
			return false;
		}
	}
	else if (total > cap) {
		return true;
	}
	else { return false; }
}

//function to parse "x.set val val" to set x's hand as such
void dev(std::string cmd, std::vector<card*>& hand) {
	int i = 0;
	hand.clear();
	while (i < cmd.length()) {
		if (cmd[i] == ' ') {
			i++; //skip space
			std::string val = "";
			while (i < cmd.length() && isdigit(cmd[i])) {
				val += cmd[i];
				i++;
			}
			std::cout << "val: " << val << std::endl;
			hand.push_back(og[findcard(og, stoi(val))]);
		}
		else { i++; }
	}
}

int main() {
	std::vector<card*> cards;
	cards.resize(52);

	std::vector<card*> dealer;
	std::vector<card*> player;

	int gameover = -1;
	//-1 = game in progress
	//0 = tie
	//1 = win
	//2 = lose

	//TODO: splitting pairs, double down
	int money = 100;
	std::cout << "The dealer will stand at 17" << std::endl;
	std::cout << "Player aces are 11 unless they would bust the hand, then they are 1" << std::endl;
	std::cout << "Dealer aces are always 11" << std::endl << std::endl;
	while (money > 0) {
		//initialization
		dealer.clear();
		player.clear();

		cards = og;
		resetdeck(cards);

		cards = shuffle(cards);

		std::stack<card*> deck;
		for (int i = 0; i < cards.size(); i++) {
			deck.push(cards[i]);
		}

		for (int i = 0; i < 2; i++) {
			player.push_back(deck.top());
			deck.pop();
			player[i]->hidden = false;

			dealer.push_back(deck.top());
			deck.pop();
		}
		dealer[1]->hidden = false; //reveal dealer's 2nd card
		gameover = -1;

		//game start
		if (dealer[0]->hidden == false) {
			dealer[0]->hidden = true;
		}

		std::cout << "Your money: " << money << std::endl;
		std::cout << "Place your bet:" << std::endl;
		std::string sbet;
		bool invalidbet = true;
		while (invalidbet == true) {
			std::cin >> sbet;
			for (int i = 0; i < sbet.length(); i++) {
				if (isdigit(sbet[i]) == false) {
					break;
				}
				else if (i == sbet.length() - 1 && stoi(sbet) <= money && stoi(sbet) > 0) {
					invalidbet = false;
					std::cout << "success" << std::endl;
				}
			}
			if (invalidbet) {
				std::cout << "Invalid input, try again:" << std::endl;
			}
		}
		std::cin.clear();
		int bet = stoi(sbet);
		money -= bet;

		while (gameover == -1) {
			std::cout << "Your turn!" << std::endl;
			//player turn

			//CHECK FOR SPLIT

			bool stand = false;
			while (stand == false) {
				std::cout << "Dealer's hand:" << std::endl;
				printhand(dealer);
				std::cout << std::endl;
				std::cout << "Your hand, total value " << gettotalval(player) << ":" << std::endl;
				printhand(player);
				std::cout << std::endl;

				//check if either player started with 21
				if (gettotalval(player) == 21) {
					if (gettotalval(dealer) == 21) {
						gameover = 0;
						break;
					}
					else {
						gameover = 1;
						break;
					}
				}
				else if (gettotalval(dealer) == 21) {
					gameover = 2;
					break;
				}

				std::cout << "Input 'hit' or 'stand'" << std::endl;
				std::string input = "invalid";
				while (input == "invalid") {
					std::getline(std::cin, input);
					//dev command to set cards
					if (input.substr(0, 10) == "player.set" || input.substr(0, 10) == "dealer.set") {
						if (input.substr(0, 10) == "player.set") {
							dev(input, player);
						}
						else {
							dev(input, dealer);
						}
						std::cout << "Command successful" << std::endl;
						input = "invalid";
					}
					//gameplay
					else if (input != "hit" && input != "stand") {
						if (input.length() > 0) {
							std::cout << "Invalid input! Type 'hit' or 'stand'" << std::endl;
						}
						input = "invalid";
					}
					std::cout << std::endl;
				}

				if (input == "hit") {
					player.push_back(deck.top());
					deck.pop();
					player[player.size() - 1]->hidden = false;
					std::cout << "You received a " << player[player.size() - 1]->val << " of " << player[player.size() - 1]->suit << std::endl;
					//check for bust
					if (bust(player, 21, false)) { gameover = 2; stand = true; }
				}
				else if (input == "stand") { stand = true; }
			}

			std::cout << "Dealer's turn!" << std::endl;
			//dealer turn
			//hit at 16 and below, stand at 17 and above
			dealer[0]->hidden = false;

			std::cout << "Dealer's hand, total value " << gettotalval(dealer) << ":" << std::endl;
			printhand(dealer);
			while (!(bust(dealer, 16, true))) {
				std::cout << std::endl;
				std::cout << "Dealer hits" << std::endl;
				dealer.push_back(deck.top());
				deck.pop();
				dealer[dealer.size() - 1]->hidden = false;
				std::cout << "Dealer's hand, total value " << gettotalval(dealer) << ":" << std::endl;
				printhand(dealer);
			}
			std::cout << "Dealer stands" << std::endl << std::endl;

			//check for bust
			if (bust(dealer, 21, true)) {
				if (gameover == -1) { //if player did not bust but dealer did bust
					gameover = 1;
				}
				else if (gameover == 2) { //both player and dealer busted
					//do nothing because dealer wins in this situation apparently
				}
			}

			//compare player and dealer if neither busted
			if (gameover == -1) {
				if (gettotalval(player) > gettotalval(dealer)) {
					gameover = 1;
				}
				else if (gettotalval(player) < gettotalval(dealer)) {
					gameover = 2;
				}
				else if (gettotalval(player) == gettotalval(dealer)) {
					gameover = 0;
				}
			}
		}
		if (gameover == 0) {
			std::cout << "Tie! Bet returned" << std::endl;
			std::cout << "Dealer: " << gettotalval(dealer) << std::endl;
			std::cout << "You: " << gettotalval(player) << std::endl;
			money += bet;
		}
		else if (gameover == 1) {
			money += bet * 2;
			std::cout << "You won! Bet won x2" << std::endl;
			std::cout << "Dealer: " << gettotalval(dealer) << std::endl;
			std::cout << "You: " << gettotalval(player) << std::endl;
		}
		else if (gameover == 2) {
			//nothing happens, you already lost the bet
			std::cout << "You lost! Bet lost" << std::endl;
			std::cout << "Dealer: " << gettotalval(dealer) << std::endl;
			std::cout << "You: " << gettotalval(player) << std::endl;
		}

		std::cout << "Play again? y/n" << std::endl;
		std::string qnext = "invalid";
		while (qnext == "invalid") {
			std::cin >> qnext;
			if (qnext != "y" && qnext != "n") {
				std::cout << "Invalid input! Type 'y' or 'n'" << std::endl;
				qnext = "invalid";
			}
		}
		std::cout << std::endl;

		if (qnext == "n") { break; }
	}

	std::cout << "Your final money was " << money;
}