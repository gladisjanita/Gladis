#include <stdio.h>

int main() {
    int pilihan = 0;
    float saldo, tarik, setor, transfer, admin;
    int jenis;

    printf("Masukkan saldo awal Anda: Rp ");
    scanf("%f", &saldo);

    while (pilihan != 5) {

        printf("\n======= MESIN ATM =======\n");
        printf("1. Cek Saldo\n");
        printf("2. Tarik Tunai\n");
        printf("3. Setor Tunai\n");
        printf("4. Transfer\n");
        printf("5. Keluar\n");
        printf("==========================\n");
        printf("Pilih menu: ");
        scanf("%d", &pilihan);

        if (pilihan == 1) {
            printf("\nSaldo Anda: Rp %.2f\n", saldo);
        }

        else if (pilihan == 2) {
            printf("\nMasukkan jumlah penarikan: Rp ");
            scanf("%f", &tarik);

            if (tarik > saldo) {
                printf("Saldo tidak cukup!\n");
            } else {
                saldo -= tarik;
                printf("Penarikan berhasil! Saldo sekarang: Rp %.2f\n", saldo);
            }
        }

        else if (pilihan == 3) {
            printf("\nMasukkan jumlah setoran: Rp ");
            scanf("%f", &setor);

            saldo += setor;
            printf("Setoran berhasil! Saldo sekarang: Rp %.2f\n", saldo);
        }

        else if (pilihan == 4) {
            printf("\nMasukkan jumlah transfer: Rp ");
            scanf("%f", &transfer);

            printf("Transfer ke:\n");
            printf("1. Sesama Bank (gratis)\n");
            printf("2. Beda Bank (biaya 6500)\n");
            printf("Pilih jenis: ");
            scanf("%d", &jenis);

            if (jenis == 1) {
                admin = 0;
            }
            else if (jenis == 2) {
                admin = 6500;
            }
            else {
                printf("Jenis transfer tidak valid!\n");
                continue;
            }

            if (transfer + admin > saldo) {
                printf("Saldo tidak cukup untuk transfer!\n");
            } else {
                saldo -= (transfer + admin);
                printf("Transfer berhasil!\n");
                printf("Biaya admin: Rp %.2f\n", admin);
                printf("Saldo sekarang: Rp %.2f\n", saldo);
            }
        }

        else if (pilihan == 5) {
            printf("\nTerima kasih sudah memakai ATM.\n");
        }

        else {
            printf("\nPilihan tidak valid!\n");
        }

    }

    return 0;
}
