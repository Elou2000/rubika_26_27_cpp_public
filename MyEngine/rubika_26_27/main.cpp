#include <SFML/Graphics.hpp>
#include <Imgui/imgui.h>
#include <Imgui/imgui-SFML.h>

#include <Globals.h>
#include <Profiler.h>
#include <Logger.h>
#include <DebugMgr.h>
#include "SceneMgr.h"
#include "TextureMgr.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "Sprite.h"
#include "InputMgr.h"
#include "Player.h"
#include "Obstacle.h"
#include "DynamicColliderComponent.h"
#include "SFML/Window.hpp"

void SetUpGame()
{
    gData.TextureMgr->LoadTexture("C:\\Users\\e.betremieux\\Documents\\GitHub\\rubika_26_27_cpp_public\\MyEngine\\Ressources\\Basement.png");
    gData.TextureMgr->LoadTexture("C:\\Users\\e.betremieux\\Documents\\GitHub\\rubika_26_27_cpp_public\\MyEngine\\Ressources\\Antonio_Belpaese.png");
    gData.TextureMgr->LoadTexture("C:\\Users\\e.betremieux\\Documents\\GitHub\\rubika_26_27_cpp_public\\MyEngine\\Ressources\\enemy_texture_1.png");
    gData.TextureMgr->LoadTexture("C:\\Users\\e.betremieux\\Documents\\GitHub\\rubika_26_27_cpp_public\\MyEngine\\Ressources\\enemy_texture_2.png");
    gData.TextureMgr->LoadTexture("C:\\Users\\e.betremieux\\Documents\\GitHub\\rubika_26_27_cpp_public\\MyEngine\\Ressources\\enemy_texture_3.png");
    gData.TextureMgr->LoadTexture("C:\\Users\\e.betremieux\\Documents\\GitHub\\rubika_26_27_cpp_public\\MyEngine\\Ressources\\enemy_texture_4.png");
    gData.TextureMgr->LoadTexture("C:\\Users\\e.betremieux\\Documents\\GitHub\\rubika_26_27_cpp_public\\MyEngine\\Ressources\\enemy_texture_5.png");
    gData.TextureMgr->LoadTexture("C:\\Users\\e.betremieux\\Documents\\GitHub\\rubika_26_27_cpp_public\\MyEngine\\Ressources\\IsaacSprite.png");
    
    gData.SceneMgr->AddNewScene(new Scene("Base Scene"));
    gData.SceneMgr->sceneList[0]->SetupGrid(sf::FloatRect({ 0.f, 0.f }, { 4000.f, 4000.f }), sf::Vector2f(64.f, 64.f));

    
    for (int i = 0; i < 10; i++) {
        Obstacle* newObstacle = new Obstacle("Obstacle");
        newObstacle->GetComponent<TransformComponent>()->SetWorldPosition(sf::Vector2f(100 + i * 200,  i * 100));
        gData.SceneMgr->sceneList[0]->AddEntityToPool(newObstacle);
    }
    
    //Obstacle* newObstacle2 = new Obstacle("Obstacle2");
    //gData.SceneMgr->sceneList[0]->AddEntityToPool(newObstacle2);
    //newObstacle2->GetComponent<TransformComponent>()->SetWorldPosition(sf::Vector2f(600, 500));
 
    Player* newEntity = new Player("John Entity");
    gData.SceneMgr->sceneList[0]->AddEntityToPool(newEntity);
}

void SetUpInputs()
{
    gData.InputMgr->LoadInputs("C:\\Users\\e.betremieux\\Documents\\GitHub\\rubika_26_27_cpp_public\\MyEngine\\Ressources\\InputsData.xml");
   
}

int main()
{
    gData.Init();
    SetUpGame();
    SetUpInputs();
    

    sf::RenderWindow window(sf::VideoMode({ 1080, 720 }), "SFML works!", sf::State::Windowed);

    ImGui::SFML::Init(window);

    Logger::Info("This is an info message. That is useful");
    Logger::Warning("This is a warning message. Should I worry?");
    Logger::Error("This is an error message. Oh sh*t!!");

    sf::Clock clock;
    clock.restart();

    while (window.isOpen())
    {
        gData.IncrementFrameCount();
        
        PROFILER_EVENT_BEGIN(PROFILER_COLOR_BLACK, "Frame %llu", gData.FrameCount);

        int deltaTimeMicroS = clock.getElapsedTime().asMicroseconds();
        float fDeltaTimeMS = (float)deltaTimeMicroS / 1000.f;
        float fDeltaTimeS = fDeltaTimeMS / 1000.f;
        sf::Time imGuiTime = clock.restart();

        PROFILER_EVENT_BEGIN(PROFILER_COLOR_BLUE, "Event & Input");

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

#ifdef _USE_IMGUI
            ImGui::SFML::ProcessEvent(window, event.value());
#endif
        }

        PROFILER_EVENT_END();

        PROFILER_EVENT_BEGIN(PROFILER_COLOR_RED, "Update");
#ifdef _USE_IMGUI
        PROFILER_EVENT_BEGIN(PROFILER_COLOR_RED, "UPDATE SFML");
        ImGui::SFML::Update(window, imGuiTime);
        PROFILER_EVENT_END();
       
#endif

        // sample
        //ImGui::ShowDemoWindow();
        //Logger::DrawLogger();
        gData.DebugMgr->Draw();
        

        PROFILER_EVENT_END();

        PROFILER_EVENT_BEGIN(PROFILER_COLOR_GREEN, "Draw");
        window.clear();
        gData.SceneMgr->Update(fDeltaTimeS);
        gData.InputMgr->Update(fDeltaTimeS);
        gData.SceneMgr->Draw(window);
#ifdef _USE_IMGUI
        ImGui::SFML::Render(window);
       
#endif
       
        window.display();

        PROFILER_EVENT_END();

        PROFILER_EVENT_END();
    }

    ImGui::SFML::Shutdown();

    gData.Shut();
    gData.Destroy();

    return 0;
}

