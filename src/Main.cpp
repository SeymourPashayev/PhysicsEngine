// Main.cpp
// Created: Thu 3 Nov 2022
// ------------------------
// Seymour Pashayev
// gitHub:@SeymourPashayev
// ------------------------

#include "Application.hpp"

int main(int argc, char *args[]) {
    Application app;

    app.Setup();

    while (app.IsRunning()) {
        app.Input();
        app.Update();
        app.Render();
    }

    app.Destroy();

    return 0;
}
