// ======================
// main.cpp
// ======================

#include <iostream>
#include <fstream>
#include <string>
#include "user.h"
#include "admin.h"

using namespace std;

string roleLogin;
string userLogin;

void registerUser()
{
    string username;
    string password;

    cout << "\n===== REGISTER =====\n";

    cout << "Username : ";
    cin >> username;

    cout << "Password : ";
    cin >> password;

    ofstream file("akun.txt", ios::app);

    file << "user"
         << "|"
         << username
         << "|"
         << password
         << endl;

    file.close();

    cout << "\nRegister berhasil!\n";
}

bool loginUser()
{
    string username;
    string password;

    cout << "\n===== LOGIN =====\n";

    cout << "Username : ";
    cin >> username;

    cout << "Password : ";
    cin >> password;

    ifstream file("akun.txt");

    string role;
    string user;
    string pass;

    while(getline(file, role, '|'))
    {
        getline(file, user, '|');
        getline(file, pass);

        if(username == user &&
           password == pass)
        {
            roleLogin = role;
            userLogin = user;

            cout << "\nLogin berhasil!\n";

            file.close();

            return true;
        }
    }

    file.close();

    cout << "\nLogin gagal!\n";

    return false;
}

bool menuAuth()
{
    int pilih;

    do
    {
        cout << "\n===== MENU AUTH =====\n";

        cout << "1. Login\n";
        cout << "2. Register\n";
        cout << "3. Exit\n";

        cout << "Pilih : ";
        cin >> pilih;

        switch(pilih)
        {
            case 1:

                if(loginUser())
                {
                    return true;
                }

                break;

            case 2:

                registerUser();
                break;

            case 3:

                return false;

            default:

                cout << "\nPilihan tidak valid!\n";
        }

    } while(true);
}

int main()
{
    if(!menuAuth())
    {
        return 0;
    }

    if(roleLogin == "admin")
    {
        menuAdmin();
    }
    else
    {
        menuUser();
    }

    return 0;
}