#include <graphx.h>
#include <keypadc.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <sys/rtc.h>
#include <ti/getcsc.h>

int main(void)
{

    gfx_Begin();
    gfx_FillScreen(255);
    gfx_SetTextScale(2,2);

    srand(rtc_Time());

    int x = 20, y = 220, col = 20, vel = 0, gravity = 1, i = 1, speed = 5, score = 0;
    int curx = x;
    int cury = y;
    bool run = true, change = true, jumping = false;
    bool key, prevkey;

    int obstacle_types[3][4] = {
        {300,220,20,20}, //x, y, width, height
        {300,210,20,10}, //x, y, width, height
        {300,230,40,10}
    };
    int next = 150+(rand() % 100);

    int obstacles[2][4];
    int obstacle = rand() % 3;
    memcpy(obstacles[0], obstacle_types[obstacle], sizeof(obstacle_types[obstacle]));
    obstacles[1][0] = -100;

    while(run) {
        if (change){
            gfx_SetColor(255);
            gfx_FillRectangle_NoClip(curx,cury,20,20);
            gfx_SetColor(col);
            gfx_FillRectangle_NoClip(x,y,20,20);
            change = false;
        }
        cury = y;
        kb_Scan();
        key = kb_Data[7] == kb_Up;
        if (key && !prevkey){
            if (!jumping){
                jumping = true;
                vel = 10;
            }
        }
        prevkey = key;
        if (kb_Data[6] & kb_Enter){
            run = false;
        }
        if (jumping){
            if (220 < y){
                y = 220;
                jumping = false;
            }
            change = true;
        }
        i++;
        if (i % 15 == 0){
            gfx_SetColor(255);
            gfx_FillRectangle(0, 0, (floor(log10(score)) + 2)*12, 15);
            gfx_PrintInt(score, floor(log10(score)) + 1);
            gfx_SetTextXY(0,0);
            if (jumping){
                y -= vel;
                vel -= gravity;
            }
            for (int j = 0; j < 2; j++){
                if (obstacles[j][0] != -100){
                    obstacles[j][0]-=speed;
                    gfx_SetColor(255);
                    gfx_FillRectangle(obstacles[j][0]+obstacles[j][2], obstacles[j][1], speed, obstacles[j][3]);
                    gfx_SetColor(128);
                    gfx_FillRectangle(obstacles[j][0], obstacles[j][1], obstacles[j][2], obstacles[j][3]);
                    if (x < obstacles[j][0] + obstacles[j][2] && x + 20 > obstacles[j][0] &&
                        y < obstacles[j][1] + obstacles[j][3] && y + 20 > obstacles[j][1]) {
                        run = false;
                    }
                }
                if (obstacles[j][0] + obstacles[j][2] < 0) {
                    obstacles[j][0] = -100;
                }
                if (300-obstacles[j][0] >= next && obstacles[j][0] != -100){
                    for (int q = 0; q < 2; q++){
                        if (obstacles[q][0] == -100) {
                            obstacle = rand() % 3;
                            memcpy(obstacles[q], obstacle_types[obstacle], sizeof(obstacle_types[obstacle]));
                            next = 150 + (rand() % 100);
                            break;
                        }
                    }
                }
            }
            score++;
        }
        if (i == 3000){
            speed++;
            i = 1;
        }
    }

    gfx_FillScreen(255);
    gfx_SetTextScale(4,4);
    char str[(int) floor(log10(score) + 1)];
    sprintf(str, "%d", score);
    gfx_PrintStringXY(str,
                      (GFX_LCD_WIDTH - gfx_GetStringWidth(str)) / 2,
                      (GFX_LCD_HEIGHT - 8) / 2);
    while (!os_GetCSC());
    gfx_End();
    
    return 0;
}
