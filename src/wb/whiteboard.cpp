#include <wb/whiteboard.h>

#include <ostream>
#include <raylib.h>
#include <wb/drawing.h>
#include <wb/geometry.h>
#include <wb/rendering.h>
#include <wb/utils.h>
#include <wb/view.h>

namespace wb {

static const int DEFAULT_SCREEN_WIDTH = 800;
static const int DEFAULT_SCREEN_HEIGHT = 600;

geometry::Vec2 getMousePos() {
    return geometry::Vec2{GetMouseX(), GetMouseY()};
}

struct Whiteboard {
    enum class Mode {
        Move,
        Draw,
        Erase,
    };
    View view;
    Mode mode;
    PartialDrawing currentDrawing;
    geometry::Vec2 mousePosition;

    Whiteboard()
        : view(geometry::Vec2{0, 0},
               geometry::Vec2{DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT}),
          mode(Mode::Move) {}
};

std::ostream &operator<<(std::ostream &out, Whiteboard::Mode mode) {
    switch (mode) {
    case Whiteboard::Mode::Move:
        return out << "Whiteboard::Mode::Move";
    case Whiteboard::Mode::Draw:
        return out << "Whiteboard::Mode::Draw";
    case Whiteboard::Mode::Erase:
        return out << "Whiteboard::Mode::Erase";
    }
    return out << "Whiteboard::Mode::Invalid";
}

void processInputMove(Whiteboard &w) {
    DEBUG_ONLY(static bool changed = false);
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        geometry::Vec2 mousePosition = w.view.getViewPosition(getMousePos());
        geometry::Vec2 lastPosition = w.view.getViewPosition(w.mousePosition);
        if (mousePosition == lastPosition) {
            return;
        }
        DEBUG_ONLY(changed = true);
        w.view.translate(mousePosition - lastPosition);
    } else {
        DEBUG_ONLY({
            if (changed) {
                changed = false;
                PRINT_DBG(w.view.getPosition());
            }
        })
    }
}

void processInputDraw(Whiteboard &w) {
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        geometry::Vec2 mousePosition = w.view.getViewPosition(getMousePos());
        w.currentDrawing.addPoint(mousePosition);
    }
}

void processInputErase(Whiteboard &w) {}

void processInput(Whiteboard &w) {
    switch (w.mode) {
    case Whiteboard::Mode::Move:
        processInputMove(w);
        break;
    case Whiteboard::Mode::Draw:
        processInputDraw(w);
        break;
    case Whiteboard::Mode::Erase:
        processInputErase(w);
        break;
    }
    auto previousMode = w.mode;
    if (IsWindowResized()) {
        w.view.setSize(geometry::Vec2{GetScreenWidth(), GetScreenHeight()});
    }
    if (IsKeyPressed(KEY_E)) {
        w.mode = Whiteboard::Mode::Erase;
    }
    if (IsKeyPressed(KEY_D)) {
        w.mode = Whiteboard::Mode::Draw;
    }
    if (IsKeyPressed(KEY_W)) {
        w.mode = Whiteboard::Mode::Move;
    }
    if (w.mode != previousMode) {
        PRINT_DBG(w.mode);
    }
    w.mousePosition = getMousePos();
}

void renderWhiteboard(Whiteboard &w) {
    rendering::render(w.view, w.currentDrawing);
}

void runWhiteboard() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(200);
    Whiteboard whiteboard;
    InitWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, "Whiteboard");
    whiteboard.mousePosition = getMousePos();
    while (!WindowShouldClose()) {
        processInput(whiteboard);
        PollInputEvents();
        BeginDrawing();
        ClearBackground(WHITE);
        renderWhiteboard(whiteboard);
        EndDrawing();
    }
    CloseWindow();
}

}; // namespace wb
