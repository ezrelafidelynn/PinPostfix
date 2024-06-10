#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <windows.h>
#include <unistd.h>
#include <wincon.h>



#define MAX_LINE_LENGTH 100
#define MAX_QUIZZES 100

//struct untuk quiz
typedef struct {
    char question[MAX_LINE_LENGTH];
    char answer[MAX_LINE_LENGTH];
} Quiz;

//struct untuk leaderboard
typedef struct {
    char name[MAX_LINE_LENGTH];
    int points;
} Player;

//struct untuk converter
typedef struct node 
{
    char data;
    struct node *left, *right;
} Node;

typedef Node* NodePtr;

//fungsi untuk converternya
NodePtr createNode(char c);
int isOperator(char c);
void infixToPostfix(NodePtr root);
void infixToPrefix(NodePtr root);
void prefixToInfix(NodePtr root);
void prefixToPostfix(NodePtr root);
void postfixToInfix(NodePtr root);
void postfixToPrefix(NodePtr root);
NodePtr buildExpressionTreeInfix(char *s);
NodePtr buildExpressionTreePost(char postfix[]);
NodePtr buildExpressionTreePre(char prefix[]);

//fungsi add player to leaderboard jadi bisa langsung dipanggil saja
void addPlayerToLeaderboard(Player *player) {
    FILE *leaderboardFile = fopen("leaderboard.txt", "a");//append leaderboard.txt bisa diubah sesuka hati
    if (leaderboardFile == NULL) {
        printf("Error opening leaderboard file.\n");//validasi
        exit(1);
    }
    fprintf(leaderboardFile, "%s %d\n", player->name, player->points);//print nama dan poin yg bisa didapatkan dari quiznya
    fclose(leaderboardFile);//close untuk disave
}

void readQuizFromFile(char *filename, Quiz *quizzes, int *quizCount) {//baca infixToPostfix atau infixToPrefix
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file %s\n", filename);
        exit(1);
    }

    *quizCount = 0;
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = 0;

        char *question = strtok(line, ";");
        char *answer = strtok(NULL, ";");

        strcpy(quizzes[*quizCount].question, question);//copy dari struct ke variabel biasa (pertanyaannya)
        strcpy(quizzes[*quizCount].answer, answer);//copy dari struct ke variabel biasa (jawabannya)

        (*quizCount)++;//tambah count 
    }

    fclose(file);//close file
}

void shuffleQuiz(Quiz *quizzes, int quizCount) {//supaya saat ditampilkan tidak akan mengeluarkan soal yang sama
    srand(time(NULL));
    for (int i = quizCount - 1; i > 0; i--) {
        int j = rand() % (i + 1);//randomize number
        Quiz temp = quizzes[i];
        quizzes[i] = quizzes[j];//tuker value 
        quizzes[j] = temp;
    }
}

int quizEasy(Quiz *quizzes, int quizCount, Player *player) {
    int choice;
    int numQuestions = 0;
    printf("Enter the number of questions you want to answer: ");//supaya tidak sekaligus keluar semua pertanyaannya
    scanf("%d", &numQuestions);
    getchar();
    
    int currentPoints = 0;
    do {
        shuffleQuiz(quizzes, quizCount);

        for (int i = 0; i < numQuestions; i++) {
            printf("\n%s\n",quizzes[i].question);//print questionnya
			puts("");
			printf("Note: Please the numbers with space for the quiz!\n");//untuk menghindari error dari user
			puts("");
            printf("Your answer: ");
            char userAnswer[MAX_LINE_LENGTH];
            fgets(userAnswer, sizeof(userAnswer), stdin);//ngambil input jawabannya
            userAnswer[strcspn(userAnswer, "\n")] = '\0';

			//validasi jawabannya
            if (strcmp(userAnswer, quizzes[i].answer) == 0) {//kalau sama yowis bener
                printf("Correct!\n");
                currentPoints += 10;
            } else {
                printf("Incorrect. The correct answer is: %s\n", quizzes[i].answer);
            }
        }
        printf("Quiz completed. Good job! Your score is %d\n", currentPoints);//hasil akhir untuk user bisa lihat

        printf("Do you want to do more questions? (1 for Yes, 0 for No): ");//untuk lanjut ronde 2 atau tidak
        scanf("%d", &choice);
        getchar();//buffer

        if (choice != 1 && choice != 0) {
            printf("Invalid choice. Please enter 1 for Yes or 0 for No.\n");//validasi
        }
    } while (choice == 1);

    player->points += currentPoints; //untuk menambahkan poin

    return currentPoints;//return nilai poin yg terbaru
}

int quizMedium(Quiz *quizzes, int quizCount, Player *player) {
    int choice;
    int numQuestions = 0;
    printf("Enter the number of questions you want to answer: ");//supaya tidak sekaligus keluar semua pertanyaannya
    scanf("%d", &numQuestions);
    getchar();
    
    int currentPoints = 0;
    do {
        shuffleQuiz(quizzes, quizCount);

        for (int i = 0; i < numQuestions; i++) {
            printf("\n%s\n",quizzes[i].question);//print questionnya
			puts("");
			printf("Note: Please the numbers with space for the quiz!\n");//untuk menghindari error dari user
			puts("");
            printf("Your answer: ");
            char userAnswer[MAX_LINE_LENGTH];
            fgets(userAnswer, sizeof(userAnswer), stdin);//ngambil input jawabannya
            userAnswer[strcspn(userAnswer, "\n")] = '\0';

			//validasi jawabannya
            if (strcmp(userAnswer, quizzes[i].answer) == 0) {//kalau sama yowis bener
                printf("Correct!\n");
                currentPoints += 30;
            } else {
                printf("Incorrect. The correct answer is: %s\n", quizzes[i].answer);
            }
        }
        printf("Quiz completed. Good job! Your score is %d\n", currentPoints);//hasil akhir untuk user bisa lihat

        printf("Do you want to do more questions? (1 for Yes, 0 for No): ");//untuk lanjut ronde 2 atau tidak
        scanf("%d", &choice);
        getchar();//buffer

        if (choice != 1 && choice != 0) {
            printf("Invalid choice. Please enter 1 for Yes or 0 for No.\n");//validasi
        }
    } while (choice == 1);

    player->points += currentPoints; //untuk menambahkan poin

    return currentPoints;//return nilai poin yg terbaru
}

int quizDifficult(Quiz *quizzes, int quizCount, Player *player) {
    int choice;
    int numQuestions = 0;
    printf("Enter the number of questions you want to answer: ");//supaya tidak sekaligus keluar semua pertanyaannya
    scanf("%d", &numQuestions);
    getchar();
    
    int currentPoints = 0;
    do {
        shuffleQuiz(quizzes, quizCount);

        for (int i = 0; i < numQuestions; i++) {
            printf("\n%s\n",quizzes[i].question);//print questionnya
			puts("");
			printf("Note: Please the numbers with space for the quiz!\n");//untuk menghindari error dari user
			puts("");
            printf("Your answer: ");
            char userAnswer[MAX_LINE_LENGTH];
            fgets(userAnswer, sizeof(userAnswer), stdin);//ngambil input jawabannya
            userAnswer[strcspn(userAnswer, "\n")] = '\0';

			//validasi jawabannya
            if (strcmp(userAnswer, quizzes[i].answer) == 0) {//kalau sama yowis bener
                printf("Correct!\n");
                currentPoints += 50;
            } else {
                printf("Incorrect. The correct answer is: %s\n", quizzes[i].answer);
            }
        }
        printf("Quiz completed. Good job! Your score is %d\n", currentPoints);//hasil akhir untuk user bisa lihat

        printf("Do you want to do more questions? (1 for Yes, 0 for No): ");//untuk lanjut ronde 2 atau tidak
        scanf("%d", &choice);
        getchar();//buffer

        if (choice != 1 && choice != 0) {
            printf("Invalid choice. Please enter 1 for Yes or 0 for No.\n");//validasi
        }
    } while (choice == 1);

    player->points += currentPoints; //untuk menambahkan poin

    return currentPoints;//return nilai poin yg terbaru
}



void deletePlayerFromLeaderboard(char *name) {//dari nama yang user input diawal bisa langsung didelete
    FILE *tempFile = fopen("temp.txt", "w");//write karena akan delete
    FILE *leaderboardFile = fopen("leaderboard.txt", "r");//read leaderboard .txt file
    if (tempFile == NULL || leaderboardFile == NULL) {//validasi opening 2 file barusan
        printf("Error opening files.\n");
        exit(1);
    }
    
    //proses delete menggunakan temp dan update ke leaderboard.txt
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), leaderboardFile) != NULL) {//ngambil dari filenya
        char tempName[MAX_LINE_LENGTH];
        sscanf(line, "%s", tempName);//ambil input
        if (strcmp(tempName, name) != 0) {
            fprintf(tempFile, "%s", line);//delete namanya
        }
    }
    
    fclose(tempFile);
    fclose(leaderboardFile);//save nama yang sudah di delete
    
    remove("leaderboard.txt");
    rename("temp.txt", "leaderboard.txt");//rename karena temp.txt yang merupakan file yg sudah didelete
}

//mirip dengan delete tapi ada tambahan jadi nama baru
void updatePlayerNameInLeaderboard(char *oldName, char *newName) {
    FILE *tempFile = fopen("temp.txt", "w");
    FILE *leaderboardFile = fopen("leaderboard.txt", "r");
    if (tempFile == NULL || leaderboardFile == NULL) {//validasi
        printf("Error opening files.\n");
        exit(1);
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), leaderboardFile) != NULL) {
        char tempName[MAX_LINE_LENGTH];//temporary variabel
        int tempPoints;
        sscanf(line, "%s %d", tempName, &tempPoints);
        if (strcmp(tempName, oldName) == 0) {//kalau sama update kalo fprintf si line nya
            fprintf(tempFile, "%s %d\n", newName, tempPoints);
        } else {
            fprintf(tempFile, "%s", line);
        }
    }

    fclose(tempFile);
    fclose(leaderboardFile);//save file yg udh di update

    remove("leaderboard.txt");
    rename("temp.txt", "leaderboard.txt");//rename temp karena merupakan file yg sudah di update
}

//view leaderboardnya
void displayLeaderboard() {
    printf("\n\nLeaderboard:\n");
    FILE *leaderboardFile = fopen("leaderboard.txt", "r");//read saja tidak ush write/append
    if (leaderboardFile == NULL) {
        printf("Error opening leaderboard file.\n");//validasi
        exit(1);
    }
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), leaderboardFile) != NULL) {//print semua yg ada di leaderboard
        printf("%s", line);
    }
    fclose(leaderboardFile);//close file kalau udh di akhir function
}

void startQuizPostfixEasy(Player *player) {
    Quiz quizzes[MAX_QUIZZES];//deklarasi struct
    int quizCount = 0;
    readQuizFromFile("infixToPostfixEasy.txt", quizzes, &quizCount);//read file yg postfix

    int currentPoints = quizEasy(quizzes, quizCount, player);//poin yang akan dimasukan dari quiz ke leaderboard
    
    printf("Congratulations, %s! Your final score is: %d\n", player->name, currentPoints);//display final score
    
    FILE *leaderboardFile = fopen("leaderboard.txt", "r+");
    if (leaderboardFile == NULL) {
        printf("Error opening leaderboard file.\n");//validasi
        exit(1);
    }
    char tempFile[MAX_LINE_LENGTH] = "temp.txt";
    FILE *tempLeaderboardFile = fopen(tempFile, "w");
    if (tempLeaderboardFile == NULL) {
        printf("Error creating temporary file.\n");//validasi
        fclose(leaderboardFile);
        exit(1);
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), leaderboardFile) != NULL) {
        char tempName[MAX_LINE_LENGTH];
        int tempPoints;
        sscanf(line, "%s %d", tempName, &tempPoints);
        if (strcmp(tempName, player->name) == 0) {
            tempPoints += currentPoints;
            fprintf(tempLeaderboardFile, "%s %d\n", tempName, tempPoints);//print ke leaderboard nama dengan poin barunya
        } else {
            fprintf(tempLeaderboardFile, "%s", line);//print dari leaderboard biasa
        }
    }

    fclose(leaderboardFile);
    fclose(tempLeaderboardFile);//tutup utk save poin ke leaderboard

    remove("leaderboard.txt");
    rename(tempFile, "leaderboard.txt");//rename untuk kembali jadi leaderboard lagi
}

void startQuizPostfixMedium(Player *player) {
    Quiz quizzes[MAX_QUIZZES];
    int quizCount = 0;
    readQuizFromFile("infixToPostfixMedium.txt", quizzes, &quizCount);//read file yg postfix

    int currentPoints = quizMedium(quizzes, quizCount, player);//poin yang akan dimasukan dari quiz ke leaderboard
    
    printf("Congratulations, %s! Your final score is: %d\n", player->name, currentPoints);//display final score
    
    FILE *leaderboardFile = fopen("leaderboard.txt", "r+");
    if (leaderboardFile == NULL) {
        printf("Error opening leaderboard file.\n");//validasi
        exit(1);
    }
    char tempFile[MAX_LINE_LENGTH] = "temp.txt";
    FILE *tempLeaderboardFile = fopen(tempFile, "w");
    if (tempLeaderboardFile == NULL) {
        printf("Error creating temporary file.\n");//validasi
        fclose(leaderboardFile);
        exit(1);
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), leaderboardFile) != NULL) {
        char tempName[MAX_LINE_LENGTH];
        int tempPoints;
        sscanf(line, "%s %d", tempName, &tempPoints);
        if (strcmp(tempName, player->name) == 0) {
            tempPoints += currentPoints;
            fprintf(tempLeaderboardFile, "%s %d\n", tempName, tempPoints);//print ke leaderboard nama dengan poin barunya
        } else {
            fprintf(tempLeaderboardFile, "%s", line);//print dari leaderboard biasa
        }
    }

    fclose(leaderboardFile);
    fclose(tempLeaderboardFile);//tutup utk save poin ke leaderboard

    remove("leaderboard.txt");
    rename(tempFile, "leaderboard.txt");//rename untuk kembali jadi leaderboard lagi
}

void startQuizPostfixDifficult(Player *player) {
    Quiz quizzes[MAX_QUIZZES];
    int quizCount = 0;
    readQuizFromFile("infixToPostfixDifficult.txt", quizzes, &quizCount);//read file yg postfix

    int currentPoints = quizDifficult(quizzes, quizCount, player);//poin yang akan dimasukan dari quiz ke leaderboard
    
    printf("Congratulations, %s! Your final score is: %d\n", player->name, currentPoints);//display final score
    
    FILE *leaderboardFile = fopen("leaderboard.txt", "r+");
    if (leaderboardFile == NULL) {
        printf("Error opening leaderboard file.\n");//validasi
        exit(1);
    }
    char tempFile[MAX_LINE_LENGTH] = "temp.txt";
    FILE *tempLeaderboardFile = fopen(tempFile, "w");
    if (tempLeaderboardFile == NULL) {
        printf("Error creating temporary file.\n");//validasi
        fclose(leaderboardFile);
        exit(1);
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), leaderboardFile) != NULL) {
        char tempName[MAX_LINE_LENGTH];
        int tempPoints;
        sscanf(line, "%s %d", tempName, &tempPoints);
        if (strcmp(tempName, player->name) == 0) {
            tempPoints += currentPoints;
            fprintf(tempLeaderboardFile, "%s %d\n", tempName, tempPoints);//print ke leaderboard nama dengan poin barunya
        } else {
            fprintf(tempLeaderboardFile, "%s", line);//print dari leaderboard biasa
        }
    }

    fclose(leaderboardFile);
    fclose(tempLeaderboardFile);//tutup utk save poin ke leaderboard

    remove("leaderboard.txt");
    rename(tempFile, "leaderboard.txt");//rename untuk kembali jadi leaderboard lagi
}

void startQuizPrefixEasy(Player *player) {
    Quiz quizzes[MAX_QUIZZES];
    int quizCount = 0;
    readQuizFromFile("infixToPrefixEasy.txt", quizzes, &quizCount);//read file prefix

    int currentPoints = quizEasy(quizzes, quizCount, player);//poin yang akan dimasukan dari quiz ke leaderboard
    
    printf("Congratulations, %s! Your final score is: %d\n", player->name, currentPoints);//display final score
    
    FILE *leaderboardFile = fopen("leaderboard.txt", "r+");
    if (leaderboardFile == NULL) {
        printf("Error opening leaderboard file.\n");//validasi
        exit(1);
    }
    char tempFile[MAX_LINE_LENGTH] = "temp.txt";
    FILE *tempLeaderboardFile = fopen(tempFile, "w");
    if (tempLeaderboardFile == NULL) {
        printf("Error creating temporary file.\n");//validasi
        fclose(leaderboardFile);
        exit(1);
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), leaderboardFile) != NULL) {
        char tempName[MAX_LINE_LENGTH];
        int tempPoints;
        sscanf(line, "%s %d", tempName, &tempPoints);
        if (strcmp(tempName, player->name) == 0) {
            tempPoints += currentPoints;
            fprintf(tempLeaderboardFile, "%s %d\n", tempName, tempPoints);//print ke leaderboard nama dengan poin barunya
        } else {
            fprintf(tempLeaderboardFile, "%s", line);//print dari leaderboard biasa
        }
    }

    fclose(leaderboardFile);
    fclose(tempLeaderboardFile);//tutup utk save poin ke leaderboard

    remove("leaderboard.txt");
    rename(tempFile, "leaderboard.txt");//rename untuk kembali jadi leaderboard lagi
}

void startQuizPrefixMedium(Player *player) {
    Quiz quizzes[MAX_QUIZZES];
    int quizCount = 0;
    readQuizFromFile("infixToPrefixMedium.txt", quizzes, &quizCount);//read file prefix

    int currentPoints = quizMedium(quizzes, quizCount, player);//poin yang akan dimasukan dari quiz ke leaderboard
    
    printf("Congratulations, %s! Your final score is: %d\n", player->name, currentPoints);//display final score
    
    FILE *leaderboardFile = fopen("leaderboard.txt", "r+");
    if (leaderboardFile == NULL) {
        printf("Error opening leaderboard file.\n");//validasi
        exit(1);
    }
    char tempFile[MAX_LINE_LENGTH] = "temp.txt";
    FILE *tempLeaderboardFile = fopen(tempFile, "w");
    if (tempLeaderboardFile == NULL) {
        printf("Error creating temporary file.\n");//validasi
        fclose(leaderboardFile);
        exit(1);
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), leaderboardFile) != NULL) {//kalau masih ada data bakal loop
        char tempName[MAX_LINE_LENGTH];
        int tempPoints;
        sscanf(line, "%s %d", tempName, &tempPoints);
        if (strcmp(tempName, player->name) == 0) {
            tempPoints += currentPoints;
            fprintf(tempLeaderboardFile, "%s %d\n", tempName, tempPoints);//print ke leaderboard nama dengan poin barunya
        } else {
            fprintf(tempLeaderboardFile, "%s", line);//print dari leaderboard biasa
        }
    }

    fclose(leaderboardFile);
    fclose(tempLeaderboardFile);//tutup utk save poin ke leaderboard

    remove("leaderboard.txt");
    rename(tempFile, "leaderboard.txt");//rename untuk kembali jadi leaderboard lagi
}

void startQuizPrefixDifficult(Player *player) {
    Quiz quizzes[MAX_QUIZZES];
    int quizCount = 0;
    readQuizFromFile("infixToPrefixDifficult.txt", quizzes, &quizCount);//read file prefix

    int currentPoints = quizDifficult(quizzes, quizCount, player);//poin yang akan dimasukan dari quiz ke leaderboard
    
    printf("Congratulations, %s! Your final score is: %d\n", player->name, currentPoints);//display final score
    
    FILE *leaderboardFile = fopen("leaderboard.txt", "r+");
    if (leaderboardFile == NULL) {
        printf("Error opening leaderboard file.\n");//validasi
        exit(1);
    }
    char tempFile[MAX_LINE_LENGTH] = "temp.txt";
    FILE *tempLeaderboardFile = fopen(tempFile, "w");
    if (tempLeaderboardFile == NULL) {
        printf("Error creating temporary file.\n");//validasi
        fclose(leaderboardFile);
        exit(1);
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), leaderboardFile) != NULL) {
        char tempName[MAX_LINE_LENGTH];
        int tempPoints;
        sscanf(line, "%s %d", tempName, &tempPoints);
        if (strcmp(tempName, player->name) == 0) {
            tempPoints += currentPoints;
            fprintf(tempLeaderboardFile, "%s %d\n", tempName, tempPoints);//print ke leaderboard nama dengan poin barunya
        } else {
            fprintf(tempLeaderboardFile, "%s", line);//print dari leaderboard biasa
        }
    }

    fclose(leaderboardFile);
    fclose(tempLeaderboardFile);//tutup utk save poin ke leaderboard

    remove("leaderboard.txt");
    rename(tempFile, "leaderboard.txt");//rename untuk kembali jadi leaderboard lagi
}


void startLeaderboardMenu(Player *player) {
	int menuChoice;	
    do {
    	//menu leaderboard
        printf("\033[1;93m");
        printf(" _                 _     _                     _\n");
        printf("| |   ___  __ _ __| |___| |__ ___  __ _ ___ __| |\n");
        printf("| |__/ ._\\/ .  | .  | ._|  . | . |/ .  |  _|  . |\n");
        printf("|____|___|\\__,_|____|___|____|___|\\__,_|_| |____|\n");
        printf("\033[0m");
        puts("");
        printf("\033[1;96m");
        printf("=======================================\n");
        puts("");
        printf("\033[0m");
        printf("\033[1;34m");
        printf("1. View Leaderboard\n");
        printf("2. Update Name\n");
        printf("3. Delete Name\n");
        printf("0. Return to Main Menu\n");
        printf("\033[0m");
        printf("Enter your choice: ");
        scanf("%d", &menuChoice);
        getchar();

        switch (menuChoice) {
            case 1:
                displayLeaderboard();
                break;
            case 2:
                printf("Enter your new name: ");
                char newName[MAX_LINE_LENGTH];
                fgets(newName, sizeof(newName), stdin);
                newName[strcspn(newName, "\n")] = '\0';
                updatePlayerNameInLeaderboard(player->name, newName);
                strcpy(player->name, newName);
                printf("Name updated successfully!\n");
                break;
            case 3:
                printf("Are you sure you want to delete your account? (1 for Yes, 0 for No): ");
                int confirmation;
                scanf("%d", &confirmation);
                getchar();
                if (confirmation == 1) {
                    deletePlayerFromLeaderboard(player->name);
                    printf("Account deleted successfully!\n");
                    return;
                } else if (confirmation == 0) {
                    printf("Account deletion cancelled.\n");
                } else {
                    printf("Invalid choice. Please enter 1 for Yes or 0 for No.\n");
                }
                break;
            case 0:
                return;
            default:
                printf("Invalid choice. Please enter a number between 1 and 4.\n");
        }
    } while (menuChoice != 4);
}


void QuizMenu() {
    Player player;//deklarasi struct
    printf("Enter your name: ");
    fgets(player.name, sizeof(player.name), stdin);
    player.name[strcspn(player.name, "\n")] = '\0';//masukan nama playernya
    player.points = 0;//buat 0 dulu

    addPlayerToLeaderboard(&player);//masukan ke leaderboard

    int level;
    int quizType;
    int choice;
    do {
        printf("\033[1;93m");
        printf(" _____         _____     _   ___             _____         ___    _______ \n");
        printf("|  __ |. _,__ |  __ |___| |_|  _| .         /     \\      .|_  |  |__   __|.  \n");
        printf("|  ___|_|  _ \\|  ___| __|  _| |_  _ __  __ |  ()  |\\   /|_ / /      | |   _  ________   ___\n");
        printf("| |   | | | | | | . |__ \\ | |  _|| |\\ \\/ / |      | |_| | | /_      | |  | ||  _   _ \\ / ._|\n");
        printf("|_|   |_|_| |_|_|___|___/_| |_|  |_|/_/\\_\\ |____\\_\\_____|_|___|     |_|  |_||_| |_| |_|\\___|\n");
        printf("\033[0m");

        printf("\033[1;36m");
        printf("======================\n");
        printf("\033[0m");
        printf("\033[1;34m");
        printf("1. Start Quiz\n");
        printf("2. Leaderboard\n");
        printf("0. Return to Main Menu\n");
        printf("\033[0m");
        printf("\t>> ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: {
            	//menu difficulty
            	printf("\033[1;36m");
                printf("\n\nSelect Difficulty Level\n");
                printf("\033[0m");
        		printf("\033[1;34m");
                printf("1. Easy\n");
                printf("2. Medium\n");
                printf("3. Difficult\n");
                printf("0. Return to Main Menu\n");
                printf("\033[0m");
                printf("\t>> ");
                scanf("%d", &level);
                getchar();
				
				//menu mau quiz yg mana
				printf("\033[1;36m");
                printf("\n\nInToPrefix Time\n");
                printf("\033[0m");
        		printf("\033[1;34m");
                printf("1. Infix to Postfix Trivia\n");
                printf("2. Infix to Prefix Trivia\n");
                printf("0. Return to Main Menu\n");
                printf("\033[0m");
                puts("");
                printf("\t>> ");
                scanf("%d", &quizType);
                getchar();

                switch (quizType) {
                    case 1:
                        switch (level) {//menu di dalem menu makanya kayak double
                            case 1:
                                startQuizPostfixEasy(&player);
                                break;
                            case 2:
                                startQuizPostfixMedium(&player);
                                break;
                            case 3:
                                startQuizPostfixDifficult(&player);
                                break;
                            case 0:
				                break;
                            default:
                                printf("Please insert a valid number.\n");
                        }
                        break;
                    case 2:
                        switch (level) {//menu di dalem menu makanya kayak double
                            case 1:
                                startQuizPrefixEasy(&player);
                                break;
                            case 2:
                                startQuizPrefixMedium(&player);
                                break;
                            case 3:
                                startQuizPrefixDifficult(&player);
                                break;
                            case 0:
				                break;
                            default:
                                printf("Please insert a valid number.\n");
                        }
                        break;
                    case 0:
                        break;
                    default:
                        printf("Please insert a valid number.\n");
                }
                break;
            }
            case 2:
                startLeaderboardMenu(&player);//leaderboard menu
                break;
            case 0:
                printf("Thank you for playing! See you next time!\n");
                break;
            default:
                printf("Please insert a valid number.\n");
        }
    } while (choice != 0);
}

//CONVERTER
NodePtr createNode(char c)//smirip seperti createNode
{
    NodePtr n = (NodePtr)malloc(sizeof(Node));
    n->data = c;
    n->left = n->right = NULL;
    return n;
}

int isOperator(char c) {//fungsi utk tahu ini operator atau bukan
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

void infixToPostfix(NodePtr root)//cara print seperti postfix
{
    if (root != NULL) {
        infixToPostfix(root->left);
        infixToPostfix(root->right);
        printf("%c", root->data);
    }
}

//tinggal perkara cara baca saja
void infixToPrefix(NodePtr root)//cara print prefix
{
    if (root != NULL) {
        printf("%c", root->data);
        infixToPrefix(root->left);
        infixToPrefix(root->right);
    }
}

//tinggal perkara cara baca saja
void prefixToInfix(NodePtr root) {//cara printnya seperti infix
    if (root) {
        if (isOperator(root->data)) {
            prefixToInfix(root->left);
            printf("%c", root->data);
            prefixToInfix(root->right);
        } else {
            printf("%c", root->data);
        }
    }
}

//tinggal perkara cara baca saja
void prefixToPostfix(NodePtr root) {//cara printnya seperti postfix
    if (root) {
        prefixToPostfix(root->left);
        prefixToPostfix(root->right);
        printf("%c", root->data);
    }
}

//tinggal perkara cara baca saja
void postfixToInfix(NodePtr root) {//print dengan cara infix
    if (root) {
        int isOperatorNode = isOperator(root->data);
        postfixToInfix(root->left);
        printf("%c", root->data);
        postfixToInfix(root->right);
    }
}

//tinggal perkara cara baca saja
void postfixToPrefix(NodePtr root) {//postfix harus diubah menajdi infix dulu kalo nggak dibalikin doang
    if (root) {
        infixToPrefix(root);
    }
}



//untuk kalkulasinya
NodePtr buildExpressionTreeInfix(char *s)
{
    NodePtr stN[100];
    int topN = -1;//deklarasi awalnya -1 karena tidak ada

    char stC[100];
    int topC = -1;//deklarasi awalnya -1 karena tidak ada

    NodePtr t, t1, t2;

    int p[123] = { 0 };
    p['+'] = p['-'] = 1, p['/'] = p['*'] = 2, p['^'] = 3,
    p[')'] = 0;//operatornya

    for (int i = 0; i < strlen(s); i++) //looping trs sampe beres
    {
        if (s[i] == '(') 
        {
            stC[++topC] = s[i];
        }
        else if (isalpha(s[i])) 
        {
            t = createNode(s[i]);//buat baru
            stN[++topN] = t;
        }
        else if (p[s[i]] > 0) 
        {
            while (
                topC >= 0 && stC[topC] != '('
                && ((s[i] != '^' && p[stC[topC]] >= p[s[i]])
                    || (s[i] == '^'
                        && p[stC[topC]] > p[s[i]]))) //syarat whilenya memang banyak tapi intinya buat validasi juga
            {
                t = createNode(stC[topC--]);//buat t

                t1 = stN[topN--];

                t2 = stN[topN--];

                t->left = t2;//treenya
                t->right = t1;

                stN[++topN] = t;
            }

            stC[++topC] = s[i];//stack
        }
        else if (s[i] == ')') 
        {
            while (topC >= 0 && stC[topC] != '(') 
            {
                t = createNode(stC[topC--]);
                t1 = stN[topN--];
                t2 = stN[topN--];
                t->left = t2;
                t->right = t1;
                stN[++topN] = t;
            }
            topC--;
        }
    }
    t = stN[topN];
    return t;
}

NodePtr buildExpressionTreePost(char postfix[]) {
    NodePtr stack[100];
    int top = -1;

    for (int i = 0; postfix[i] != '\0'; i++) {
        NodePtr newNode = createNode(postfix[i]);
        if (!isOperator(postfix[i])) {
            stack[++top] = newNode;
        } else {
            newNode->right = stack[top--];//else tree = stack[top--]
            newNode->left = stack[top--];
            stack[++top] = newNode;
        }
    }
    return stack[top];
}

NodePtr buildExpressionTreePre(char prefix[]) {
    NodePtr stack[100];
    int top = -1;

    int length = strlen(prefix);

    for (int i = length - 1; i >= 0; i--) {
        char c = prefix[i];

        if (isOperator(c)) {
        	NodePtr newNode = createNode(c);
            newNode->left = stack[top--];//treenya jadi stack[top--]
            newNode->right = stack[top--];
            stack[++top] = newNode;
        } else {
            NodePtr newNode = createNode(c);//buat baru
            stack[++top] = newNode;//masukan ke stack
        }
    }

    return stack[top];
}

void converterMenu(){
	int choice;
    char buffer[10];
    char postfix[100], prefix[100], infix[100];
    NodePtr root = NULL;
    int len;

    while (1) {
    	//random design
        printf("\033[1;93m");  // Blue
        printf(" ___                       _             \n");
        printf("|  _|___ ____ _  __ __ ___| |_  ___ ___ \n");
        printf("| |_| . |    | \\/ / ._|  _|  _|/ ._|  _|\n");
        printf("|___|___|_||_|\\__/\\___|_| |__| \\___|_|  \n");
        puts("");
        printf("\033[0m");
	    printf("\n\n\033[1;36m");
        printf("====================\n");
        printf("\033[0m");
	    printf("\033[1;34m");
        puts("");
        printf("1. Infix to Postfix\n");
        printf("2. Infix to Prefix\n");
        printf("3. Postfix to Infix\n");
        printf("4. Postfix to Prefix\n");
        printf("5. Prefix to Infix\n");
        printf("6. Prefix to Postfix\n");
        printf("0. Return to Main Menu\n");
        puts("");
        printf("Note: When you use this converter, please insert alphabets with no spaces! Otherwise, you won't get the answer you need!\n");
        puts("");
        printf("\033[0m");  // Yellow
        printf("\t>> ");
        scanf("%d", &choice);
        fgets(buffer, sizeof(buffer), stdin);
		puts("");
        switch (choice) {
            case 1:
            {
	            char s[100];
	            printf("Enter an infix expression: ");
	            scanf(" %[^\n]s", s);
	            int len = strlen(s);
	            char str[len + 3];//untuk spasi
	            strcpy(str + 1, s);
	            str[0] = '(';
	            str[len + 1] = ')';
	            str[len + 2] = '\0';
	            NodePtr root = buildExpressionTreeInfix(str);//buat dulu treenya
	            printf("Postfix traversal: ");
	            infixToPostfix(root);//hasilnya dari fungsi ini
	            printf("\n");
	            printf("\n");
	            break;
       		}
            case 2:
            {
        		char s[100];
	            printf("Enter an infix expression: ");
	            scanf(" %[^\n]s", s);
	            int len = strlen(s);
	            char str[len + 3]; 
	            strcpy(str + 1, s);
	            str[0] = '(';
	            str[len + 1] = ')';
	            str[len + 2] = '\0';
	            NodePtr root = buildExpressionTreeInfix(str);//buat dulu treenya
	            printf("Prefix traversal: ");
	            infixToPrefix(root);//hasilnya dari fungsi ini
	            printf("\n");
	            printf("\n");
	            break;
    		}
            case 3:
                printf("Enter postfix expression: ");
                fgets(postfix, sizeof(postfix), stdin);
                postfix[strcspn(postfix, "\n")] = '\0';
                root = buildExpressionTreePost(postfix);//buat dulu treenya
                printf("Infix expression: ");
                postfixToInfix(root);//hasilnya dari fungsi ini
                printf("\n");
                printf("\n");
                break;
            case 4:
                printf("Enter postfix expression: ");
                fgets(postfix, sizeof(postfix), stdin);
                postfix[strcspn(postfix, "\n")] = '\0';
                root = buildExpressionTreePost(postfix);//buat dulu treenya
                printf("Prefix expression: ");
                postfixToPrefix(root);//hasilnya dari fungsi ini
                printf("\n");
                printf("\n");
                break;
            case 5:
                printf("Enter prefix expression: ");
                fgets(prefix, sizeof(prefix), stdin);
                prefix[strcspn(prefix, "\n")] = '\0';
                root = buildExpressionTreePre(prefix);//buat dulu treenya
                printf("Infix expression: ");
                prefixToInfix(root);//hasilnya dari fungsi ini
                printf("\n");
                printf("\n");
                break;
            case 6:
                printf("Enter prefix expression: ");
                fgets(prefix, sizeof(prefix), stdin);
                prefix[strcspn(prefix, "\n")] = '\0';
                root = buildExpressionTreePre(prefix);//buat dulu treenya
                printf("Postfix expression: ");
                prefixToPostfix(root);//hasilnya dari fungsi ini
                printf("\n");
                printf("\n");
                break;
            case 0:
                return;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
}

void menu(){
	//maun menu
	printf("\033[1;93m");
    printf(" _____         _____     _   ___\n");
    printf("|  __ |. _,__ |  __ |___| |_|  _| .\n");
    printf("|  ___|_|  _ \\|  ___| __|  _| |_  _ __  __ \n");
    printf("| |   | | | | | | . |__ \\ | |  _|| |\\ \\/\n");
    printf("|_|   |_|_| |_|_|___|___/_| |_|  |_|/_/\\_\\\n");
    printf("\033[0m");

	printf("\n\n\033[1;36m");
    printf("The only program that has all kinds of converter and a quiz to test your knowledge!\n");
    printf("\033[0m");
    printf("\033[1;34m");
    printf("1. Converter\n");
    printf("2. Quiz\n");
    printf("0. Exit\n");
    printf("\033[0m");
	puts("");
}

void enableVirtualTerminalProcessing() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hOut, dwMode)) return;
}

void printColored(const char* text, const char* color) {
    printf("%s%s\033[0m", color, text);//ini reset warna buat nambahin warna ya
}

void clearConsole() {
    system("cls");
}

void switchColors() {
    const char* colors[] = {"\033[1;31m", "\033[1;34m", "\033[1;32m", "\033[1;33m"};//warnanya dijadiin array jadi bisa gampang di loopingnya
    int numColors = sizeof(colors) / sizeof(colors[0]);//hitung brp warna
    
    for (int i = 0; i < numColors; i++) {
        clearConsole();//intinya refresh screen nya jadi ga ada input sama sekali
        printColored("Thank you for visiting this program! You're awesome!\n", colors[i]);
        printColored("The Team that makes this program beautifully consists of:\n", colors[i]);
        printColored("Ezrela Fidelynn Surya Wibisana - 2702260086\n", colors[i]);
        printColored("Stanley Theon Siva - 2702267281\n", colors[i]);
        printColored("Gabrielle Janelyn Natasha Suhalim - 2702262394\n", colors[i]);
        fflush(stdout);//buat buffer
        sleep(1);//tunggu 1 detik
    }
}


int main() {
    int choice;
    do {
        menu();
        printf("\033[1;36m");
        printf("\t>> ");
        printf("\033[0m");
        scanf("%d", &choice);
        getchar();
        switch (choice) {
            case 1:
                converterMenu(); // Menu for converters
                break;
            case 2:
                QuizMenu(); // Menu for quiz
                break;
            case 0:
                // Closing message
                enableVirtualTerminalProcessing();
    			switchColors();
                break;
            default:
                printf("\033[1;31m");
                printf("Please enter a valid number.\n\n");// validasi
                printf("\033[0m");
        }
    } while (choice != 0);
    return 0;
}