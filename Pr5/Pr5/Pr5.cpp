#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <optional>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>

// Стани симуляції
enum class SimulationStatus
{
    Running,
    Paused,
    Finished
};

struct SimulationState
{
    float positionX = 0.f;
    float velocityX = 0.f;
    float elapsedTime = 0.f;

    float brakingStartX = 0.f;
    float brakingDistance = 0.f;
    bool isMeasuringBraking = false;

    float trajectoryTimer = 0.f;
    SimulationStatus status = SimulationStatus::Paused;
    std::vector<sf::Vector2f> trajectory;
};

const char* getStatusName(SimulationStatus status)
{
    switch (status)
    {
    case SimulationStatus::Running:
        return "RUNNING";
    case SimulationStatus::Paused:
        return "PAUSED";
    case SimulationStatus::Finished:
        return "FINISHED";
    }

    return "UNKNOWN";
}

void handleInput(
    sf::RenderWindow& window,
    SimulationState& state,
    float& tractionForce,
    float& brakingForce,
    float startX)
{
    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            window.close();
            continue;
        }

        if (!event->is<sf::Event::KeyPressed>())
        {
            continue;
        }

        const auto* key = event->getIf<sf::Event::KeyPressed>();
        if (key == nullptr)
        {
            continue;
        }

        // Пробіл скидає стан моделі, але зберігає налаштовані сили.
        if (key->code == sf::Keyboard::Key::Space)
        {
            state = SimulationState{};
            state.positionX = startX;
            state.brakingStartX = startX;
            continue;
        }

        // Enter запускає рух, ставить на паузу або продовжує його.
        if (key->code == sf::Keyboard::Key::Enter)
        {
            if (state.status == SimulationStatus::Running)
            {
                state.status = SimulationStatus::Paused;
            }
            else if (state.status == SimulationStatus::Paused)
            {
                state.status = SimulationStatus::Running;
            }
            continue;
        }
        
        if (state.status != SimulationStatus::Finished)
        {
            if (key->code == sf::Keyboard::Key::Up)
            {
                tractionForce += 5.f;
            }
            else if (key->code == sf::Keyboard::Key::Down)
            {
                tractionForce -= 5.f;
            }
            else if (key->code == sf::Keyboard::Key::Right)
            {
                brakingForce += 5.f;
            }
            else if (key->code == sf::Keyboard::Key::Left)
            {
                brakingForce -= 5.f;
            }
        }
    }

    tractionForce = std::clamp(tractionForce, 0.f, 100.f);
    brakingForce = std::clamp(brakingForce, 0.f, 100.f);
}

// Чисельне оновлення фізичної моделі.
// Спрощена модель: a = сила тяги - сила гальмування.
void updatePhysics(
    float dt,
    SimulationState& state,
    float tractionForce,
    float brakingForce,
    float startX,
    float carWidth,
    float carHeight,
    float carY,
    float finishX)
{
    if (state.status != SimulationStatus::Running || dt <= 0.f)
    {
        return;
    }

    // Обмежуємо надто великий часовий крок.
    dt = std::min(dt, 0.05f);
    state.elapsedTime += dt;

    const float oldPosition = state.positionX;
    const float oldVelocity = state.velocityX;

    // Прискорення визначається різницею тяги та гальмування.
    float acceleration = tractionForce - brakingForce;

    // Якщо машина стоїть, від'ємне прискорення не змушує її рухатися назад.
    if (oldVelocity <= 0.f && acceleration < 0.f)
    {
        acceleration = 0.f;
    }

    // Якщо гальмування припинилося до повної зупинки, зберігаємо вже пройдений шлях.
    if (state.isMeasuringBraking && acceleration >= 0.f)
    {
        state.brakingDistance = oldPosition - state.brakingStartX;
        state.isMeasuringBraking = false;
    }

    // Починаємо вимірювати гальмівний шлях, коли прискорення від'ємне,
    // а автомобіль ще рухається вперед.
    if (!state.isMeasuringBraking && acceleration < 0.f && oldVelocity > 0.f)
    {
        state.brakingStartX = oldPosition;
        state.brakingDistance = 0.f;
        state.isMeasuringBraking = true;
    }

    const float newVelocity = oldVelocity + acceleration * dt;

    // Якщо автомобіль зупиниться всередині поточного кроку, обчислюємо
    // положення в момент зупинки. Швидкість не переходить у від'ємні значення.
    if (oldVelocity > 0.f && acceleration < 0.f && newVelocity <= 0.f)
    {
        const float stopTime = oldVelocity / (-acceleration);
        state.positionX = oldPosition
            + oldVelocity * stopTime
            + 0.5f * acceleration * stopTime * stopTime;
        state.velocityX = 0.f;

        if (state.isMeasuringBraking)
        {
            state.brakingDistance = state.positionX - state.brakingStartX;
            state.isMeasuringBraking = false;
        }
    }
    else
    {
        // Рівняння руху за сталого прискорення протягом dt:
        // v(t + dt) = v(t) + a * dt
        // x(t + dt) = x(t) + v(t) * dt + a * dt^2 / 2
        state.velocityX = std::max(0.f, newVelocity);
        state.positionX = oldPosition
            + oldVelocity * dt
            + 0.5f * acceleration * dt * dt;

        // Захист від руху за стартову позицію через похибки обчислень.
        state.positionX = std::max(startX, state.positionX);
    }

    // Коли передня частина автомобіля доходить до фінішу, симуляція зупиняється.
    if (state.positionX >= finishX)
    {
        state.positionX = finishX;
        state.velocityX = 0.f;
        state.status = SimulationStatus::Finished;
        state.isMeasuringBraking = false;
    }

    state.trajectoryTimer += dt;
    if (state.positionX != oldPosition && state.trajectoryTimer >= 0.04f)
    {
        state.trajectory.push_back(
            { state.positionX + carWidth / 2.f, carY + carHeight / 2.f });
        state.trajectoryTimer = 0.f;

        if (state.trajectory.size() > 400)
        {
            state.trajectory.erase(state.trajectory.begin());
        }
    }
}

std::string makeInfoString(
    const SimulationState& state,
    float tractionForce,
    float brakingForce,
    float startX)
{
    std::ostringstream info;
    info << std::fixed << std::setprecision(1);

    info << "CAR SIMULATION\n"
        << "Status: " << getStatusName(state.status) << "\n"
        << "Position X: " << state.positionX << " px\n"
        << "Travelled distance: " << state.positionX - startX << " px\n"
        << "Velocity: " << state.velocityX << " px/s\n"
        << "Traction: " << tractionForce << "\n"
        << "Braking: " << brakingForce << "\n"
        << "Elapsed time: " << state.elapsedTime << " s\n"
        << "Braking distance: " << state.brakingDistance << " px\n\n"
        << "UP/DOWN: traction +/-5\n"
        << "RIGHT/LEFT: braking +/-5\n"
        << "ENTER: start / pause / resume\n"
        << "SPACE: reset to start and pause";

    if (state.status == SimulationStatus::Finished)
    {
        info << "\nFINISH! Time: " << state.elapsedTime << " s";
    }

    return info.str();
}

void drawScene(
    sf::RenderWindow& window,
    const sf::RectangleShape& car,
    const sf::RectangleShape& road,
    const sf::RectangleShape& finishLine,
    const SimulationState& state,
    const sf::Text& infoText)
{
    window.clear(sf::Color(35, 35, 45));

    sf::VertexArray trajectoryLine(sf::PrimitiveType::LineStrip);
    for (const auto& point : state.trajectory)
    {
        trajectoryLine.append(sf::Vertex{ point, sf::Color(180, 180, 180, 150) });
    }

    window.draw(road);
    window.draw(trajectoryLine);
    window.draw(finishLine);
    window.draw(car);
    window.draw(infoText);
    window.display();
}

int main()
{
    sf::RenderWindow window(
        sf::VideoMode({ 1000u, 600u }),
        "Car Simulation");

    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);

    sf::Font font;
    if (!font.openFromFile("../../../resources/fonts/ARIAL.TTF"))
    {
        std::cerr << "Cannot load font.\n";
        return 1;
    }

    sf::Text infoText(font, "", 18);
    infoText.setFillColor(sf::Color::White);
    infoText.setPosition({ 20.f, 20.f });

    float carWidth = 80.f;
    float carHeight = 45.f;
    float startX = 50.f;
    float carY = 455.f;
    float finishX = 1000.f - carWidth;

    sf::RectangleShape car({ carWidth, carHeight });
    car.setFillColor(sf::Color::Cyan);
    car.setPosition({ startX, carY });

    sf::RectangleShape road({ 1000.f, 50.f });
    road.setFillColor(sf::Color(20, 20, 25));
    road.setPosition({ 0.f, 510.f });

    sf::RectangleShape finishLine({ 4.f, 110.f });
    finishLine.setFillColor(sf::Color::Red);
    finishLine.setPosition({ 1000.f - 4.f, 410.f });

    SimulationState state;
    state.positionX = startX;
    state.brakingStartX = startX;

    float tractionForce = 20.f;
    float brakingForce = 5.f;

    sf::Clock clock;

    while (window.isOpen())
    {
        const float dt = clock.restart().asSeconds();

        handleInput(window, state, tractionForce, brakingForce, startX);
        updatePhysics(
            dt, state, tractionForce, brakingForce,
            startX, carWidth, carHeight, carY, finishX);

        car.setPosition({ state.positionX, carY });
        infoText.setString(makeInfoString(
            state, tractionForce, brakingForce, startX));

        drawScene(window, car, road, finishLine, state, infoText);
    }

    return 0;
}
