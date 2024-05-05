#ifndef MENU_H
#define MENU_H

void changeOption(bool** face, IntTuple* prevcoords, IntTuple* newcoords)
{

  for(int i = prevcoords->get_x(); i < prevcoords->get_x() + 2; i++)
  {
     for(int j = prevcoords->get_y(); j < prevcoords->get_y() + 2; j++)
     {
       face[j][i]=false;
     }
  }

  for(int i = newcoords->get_x(); i < newcoords->get_x() + 2; i++)
  {
    for(int j = newcoords->get_y(); j < newcoords->get_y() +2; j++)
    {
      face[j][i]=true;
    }
  }

};

#endif

