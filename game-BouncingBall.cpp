#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <tchar.h>
#include <chrono>
#include <thread>
#include <windows.h>

int main()
{
    int x = 100, y = 580, r = 20;
    int vy = 0, g = 1;
    int w = 640, h = 480;
    int score = 0;
    int jumpHeight = -25;
    int maxJumps = 1;

    int jumpCounter = 0;
    int jumpCooldown = 200;
    auto lastKeyPressTime = std::chrono::steady_clock::now();

    int speedIncreaseInterval = 5000;
    auto lastSpeedIncreaseTime = std::chrono::steady_clock::now();

    initgraph(w, h);
    srand((unsigned int)time(NULL));

    int obs_x = w;
    int obs_y = h - (rand() % 100 + 50);
    int obs_width = 30;
    int obs_height = h - obs_y;
    int obs_speed = rand() % 5 + 5;

    while (1)
    {
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastSpeedIncreaseTime).count();
        if (elapsedTime >= speedIncreaseInterval)
        {
            obs_speed++;
            lastSpeedIncreaseTime = currentTime;
        }

        if (GetAsyncKeyState(VK_SPACE) & 0x8000)
        {
            auto keyPressTime = std::chrono::steady_clock::now();
            auto timeSinceLastPress = std::chrono::duration_cast<std::chrono::milliseconds>(keyPressTime - lastKeyPressTime).count();
            if (timeSinceLastPress >= jumpCooldown)
            {
                if (jumpCounter < maxJumps)
                {
                    vy = jumpHeight;
                    jumpCounter++;
                    lastKeyPressTime = keyPressTime;
                }
            }
        }
        else
        {
            if (y >= h - r)
            {
                y = h - r;
                vy = 0;
                jumpCounter = 0;
                maxJumps = score + 1;
            }
        }

        vy += g;
        y += vy;

        obs_x -= obs_speed;

        if (obs_x + obs_width < 0)
        {
            obs_x = w;
            obs_height = rand() % 100 + 50;
            obs_y = h - obs_height;
            score++;
            obs_speed = rand() % 5 + 5;
        }

        if ((x + r >= obs_x && x - r <= obs_x + obs_width) && (y + r >= obs_y))
        {
            printf("Game Over! Final Score: %d\n", score);
            break;
        }

        cleardevice();
        fillcircle(x, y, r);
        fillrectangle(obs_x, obs_y, obs_x + obs_width, obs_y + obs_height);

        settextstyle(20, 0, _T("Arial"));
        TCHAR score_text[20];
        _stprintf_s(score_text, _T("Score: %d"), score);
        outtextxy(10, 10, score_text);

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    closegraph();
    return 0;
}
 