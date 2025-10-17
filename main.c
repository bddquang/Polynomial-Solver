// Khai báo các thư viện
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>
#include <windows.h>
#include <string.h>
#include <ctype.h>


#define BAC_TOI_DA 10 // Bậc tối đa của đa thức
#define LAN_LAP_TOI_DA 1000 // Số lần lặp tối đa cho các phương pháp lặp

// Cấu trúc cho một đa thức
typedef struct {
    int bac;
    double he_so[BAC_TOI_DA + 1];
} DaThuc;

// Khai báo nguyên mẫu các hàm
void nhap_da_thuc(DaThuc *dt, FILE *fout);
double gia_tri(DaThuc *dt, double x);
DaThuc dao_ham(DaThuc *dt);
int nhap_dau_vao(double *a, double *b, FILE *fout, int thap_phan);
void tim_khoang_nghiem(DaThuc *dt, FILE *fout, int thap_phan);
void chia_doi(DaThuc *dt, FILE *fout, int thap_phan);
void ve_do_thi(DaThuc *dt, FILE *fout, int thap_phan);
int kiem_tra_day_cung(DaThuc *dt, double a, double b, FILE *fout);
double day_cung(DaThuc *dt, FILE *fout, int thap_phan, DaThuc dt_dao_ham, DaThuc dt_dao_ham_2);
double day_cung_eps(DaThuc *dt, double *sai_so1, double *sai_so2, FILE *fout, int thap_phan, DaThuc dt_dao_ham, DaThuc dt_dao_ham_2, int dung_boi_sai_so1);
void sub_menu(DaThuc *dt, DaThuc *dt_dao_ham, DaThuc *dt_dao_ham_2, FILE *fout, int thap_phan);
void menu(DaThuc *dt, DaThuc *dt_dao_ham, DaThuc *dt_dao_ham_2, FILE *fout, int thap_phan, int *nhap_tiep);

int main() {
    DaThuc dt;
    int thap_phan;
    FILE *fout = fopen("ketqua.txt", "w");
    if (fout == NULL) {
        printf("Loi: Khong the mo tep ketqua.txt\n");
        return 1;
    }
    setvbuf(fout, NULL, _IONBF, 0);

    while (1) {
        printf("Nhap so chu so thap phan: ");
        double temp;
        if (scanf("%lf", &temp) == 1 && temp >= 0 && temp == (int)temp) {
            thap_phan = (int)temp;
            fprintf(fout, "So chu so thap phan: %d\n", thap_phan);
            while (getchar() != '\n');   //xóa bộ nhớ đệm
            break;
        } else {
            printf("Loi: So chu so thap phan phai la so nguyen khong am. Vui long nhap lai.\n");
            fprintf(fout, "Loi: So chu so thap phan phai la so nguyen khong am. Vui long nhap lai.\n");
            while (getchar() != '\n');
        }
    }
    fprintf(fout, "-----------------\n");

    while (1) {
        printf("Nhap da thuc f(x):\n");
        nhap_da_thuc(&dt, fout);
        
        if (ferror(fout)) {
              printf("Loi: Ghi vao tep ketqua.txt that bai.\n");
              fclose(fout);
              return 1;
          }

        DaThuc dt_dao_ham = dao_ham(&dt);
        DaThuc dt_dao_ham_2 = dao_ham(&dt_dao_ham);

        printf("Da thuc vua nhap: f(x) = "); //hiển thị đa thức
        fprintf(fout, "Da thuc vua nhap: f(x) = ");
        for (int i = dt.bac; i >= 0; i--) {
            if (i != dt.bac && dt.he_so[i] >= 0) {
                printf("+");
                fprintf(fout, "+");
            }
            if (i == 0) {
                printf("%.*f", thap_phan, dt.he_so[i]);
                fprintf(fout, "%.*f", thap_phan, dt.he_so[i]);
            } else if (i == 1) {
                printf("%.*fx", thap_phan, dt.he_so[i]);
                fprintf(fout, "%.*fx", thap_phan, dt.he_so[i]);
            } else {
                printf("%.*fx^%d", thap_phan, dt.he_so[i], i);
                fprintf(fout, "%.*fx^%d", thap_phan, dt.he_so[i], i);
            }
        }
        printf("\n");
        fprintf(fout, "\n-----------------\n");
        printf("\nNhan Enter de tiep tuc toi menu...");
        while (getch() != 13);
        
	 	int nhap_tiep = 0;
        menu(&dt, &dt_dao_ham, &dt_dao_ham_2, fout, thap_phan, &nhap_tiep); // gọi hàm menu
        if (nhap_tiep==0) 
            break; //thoát chương trình
    }
    fclose(fout);
    return 0;
}

void menu(DaThuc *dt, DaThuc *dt_dao_ham, DaThuc *dt_dao_ham_2, FILE *fout, int thap_phan, int *nhap_tiep) {
    int chon = 0;
    int max_chon = 6; //số lương chọn tối đa
    int box_width = 60; //chiều rộng khung menu
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    const char *menu_items[] = {
        "0. Thoat",
        "1. Tim khoang chua nghiem",
        "2. Tim khoang phan ly nghiem (chia doi)",
        "3. Ve do thi f(x)",
        "4. Tim nghiem gan dung voi so lan lap n (day cung)",
        "5. Tim nghiem gan dung voi sai so eps (day cung)",
        "6. Nhap da thuc moi"
    };

    system("cls");
    while (1) {
        SetConsoleCursorPosition(hConsole, (COORD){0, 0});
        printf("+============ Chuong trinh tim nghiem gan dung ============+\n");
        printf("+%.*s+\n", box_width - 2, "------------------------------------------------------------");
        // In các chức năng menu, tô sáng chức năng được chọn
        for (int i = 0; i <= max_chon; i++) {
            SetConsoleTextAttribute(hConsole, i == chon ? (0 | (14 << 4)) : 7);
            printf("| %-*s |\n", box_width - 4, menu_items[i]);
        }
        SetConsoleTextAttribute(hConsole, 7);
        printf("+%.*s+\n", box_width - 2, "------------------------------------------------------------");

        printf("\nSu dung phim len/xuong de chon, Enter de xac nhan.\n");
		
		// lấy phím từ người dùng
        int key = getch(); 
        if (key == 0 || key == 224) {
            key = getch();
            if (key == 72) chon = (chon == 0) ? max_chon : chon - 1;      // Phím lên: Giảm chon, vòng lại nếu ở đầu
            else if (key == 80) chon = (chon == max_chon) ? 0 : chon + 1; // Phím xuống: Tăng chon, vòng lại nếu ở cuối
        } else if (key == 13) {                                           // Phím Enter: Xác nhận lựa chọn
            system("cls");
            fprintf(fout, "Chon chuc nang: %d\n", chon);

            switch (chon) {
                case 1:
                    tim_khoang_nghiem(dt, fout, thap_phan);
                    break;
                case 2:
                    chia_doi(dt, fout, thap_phan);
                    break;
                case 3:
                    ve_do_thi(dt, fout, thap_phan);
                    break;
                case 4:
                    day_cung(dt, fout, thap_phan, *dt_dao_ham, *dt_dao_ham_2);
                    break;
                case 5:
                    sub_menu(dt, dt_dao_ham, dt_dao_ham_2, fout, thap_phan);
                    break;
                case 6:
                    *nhap_tiep = 1;
                    fprintf(fout, "Quay lai nhap da thuc moi.\n");
                    fprintf(fout, "-----------------\n");
                    return;
                case 0:
                    *nhap_tiep = 0;
                    printf("Ket thuc chuong trinh.\n");
                    fprintf(fout, "Ket thuc chuong trinh.\n");
                    return;
            }
            if 	(chon != 0) {
                printf("\nNhan Enter de tiep tuc...");
                fprintf(fout, "-----------------\n");
                while (getch() != 13);
                system("cls");
            }
        }
    }
}

void sub_menu(DaThuc *dt, DaThuc *dt_dao_ham, DaThuc *dt_dao_ham_2, FILE *fout, int thap_phan) {
    int sub_chon = 0, max_sub_chon = 2; //số lượng chọn tối đa
    int box_width = 60;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    const char *sub_menu_items[] = {
        "0. Quay lai menu chinh",
        "1. Tinh theo sai so |x_k - x_{k-1}| < eps_0",
        "2. Tinh theo sai so |f(x_k)|/m < eps"
    };

    while (1) {
        SetConsoleCursorPosition(hConsole, (COORD){0, 0});
        printf("+=========== Tim nghiem gan dung voi sai so eps ===========+\n");
        printf("+%.*s+\n", box_width - 2, "------------------------------------------------------------");
        // In các chức năng menu phụ, tô sáng chức năng được chọn
        for (int i = 0; i <= max_sub_chon; i++) {
            SetConsoleTextAttribute(hConsole, i == sub_chon ? (0 | (14 << 4)) : 7);
            printf("| %-*s |\n", box_width - 4, sub_menu_items[i]);
        }
        SetConsoleTextAttribute(hConsole, 7);
        printf("+%.*s+\n", box_width - 2, "------------------------------------------------------------");
        printf("\nSu dung phim len/xuong de chon, Enter de xac nhan.\n");

		//Lấy phím từ người dùng
        int key = getch();
        if (key == 0 || key == 224) {
            key = getch();
            if (key == 72) sub_chon = (sub_chon == 0) ? max_sub_chon : sub_chon - 1;
            else if (key == 80) sub_chon = (sub_chon == max_sub_chon) ? 0 : sub_chon + 1;
        } else if (key == 13) {
            system("cls");
            fprintf(fout, "Chon chuc nang phu: %d\n", sub_chon);
            if (sub_chon == 0) break; //thoát menu phụ
            else if (sub_chon == 1 || sub_chon == 2) {
                double sai_so1, sai_so2;
                double nghiem = day_cung_eps(dt, &sai_so1, &sai_so2, fout, thap_phan, *dt_dao_ham, *dt_dao_ham_2, sub_chon == 1);
                if (nghiem == 0) {    // nếu lỗi, quay lại menu phụ
                    while (getch() != 13);
                    system("cls");
                    continue; 
                } else {
                    printf("Nghiem gan dung: %.*f\n", thap_phan, nghiem);
                    printf("Sai so |x_k - x_{k-1}| = %.*e\n", thap_phan, sai_so1);
                    printf("Sai so |f(x_k)|/m = %.*e\n", thap_phan, sai_so2);
                    fprintf(fout, "Nghiem gan dung: %.*f\n", thap_phan, nghiem);
                    fprintf(fout, "Sai so |x_k - x_{k-1}| = %.*e\n", thap_phan, sai_so1);
                    fprintf(fout, "Sai so |f(x_k)|/m = %.*e\n", thap_phan, sai_so2);
                    printf("\nNhan Enter de tiep tuc...");
                    fprintf(fout, "-----------------\n");
                    while (getch() != 13);
                    system("cls");
                    continue;         // quay lại menu phụ
                }
            }
        }
    }
}

void nhap_da_thuc(DaThuc *dt, FILE *fout) {
    double temp;    //biến tạm thời lưu giá trị nhập
    while (1) {
        printf("Nhap bac cua da thuc: ");
        if (scanf("%lf", &temp) == 1 && temp >= 0 && temp <= BAC_TOI_DA && temp == (int)temp) {
            dt->bac = (int)temp;  //gán bậc cho đa thức
            fprintf(fout, "Nhap bac cua da thuc: %d\n", dt->bac);
            while (getchar() != '\n');   //xóa bộ đệm khi nhập
            break;
        } else {
            printf("Loi: Bac cua da thuc phai la so nguyen khong am va khong qua %d. Vui long nhap lai.\n", BAC_TOI_DA);
            fprintf(fout, "Loi: Bac cua da thuc phai la so nguyen khong am va khong qua %d. Vui long nhap lai.\n", BAC_TOI_DA);
            while (getchar() != '\n');
        }
    }
    for (int i = dt->bac; i >= 0; i--) {
        while (1) {
            printf("Nhap he so bac %d: ", i);
            if (scanf("%lf", &dt->he_so[i]) == 1) {
                fprintf(fout, "Nhap he so bac %d: %.*f\n", i, 6, dt->he_so[i]);
                while (getchar() != '\n');
                break;
            }
            printf("Loi: He so phai la so thuc. Nhap lai.\n");
            fprintf(fout, "Loi: He so bac %d khong hop le. Nhap lai.\n", i);
            while (getchar() != '\n');
        }
    }
    fprintf(fout, "-----------------\n");
}

double gia_tri(DaThuc *dt, double x) {
    double ket_qua = 0;
    for (int i = dt->bac; i >= 0; i--) {
        ket_qua = ket_qua * x + dt->he_so[i];
    }
    return ket_qua;
}

DaThuc dao_ham(DaThuc *dt) {
    DaThuc dt_dao_ham;
    dt_dao_ham.bac = dt->bac - 1;

    if (dt_dao_ham.bac < 0) {
        dt_dao_ham.bac = 0;
        dt_dao_ham.he_so[0] = 0.0;
        return dt_dao_ham;
    }

    for (int i = 0; i <= dt_dao_ham.bac; i++) {
        dt_dao_ham.he_so[i] = (i + 1) * dt->he_so[i + 1];
    }
    return dt_dao_ham;
}

int nhap_dau_vao(double *a, double *b, FILE *fout, int thap_phan) {
    while (1) {
        printf("Nhap [a, b]: ");
        if (scanf("%lf%lf", a, b) == 2 && *a < *b) {
            fprintf(fout, "Nhap doan [a, b]: %.*f, %.*f\n", thap_phan, *a, thap_phan, *b);
            while (getchar() != '\n');  //xóa bộ nhớ đệm
            return 1;
        } else if (*a >= *b) {
            printf("Loi: a phai nho hon b. Nhap lai.\n");
            fprintf(fout, "Loi: a phai nho hon b. Nhap lai.\n");
            while (getchar() != '\n');
        } else {
            printf("Loi: Vui long nhap hai so thuc. Nhap lai.\n");
            fprintf(fout, "Loi: Nhap sai dinh dang khoang [a, b]. Nhap lai.\n");
            while (getchar() != '\n');
        }
    }
}

void tim_khoang_nghiem(DaThuc *dt, FILE *fout, int thap_phan) {
    double a, b, buoc;
    if (!nhap_dau_vao(&a, &b, fout, thap_phan)) {
        return;
    }
    while (1) {
        printf("Nhap buoc chia: ");
        if (scanf("%lf", &buoc) == 1 && buoc > 0) {
            fprintf(fout, "Nhap buoc chia: %.*f\n", thap_phan, buoc);
            while (getchar() != '\n');
            break;  
        }
        printf("Loi: Buoc chia phai la so duong. Nhap lai.\n");
        fprintf(fout, "Loi: Buoc chia khong hop le. Nhap lai.\n");
        while (getchar() != '\n');
    }
    double x1 = a, x2 = a + buoc;
    int tim_thay = 0;  //báo hiệu tìm thấy nghiệm
    printf("Cac khoang chua nghiem tren [%.*f, %.*f]:\n", thap_phan, a, thap_phan, b);
    fprintf(fout, "Cac khoang chua nghiem tren [%.*f, %.*f]:\n", thap_phan, a, thap_phan, b);
    while (x2 <= b) {
        double y1 = gia_tri(dt, x1);
        double y2 = gia_tri(dt, x2);
        if (y1 * y2 <= 0) {
            printf("Khoang (%.*f, %.*f) chua nghiem\n", thap_phan, x1, thap_phan, x2);
            fprintf(fout, "Khoang (%.*f, %.*f) chua nghiem\n", thap_phan, x1, thap_phan, x2);
            tim_thay = 1;  //cập nhật tìm thấy 
        }
        x1 = x2;
        x2 += buoc;
    }
    if (!tim_thay) {  //nếu không tìm thấy 
        printf("Khong tim thay khoang chua nghiem tren [%.*f, %.*f]\n", thap_phan, a, thap_phan, b);
        fprintf(fout, "Khong tim thay khoang chua nghiem tren [%.*f, %.*f]\n", thap_phan, a, thap_phan, b);
    }
}

void chia_doi(DaThuc *dt, FILE *fout, int thap_phan) {
    double a, b;
    if (!nhap_dau_vao(&a, &b, fout, thap_phan)) {
        return;
    }
    double fa = gia_tri(dt, a);
    double fb = gia_tri(dt, b);
    if (fa * fb > 0) {
        printf("Khoang (%.*f, %.*f) khong phai khoang phan ly nghiem!\n", thap_phan, a, thap_phan, b);
        fprintf(fout, "Khoang (%.*f, %.*f) khong phai khoang phan ly nghiem!\n", thap_phan, a, thap_phan, b);
        fprintf(fout, "-----------------\n");
        return;
    }
    printf("Thu hep khoang phan ly nghiem:\n");
    fprintf(fout, "Thu hep khoang phan ly nghiem:\n");
    while (fabs(a - b) > 0.5) {  // Lặp cho đến khi khoảng nhỏ hơn 0.5
        double c = (a + b) / 2;
        double fc = gia_tri(dt, c);
        printf("a=%.*f, b=%.*f, c=%.*f, f(c)=%.*f\n", thap_phan, a, thap_phan, b, thap_phan, c, thap_phan, fc);
        fprintf(fout, "a=%.*f, b=%.*f, c=%.*f, f(c)=%.*f\n", thap_phan, a, thap_phan, b, thap_phan, c, thap_phan, fc);
        if (fa * fc < 0) {
            b = c;
            fb = fc;
        } else {
            a = c;
            fa = fc;
        }
    }
    printf("Khoang phan ly nghiem: (%.*f, %.*f)\n", thap_phan, a, thap_phan, b);
    fprintf(fout, "Khoang phan ly nghiem: (%.*f, %.*f)\n", thap_phan, a, thap_phan, b);
}

void ve_do_thi(DaThuc *dt, FILE *fout, int thap_phan) {
    double a, b;
    if (!nhap_dau_vao(&a, &b, fout, thap_phan)) {
        return;
    }

    int rong = 80, cao = 24;  //kích thước khung đồ thị
    char canvas[24][85];   	  //mảng lưu kí tự đồ thị
    double giatri_x[80], giatri_y[80];  //mảng lưu giá trị x và y
    double x_min = a, x_max = b;
    double y_min = 1e9, y_max = -1e9;
	// Tính giá trị f(x) tại các điểm trên trục x và tìm y_min, y_max
    for (int i = 0; i < rong; i++) {
        double x = x_min + (x_max - x_min) * i / (rong - 1); // Tính x theo tỷ lệ
        double y = gia_tri(dt, x);                           // tính f(x)
        giatri_x[i] = x;
        giatri_y[i] = y;
        if (y < y_min) y_min = y;
        if (y > y_max) y_max = y;
    }

    if (fabs(y_max - y_min) < 1e-12) {
        y_max += 1;
        y_min -= 1;
    }

    for (int i = 0; i < cao; i++) {     //tạo khung bằng khoảng trống
        for (int j = 0; j < rong; j++)
            canvas[i][j] = ' ';
        canvas[i][rong] = '\0';
    }

    int y0 = (int)((0 - y_min) / (y_max - y_min) * (cao - 1));   //vị trí y=0
    if (y0 >= 0 && y0 < cao)
        for (int j = 0; j < rong; j++)
            canvas[cao - 1 - y0][j] = '-';                       //vẽ trục hoành

    int x0 = (int)((0 - x_min) / (x_max - x_min) * (rong - 1));  //vị trí x=0
    if (x0 >= 0 && x0 < rong)
        for (int i = 0; i < cao; i++)
            canvas[i][x0] = '|';                                 //vẽ trục tung
	//vẽ đồ thị f(x)
    for (int i = 0; i < rong; i++) {
        int y_pos = (int)((giatri_y[i] - y_min) / (y_max - y_min) * (cao - 1)); //tính vị trí y trên khung
        y_pos = cao - 1 - y_pos;
        if (y_pos >= 0 && y_pos < cao) {
            if (canvas[y_pos][i] == '|') canvas[y_pos][i] = '+';      //giao điểm với Ox
            else if (canvas[y_pos][i] == '-') canvas[y_pos][i] = '+'; //giao điểm với Oy
            else canvas[y_pos][i] = '*';                              //điểm của đồ thị
        }
    }
	//In đồ thị
    printf("Do thi ASCII cua f(x) tren [%.*f, %.*f]:\n\n", thap_phan, x_min, thap_phan, x_max);
    fprintf(fout, "Do thi ASCII cua f(x) tren [%.*f, %.*f]:\n\n", thap_phan, x_min, thap_phan, x_max);
    for (int i = 0; i < cao; i++) {
        double y_label = y_max - i * (y_max - y_min) / (cao - 1);     //in kí hiệu trục Oy
        printf("%*.*f |%s\n", thap_phan + 3, thap_phan, y_label, canvas[i]);
        fprintf(fout, "%*.*f |%s\n", thap_phan + 3, thap_phan, y_label, canvas[i]);
    }
	//in kí hiệu trục Ox
    printf("        ");
    fprintf(fout, "        ");
    for (int j = 0; j < rong; j += 10) {
        printf("%-*.*f", thap_phan + 5, thap_phan, x_min + (x_max - x_min) * j / (rong - 1));
        fprintf(fout, "%-*.*f", thap_phan + 5, thap_phan, x_min + (x_max - x_min) * j / (rong - 1));
    }
    printf("\nTruc tung (|), truc hoanh (-), do thi (*), giao diem (+)\n");
    fprintf(fout, "\nTruc tung (|), truc hoanh (-), do thi (*), giao diem (+)\n");
}

int kiem_tra_day_cung(DaThuc *dt, double a, double b, FILE *fout) {
    double fa = gia_tri(dt, a);
    double fb = gia_tri(dt, b);
    if (fa == 0 || fb == 0) {
        printf("Loi: f(a) hoac f(b) bang 0.\n");
        fprintf(fout, "Loi: f(a) hoac f(b) bang 0.\n");
        printf("\nNhan Enter de tiep tuc...");
        fprintf(fout, "-----------------\n");
        return 0;
    }
    if (fa * fb > 0) {
        printf("Loi: f(a) va f(b) cung dau.\n");
        fprintf(fout, "Loi: f(a) va f(b) cung dau.\n");
        printf("\nNhan Enter de tiep tuc...");
        fprintf(fout, "-----------------\n");
        return 0;
    }

    return 1;
}

double day_cung(DaThuc *dt, FILE *fout, int thap_phan, DaThuc dt_dao_ham, DaThuc dt_dao_ham_2) {
    double a, b;
    int lan_lap;
    if (!nhap_dau_vao(&a, &b, fout, thap_phan)) {
        return 0;
    }
    if (!kiem_tra_day_cung(dt, a, b, fout)) {
        return 0;
    }
    while (1) {
    printf("Nhap so lan lap n: ");
    if (scanf("%d", &lan_lap) == 1 && lan_lap >= 0) {
        fprintf(fout, "Nhap so lan lap n: %d\n", lan_lap);
        while (getchar() != '\n');
        break;
    }
    printf("Loi: So lan lap phai la so nguyen khong am. Nhap lai.\n");
    fprintf(fout, "Loi: So lan lap phai la so nguyen khong am. Nhap lai.\n");
    while (getchar() != '\n');
	}

    double d, x_0;
    double fa = gia_tri(dt, a);
    double f_dao_ham_2_a = gia_tri(&dt_dao_ham_2, a);
	// Chọn điểm d và điểm bắt đầu x_0 dựa trên dấu f(a) * f''(a)
    if (fa * f_dao_ham_2_a > 0) {
        d = a;
        x_0 = b;
    } else {
        d = b;
        x_0 = a;
    }

    double f1_a = gia_tri(&dt_dao_ham, a);
    double f1_b = gia_tri(&dt_dao_ham, b);
    double m = fmin(fabs(f1_a), fabs(f1_b));
    double M = fmax(fabs(f1_a), fabs(f1_b));

    if (M == 0) {
        printf("Loi: M = 0, khong the tinh sai so |x_k - x_{k-1}| * (M - m)/M.\n");
        fprintf(fout, "Loi: M = 0, khong the tinh sai so |x_k - x_{k-1}| * (M - m)/M.\n");
        return x_0;
    }

    printf("Phuong phap day cung voi n=%d (d=%.*f, x0=%.*f, m=%.*e, M=%.*e):\n\n", lan_lap, thap_phan, d, thap_phan, x_0, thap_phan, m, thap_phan, M);
    fprintf(fout, "Phuong phap day cung voi n=%d (d=%.*f, x0=%.*f, m=%.*e, M=%.*e):\n\n", lan_lap, thap_phan, d, thap_phan, x_0, thap_phan, m, thap_phan, M);
    //In bảng giá trị
    printf("%-5s | %-*s | %-*s | %-*s | %-*s\n", "Lap", thap_phan + 10, "x_k", thap_phan + 10, "f(x_k)", thap_phan + 30, "Sai so |x_k - x_{k-1}|*(M-m)/M", thap_phan + 10, "Sai so |f(x_k)|/m");
    fprintf(fout, "%-5s | %-*s | %-*s | %-*s | %-*s\n", "Lap", thap_phan + 10, "x_k", thap_phan + 10, "f(x_k)", thap_phan + 30, "Sai so |x_k - x_{k-1}|*(M-m)/M", thap_phan + 10, "Sai so |f(x_k)|/m");
    printf("----------------------------------------------------------------------------------------------------------------\n");
    fprintf(fout, "----------------------------------------------------------------------------------------------------------------\n");

    double sai_so1, sai_so2;
    for (int i = 1; i <= lan_lap; i++) {
        double f_d = gia_tri(dt, d);
        double f_x0 = gia_tri(dt, x_0);

        if (f_d == f_x0) {
            printf("Khong the tinh: Mau so (f(d) - f(x_0)) bang 0.\n");
            fprintf(fout, "Khong the tinh: Mau so (f(d) - f(x_0)) bang 0.\n");
            break;
        }

        double x_tiep = x_0 - f_x0 * (d - x_0) / (f_d - f_x0);

        double f_x_tiep = gia_tri(dt, x_tiep);
        sai_so1 = fabs(x_tiep - x_0) * (M - m) / M;
        sai_so2 = fabs(f_x_tiep) / m;
		//in kết quả mỗi lần lặp
        printf("%-5d | %-*.*f | %-*.*f | %-*.*e | %-*.*e\n", i,
               thap_phan + 10, thap_phan, x_tiep,
               thap_phan + 10, thap_phan, f_x_tiep,
               thap_phan + 30, thap_phan, sai_so1,
               thap_phan + 10, thap_phan, sai_so2);
        fprintf(fout, "%-5d | %-*.*f | %-*.*f | %-*.*e | %-*.*e\n", i,
               thap_phan + 10, thap_phan, x_tiep,
               thap_phan + 10, thap_phan, f_x_tiep,
               thap_phan + 30, thap_phan, sai_so1,
               thap_phan + 10, thap_phan, sai_so2);
        x_0 = x_tiep;  //cập nhật x0
    }
    printf("----------------------------------------------------------------------------------------------------------------\n");
    fprintf(fout, "----------------------------------------------------------------------------------------------------------------\n");
    //In nghiệm cuối cùng
    printf("Nghiem gan dung: %.*f\n", thap_phan, x_0);
    printf("Sai so |x_n - x_{n-1}|*(M-m)/M = %.*e\n", thap_phan, sai_so1);
    printf("Sai so |f(x_n)|/m = %.*e\n", thap_phan, sai_so2);
    fprintf(fout, "Nghiem gan dung: %.*f\n", thap_phan, x_0);
    fprintf(fout, "Sai so |x_n - x_{n-1}|*(M-m)/M = %.*e\n", thap_phan, sai_so1);
    fprintf(fout, "Sai so |f(x_n)|/m = %.*e\n", thap_phan, sai_so2);
    return x_0;
}

double day_cung_eps(DaThuc *dt, double *sai_so1, double *sai_so2, FILE *fout, int thap_phan, DaThuc dt_dao_ham, DaThuc dt_dao_ham_2, int dung_boi_sai_so1) {
    double a, b, eps;
    if (!nhap_dau_vao(&a, &b, fout, thap_phan)) {
        return 0;
    }
    if (!kiem_tra_day_cung(dt, a, b, fout)) {
        return 0;
    }

    while (1) {
        printf("Nhap sai so eps: ");
        if (scanf("%lf", &eps) == 1 && eps > 0) {
            fprintf(fout, "Nhap sai so eps: %.*e\n", thap_phan, eps);
            while (getchar() != '\n'); 
            break;
        }
        printf("Loi: Sai so eps phai la so duong. Nhap lai.\n");
        fprintf(fout, "Loi: Sai so eps phai la so duong.\n");
        while (getchar() != '\n');
    }

    double d, x_0;
    double fa = gia_tri(dt, a);
    double f_dao_ham_2_a = gia_tri(&dt_dao_ham_2, a);
	// Chọn điểm d và điểm bắt đầu x_0 dựa trên dấu f(a) * f''(a)
    if (fa * f_dao_ham_2_a > 0) {
        d = a;
        x_0 = b;
    } else {
        d = b;
        x_0 = a;
    }

    double f1_a = gia_tri(&dt_dao_ham, a);
    double f1_b = gia_tri(&dt_dao_ham, b);
    double m = fmin(fabs(f1_a), fabs(f1_b));
    double M = fmax(fabs(f1_a), fabs(f1_b));

    double eps_0 = 0;
    if (M == m) {
        printf("Khong the tinh: M va m bang nhau.\n");
        fprintf(fout, "Khong the tinh: M va m bang nhau.\n");
        printf("Nhan ENTER de tiep tuc");
        eps_0 = eps;
        return 0;
    } else {
        eps_0 = (eps * m) / (M - m);
    }

    printf("Phuong phap day cung voi eps=%.*f (d=%.*f, x0=%.*f, m=%.*e, eps_0=%.*e):\n\n", thap_phan, eps, thap_phan, d, thap_phan, x_0, thap_phan, m, thap_phan, eps_0);
    fprintf(fout, "Phuong phap day cung voi eps=%.*f (d=%.*f, x0=%.*f, m=%.*e, eps_0=%.*e):\n\n", thap_phan, eps, thap_phan, d, thap_phan, x_0, thap_phan, m, thap_phan, eps_0);
    //in bảng giá trị
    printf("%-5s | %-*s | %-*s | %-*s | %-*s\n", "Lap", thap_phan + 10, "x_k", thap_phan + 10, "f(x_k)", thap_phan + 30, "Sai so |x_k - x_{k-1}|", thap_phan + 10, "Sai so |f(x_k)|/m");
    fprintf(fout, "%-5s | %-*s | %-*s | %-*s | %-*s\n", "Lap", thap_phan + 10, "x_k", thap_phan + 10, "f(x_k)", thap_phan + 30, "Sai so |x_k - x_{k-1}|", thap_phan + 10, "Sai so |f(x_k)|/m");
    printf("----------------------------------------------------------------------------------------------------------------\n");
    fprintf(fout, "----------------------------------------------------------------------------------------------------------------\n");

    int lan_lap = 1;  //đếm số lần lặp
    // Lặp cho đến khi đạt sai số yêu cầu
    do {
        double f_d = gia_tri(dt, d);
        double f_x0 = gia_tri(dt, x_0);

        if (f_d == f_x0) {
            printf("Khong the tinh: Mau so (f(d) - f(x_0)) bang 0.\n");
            fprintf(fout, "Khong the tinh: Mau so (f(d) - f(x_0)) bang 0.\n");
            break;
        }

        double x_tiep = x_0 - f_x0 * (d - x_0) / (f_d - f_x0);

        *sai_so1 = fabs(x_tiep - x_0);
        double f_x_tiep = gia_tri(dt, x_tiep);
        *sai_so2 = fabs(f_x_tiep) / m;
		//in kết quả mỗi lần lặp
        printf("%-5d | %-*.*f | %-*.*f | %-*.*e | %-*.*e\n", lan_lap,
               thap_phan + 10, thap_phan, x_tiep,
               thap_phan + 10, thap_phan, f_x_tiep,
               thap_phan + 30, thap_phan, *sai_so1,
               thap_phan + 10, thap_phan, *sai_so2);
        fprintf(fout, "%-5d | %-*.*f | %-*.*f | %-*.*e | %-*.*e\n", lan_lap,
               thap_phan + 10, thap_phan, x_tiep,
               thap_phan + 10, thap_phan, f_x_tiep,
               thap_phan + 30, thap_phan, *sai_so1,
               thap_phan + 10, thap_phan, *sai_so2);

        x_0 = x_tiep;
        lan_lap++;
        if (lan_lap > LAN_LAP_TOI_DA) {
            printf("Canh bao: Vuot qua so lan lap toi da (%d).\n", LAN_LAP_TOI_DA);
            fprintf(fout, "Canh bao: Vuot qua so lan lap toi da (%d).\n", LAN_LAP_TOI_DA);
            break;
        }
    } while ((dung_boi_sai_so1 ? *sai_so1 >= eps_0 : *sai_so2 >= eps)); // Điều kiện dừng dựa trên sai_so1 hoặc sai_so2

    printf("----------------------------------------------------------------------------------------------------------------\n");
    fprintf(fout, "----------------------------------------------------------------------------------------------------------------\n");
    //In lý do dừng lặp
    // Dừng do sai_so1
    if (dung_boi_sai_so1 && *sai_so1 < eps_0) {  
        printf("Dung tai lan lap %d vi |x_k - x_{k-1}| = %.*e < eps_0 = %.*e\n\n", lan_lap - 1, thap_phan, *sai_so1, thap_phan, eps_0);
        fprintf(fout, "Dung tai lan lap %d vi |x_k - x_{k-1}| = %.*e < eps_0 = %.*e\n\n", lan_lap - 1, thap_phan, *sai_so1, thap_phan, eps_0);
    // Dừng do sai_so2
    } else if (!dung_boi_sai_so1 && *sai_so2 < eps) {   
        printf("Dung tai lan lap %d vi |f(x_k)|/m = %.*e < eps = %.*e\n\n", lan_lap - 1, thap_phan, *sai_so2, thap_phan, eps);
        fprintf(fout, "Dung tai lan lap %d vi |f(x_k)|/m = %.*e < eps = %.*e\n\n", lan_lap - 1, thap_phan, *sai_so2, thap_phan, eps);
    //Dừng do lỗi
    } else {
        printf("Dung tai lan lap %d do vuot qua so lan lap toi da hoac mau so bang 0.\n\n", lan_lap - 1);
        fprintf(fout, "Dung tai lan lap %d do vuot qua so lan lap toi da hoac mau so bang 0.\n\n", lan_lap - 1);
    }
    return x_0;
}