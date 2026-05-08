// ======================
// admin.cpp
// ======================

#include "admin.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void lihatLapangan()
{
    ifstream file("lapangan.txt");

    string id;
    string nama;
    string harga;
    string status;

    cout << "\n===== DAFTAR LAPANGAN =====\n";

    while(getline(file, id, '|'))
    {
        getline(file, nama, '|');
        getline(file, harga, '|');
        getline(file, status);

        cout << "ID      : " << id << endl;
        cout << "Nama    : " << nama << endl;
        cout << "Harga   : " << harga << endl;
        cout << "Status  : " << status << endl;
        cout << "----------------------\n";
    }

    file.close();
}

void tambahLapangan()
{
    string nama;
    string harga;

    cin.ignore();

    cout << "\nNama Lapangan : ";
    getline(cin, nama);

    cout << "Harga : ";
    getline(cin, harga);

    int id = 1;

    ifstream cek("lapangan.txt");

    string temp;

    while(getline(cek, temp))
    {
        id++;
    }

    cek.close();

    ofstream file("lapangan.txt", ios::app);

    file << id
         << "|"
         << nama
         << "|"
         << harga
         << "|"
         << "ready"
         << endl;

    file.close();

    cout << "\nLapangan berhasil ditambahkan!\n";
}

void tambahAdmin()
{
    string username;
    string password;

    cout << "\nUsername Admin : ";
    cin >> username;

    cout << "Password : ";
    cin >> password;

    ofstream file("akun.txt", ios::app);

    file << "admin"
         << "|"
         << username
         << "|"
         << password
         << endl;

    file.close();

    cout << "\nAdmin berhasil ditambahkan!\n";
}

void approveBooking()
{
    ifstream file("booking.txt");

    string id;
    string user;
    string lap;
    string jam;
    string status;

    cout << "\n===== BOOKING PENDING =====\n";

    while(getline(file, id, '|'))
    {
        getline(file, user, '|');
        getline(file, lap, '|');
        getline(file, jam, '|');
        getline(file, status);

        if(status == "pending")
        {
            cout << id
                 << " | "
                 << user
                 << " | "
                 << lap
                 << " | "
                 << jam
                 << endl;
        }
    }

    file.close();

    string pilihId;
    string aksi;

    cout << "\nMasukkan ID Booking : ";
    cin >> pilihId;

    cout << "Status (acc/decline) : ";
    cin >> aksi;

    ifstream baca("booking.txt");
    ofstream temp("temp.txt");

    while(getline(baca, id, '|'))
    {
        getline(baca, user, '|');
        getline(baca, lap, '|');
        getline(baca, jam, '|');
        getline(baca, status);

        if(id == pilihId)
        {
            status = aksi;
        }

        temp << id
             << "|"
             << user
             << "|"
             << lap
             << "|"
             << jam
             << "|"
             << status
             << endl;
    }

    baca.close();
    temp.close();

    remove("booking.txt");
    rename("temp.txt", "booking.txt");

    cout << "\nStatus booking berhasil diubah!\n";
}

void lihatPendapatan()
{
    ifstream booking("booking.txt");

    string id;
    string user;
    string lap;
    string jam;
    string status;

    int total = 0;

    while(getline(booking, id, '|'))
    {
        getline(booking, user, '|');
        getline(booking, lap, '|');
        getline(booking, jam, '|');
        getline(booking, status);

        if(status == "acc")
        {
            ifstream lapangan("lapangan.txt");

            string idLap;
            string nama;
            string harga;
            string statusLap;

            while(getline(lapangan, idLap, '|'))
            {
                getline(lapangan, nama, '|');
                getline(lapangan, harga, '|');
                getline(lapangan, statusLap);

                if(nama == lap)
                {
                    total += stoi(harga);
                }
            }

            lapangan.close();
        }
    }

    booking.close();

    cout << "\nTotal Pendapatan : "
         << total
         << endl;
}

void menuAdmin()
{
    int pilih;

    do
    {
        cout << "\n===== MENU ADMIN =====\n";

        cout << "1. Lihat Lapangan\n";
        cout << "2. Tambah Lapangan\n";
        cout << "3. Approve Booking\n";
        cout << "4. Tambah Admin\n";
        cout << "5. Lihat Pendapatan\n";
        cout << "6. Exit\n";

        cout << "Pilih : ";
        cin >> pilih;

        switch(pilih)
        {
            case 1:
                lihatLapangan();
                break;

            case 2:
                tambahLapangan();
                break;

            case 3:
                approveBooking();
                break;

            case 4:
                tambahAdmin();
                break;

            case 5:
                lihatPendapatan();
                break;

            case 6:
                cout << "\nLogout...\n";
                break;

            default:
                cout << "\nPilihan tidak valid!\n";
        }

    } while(pilih != 6);
}