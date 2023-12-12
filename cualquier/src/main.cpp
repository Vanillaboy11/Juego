#include "raylib.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ANSWER_LEN 50 // Longitud maxima de la respuesta

// estructura para las preguntas y respuestas del usuario

typedef struct
{
    char question[100];
    char answer[MAX_ANSWER_LEN];
} Question;

// estructura para el menu

typedef enum
{
    MENU,
    START_GAME,
    OPTIONS,
    EXIT
} GameScene;

// estructura para la dificultad

typedef enum
{
    EASY,
    HARD
} GameDifficulty;

bool runningGame = true; // Variable de control del estado del juego

// revisa que la respuesta sea correcta

bool checkAnswer(const char *userAnswer, const char *correctAnswer)
{
    return (strcmp(userAnswer, correctAnswer) == 0);
}

void RunGame(GameDifficulty difficulty);

int main(void)
{
    SetTargetFPS(60);

    Sound selectSound; // declaracion del sonido

    Music menuMusic; // Declaración de la música para el men
    const int screenWidth = 1920;
    const int screenHeight = 1080;

    InitAudioDevice();
    InitWindow(screenWidth, screenHeight, "Historiadores");

    // Cargar la imagen de fondo
    Texture2D background = LoadTexture("C:\\Users\\zero_\\Desktop\\fondo3.png");

    // cargar sonido
    selectSound = LoadSound("c:\\Users\\zero_\\Desktop\\interface-124464.mp3");
    menuMusic = LoadMusicStream("C:\\Users\\zero_\\Desktop\\juego mex.mp3");

    GameScene currentScene = MENU;
    GameDifficulty difficulty = EASY;

    while (!WindowShouldClose())
    {

        BeginDrawing();
        ClearBackground(RAYWHITE);
        PlayMusicStream(menuMusic);      // Reproducir la música del menú
        SetMusicVolume(menuMusic, 0.2f); // Ajustar el volumen de la música
        Rectangle barra = {810, 480, 320, 300};
        Rectangle barra2 = {750, 470, 430, 300};

        // Dibujar el fondo
        DrawTexture(background, 0, 0, WHITE);

        // Dibujar el menú según el estado actual
        switch (currentScene)
        {
        case MENU:
            UpdateMusicStream(menuMusic);
            DrawText("MENU", screenWidth / 2 - MeasureText("MENU", 60) / 2, 300, 60, WHITE);
            DrawRectangleRounded(barra, 0.35, 0.50, BLACK);
            DrawText("1. Jugar", 890, 500, 40, WHITE);
            DrawText("2. Dificultad", 850, 600, 40, WHITE);
            DrawText("3. Salir", 890, 700, 40, WHITE);

            // Lógica para cambiar el estado del menú
            if (IsKeyPressed(KEY_ONE))
            {
                PlaySound(selectSound);
                currentScene = START_GAME;
            }
            else if (IsKeyPressed(KEY_TWO))
            {
                PlaySound(selectSound);
                currentScene = OPTIONS;
            }
            else if (IsKeyPressed(KEY_THREE))
            {
                currentScene = EXIT;
            }
            break;

        case START_GAME:
            StopMusicStream(menuMusic); // Detener la música del menú al iniciar el juego
            RunGame(difficulty);        // Llama a la función que ejecuta el juego
            currentScene = MENU;        // Una vez que el juego termina, vuelve al menú
            PlayMusicStream(menuMusic); // Volver a reproducir la música del menú al regresar
            break;

        case OPTIONS:
            UpdateMusicStream(menuMusic);
            DrawText("DIFICULTAD", screenWidth / 2 - MeasureText("DIFICULTAD", 60) / 2, 300, 60, WHITE);
            DrawRectangleRounded(barra2, 0.35, 0.50, BLACK);
            DrawText("F. Fácil", 800, 500, 40, WHITE);
            DrawText("D. Difícil", 800, 600, 40, WHITE);
            DrawText("M. Volver al Menú", 790, 700, 40, WHITE);

            if (IsKeyPressed(KEY_F))
            {
                PlaySound(selectSound);
                currentScene = OPTIONS;
                difficulty = EASY; // Cambiar la dificultad a Fácil
            }
            else if (IsKeyPressed(KEY_D))
            {
                PlaySound(selectSound);
                currentScene = OPTIONS;
                difficulty = HARD; // Cambiar la dificultad a Difícil
            }
            else if (IsKeyPressed(KEY_M))
            {
                PlaySound(selectSound);
                currentScene = MENU;
            }
            break;

        case EXIT:
            CloseWindow();
            break;

        default:
            break;
        }

        EndDrawing();
    }

    UnloadTexture(background); // Descargar la textura después de usarla
    UnloadSound(selectSound);  // Descargar el sonido al salir del programa
    CloseWindow();

    return 0;
}

void RunGame(GameDifficulty difficulty)
{
    const int screenWidth = 1920;
    const int screenHeight = 1080;

    Sound correctSound;   // declaración del sonido para respuestas correctas
    Sound incorrectSound; // declaración del sonido para respuestas incorrectas
    Music gameMusic;
    correctSound = LoadSound("c:\\Users\\zero_\\Desktop\\correct.mp3");
    incorrectSound = LoadSound("C:\\Users\\zero_\\Desktop\\training-program-incorrect1-88736.mp3");

    Font font = LoadFont("C:\\Users\\zero_\\Desktop\\Lato-Regular.ttf"); // Reemplaza con la ruta a tu archivo de fuente

    gameMusic = LoadMusicStream("C:\\Users\\zero_\\Desktop\\juego 4.mp3");

    Rectangle ans = {370, 560, 1080, 80};

    int MAX_QUESTIONS;

    FILE *file;

    if (difficulty == EASY)
    {
        file = fopen("preguntas.txt", "r"); // abre preguntas faciles
        if (file == NULL)
        {
            printf("Error al abrir el archivo.\n");
        }

        MAX_QUESTIONS = 10;
    }
    else
    {
        file = fopen("C:\\Users\\zero_\\Desktop\\qstHard.txt", "r"); // abre preguntas dificiles
        if (file == NULL)
        {
            printf("Error al abrir el archivo.\n");
        }
        MAX_QUESTIONS = 20;
    }

    Question questions[MAX_QUESTIONS];
    int numQuestions = 0;

    char line[256];
    while (fgets(line, sizeof(line), file)) // se encarga de cambiar de pregunta
    {
        char *token = strtok(line, "-\n");

        if (token != NULL)
        {
            strcpy(questions[numQuestions].question, token);

            token = strtok(NULL, "-\n");
            if (token != NULL)
            {
                strcpy(questions[numQuestions].answer, token);
                numQuestions++;
            }
        }

        if (numQuestions >= MAX_QUESTIONS)
        {
            break;
        }
    }

    fclose(file);

    Texture2D backgroundImage = LoadTexture("C:\\Users\\zero_\\Desktop\\fondo3.png"); // agrega un fondow

    int currentQuestion = 0;
    char userInput[MAX_ANSWER_LEN];
    bool gameOver = false;
    double startTime = GetTime(); // Inicializa el tiempo al iniciar el juego
    int score = 0;
    bool timerExpired = false;

    int time; // tiempo

    if (difficulty == EASY)
    {
        time = 20;
    }
    else
    {
        time = 10;
    }

    SetTargetFPS(60);

    bool startGame = false;

    gameMusic = LoadMusicStream("C:\\Users\\zero_\\Desktop\\juego 4.mp3");

    while (!WindowShouldClose())
    {
        PlayMusicStream(gameMusic);      // Reproducir la música del menú
        SetMusicVolume(gameMusic, 0.2f); // Ajustar el volumen de la música
        UpdateMusicStream(gameMusic);

        double currentTime = GetTime();
        double elapsedTime = currentTime - startTime;
        if (!startGame)
        {

            startGame = true;
        }
        else
        {
            if (!gameOver)
            {
                if (elapsedTime >= time && !timerExpired)
                {
                    timerExpired = true;
                    printf("Tiempo agotado. Respuesta incorrecta.\n");
                    PlaySound(incorrectSound); // Reproducir el sonido de respuesta incorrecta
                    currentQuestion++;
                    if (currentQuestion >= MAX_QUESTIONS)
                    {
                        gameOver = true;
                    }
                    userInput[0] = '\0';
                    startTime = GetTime(); // Reinicia el tiempo para la siguiente pregunta
                    timerExpired = false;  // Reinicia el temporizador
                }

                BeginDrawing();
                ClearBackground((Color){255, 255, 200, 255});

                DrawTexture(backgroundImage, 0, 0, WHITE);

                // Medir la longitud del texto para centrarlo horizontalmente
                float textWidth = MeasureText(questions[currentQuestion].question, 30);
                float posX = (screenWidth - textWidth) / 2; // Calcular la posición X para centrar
                float posYQuestion = screenHeight / 4;      // Ajustar la posición vertical de la pregunta
                float posYAnswer = posYQuestion + 200;      // Ajustar la posición vertical de la respuesta

                DrawText(questions[currentQuestion].question, posX, posYQuestion, 30, WHITE); // escribe la pregunta
                DrawText("Ingrese su respuesta:", 200, posYAnswer, 30, WHITE);                // Ajustar la posición vertical de la respuesta
                DrawRectangleRounded(ans, 0.35, 0.50, BLACK);

                if (!timerExpired)
                {
                    int key = GetKeyPressed();
                    if (key != 0)
                    {
                        if (key != KEY_ENTER && key != KEY_BACKSPACE && strlen(userInput) < MAX_ANSWER_LEN - 1)
                        {
                            int len = strlen(userInput);
                            userInput[len] = (char)key;
                            userInput[len + 1] = '\0';
                        }
                        else if (key == KEY_BACKSPACE && strlen(userInput) > 0)
                        {
                            userInput[strlen(userInput) - 1] = '\0';
                        }
                        else if (key == KEY_ENTER)
                        {
                            if (checkAnswer(userInput, questions[currentQuestion].answer))
                            {
                                printf("¡Respuesta correcta!\n");
                                PlaySound(correctSound); // Reproducir el sonido de respuesta correcta
                                score += 10;             // Cada respuesta correcta otorga 8 puntos
                            }
                            else
                            {
                                printf("Respuesta incorrecta.\n");
                                PlaySound(incorrectSound); // Reproducir el sonido de respuesta incorrecta
                            }

                            currentQuestion++;
                            if (currentQuestion >= MAX_QUESTIONS)
                            {
                                gameOver = true;
                            }
                            else
                            {
                                userInput[0] = '\0';
                                timerExpired = false;
                                startTime = GetTime(); // Reinicia el tiempo para la siguiente pregunta
                            }
                        }
                    }
                }

                DrawText(userInput, 400, 600, 30, WHITE);
                DrawText(TextFormat("Puntuación: %d/100", score), 200, 900, 30, WHITE);

                EndDrawing();
            }
            else
            {
                FILE *scoreFile = fopen("puntuacion.txt", "w");
                if (scoreFile != NULL)
                {
                    fprintf(scoreFile, "Puntuación final: %d/100\n", score);
                    fclose(scoreFile);
                }

                BeginDrawing();

                if (score >= 60)
                {
                    DrawTexture(backgroundImage, 0, 0, YELLOW);
                    DrawText("¡Juego terminado!", 800, 300, 40, WHITE);
                    DrawText("FELICIDADES GANASTE!!!", 730, 500, 40, YELLOW);
                    DrawText(TextFormat("Puntuación: %d/100", score), 850, 600, 30, WHITE);
                }
                else
                {
                    DrawTexture(backgroundImage, 0, 0, WHITE);
                    DrawText("¡Juego terminado!", 800, 300, 40, WHITE);
                    DrawText("HAZ PERDIDO", 850, 500, 40, RED);
                    DrawText(TextFormat("Puntuación: %d/100", score), 850, 600, 30, WHITE);
                }
                DrawText("Presione ESC para volver al inicio", 730, 700, 30, WHITE);
                EndDrawing();
                UnloadTexture(backgroundImage);
            }
        }
    }

    UnloadTexture(backgroundImage); // Descarga la textura cuando ya no se necesite
    UnloadSound(correctSound);
    UnloadSound(incorrectSound);
}
