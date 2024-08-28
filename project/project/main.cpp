#include "Application.hpp"

int main() {
    Application app;
    app.run();
}

/*#include "portable-file-dialogs.h"
#include "raylib.h"
#include "GUI.hpp"

#include <iostream>

void test_open_file();

int main()
{
    // Check that a backend is available
   if (!pfd::settings::available())
    {
        std::cout << "Portable File Dialogs are not available on this platform.\n";
        return 1;
    }

    // Set verbosity to true
    pfd::settings::verbose(true);
    InitWindow(1000, 500, "FFF");
    Button Test = Button({100,100,100,100}, "TEST", -1, BLACK, 20);
    while(!WindowShouldClose()) {
        std::cout << Test.handle();
        if (Test.handle()) {
            
            test_open_file();
        };

        BeginDrawing();
        ClearBackground(LIGHTGRAY);
        Test.draw();
        EndDrawing();
    }
}
         
void test_open_file()
{
    // File open
    auto f = pfd::open_file("Choose files to read", pfd::path::home(),
                            { "Text Files (.txt .text)", "*.txt *.text",
                              "All Files", "*" },
                            pfd::opt::none);
    std::cout << "Selected files:";
//        std::cout << " " + f.result().front();
    // for (auto const &name : f.result())
    std::cout << "\n";
}

*/
