#include <3ds.h>
#include <stdio.h>
#include <math.h>

typedef struct {
    float x, y;
} Obj;

int main(int argc, char **argv)
{
    gfxInitDefault();
    consoleInit(GFX_TOP, NULL);

    PrintConsole bottom;
    consoleInit(GFX_BOTTOM, &bottom);

    Obj player = {80,120};
    Obj enemy  = {320,120};
    Obj ball   = {200,120};

    float ballVX = 2.0f;
    float ballVY = 1.5f;

    int scorePlayer = 0;
    int scoreEnemy = 0;

    while (aptMainLoop())
    {
        hidScanInput();

        u32 kDown = hidKeysDown();
        if(kDown & KEY_START)
            break;

        circlePosition pos;
        hidCircleRead(&pos);

        player.x += pos.dx / 20.0f;
        player.y -= pos.dy / 20.0f;

        if(player.x < 5) player.x = 5;
        if(player.x > 395) player.x = 395;
        if(player.y < 10) player.y = 10;
        if(player.y > 230) player.y = 230;

        if(enemy.y < ball.y) enemy.y += 1.5f;
        if(enemy.y > ball.y) enemy.y -= 1.5f;

        enemy.x = 320;

        float dx = ball.x - player.x;
        float dy = ball.y - player.y;

        if(dx*dx + dy*dy < 225)
        {
            ballVX = dx * 0.15f;
            ballVY = dy * 0.15f;
        }

        dx = ball.x - enemy.x;
        dy = ball.y - enemy.y;

        if(dx*dx + dy*dy < 225)
        {
            ballVX = dx * 0.15f;
            ballVY = dy * 0.15f;
        }

        ball.x += ballVX;
        ball.y += ballVY;

        if(ball.y < 5 || ball.y > 235)
            ballVY = -ballVY;

        if(ball.x < 0)
        {
            scoreEnemy++;
            ball.x = 200;
            ball.y = 120;
            ballVX = 2;
            ballVY = 1;
        }

        if(ball.x > 399)
        {
            scorePlayer++;
            ball.x = 200;
            ball.y = 120;
            ballVX = -2;
            ballVY = 1;
        }

        consoleSelect(GFX_TOP);
        consoleClear();

        printf("===== 3DS Soccer =====\n\n");
        printf("Campo: 400x240\n\n");

        printf("Jogador Azul : %.0f %.0f\n",player.x,player.y);
        printf("CPU Vermelha : %.0f %.0f\n",enemy.x,enemy.y);
        printf("Bola         : %.0f %.0f\n",ball.x,ball.y);

        printf("\nGol Esquerda = CPU");
        printf("\nGol Direita  = Jogador");

        consoleSelect(GFX_BOTTOM);
        consoleClear();

        printf("PLACAR\n\n");
        printf("Voce : %d\n",scorePlayer);
        printf("CPU  : %d\n\n",scoreEnemy);

        printf("Circle Pad = mover\n");
        printf("START = sair\n");

        gfxFlushBuffers();
        gfxSwapBuffers();
        gspWaitForVBlank();
    }

    gfxExit();
    return 0;
}
