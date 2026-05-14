#include <raylib.h>
#include <vector>
#include <algorithm>
#include <random>
#include <string>
#include <cmath>
#include <cstdio>

struct Job {
    int id;
    int duration;
    int deadline;
    int start    = 0;
    int finish   = 0;
    int lateness = 0;
};

static int              g_n;
static std::vector<Job> g_original;
static std::vector<Job> g_sched;
static int              g_totalTime;
static int              g_maxLateness;

static Color PALETTE[] = {
    {255, 100, 100, 255}, {100, 210, 120, 255}, {100, 150, 255, 255},
    {255, 200,  60, 255}, {200, 100, 255, 255}, {255, 150,  60, 255},
    { 60, 220, 220, 255}, {255, 100, 200, 255}, {150, 255, 100, 255},
    {120, 180, 255, 255}, {255, 180, 120, 255}, {200, 200,  80, 255},
    { 80, 255, 200, 255}, {220, 100, 150, 255}, {160, 160, 255, 255},
};

static void generate() {
    std::random_device rd;
    std::mt19937 gen(rd());

    g_n = std::uniform_int_distribution<>(10, 15)(gen);

    g_original.clear();
    g_original.resize(g_n);

    std::uniform_int_distribution<> durDist(1, 5);
    int sumDur = 0;
    for (int i = 0; i < g_n; i++) {
        g_original[i].id       = i + 1;
        g_original[i].duration = durDist(gen);
        sumDur += g_original[i].duration;
    }

    // Deadlines: spread so some jobs will be late, some won't
    int dLow  = std::max(2, sumDur / (g_n + 1));
    int dHigh = (int)(sumDur * 1.35);
    std::uniform_int_distribution<> deadDist(dLow, dHigh);
    for (auto& j : g_original)
        j.deadline = deadDist(gen);

    // EDF: sort by deadline ascending
    g_sched = g_original;
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
    const int SW = 1280;
    const int SH = 800;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(SW, SH, "Minimização do Atraso Máximo — EDF");
    SetTargetFPS(60);

    generate();

    int   step    = 0;
    float timer   = 0.0f;
    float delay   = 1.1f;
    bool  paused  = false;

    while (!WindowShouldClose()) {

        // ── Input ──────────────────────────────────────────────────────────
        if (IsKeyPressed(KEY_SPACE))                         paused = !paused;
        if (IsKeyPressed(KEY_R))  { generate(); step = 0; timer = 0; paused = false; }
        if (IsKeyPressed(KEY_RIGHT) && step < g_n)          step++;
        if (IsKeyPressed(KEY_LEFT)  && step > 0)            step--;
        if (IsKeyPressed(KEY_UP)   && delay > 0.3f)         delay -= 0.1f;
        if (IsKeyPressed(KEY_DOWN) && delay < 3.0f)         delay += 0.1f;

        if (!paused && step < g_n) {
            timer += GetFrameTime();
            if (timer >= delay) { timer = 0.0f; step++; }
        }

        // ── Layout ─────────────────────────────────────────────────────────
        const int W        = GetScreenWidth();
        const int H        = GetScreenHeight();
        const int TABLE_X  = 14;
        const int TABLE_W  = 318;
        const int GANTT_X  = TABLE_X + TABLE_W + 18;
        const int GANTT_Y  = 105;
        const int GANTT_W  = W - GANTT_X - 14;
        const int GANTT_H  = 72;
        const int ROW_H    = std::max(22, (H - 200) / (g_n + 1));

        float scale = (float)GANTT_W / (g_totalTime + 1);

        // ── Draw ───────────────────────────────────────────────────────────
        BeginDrawing();
        ClearBackground({12, 12, 30, 255});

        // Title
        DrawText("Minimização do Atraso Máximo", 12, 10, 26, {255, 80, 180, 255});
        DrawText("Algoritmo Guloso: EDF (Earliest Deadline First)",
                 12, 42, 17, {160, 160, 255, 255});

        char buf[160];
        snprintf(buf, sizeof(buf),
                 "n=%d tarefas  |  [SPACE] pausar  [R] reiniciar  "
                 "[←→] passo  [↑↓] velocidade (%.1fs)", g_n, delay);
        DrawText(buf, 12, 64, 13, {90, 90, 150, 255});

        // ── Table header ───────────────────────────────────────────────────
        const int TABLE_Y = 88;
        const int C0 = TABLE_X,      C1 = TABLE_X + 48, C2 = TABLE_X + 100,
                  C3 = TABLE_X + 155, C4 = TABLE_X + 210, C5 = TABLE_X + 265;

        DrawText("Job", C0, TABLE_Y, 15, {200, 200, 255, 255});
        DrawText("tᵢ",  C1, TABLE_Y, 15, {180, 220, 255, 255});
        DrawText("dᵢ",  C2, TABLE_Y, 15, {255, 220,  80, 255});
        DrawText("fᵢ",  C3, TABLE_Y, 15, {100, 255, 160, 255});
        DrawText("lᵢ",  C4, TABLE_Y, 15, {255, 120, 120, 255});
        DrawText("#",   C5, TABLE_Y, 15, {160, 160, 180, 255});
        DrawLine(TABLE_X, TABLE_Y + 18, TABLE_X + TABLE_W, TABLE_Y + 18,
                 {55, 55, 95, 255});

        // ── Table rows ─────────────────────────────────────────────────────
        for (int i = 0; i < g_n; i++) {
            const auto& job  = g_sched[i];
            int          y   = TABLE_Y + 20 + i * ROW_H;
            bool placed  = (i < step);
            bool current = (i == step - 1);

            if (current) {
                DrawRectangle(TABLE_X - 2, y, TABLE_W + 4, ROW_H - 2,
                              {40, 40, 80, 210});
                DrawRectangleLines(TABLE_X - 2, y, TABLE_W + 4, ROW_H - 2,
                                   {100, 100, 200, 200});
            }

            Color c = placed ? PALETTE[i % 15] : (Color){72, 72, 105, 255};

            snprintf(buf, sizeof(buf), "J%d", job.id);
            DrawText(buf, C0, y + 3, 14, c);

            Color dimC = placed ? WHITE : (Color){72, 72, 105, 255};
            snprintf(buf, sizeof(buf), "%d", job.duration);
            DrawText(buf, C1 + 6, y + 3, 14, dimC);

            snprintf(buf, sizeof(buf), "%d", job.deadline);
            DrawText(buf, C2 + 6, y + 3, 14, dimC);

            if (placed) {
                snprintf(buf, sizeof(buf), "%d", job.finish);
                DrawText(buf, C3 + 6, y + 3, 14, {100, 255, 160, 255});

                Color lc = job.lateness > 0 ? (Color){255, 100, 100, 255}
                                            : (Color){80, 220, 100, 255};
                snprintf(buf, sizeof(buf), "%d", job.lateness);
                DrawText(buf, C4 + 12, y + 3, 14, lc);
            }

            snprintf(buf, sizeof(buf), "%d", i + 1);
            DrawText(buf, C5 + 12, y + 3, 13, {130, 130, 160, 255});
        }

        // ── Gantt background ───────────────────────────────────────────────
        DrawText("Linha do Tempo — Gantt", GANTT_X, GANTT_Y - 22, 16, WHITE);
        DrawRectangle(GANTT_X, GANTT_Y, GANTT_W, GANTT_H, {20, 20, 50, 255});
        DrawRectangleLines(GANTT_X, GANTT_Y, GANTT_W, GANTT_H, {55, 55, 95, 255});

        // ── Gantt: placed job bars ─────────────────────────────────────────
        for (int i = 0; i < step; i++) {
            const auto& job = g_sched[i];
            int bx = GANTT_X + (int)(job.start  * scale);
            int bw = (int)(job.duration * scale);
            Color c = PALETTE[i % 15];

            DrawRectangle(bx, GANTT_Y, bw, GANTT_H, c);
            DrawRectangleLines(bx, GANTT_Y, bw, GANTT_H, {0, 0, 0, 160});

            snprintf(buf, sizeof(buf), "J%d", job.id);
            int tw = MeasureText(buf, 13);
            if (bw >= tw + 6)
                DrawText(buf, bx + (bw - tw) / 2, GANTT_Y + GANTT_H / 2 - 7,
                         13, {0, 0, 0, 220});
        }

        // ── Gantt: pulsing outline on current bar ──────────────────────────
        if (step > 0 && step <= g_n) {
            const auto& cur = g_sched[step - 1];
            int bx = GANTT_X + (int)(cur.start   * scale);
            int bw = (int)(cur.duration * scale);
            float p = (float)(sin(GetTime() * 5.0) * 0.5 + 0.5);
            DrawRectangleLines(bx, GANTT_Y, bw, GANTT_H,
                               {255, 255, 255, (unsigned char)(p * 200 + 55)});
        }

        // ── Timeline ticks ─────────────────────────────────────────────────
        {
            int ts = std::max(1, g_totalTime / 20);
            for (int t = 0; t <= g_totalTime; t += ts) {
                int x = GANTT_X + (int)(t * scale);
                DrawLine(x, GANTT_Y + GANTT_H, x, GANTT_Y + GANTT_H + 6,
                         {90, 90, 140, 255});
                snprintf(buf, sizeof(buf), "%d", t);
                DrawText(buf, x - 5, GANTT_Y + GANTT_H + 8, 12, {110, 110, 160, 255});
            }
        }

        // ── Deadline markers (for placed jobs) ─────────────────────────────
        for (int i = 0; i < step; i++) {
            const auto& job = g_sched[i];
            int dx  = GANTT_X + (int)(job.deadline * scale);
            Color dc = job.lateness > 0 ? (Color){255, 80,  80, 255}
                                        : (Color){ 80, 255, 120, 255};

            DrawLine(dx, GANTT_Y - 22, dx, GANTT_Y + GANTT_H, dc);
            DrawTriangle({(float)dx,     (float)(GANTT_Y - 6)},
                         {(float)(dx-5), (float)(GANTT_Y - 22)},
                         {(float)(dx+5), (float)(GANTT_Y - 22)}, dc);

            // Alternate label height so they don't pile up
            int labelY = GANTT_Y - 37 - (i % 3) * 13;
            snprintf(buf, sizeof(buf), "d%d=%d", job.id, job.deadline);
            DrawText(buf, dx - 8, labelY, 10, dc);
        }

        // ── Lateness brackets (below Gantt) ────────────────────────────────
        {
            int bracketBaseY = GANTT_Y + GANTT_H + 30;
            int row = 0;
            for (int i = 0; i < step; i++) {
                const auto& job = g_sched[i];
                if (job.lateness <= 0) continue;
                int x1 = GANTT_X + (int)(job.deadline * scale);
                int x2 = GANTT_X + (int)(job.finish   * scale);
                int by = bracketBaseY + (row % 3) * 20;
                row++;

                DrawLine(x1, by, x2, by, {255, 80, 80, 255});
                DrawLine(x1, by - 4, x1, by + 4, {255, 80, 80, 255});
                DrawLine(x2, by - 4, x2, by + 4, {255, 80, 80, 255});

                snprintf(buf, sizeof(buf), "l%d=%d", job.id, job.lateness);
                int tw = MeasureText(buf, 11);
                if (x2 - x1 > 6)
                    DrawText(buf, (x1 + x2) / 2 - tw / 2, by + 5, 11,
                             {255, 130, 130, 255});
            }
        }

        // ── Stats bar ──────────────────────────────────────────────────────
        int statsY = H - 115;
        DrawLine(0, statsY - 6, W, statsY - 6, {38, 38, 78, 255});

        if (step == 0) {
            DrawText("Tarefas ordenadas por deadline (EDF). Pressione SPACE para iniciar.",
                     GANTT_X, statsY, 15, {200, 200, 100, 255});
        } else {
            const auto& cur = g_sched[step - 1];
            snprintf(buf, sizeof(buf),
                     "J%d agendada: início=%d  duração=%d  término=%d  "
                     "deadline=%d  →  atraso=%d",
                     cur.id, cur.start, cur.duration, cur.finish,
                     cur.deadline, cur.lateness);
            DrawText(buf, GANTT_X, statsY, 15, {200, 220, 255, 255});
        }

        // Running max lateness
        int curMax = 0;
        for (int i = 0; i < step; i++)
            curMax = std::max(curMax, g_sched[i].lateness);

        snprintf(buf, sizeof(buf), "Atraso máximo atual  L = %d", curMax);
        Color lc = curMax > 0 ? (Color){255, 100, 100, 255}
                              : (Color){ 80, 220, 100, 255};
        DrawText(buf, GANTT_X, statsY + 28, 20, lc);

        if (step == g_n) {
            int lateCount = 0;
            for (auto& j : g_sched) if (j.lateness > 0) lateCount++;
            snprintf(buf, sizeof(buf),
                     "Concluído!  L* = %d (ótimo pelo EDF)   "
                     "%d/%d tarefas em atraso",
                     g_maxLateness, lateCount, g_n);
            Color fc = g_maxLateness > 0 ? (Color){255, 160, 50, 255}
                                         : (Color){ 80, 255, 150, 255};
            DrawText(buf, GANTT_X, statsY + 58, 17, fc);
        }

        // Left panel bottom — complexity
        DrawLine(TABLE_X, statsY - 6, TABLE_X + TABLE_W, statsY - 6,
                 {38, 38, 78, 255});
        DrawText("Complexidade:",            TABLE_X, statsY,      14, {160,160,200,255});
        DrawText("Tempo: O(n log n)",        TABLE_X, statsY + 18, 13, {100,220,100,255});
        DrawText("Espaço: O(n)",             TABLE_X, statsY + 34, 13, {100,220,100,255});
        snprintf(buf, sizeof(buf), "Passo %d / %d", step, g_n);
        DrawText(buf,                        TABLE_X, statsY + 58, 14, {160,160,200,255});

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
