#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
    char marca[50];
    char model[50];
    int an;
    int locuri;
    char motorizare[50];
    char combustibil[50];
    int rezervat;
} Vehicul;

void eliberareTerminal() {
    system("cls");
}

void afisareSubMeniu() {
    int optiune;
    do {
        printf("\n--- Gestionare Flota ---\n");
        printf("1. Adaugare vehicul\n");
        printf("2. Actualizare vehicul\n");
        printf("3. Eliminare vehicul\n");
        printf("4. Vizualizare autoturisme disponibile\n");
        printf("5. Rezervare autoturism\n");
        printf("6. Anulare rezervare\n");
        printf("7. Inapoi\n");
        printf("Alege o optiune: ");
        scanf("%d", &optiune);
        getchar();
        switch (optiune) {
            case 1:
                printf("Ati ales optiunea de adaugare vehicul.\n");
                Vehicul v;
                printf("Introduceti marca vehiculului: ");
                fgets(v.marca, sizeof(v.marca), stdin);
                v.marca[strcspn(v.marca, "\n")] = 0;
                printf("Introduceti modelul vehiculului: ");
                fgets(v.model, sizeof(v.model), stdin);
                v.model[strcspn(v.model, "\n")] = 0;
                printf("Introduceti anul vehiculului: ");
                scanf("%d", &v.an);
                getchar();
                printf("Introduceti numarul de locuri: ");
                scanf("%d", &v.locuri);
                getchar();
                printf("Introduceti motorizarea vehiculului (ex: 2.0L, 180 HP): ");
                fgets(v.motorizare, sizeof(v.motorizare), stdin);
                v.motorizare[strcspn(v.motorizare, "\n")] = 0;
                printf("Introduceti tipul de combustibil (Benzina/Diesel): ");
                fgets(v.combustibil, sizeof(v.combustibil), stdin);
                v.combustibil[strcspn(v.combustibil, "\n")] = 0;
                v.rezervat = 0;
                FILE *f = fopen("statistici.txt", "a");
                fprintf(f, "Marca:%s, Model:%s(%d), Locuri:%d, Motorizare:%s, %s, Rezervat:%s\n",
                        v.marca, v.model, v.an, v.locuri, v.motorizare, v.combustibil, v.rezervat ? "Da" : "Nu");
                fclose(f);


                printf("\033[1;32mVehicul adaugat cu succes in fisier!\n\033[0m");
                break;

            case 2: {
                printf("Ati ales optiunea de actualizare vehicul.\n");

                char marcaCautata[50], modelCautat[50];
                printf("Introduceti marca vehiculului de actualizat: ");
                fgets(marcaCautata, sizeof(marcaCautata), stdin);
                marcaCautata[strcspn(marcaCautata, "\n")] = 0;
                printf("Introduceti modelul vehiculului de actualizat: ");
                fgets(modelCautat, sizeof(modelCautat), stdin);
                modelCautat[strcspn(modelCautat, "\n")] = 0;

                FILE *fin = fopen("statistici.txt", "r");
                FILE *fout = fopen("temp.txt", "w");
                char linie[200];
                int gasit = 0;
                while (fgets(linie, sizeof(linie), fin)) {
                    if (!gasit && strstr(linie, marcaCautata) && strstr(linie, modelCautat)) {
                        gasit = 1;
                        Vehicul v;
                        printf("Introduceti NOUA marca: ");
                        fgets(v.marca, sizeof(v.marca), stdin);
                        v.marca[strcspn(v.marca, "\n")] = 0;

                        printf("Introduceti NOUL model: ");
                        fgets(v.model, sizeof(v.model), stdin);
                        v.model[strcspn(v.model, "\n")] = 0;

                        printf("Introduceti NOUL an: ");
                        scanf("%d", &v.an); getchar();

                        printf("Introduceti NOUL numar de locuri: ");
                        scanf("%d", &v.locuri); getchar();

                        printf("Introduceti cilindreea (ex: 2.0L): ");
                        fgets(v.motorizare, sizeof(v.motorizare), stdin);
                        v.motorizare[strcspn(v.motorizare, "\n")] = 0;

                        printf("Introduceti NOUL combustibil: ");
                        fgets(v.combustibil, sizeof(v.combustibil), stdin);
                        v.combustibil[strcspn(v.combustibil, "\n")] = 0;

                        v.rezervat = 0;
                        fprintf(fout, "Marca:%s, Model:%s(%d), Locuri:%d, Motorizare:%s, %s, Rezervat:%s\n",
                                v.marca, v.model, v.an, v.locuri, v.motorizare, v.combustibil, v.rezervat ? "Da" : "Nu");
                    } else {
                        fputs(linie, fout);
                    }
                }

                fclose(fin);
                fclose(fout);
                remove("statistici.txt");
                rename("temp.txt", "statistici.txt");

                if (gasit)
                    printf("\033[1;32mVehicul actualizat cu succes.\n\033[0m");
                else
                    printf("\033[1;31mVehiculul nu a fost gasit.\n\033[0m");
                break;
            }
            case 3: {
                printf("Ati ales optiunea de eliminare vehicul.\n");

                FILE *fin = fopen("statistici.txt", "r");
                if (!fin) {
                    printf("\033[1;31mEroare la deschiderea fisierului.\n\033[0m");
                    break;
                }

                char vehicule[100][200];
                int nrVehicule = 0;

                printf("\n--- Lista vehicule ---\n");
                while (fgets(vehicule[nrVehicule], sizeof(vehicule[nrVehicule]), fin)) {
                    printf("%d. %s", nrVehicule + 1, vehicule[nrVehicule]);
                    nrVehicule++;
                }
                fclose(fin);

                if (nrVehicule == 0) {
                    printf("\033[1;31mNu exista vehicule inregistrate.\n\033[0m");
                    break;
                }

                int index;
                printf("Introduceti numarul vehiculului de eliminat (1-%d): ", nrVehicule);
                scanf("%d", &index); getchar();

                if (index < 1 || index > nrVehicule) {
                    printf("\033[1;31mIndex invalid.\n\033[0m");
                    break;
                }

                FILE *fout = fopen("temp.txt", "w");
                if (!fout) {
                    printf("\033[1;31mEroare la scrierea fisierului temporar.\n\033[0m");
                    break;
                }

                for (int i = 0; i < nrVehicule; i++) {
                    if (i != index - 1) {
                        fputs(vehicule[i], fout);
                    }
                }

                fclose(fout);
                remove("statistici.txt");
                rename("temp.txt", "statistici.txt");

                printf("\033[1;32mVehiculul #%d a fost eliminat cu succes.\n\033[0m", index);
                break;
            }
            case 4: {
                printf("Ati ales optiunea de vizualizare autoturisme disponibile.\n");

                FILE *f = fopen("statistici.txt", "r");
                if (!f) {
                    printf("\033[1;31mEroare la deschiderea fisierului.\n\033[0m");
                    break;
                }

                char linie[200];
                int nr = 0;

                printf("\n--- Autoturisme disponibile ---\n");
                while (fgets(linie, sizeof(linie), f)) {
                    if (strstr(linie, "Rezervat:Nu")) {
                        printf("%d. %s", ++nr, linie);
                    }
                }
                fclose(f);

                if (nr == 0)
                    printf("\033[1;31mNu exista autoturisme disponibile momentan.\n\033[0m");

                break;
            }
            case 5: {
                printf("Ati ales optiunea de a rezerva un autoturism.\n");

                FILE *fin = fopen("statistici.txt", "r");
                char vehicule[100][200];
                int indexuriOriginale[100];
                int nrDisponibile = 0, linieTotala = 0;
                char linie[200];
                printf("\n--- Autoturisme disponibile pentru rezervare ---\n");
                while (fgets(linie, sizeof(linie), fin)) {
                    if (strstr(linie, "Rezervat:Nu")) {
                        strcpy(vehicule[nrDisponibile], linie);
                        indexuriOriginale[nrDisponibile] = linieTotala;
                        printf("%d. %s", nrDisponibile + 1, linie);
                        nrDisponibile++;
                    }
                    linieTotala++;
                }
                fclose(fin);

                if (nrDisponibile == 0) {
                    printf("\033[1;31mNu exista autoturisme disponibile pentru rezervare.\n\033[0m");
                    break;
                }

                int opt;
                printf("Introdu numarul autoturismului pe care vrei sa-l rezervi: ");
                scanf("%d", &opt); getchar();

                if (opt < 1 || opt > nrDisponibile) {
                    printf("\033[1;31mOptiune invalida.\n\033[0m");
                    break;
                }
                FILE *fin2 = fopen("statistici.txt", "r");
                FILE *fout = fopen("temp.txt", "w");
                if (!fin2 || !fout) {
                    printf("\033[1;31mEroare la deschiderea fisierelor pentru actualizare.\n\033[0m");
                    if (fin2) fclose(fin2);
                    if (fout) fclose(fout);
                    break;
                }

                int linieCurenta = 0;
                while (fgets(linie, sizeof(linie), fin2)) {
                    if (linieCurenta == indexuriOriginale[opt - 1]) {
                        char *poz = strstr(linie, "Rezervat:Nu");
                        if (poz) {
                            strncpy(poz, "Rezervat:Da", strlen("Rezervat:Da"));
                        }
                    }
                    fputs(linie, fout);
                    linieCurenta++;
                }

                fclose(fin2);
                fclose(fout);
                remove("statistici.txt");
                rename("temp.txt", "statistici.txt");

                printf("\033[1;32mAutoturismul a fost rezervat cu succes!\n\033[0m");
                break;
            }
            case 6: {
                printf("Ati ales optiunea de a anula rezervarea.\n");

                FILE *fin = fopen("statistici.txt", "r");
                char vehicule[100][200];
                int indexuriOriginale[100];
                int nrRezervate = 0, linieTotala = 0;
                char linie[200];

                printf("\n--- Autoturisme rezervate ---\n");
                while (fgets(linie, sizeof(linie), fin)) {
                    if (strstr(linie, "Rezervat:Da")) {
                        strcpy(vehicule[nrRezervate], linie);
                        indexuriOriginale[nrRezervate] = linieTotala;
                        printf("%d. %s", nrRezervate + 1, linie);
                        nrRezervate++;
                    }
                    linieTotala++;
                }
                fclose(fin);

                if (nrRezervate == 0) {
                    printf("\033[1;31mNu exista rezervari active.\n\033[0m");
                    break;
                }

                int opt;
                printf("Introdu numarul rezervarii pe care vrei sa o anulezi: ");
                scanf("%d", &opt); getchar();

                if (opt < 1 || opt > nrRezervate) {
                    printf("\033[1;31mOptiune invalida.\n\033[0m");
                    break;
                }

                FILE *fin2 = fopen("statistici.txt", "r");
                FILE *fout = fopen("temp.txt", "w");
                if (!fin2 || !fout) {
                    printf("\033[1;31mEroare la deschiderea fisierelor pentru actualizare.\n\033[0m");
                    if (fin2) fclose(fin2);
                    if (fout) fclose(fout);
                    break;
                }

                int linieCurenta = 0;
                while (fgets(linie, sizeof(linie), fin2)) {
                    if (linieCurenta == indexuriOriginale[opt - 1]) {
                        char *poz = strstr(linie, "Rezervat:Da");
                        if (poz) {
                            strncpy(poz, "Rezervat:Nu", strlen("Rezervat:Nu"));
                        }
                    }
                    fputs(linie, fout);
                    linieCurenta++;
                }

                fclose(fin2);
                fclose(fout);
                remove("statistici.txt");
                rename("temp.txt", "statistici.txt");

                printf("\033[1;32mRezervarea a fost anulata cu succes!\n\033[0m");
                break;
            }
            case 7:
                printf("Inapoi catre meniul principal.\n");
                return;
        }
        printf("Apasa Enter pentru a continua.\n");
        getchar(); getchar();
        eliberareTerminal();
    } while (1);
}

void afisareMeniu() {
    int optiune;
    do {
        printf("\n--- Meniu Principal ---\n");
        printf("1. Gestionare Flota\n");
        printf("2. Statistica\n");
        printf("3. Iesire\n");
        printf("Alege o optiune: ");
        scanf("%d", &optiune);
        getchar();

        switch (optiune) {
            case 1:
                afisareSubMeniu();
                break;
            case 2:
                printf("Ati ales optiunea de statistica.\n");
                FILE *f = fopen("statistici.txt", "r");
                char l[150];
                printf("\n--- Statistici ---\n");
                while (fgets(l, sizeof(l), f)) {
                    printf("%s", l);
                }
                fclose(f);
                printf("\n--- Sfarsit statistici ---\n");
                break;
            case 3:
                printf("Ati ales optiunea de iesire.\n");
                return;
        }

        printf("Apasa Enter pentru a continua.\n");
        getchar();
        eliberareTerminal();
    } while (1);
}

int main() {
    afisareMeniu();
    return 0;
}
