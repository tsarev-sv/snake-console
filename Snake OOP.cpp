#include<iostream>
#include<conio.h>
#include<vector>
#include<cstdlib>
#include<ctime>
#include<windows.h>
#include<fstream>

using namespace std;

const int HEIGHT = 20;
const int WIDTH = 20;

class Point {
public:
	int x, y;
	Point(int x_ = 0,int y_ = 0): x(x_),y(y_){}
};

class RecordManager {
private:
	string filename;
public:
	RecordManager(string file) : filename(file){}

	void saveRecord(int score,string Player = "player") {
		int OldRecord = LoadRecord();
		if (score > OldRecord) {
			ofstream file(filename);
			if (file.is_open()) {
				file << Player << " " << score;
			}
		}
	}

	int LoadRecord() {
		ifstream file(filename);
		int score = 0;
		string name;
		if (file.is_open()) {
			file >> name >> score;
		}
		return score;
	}
};

class Snake {
private:
	int score;
	char direction;
	char symbol;
	vector<Point>body;
public:
	Snake(int PointX, int PointY, char sym, char dir = 'd')
		: score(0), symbol(sym), direction(dir) {
		body.push_back(Point(PointX, PointY));
		body.push_back(Point(PointX-1, PointY));
		body.push_back(Point(PointX-2, PointY));
	}

	void move() {
		Point newHead = body[0];
		switch (direction) {
		case 'w':newHead.y--; break;
		case 'a':newHead.x--; break;
		case 's':newHead.y++; break;
		case 'd':newHead.x++; break;
		}
		body.insert(body.begin(), newHead);
		body.pop_back();
	}

	void setDirection(char newDir) {
		if ((newDir == 'w' && direction != 's') ||
			(newDir == 's' && direction != 'w') ||
			(newDir == 'a' && direction != 'd') ||
			(newDir == 'd' && direction != 'a')) {
			direction = newDir;
		}
	}

	void grow() {
		Point tail = body.back();
		body.push_back(tail);
		score += 10;
	}

	bool CollisionWithSelf() {
		Point newHead = body[0];
		for (size_t i = 1; i < body.size(); i++) {
			if (newHead.x == body[i].x && newHead.y == body[i].y) {
				return true;
			}
		}
		return false;
	}

	bool CollisionWithWall() {
		Point newHead = body[0];
		if (newHead.x == 0 || newHead.x == WIDTH - 1 || newHead.y == 0 || newHead.y == HEIGHT - 1) {
			return true;
		}
		return false;
	}

	bool CollisionWith(Snake& other) {
		Point newHead = body[0];
		for (Point p : other.getBody()) {
			if (newHead.x == p.x && newHead.y == p.y) {
				return true;
			}
		}
		return false;
	}

	const int getScore() const { return score; }
	const Point getHead() const { return body[0]; }
	const vector<Point> getBody() const { return body; }
	const char getSymbol() const { return symbol; }

};

class Game {
private:
	Snake snake1;
	Snake snake2;
	Point food;
	bool gameOver;
	int mode;
	int level;
	int subMode;
	RecordManager records;

	void generateFoodTwo() {
		bool validPosition = false;
		while (!validPosition) {
			int randX = rand() % (WIDTH - 2) + 1;
			int randY = rand() % (HEIGHT - 2) + 1;
			bool onSnake1 = false, onSnake2 = false;
			for (Point p : snake1.getBody()) {
				if (p.x == randX && p.y == randY) {
					onSnake1 = true;
				}
			}
			for (Point p : snake2.getBody()) {
				if (p.x == randX && p.y == randY) {
					onSnake2 = true;
				}
			}
			if (!onSnake1 && !onSnake2) {
				food = Point(randX, randY);
				validPosition = true;
			}
		}
	}

	void generateFoodSingle() {
		bool validPosition = false;
		while (!validPosition) {
			int randX = rand() % (WIDTH - 2) + 1;
			int randY = rand() % (HEIGHT - 2) + 1;
			bool onSnake = false;
			for (Point p : snake1.getBody()) {
				if (p.x == randX && p.y == randY) {
					onSnake = true;
				}
			}
			if (!onSnake) {
				food = Point(randX, randY);
				validPosition = true;
			}
		}
	}

	void handleInput() {
		if(_kbhit()) {
			int ch = _getch();
			if (ch == 'w')snake1.setDirection('w');
			if (ch == 'a')snake1.setDirection('a');
			if (ch == 's')snake1.setDirection('s');
			if (ch == 'd')snake1.setDirection('d');
			if (mode == 2) {
				if (ch == 0 || ch == 224) {
					int key = _getch();
					if (key == 72)snake2.setDirection('w');
					if (key == 80)snake2.setDirection('s');
					if (key == 75)snake2.setDirection('a');
					if (key == 77)snake2.setDirection('d');
				}
			}
			if (ch == 'x' || ch == 'X') gameOver = true;
			if (ch == 'p' || ch == 'P') {
				cout << "Press p to continue..." << endl;
				char keyPause;
				do {
					keyPause = _getch();
				} while (keyPause != 'p' && keyPause != 'P');
				
				while (_kbhit()) {
					_getch();
				}
			}
		}
	}

	void update() {
		snake1.move();

		Point head1 = snake1.getHead();
		if (mode == 1) {
			if (head1.x == food.x && head1.y == food.y) {
				snake1.grow();
				generateFoodSingle();
			}
		}
		else {
			if (head1.x == food.x && head1.y == food.y) {
				snake1.grow();
				generateFoodTwo();
			}
		}

		if (mode == 2) {
			snake2.move();
			Point head2 = snake2.getHead();
			if (head2.x == food.x && head2.y == food.y) {
				snake2.grow();
				generateFoodTwo();
			}
			if (snake2.CollisionWithSelf() || snake2.CollisionWithWall()) {
				gameOver = true;
			}
			if (snake2.CollisionWith(snake1) || snake1.CollisionWith(snake2)) {
				gameOver = true;
			}
		}
		if (snake1.CollisionWithSelf() || snake1.CollisionWithWall()) {
			gameOver = true;
		}	
	}

	void drawSplitScreen() {
		system("cls");
		int** field1 = new int* [HEIGHT];
		for (int i = 0; i < HEIGHT; i++) {
			field1[i] = new int[WIDTH];
			for (int j = 0; j < WIDTH; j++) {
				if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1) {
					field1[i][j] = 1;
				}
				else {
					field1[i][j] = 0;
				}
			}
		}
		for (Point p : snake1.getBody()) {
			if (p.x > 0 && p.x < WIDTH - 1 && p.y> 0 && p.y < HEIGHT - 1) {
				field1[p.y][p.x] = 2;
			}
		}
		if (food.x > 0 && food.x < WIDTH - 1 && food.y > 0 && food.y < HEIGHT - 1) {
			field1[food.y][food.x] = 4;
		}
		int** field2 = new int* [HEIGHT];
		for (int i = 0; i < HEIGHT; i++) {
			field2[i] = new int[WIDTH];
			for (int j = 0; j < WIDTH; j++) {
				if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1) {
					field2[i][j] = 1;
				}
				else {
					field2[i][j] = 0;
				}
			}
		}
		for (Point p : snake2.getBody()) {
			if (p.x > 0 && p.x < WIDTH - 1 && p.y> 0 && p.y < HEIGHT - 1) {
				field2[p.y][p.x] = 3;
			}
		}
		if (food.x > 0 && food.x < WIDTH - 1 && food.y > 0 && food.y < HEIGHT - 1) {
			field2[food.y][food.x] = 4;
		}

		cout << "===== PLAYER1 (WASD) Score: " << snake1.getScore()
			<< "          ===== PLAYER2 (Arrows) Score: " << snake2.getScore() << " =====" << endl;
		for (int i = 0; i < HEIGHT; i++) {
			for (int j = 0; j < WIDTH; j++) {
				if (field1[i][j] == 1) {
					cout << '#';
				}
				else if (field1[i][j] == 2) {
					cout << snake1.getSymbol();
				}
				else if (field1[i][j] == 4) {
					cout << 'F';
				}
				else {
					cout << ' ';
				}
			}
			cout << "         ";
			for (int j = 0; j < WIDTH; j++) {
				if (field2[i][j] == 1) {
					cout << '#';
				}
				else if (field2[i][j] == 3) {
					cout << snake2.getSymbol();
				}
				else if (field2[i][j] == 4) {
					cout << 'F';
				}
				else {
					cout << ' ';
				}
			}
			cout << endl;
		}
		for (int i = 0; i < HEIGHT; i++) {
			delete[] field1[i];
			delete[] field2[i];
		}
		delete[] field1;
		delete[] field2;

		field1 = nullptr;
		field2 = nullptr;
	}

	void drawSingleField() {
		system("cls");
		int** field = new int* [HEIGHT];
		for (int i = 0; i < HEIGHT; i++) {
			field[i] = new int[WIDTH];
			for (int j = 0; j < WIDTH; j++) {
				if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1) {
					field[i][j] = 1;
				}
				else {
					field[i][j] = 0;
				}
			}
		}
		for (Point p : snake1.getBody()) {
			if (p.x > 0 && p.x < WIDTH - 1 && p.y > 0 && p.y < HEIGHT - 1) {
				field[p.y][p.x] = 2;
			}
		}
		if (food.x > 0 && food.x < WIDTH - 1 && food.y > 0 && food.y < HEIGHT - 1) {
			field[food.y][food.x] = 4;
		}
		if (mode == 2) {
			for (Point p : snake2.getBody()) {
				if (p.x > 0 && p.x < WIDTH - 1 && p.y > 0 && p.y < HEIGHT - 1) {
					field[p.y][p.x] = 3;
				}
			}
		}
		if (mode == 2) {
			cout << "===== PLAYER1 (WASD) Score: " << snake1.getScore()
				<< "           ===== PLAYER2 (Arrows) Score: " << snake2.getScore() << " =====" << endl;
		}
		else {
			cout << "===== PLAYER1 (WASD) Score: " << snake1.getScore() << " =====" << endl;
		}

		for (int i = 0; i < HEIGHT; i++) {
			for (int j = 0; j < WIDTH; j++) {
				if (field[i][j] == 1) {
					cout << '#';
				}
				else if (field[i][j] == 2) {
					cout << snake1.getSymbol();
				}
				else if (field[i][j] == 3) {
					cout << snake2.getSymbol();
				}
				else if (field[i][j] == 4) {
					cout << 'F';
				}
				else {
					cout << ' ';
				}
			}
			cout << endl;
		}
		for (int i = 0; i < HEIGHT; i++) {
			delete[] field[i];
		}
		delete[] field;
		field = nullptr;
	}

	const int getDelay() const {
		switch (level) {
		case 1:return 150;
		case 2:return 60;
		case 3:return 10;
		default:return 100;
		}
	}

	public:
		Game(int gameMode, int difficulty, int split = 1)
			: snake1(10, 10, 'O'),
			snake2(10, 15, 'X'),
			gameOver(false),
			mode(gameMode),
			level(difficulty),
			subMode(split),
			records("snake-record.txt") {
			if (mode == 1) {
				generateFoodSingle();
			}
			else {
				generateFoodTwo();
			}
		}

		void run() {
			while (!gameOver) {
				handleInput();
				update();

				if (mode == 2 && subMode == 2) {
					drawSplitScreen();
				}
				else {
					drawSingleField();
				}
				Sleep(getDelay());
			}
			cout << "===== GAME OVER =====" << endl;
			if (mode == 2) {
				cout << "PLAYER1 Score: " << snake1.getScore() << endl;
				cout << "PLAYER2 Score: " << snake2.getScore() << endl;
				if (snake2.getScore() > snake1.getScore()) {
					cout << "PLAYER2 WINS" << endl;
				}
				if(snake1.getScore() > snake2.getScore()){
					cout << "PLAYER1 WINS" << endl;
				}
			}
			else {
				cout << "Your score: " << snake1.getScore() << endl;
				records.saveRecord(snake1.getScore());
				cout << "Top record: " << records.LoadRecord() << endl;
			}
			cout << "Press any key to return menu..." << endl;
			_getch();
		}
};

void showMenu() {
	system("cls");
	cout << "====================" << endl;
	cout << "     SNAKE GAME" << endl;
	cout << "====================" << endl;
	cout << "1. One player" << endl;
	cout << "2. Two players" << endl;
	cout << "3. Exit" << endl;
	cout << "====================" << endl;
}

void showDifficultyMenu() {
	system("cls");
	cout << "====================" << endl;
	cout << "     DIFFICULTY" << endl;
	cout << "====================" << endl;
	cout << "1. Easy (slow)" << endl;
	cout << "2. Normal" << endl;
	cout << "3. Hard (fast)" << endl;
	cout << "====================" << endl;
}

void showTwoPlayersMenu() {
	system("cls");
	cout << "====================" << endl;
	cout << "     TWO PLAYERS" << endl;
	cout << "====================" << endl;
	cout << "1. Same field" << endl;
	cout << "2. Split screen" << endl;
	cout << "====================" << endl;
}

int main() {
	srand(time(0));

	while (true) {
		showMenu();
		int choice = 0;
		while (!(cin >> choice) || choice < 1 || choice > 3) {
			cout << "Please enter a valid number (1,2,3): " << endl;
			cin.clear();
			cin.ignore(1000, '\n');
		}
		if (choice == 3) {
			cout << "Goodbye!";
			break;
		}
		if (choice == 1 || choice == 2) {
			showDifficultyMenu();
			int diff = 0;
			while (!(cin >> diff) || diff < 1 || diff > 3) {
				cout << "Please enter a valid number (1,2,3): " << endl;
				cin.clear();
				cin.ignore(1000, '\n');
			}
			if (choice == 1) {
				Game game(1, diff);
				game.run();
			}
			else {
				showTwoPlayersMenu();
				int subMode = 0;
				while (!(cin >> subMode) || subMode < 1 || subMode > 2) {
					cout << "Please enter a valid number (1,2): " << endl;
					cin.clear();
					cin.ignore(1000, '\n');
				}
				Game game(2, diff, subMode);
				game.run();
			}
		}
	}
}