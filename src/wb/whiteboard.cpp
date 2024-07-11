#include <fstream>
#include <sstream>
#include <utility>
#include <vector>
#include <wb/whiteboard.h>

#include <ostream>
#include <raylib.h>
#include <wb/drawing.h>
#include <wb/geometry.h>
#include <wb/rendering.h>
#include <wb/serialization.h>
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
    std::vector<Drawing> drawings;
    geometry::Vec2 mousePosition;
    std::string filepath;

    Whiteboard()
        : view(geometry::Vec2{0, 0},
               geometry::Vec2{DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT}),
          mode(Mode::Move) {}
};

void saveWhiteboard(Whiteboard &w) {
    std::ofstream file(w.filepath);
    if (file) {
        serialization::serialize(file, w.drawings);
    }
}

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

class WhiteboardMode {
  public:
    virtual ~WhiteboardMode() {};
    virtual void handleInput(Whiteboard &) = 0;
    virtual void cleanup(Whiteboard &) = 0;
};

class WhiteboardMoveMode : public WhiteboardMode {
  public:
    virtual void handleInput(Whiteboard &w) {
        DEBUG_ONLY(static bool changed = false);
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            geometry::Vec2 mousePosition =
                w.view.getViewPosition(getMousePos());
            geometry::Vec2 lastPosition =
                w.view.getViewPosition(w.mousePosition);
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
            });
        }
    }
    void cleanup(Whiteboard &w) {}
};

class WhiteboardDrawMode : public WhiteboardMode {
  public:
    virtual void handleInput(Whiteboard &w) {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            geometry::Vec2 mousePosition =
                w.view.getViewPosition(getMousePos());
            if (mousePosition == w.mousePosition &&
                !w.currentDrawing.getPoints().empty()) {
                return;
            }
            w.currentDrawing.addPoint(mousePosition);
        } else {
            if (w.currentDrawing.getPoints().empty()) {
                return;
            }
            w.drawings.push_back(Drawing::finalizeDrawing(w.currentDrawing));
            w.currentDrawing.clear();
        }
    }
    void cleanup(Whiteboard &w) {
        if (!w.currentDrawing.getPoints().empty()) {
            w.drawings.push_back(Drawing::finalizeDrawing(w.currentDrawing));
            w.currentDrawing.clear();
        }
    }
};

class WhiteboardEraseMode : public WhiteboardMode {
  public:
    virtual void handleInput(Whiteboard &w) {
        if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            return;
        }
        geometry::Vec2 mousePosition = getMousePos();
        if (mousePosition == w.mousePosition) {
            return;
        }
        geometry::LineSegment mouseMovement{
            .start = w.view.getViewPosition(w.mousePosition),
            .end = w.view.getViewPosition(mousePosition),
        };
        for (std::size_t i = 0; i < w.drawings.size(); i++) {
            Drawing &drawing = w.drawings[i];
            if (drawing.intersects(mouseMovement)) {
                std::swap(w.drawings[i], w.drawings.back());
                w.drawings.pop_back();
                i--;
                break;
            }
        }
    }
    void cleanup(Whiteboard &w) {}
};

class BaseWhiteboardMode : public WhiteboardMode {
  public:
    BaseWhiteboardMode() {
		drawMode = new WhiteboardDrawMode();
		eraseMode = new WhiteboardEraseMode();
		moveMode = new WhiteboardMoveMode();
        m_currentMode = moveMode;
    }

    void handleInput(Whiteboard &w) override {
        m_currentMode->handleInput(w);
        if (IsWindowResized()) {
            w.view.setSize(geometry::Vec2{GetScreenWidth(), GetScreenHeight()});
            PRINT_DBG(w.view.getSize());
        }
        if (IsKeyPressed(KEY_E)) {
            w.mode = Whiteboard::Mode::Erase;
            if (m_currentMode != eraseMode) {
                m_currentMode->cleanup(w);
            }
            m_currentMode = eraseMode;
        }
        if (IsKeyPressed(KEY_D)) {
            w.mode = Whiteboard::Mode::Draw;
            if (m_currentMode != drawMode) {
                m_currentMode->cleanup(w);
            }
            m_currentMode = drawMode;
        }
        if (IsKeyPressed(KEY_W)) {
            w.mode = Whiteboard::Mode::Move;
            if (m_currentMode != moveMode) {
                m_currentMode->cleanup(w);
            }
            m_currentMode = moveMode;
        }
        w.mousePosition = getMousePos();

        if (IsKeyPressed(KEY_ZERO)) {
            w.view.resetZoom();
        }

        float wheelMove = GetMouseWheelMove();
        w.view.zoom(w.mousePosition, wheelMove);

        if (IsKeyPressed(KEY_S)) {
            saveWhiteboard(w);
        }
    }

    void cleanup(Whiteboard &w) override {}

  private:
    WhiteboardMode *drawMode;
    WhiteboardMode *eraseMode;
    WhiteboardMode *moveMode;
    WhiteboardMode *m_currentMode;
};

void renderWhiteboard(Whiteboard &w) {
    rendering::render(w.view, w.currentDrawing);
    for (Drawing &drawing : w.drawings) {
        rendering::render(w.view, drawing);
    }
    std::stringstream ss;
    switch (w.mode) {
    case Whiteboard::Mode::Draw:
        ss << "DRAW ";
        break;
    case Whiteboard::Mode::Erase:
        ss << "ERASE ";
        break;
    case Whiteboard::Mode::Move:
        ss << "MOVE ";
        break;
    }
    ss << "X: " << w.view.getPosition().x << " Y: " << w.view.getPosition().y
       << " | " << w.view.getZoom() << "%" << " | "
       << GetFPS() << " FPS";
    rendering::renderStatusBar(w.view, ss.str());
}

void runWhiteboard() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(200);
    Whiteboard whiteboard;
    InitWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, "Whiteboard");
    whiteboard.mousePosition = getMousePos();
    BaseWhiteboardMode baseMode;
    while (!WindowShouldClose()) {
        baseMode.handleInput(whiteboard);
        BeginDrawing();
        ClearBackground(WHITE);
        renderWhiteboard(whiteboard);
        EndDrawing();
    }
    CloseWindow();
}

void runWhiteboard(std::string filepath) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(200);
    Whiteboard whiteboard;
    whiteboard.filepath = filepath;
    std::ifstream file;
    file.open(filepath);
    if (file) {
        serialization::deserialize(file, whiteboard.drawings);
    }
    InitWindow(DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, "Whiteboard");
    whiteboard.mousePosition = getMousePos();
    BaseWhiteboardMode baseMode;
    while (!WindowShouldClose()) {
        baseMode.handleInput(whiteboard);
        PollInputEvents();
        BeginDrawing();
        ClearBackground(WHITE);
        renderWhiteboard(whiteboard);
        EndDrawing();
    }
    CloseWindow();
}

}; // namespace wb
