#include <iostream>
#include <Windows.h>
using namespace std;

enum RENK {
    RENK_SÝYAH = 0,
    RENK_KOYUMAVI = 1,
    RENK_KOYUYESIL = 2,
    RENK_AQUA = 3,
    RENK_KAHVERENGI = 4,
    RENK_KOYUMOR = 5,
    RENK_ACIKKAHVE = 6,
    RENK_GRI = 7,
    RENK_KOYUGRI = 8,
    RENK_MAVI = 9,
    RENK_YESIL = 10,
    RENK_ACIKMAVI = 11,
    RENK_KIRMIZI = 12,
    RENK_MOR = 13,
    RENK_SARI = 14,
    RENK_BEYAZ = 15
};

enum YON {
    YON_SOL = 1,
    YON_SAG = 2,
    YON_YUKARI = 3,
    YON_ASAGI = 4
};

struct YilanHucre {
    int x;
    int y;
    int yon;
    char karakter;
};

const int genislik = 80;
const int yukseklik = 20;
const int maxYilanUzunlugu = 500;
const char yilanKarakteri = 219;
int kuyrukUzunlugu = 0;
char sahne[genislik][yukseklik];
char tuslar[256];

YilanHucre yilanKuyrugu[maxYilanUzunlugu];

// Sahne sýnýfý
class Sahne {
public:
    static const int genislik = 80;
    static const int yukseklik = 20;
    char sahne[genislik][yukseklik];

    void sahneyiTemizle() {
        for (int y = 0; y < yukseklik; y++) {
            for (int x = 0; x < genislik; x++) {
                sahne[x][y] = ' ';
            }
        }
    }

    void sahneyiCiz() {
        for (int y = 0; y < yukseklik; y++) {
            for (int x = 0; x < genislik; x++) {
                cout << sahne[x][y];
            }
            cout << endl;
        }
    }

    void sinirlariOlustur() {
        for (int x = 0; x < genislik; x++) {
            sahne[x][0] = 176;
            sahne[x][yukseklik - 1] = 176;
        }
        for (int y = 0; y < yukseklik; y++) {
            sahne[0][y] = 176;
            sahne[genislik - 1][y] = 176;
        }
    }

    void gotoxy(int x, int y) {
        COORD coord;
        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }

    void kursoruGizle() {
        HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(out, &cursorInfo);
        cursorInfo.bVisible = false;
        SetConsoleCursorInfo(out, &cursorInfo);
    }

    void renkAta(RENK arkaPlan, RENK yazirengi) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), arkaPlan * 16 + yazirengi);
    }
};

// Yilan sýnýfý, Sahne sýnýfýndan kalýtým alýr
class Yilan : public Sahne {
public:
    YilanHucre yilanKuyrugu[maxYilanUzunlugu];

    void yilanOlustur() {
        kuyrukUzunlugu = 1;
        yilanKuyrugu[0].x = 20;
        yilanKuyrugu[0].y = 10;
        yilanKuyrugu[0].yon = YON_SAG;
        yilanKuyrugu[0].karakter = yilanKarakteri;
        yilanKuyrugunaEkle();
        yilanKuyrugunaEkle();
    }

    void yilanKuyrugunaEkle() {
        if (kuyrukUzunlugu == maxYilanUzunlugu)
            return;

        int x = yilanKuyrugu[kuyrukUzunlugu - 1].x;
        int y = yilanKuyrugu[kuyrukUzunlugu - 1].y;
        int yon = yilanKuyrugu[kuyrukUzunlugu - 1].yon;
        char kar = yilanKuyrugu[kuyrukUzunlugu - 1].karakter;

        switch (yilanKuyrugu[kuyrukUzunlugu - 1].yon) {
        case YON_SAG:
            x--;
            break;
        case YON_SOL:
            x++;
            break;
        case YON_ASAGI:
            y--;
            break;
        case YON_YUKARI:
            y++;
            break;
        }
        yilanKuyrugu[kuyrukUzunlugu].x = x;
        yilanKuyrugu[kuyrukUzunlugu].y = y;
        yilanKuyrugu[kuyrukUzunlugu].karakter = kar;
        yilanKuyrugu[kuyrukUzunlugu].yon = yon;
        kuyrukUzunlugu++;
    }

    void yilaniHareketEttir() {
        for (int i = 0; i < kuyrukUzunlugu; i++) {
            switch (yilanKuyrugu[i].yon) {
            case YON_SAG:
                yilanKuyrugu[i].x++;
                break;
            case YON_SOL:
                yilanKuyrugu[i].x--;
                break;
            case YON_ASAGI:
                yilanKuyrugu[i].y++;
                break;
            case YON_YUKARI:
                yilanKuyrugu[i].y--;
                break;
            }
        }
        for (int i = kuyrukUzunlugu - 1; i > 0; i--) {
            yilanKuyrugu[i].yon = yilanKuyrugu[i - 1].yon;
        }
    }

    void yilaniSahneyeYerlestir() {
        for (int i = 0; i < kuyrukUzunlugu; i++) {
            int x = yilanKuyrugu[i].x;
            int y = yilanKuyrugu[i].y;
            sahne[x][y] = yilanKuyrugu[i].karakter;
        }
    }

    void klavyeKontrol() {
        for (int x = 0; x < 256; x++) {
            tuslar[x] = (char)(GetAsyncKeyState(x) >> 8);
        }

        if (tuslar['A'] != 0) {
            yilanKuyrugu[0].yon = YON_SOL;
        }
        if (tuslar['D'] != 0) {
            yilanKuyrugu[0].yon = YON_SAG;
        }
        if (tuslar['W'] != 0) {
            yilanKuyrugu[0].yon = YON_YUKARI;
        }
        if (tuslar['S'] != 0) {
            yilanKuyrugu[0].yon = YON_ASAGI;
        }
        if (tuslar['P'] != 0) {
            yilanKuyrugunaEkle();
        }
    }
};

// Main fonksiyonu
int main() {
   
    Yilan yilan;
    
    yilan.sahneyiTemizle();
    yilan.sinirlariOlustur();
    yilan.yilanOlustur(); // Yýlaný oluþturuyoruz

    while (true) {
        yilan.kursoruGizle();
        yilan.sahneyiTemizle();
        yilan.sinirlariOlustur();
        yilan.klavyeKontrol();
        yilan.yilaniHareketEttir();
        yilan.yilaniSahneyeYerlestir();

        yilan.gotoxy(0, 0);
        yilan.sahneyiCiz();
        yilan.renkAta(RENK_KOYUGRI, RENK_YESIL);
        Sleep(30); // 30 milisaniye bekle
    }

    return 0;
}
