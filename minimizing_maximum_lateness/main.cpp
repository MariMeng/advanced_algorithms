#include <raylib.h>
#include <vector>
#include <algorithm>
#include <random>
#include <cmath>
#include <cstdio>

#define FONT_PATH "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"
#define FONT_BOLD "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf"

struct Job {
    int id;
    int duration;
    int deadline;
    int start    = 0;
    int finish   = 0;
    int lateness = 0;
};

static int              g_n;
static std::vector<Job> g_sched;
static int              g_totalTime;
static int              g_maxLateness;

static Font g_font;
static Font g_fontBold;

static Color PALETTE[] = {
    {255, 100, 100, 255}, {100, 210, 120, 255}, {100, 150, 255, 255},
    {255, 200,  60, 255}, {200, 100, 255, 255}, {255, 150,  60, 255},
    { 60, 220, 220, 255}, {255, 100, 200, 255}, {150, 255, 100, 255},
    {120, 180, 255, 255}, {255, 180, 120, 255}, {200, 200,  80, 255},
    { 80, 255, 200, 255}, {220, 100, 150, 255}, {160, 160, 255, 255},
};

static void txt(const char* s, int x, int y, int sz, Color c) {
    DrawTextEx(g_font, s, {(float)x, (float)y}, (float)sz, 1.0f, c);
}
static void txtB(const char* s, int x, int y, int sz, Color c) {
    DrawTextEx(g_fontBold, s, {(float)x, (float)y}, (float)sz, 1.0f, c);
}
static int msr(const char* s, int sz) {
    return (int)MeasureTextEx(g_font, s, (float)sz, 1.0f).x;
}

static void generate() {
    std::random_device rd;
    std::mt19937 gen(rd());

    g_n = std::uniform_int_distribution<>(10, 15)(gen);
    g_sched.clear();
    g_sched.resize(g_n);

    std::uniform_int_distribution<> durDist(1, 5);
    int sumDur = 0;
    for (int i = 0; i < g_n; i++) {
        g_sched[i].id       = i + 1;
        g_sched[i].duration = durDist(gen);
        sumDur += g_sched[i].duration;
    }

    int dLow  = std::max(2, sumDur / (g_n + 1));
    int dHigh = (int)(sumDur * 1.35);
    std::uniform_int_distribution<> deadDist(dLow, dHigh);
    for (auto& j : g_sched)
        j.deadline = deadDist(gen);

    // EDF: sort by deadline
    std::sort(g_sched.begin(), g_sched.end(),
              [](const Job& a, const Job& b){ return a.deadline < b.deadline; });

    int time = 0;
    g_maxLateness = 0;
    for (auto& job : g_sched) {
        job.start    = time;
        job.finish   = time + job.duration;
        job.lateness = std::max(0, job.finish - job.deadline);
        g_maxLateness = std::max(g_maxLateness, job.lateness);
        time = job.finish;
    }
    g_totalTime = g_sched.back().finish;
}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1280, 800, "Minimizing Maximum Lateness - EDF");
    SetTargetFPS(60);

    g_font     = LoadFontEx(FONT_PATH, 32, nullptr, 250);
    g_fontBold = LoadFontEx(FONT_BOLD, 32, nullptr, 250);
    SetTextureFilter(g_font.texture,     TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(g_fontBold.texture, TEXTURE_FILTER_BILINEAR);

    generate();

    int   step   = 0;
    float timer  = 0.0f;
    float delay  = 1.1f;
    bool  paused = false;

    while (!WindowShouldClose()) {

        // Input
        if (IsKeyPressed(KEY_SPACE))                paused = !paused;
        if (IsKeyPressed(KEY_R))  { generate(); step = 0; timer = 0; paused = false; }
        if (IsKeyPressed(KEY_RIGHT) && step < g_n)  step++;
        if (IsKeyPressed(KEY_LEFT)  && step > 0)    step--;
        if (IsKeyPressed(KEY_UP)   && delay > 0.3f) delay -= 0.1f;
        if (IsKeyPressed(KEY_DOWN) && delay < 3.0f) delay += 0.1f;

        if (!paused && step < g_n) {
            timer += GetFrameTime();
            if (timer >= delay) { timer = 0.0f; step++; }
        }

        const int W       = GetScreenWidth();
        const int H       = GetScreenHeight();
        const int TABLE_X = 14;
        const int TABLE_W = 330;
        const int GANTT_X = TABLE_X + TABLE_W + 18;
        const int GANTT_Y = 110;
        const int GANTT_W = W - GANTT_X - 14;
        const int GANTT_H = 74;
        const int ROW_H   = std::max(22, (H - 210) / (g_n + 1));
        const int TABLE_Y = 92;

        float scale = (float)GANTT_W / (g_totalTime + 1);

        BeginDrawing();
        ClearBackground({12, 12, 30, 255});

        // Title
        txtB("Minimizing Maximum Lateness", 12, 8,  26, {255, 80, 180, 255});
        txt ("Greedy Algorithm: EDF (Earliest Deadline First)", 12, 40, 17, {160, 160, 255, 255});

        char buf[200];
        snprintf(buf, sizeof(buf),
                 "n=%d jobs  |  [SPACE] pause  [R] restart  [< >] step  [^ v] speed (%.1fs)",
                 g_n, delay);
        txt(buf, 12, 64, 13, {90, 90, 150, 255});

        // Table header
        const int C0=TABLE_X, C1=TABLE_X+52, C2=TABLE_X+105,
                  C3=TABLE_X+160, C4=TABLE_X+218, C5=TABLE_X+275;

        txtB("Job",  C0, TABLE_Y, 15, {200, 200, 255, 255});
        txtB("Dur",  C1, TABLE_Y, 15, {180, 220, 255, 255});
        txtB("Dead", C2, TABLE_Y, 15, {255, 220,  80, 255});
        txtB("Fin",  C3, TABLE_Y, 15, {100, 255, 160, 255});
        txtB("Late", C4, TABLE_Y, 15, {255, 120, 120, 255});
        txtB("Ord",  C5, TABLE_Y, 15, {160, 160, 180, 255});
        DrawLine(TABLE_X, TABLE_Y + 20, TABLE_X + TABLE_W, TABLE_Y + 20, {55, 55, 95, 255});

        // Table rows
        for (int i = 0; i < g_n; i++) {
            const auto& job  = g_sched[i];
            int          y   = TABLE_Y + 22 + i * ROW_H;
            bool placed  = (i < step);
            bool current = (i == step - 1);

            if (current) {
                DrawRectangle(TABLE_X-2, y, TABLE_W+4, ROW_H-2, {40, 40, 80, 210});
                DrawRectangleLines(TABLE_X-2, y, TABLE_W+4, ROW_H-2, {100, 100, 200, 200});
            }

            Color c    = placed ? PALETTE[i % 15] : (Color){72, 72, 105, 255};
            Color dimC = placed ? WHITE            : (Color){72, 72, 105, 255};

            snprintf(buf, sizeof(buf), "J%d", job.id);
            txt(buf, C0, y+3, 14, c);

            snprintf(buf, sizeof(buf), "%d", job.duration);
            txt(buf, C1+6, y+3, 14, dimC);

            snprintf(buf, sizeof(buf), "%d", job.deadline);
            txt(buf, C2+6, y+3, 14, dimC);

            if (placed) {
                snprintf(buf, sizeof(buf), "%d", job.finish);
                txt(buf, C3+6, y+3, 14, {100, 255, 160, 255});

                Color lc = job.lateness > 0 ? (Color){255, 100, 100, 255}
                                            : (Color){ 80, 220, 100, 255};
                snprintf(buf, sizeof(buf), "%d", job.lateness);
                txt(buf, C4+12, y+3, 14, lc);
            }

            snprintf(buf, sizeof(buf), "%d", i+1);
            txt(buf, C5+12, y+3, 13, {130, 130, 160, 255});
        }

        // Gantt background
        txt("Timeline (Gantt)", GANTT_X, GANTT_Y - 24, 16, WHITE);
        DrawRectangle(GANTT_X, GANTT_Y, GANTT_W, GANTT_H, {20, 20, 50, 255});
        DrawRectangleLines(GANTT_X, GANTT_Y, GANTT_W, GANTT_H, {55, 55, 95, 255});

        // Job bars
        for (int i = 0; i < step; i++) {
            const auto& job = g_sched[i];
            int bx = GANTT_X + (int)(job.start    * scale);
            int bw = (int)   (job.duration * scale);
            Color c = PALETTE[i % 15];

            DrawRectangle(bx, GANTT_Y, bw, GANTT_H, c);
            DrawRectangleLines(bx, GANTT_Y, bw, GANTT_H, {0, 0, 0, 160});

            snprintf(buf, sizeof(buf), "J%d", job.id);
            int tw = msr(buf, 13);
            if (bw >= tw + 6)
                txt(buf, bx + (bw - tw)/2, GANTT_Y + GANTT_H/2 - 7, 13, {0, 0, 0, 220});
        }

        // Pulsing outline on current bar
        if (step > 0 && step <= g_n) {
            const auto& cur = g_sched[step - 1];
            int bx = GANTT_X + (int)(cur.start    * scale);
            int bw = (int)   (cur.duration * scale);
            float p = (float)(sin(GetTime() * 5.0) * 0.5 + 0.5);
            DrawRectangleLines(bx, GANTT_Y, bw, GANTT_H,
                               {255, 255, 255, (unsigned char)(p * 200 + 55)});
        }

        // Timeline ticks
        {
            int ts = std::max(1, g_totalTime / 20);
            for (int t = 0; t <= g_totalTime; t += ts) {
                int x = GANTT_X + (int)(t * scale);
                DrawLine(x, GANTT_Y + GANTT_H, x, GANTT_Y + GANTT_H + 6, {90, 90, 140, 255});
                snprintf(buf, sizeof(buf), "%d", t);
                txt(buf, x - 5, GANTT_Y + GANTT_H + 8, 12, {110, 110, 160, 255});
            }
        }

        // Deadline markers
        for (int i = 0; i < step; i++) {
            const auto& job = g_sched[i];
            int dx  = GANTT_X + (int)(job.deadline * scale);
            Color dc = job.lateness > 0 ? (Color){255, 80,  80, 255}
                                        : (Color){ 80, 255, 120, 255};

            DrawLine(dx, GANTT_Y - 22, dx, GANTT_Y + GANTT_H, dc);
            DrawTriangle({(float)dx,     (float)(GANTT_Y - 6)},
                         {(float)(dx-5), (float)(GANTT_Y - 22)},
                         {(float)(dx+5), (float)(GANTT_Y - 22)}, dc);

            int labelY = GANTT_Y - 38 - (i % 3) * 13;
            snprintf(buf, sizeof(buf), "d%d=%d", job.id, job.deadline);
            txt(buf, dx - 8, labelY, 10, dc);
        }

        // Lateness brackets
        {
            int baseY = GANTT_Y + GANTT_H + 30;
            int row   = 0;
            for (int i = 0; i < step; i++) {
                const auto& job = g_sched[i];
                if (job.lateness <= 0) continue;
                int x1 = GANTT_X + (int)(job.deadline * scale);
                int x2 = GANTT_X + (int)(job.finish   * scale);
                int by = baseY + (row % 3) * 20;
                row++;

                DrawLine(x1, by, x2, by, {255, 80, 80, 255});
                DrawLine(x1, by-4, x1, by+4, {255, 80, 80, 255});
                DrawLine(x2, by-4, x2, by+4, {255, 80, 80, 255});

                snprintf(buf, sizeof(buf), "l%d=%d", job.id, job.lateness);
                int tw = msr(buf, 11);
                if (x2 - x1 > 6)
                    txt(buf, (x1+x2)/2 - tw/2, by+5, 11, {255, 130, 130, 255});
            }
        }

        // Stats bar
        int statsY = H - 118;
        DrawLine(0, statsY - 6, W, statsY - 6, {38, 38, 78, 255});

        if (step == 0) {
            txt("Jobs sorted by deadline (EDF). Press SPACE to start.",
                GANTT_X, statsY, 15, {200, 200, 100, 255});
        } else {
            const auto& cur = g_sched[step - 1];
            snprintf(buf, sizeof(buf),
                     "J%d scheduled: start=%d  dur=%d  finish=%d  deadline=%d  ->  lateness=%d",
                     cur.id, cur.start, cur.duration, cur.finish, cur.deadline, cur.lateness);
            txt(buf, GANTT_X, statsY, 15, {200, 220, 255, 255});
        }

        int curMax = 0;
        for (int i = 0; i < step; i++)
            curMax = std::max(curMax, g_sched[i].lateness);

        snprintf(buf, sizeof(buf), "Current max lateness  L = %d", curMax);
        Color lc = curMax > 0 ? (Color){255, 100, 100, 255} : (Color){80, 220, 100, 255};
        txtB(buf, GANTT_X, statsY + 30, 20, lc);

        if (step == g_n) {
            int lateCount = 0;
            for (auto& j : g_sched) if (j.lateness > 0) lateCount++;
            snprintf(buf, sizeof(buf),
                     "Done!  L* = %d (optimal by EDF)   %d/%d jobs late",
                     g_maxLateness, lateCount, g_n);
            Color fc = g_maxLateness > 0 ? (Color){255, 160, 50, 255} : (Color){80, 255, 150, 255};
            txt(buf, GANTT_X, statsY + 62, 17, fc);
        }

        // Left panel bottom
        DrawLine(TABLE_X, statsY-6, TABLE_X+TABLE_W, statsY-6, {38, 38, 78, 255});
        txt("Complexity:",        TABLE_X, statsY,      14, {160, 160, 200, 255});
        txt("Time:  O(n log n)",  TABLE_X, statsY + 20, 13, {100, 220, 100, 255});
        txt("Space: O(n)",        TABLE_X, statsY + 36, 13, {100, 220, 100, 255});
        snprintf(buf, sizeof(buf), "Step %d / %d", step, g_n);
        txt(buf, TABLE_X, statsY + 60, 14, {160, 160, 200, 255});

        EndDrawing();
    }

    UnloadFont(g_font);
    UnloadFont(g_fontBold);
    CloseWindow();
    return 0;
}
