#include <iostream>
#include "../System/Core/Core.hpp"
#include "../Manager/Component/ComponentRegistry.hpp"
#include "GameLogic/Factory/ControllerFactory.h"
#include "GameLogic/Factory/RotatingFactory.h"
#include "../Manager/Level/LevelManager.hpp"

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR cmd_line, int show_cmd)
{
    E5_LEAK_CHECKS(-1);
    auto app = new CS460::Application();
    app->Initialize();
    //app->GetLevelManager()->SetInitialLevel("Collider");
    //Add game components
    app->GetComponentRegistry()->AddFactory(new Game::ControllerFactory);
    app->GetComponentRegistry()->AddFactory(new Game::RotatingFactory);

    app->Update();
    app->Shutdown();
    delete app;
    return 0;
}

