#include <iostream>
#include <array>
#include <ctime> // для time()
#include <cstdlib> // для rand() и srand()
#include <cassert>

class Desk;
class Card
{
public:
	enum CardSuit
	{
		SUIT_CLUB,
		SUIT_DIAMOND,
		SUIT_HEART,
		SUIT_SPADE,
		MAX_SUITS
	};
	enum CardRank
	{
		RANK_2,
		RANK_3,
		RANK_4,
		RANK_5,
		RANK_6,
		RANK_7,
		RANK_8,
		RANK_9,
		RANK_10,
		RANK_JACK,
		RANK_QUEEN,
		RANK_KING,
		RANK_ACE,
		MAX_RANKS
	};
private:
	CardRank _rank;
	CardSuit _suit;
public:
	Card(CardRank rank = MAX_RANKS, CardSuit suit = MAX_SUITS)
		: _rank(rank), _suit(suit)
	{}
	void printCard() const
	{
		switch (_rank)
		{
		case RANK_2:		std::cout << '2'; break;
		case RANK_3:		std::cout << '3'; break;
		case RANK_4:		std::cout << '4'; break;
		case RANK_5:		std::cout << '5'; break;
		case RANK_6:		std::cout << '6'; break;
		case RANK_7:		std::cout << '7'; break;
		case RANK_8:		std::cout << '8'; break;
		case RANK_9:		std::cout << '9'; break;
		case RANK_10:		std::cout << 'T'; break;
		case RANK_JACK:		std::cout << 'J'; break;
		case RANK_QUEEN:	std::cout << 'Q'; break;
		case RANK_KING:		std::cout << 'K'; break;
		case RANK_ACE:		std::cout << 'A'; break;
		}

		switch (_suit)
		{
		case SUIT_CLUB:		std::cout << 'C'; break;
		case SUIT_DIAMOND:	std::cout << 'D'; break;
		case SUIT_HEART:	std::cout << 'H'; break;
		case SUIT_SPADE:	std::cout << 'S'; break;
		}
	}
	int getCardValue() const
	{
		switch (_rank)
		{
		case RANK_2:		return 2;
		case RANK_3:		return 3;
		case RANK_4:		return 4;
		case RANK_5:		return 5;
		case RANK_6:		return 6;
		case RANK_7:		return 7;
		case RANK_8:		return 8;
		case RANK_9:		return 9;
		case RANK_10:		return 10;
		case RANK_JACK:		return 10;
		case RANK_QUEEN:	return 10;
		case RANK_KING:		return 10;
		case RANK_ACE:		return 11;
		}
		return 0;
	}
};
char getPlayerChoice();
class Deck
{
private:
	std::array<Card, 52> _deck;
	int _cardIndex{ 0 };
	static int getRandomNumber(int min, int max)
	{
		static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);

		return static_cast<int>(rand() * fraction * (max - min + 1) + min);
	}
	void swapCard(Card &a, Card &b)
	{
		Card temp = a;
		a = b;
		b = temp;
	}
public:
	Deck()
	{
		int card{ 0 };
		for (int suit = 0; suit < Card::MAX_SUITS; ++suit)
			for (int rank = 0; rank < Card::MAX_RANKS; ++rank)
			{
				_deck[card] = Card(static_cast<Card::CardRank>(rank), static_cast<Card::CardSuit>(suit));
				++card;
			}
	}
	void printDeck() const
	{
		for (const auto &card : _deck)
		{
			card.printCard();
			std::cout << ' ';
		}

		std::cout << '\n';
	}
	void shuffleDeck()
	{
		for (int index = 0; index < 52; ++index)
		{
			int swapIndex = getRandomNumber(0, 51);
			swapCard(_deck[index], _deck[swapIndex]);
		}
		_cardIndex = 0;
	}
	const Card& dealCard()
	{
		assert(_cardIndex < 52);
		return _deck[_cardIndex++];
	}

	bool playBlackjack()
	{
		const Card card = _deck[0];

		int playerTotal = 0;
		int dealerTotal = 0;

		// Дилер получает одну карту
		dealerTotal += card.getCardValue();
		std::cout << "The dealer is showing: " << dealerTotal << '\n';

		// Игрок получает две карты
		playerTotal += card.getCardValue();
		playerTotal += card.getCardValue();

		// Игрок начинает
		while (1)
		{
			std::cout << "You have: " << playerTotal << '\n';
			char choice = getPlayerChoice();
			if (choice == 's')
				break;

			playerTotal += card.getCardValue();

			// Смотрим, не проиграл ли игрок
			if (playerTotal > 21)
				return false;
		}

		// Если игрок не проиграл (у него не больше 21 очка), тогда дилер получает карты до тех пор, пока у него в сумме будет не меньше 17 очков
		while (dealerTotal < 17)
		{
			dealerTotal += card.getCardValue();
			std::cout << "The dealer now has: " << dealerTotal << '\n';
		}

		// Если у дилера больше 21, то он проиграл, а игрок выиграл
		if (dealerTotal > 21)
			return true;

		return (playerTotal > dealerTotal);
	}
};

char getPlayerChoice()
{
	std::cout << "(h) to hit, or (s) to stand: ";
	char choice;
	do
	{
		std::cin >> choice;
	} while (choice != 'h' && choice != 's');

	return choice;
}



int main()
{
	srand(static_cast<unsigned int>(time(0))); // используем системные часы в качестве стартового значения
	rand(); // пользователям Visual Studio: делаем сброс первого случайного числа

	Deck deck;
	deck.shuffleDeck();
	deck.printDeck();
	deck.shuffleDeck();

	if (deck.playBlackjack())
		std::cout << "You win!\n";
	else
		std::cout << "You lose!\n";

	return 0;
}
