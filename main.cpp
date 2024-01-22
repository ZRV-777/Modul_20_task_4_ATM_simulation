#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

enum
{
    ONE_HUNDRED_RUB = 100,
    TWO_HUNDRED_RUB = 200,
    FIVE_HUNDRED_RUB = 500,
    ONE_THOUSAND_RUB = 1000,
    TWO_THOUSAND_RUB = 2000,
    FIVE_THOUSAND_RUB = 5000
};

//massive of banknotes
const int banknotes[6] = {ONE_HUNDRED_RUB, TWO_HUNDRED_RUB, FIVE_HUNDRED_RUB, ONE_THOUSAND_RUB,
                          TWO_THOUSAND_RUB, FIVE_THOUSAND_RUB};
//count of banknotes for user
int user_5000 = 0, user_2000 = 0, user_1000 = 0, user_500 = 0, user_200 = 0, user_100 = 0;
//count of banknotes for ATM
int atm_5000 = 0, atm_2000 = 0, atm_1000 = 0, atm_500 = 0, atm_200 = 0, atm_100 = 0;
//total cash in atm
int total_atm_money = 0;

const int ATM_BANKNOTES_QUANTITY = 1000;
const int ZERO = 0;

//ATM filling function
void fill_ATM (fstream& fio);
//ATM show banknotes function
void show_ATM (fstream& fio);
//global variables initializer
void ATM_state(fstream& fio);
//user input moneys function
void get_money_from_ATM (fstream& fio, int& sum, const int& nominal, int& atm_nom_amount, int& user_nom_amount);
//return money to ATM, if there is money, but it does not match the bills
void return_money_to_ATM (fstream& fio, int& sum, const int& nominal, int& atm_nom_amount, int& user_nom_amount);

int main()
{
    fstream fio;
    fio.open("C:\\develop\\test\\build\\modul_20_task_4\\atm.bin", ios::binary | ios::out | ios::in);
    if (!fio.is_open())
    {
        cerr << "Error! File is not open!" << endl;
        return 1;
    }

    for (int i = 0; i < ATM_BANKNOTES_QUANTITY; i++)
    {
        fio.write((char*) &ZERO, sizeof(int));
    }
    //first time fill money ATM
    fill_ATM(fio);

    char ans = '+';
    while (ans != 'q')
    {
        //zeroing banknotes for user
        user_5000 = 0, user_2000 = 0, user_1000 = 0, user_500 = 0, user_200 = 0, user_100 = 0;
        cout << "Take money, fill the ATM or leave (-/+/q): ";
        cin >> ans;
        //if minus, take money from ATM
        if (ans == '-')
        {
            int sum = 0;
            cout << endl << "Enter sum: ";
            cin >> sum;
            //if ATM haven`t money, fill ATM
            if (total_atm_money == ZERO)
            {
                cout << "No money in ATM!" << endl;
                fill_ATM(fio);
            }
            //if sum > total cash in ATM, ask for less
            //or if sum % 100 != 0; aks equal sum
            while (sum > total_atm_money || sum % 100 != 0)
            {
                if (sum > total_atm_money)
                {
                    cout << "Not enough money in ATM!" << endl;
                    cout << "Try a lesser sum: ";
                    cin >> sum;
                }
                else if (sum % 100 != 0)
                {
                    cout << "Enter sum divisible by 100: ";
                    cin >> sum;
                }
            }
            cout << "Sum give to user - " << sum << endl; //sum user got

            //start input moeny from ATM, started from 5000
            get_money_from_ATM(fio, sum, FIVE_THOUSAND_RUB, atm_5000, user_5000);
            get_money_from_ATM(fio, sum, TWO_THOUSAND_RUB, atm_2000, user_2000);
            get_money_from_ATM(fio, sum, ONE_THOUSAND_RUB, atm_1000, user_1000);
            get_money_from_ATM(fio, sum, FIVE_HUNDRED_RUB, atm_500, user_500);
            get_money_from_ATM(fio, sum, TWO_HUNDRED_RUB, atm_200, user_200);
            get_money_from_ATM(fio, sum, ONE_HUNDRED_RUB, atm_100, user_100);

            //show how many banknotes user got
            cout << "5000 - " << user_5000 << ", total - " << user_5000 * FIVE_THOUSAND_RUB << endl;
            cout << "2000 - " << user_2000 << ", total - " << user_2000 * TWO_THOUSAND_RUB << endl;
            cout << "1000 - " << user_1000 << ", total - " << user_1000 * ONE_THOUSAND_RUB << endl;
            cout << "500 - " << user_500 << ", total - " << user_500 * FIVE_HUNDRED_RUB << endl;
            cout << "200 - " << user_200 << ", total - " << user_200 * TWO_HUNDRED_RUB << endl;
            cout << "100 - " << user_100 << ", total - " << user_100 * ONE_HUNDRED_RUB << endl << endl;

            //show how many banknotes ATM has
            cout << "ATM`s money" << endl;
            cout << "5000 - " << atm_5000 << ", total - " << atm_5000 * FIVE_THOUSAND_RUB << endl;
            cout << "2000 - " << atm_2000 << ", total - " << atm_2000 * TWO_THOUSAND_RUB << endl;
            cout << "1000 - " << atm_1000 << ", total - " << atm_1000 * ONE_THOUSAND_RUB << endl;
            cout << "500 - " << atm_500 << ", total - " << atm_500 * FIVE_HUNDRED_RUB << endl;
            cout << "200 - " << atm_200 << ", total - " << atm_200 * TWO_HUNDRED_RUB << endl;
            cout << "100 - " << atm_100 << ", total - " << atm_100 * ONE_HUNDRED_RUB << endl << endl;

            cout << "Total cash in ATM - " << total_atm_money << endl; //total money in ATM
            cout << "ATM state after giving money" << endl; //ATM state after giving money

            //if sum giving an ATM is not equal 0, return money to ATM
            if (sum != 0)
            {
                return_money_to_ATM(fio, sum, FIVE_THOUSAND_RUB, atm_5000, user_5000);
                return_money_to_ATM(fio, sum, TWO_THOUSAND_RUB, atm_2000, user_2000);
                return_money_to_ATM(fio, sum, ONE_THOUSAND_RUB, atm_1000, user_1000);
                return_money_to_ATM(fio, sum, FIVE_HUNDRED_RUB, atm_500, user_500);
                return_money_to_ATM(fio, sum, TWO_HUNDRED_RUB, atm_200, user_200);
                return_money_to_ATM(fio, sum, ONE_HUNDRED_RUB, atm_100, user_100);

                cout << "ATM state after returnign money" << endl;
                //show_ATM(fio);

                cout << "User`s money after returning" << endl;
                cout << "5000 - " << user_5000 << ", total - " << user_5000 * FIVE_THOUSAND_RUB << endl;
                cout << "2000 - " << user_2000 << ", total - " << user_2000 * TWO_THOUSAND_RUB << endl;
                cout << "1000 - " << user_1000 << ", total - " << user_1000 * ONE_THOUSAND_RUB << endl;
                cout << "500 - " << user_500 << ", total - " << user_500 * FIVE_HUNDRED_RUB << endl;
                cout << "200 - " << user_200 << ", total - " << user_200 * TWO_HUNDRED_RUB << endl;
                cout << "100 - " << user_100 << ", total - " << user_100 * ONE_HUNDRED_RUB << endl;

                cout << "ATM`s money after returning" << endl;
                cout << "5000 - " << atm_5000 << endl;
                cout << "2000 - " << atm_2000 << endl;
                cout << "1000 - " << atm_1000 << endl;
                cout << "500 - " << atm_500 << endl;
                cout << "200 - " << atm_200 << endl;
                cout << "100 - " << atm_100 << endl << endl;

                cout << endl << "Total ATM money - " << total_atm_money << endl;
            }
        }

            //if input +, fill ATM
        else if (ans == '+')
        {
            fill_ATM(fio);
        }
            //if input any other char, exit
        else break;
    }

    fio.close();
}

void fill_ATM(fstream& fio)
{
    cout << "Filling ATM" << endl;
    fio.clear(); //clear the buffer
    fio.seekg(ZERO, ios::beg); //set pointer to the beginning of the file

    for (int i = 0; i < ATM_BANKNOTES_QUANTITY; i++)
    {
        int n; //buffer for numbers from file
        fio.read((char*) &n, sizeof(int));
        int cursor = fio.tellg(); //cursor position from read
        if (n == ZERO)
        {
            fio.seekp(cursor - sizeof(int), ios::beg);//return on int bited back
            fio.write((char*)(&banknotes[rand() % 6]), sizeof(int));//write random digit instead of zero
        }
    }
    ATM_state(fio);//refresh ATM state
    cout << "ATM state after filling with money" << endl;
}

void ATM_state(fstream& fio)
{
    fio.clear();
    fio.seekg(ZERO, ios::beg);
    // zeroing banknotes in ATM
    atm_5000 = 0, atm_2000 = 0, atm_1000 = 0, atm_500 = 0, atm_200 = 0, atm_100 = 0;

    total_atm_money = 0;
    for (int i = 0; i < ATM_BANKNOTES_QUANTITY; i++)
    {
        int n;
        fio.read((char*) &n, sizeof(int));
        //read banknotes and sum total cash in ATM
        switch (n)
        {
            case FIVE_THOUSAND_RUB : atm_5000++;
                total_atm_money += FIVE_THOUSAND_RUB;
                break;
            case TWO_THOUSAND_RUB : atm_2000++;
                total_atm_money += TWO_THOUSAND_RUB;
                break;
            case ONE_THOUSAND_RUB : atm_1000++;
                total_atm_money += ONE_THOUSAND_RUB;
                break;
            case FIVE_HUNDRED_RUB : atm_500++;
                total_atm_money += FIVE_HUNDRED_RUB;
                break;
            case TWO_HUNDRED_RUB : atm_200++;
                total_atm_money += TWO_HUNDRED_RUB;
                break;
            case ONE_HUNDRED_RUB : atm_100++;
                total_atm_money += ONE_HUNDRED_RUB;
                break;
        }
    }
}

void get_money_from_ATM (fstream& fio, int& sum, const int& nominal, int& atm_nom_amount, int& user_nom_amount)
{
    fio.clear();
    fio.seekp(ZERO, ios::beg);

    for (int i = 0; i < ATM_BANKNOTES_QUANTITY && sum >= nominal && atm_nom_amount > ZERO; i++)
    {
        int n;
        fio.read((char*) &n, sizeof(int));
        int cursor = fio.tellg();
        //if n equal nominal
        if (n == nominal)
        {
            sum -= nominal;
            total_atm_money -= nominal;
            user_nom_amount++;
            atm_nom_amount--;
            fio.seekp(cursor-sizeof(int), ios::beg);
            fio.write((char*) &ZERO, sizeof(int));
        }
    }
}

void return_money_to_ATM(fstream& fio, int& sum, const int& nominal, int& atm_nom_amount, int& user_nom_amount)
{
    fio.clear();
    fio.seekp(ZERO, ios::beg);

    for (int i = 0; i < ATM_BANKNOTES_QUANTITY && user_nom_amount > 0; i++)
    {
        int n;
        fio.read((char*) &n, sizeof(int));
        int cursor = fio.tellg();
        if (n == ZERO)
        {
            sum += nominal;
            total_atm_money += nominal;
            user_nom_amount--;
            atm_nom_amount++;
            fio.seekp(cursor - sizeof(int), ios::beg);
            fio.write((char*) &nominal, sizeof(int));
        }
    }
}