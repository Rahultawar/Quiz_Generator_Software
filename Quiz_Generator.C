/*
    QUIZ GENERATOR SOFTWARE
    DATE CREATED: 12th January 2025
    CREATED BY: Rahul Tawar
    LINKEDIN: www.linkedin.com/in/iamrahultawar19
    TWITTER: iamrahultawar
*/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>

#define RESET "\033[0m"    // Resets all attributes to default
#define RED "\033[31m"     // Red color for errors
#define GREEN "\033[32m"   
#define YELLOW "\033[33m"  
#define MAGENTA "\033[35m" 
#define CYAN "\033[36m"

#define LENGTH 50
#define TOTAL_QUESTIONS 10

char title[] = {"Quiz Generator Software"};

// Struct representing a Question
typedef struct Question
{
    char question[LENGTH];
    char option[4][LENGTH];
    char correctAnswer;
    int optionCount;
} Question;

// Struct representing a User
typedef struct User
{
    char username[LENGTH];
    char password[LENGTH];
} User;

// Struct representing a Score entry
typedef struct Score
{
    char dateTime[LENGTH];
    int score;
} Score;

// Displays the menu options
void Menu()
{
    clrscr();
    textcolor(10);
    gotoxy(27, 5);
    printf(CYAN "Welcome to the Quiz Generator\n" RESET);
    textcolor(2);
    gotoxy(22, 6);
    printf(YELLOW "______________________________________" RESET);
    gotoxy(25, 8);
    printf(CYAN "	    Menu" RESET);
    gotoxy(22, 9);
    printf(YELLOW "______________________________________" RESET);
    gotoxy(22, 11);
    printf(MAGENTA "1. Start Quiz\n" RESET);
    gotoxy(22, 12);
    printf(MAGENTA "2. View Score\n" RESET);
    gotoxy(22, 13);
    printf(MAGENTA "3. Exit\n" RESET);
}

// Function to set up login credentials
void storeLoginCredentials()
{
    User user;
    FILE *fuser;
    printf("Username: ");
    scanf("%s", user.username);
    printf("Password: ");
    scanf("%s", user.password);

    fuser = fopen("login.bin", "wb");
    if (fuser == NULL)
    {
        printf(RED "Error opening login file!\n" RESET);
        return;
    }
    fwrite(&user, sizeof(User), 1, fuser);
    fclose(fuser);
}

// Verifies login credentials from a binary file
int verifyLogin()
{
    User user, storedUser;
    FILE *fuser = fopen("login.bin", "rb");
    int i;
    if (fuser == NULL)
    {
        printf(RED "Login credentials not set up! Setting up default credentials.\n" RESET);
        storeLoginCredentials();
        fuser = fopen("login.bin", "rb");
    }

    fread(&storedUser, sizeof(User), 1, fuser);
    fclose(fuser);

    do
    {
        clrscr();
        textcolor(10);
        gotoxy(30, 5);
        for (i = 0; title[i] != '\0'; i++)
        {
            _setcursortype(1);
            printf(CYAN "%c" RESET, title[i]);
            delay(50);
            sound(1500);    // Generate sound
            delay(50);
            nosound();
        }
        textcolor(2);
        gotoxy(22, 6);
        printf(YELLOW "______________________________________" RESET);
        gotoxy(25, 8);
        printf(CYAN "            Login" RESET);
        gotoxy(22, 9);
        printf(YELLOW "______________________________________" RESET);
        gotoxy(22, 11);
        printf("Username: ");   // Get username
        scanf("%s", user.username);
        gotoxy(22, 12);
        printf("Password: ");   // Get password
        scanf("%s", user.password);

        if (strcmp(user.username, storedUser.username) != 0 || strcmp(user.password, storedUser.password) != 0)
        {
            gotoxy(22, 14);
            printf(RED "Incorrect username or password. Try again.\n" RESET);
            getch();
        }
        else
        {
            gotoxy(22, 14);
            printf(GREEN "Login successful!\n" RESET);
            getch();
            return 1;
        }
    } while (1);
}

// Reads questions from a file
void readQuestionsFromFile(const char *filename, Question *questions)
{
    FILE *fques = fopen(filename, "r");
    char line[200];
    int i = 0, j;

    if (fques == NULL)
    {
        printf(RED "Error opening questions file!\n" RESET);
        return;
    }

    while (fgets(line, sizeof(line), fques) != NULL && i < TOTAL_QUESTIONS)
    {
        char *token = strtok(line, "|");
        if (token != NULL)
        {
            strcpy(questions[i].question, token);   // Store question
        }

        questions[i].optionCount = 0;
        for (j = 0; j < 4; j++)
        {
            token = strtok(NULL, "|");
            if (token != NULL)
            {
                strcpy(questions[i].option[questions[i].optionCount], token);   // Store options
                questions[i].optionCount++;
            }
            else
            {
                break;
            }
        }

        token = strtok(NULL, "|");
        if (token != NULL)
        {
            questions[i].correctAnswer = token[0];  // Store correct answer
        }
        i++;
    }

    fclose(fques);
}

// Displays a single question along with its options
void displayQuestion(Question q, int questionNumber)
{
    int i, j = 12;
    textcolor(2);
    clrscr();
    gotoxy(22, 6);
    printf(YELLOW "______________________________________" RESET);
    gotoxy(25, 8);
    printf(CYAN "	    Quiz" RESET);
    gotoxy(22, 9);
    printf(YELLOW "______________________________________" RESET);
    gotoxy(22, 11);
    printf(YELLOW "Question %d: %s\n" RESET, questionNumber, q.question);   // Displays question
    for (i = 0; i < q.optionCount; i++)
    {
        gotoxy(22, j);
        printf(MAGENTA "%c) %s\n" RESET, 'A' + i, q.option[i]); // Displays options
        j++;
    }
}

// Gets input from the user for a multiple-choice answer
char getUserInput()
{
    char userAnswer;
    while (1)
    {
        gotoxy(22, 17);
        printf(CYAN "Your answer (A/B/C/D): " RESET);
        scanf(" %c", &userAnswer);

        if (userAnswer >= 'A' && userAnswer <= 'D')
        {
            gotoxy(22, 19);
            printf("                                           ");
            return userAnswer;
        }
        else
        {
            gotoxy(22, 19);
            printf(RED "Invalid input. Please enter A, B, C, or D.\n" RESET);
        }
    }
}

// Stores the user's score in a binary file
void storeScore(int score)
{
    FILE *fscore;
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    Score entry;

    strftime(entry.dateTime, sizeof(entry.dateTime), "%Y-%m-%d %H:%M:%S", tm_info);
    entry.score = score;

    fscore = fopen("score.bin", "ab");
    if (fscore == NULL)
    {
        printf(RED "Error opening score file!\n" RESET);
        return;
    }
    fwrite(&entry, sizeof(Score), 1, fscore);
    fclose(fscore);
}

// Displays all stored scores from the binary file
void viewScores()
{
    FILE *fscore = fopen("score.bin", "rb");
    Score entry;
    int i, j;
    if (fscore == NULL)
    {
        printf(RED "No scores recorded yet!\n" RESET);
        return;
    }

    clrscr();
    textcolor(2);
    gotoxy(22, 6);
    printf(YELLOW "______________________________________" RESET);
    gotoxy(25, 8);
    printf(CYAN "       Recorded Scores" RESET);
    gotoxy(22, 9);
    printf(YELLOW "______________________________________" RESET);
    j = 11;
    while (fread(&entry, sizeof(Score), 1, fscore))
    {
        gotoxy(22, j);
        printf(GREEN "Date: %s | Score: %d\n" RESET, entry.dateTime, entry.score);  // Displays score
        j++;
    }

    fclose(fscore);
    getch();
}

// Runs the quiz
void startQuiz(Question questions[])
{
    int score = 0, i;
    char userAnswer;

    for (i = 0; i < TOTAL_QUESTIONS; i++)
    {
        displayQuestion(questions[i], i + 1);
        userAnswer = getUserInput();

        if (userAnswer == questions[i].correctAnswer)   // Check user answer and correct Answer
        {
            gotoxy(22, 19);
            printf(GREEN "Correct!\n" RESET);
            score++;
        }
        else
        {
            gotoxy(22, 19);
            printf(RED "Incorrect! The correct answer was %c.\n" RESET, questions[i].correctAnswer);
        }
        getch(); // Pause for user to view feedback
    }
    gotoxy(22, 21);
    printf(YELLOW "You scored %d out of %d." RESET, score, TOTAL_QUESTIONS);    // Final Score
    storeScore(score);
    getch();
}

int main()
{
    int choice;
    Question questions[TOTAL_QUESTIONS];

    if (!verifyLogin())
    {
        return 1;
    }

    do
    {
        Menu();
        gotoxy(22, 15);
        printf(YELLOW "Enter your choice: " RESET);
        scanf("%d", &choice);

        switch (choice)
        {
        case 1: // Start Quiz
            readQuestionsFromFile("questions.txt", questions);
            startQuiz(questions);
            break;
        case 2: // View Score
            viewScores();
            break;
        case 3: // Exit
            clrscr();
            gotoxy(25, 10);
            printf("Thank you for using the program.\n");
            gotoxy(25, 12);
            printf("Goodbye! Press any key to exit...");
            getch();
            break;
        default:
            printf("Invalid choice! Try again.\n");
        }
        getch();
    } while (choice != 3);

    return 0;
}
