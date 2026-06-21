#include <bits/stdc++.h>
using namespace std;

// Struktur data pendukung
struct Activity
{
    string type;
    string detail;
};

struct Consultation
{
    int num;
    string student;
    string lecturer;
};

struct ServiceReq
{
    string student;
    int prio;
    string type;
    // Operator untuk priority queue (semakin kecil angka prioritas, semakin tinggi prioritasnya)
    bool operator>(const ServiceReq &o) const
    {
        return prio > o.prio;
    }
};

class SmartCampus
{
private:
    // 1. Map (Hash Table): Login & Akun
    map<string, string> accounts;

    // 2. Linked List: Riwayat Aktivitas
    list<Activity> activityHistory;

    // 3. Queue: Antrian Konsultasi
    queue<Consultation> consultQ;
    int nextQueueNum = 1;

    // 4. Tree: Struktur Organisasi (Disimpan sebagai map parent-child)
    map<string, vector<string>> orgStructure;

    // 5. Graph: Navigasi Kampus
    map<string, vector<pair<int, string>>> campusGraph;
    map<string, string> buildingNames; // ID Gedung -> Nama Lengkap

    // 6. Stack: Undo KRS
    stack<string> krsStack;
    set<string> currentKRS; // Set untuk memastikan mata kuliah unik

    // 7. Priority Queue: Layanan Akademik Prioritas
    priority_queue<ServiceReq, vector<ServiceReq>, greater<ServiceReq>> servicePQ;

public:
    // --- Fitur 1: Login (Map) ---
    void registerAccount(string nim, string password)
    {
        accounts[nim] = password;
        logActivity("Registrasi", "Akun " + nim + " berhasil dibuat");
        cout << "[Sistem] Akun berhasil dibuat.\n";
    }

    bool login(string nim, string password)
    {
        if (accounts.find(nim) != accounts.end() && accounts[nim] == password)
        {
            logActivity("Login", "User " + nim + " berhasil login");
            cout << "[Sistem] Login Berhasil!\n";
            return true;
        }
        cout << "[Sistem] Login Gagal. Periksa NIM atau Password.\n";
        return false;
    }

    // --- Fitur 2: Riwayat (Linked List) ---
    void logActivity(string type, string detail)
    {
        Activity act;
        act.type = type;
        act.detail = detail;
        activityHistory.push_back(act);
    }

    void showHistory()
    {
        cout << "\n--- Riwayat Aktivitas Anda ---\n";
        if (activityHistory.empty())
        {
            cout << "Belum ada aktivitas.\n";
            return;
        }
        for (const auto &act : activityHistory)
        {
            cout << "- [" << act.type << "] " << act.detail << endl;
        }
    }

    // --- Fitur 3: Antrian Konsultasi (Queue) ---
    void joinConsultationQueue(string student, string lecturer)
    {
        Consultation c;
        c.num = nextQueueNum++;
        c.student = student;
        c.lecturer = lecturer;
        consultQ.push(c);
        logActivity("Antrian", student + " masuk antrian dosen " + lecturer);
        cout << "[Antrian] Nomor antrian Anda: " << c.num << endl;
    }

    void serveNextConsultation()
    {
        if (!consultQ.empty())
        {
            Consultation c = consultQ.front();
            consultQ.pop();
            cout << "[Antrian] Memanggil: " << c.student << " bertemu dengan " << c.lecturer << endl;
        }
        else
        {
            cout << "[Antrian] Antrian konsultasi kosong.\n";
        }
    }

    // --- Fitur 4: Struktur Organisasi (Tree) ---
    void addOrgRelation(string boss, string subordinate)
    {
        orgStructure[boss].push_back(subordinate);
    }

    void displayOrgTree(string boss, int level = 0)
    {
        for (int i = 0; i < level; i++)
            cout << "  ";
        cout << "|-- " << boss << endl;
        if (orgStructure.find(boss) != orgStructure.end())
        {
            for (const string &sub : orgStructure[boss])
            {
                displayOrgTree(sub, level + 1);
            }
        }
    }

    // --- Fitur 5: Navigasi (Graph + Dijkstra) ---
    void addBuilding(string id, string name)
    {
        buildingNames[id] = name;
    }

    void addPath(string from, string to, int distance)
    {
        campusGraph[from].push_back({distance, to});
        campusGraph[to].push_back({distance, from}); // Undirected graph
    }

    void findRoute(string startID, string endID)
    {
        map<string, int> dist;
        map<string, string> prev;
        priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> pq;

        // Inisialisasi jarak
        for (auto &b : buildingNames)
        {
            dist[b.first] = INT_MAX;
        }
        dist[startID] = 0;
        pq.push({0, startID});

        while (!pq.empty())
        {
            int d = pq.top().first;
            string u = pq.top().second;
            pq.pop();

            if (d > dist[u])
                continue;

            for (auto &edge : campusGraph[u])
            {
                int w = edge.first;
                string v = edge.second;
                if (dist[u] + w < dist[v])
                {
                    dist[v] = dist[u] + w;
                    prev[v] = u;
                    pq.push({dist[v], v});
                }
            }
        }

        if (dist[endID] == INT_MAX)
        {
            cout << "[Navigasi] Tidak ada rute yang tersedia menuju tujuan tersebut.\n";
        }
        else
        {
            cout << "[Navigasi] Rute Tercepat ke " << buildingNames[endID] << ": " << dist[endID] << " meter.\n";
        }
    }

    // --- Fitur 6: Undo KRS (Stack) ---
    void addCourse(string courseCode)
    {
        if (currentKRS.find(courseCode) == currentKRS.end())
        {
            currentKRS.insert(courseCode);
            krsStack.push(courseCode); // Simpan ke stack untuk fitur undo
            logActivity("KRS", "Menambah mata kuliah " + courseCode);
            cout << "[KRS] Mata kuliah " << courseCode << " berhasil ditambahkan.\n";
        }
        else
        {
            cout << "[KRS] Mata kuliah sudah ada dalam daftar.\n";
        }
    }

    void undoLastCourse()
    {
        if (!krsStack.empty())
        {
            string lastCourse = krsStack.top();
            krsStack.pop();
            currentKRS.erase(lastCourse);
            logActivity("KRS Undo", "Membatalkan penambahan " + lastCourse);
            cout << "[KRS] Undo berhasil: " << lastCourse << " telah dihapus.\n";
        }
        else
        {
            cout << "[KRS] Tidak ada aksi yang dapat di-undo.\n";
        }
    }

    void showKRS()
    {
        cout << "[KRS] Daftar Mata Kuliah Saat Ini: ";
        if (currentKRS.empty())
        {
            cout << "Kosong";
        }
        else
        {
            for (const auto &c : currentKRS)
                cout << c << " ";
        }
        cout << endl;
    }

    // --- Fitur 7: Prioritas Layanan (Priority Queue) ---
    void requestService(string student, int priorityLevel, string type)
    {
        servicePQ.push({student, priorityLevel, type});
        logActivity("Layanan", student + " mengajukan layanan prioritas " + to_string(priorityLevel));
        cout << "[Layanan] Permintaan layanan diterima.\n";
    }

    void processNextService()
    {
        if (!servicePQ.empty())
        {
            ServiceReq req = servicePQ.top();
            servicePQ.pop();
            cout << "[Layanan] Melayani: " << req.student
                 << " (Prioritas: " << req.prio << ", Jenis: " << req.type << ")\n";
        }
        else
        {
            cout << "[Layanan] Antrian layanan kosong.\n";
        }
    }
};

int main()
{
    SmartCampus kampus;
    int choice;

    // Setup data awal gedung untuk navigasi
    kampus.addBuilding("G1", "Gerbang Utama");
    kampus.addBuilding("G2", "Fakultas Teknik");
    kampus.addBuilding("G3", "Perpustakaan");
    kampus.addBuilding("G4", "Kantin Pusat");

    kampus.addPath("G1", "G2", 100);
    kampus.addPath("G1", "G3", 150);
    kampus.addPath("G2", "G4", 50);
    kampus.addPath("G3", "G4", 80);

    // Setup data awal organisasi
    kampus.addOrgRelation("Rektor", "Wakil Rektor Akademik");
    kampus.addOrgRelation("Rektor", "Wakil Rektor Kemahasiswaan");
    kampus.addOrgRelation("Wakil Rektor Akademik", "Dekan Fakultas Teknik");

    do
    {
        cout << "\n============================================\n";
        cout << "   SMART CAMPUS NAVIGATION SYSTEM\n";
        cout << "============================================\n";
        cout << "1. Registrasi & Login\n";
        cout << "2. Lihat Struktur Organisasi\n";
        cout << "3. Navigasi Kampus (Cari Rute)\n";
        cout << "4. Pengisian KRS (Tambah/Undo)\n";
        cout << "5. Antrian Konsultasi Dosen\n";
        cout << "6. Layanan Akademik Prioritas\n";
        cout << "7. Lihat Riwayat Aktivitas\n";
        cout << "8. Keluar\n";
        cout << "--------------------------------------------\n";
        cout << "Pilihan Menu: ";
        cin >> choice;

        if (choice == 1)
        {
            int sub;
            cout << "\n[Menu Akun]\n1. Registrasi\n2. Login\nPilihan: ";
            cin >> sub;
            if (sub == 1)
            {
                string nim, pass;
                cout << "NIM: ";
                cin >> nim;
                cout << "Password: ";
                cin >> pass;
                kampus.registerAccount(nim, pass);
            }
            else if (sub == 2)
            {
                string nim, pass;
                cout << "NIM: ";
                cin >> nim;
                cout << "Password: ";
                cin >> pass;
                kampus.login(nim, pass);
            }
        }
        else if (choice == 2)
        {
            cout << "\n[Struktur Organisasi]\n";
            kampus.displayOrgTree("Rektor");
        }
        else if (choice == 3)
        {
            string start, end;
            cout << "\n[Navigasi]\n";
            cout << "Daftar Gedung: G1(Gerbang), G2(Teknik), G3(Perpus), G4(Kantin)\n";
            cout << "Dari (ID): ";
            cin >> start;
            cout << "Ke (ID): ";
            cin >> end;
            kampus.findRoute(start, end);
        }
        else if (choice == 4)
        {
            int sub;
            cout << "\n[KRS]\n1. Tambah Matkul\n2. Undo Terakhir\n3. Lihat KRS\nPilihan: ";
            cin >> sub;
            if (sub == 1)
            {
                string code;
                cout << "Kode Matkul: ";
                cin >> code;
                kampus.addCourse(code);
            }
            else if (sub == 2)
            {
                kampus.undoLastCourse();
            }
            else if (sub == 3)
            {
                kampus.showKRS();
            }
        }
        else if (choice == 5)
        {
            int sub;
            cout << "\n[Konsultasi]\n1. Ambil Antrian\n2. Panggil Berikutnya\nPilihan: ";
            cin >> sub;
            if (sub == 1)
            {
                string nama, dosen;
                cout << "Nama Mahasiswa: ";
                cin >> nama;
                cout << "Nama Dosen: ";
                cin >> dosen;
                kampus.joinConsultationQueue(nama, dosen);
            }
            else if (sub == 2)
            {
                kampus.serveNextConsultation();
            }
        }
        else if (choice == 6)
        {
            int sub;
            cout << "\n[Layanan Prioritas]\n1. Ajukan Layanan\n2. Proses Layanan\nPilihan: ";
            cin >> sub;
            if (sub == 1)
            {
                string nama, jenis;
                int prio;
                cout << "Nama Mahasiswa: ";
                cin >> nama;
                cout << "Jenis Layanan: ";
                cin >> jenis;
                cout << "Prioritas (1-Tertinggi, 5-Terendah): ";
                cin >> prio;
                kampus.requestService(nama, prio, jenis);
            }
            else if (sub == 2)
            {
                kampus.processNextService();
            }
        }
        else if (choice == 7)
        {
            kampus.showHistory();
        }
    } while (choice != 8);

    cout << "Terima kasih telah menggunakan Smart Campus System." << endl;
    return 0;
}