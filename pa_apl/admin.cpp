#include "admin.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cctype>

using namespace std;

void menuAuth();

int ambilJamSekarang()
{
    time_t sekarang = time(0);
    tm *waktu = localtime(&sekarang);

    return waktu->tm_hour;
}

void updateBookingSelesai(int jamSekarang)
{
    ifstream file("booking.txt");

    if(!file.is_open())
    {
        return;
    }

    ofstream temp("temp.txt");

    string header;
    getline(file, header);

    temp << "ID|USER|LAPANGAN|JAM_MULAI|DURASI|JAM_SELESAI|STATUS\n";

    string id, user, lap, jamMulai, durasi, jamSelesai, status;

    while(getline(file, id, '|'))
    {
        getline(file, user, '|');
        getline(file, lap, '|');
        getline(file, jamMulai, '|');
        getline(file, durasi, '|');
        getline(file, jamSelesai, '|');
        getline(file, status);

        if(jamSelesai.length() >= 2 &&
           isdigit(jamSelesai[0]) &&
           isdigit(jamSelesai[1]))
        {
            int selesai = stoi(jamSelesai.substr(0, 2));

            if(status == "acc" && jamSekarang >= selesai)
            {
                status = "done";
            }
        }

        temp << id << "|"
             << user << "|"
             << lap << "|"
             << jamMulai << "|"
             << durasi << "|"
             << jamSelesai << "|"
             << status << endl;
    }

    file.close();
    temp.close();

    remove("booking.txt");
    rename("temp.txt", "booking.txt");
}

string formatRupiah(int angka)
{
    string hasil = to_string(angka);
    string rupiah = "";

    int hitung = 0;

    for(int i = hasil.length() - 1; i >= 0; i--)
    {
        rupiah = hasil[i] + rupiah;
        hitung++;

        if(hitung == 3 && i != 0)
        {
            rupiah = "." + rupiah;
            hitung = 0;
        }
    }

    return "Rp " + rupiah;
}

void lihatLapangan()
{
    ifstream file("lapangan.txt");

    string id, nama, harga, status;

    cout << "\n===== DAFTAR LAPANGAN =====\n";

    while(getline(file, id, '|'))
    {
        getline(file, nama, '|');
        getline(file, harga, '|');
        getline(file, status);

        cout << "ID      : " << id << endl;
        cout << "Nama    : " << nama << endl;
        cout << "Harga   : "
        << formatRupiah(stoi(harga)) << endl;
        cout << "Status  : " << status << endl;
        cout << "----------------------\n";
    }

    file.close();
}

void tambahLapangan()
{
    string nama, harga;

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
         << "|ready"
         << endl;

    file.close();

    cout << "\nLapangan berhasil ditambahkan!\n";
}

void tambahAdmin()
{
    string username, password;

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

    string header;
    getline(file, header);

    string id, user, lap, jamMulai, durasi, jamSelesai, status;

    cout << "\n===== BOOKING PENDING =====\n";

    bool adaPending = false;

    while(getline(file, id, '|'))
    {
        getline(file, user, '|');
        getline(file, lap, '|');
        getline(file, jamMulai, '|');
        getline(file, durasi, '|');
        getline(file, jamSelesai, '|');
        getline(file, status);

        if(status == "pending")
        {
            adaPending = true;

            int hargaLapangan = 0;

            ifstream lapanganFile("lapangan.txt");

            string idLap, namaLap, hargaLap, statusLap;

            while(getline(lapanganFile, idLap, '|'))
            {
                getline(lapanganFile, namaLap, '|');
                getline(lapanganFile, hargaLap, '|');
                getline(lapanganFile, statusLap);

                if(idLap == lap)
                {
                    hargaLapangan = stoi(hargaLap);
                    break;
                }
            }

            lapanganFile.close();

            int totalHarga = hargaLapangan * stoi(durasi);

            cout << "ID        : " << id << endl;
            cout << "User      : " << user << endl;
            cout << "Lapangan  : " << lap << endl;
            cout << "Jam       : " << jamMulai << " - " << jamSelesai << endl;
            cout << "Durasi    : " << durasi << " jam" << endl;
            cout << "Harga/Jam : " << formatRupiah(hargaLapangan) << endl;
            cout << "Total     : " << formatRupiah(totalHarga) << endl;
            cout << "Status    : " << status << endl;
            cout << "----------------------\n";
        }
    }

    file.close();

    if(!adaPending)
    {
        cout << "\nBelum ada booking berstatus pending!\n";
        return;
    }

    string pilihId, aksi;

    cout << "\nMasukkan ID Booking : ";
    cin >> pilihId;

    cout << "Status (acc/decline) : ";
    cin >> aksi;

    if(aksi != "acc" && aksi != "decline")
    {
        cout << "\nStatus tidak valid!\n";
        return;
    }

    ifstream baca("booking.txt");
    ofstream temp("temp.txt");

    getline(baca, header);

    temp << "ID|USER|LAPANGAN|JAM_MULAI|DURASI|JAM_SELESAI|STATUS\n";

    bool ditemukan = false;

    while(getline(baca, id, '|'))
    {
        getline(baca, user, '|');
        getline(baca, lap, '|');
        getline(baca, jamMulai, '|');
        getline(baca, durasi, '|');
        getline(baca, jamSelesai, '|');
        getline(baca, status);

        if(id == pilihId)
        {
            status = aksi;
            ditemukan = true;
        }

        temp << id << "|"
             << user << "|"
             << lap << "|"
             << jamMulai << "|"
             << durasi << "|"
             << jamSelesai << "|"
             << status << endl;
    }

    baca.close();
    temp.close();

    remove("booking.txt");
    rename("temp.txt", "booking.txt");

    if(ditemukan)
        cout << "\nStatus booking berhasil diubah!\n";
    else
        cout << "\nID booking tidak ditemukan!\n";
}

void lihatPendapatan()
{
    updateBookingSelesai(ambilJamSekarang());

    ifstream booking("booking.txt");

    string header;
    getline(booking, header);

    string id, user, lap, jamMulai, durasi, jamSelesai, status;

    int total = 0;

    while(getline(booking, id, '|'))
    {
        getline(booking, user, '|');
        getline(booking, lap, '|');
        getline(booking, jamMulai, '|');
        getline(booking, durasi, '|');
        getline(booking, jamSelesai, '|');
        getline(booking, status);

        if(status == "acc" || status == "done")
        {
            ifstream lapangan("lapangan.txt");

            string idLap, nama, harga, statusLap;

            while(getline(lapangan, idLap, '|'))
            {
                getline(lapangan, nama, '|');
                getline(lapangan, harga, '|');
                getline(lapangan, statusLap);

                if(idLap == lap)
                {
                    total += stoi(harga) * stoi(durasi);
                }
            }

            lapangan.close();
        }
    }

    booking.close();

    cout << "\nTotal Pendapatan : " 
    << formatRupiah(total) << endl;
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
        if(cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');

            cout << "\nInput Hanya Bisa Angka!\n";
            continue;
        }
        

        switch(pilih)
        {
            case 1:
                lihatLapangan();
                break;

            case 2:
                tambahLapangan();
                break;

            case 3:
                updateBookingSelesai(ambilJamSekarang());
                approveBooking();
                // int jamTes;

                // cout << "Masukkan jam sekarang untuk test: ";
                // cin >> jamTes;

                // updateBookingSelesai(jamTes);
                break;

            case 4:
                tambahAdmin();
                break;

            case 5:
                lihatPendapatan();
                break;

            case 6:
                cout << "\nLogout...\n";
                return;

            default:
                cout << "\nPilihan tidak valid!\n";
        }

    } while(true);
}