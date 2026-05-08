// ======================
// user.cpp
// ======================

#include "user.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

extern string userLogin;

bool cekJamTersedia(string lapangan, string jam)
{
    ifstream file("booking.txt");

    string id;
    string user;
    string lap;
    string jamFile;
    string status;

    while(getline(file, id, '|'))
    {
        getline(file, user, '|');
        getline(file, lap, '|');
        getline(file, jamFile, '|');
        getline(file, status);

        if(lap == lapangan &&
           jamFile == jam &&
           (status == "pending" ||
            status == "acc"))
        {
            file.close();

            return false;
        }
    }

    file.close();

    return true;
}

void tampilLapangan()
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

        if(status == "ready")
        {
            cout << "ID      : " << id << endl;
            cout << "Nama    : " << nama << endl;
            cout << "Harga   : " << harga << endl;
            cout << "----------------------\n";
        }
    }

    file.close();
}

void bookingLapangan()
{
    tampilLapangan();

    string lapangan;
    string jam;

    cout << "\nUser : " << userLogin << endl;

    cin.ignore();

    cout << "Pilih Lapangan : ";
    getline(cin, lapangan);

    string daftarJam[] =
    {
        "08:00",
        "10:00",
        "12:00",
        "14:00",
        "16:00",
        "18:00",
        "20:00"
    };

    cout << "\n===== JAM TERSEDIA =====\n";

    for(int i = 0; i < 7; i++)
    {
        if(cekJamTersedia(lapangan, daftarJam[i]))
        {
            cout << daftarJam[i] << endl;
        }
    }

    cout << "\nPilih Jam : ";
    getline(cin, jam);

    if(!cekJamTersedia(lapangan, jam))
    {
        cout << "\nJam tidak tersedia!\n";
        return;
    }

    int id = 1;

    ifstream cek("booking.txt");

    string temp;

    while(getline(cek, temp))
    {
        id++;
    }

    cek.close();

    ofstream file("booking.txt", ios::app);

    file << id
         << "|"
         << userLogin
         << "|"
         << lapangan
         << "|"
         << jam
         << "|"
         << "pending"
         << endl;

    file.close();

    cout << "\nBooking berhasil dibuat!\n";
}

void riwayatBooking()
{
    ifstream file("booking.txt");

    string id;
    string user;
    string lap;
    string jam;
    string status;

    cout << "\n===== RIWAYAT BOOKING =====\n";

    while(getline(file, id, '|'))
    {
        getline(file, user, '|');
        getline(file, lap, '|');
        getline(file, jam, '|');
        getline(file, status);

        if(user == userLogin)
        {
            cout << "ID       : " << id << endl;
            cout << "Lapangan : " << lap << endl;
            cout << "Jam      : " << jam << endl;
            cout << "Status   : " << status << endl;
            cout << "----------------------\n";
        }
    }

    file.close();
}

void ubahJadwal()
{
    riwayatBooking();

    string pilihId;
    string jamBaru;

    cout << "\nMasukkan ID Booking : ";
    cin >> pilihId;

    cin.ignore();

    cout << "Jam Baru : ";
    getline(cin, jamBaru);

    ifstream file("booking.txt");
    ofstream temp("temp.txt");

    string id;
    string user;
    string lap;
    string jam;
    string status;

    while(getline(file, id, '|'))
    {
        getline(file, user, '|');
        getline(file, lap, '|');
        getline(file, jam, '|');
        getline(file, status);

        if(id == pilihId)
        {
            if(cekJamTersedia(lap, jamBaru))
            {
                jam = jamBaru;
                status = "pending";
            }
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

    file.close();
    temp.close();

    remove("booking.txt");
    rename("temp.txt", "booking.txt");

    cout << "\nJadwal berhasil diubah!\n";
}

void batalkanBooking()
{
    riwayatBooking();

    string pilihId;

    cout << "\nMasukkan ID Booking : ";
    cin >> pilihId;

    ifstream file("booking.txt");
    ofstream temp("temp.txt");

    string id;
    string user;
    string lap;
    string jam;
    string status;

    while(getline(file, id, '|'))
    {
        getline(file, user, '|');
        getline(file, lap, '|');
        getline(file, jam, '|');
        getline(file, status);

        if(id == pilihId)
        {
            status = "cancel";
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

    file.close();
    temp.close();

    remove("booking.txt");
    rename("temp.txt", "booking.txt");

    cout << "\nBooking berhasil dibatalkan!\n";
}

void menuUser()
{
    int pilih;

    do
    {
        cout << "\n===== MENU USER =====\n";

        cout << "1. Booking Lapangan\n";
        cout << "2. Ubah Jadwal\n";
        cout << "3. Batalkan Booking\n";
        cout << "4. Riwayat Booking\n";
        cout << "5. Exit\n";

        cout << "Pilih : ";
        cin >> pilih;

        switch(pilih)
        {
            case 1:
                bookingLapangan();
                break;

            case 2:
                ubahJadwal();
                break;

            case 3:
                batalkanBooking();
                break;

            case 4:
                riwayatBooking();
                break;

            case 5:
                cout << "\nLogout...\n";
                break;

            default:
                cout << "\nPilihan tidak valid!\n";
        }

    } while(pilih != 5);
}