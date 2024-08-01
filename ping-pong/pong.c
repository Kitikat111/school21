#include <math.h>
#include <stdio.h>

void show_field(int rocket_1, int rocket_2, int ball_x, int ball_y, int score_player_1, int score_player_2);
int check_Wall_x(int ball_x, int ball_y, int vector_X, int rocket_1, int rocket_2);
int check_Wall_y(int ball_y, int vector_Y, int heigh);

int main() {
    int rocket_1 = 12, rocket_2 = 12, ball_x = 39, ball_y = 12, score_player_1 = 0, score_player_2 = 0,
        heigh = 25, vector_X = 1, vector_Y = 1;
    show_field(rocket_1, rocket_2, ball_x, ball_y, score_player_1, score_player_2);
    while (score_player_1 < 21 && score_player_2 < 21) {
        char c;
        scanf("%c", &c);
        // Управление ракетками
        if (c == 'z') {
            if (rocket_1 != 23) rocket_1++;
        } else if (c == 'a') {
            if (rocket_1 != 1) rocket_1--;
        } else if (c == 'm') {
            if (rocket_2 != 23) rocket_2++;
        } else if (c == 'k') {
            if (rocket_2 != 1) rocket_2--;
        } else if (c != ' ')
            continue;
        ball_x += vector_X;
        // Сдвиг шарика
        ball_y += vector_Y;
        vector_X = check_Wall_x(ball_x, ball_y, vector_X, rocket_1, rocket_2);
        vector_Y = check_Wall_y(ball_y, vector_Y, heigh);
        // Изменение счета
        if (ball_x == 79) {
            score_player_1 = score_player_1 + 1;
            ball_x = 39, ball_y = 12, vector_X = 1, vector_Y = 1;
        } else if (ball_x == 0) {
            score_player_2 = score_player_2 + 1;
            ball_x = 39, ball_y = 12, vector_X = 1, vector_Y = 1;
        }
        // Выход из цикла при счете 21
        if (score_player_1 == 21) {
            printf("\033[H\033[JСчёт: %d - %d \nПоздравляем с победой игрока 1!\n", score_player_1,
                   score_player_2);
            break;
        } else if (score_player_2 == 21) {
            printf("\033[H\033[JСчёт: %d - %d \nПоздравляем с победой игрока 2!\n", score_player_1,
                   score_player_2);
            break;
        }
        show_field(rocket_1, rocket_2, ball_x, ball_y, score_player_1, score_player_2);
    }
    return 0;
}

// Рисовка поля
void show_field(int rocket_1, int rocket_2, int ball_x, int ball_y, int score_player_1, int score_player_2) {
    int x_max = 80, y_max = 25;  // Размеры поля
    printf(
        "\033[H\033[J------------------------------------------------------------------------"
        "---\n| Score: %d - %d | First player controls: a-z | Second player controls: "
        "k-m |\n",
        score_player_1, score_player_2);
    for (int i = 0; i < x_max + 2; i++) printf("-");  // Верхняя граница
    printf("\n");
    for (int y_cur = 0; y_cur < y_max; y_cur++) {
        printf("|");  // Левая граница
        int x_cur = 0;
        while (x_cur < x_max) {
            if (x_cur == 0 && y_cur == rocket_1)
                printf("]");  // Первая ракетка
            else if (x_cur == 0 && y_cur == rocket_1 - 1)
                printf("]");
            else if (x_cur == 0 && y_cur == rocket_1 + 1)
                printf("]");
            else if (x_cur == ball_x && y_cur == ball_y)
                printf("O");  // Шарик
            else if (x_cur == 38)
                printf("|");  // Середина
            else if (x_cur == 39)
                printf("|");  // Середина
            else if (x_cur == x_max - 1 && y_cur == rocket_2)
                printf("[");  // Вторая ракетка
            else if (x_cur == x_max - 1 && y_cur == rocket_2 - 1)
                printf("[");
            else if (x_cur == x_max - 1 && y_cur == rocket_2 + 1)
                printf("[");
            else
                printf(" ");
            x_cur++;
        }
        printf("|\n");  // Правая граница
    }
    for (int i = 0; i < x_max + 2; i++) printf("-");  // Нижняя граница
    printf("\n");
}
// Отскок от ракеток
int check_Wall_x(int ball_x, int ball_y, int vector_X, int rocket_1, int rocket_2) {
    if (ball_x == 1 && (ball_y == rocket_1 - 1 || ball_y == rocket_1 || ball_y == rocket_1 + 1)) {
        vector_X = -(vector_X);
    }
    if (ball_x == 78 && (ball_y == rocket_2 - 1 || ball_y == rocket_2 || ball_y == rocket_2 + 1)) {
        vector_X = -(vector_X);
    }
    return vector_X;
}
// Отскок от стенок
int check_Wall_y(int ball_y, int vector_Y, int heigh) {
    if (ball_y == 0 || ball_y == heigh - 1) {
        vector_Y = -(vector_Y);
    }
    return vector_Y;
}
