#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <queue>
#include <climits>

using namespace std;

// Struktur untuk merepresentasikan data penumpang
struct Penumpang {
    int id;               // ID unik penumpang
    string nama;          // Nama penumpang
    string titikNaik;     // Stasiun tempat naik
    string titikTurun;    // Stasiun tempat turun
    string statusTiket;   // Status tiket (aktif/tidak aktif)
    
    // Default constructor (diperlukan untuk unordered_map)
    Penumpang() : id(0), nama(""), titikNaik(""), titikTurun(""), statusTiket("") {}
    
    // Constructor untuk inisialisasi data penumpang
    Penumpang(int _id, string _nama, string _naik, string _turun, string _status) 
        : id(_id), nama(_nama), titikNaik(_naik), titikTurun(_turun), statusTiket(_status) {}
};

// Struktur untuk merepresentasikan edge dalam graph (koneksi antar stasiun)
struct Edge {
    string tujuan;    // Stasiun tujuan
    int jarak;        // Jarak/bobot ke stasiun tujuan
    
    // Constructor untuk inisialisasi edge
    Edge(string _tujuan, int _jarak) : tujuan(_tujuan), jarak(_jarak) {}
};

// Struktur untuk menyimpan informasi rute beserta jumlah penumpangnya
struct InfoRute {
    string namaRute;      // Nama rute
    int jumlahPenumpang;  // Jumlah penumpang di rute tersebut
    
    // Constructor untuk inisialisasi info rute
    InfoRute(string _nama, int _jumlah) : namaRute(_nama), jumlahPenumpang(_jumlah) {}
};

class GoBusNet {
private:
    // Graph untuk merepresentasikan jaringan halte dan rute bus
    unordered_map<string, vector<Edge>> graph;
    
    // Hash Table untuk menyimpan data penumpang dengan ID sebagai key
    unordered_map<int, Penumpang> hashTablePenumpang;
    
    // Vector untuk menyimpan informasi rute dan jumlah penumpangnya
    vector<InfoRute> infoRute;

public:
    // Method untuk menambahkan halte baru ke dalam sistem
    void tambahHalte(const string& namaHalte) {
        // Jika halte belum ada dalam graph, tambahkan sebagai node baru
        if (graph.find(namaHalte) == graph.end()) {
            graph[namaHalte] = vector<Edge>();
            cout << "Halte " << namaHalte << " berhasil ditambahkan." << endl;
        } else {
            cout << "Halte " << namaHalte << " sudah ada dalam sistem." << endl;
        }
    }
    
    // Method untuk menambahkan rute (edge) antara dua halte
    void tambahRute(const string& halteAsal, const string& halteTujuan, int jarak) {
        // Pastikan kedua halte sudah ada dalam sistem
        if (graph.find(halteAsal) == graph.end()) {
            tambahHalte(halteAsal);
        }
        if (graph.find(halteTujuan) == graph.end()) {
            tambahHalte(halteTujuan);
        }
        
        // Tambahkan edge dari halte asal ke halte tujuan
        graph[halteAsal].push_back(Edge(halteTujuan, jarak));
        // Tambahkan edge sebaliknya untuk membuat graf tidak berarah
        graph[halteTujuan].push_back(Edge(halteAsal, jarak));
        
        cout << "Rute dari " << halteAsal << " ke " << halteTujuan 
             << " dengan jarak " << jarak << " berhasil ditambahkan." << endl;
    }
    
    // Method untuk menambahkan data penumpang ke hash table
    void tambahPenumpang(int id, const string& nama, const string& titikNaik, 
                        const string& titikTurun, const string& statusTiket) {
        // Cek apakah ID sudah ada
        if (hashTablePenumpang.find(id) != hashTablePenumpang.end()) {
            cout << "Penumpang dengan ID " << id << " sudah ada dalam sistem." << endl;
            return;
        }
        
        // Gunakan emplace untuk membuat objek langsung di dalam map
        hashTablePenumpang.emplace(id, Penumpang(id, nama, titikNaik, titikTurun, statusTiket));
        
        cout << "Penumpang " << nama << " dengan ID " << id << " berhasil ditambahkan." << endl;
    }
    
    // Method untuk mencari penumpang berdasarkan ID
    void cariPenumpang(int id) {
        // Cari penumpang dalam hash table
        auto it = hashTablePenumpang.find(id);
        if (it != hashTablePenumpang.end()) {
            // Jika ditemukan, tampilkan informasi penumpang
            Penumpang& p = it->second;
            cout << "\n=== INFORMASI PENUMPANG ===" << endl;
            cout << "ID: " << p.id << endl;
            cout << "Nama: " << p.nama << endl;
            cout << "Titik Naik: " << p.titikNaik << endl;
            cout << "Titik Turun: " << p.titikTurun << endl;
            cout << "Status Tiket: " << p.statusTiket << endl;
        } else {
            cout << "Penumpang dengan ID " << id << " tidak ditemukan." << endl;
        }
    }
    
    // Method untuk mencari rute terpendek menggunakan algoritma Dijkstra
    void cariRuteTerpendek(const string& asal, const string& tujuan) {
        // Periksa apakah halte asal dan tujuan ada dalam sistem
        if (graph.find(asal) == graph.end() || graph.find(tujuan) == graph.end()) {
            cout << "Halte asal atau tujuan tidak ditemukan dalam sistem." << endl;
            return;
        }
        
        // Inisialisasi jarak dari asal ke semua node dengan infinity
        unordered_map<string, int> jarak;
        unordered_map<string, string> previous; // Untuk melacak jalur
        for (auto& pair : graph) {
            jarak[pair.first] = INT_MAX;
        }
        jarak[asal] = 0; // Jarak dari asal ke dirinya sendiri = 0
        
        // Priority queue untuk memilih node dengan jarak terpendek
        priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> pq;
        pq.push({0, asal});
        
        // Algoritma Dijkstra
        while (!pq.empty()) {
            int jarakSaatIni = pq.top().first;
            string nodeSaatIni = pq.top().second;
            pq.pop();
            
            // Jika sudah mencapai tujuan, hentikan pencarian
            if (nodeSaatIni == tujuan) break;
            
            // Jika jarak saat ini lebih besar dari jarak yang sudah dicatat, skip
            if (jarakSaatIni > jarak[nodeSaatIni]) continue;
            
            // Periksa semua tetangga dari node saat ini
            for (const Edge& edge : graph[nodeSaatIni]) {
                int jarakBaru = jarak[nodeSaatIni] + edge.jarak;
                
                // Jika ditemukan jalur yang lebih pendek, update
                if (jarakBaru < jarak[edge.tujuan]) {
                    jarak[edge.tujuan] = jarakBaru;
                    previous[edge.tujuan] = nodeSaatIni;
                    pq.push({jarakBaru, edge.tujuan});
                }
            }
        }
        
        // Tampilkan hasil
        if (jarak[tujuan] == INT_MAX) {
            cout << "Tidak ada rute dari " << asal << " ke " << tujuan << endl;
        } else {
            cout << "\n=== RUTE TERPENDEK ===" << endl;
            cout << "Dari " << asal << " ke " << tujuan << endl;
            cout << "Jarak total: " << jarak[tujuan] << endl;
            
            // Rekonstruksi jalur
            vector<string> jalur;
            string current = tujuan;
            while (current != asal) {
                jalur.push_back(current);
                current = previous[current];
            }
            jalur.push_back(asal);
            reverse(jalur.begin(), jalur.end());
            
            cout << "Jalur: ";
            for (size_t i = 0; i < jalur.size(); i++) {
                cout << jalur[i];
                if (i < jalur.size() - 1) cout << " -> ";
            }
            cout << endl;
        }
    }
    
    // Method untuk menghitung jumlah penumpang per rute
    void hitungPenumpangPerRute() {
        // Clear data sebelumnya
        infoRute.clear();
        
        // Map untuk menghitung penumpang per rute (asal-tujuan)
        unordered_map<string, int> hitungRute;
        
        // Iterasi melalui semua penumpang dalam hash table
        for (const auto& pair : hashTablePenumpang) {
            const Penumpang& p = pair.second;
            // Hanya hitung penumpang dengan tiket aktif
            if (p.statusTiket == "aktif") {
                string rute = p.titikNaik + " -> " + p.titikTurun;
                hitungRute[rute]++;
            }
        }
        
        // Masukkan hasil perhitungan ke vector InfoRute
        for (const auto& pair : hitungRute) {
            infoRute.push_back(InfoRute(pair.first, pair.second));
        }
    }
    
    // Method untuk mengurutkan rute berdasarkan jumlah penumpang (descending)
    void urutkanRuteBerdasarkanPenumpang() {
        // Hitung penumpang per rute terlebih dahulu
        hitungPenumpangPerRute();
        
        // Sorting menggunakan lambda function untuk mengurutkan secara descending
        sort(infoRute.begin(), infoRute.end(), 
             [](const InfoRute& a, const InfoRute& b) {
                 return a.jumlahPenumpang > b.jumlahPenumpang; // Descending order
             });
        
        // Tampilkan hasil sorting
        cout << "\n=== RUTE DIURUTKAN BERDASARKAN JUMLAH PENUMPANG ===" << endl;
        cout << "No.\tRute\t\t\t\tJumlah Penumpang" << endl;
        cout << "---\t----\t\t\t\t----------------" << endl;
        
        for (size_t i = 0; i < infoRute.size(); i++) {
            cout << (i+1) << ".\t" << infoRute[i].namaRute 
                 << "\t\t\t" << infoRute[i].jumlahPenumpang << endl;
        }
        
        if (infoRute.empty()) {
            cout << "Tidak ada data rute dengan penumpang aktif." << endl;
        }
    }
    
    // Method untuk menampilkan semua halte dalam sistem
    void tampilkanSemuaHalte() {
        cout << "\n=== DAFTAR HALTE ===" << endl;
        int nomor = 1;
        for (const auto& pair : graph) {
            cout << nomor++ << ". " << pair.first << endl;
        }
    }
    
    // Method untuk menampilkan semua penumpang
    void tampilkanSemuaPenumpang() {
        cout << "\n=== DAFTAR PENUMPANG ===" << endl;
        cout << "ID\tNama\t\tNaik\t\tTurun\t\tStatus" << endl;
        cout << "--\t----\t\t----\t\t-----\t\t------" << endl;
        
        for (const auto& pair : hashTablePenumpang) {
            const Penumpang& p = pair.second;
            cout << p.id << "\t" << p.nama << "\t\t" << p.titikNaik << "\t\t" 
                 << p.titikTurun << "\t\t" << p.statusTiket << endl;
        }
    }
};

// Function untuk menampilkan menu utama
void tampilkanMenu() {
    cout << "\n======================================" << endl;
    cout << "      SISTEM GOBUSNET - MENU UTAMA    " << endl;
    cout << "======================================" << endl;
    cout << "1. Tambah Halte" << endl;
    cout << "2. Tambah Rute" << endl;
    cout << "3. Tambah Penumpang" << endl;
    cout << "4. Cari Penumpang" << endl;
    cout << "5. Cari Rute Terpendek" << endl;
    cout << "6. Urutkan Rute Berdasarkan Penumpang" << endl;
    cout << "7. Tampilkan Semua Halte" << endl;
    cout << "8. Tampilkan Semua Penumpang" << endl;
    cout << "9. Keluar" << endl;
    cout << "======================================" << endl;
    cout << "Pilihan Anda: ";
}

int main() {
    // Inisialisasi objek sistem GoBusNet
    GoBusNet sistem;
    int pilihan;
    
    // Inisialisasi sistem dengan data KRL Jabodetabek
    cout << "Inisialisasi sistem dengan data KRL Jabodetabek..." << endl;
    
    // === JALUR MERAH (Jakarta Kota - Bogor/Depok) ===
    sistem.tambahHalte("Jakarta Kota");
    sistem.tambahHalte("Jayakarta");
    sistem.tambahHalte("Kemayoran");
    sistem.tambahHalte("Rajawali");
    sistem.tambahHalte("Kampung Bandan");
    sistem.tambahHalte("Ancol");
    sistem.tambahHalte("Tanjung Priok");
    sistem.tambahHalte("Duri");
    sistem.tambahHalte("Muara Angke");
    sistem.tambahHalte("Gambir");
    sistem.tambahHalte("Juanda");
    sistem.tambahHalte("Sudirman");
    sistem.tambahHalte("Karet");
    sistem.tambahHalte("Tanah Abang");
    sistem.tambahHalte("Palmerah");
    sistem.tambahHalte("Kebayoran");
    sistem.tambahHalte("Pondok Ranji");
    sistem.tambahHalte("Sudimara");
    sistem.tambahHalte("Rawa Buntu");
    sistem.tambahHalte("Serpong");
    sistem.tambahHalte("Cisauk");
    sistem.tambahHalte("Cicayur");
    sistem.tambahHalte("Parung Panjang");
    sistem.tambahHalte("Tebet");
    sistem.tambahHalte("Manggarai");
    sistem.tambahHalte("Cawang");
    sistem.tambahHalte("Duren Kalibata");
    sistem.tambahHalte("Pasar Minggu Baru");
    sistem.tambahHalte("Pasar Minggu");
    sistem.tambahHalte("Tanjung Barat");
    sistem.tambahHalte("Lenteng Agung");
    sistem.tambahHalte("Universitas Pancasila");
    sistem.tambahHalte("Universitas Indonesia");
    sistem.tambahHalte("Pondok Cina");
    sistem.tambahHalte("Depok");
    sistem.tambahHalte("Depok Baru");
    sistem.tambahHalte("Citayam");
    sistem.tambahHalte("Bojong Gede");
    sistem.tambahHalte("Cilebut");
    sistem.tambahHalte("Bogor");
    
    // === JALUR BIRU (Manggarai - Jatinegara - Bekasi) ===
    sistem.tambahHalte("Jatinegara");
    sistem.tambahHalte("Klender Baru");
    sistem.tambahHalte("Buaran");
    sistem.tambahHalte("Klender");
    sistem.tambahHalte("Cakung");
    sistem.tambahHalte("Kranji");
    sistem.tambahHalte("Bekasi Timur");
    sistem.tambahHalte("Bekasi");
    
    // === JALUR COKLAT (Tanah Abang - Rangkasbitung) ===
    sistem.tambahHalte("Duri");
    sistem.tambahHalte("Angke");
    sistem.tambahHalte("Kampung Bandan");
    sistem.tambahHalte("Pesing");
    sistem.tambahHalte("Grogol");
    sistem.tambahHalte("Taman Kota");
    sistem.tambahHalte("Bojong Indah");
    sistem.tambahHalte("Rawa Buaya");
    sistem.tambahHalte("Kalideres");
    sistem.tambahHalte("Poris");
    sistem.tambahHalte("Batu Ceper");
    sistem.tambahHalte("Tangerang");
    sistem.tambahHalte("Tigaraksa");
    sistem.tambahHalte("Tenjo");
    sistem.tambahHalte("Daru");
    sistem.tambahHalte("Rangkasbitung");
    
    // === JALUR PINK (Tanjung Priok Loop) ===
    sistem.tambahHalte("Pasar Senen");
    sistem.tambahHalte("Gang Sentiong");
    sistem.tambahHalte("Kramat");
    sistem.tambahHalte("Pondok Jati");
    sistem.tambahHalte("Tugu");
    
    // === STASIUN TAMBAHAN ===
    sistem.tambahHalte("Cikini");
    sistem.tambahHalte("Gondangdia");
    
    // RUTE JALUR MERAH - Lingkar Jakarta
    sistem.tambahRute("Jakarta Kota", "Jayakarta", 2);
    sistem.tambahRute("Jayakarta", "Kemayoran", 3);
    sistem.tambahRute("Kemayoran", "Rajawali", 2);
    sistem.tambahRute("Rajawali", "Kampung Bandan", 3);
    sistem.tambahRute("Kampung Bandan", "Ancol", 4);
    sistem.tambahRute("Ancol", "Tanjung Priok", 5);
    sistem.tambahRute("Rajawali", "Duri", 3);
    sistem.tambahRute("Duri", "Muara Angke", 6);
    sistem.tambahRute("Jakarta Kota", "Gambir", 3);
    sistem.tambahRute("Gambir", "Juanda", 2);
    sistem.tambahRute("Juanda", "Sudirman", 2);
    sistem.tambahRute("Sudirman", "Karet", 2);
    sistem.tambahRute("Karet", "Tanah Abang", 2);
    sistem.tambahRute("Tanah Abang", "Palmerah", 3);
    sistem.tambahRute("Palmerah", "Kebayoran", 4);
    sistem.tambahRute("Kebayoran", "Pondok Ranji", 5);
    sistem.tambahRute("Pondok Ranji", "Sudimara", 3);
    sistem.tambahRute("Sudimara", "Rawa Buntu", 4);
    sistem.tambahRute("Rawa Buntu", "Serpong", 5);
    sistem.tambahRute("Serpong", "Cisauk", 6);
    sistem.tambahRute("Cisauk", "Cicayur", 4);
    sistem.tambahRute("Cicayur", "Parung Panjang", 7);
    
    // RUTE JALUR MERAH - Bogor Line
    sistem.tambahRute("Sudirman", "Manggarai", 4);
    sistem.tambahRute("Manggarai", "Tebet", 3);
    sistem.tambahRute("Tebet", "Cawang", 4);
    sistem.tambahRute("Cawang", "Duren Kalibata", 3);
    sistem.tambahRute("Duren Kalibata", "Pasar Minggu Baru", 2);
    sistem.tambahRute("Pasar Minggu Baru", "Pasar Minggu", 2);
    sistem.tambahRute("Pasar Minggu", "Tanjung Barat", 4);
    sistem.tambahRute("Tanjung Barat", "Lenteng Agung", 3);
    sistem.tambahRute("Lenteng Agung", "Universitas Pancasila", 3);
    sistem.tambahRute("Universitas Pancasila", "Universitas Indonesia", 4);
    sistem.tambahRute("Universitas Indonesia", "Pondok Cina", 2);
    sistem.tambahRute("Pondok Cina", "Depok Baru", 3);
    sistem.tambahRute("Depok Baru", "Depok", 2);
    sistem.tambahRute("Depok", "Citayam", 5);
    sistem.tambahRute("Citayam", "Bojong Gede", 4);
    sistem.tambahRute("Bojong Gede", "Cilebut", 6);
    sistem.tambahRute("Cilebut", "Bogor", 8);
    
    // RUTE JALUR BIRU - Bekasi Line
    sistem.tambahRute("Manggarai", "Jatinegara", 5);
    sistem.tambahRute("Jatinegara", "Klender Baru", 4);
    sistem.tambahRute("Klender Baru", "Buaran", 3);
    sistem.tambahRute("Buaran", "Klender", 2);
    sistem.tambahRute("Klender", "Cakung", 5);
    sistem.tambahRute("Cakung", "Kranji", 6);
    sistem.tambahRute("Kranji", "Bekasi Timur", 4);
    sistem.tambahRute("Bekasi Timur", "Bekasi", 3);
    
    // RUTE JALUR COKLAT - Rangkasbitung Line
    sistem.tambahRute("Tanah Abang", "Duri", 4);
    sistem.tambahRute("Duri", "Angke", 3);
    sistem.tambahRute("Angke", "Kampung Bandan", 2);
    sistem.tambahRute("Kampung Bandan", "Pesing", 4);
    sistem.tambahRute("Pesing", "Grogol", 3);
    sistem.tambahRute("Grogol", "Taman Kota", 2);
    sistem.tambahRute("Taman Kota", "Bojong Indah", 3);
    sistem.tambahRute("Bojong Indah", "Rawa Buaya", 4);
    sistem.tambahRute("Rawa Buaya", "Kalideres", 3);
    sistem.tambahRute("Kalideres", "Poris", 5);
    sistem.tambahRute("Poris", "Batu Ceper", 4);
    sistem.tambahRute("Batu Ceper", "Tangerang", 3);
    sistem.tambahRute("Tangerang", "Tigaraksa", 8);
    sistem.tambahRute("Tigaraksa", "Tenjo", 6);
    sistem.tambahRute("Tenjo", "Daru", 5);
    sistem.tambahRute("Daru", "Rangkasbitung", 7);
    
    // RUTE JALUR PINK - Tanjung Priok Loop
    sistem.tambahRute("Gambir", "Pasar Senen", 3);
    sistem.tambahRute("Pasar Senen", "Gang Sentiong", 2);
    sistem.tambahRute("Gang Sentiong", "Kramat", 2);
    sistem.tambahRute("Kramat", "Pondok Jati", 3);
    sistem.tambahRute("Pondok Jati", "Tugu", 4);
    sistem.tambahRute("Tugu", "Tanjung Priok", 5);
    
    // Koneksi Antar Jalur
    sistem.tambahRute("Duri", "Kampung Bandan", 2);
    sistem.tambahRute("Manggarai", "Tebet", 3);
    sistem.tambahRute("Jatinegara", "Cawang", 3);
    
    // Data penumpang sample dengan stasiun KRL
    sistem.tambahPenumpang(1, "Andi Wijaya", "Jakarta Kota", "Bogor", "aktif");
    sistem.tambahPenumpang(2, "Sari Dewi", "Gambir", "Depok", "aktif");
    sistem.tambahPenumpang(3, "Budi Santoso", "Tanah Abang", "Serpong", "aktif");
    sistem.tambahPenumpang(4, "Maya Sari", "Sudirman", "Universitas Indonesia", "aktif");
    sistem.tambahPenumpang(5, "Rudi Hartono", "Manggarai", "Bekasi", "aktif");
    sistem.tambahPenumpang(6, "Lina Kusuma", "Duri", "Tangerang", "non-aktif");
    sistem.tambahPenumpang(7, "Agus Pramono", "Pasar Senen", "Tanjung Priok", "aktif");
    sistem.tambahPenumpang(8, "Rina Wati", "Cawang", "Cikini", "aktif");
    sistem.tambahPenumpang(9, "Doni Saputra", "Kebayoran", "Parung Panjang", "aktif");
    sistem.tambahPenumpang(10, "Fitri Handayani", "Tebet", "Citayam", "non-aktif");
    
    // Loop utama program
    do {
        tampilkanMenu();
        cin >> pilihan;
        
        switch (pilihan) {
            case 1: {
                // Tambah halte baru
                string namaHalte;
                cout << "Masukkan nama halte: ";
                cin.ignore(); // Membersihkan buffer input
                getline(cin, namaHalte);
                sistem.tambahHalte(namaHalte);
                break;
            }
            case 2: {
                // Tambah rute baru
                string halteAsal, halteTujuan;
                int jarak;
                cout << "Masukkan halte asal: ";
                cin.ignore();
                getline(cin, halteAsal);
                cout << "Masukkan halte tujuan: ";
                getline(cin, halteTujuan);
                cout << "Masukkan jarak: ";
                cin >> jarak;
                sistem.tambahRute(halteAsal, halteTujuan, jarak);
                break;
            }
            case 3: {
                // Tambah penumpang baru
                int id;
                string nama, titikNaik, titikTurun, status;
                cout << "Masukkan ID penumpang: ";
                cin >> id;
                cout << "Masukkan nama penumpang: ";
                cin.ignore();
                getline(cin, nama);
                cout << "Masukkan titik naik: ";
                getline(cin, titikNaik);
                cout << "Masukkan titik turun: ";
                getline(cin, titikTurun);
                cout << "Masukkan status tiket (aktif/non-aktif): ";
                getline(cin, status);
                sistem.tambahPenumpang(id, nama, titikNaik, titikTurun, status);
                break;
            }
            case 4: {
                // Cari penumpang berdasarkan ID
                int id;
                cout << "Masukkan ID penumpang yang dicari: ";
                cin >> id;
                sistem.cariPenumpang(id);
                break;
            }
            case 5: {
                // Cari rute terpendek
                string asal, tujuan;
                cout << "Masukkan halte asal: ";
                cin.ignore();
                getline(cin, asal);
                cout << "Masukkan halte tujuan: ";
                getline(cin, tujuan);
                sistem.cariRuteTerpendek(asal, tujuan);
                break;
            }
            case 6: {
                // Urutkan rute berdasarkan jumlah penumpang
                sistem.urutkanRuteBerdasarkanPenumpang();
                break;
            }
            case 7: {
                // Tampilkan semua halte
                sistem.tampilkanSemuaHalte();
                break;
            }
            case 8: {
                // Tampilkan semua penumpang
                sistem.tampilkanSemuaPenumpang();
                break;
            }
            case 9: {
                // Keluar dari program
                cout << "Terima kasih telah menggunakan sistem GoBusNet!" << endl;
                break;
            }
            default: {
                cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
                break;
            }
        }
    } while (pilihan != 9);
    
    return 0;
}