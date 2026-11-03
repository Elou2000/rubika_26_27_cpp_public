#include "Player.h"
#include "DynamicColliderComponent.h"

Player::Player(std::string friendlyName) : Entity(friendlyName)
{ 
    transform = AddComponent<TransformComponent>();  
    AddComponent<RenderComponent>();
    AddComponent<DynamicColliderComponent>();
    GetComponent<DynamicColliderComponent>()->SetHitboxSize(sf::Vector2f(18, 12));
    SetUpInputs();
    transform->SetScale(sf::Vector2f(5, 5));

}

void Player::SetUpInputs()
{
    gData.InputMgr->CreateInputAction("Up");
    gData.InputMgr->CreateInputAction("Down");
    gData.InputMgr->CreateInputAction("Right");
    gData.InputMgr->CreateInputAction("Left");
    
    gData.InputMgr->RegisterToInputAction("Up", "GoUp", [this](float value) {
        if (value != 0)
        {
            Move(Up);
        }
        },
       this);
  

    gData.InputMgr->RegisterToInputAction("Down", "GoDown", [this](float value)
        {if (value != 0)
    {
        Move(Down);
    }; }, this);
 

    gData.InputMgr->RegisterToInputAction("Right", "GoRight", [this](float value)
        {if (value != 0)
    {
        Move(Right);
    }; }, this);
   

    gData.InputMgr->RegisterToInputAction("Left", "GoLeft", [this](float value)
        {if (value != 0)
    {
        Move(Left);
    }; }, this);
   
}

void Player::Move(dir direction)
{
    sf::Vector2f desiredPos = transform->GetWorldPosition();
    switch (direction)
    {
    case Up:
        desiredPos += sf::Vector2f(0, -speed);
        break;
    case Right:
        desiredPos += sf::Vector2f(speed, 0);
        break;
    case Down:
        desiredPos += sf::Vector2f(0, speed);
        break;
    case Left:
        desiredPos += sf::Vector2f(-speed, 0);
        break;
    default:
        break;
    }
    transform->SetWorldPosition(desiredPos);
}




