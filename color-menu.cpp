#include "runner.h"
#include "face.h"
#include "led-matrix.h"
#include "controller.cpp"
#include "color-menu.h"

#include <limits.h>
#include <math.h>
#include <chrono>
#include <cassert>
#include <map>
#include <nlohmann/json.hpp>

extern std::chrono::system_clock::time_point* controller1buttons;
extern std::chrono::system_clock::time_point* controller2buttons;

extern int g_red;
extern int g_blue;
extern int g_green;

class ColorMenu : public Runner {
public:
  ColorMenu(RGBMatrix *m) : Runner(m), matrix_(m) {
    off_screen_canvas_ = m->CreateFrameCanvas();
  }
  void Run() override {
    canvas()->Clear();

    bool** menu = FileToFace("blank-base", true);

    int flowcycle = 1280000; // Integer that is used to divide x in the cosign equation. Higher = slower face floating.
    int flowcounter = 0; // Incremented integer that is used for the cosign function.
    int flowcountercompare = -8; // Used to keep track of the last integer that was used in the cosign function.
    int button; // Button that is pressed.
    bool buttonPressed = false; // Wether or not a button is pressed.
    bool** currentMenu = menu; // Current face
    bool drawNewFace = true;
    bool** prevMenu = menu;
    int currentOption = 0;
    bool selectPushedOnce = 0; // Bug where the select button is already pushed

    std::map<int, colorOption*> MenuItems;

    std::ifstream inputFile("configs/colors.json");

    if (!inputFile.is_open()) {
        std::cout << "Failed to open the colors.json file." << std::endl;
        return;
    }

    // Parse the JSON file
    nlohmann::json_abi_v3_11_2::json jsonData;
    inputFile >> jsonData;

    int i = 0;

    for (const auto& color: jsonData) {
      colorOption* newOption = new colorOption(color["name"], color["rgb"][0], color["rgb"][1], color["rgb"][2], color["coords"][0], color["coords"][1]);
      MenuItems.insert(std::make_pair(i, newOption));
      i++;
    }


    while (!interrupt_received) {

        /* If a button is pressed, maintain the same face that we have been
        drawing. If not, then go back to basic face until a new button is
        pressed */

        flowcounter++;
        drawNewFace = false;


        double cosign = 2 * cos(flowcounter / flowcycle);

        if(flowcountercompare != (int)(cosign))
        {
          flowcountercompare = (int)(cosign);
          drawNewFace = true;
        }

        if(prevMenu != currentMenu)
        {
          prevMenu = currentMenu;
          drawNewFace = true;
        }

	button = current_button_pushed(controller1buttons);


        // FIXME: The number of below if statements sucks. Maybe a dictionary of pointers?

	if(buttonPressed == true)
	{
	  if(button == 0)
	  {
	    buttonPressed = false;
            drawNewFace = true;
	  }
	}
	else
	{
	  if(button != 0)
	  {
	    buttonPressed = true;
            drawNewFace = true;

	    if(button == 11 || button == 18 || button == 19)
            {
              if(currentOption == 0)
              {
                currentOption = 7;
              }
              else
              {
                currentOption = currentOption - 1;
              }
	    }
	    else if(button == 12 || button == 8 || button == 7)
	    {
              if(currentOption == 7)
              {
                currentOption = 0;
              }
              else
              {
                currentOption = currentOption + 1;
              }
            }
            else if(button == 10 || button == 6 || button == 17 || button == 9 || button == 5 || button == 16)
            {
              currentOption = currentOption + 4;
            }

            currentOption = abs(currentOption) % 8;

            if(button == 2 && selectPushedOnce == true)
            {
              g_red = MenuItems[currentOption]->getRed();
              g_green = MenuItems[currentOption]->getGreen();
              g_blue = MenuItems[currentOption]->getBlue();
              return;
            }
            else if(button == 2 && selectPushedOnce == false)
            {
              selectPushedOnce = true;
            }

	 }

         if(drawNewFace)
         {
           canvas()->Clear();

           for(int i = 0; i < MenuItems.size(); i++)
           {
             bool selected = currentOption == i;
             drawColorOption(MenuItems[i]->getRed(), MenuItems[i]->getGreen(), MenuItems[i]->getBlue(), (int)(cosign), MenuItems[i]->getX(), MenuItems[i]->getY(), selected);
           }

         }
	}
     }

 }
 private:
   RGBMatrix *const matrix_;
   FrameCanvas *off_screen_canvas_;
};
