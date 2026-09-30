#include <iostream>
#include <optional>
#include <vector>

#include <SFML/Graphics.hpp>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const int FPS_LIMIT = 30;
const int GRAPH_SAMPLES =100;
const float RADIUS = 10.f;
const float SIDE = 10.f;
const int FRAMES_PER_ANIM = 60;
const float GRAPH_LEFT = 100.f;
const float GRAPH_BOTTOM = 750.f;
const float GRAPH_WIDTH = 600.f;
const float GRAPH_HEIGHT = 300.f;

using Point2D = sf::Vector2f;

int frameCount = 0;
int selected = -1;

std::vector<Point2D> points={ {100.f,600.f}, {250.f,250.f}, {400.f,400.f}, {600.f,300.f}};


// TODO: (Part 1) Define a function that samples a cubic Bezier curve at t in [0, 1].



Point2D getPoint(const std::vector<sf::Vector2f>& pts, float t) { 
    float u = 1 - t;
    Point2D e = (u * u * u * pts[0]) + (3 * u * u * t * pts[1]) + (3 * u * t * t * pts[2]) + (t * t * t * pts[3]);

    return e; 
}

// TODO: (Part 2) Define a function that returns the curve's slope at t in [0, 1].
Point2D getSlope(const std::vector<sf::Vector2f>& pts, float t) { 
    float u = 1 - t;
    Point2D slope = 3*u*u*(pts[1] -pts[0])  +  6*u*t*(pts[2] - pts[1])  +  3*t*t*(pts[3] - pts[2]);
    return slope; 
}

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
            if(mouse->button == sf::Mouse::Button::Left){
                //find closets point.
                Point2D mousePos = sf::Vector2f(mouse->position);
                int index = 0;
                float bstDist = 9999999;
                for(int i = 0; i < points.size();++i) {
                    float dist = (points[i]- mousePos).length();
                    if(dist < bstDist){
                        bstDist = dist;
                        index = i;
                    } 
                }
                selected = index;
                points[selected] = mousePos;
                
            }
        } else if (const auto* mouse = event->getIf<sf::Event::MouseButtonReleased>()) {
            // TODO: (Part 3) On left-button release, stop dragging.
            selected = -1;
        } else if (const auto* mouse = event->getIf<sf::Event::MouseMoved>()) {
            // TODO: (Part 3) Move the selected control point to mouse->position.
            // TODO: (Part 4) Maintain matching slopes at shared endpoints.
            // When moving point 3, move point 5 without changing its distance
            // from point 4 (point numbers here start at 1).
            if(selected != -1){
                Point2D mousePos = sf::Vector2f(mouse->position);
                points[selected] = mousePos;
                int m = selected;
                if ( selected % 3 == 2  &&  points.size()> m +2) {
                    if ((points[m+1] - points[m]).length() > 0){
                        Point2D direction = (points[m+1] - points[m]).normalized();
                        float distance = 1;
                        if(float distance = (points[m+1]-points[m+2]).length()>0){
                            distance = (points[m+1]-points[m+2]).length();
                        }
                        points[m+2] = points[m+1] + (direction*distance);
                    }

                }
            }
        } else if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
            // TODO: (Part 4) '+' adds three control points; '-' removes three,
            // keeping at least four points.
            //sf::Keyboard::Key::Equal
            if(key->code == sf::Keyboard::Key::Equal || key->code == sf::Keyboard::Key::Add){
                for(int i = 0; i < 3; ++i){
                    Point2D offset = points.back() + Point2D({20,0});
                    points.push_back( offset );
                }
            }
            else if(key->code == sf::Keyboard::Key::Hyphen || key->code == sf::Keyboard::Key::Subtract){
                for(int i = 0; i < 3; ++i){
                    if(points.size() > 4){
                        points.pop_back();
                    }
                }
            }
        }
    }
}

void render(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);
    // ====== ====== ======
    // TODO: (Part 1) Sample GetPoint over t in [0, 1] and connect samples using the line-drawing
    // code from your project. Draw all four control points as circles after drawing the curve.
    // ====== ====== ======
    //Graph line
    
    for(int s = 0; s + 3 < points.size(); s+=3){
        std::vector<Point2D> seg = {points[s], points[s+1], points[s+2], points[s+3]};
        sf::VertexArray curve(sf::PrimitiveType::LineStrip, GRAPH_SAMPLES);
        for (int i = 0; i < GRAPH_SAMPLES; i++) {
            float time = i / static_cast<float>(GRAPH_SAMPLES - 1);
            curve[i].position = {getPoint(seg,time)};
            curve[i].color = sf::Color::Yellow;
        }
        window.draw(curve);
        sf::Vertex handles[] = {
        {seg[0]}, {seg[1]},   
        {seg[2]}, {seg[3]},       
        };
        window.draw(handles, 4, sf::PrimitiveType::Lines);
    }
    

    for (int i = 0; i < points.size(); i++) {
        // make a circle
        sf::CircleShape circle(RADIUS);
        circle.setOrigin({RADIUS, RADIUS});
        circle.setPosition(points[i]);
        circle.setFillColor(sf::Color::Red);
        window.draw(circle);

    }
    // ====== ====== ======
    // TODO: (Part 2) Draw a small square moving repeatedly along the curve.
    // Use GetSlope to orient it to the curve at each time step.
    // ====== ====== ======

    // make a square
    float t = (frameCount % FRAMES_PER_ANIM) / static_cast<float>(FRAMES_PER_ANIM);
    frameCount++;
    sf::RectangleShape square({SIDE, SIDE});
    square.setOrigin({SIDE/2, SIDE/2});
    square.setPosition(getPoint(points, t));
    square.setRotation(getSlope(points, t).angle());
    square.setFillColor(sf::Color::Green);
    window.draw(square);

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

// void render(sf::RenderWindow& window) {

//     window.clear(sf::Color::Black);

//     float t = (frameCount % FRAMES_PER_ANIM) / static_cast<float>(FRAMES_PER_ANIM);
//     frameCount++;

//     //circle
//     float x = tween(RADIUS, WINDOW_WIDTH - RADIUS, t);
//     float y = WINDOW_HEIGHT / 3.f;

//     sf::CircleShape circle(RADIUS);
//     circle.setOrigin({RADIUS, RADIUS});
//     circle.setPosition({x, y});
//     circle.setFillColor(sf::Color::Yellow);
//     window.draw(circle);

//     const float graphRight = GRAPH_LEFT + GRAPH_WIDTH;
//     const float graphTop = GRAPH_BOTTOM - GRAPH_HEIGHT;

//     sf::Vertex axes[] = {
//         {{GRAPH_LEFT, GRAPH_BOTTOM}}, {{graphRight, GRAPH_BOTTOM}},
//         {{GRAPH_LEFT, GRAPH_BOTTOM}}, {{GRAPH_LEFT, graphTop}},
//     };
//     window.draw(axes, 4, sf::PrimitiveType::Lines);

//     //Graph line
//     sf::VertexArray curve(sf::PrimitiveType::LineStrip, GRAPH_SAMPLES);
//     for (int i = 0; i < GRAPH_SAMPLES; i++) {
//         float gx = i / static_cast<float>(GRAPH_SAMPLES - 1);
//         curve[i].position = {lerp(GRAPH_LEFT, graphRight, gx), tween(GRAPH_BOTTOM, graphTop, gx)};
//         curve[i].color = sf::Color::Yellow;
//     }
//     window.draw(curve);

//     //Current point on the curve
//     const float DOT_RADIUS = 5.f;
//     sf::CircleShape dot(DOT_RADIUS);
//     dot.setOrigin({DOT_RADIUS, DOT_RADIUS});
//     dot.setPosition({lerp(GRAPH_LEFT, graphRight, t), tween(GRAPH_BOTTOM, graphTop, t)});
//     dot.setFillColor(sf::Color::Yellow);
//     window.draw(dot);

//     window.display();
// }

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
