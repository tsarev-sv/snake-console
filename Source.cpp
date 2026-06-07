#include<iostream>
#include<fstream>
#include<windows.h>
#include<string>
#include<vector>
#include<climits>
#include<algorithm>

using namespace std;

class Bankaccount {
private:
// Поля класса (остальные поля класса аналогично)
	string numbercard;
	int pincode;
	double balance;
	string name;
	vector<string>operations;
	bool isBlocked;
	int failedAttempts;

public:
// Конструктор: инициализируем поля класса (остальные конструкторы аналогично)
	Bankaccount(string Name, int Pincode, double Balance, string Cardnumber) : name(Name) , pincode(Pincode), balance(Balance),
		numbercard(Cardnumber), isBlocked(false), failedAttempts(0) {}
	//Пользовательская функция: сохраняет данные в account.txt (остальные пользовательские функции аналогично)
	void saveToFile() const {
		// ofstream для записи в файл
		ofstream file("account.txt");
		// Если файл открылся успешно
		if (file.is_open()) {
			// Записываем основные данные (Порядок как в конструкторе)
			file << name << "|" << pincode << "|" << balance << "|" << numbercard << endl;
			// Проходим по всем элементам вектора
			for (const auto& op : operations) {
				file << op << endl;
			}
			// Закрываем файл
			file.close();
		}
	}
	// Читаем данные из файла 
	void loadFromFile() {
		// ifstream для чтения из файла
		ifstream file("account.txt");
		// Если файла нет, выход из функции
		if (!file.is_open()) return;

		string line;
		
		// Разбиваем первую строку (основные данные)
		getline(file, line);

		// Находим разделитель и присваиваем ему позицию
		size_t pos1 = line.find("|");
		// Ищем символ | начиная с позиции после pos1
		size_t pos2 = line.find("|", pos1 + 1);
		// Ищем символ | начиная с позиции после pos2
		size_t pos3 = line.find("|", pos2 + 1);

		// Берём подстроку с позиции 0 длиной pos1 
		name = line.substr(0, pos1);
		// Берём подстроку между pos1 и pos2 и преобразуем строку в число
		pincode = stoi(line.substr(pos1 + 1, pos2 - pos1));
		// Берём подстроку между pos2 и pos3 и преобразуем строку в число с плавающей точкой
		balance = stod(line.substr(pos2 + 1, pos3 - pos2 - 1));
		// Берём подстроку от pos3 + 1 до конца строки
		numbercard = line.substr(pos3 + 1);

		// Удаляем старые операции если они были
		operations.clear();
		// Читаем строки из файла пока есть и добавляем в вектор
		while (getline(file, line)) {
			operations.push_back(line);
		}
		// Закрываем файл
		file.close();
	}

	// Геттер: возвращаем значения приватных полей (остальные геттеры аналогично)
	double getBalance() const {
		return balance;
	}
	double getPincode() const {
		return pincode;
	}
	string getName() const {
		return name;
	}
	string getNumbercard() const {
		return numbercard;
	}
	bool getisBlocked() const {
		return isBlocked;
	}
	// Сеттер: меняет  значения приватных  полей класса (остальные сеттеры аналогично)
	void setBalance(double  newbalance) {
		balance = newbalance;
	}
	//  Добавляем в вектор значения  по  ссылке
	void addOperations(const string& operation)  {
		operations.push_back(operation);
		if (operations.size() > 10) {
			operations.erase(operations.begin());
		}
	}
	// Возвращаем значения приватных полей
	vector<string> getOperations() const{
		return operations;
	}
	// Если карта заблокирована, то добавляем сообщение в список операций
	void BlockCard() {
		if (!isBlocked) { // Если карта не заблокирована
			isBlocked = true; // isBlocked = true (заблокирована)
			addOperations("Карта заблокирована из-за 3-х неудачных попыток ввести PIN код");
			saveToFile();
		}
	}
	// Обнуляем количество попыток
	void  resetFailedAttempts() {
		failedAttempts = 0;
	}
	// Возвращает значение приватного поля failedAttempts;
	int getFailedAttempts() const {
		return failedAttempts;
	}
	// Проверка карты на блокировку
	void  IncrementFailedAttempts() {
		// Если isBlocked = true
		if (isBlocked) {
			return;
		}
		// Если isBlocked = false
		failedAttempts++;
		if (failedAttempts == 3) {
			BlockCard();
		}
	}
};

class ATMcontroller {
private:
	Bankaccount* account;
	bool isAvtorisovan;
public:
	// Передача методов объекта типа Bankaccount, инициализация поля класса
	ATMcontroller(Bankaccount* acc) : account(acc), isAvtorisovan(false) {}

	// Сравнение введенного номера  карты с номером карты объекта
	bool CheckNumbercard(const string& Cardnumber) const {
		if (!account) return  false;
		return account->getNumbercard() == Cardnumber;
	}
	// Возвращаем true, если есть объект и карта не заблокирована
	bool isCardActive() const {
		return account && !account->getisBlocked();
	}
	// Возвращает количество возможных попыток
	int getRemainingAttempts() const {
		if (!account || account->getisBlocked()) return 0;
		return 3 - account->getFailedAttempts();
	}
	// Возвращаем имя пользователя из объекта account
	string getUserName() const {
		if (!account) return "Неизвестно";
		return account->getName();
	}
	// Проверка пинкода
	bool CheckPin(int pin) {
		//  Если  указателя на объекта нет  
		if (!account) {
			cout << "Ошибка! Счет не найден" << endl;
			return  false;
		} // Если карта заблокирована
		if (account->getisBlocked()) {
			cout << "Карта заблокирована! Обратитесь в банк." << endl;
			return false;
		}
		// Если пинкод правильный то, успешная авторизация  и выход на главное меню
		if (account->getPincode() == pin) {
			account->resetFailedAttempts(); // Обнуление счетчика попыток
			isAvtorisovan = true; //Состояние: Авторизован = true
			cout << "Вы успешно авторизованы! С возвращением, " << getUserName() << "!" << endl;
			return true;
		}

		else {
			// Иначе увеличиваем счетчик попыток
			account->IncrementFailedAttempts();
			int attemptsLeft = 3 - account->getFailedAttempts();
			// Если попытки  есть 
			if (attemptsLeft > 0) {
				cout << "Неверный PIN! Попыток осталось: " << attemptsLeft << "из 3: " << endl;
			}
			// Если попыток не осталось  
			else {
				cout << "Карта заблокирована! Обратитесь в банк." << endl;
			}
			return  false;
		}
	}
	// Проверка авторизован ли пользователь
	bool authorize(const string& Cardnumber, int pin, string& message) {
		// Если нет счета
		if (!account) {
			message = "Ошибка: Счёт не найден!";
			return false;
		}
		// Если карта не найдена
		if (!CheckNumbercard(Cardnumber)) {
			message = "Ошибка: Карта не найдена!";
			return false;
		}
		// Проверка карты на  блокировку
		if (account->getisBlocked()) {
			message = "Карта заблокирована! Превышено количество попыток! Обратитесь в банк.";
			return false;
		}
		// Если пикод правильный, то успешная авторизация
		if (CheckPin(pin)) {
			message = "Авторизация прошла успешно! Добро пожаловать, " + account->getName();
			return true;
		}
		// Если попытка неудачная (неверный PIN)
		int attemptsLeft = getRemainingAttempts();
		message = "Неверный PIN! Осталось попыток: " + to_string(attemptsLeft);
		return  false;
	}
	// Совершаем выход из программы
	void logout() {
		isAvtorisovan = false;
	}
	// Операция снять наличные
	bool snyat(int summa1) {
		// Если счёта нет или пользователь не авторизован
		if (!account || !isAvtorisovan) {
			return false;
		}
		// Если счёт заблокирован
		if (account->getisBlocked()) {
			cout << "Ошибка! Ваш счёт заблокирован" << endl;
			return false;
		}
		// Если сумма отрицательная или равна 0
		if (summa1<=0) {
			cout << "Ошибка! Сумма не может быть отрицательной" << endl;
			return false;
		}
		// Если сумма не кратна 100
		if (summa1 % 100 != 0){
			cout << "Ошибка! Сумма должна быть кратна 100" << endl;
			return false;
		}
		// Если сумма снятия больше суммы баланса
		if (summa1 > account->getBalance()) {
			cout << "Ошибка! Сумма снятия больше суммы баланса";
			return false;
		}
		// В остальных случаях совершаем операцию снятия и добавляем её в список совершенных операций
		account->setBalance(account->getBalance() - summa1);
		account->addOperations("Снятие: " + to_string(summa1) + " руб.");
		account->saveToFile();
		return true;
	}
	// Операция внести наличные
	bool vnesti(int summa2) {
		// Если счёта нет или пользователь не авторизован
		if (!account || !isAvtorisovan) {
			return false;
		}
		// Если счёт заблокирован
		if (account->getisBlocked()) {
			cout << "Ошибка! Ваш счёт заблокирован" << endl;
			return false;
		}
		// Если сумма отрицательная или равна 0
		if (summa2 <= 0) {
			cout << "Сумма должна быть положительной!" << endl;
			return false;
		}
		// Если сумма корректная, то выполняем операцию и добавляем её в список операций
		account->setBalance(account->getBalance() + summa2);
		account->addOperations("Внесение: "  +  to_string(summa2) + " руб." );
		account->saveToFile();
		return true;
	}
	// Операция перевод по номеру карты
	bool perevesti(int summa3) {
		// Если счёта нет или пользователь не авторизован
		if (!account || !isAvtorisovan) {
			return false;
		}
		// Если счёт заблокирован
		if (account->getisBlocked()) {
			cout << "Ошибка! Ваш счёт заблокирован" << endl;
			return false;
		}
		// Если сумма отрицательная или равна 0
		if (summa3 <= 0) {
			cout << "Сумма должна быть положительной!" << endl;
			return false;
		}
		// Если сумма перевода больше суммы баланса
		if (summa3 > account->getBalance()) {
			cout << "Сумма перевода больше суммы баланса" << endl;
			return false;
		}
		// Если сумма корректная, то выполняем операцию и добавляем её в список операций
		account->setBalance(account->getBalance() - summa3);
		account->addOperations("Перевод: " + to_string(summa3) + " руб.");
		account->saveToFile();
		return true;
	}
	// Возвращает баланс через указатель на объект account
	double getBalance() const {
		return account->getBalance();
	}
	// Возвращает копию списка операций через указатель на объект account
	vector<string> getOperations() const{
		return account->getOperations();
	}
};

class ATMInterface {
private:
	ATMcontroller* Atm;
	// Обновляем флаг буфера ввода и очищаем буфер до переноса на следующую строку
	void clearBuffer() {
		cin.clear();
		cin.ignore(INT_MAX, '\n');
	}
	// Выводим главное меню
	void showmenu() {
		cout << "===== ГЛАВНОЕ МЕНЮ =====" ;
		cout << "\n1. Показать текущий баланс\n" <<"2. Снять наличные\n";
		cout << "3. Внести наличные\n" << "4. Перевести деньги другому пользователю\n";
		cout << "5. Показать последние 5-10 операций\n" << "6. Выйти\n";
	}
	// Авторизация пользователя
	bool authorize() {
		const int ATTEMPTS_MAX = 3;
		int attempts = 0;

		cout << "\n=============== АВТОРИЗАЦИЯ ================\n";
		cout << "======= ДОБРО ПОЖАЛОВАТЬ В БАНКОМАТ! =======\n";
		cout << "============================================\n";

		// Пока количество  попыток не использовано
		while (attempts < ATTEMPTS_MAX) {
			cout << "\nПопытка: " << (attempts + 1) << " из " << ATTEMPTS_MAX << endl;

			string cardnumber;
			cout << "\nВведите номер карты (16 цифр): ";
			cin >> cardnumber;

			// Если длина номера карты не 16 или не все символы карты цифры
			if (cardnumber.length() != 16 || !all_of(cardnumber.begin(), cardnumber.end(), ::isdigit)) {
				cout << "Ошибка! Неверный формат номера карты" << endl;
				// Увеличиваем количество попыток и пишем continue, чтобы не пришлось вводить PIN-код
				attempts++;
				continue;
			}

			int pin;
			cout << "Введите PIN-код: ";
			cin >> pin;

			// Авторизация пользователя и вывод  сообщения
			string message;
			if (Atm->authorize(cardnumber, pin, message)) {
				cout << "\n " << message << endl;
				return true;
			}
			else {
				cout << "\n" << message << endl;
				attempts++;

				if (attempts < ATTEMPTS_MAX) {
					cout << "Осталось попыток " << (ATTEMPTS_MAX - attempts) << endl;
				}
			}
		}
		cout << "\n Превышено количество попыток!" << endl;
		return false;
	}
public:
	// Передача методов объекта типа ATMcontroller, инициализация поля класса
	ATMInterface(ATMcontroller* atm) : Atm(atm){}
	// При корректной  авторизации выводит меню
	void run() {

		if (!authorize()) {
			cout << "\nАвторизация не пройдена.";
			return;
		}

		int y;
		bool running = true;
		// Пока running = true, выводим меню
		while (running) {
			showmenu();
			// Проверка на дурака: если ввели не цифру
			while (!(cin >> y)) {
				cout << "Ошибка! Введите число от 1 до 6: ";
				// Обновляем флаг буфера ввода и очищаем буфер до переноса на следующую строку
				cin.clear();
				cin.ignore(INT_MAX, '\n');
			}

			// switch-case: конструкция множественного выбора
			switch (y) {
				// 1.Вывод баланса
			case 1:
				cout << "Ваш баланс: " << Atm->getBalance() << " руб." << endl;
				break;
				// 2.Снятие наличных
			case 2: {
				cout << "\n==СНЯТИЕ НАЛИЧНЫХ==\n";
				int summa;
				cout << "Введите сумму: " << endl;
				// Проверка на корректный ввод
				if(!(cin >> summa)) {
					cout << "Ошибка! Введите целое число.";
					cin.clear();
					cin.ignore(INT_MAX, '\n');
					break;
				}
				// Заглядываем в поток ввода и если не видем '\n', то очищаем  буфер потока ввода
				if (cin.peek() != '\n') {
					cout << "Ошибка! Введите целое число (без точек и букв)";
					cin.ignore(INT_MAX, '\n');
					break;
				}
				if (Atm->snyat(summa)) {
					cout << "Операция прошла успешно! Ваш баланс: " << Atm->getBalance() << " руб." << endl;
				}
				else {
					cout << "Ошибка! Не удалось снять деньги." << endl;
				}
			}
				  break;
				  //3.Внести наличные
			case 3: {
				cout << "\n==ВНЕСТИ НАЛИЧНЫЕ==\n";
				int summa2;
				cout << "Введите сумму: " << endl;
				if (!(cin >> summa2)) {
					cout << "Ошибка! Введите целое число.";
					cin.clear();
					cin.ignore(INT_MAX, '\n');
					break;
				}
				// Заглядываем в поток ввода и если не видем '\n', то очищаем  буфер потока ввода
				if (cin.peek() != '\n') {
					cout << "Ошибка! Введите целое число (без точек и букв)";
					cin.ignore(INT_MAX, '\n');
					break;
				}
				if (Atm->vnesti(summa2)) {
					cout << "Операция прошла успешно! Ваш баланс: " << Atm->getBalance() << " руб." << endl;
				}
				else {
					cout << "Ошибка! Не удалось внести деньги." << endl;
				}
			}
				  break;
				  //4.Перевод другому пользователю по номеру карты
			case 4: {
				cout << "\n==ПЕРЕВОД ДРУГОМУ ПОЛЬЗОВАТЕЛЮ==\n";
				string usercard;
				cout << "Введите номер карты  получателя: " << endl;
				cin >> usercard;

				if (usercard.length() != 16 ||
					!all_of(usercard.begin(), usercard.end(), ::isdigit)) {
					cout << "Карта не содержит 16 цифр" << endl;
				}
				else {
					int summa3;
					cout << "Введите сумму для перевода: " << endl;
					if (!(cin >> summa3)) {
						cout << "Ошибка! Введите целое число.";
						cin.clear();
						cin.ignore(INT_MAX, '\n');
						break;
					}
					// Заглядываем в поток ввода и если не видем '\n', то очищаем  буфер потока ввода
					if (cin.peek() != '\n') {
						cout << "Ошибка! Введите целое число (без точек и букв)";
						cin.ignore(INT_MAX, '\n');
						break;
					}

					if (Atm->perevesti(summa3)) {
						cout << "Перевод по карте " << usercard << " на сумму " << summa3 << " руб. прошел успешно." << endl;
						cout << "Ваш баланс: " << Atm->getBalance();
					}
					else {
						cout << "Ошибка! Не удалось совершить перевод.";
					}
				}
				break;
			}
				  //5.Список операций
			case 5: {
				cout << "\n==ПОСЛЕДНИЕ ОПЕРАЦИИ==\n";
				vector<string> operations = Atm->getOperations();

				if (operations.empty()) {
					cout << "Операций пока нет." << endl;
				}
				else {
					cout << "Ваши последние операции:" << endl;
					for (size_t i = 0; i < operations.size(); i++) {
						cout << i + 1 << ". " << operations[i] << endl;
					}
				}
				break;
			}
				  //6.Выход из банкомата
			case 6: {
				cout << "\n== ВЫХОД ==\n";
				cout << "До свидания, " << Atm->getUserName() << "!" << endl;
				Atm->logout();
				running = false;
				break;
			}
			default: {
				cout << "Неверный символ! Введите число от 1 до 6." << endl;
				break;
			}
			}

			if (running) {
				cout << "\nНажмите Enter для продолжения\n";
				cin.get();
			}
		}
	}
};

int main2() {
	SetConsoleCP(65001);
	SetConsoleOutputCP(65001);

	
	Bankaccount account("",0,0.0,"");
	account.loadFromFile();
	
	// Если поле name пустое, то создаём данные по умолчанию
	if (account.getName().empty()) {
		account = Bankaccount("Макс",2578,100000.00, "5092345618744567");
	}

	ATMcontroller atm(&account);
	ATMInterface atmInterface(&atm);


	atmInterface.run();

	account.saveToFile();
	return 0;
}