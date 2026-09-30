#include "Entities/Parasite.hpp"
const float PARASITE_MAX_SPEED = 400.f;
const float PARASITE_ACCEL = 2000.f;
const float PARASITE_FRICTION = 1000.f;
const float PARASITE_WIDTH = 50.f;
const float PARASITE_HEIGHT = 60.f;
const float PARASITE_GRAVITY = 1500.f;
const float PARASITE_JUMP_FORCE = 600.f;
const float FLOOR_Y = 600.f;
const float GAME_WIDTH = 1920.f;
const float MAX_FALL_SPEED = 1000.f;
const float PARASITE_DASH_FORCE = 1500.f;
const float DASH_COOLDOWN_TIME = 5.0f;
const float DASH_DURATION = 0.20f;
const float EYE_SIZE = 12.f;
const float PUPIL_SIZE = 6.f;
const sf::Color PARASITE_COLOR = sf::Color(200, 50, 100);

Parasite::Parasite(sf::Vector2f position)
: body(sf::Vector2f(PARASITE_WIDTH, PARASITE_HEIGHT)),
  eye(sf::Vector2f(EYE_SIZE, EYE_SIZE)),
  pupil(sf::Vector2f(PUPIL_SIZE, PUPIL_SIZE)),
  accel(PARASITE_ACCEL),
  friction(PARASITE_FRICTION),
  velocity(sf::Vector2f(0.f, 0.f)),
  jumpForce(PARASITE_JUMP_FORCE),
  gravity(PARASITE_GRAVITY),
  isOnGround(false),
  dashCooldown(0.f),
  dashForce(PARASITE_DASH_FORCE),
  hasDash(true),
  dashTimer(0.f)
{
    body.setFillColor(PARASITE_COLOR);
    body.setPosition(position);
    eye.setFillColor(sf::Color::White);
    pupil.setFillColor(sf::Color::Black);
}

bool Parasite::isJumpKeyPressed()
{
    return sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || 
           sf::Keyboard::isKeyPressed(sf::Keyboard::W)     ||
           sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
}
bool Parasite::isLeftKeyPressed()
{
    return sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
}
bool Parasite::isRightKeyPressed()
{
    return sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
}

bool Parasite::isDashKeyPressed()
{
    return sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);
}

bool Parasite::checkMoveKeys(float dt)
{
    bool left = isLeftKeyPressed();
    bool right = isRightKeyPressed();

    if (left && right)
    {
        return false;
    }

    if (left)
    {
        velocity.x -= accel * dt;
        return true;
    }

    if (right)
    {
        velocity.x += accel * dt;
        return true;
    }
    
    return false;
}

void Parasite::resolveFriction(float dt)
{
    float f = isOnGround ? friction : friction * 0.3f;
    if (velocity.x > 0.f)
    {
        velocity.x -= f * dt;
        if (velocity.x < 0) velocity.x = 0.f;
    }
    else
    {
        velocity.x += f * dt;
        if (velocity.x > 0.f) velocity.x = 0.f;
    }
}

void Parasite::checkMaxSpeed()
{
    if (velocity.x > PARASITE_MAX_SPEED)
    {
        velocity.x = PARASITE_MAX_SPEED;
    }
    if (velocity.x < -PARASITE_MAX_SPEED)
    {
        velocity.x = -PARASITE_MAX_SPEED;
    }
}

void Parasite::horizontalDash(float dt)
{
    float drag = friction * 3.0f;

    if (velocity.x > 0.f)
    {
        velocity.x -= drag * dt;
        velocity.x = std::max(velocity.x, PARASITE_MAX_SPEED);
    }
    else
    {
        velocity.x += drag * dt;
        velocity.x = std::min(velocity.x, -PARASITE_MAX_SPEED);
    }
}

void Parasite::horizontalCommon(float dt)
{
    bool isMoving = checkMoveKeys(dt);

    if (!isMoving)
    {
        resolveFriction(dt);
    }

    checkMaxSpeed();
}

void Parasite::updateHorizontal(float dt)
{
    if (std::abs(velocity.x) > PARASITE_MAX_SPEED)
    {
        horizontalDash(dt);
    }
    else
    {
        horizontalCommon(dt);
    }
}

void Parasite::updateVertical(float dt)
{
    float curGravity = gravity;

    if (!isJumpKeyPressed() && velocity.y < 0.f)
    {
        curGravity *= 2.0f;
    }

    velocity.y += curGravity * dt;

    if (velocity.y > MAX_FALL_SPEED) {
       velocity.y = MAX_FALL_SPEED;
    }

    if (isJumpKeyPressed() && isOnGround)
    {
        velocity.y = -PARASITE_JUMP_FORCE;
        isOnGround = false;
    }
}

void Parasite::checkXBoundaries()
{
    if (body.getPosition().x < 0)
    {
        body.setPosition(0, body.getPosition().y);
        velocity.x = 0;
    }
    
    float rightX = body.getPosition().x + PARASITE_WIDTH;
    if (rightX >= GAME_WIDTH)
    {
        body.setPosition(GAME_WIDTH - PARASITE_WIDTH, body.getPosition().y);
        velocity.x = 0;
    }
}

void Parasite::resolveCollisions()
{
    isOnGround = false;
    
    float bottomY = body.getPosition().y + PARASITE_HEIGHT;
    if (bottomY >= FLOOR_Y)
    {
        body.setPosition(body.getPosition().x, FLOOR_Y - PARASITE_HEIGHT);
        velocity.y = 0.f;
        isOnGround = true;
        hasDash = true;
    }

    checkXBoundaries();
}

void Parasite::updateDash(float dt, sf::Vector2f mousePos)
{
    if (dashCooldown > 0.f)
    {
        dashCooldown -= dt;
    }

    if (isDashKeyPressed() && dashCooldown <= 0.f && hasDash)
    {
        sf::Vector2f center = body.getPosition() + sf::Vector2f(PARASITE_WIDTH / 2.f, PARASITE_HEIGHT / 2.f);

        float dx = mousePos.x - center.x;
        float dy = mousePos.y - center.y;

        float length = std::sqrt(dx * dx + dy * dy);

        if (length != 0)
        {
            velocity.x = (dx / length) * dashForce;
            velocity.y = (dy / length) * dashForce;

            dashCooldown = DASH_COOLDOWN_TIME;
            dashTimer = DASH_DURATION;
            hasDash = false;
            isOnGround = false;
        }
    }
}

void Parasite::updateDashTimer(float dt)
{
    if (dashTimer > 0.f)
    {
        dashTimer -= dt;

        if (dashTimer <= 0.f)
        {
            velocity.x *= 0.5f;
            velocity.y *= 0.5f;
        }
    }
}

void Parasite::updateMove(float dt)
{

    updateDashTimer(dt);
    if (dashTimer <= 0.f)
    {
        updateHorizontal(dt);
        updateVertical(dt);
    }
}

sf::FloatRect Parasite::getBounds() const
{
    return body.getGlobalBounds(); 
}

void Parasite::moveEye(sf::Vector2f mousePos)
{
    sf::Vector2f center = body.getPosition() + sf::Vector2f(PARASITE_WIDTH / 2.f, PARASITE_HEIGHT / 4.f);
    float dx = mousePos.x - center.x;
    float dy = mousePos.y - center.y;
    float length = std::sqrt(dx * dx + dy * dy);

    float dirX = 0.f;
    float dirY = 0.f;
    if (length != 0) {
        dirX = dx / length;
        dirY = dy / length;
    }
    sf::Vector2f eyeCenter(center.x + dirX * 10.f, center.y + dirY * 10.f);
    eye.setPosition(eyeCenter.x - 6.f, eyeCenter.y - 6.f);

    sf::Vector2f pupilCenter(eyeCenter.x + dirX * 3.f, eyeCenter.y + dirY * 3.f);
    pupil.setPosition(pupilCenter.x - 3.f, pupilCenter.y - 3.f);
}

void Parasite::update(float dt, sf::Vector2f mousePos)
{
    updateDash(dt, mousePos);
    
    updateMove(dt);

    body.move(velocity.x * dt, velocity.y * dt);

    resolveCollisions();

    moveEye(mousePos);

    
}

void Parasite::draw(sf::RenderWindow& window)
{
    window.draw(body);
    window.draw(eye);
    window.draw(pupil);
}