#include <iostream>
#include <optional>
#include <vector>

#include <SFML/Graphics.hpp>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const int FPS_LIMIT = 30;
const std::vector<Point2D> pts={
    p1;
    p2;
    p3;
    p4;
};

using Point2D = sf::Vector2f;

// TODO: (Part 1) Define a function that samples a cubic Bezier curve at t in [0, 1].
float lerp(float a, float b, float t) {
    return (1 - t) * a + t * b;
}

// Cubic Bezier function
std::function<float(float, float, float)> tween = [](float a, float b, float t) {
    const float p1 = 0.f, p2 = -0.5f, p3 = 1.5f, p4 = 1.f;
    float u = 1 - t;
    float e = (u * u * u * p1) + (3 * u * u * t * p2) + (3 * u * t * t * p3) + (t * t * t * p4);
    return lerp(a, b, e);
};


Point2D getPoint(const std::vector<sf::Vector2f>& pts, float t) { 
    float u = 1 - t;
    float e = (u * u * u * pts.p1) + (3 * u * u * t * pts.p2) + (3 * u * t * t * pts.p3) + (t * t * t * pts.p4);

    return Point2D(e,t); 
}

// TODO: (Part 2) Define a function that returns the curve's slope at t in [0, 1].
Point2D getSlope(const std::vector<sf::Vector2f>& pts, float t) { return Point2D{}; }

// TODO: (Part 1) Store four control points for the curve.
// TODO: (Part 2) Track animation time for the square moving along the curve.
// TODO: (Part 3) Track the index of the control point being dragged.

void handleInput(sf::Window& window, bool& shouldQuit) {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
            shouldQuit = true;
        } else if (const auto* mouse = event->getIf<sf::Event::MouseButtonPressed>()) {
            // TODO: (Part 3) On left-click, select the closest control point
            // using mouse->position and start dragging it.
            //sf::Mouse::Button::Left:
        } else if (const auto* mouse = event->getIf<sf::Event::MouseButtonReleased>()) {
            // TODO: (Part 3) On left-button release, stop dragging.
            //sf::Mouse::Button::Left:
        } else if (const auto* mouse = event->getIf<sf::Event::MouseMoved>()) {
            // TODO: (Part 3) Move the selected control point to mouse->position.
            // TODO: (Part 4) Maintain matching slopes at shared endpoints.
            // When moving point 3, move point 5 without changing its distance
            // from point 4 (point numbers here start at 1).
        } else if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
            // TODO: (Part 4) '+' adds three control points; '-' removes three,
            // keeping at least four points.
        }
    }
}

void render(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);
    // ====== ====== ======
    // TODO: (Part 1) Sample GetPoint over t in [0, 1] and connect samples using the line-drawing
    // code from your project. Draw all four control points as circles after drawing the curve.
    // ====== ====== ======

    // ====== ====== ======
    // TODO: (Part 2) Draw a small square moving repeatedly along the curve.
    // Use GetSlope to orient it to the curve at each time step.
    // ====== ====== ======

    // ====== ====== ======
    // TODO: (Part 3) Draw control handles from point 1 to 2 and point 3 to 4.
    // TODO: (Part 4) Draw all connected cubic Bezier segments and their handles.
    // ====== ====== ======

    // ====== ====== ======
    // TODO: (Bonus) Support multiple curves, a Galaga screen overlay at a 1:2 ratio, and exporting
    // curve points as C++ code for Project 1b.
    // ====== ====== ======

    window.display();
}

void render(sf::RenderWindow& window) {

    window.clear(sf::Color::Black);

    float t = (frameCount % FRAMES_PER_ANIM) / static_cast<float>(FRAMES_PER_ANIM);
    frameCount++;

    //circle
    float x = tween(RADIUS, WINDOW_WIDTH - RADIUS, t);
    float y = WINDOW_HEIGHT / 3.f;

    sf::CircleShape circle(RADIUS);
    circle.setOrigin({RADIUS, RADIUS});
    circle.setPosition({x, y});
    circle.setFillColor(sf::Color::Yellow);
    window.draw(circle);

    const float graphRight = GRAPH_LEFT + GRAPH_WIDTH;
    const float graphTop = GRAPH_BOTTOM - GRAPH_HEIGHT;

    sf::Vertex axes[] = {
        {{GRAPH_LEFT, GRAPH_BOTTOM}}, {{graphRight, GRAPH_BOTTOM}},
        {{GRAPH_LEFT, GRAPH_BOTTOM}}, {{GRAPH_LEFT, graphTop}},
    };
    window.draw(axes, 4, sf::PrimitiveType::Lines);

    //Graph line
    sf::VertexArray curve(sf::PrimitiveType::LineStrip, GRAPH_SAMPLES);
    for (int i = 0; i < GRAPH_SAMPLES; i++) {
        float gx = i / static_cast<float>(GRAPH_SAMPLES - 1);
        curve[i].position = {lerp(GRAPH_LEFT, graphRight, gx), tween(GRAPH_BOTTOM, graphTop, gx)};
        curve[i].color = sf::Color::Yellow;
    }
    window.draw(curve);

    //Current point on the curve
    const float DOT_RADIUS = 5.f;
    sf::CircleShape dot(DOT_RADIUS);
    dot.setOrigin({DOT_RADIUS, DOT_RADIUS});
    dot.setPosition({lerp(GRAPH_LEFT, graphRight, t), tween(GRAPH_BOTTOM, graphTop, t)});
    dot.setFillColor(sf::Color::Yellow);
    window.draw(dot);

    window.display();
}

int main() {
    sf::RenderWindow window;

    try {
        // Initialize window
        window.create(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Bezier Curve Editor");
        window.setFramerateLimit(FPS_LIMIT);
        // Prevent key repeats.
        window.setKeyRepeatEnabled(false);

        bool shouldQuit = false;
        // Main game loop
        while (window.isOpen()) {
            handleInput(window, shouldQuit);
            if (shouldQuit) {
                break;
            }
            render(window);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}
