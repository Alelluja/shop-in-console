#include <iostream>
#include <vector>
#include <array>
#include <fstream>
#include <string>

float user_money = 1000.0;

class shopping_cart_data
{
public:
	unsigned int game_id;
	unsigned int game_price;

public:
	std::string game_name;
};

class game_data 
{
public:
	unsigned int price;

public:
	std::string type;
	std::string name;
};

std::array<std::string, 6> game_type_names =
{
	"FPS",
	"MMORPG",
	"Adventure",
	"RPG",
	"Sport",
	"Simulator"
};

int get_choice_int();
void clear_console();

void display_shop(std::vector<game_data>& g_data, std::vector<shopping_cart_data>& s_data);
void display_games(std::vector<game_data>& g_data, std::vector<shopping_cart_data>& s_data);
void admin_menu(std::vector<game_data>& g_data);
void shopping_cart(std::vector<game_data>& g_data, std::vector<shopping_cart_data>& s_data);
void ask_for_add_to_shopping_cart(std::vector<game_data>& g_data, std::vector<shopping_cart_data>& s_data, int game_id);
void add_to_shopping_cart(std::vector<game_data>& g_data, std::vector<shopping_cart_data>& s_data, int game_index);
void choose_game(std::vector<game_data>& g_data, std::vector<shopping_cart_data>& s_data, int type);
void confirm_purchase(std::vector<game_data>& g_data, std::vector<shopping_cart_data>& s_data);
void del_from_shopping_cart_file(std::vector<shopping_cart_data>& s_data);

//Admin Menu options
void add_game(std::vector<game_data>& g_data);
void del_game(std::vector<game_data>& g_data);
void change_game_name(std::vector<game_data>& g_data);
void change_game_price(std::vector<game_data>& g_data);
void change_game_type(std::vector<game_data>& g_data);

bool is_valid_type(int i)
{
	if (i < 0)
	{
		return false;
	}

	if (i > game_type_names.size())
	{
		return false;
	}

	return true;
}

bool is_valid_game_id(int choice, std::vector<int> ids)
{
	return std::find(ids.begin(), ids.end(), choice) != ids.end();
}

bool ask_client(std::string question)
{
	printf("\nDo you want %s?\n\n", question.c_str());
	printf("1. Yes (Type 1)\n");
	printf("2. No (Type 2)\n\n");

	int choice = 0;

	printf("Choose option: ");
	choice = get_choice_int();

	return choice == 1;
}

bool write_games_file(std::vector<game_data>& g_data)
{
	std::ofstream file("shop_data.dat");

	if (!file.good())
	{
		printf("Cannot open file!\n");

		return false;
	}

	for (game_data& game : g_data)
	{
		file << game.type << "#" << game.name << "#" << game.price << std::endl;
	}

	return true;
}

bool write_shopping_cart_file(std::vector<shopping_cart_data>& s_data)
{
	std::ofstream file("shopping_cart_data.dat");

	if (!file.good())
	{
		printf("Cannot open file!\n");

		return false;
	}

	for (shopping_cart_data& shopping_cart : s_data)
	{
		file << shopping_cart.game_id << "#" << shopping_cart.game_name << "#" << shopping_cart.game_price << std::endl;
	}

	return true;
}

std::vector<game_data> read_games_file()
{
	std::ifstream stream("shop_data.dat", std::ios::out);

	if (!stream.good())
	{
		printf("Cannot open file!\n");

		return {};
	}

	std::vector<game_data> g_data;

	game_data game;

	std::string price;

	while (getline(stream, game.type, '#') && getline(stream, game.name, '#') && getline(stream, price))
	{
		game.price = stoi(price);
		g_data.push_back(game);
	}

	/*for (game_data& game : g_data)
	{
		printf("TYPE: %s | NAME: %s | PRICE: %i\n", game.type, game.name, game.price);
	}*/
	
	stream.close();

	return g_data;
}

std::vector<shopping_cart_data> read_shopping_cart_file()
{
	std::ifstream stream("shopping_cart_data.dat", std::ios::out);

	if (!stream.good())
	{
		printf("Cannot open file!\n");

		return {};
	}

	std::vector<shopping_cart_data> s_data;

	shopping_cart_data shopping_cart;

	std::string price,
		game_index;

	while (getline(stream, game_index, '#') && getline(stream, shopping_cart.game_name, '#') && getline(stream, price))
	{
		shopping_cart.game_id = stoi(game_index);
		shopping_cart.game_price = stoi(price);

		s_data.push_back(shopping_cart);
	}

	/*for (shopping_cart_data& shopping_cart : s_data)
	{
		printf("INDEX: %i | NAME: %s | PRICE: %i\n", shopping_cart.game_id, shopping_cart.game_name, shopping_cart.game_price);
	}*/

	stream.close();

	return s_data;
}

int search_game_id(std::vector<game_data>& g_data, const std::string& game_name)
{
	// Iterate over all elements in Vector
	for (int i = 0; i < g_data.size(); i++)
	{
		if (g_data[i].name == game_name)
		{
			return i;
		}
	}

	return -1;
}

// Error - message : see reference to function template instantiation
/*int search_game_id(std::vector<game_data>& g_data, const std::string& game_name)
{
	auto it = std::find(g_data.begin(), g_data.end(), game_name);

	return std::distance(g_data.begin(), it);
}*/

std::pair<std::string, int> game_field_changer(std::vector<game_data>& g_data, const std::string& question)
{
	clear_console();

	std::string game_name;

	printf("Enter the name of the game to be changed: ");
	getline(std::cin, game_name);

	int game_id = search_game_id(g_data, game_name);

	if (game_id > -1)
	{
		std::string temp_variable;

		printf("\nFound game!\n\n");

		printf("Enter the %s: ", question.c_str());
		getline(std::cin, temp_variable);

		return { temp_variable, game_id };
	}

	return { "", game_id };
}

int get_choice_int()
{
	std::string temp;

	getline(std::cin, temp);

	if (temp == "")
	{
		return 0;
	}

	return stoi(temp);
}

int main()
{
	clear_console();

	std::vector<game_data> g_data = read_games_file();
	std::vector<shopping_cart_data> s_data = read_shopping_cart_file();

	display_shop(g_data, s_data);

	write_games_file(g_data);
	write_shopping_cart_file(s_data);

	return 0;
}

void clear_console()
{
	system("cls");
}

void display_games_in_shopping_cart(std::vector<shopping_cart_data>& s_data)
{
	printf("Games in shopping cart: \n");

	for (int i = 0; i < s_data.size(); i++)
	{
		printf("%i. %s \t%i PLN\n", s_data[i].game_id, s_data[i].game_name.c_str(), s_data[i].game_price);
	}
}

void display_shop(std::vector<game_data>& g_data, std::vector<shopping_cart_data>& s_data)
{
	// Handle input
	int choice = -1;

	while(choice != 0)
	{
		// Display header.
		printf("----= GAME SHOP =----\n");

		printf("Choose option:\n\n");
		printf("1. Shop with games (Type 1)\n");
		printf("2. Shopping cart (Type 2)\n");
		printf("3. Admin Menu (Type 3)\n");
		printf("0. Exit (Type 0)\n\n");

		printf("Enter the option: ");
		choice = get_choice_int();

		// Selecting an option
		switch (choice)
		{
			case 0:
			{
				exit(0);
			}
			case 1:
			{
				display_games(g_data, s_data);

				break;
			}
			case 2:
			{
				shopping_cart(g_data, s_data);

				break;
			}
			case 3:
			{
				admin_menu(g_data);

				break;
			}
			// Invalid input.
			default: {
				clear_console();

				// Notify about invalid input.
				printf("Wrong data!\n");

				break;
			}
		}
	}
}

void display_games(std::vector<game_data>& g_data, std::vector<shopping_cart_data>& s_data)
{
	clear_console();

	printf("---= TYPE GAMES =----\n\n");

	// Display all the game types.
	for (int i = 0; i < game_type_names.size(); i++)
	{
		printf("%i. Games %s\n", i + 1, game_type_names[i].c_str());
	}
	printf("\n\n0. Exit\n\n");

	// Get user game-type choice.
	int choice = 0;

	printf("Enter the option: ");
	choice = get_choice_int();

	// Exit option.
	if (choice == 0)
	{
		exit(0);
	}

	// Bugfix.
	choice--;

	if (!is_valid_type(choice))
	{
		clear_console();

		// Notify about invalid input.
		printf("\nWrong data!\n\n");

		// Display the shop again.
		display_games(g_data, s_data);
	}
	choose_game(g_data, s_data, choice);
}

void shopping_cart(std::vector<game_data>& g_data, std::vector<shopping_cart_data>& s_data)
{
	clear_console();

	printf("----= SHOPPING CART =----\n\n");

	display_games_in_shopping_cart(s_data);

	printf("\nDo you want to buy these games?\n");
	printf("1. Yes (Type 1)\n");
	printf("2. No (Type 2)\n\n");

	int choice = 0;

	printf("Choose option: ");
	choice = get_choice_int();

	switch (choice)
	{
		case 1:
		{
			confirm_purchase(g_data, s_data);

			break;
		}
		case 2:
		{
			clear_console();

			printf("You cancel purchase.\n\n");

			display_shop(g_data, s_data);

			break;
		}
		default: {
			clear_console();

			// Notify about invalid input.
			printf("Wrong data!\n");

			break;
		}
	}
}

void choose_game(std::vector<game_data>& g_data, std::vector<shopping_cart_data>& s_data, int type)
{
	clear_console();

	// Display header.
	printf("Available games (%s):\n\n", game_type_names[type].c_str());

	std::vector<int> ids;

	// Display all the games in given type.
	for (int i = 0; i < g_data.size(); i++)
	{
		// Skip the game if it's type is not the chosen one.
		if (g_data[i].type != game_type_names[type].c_str())
		{
			continue;
		}

		printf("%i. %s \t%i PLN (Type %i)\n", i + 1, g_data[i].name.c_str(), g_data[i].price, i + 1);

		ids.push_back(i + 1);
	}

	// Handle input.
	int choice = 0;

	printf("\nEnter the index game: ");
	choice = get_choice_int();

	// Invalid input given.
	if (!is_valid_game_id(choice, ids))
	{
		clear_console();

		printf("\nWrong data!\n\n");

		display_games(g_data, s_data);
	}

	ask_for_add_to_shopping_cart(g_data, s_data, choice - 1);
}

void ask_for_add_to_shopping_cart(std::vector<game_data>& g_data, std::vector<shopping_cart_data>& s_data, int game_id) 
{
	clear_console();

	printf("Do you want to add \"%s\" to shopping cart?\n", g_data[game_id].name.c_str());

	printf("Your balance:\t%.2f\tPLN\n\n", user_money);

	printf("1. Yes (Type 1)\n");
	printf("2. No (Type 2)\n\n");

	// Handle input
	int choice = 0;

	printf("Choose option: ");
	choice = get_choice_int();

	switch (choice)
	{
		case 1:
		{
			add_to_shopping_cart(g_data, s_data, game_id);

			break;
		}
		case 2:
		{
			display_shop(g_data, s_data);

			break;
		}
		default: {
			clear_console();

			// Notify about invalid input.
			printf("Wrong data!\n");

			break;
		}
	}
}

void add_to_shopping_cart(std::vector<game_data>& g_data, std::vector<shopping_cart_data>& s_data, int game_index)
{
	shopping_cart_data scd;

	scd.game_id = game_index;
	scd.game_name = g_data[game_index].name;
	scd.game_price = g_data[game_index].price;

	s_data.push_back(scd);

	clear_console();

	write_shopping_cart_file(s_data);

	display_shop(g_data, s_data);
}

void confirm_purchase(std::vector<game_data>& g_data, std::vector<shopping_cart_data>& s_data)
{
	clear_console();

	printf("Bought games: \n\n");

	for (int i = 0; i < s_data.size(); i++)
	{
		// Charge for the game.
		user_money -= s_data[i].game_price;

		printf("- %s (%i PLN).\n", s_data[i].game_name.c_str(), s_data[i].game_price);
	}
	printf("\nBalance after buy: %.2f PLN.\n\n", user_money);
	printf("Thank you for shopping!\n\n");

	del_from_shopping_cart_file(s_data);

	display_shop(g_data, s_data);
}

void del_from_shopping_cart_file(std::vector<shopping_cart_data>& s_data)
{
	shopping_cart_data scd;

	s_data.erase(s_data.begin(), s_data.end());

	write_shopping_cart_file(s_data);
}

void admin_menu(std::vector<game_data>& g_data)
{
	clear_console();

	printf("----= ADMIN MENU =----:\n");

	printf("1. Add game (Type 1)\n");
	printf("2. Delete game (Type 2)\n");
	printf("3. Change game name (Type 3)\n");
	printf("4. Change game price (Type 4)\n");
	printf("5. Change game type (Type 5)\n\n");
	printf("0. Exit (Type 0)\n\n");

	// Handle input.
	int choice = 0;

	printf("Choose option: ");
	choice = get_choice_int();

	// Selecting an option
	switch (choice)
	{
		case 0:
		{
			break;
		}
		case 1:
		{
			if (ask_client("add game"))
			{
				add_game(g_data);
			}

			break;
		}
		case 2:
		{
			if (ask_client("delete game"))
			{
				del_game(g_data);
			}

			break;
		}
		case 3:
		{
			if (ask_client("change game name"))
			{
				change_game_name(g_data);
			}

			break;
		}
		case 4:
		{
			if (ask_client("change game price"))
			{
				change_game_price(g_data);
			}

			break;
		}
		case 5:
		{
			if (ask_client("change game type"))
			{
				change_game_type(g_data);
			}

			break;
		}
		// Invalid input.
		default: {
			clear_console();

			// Notify about invalid input.
			printf("Wrong data!\n");

			admin_menu(g_data);

			break;
		}
	}
}

void add_game(std::vector<game_data>& g_data)
{
	clear_console();

	game_data gd;

	std::string price;

	printf("Enter the type of game to add: ");
	getline(std::cin, gd.type);

	printf("Enter the name of game to add: ");
	getline(std::cin, gd.name);

	printf("Enter the price of game to add: ");
	getline(std::cin, price);

	gd.price = std::stoi(price);

	g_data.push_back(gd);

	printf("Game added:\n");

	printf("Type: %s\n", gd.type.c_str());
	printf("Name: %s\n", gd.name.c_str());
	printf("Price: %i", gd.price);
}

void del_game(std::vector<game_data>& g_data)
{
	clear_console();

	std::string game_name;

	printf("Enter the name of game to delete: ");
	getline(std::cin, game_name);

	int game_id = search_game_id(g_data, game_name);

	if (game_id > -1)
	{
		g_data.erase(g_data.begin() + game_id);
	}
}

void change_game_name(std::vector<game_data>& g_data)
{
	auto change_data = game_field_changer(g_data, "new name");

	g_data[change_data.second].name = change_data.first;
}

void change_game_type(std::vector<game_data>& g_data)
{
	auto change_data = game_field_changer(g_data, "new type");

	g_data[change_data.second].type = change_data.first;
}

void change_game_price(std::vector<game_data>& g_data)
{
	auto change_data = game_field_changer(g_data, "new price");

	g_data[change_data.second].price = stoi(change_data.first);
}