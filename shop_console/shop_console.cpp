#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

float user_money = 1000.0;

class shopping_cart_data
{
public:
	unsigned int game_id;
	unsigned int game_price;

public:
	string game_name;
};

class game_data 
{
public:
	unsigned int price;

public:
	string type;
	string name;
};

const string game_type_names[] =
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

void display_shop(vector<game_data>& g_data, vector<shopping_cart_data>& s_data);
void display_games(vector<game_data>& g_data, vector<shopping_cart_data>& s_data);
void admin_menu(vector<game_data>& g_data);
void shopping_cart(vector<game_data>& g_data, vector<shopping_cart_data>& s_data);
void ask_for_add_to_shopping_cart(vector<game_data>& g_data, vector<shopping_cart_data>& s_data, int game_id);
void add_to_shopping_cart(vector<game_data>& g_data, vector<shopping_cart_data>& s_data, int game_index);
void choose_game(vector<game_data>& g_data, vector<shopping_cart_data>& s_data, int type);
void confirm_purchase(vector<game_data>& g_data, vector<shopping_cart_data>& s_data);
void del_from_shopping_cart_file(vector<shopping_cart_data>& s_data);

//Admin Menu options
void add_game(vector<game_data>& g_data);
void del_game(vector<game_data>& g_data);
void change_game_name(vector<game_data>& g_data);
void change_game_price(vector<game_data>& g_data);
void change_game_type(vector<game_data>& g_data);

bool is_valid_type(int i)
{
	if (i < 0)
	{
		return false;
	}

	int s = *(&game_type_names + 1) - game_type_names;

	if (i > s)
	{
		return false;
	}

	return true;
}

bool is_valid_game_id(int choice, vector<int> ids)
{
	for (int i = 0; i < ids.size(); i++)
	{
		if (ids[i] == choice)
		{
			return true;
		}
	}

	return false;
}

bool ask_client(string question)
{
	cout << "\nDo you want " << question << "?\n" << endl;
	printf_s("1. Yes (Type 1)\n");
	printf_s("2. No (Type 2)\n\n");

	int choice = 0;

	printf_s("Choose option: ");
	choice = get_choice_int();

	return choice == 1;
}

bool write_games_file(vector<game_data>& g_data)
{
	ofstream file("shop_data.dat");

	if (!file.good())
	{
		printf_s("Cannot open file!\n");

		return false;
	}

	for (game_data& game : g_data)
	{
		file << game.type << "#" << game.name << "#" << game.price << endl;
	}

	return true;
}

bool write_shopping_cart_file(vector<shopping_cart_data>& s_data)
{
	ofstream file("shopping_cart_data.dat");

	if (!file.good())
	{
		printf_s("Cannot open file!\n");

		return false;
	}

	for (shopping_cart_data& shopping_cart : s_data)
	{
		file << shopping_cart.game_id << "#" << shopping_cart.game_name << "#" << shopping_cart.game_price << endl;
	}

	return true;
}

vector<game_data> read_games_file()
{
	ifstream stream("shop_data.dat", ios::out);

	if (!stream.good())
	{
		printf_s("Cannot open file!\n");

		return {};
	}

	vector<game_data> g_data;

	game_data game;

	string price;

	while (getline(stream, game.type, '#') && getline(stream, game.name, '#') && getline(stream, price))
	{
		game.price = stoi(price);
		g_data.push_back(game);
	}

	/*for (game_data& game : g_data)
	{
		cout << "TYPE: " << game.type << " NAME: " << game.name << " PRICE: " << game.price << endl;
	}*/
	
	stream.close();

	return g_data;
}

vector<shopping_cart_data> read_shopping_cart_file()
{
	ifstream stream("shopping_cart_data.dat", ios::out);

	if (!stream.good())
	{
		printf_s("Cannot open file!\n");

		return {};
	}

	vector<shopping_cart_data> s_data;

	shopping_cart_data shopping_cart;

	string price,
		game_index;

	while (getline(stream, game_index, '#') && getline(stream, shopping_cart.game_name, '#') && getline(stream, price))
	{
		shopping_cart.game_id = stoi(game_index);
		shopping_cart.game_price = stoi(price);

		s_data.push_back(shopping_cart);
	}

	/*for (shopping_cart_data& shopping_cart : s_data)
	{
		cout << "INDEX: " << shopping_cart.game_id << " NAME: " << shopping_cart.game_name << " PRICE: " << shopping_cart.game_price << endl;
	}*/

	stream.close();

	return s_data;
}

int search_game_id(vector<game_data>& g_data, const string& game_name)
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

pair<string, int> game_field_changer(vector<game_data>& g_data, const string& question)
{
	clear_console();

	string game_name;

	printf_s("Enter the name of the game to be changed: ");
	getline(cin, game_name);

	int game_id = search_game_id(g_data, game_name);

	if (game_id > -1)
	{
		string temp_variable;

		printf_s("\nFound game!\n\n");

		printf_s("Enter the %s: ", question.c_str());
		getline(cin, temp_variable);

		return { temp_variable, game_id };
	}

	return { "", game_id };
}

int get_choice_int()
{
	string temp;

	getline(cin, temp);

	if (temp == "")
	{
		return 0;
	}

	return stoi(temp);
}

int main()
{
	clear_console();

	vector<game_data> g_data = read_games_file();
	vector<shopping_cart_data> s_data = read_shopping_cart_file();

	display_shop(g_data, s_data);

	write_games_file(g_data);
	write_shopping_cart_file(s_data);

	return 0;
}

void clear_console()
{
	system("cls");
}

void display_games_in_shopping_cart(vector<shopping_cart_data>& s_data)
{
	printf("Games in shopping cart: \n");

	for (int i = 0; i < s_data.size(); i++)
	{
		printf("%i. %s\t%i PLN\n", s_data[i].game_id, s_data[i].game_name.c_str(), s_data[i].game_price);
	}
}

void display_shop(vector<game_data>& g_data, vector<shopping_cart_data>& s_data)
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

void display_games(vector<game_data>& g_data, vector<shopping_cart_data>& s_data)
{
	clear_console();

	printf("---= TYPE GAMES =----\n\n");

	// Size of game_type_names array. Pointer hack.
	int s = *(&game_type_names + 1) - game_type_names;

	// Display all the game types.
	for (int i = 0; i < s; i++)
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

void shopping_cart(vector<game_data>& g_data, vector<shopping_cart_data>& s_data)
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

void choose_game(vector<game_data>& g_data, vector<shopping_cart_data>& s_data, int type)
{
	clear_console();

	// Display header.
	printf("Available games (%s):\n\n", game_type_names[type].c_str());

	vector<int> ids;

	// Display all the games in given type.
	for (int i = 0; i < g_data.size(); i++)
	{
		// Skip the game if it's type is not the chosen one.
		if (g_data[i].type != game_type_names[type].c_str())
		{
			continue;
		}

		printf("%i. %s\t%i PLN (Type %i)\n", i + 1, g_data[i].name.c_str(), g_data[i].price, i + 1);

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

void ask_for_add_to_shopping_cart(vector<game_data>& g_data, vector<shopping_cart_data>& s_data, int game_id) 
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

void add_to_shopping_cart(vector<game_data>& g_data, vector<shopping_cart_data>& s_data, int game_index)
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

void confirm_purchase(vector<game_data>& g_data, vector<shopping_cart_data>& s_data)
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

void del_from_shopping_cart_file(vector<shopping_cart_data>& s_data)
{
	shopping_cart_data scd;

	s_data.erase(s_data.begin(), s_data.end());

	write_shopping_cart_file(s_data);
}

void admin_menu(vector<game_data>& g_data)
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

void add_game(vector<game_data>& g_data)
{
	clear_console();

	game_data gd;

	string price;

	printf_s("Enter the type of game to add: ");
	getline(cin, gd.type);

	printf_s("Enter the name of game to add: ");
	getline(cin, gd.name);

	printf_s("Enter the price of game to add: ");
	getline(cin, price);

	gd.price = stoi(price);

	g_data.push_back(gd);

	printf_s("Game added:\n");

	cout << "Type: " << gd.type << endl;
	cout << "Name: " << gd.name << endl;
	cout << "Price: " << gd.price << endl;
}

void del_game(vector<game_data>& g_data)
{
	clear_console();

	string game_name;

	printf_s("Enter the name of game to delete: ");
	getline(cin, game_name);

	int game_id = search_game_id(g_data, game_name);

	if (game_id > -1)
	{
		g_data.erase(g_data.begin() + game_id);
	}
}

void change_game_name(vector<game_data>& g_data)
{
	auto change_data = game_field_changer(g_data, "new name");

	g_data[change_data.second].name = change_data.first;
}

void change_game_type(vector<game_data>& g_data)
{
	auto change_data = game_field_changer(g_data, "new type");

	g_data[change_data.second].type = change_data.first;
}

void change_game_price(vector<game_data>& g_data)
{
	auto change_data = game_field_changer(g_data, "new price");

	g_data[change_data.second].price = stoi(change_data.first);
}