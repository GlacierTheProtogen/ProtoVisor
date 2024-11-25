#ifndef COLOR_MENU_H
#define COLOR_MENU_H

class colorOption {
  public:
    colorOption(std::string name_input, int red_input, int green_input, int blue_input, int x_input, int y_input)
    {
      name = name_input;
      r = red_input;
      g = green_input;
      b = blue_input;
      x = x_input;
      y = y_input;
    }
    std::string getName()
    {
      return name;
    }
    int getRed()
    {
      return r;
    }
    int getGreen()
    {
      return g;
    }
    int getBlue()
    {
      return b;
    }
    int getX()
    {
      return x;
    }
    int getY()
    {
      return y;
    }
  private:
    std::string name;
    int r;
    int g;
    int b;
    int x;
    int y;
};



#endif

