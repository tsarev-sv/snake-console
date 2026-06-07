#include<iostream>
#include<windows.h>
#include<vector>
#include<conio.h>
#include<cstdlib>
#include<ctime>

using namespace std;

const int HEIGHT = 20;
const int WIDTH = 20;

struct Point {
	int x, y;
	Point(int x_, int y_) : x(x_), y(y_){}
};

Point* food = nullptr;
int score = 0;

int** createField() {
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
	return field;
}

bool checkCollision(vector<Point>& snake) {
	Point newHead = snake[0];
	if (newHead.x == 0 || newHead.x == WIDTH-1 
		|| newHead.y == 0 || newHead.y == HEIGHT-1) {
		return true;
	}
	for (int i = 1; i < snake.size(); i++) {
		if (newHead.x == snake[i].x && newHead.y == snake[i].y) {
			return true;
		}
	}
	return false;
}

void generateFood(vector<Point>& snake) {
	bool validPosition = false;

	while (!validPosition) {
		int randX = rand() % (WIDTH - 2) + 1;
		int randY = rand() % (HEIGHT - 2) + 1;

		bool onSnake = false;
		for (Point p : snake) {
			if (p.x == randX && p.y == randY) {
				onSnake = true;
				break;
			}
		}
		if (!onSnake) {
			if (food == nullptr) {
				food = new Point(randX, randY);
			}
			else {
				food->x = randX;
				food->y = randY;
			}
			validPosition = true;
		}
	}
}

void drawField(int** field) {
	system("cls");
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			if (field[i][j] == 1) {
				cout << '#';
			}
			else if (food != nullptr && i == food->y && j == food->x) {
				cout << 'F';
			}
			else if (field[i][j] == 2) {
				cout << 'O';
			}
			else {
				cout << ' ';
			}
		}
		cout << endl;
	}
	cout << "Score: " << score << endl;
}

int main() {

	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	srand(time(0));

	int** field = createField();

	vector<Point> snake;
	snake.push_back(Point(10,10));
	snake.push_back(Point(9, 10));
	snake.push_back(Point(8, 10));

	generateFood(snake);

	char dir = 'd';

	while (true) {

		Point newHead = snake[0];

		if (_kbhit()) {
			char ch = _getch();
			if (ch == 'w' && dir != 's') dir = ch;
			if (ch == 'a' && dir != 'd') dir = ch;
			if (ch == 's' && dir != 'w') dir = ch;
			if (ch == 'd' && dir != 'a') dir = ch;
			if (ch == 'x' || ch == 'X') break;
		}

		if (dir == 'w') newHead.y--;
		if (dir == 'a') newHead.x--;
		if (dir == 's') newHead.y++;
		if (dir == 'd') newHead.x++;

		snake.insert(snake.begin(), newHead);
		
		bool ateFood = false;
		if (food != nullptr && newHead.x == food->x && newHead.y == food->y) {
			ateFood = true;
			score += 10;
			generateFood(snake);
		}
		if (!ateFood) {
			snake.pop_back();
		}

		if (checkCollision(snake)) {
			cout << "GAME OVER! Score: " << score << endl;
			break;
		}

		for (int i = 0; i < HEIGHT; i++) {
			for (int j = 0; j < WIDTH; j++) {
				if (field[i][j] != 1) {
					field[i][j] = 0;
				}
			}
		}

		for (Point p : snake) {
			if (p.x > 0 && p.x < WIDTH - 1 && p.y > 0 && p.y < HEIGHT - 1) {
				field[p.y][p.x] = 2;
			}
		}

		drawField(field);
		Sleep(120);
	}

	if (food != nullptr) {
		delete food;
		food = nullptr;
	}
	
	for (int i = 0; i < HEIGHT; i++) {
		delete[] field[i];
	}
	field = nullptr;


}