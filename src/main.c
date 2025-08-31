/**
 * \file main.c
 * \brief Simulador 3D do Efeito Doppler usando Raylib.
 *
 * Permite variar ângulo, velocidade, distância e alternar entre fonte móvel
 * e receptor móvel. Exibe a frequência observada calculada em tempo real.
 */
#include "raylib.h"
#include "raymath.h"
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Physical constants (air at 20C)
static const float SPEED_OF_SOUND = 343.0f; // m/s

// Utility
static float deg2rad(float d) { return d * (float)M_PI / 180.0f; }

/** \brief Estado e parâmetros do simulador de Doppler. */
typedef struct {
    // Parameters
    float baseFreq;     // Hz
    float distance;     // m (emitter->receiver)
    float angleDeg;     // degrees between velocity vector and line-of-sight
    float speed;        // m/s magnitude of moving entity
    bool  movingSource; // true: source moves, false: receiver moves

    // Visual toggles
    bool showHelp;
    bool showScreenshots;
} DopplerState;

/**
 * \brief Desenha uma grade no plano XZ.
 * \param slices Número de divisões.
 * \param spacing Espaçamento entre linhas.
 */
static void DrawGridXZ(int slices, float spacing) {
    DrawGrid(slices, spacing);
}

/**
 * \brief Desenha uma seta 3D entre dois pontos.
 * \param start Ponto inicial.
 * \param end Ponto final.
 * \param thickness Espessura (para o cone da ponta).
 * \param color Cor da seta.
 */
static void DrawArrow3D(Vector3 start, Vector3 end, float thickness, Color color) {
    DrawLine3D(start, end, color);
    Vector3 dir = Vector3Subtract(end, start);
    float len = Vector3Length(dir);
    if (len < 1e-4f) return;
    Vector3 ndir = Vector3Scale(dir, 1.0f/len);
    float headLen = fminf(0.25f*len, 0.5f);
    float headRad = thickness*2.0f;
    Vector3 base = Vector3Add(end, Vector3Scale(ndir, -headLen));
    DrawCylinderEx(base, end, headRad*0.5f, 0.0f, 12, color);
}

/**
 * \brief Calcula a frequência observada pelo efeito Doppler e a velocidade radial.
 * \param st Estado do simulador com parâmetros físicos.
 * \param fObserved Saída: frequência observada (Hz).
 * \param radialSpeed Saída: componente radial da velocidade (m/s).
 */
static void ComputeDoppler(const DopplerState* st, float* fObserved, float* radialSpeed) {
    // Line-of-sight along +X from source->receiver by default
    // angleDeg is between velocity vector and LOS
    float vr = st->speed * cosf(deg2rad(st->angleDeg)); // signed if we define direction

    // Positive vr when approaching (reduces denominator for moving source; increases numerator for moving receiver)
    // We'll consider vr positive if closing distance (towards each other). Use sign by checking configuration.
    // We define the velocity vector along +X when angle=0, pointing from source to receiver.

    // For visualization we'll place source at origin and receiver at +distance on X.
    // If movingSource: source moves towards +X for angle=0. Closing => vr > 0.

    if (radialSpeed) *radialSpeed = vr;

    float f0 = st->baseFreq;
    float c = SPEED_OF_SOUND;

    if (st->movingSource) {
        // Moving source, stationary receiver: f' = f0 * c / (c - v_s,rad)
        float denom = (c - vr);
        if (fabsf(denom) < 1e-3f) denom = (denom >= 0 ? 1e-3f : -1e-3f);
        *fObserved = f0 * c / denom;
    } else {
        // Stationary source, moving receiver: f' = f0 * (c + v_r)/c
        *fObserved = f0 * (c + vr) / c;
    }
}

/**
 * \brief Desenha o HUD 2D com parâmetros e resultados.
 * \param st Estado do simulador.
 * \param fObs Frequência observada.
 * \param vr Velocidade radial.
 * \param screenW Largura da tela.
 * \param screenH Altura da tela.
 * \param shots Texturas de screenshots (opcional).
 * \param shotsCount Quantidade de screenshots válidas.
 */
static void DrawHUD2D(const DopplerState* st, float fObs, float vr, int screenW, int screenH, Texture2D* shots, int shotsCount) {
    const int pad = 12;
    const int line = 22;
    int y = pad;

    DrawRectangle(pad-6, pad-6, 420, 180, Fade(BLACK, 0.4f));
    DrawText("Doppler Shift Simulator", pad, y, 22, RAYWHITE); y += line + 6;
    DrawText(TextFormat("Base freq: %.1f Hz", st->baseFreq), pad, y, 18, RAYWHITE); y += line;
    DrawText(TextFormat("Distance: %.1f m", st->distance), pad, y, 18, RAYWHITE); y += line;
    DrawText(TextFormat("Angle: %.1f deg", st->angleDeg), pad, y, 18, RAYWHITE); y += line;
    DrawText(TextFormat("Speed: %.1f m/s (radial: %.1f)", st->speed, vr), pad, y, 18, RAYWHITE); y += line;
    DrawText(TextFormat("Observed freq: %.2f Hz", fObs), pad, y, 18, YELLOW); y += line;
    DrawText(TextFormat("Mode: %s", st->movingSource?"Moving SOURCE":"Moving RECEIVER"), pad, y, 18, SKYBLUE); y += line;

    y += 8;
    DrawText("Controls:", pad, y, 18, LIGHTGRAY); y += line;
    DrawText("Left/Right: angle  |  Up/Down: speed  |  PgUp/PgDn: distance", pad, y, 16, LIGHTGRAY); y += line;
    DrawText("[/]: freq  |  S: toggle source/receiver  |  H: help", pad, y, 16, LIGHTGRAY);

    if (st->showScreenshots && shots && shotsCount > 0) {
        int w = 320;
        int x = screenW - w - pad;
        int y2 = pad;
        for (int i = 0; i < shotsCount; ++i) {
            Texture2D tex = shots[i];
            if (tex.id == 0) continue;
            float scale = (float)w / tex.width;
            int h = (int)(tex.height * scale);
            DrawTexturePro(tex,
                           (Rectangle){0,0,(float)tex.width,(float)tex.height},
                           (Rectangle){x,y2,(float)w,(float)h},
                           (Vector2){0,0}, 0.0f, WHITE);
            y2 += h + 8;
        }
    }

    if (st->showHelp) {
        const char* help =
            "Efeito Doppler:\n"
            "- Mudança na frequência observada devido ao movimento relativo.\n"
            "- v_radial = v * cos(angulo).\n"
            "- Fonte em movimento: f' = f0 * c / (c - v_radial).\n"
            "- Receptor em movimento: f' = f0 * (c + v_radial) / c.";
        int boxW = 520;
        DrawRectangle(screenW - boxW - pad, screenH - 160 - pad, boxW, 160, Fade(DARKBLUE, 0.7f));
        DrawText(help, screenW - boxW - pad + 10, screenH - 150 - pad, 18, RAYWHITE);
    }
}

/** \brief Ponto de entrada. Inicializa janela, câmera e laço principal. */
int main(void) {
    // Window and 3D camera
    const int screenWidth = 1280;
    const int screenHeight = 720;
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Doppler Shift - 3D Simulator");

    Camera3D cam = {0};
    cam.position = (Vector3){ 6.0f, 5.0f, 6.0f };
    cam.target   = (Vector3){ 0.0f, 0.8f, 0.0f };
    cam.up       = (Vector3){ 0.0f, 1.0f, 0.0f };
    cam.fovy     = 60.0f;
    cam.projection = CAMERA_PERSPECTIVE;

    DopplerState st = {0};
    st.baseFreq = 1000.0f;  // 1 kHz
    st.distance = 5.0f;     // meters
    st.angleDeg = 0.0f;     // deg
    st.speed    = 20.0f;    // m/s
    st.movingSource = true;
    st.showHelp = true;
    st.showScreenshots = false;

    Texture2D shots[3] = {0};
    int shotsCount = 0;
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // Resize aware
        int sw = GetScreenWidth();
        int sh = GetScreenHeight();

        // Controls
        float dt = GetFrameTime();
        if (IsKeyDown(KEY_RIGHT)) st.angleDeg += 60.0f * dt;
        if (IsKeyDown(KEY_LEFT))  st.angleDeg -= 60.0f * dt;
        if (IsKeyDown(KEY_UP))    st.speed    += 20.0f * dt;
        if (IsKeyDown(KEY_DOWN))  st.speed    -= 20.0f * dt;
        if (IsKeyPressed(KEY_S))  st.movingSource = !st.movingSource;
        if (IsKeyPressed(KEY_H))  st.showHelp = !st.showHelp;
        if (IsKeyDown(KEY_LEFT_BRACKET))  st.baseFreq -= 50.0f * dt;
        if (IsKeyDown(KEY_RIGHT_BRACKET)) st.baseFreq += 50.0f * dt;
        if (IsKeyDown(KEY_PAGE_UP))   st.distance += 5.0f * dt;
        if (IsKeyDown(KEY_PAGE_DOWN)) st.distance -= 5.0f * dt;

        if (st.speed < 0.0f) st.speed = 0.0f;
        if (st.baseFreq < 1.0f) st.baseFreq = 1.0f;
        if (st.distance < 0.5f) st.distance = 0.5f;

        // Optional simple mouse camera orbit
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            Vector2 d = GetMouseDelta();
            static float yaw = 0, pitch = 0;
            yaw   += d.x * 0.003f;
            pitch += d.y * 0.003f;
            if (pitch > 1.2f) pitch = 1.2f;
            if (pitch < -0.2f) pitch = -0.2f;
            float r = 8.0f;
            cam.position.x = r * cosf(pitch) * cosf(yaw);
            cam.position.y = r * sinf(pitch) + 3.0f;
            cam.position.z = r * cosf(pitch) * sinf(yaw);
        }
        float mw = GetMouseWheelMove();
        if (fabsf(mw) > 0.01f) cam.fovy = Clamp(cam.fovy - mw*2.0f, 20.0f, 90.0f);

        // Physics & geometry
        // Place source at origin, receiver on +X at distance
        Vector3 posSrc = (Vector3){0, 0.8f, 0};
        Vector3 posRcv = (Vector3){st.distance, 0.8f, 0};

        // Velocity vector direction: angleDeg around Y relative to +X
        float a = deg2rad(st.angleDeg);
        Vector3 vdir = (Vector3){ cosf(a), 0, sinf(a) };
        float vr; float fObs;
        ComputeDoppler(&st, &fObs, &vr);

        // 3D draw
        BeginDrawing();
        ClearBackground((Color){20, 24, 28, 255});

        BeginMode3D(cam);
        DrawGridXZ(20, 1.0f);
        // LOS
        DrawLine3D(posSrc, posRcv, Fade(WHITE, 0.35f));

        // Source and Receiver
        DrawSphere(posSrc, 0.25f, st.movingSource?ORANGE:GRAY);
        DrawSphere(posRcv, 0.25f, st.movingSource?GRAY:SKYBLUE);

        // Velocity arrow on moving entity
        Vector3 from = st.movingSource ? posSrc : posRcv;
        Vector3 to   = Vector3Add(from, Vector3Scale(vdir, 1.0f + 0.01f*st.speed));
        DrawArrow3D(from, to, 0.05f, st.movingSource?ORANGE:SKYBLUE);

        // Indicate angle between v and LOS at source position
        // Small arc on ground plane
        Vector3 p0 = posSrc;
        float rArc = 0.8f;
        int segs = 24;
        Vector3 prev = (Vector3){ p0.x + rArc, p0.y, p0.z };
        for (int i = 1; i <= segs; ++i) {
            float t = (float)i / segs * a;
            Vector3 cur = (Vector3){ p0.x + rArc*cosf(t), p0.y, p0.z + rArc*sinf(t) };
            DrawLine3D(prev, cur, Fade(YELLOW, 0.5f));
            prev = cur;
        }

        // Simple wavefront rings around source (visual flair)
        for (int i = 1; i <= 6; ++i) {
            float rr = 0.4f * i;
            Color cc = Fade(st.movingSource?ORANGE:SKYBLUE, 0.25f);
            // Draw circle in XZ plane
            for (int k = 0; k < 36; ++k) {
                float t0 = (k    )*(2*M_PI/36.0f);
                float t1 = (k + 1)*(2*M_PI/36.0f);
                Vector3 c0 = {posSrc.x + rr*cosf(t0), posSrc.y, posSrc.z + rr*sinf(t0)};
                Vector3 c1 = {posSrc.x + rr*cosf(t1), posSrc.y, posSrc.z + rr*sinf(t1)};
                DrawLine3D(c0, c1, cc);
            }
        }

        EndMode3D();

        // HUD
        DrawHUD2D(&st, fObs, vr, sw, sh, shots, shotsCount);

        EndDrawing();
    }

    // Cleanup
    for (int i = 0; i < shotsCount; ++i) {
        if (shots[i].id != 0) UnloadTexture(shots[i]);
    }
    CloseWindow();
    return 0;
}
