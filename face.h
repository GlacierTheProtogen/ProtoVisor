#ifndef FACE_H
#define FACE_H

#include <string>
#include <fstream>

//take a face name from the faces folder and convert it to a 2d array of bools
bool** FileToFace(std::string filename, bool full)
{
  std::string fullfile="faces/"+filename+".txt";
  std::ifstream newfile;
  newfile.open(fullfile);

  int width = 64;

  if(full)
  {
    width = 128;
  }

  bool** face2D = 0;
  face2D = new bool*[32];

  std::string iter;
  int columniter = 0;
  while(getline(newfile, iter)){
      face2D[columniter] = new bool[width];
      for(int j = 0; j < width; j++)
      {
          if(iter[j] == 'X')
          {
            face2D[columniter][j] = true;
          }
          else
          {
            face2D[columniter][j] = false;
          }
      }

      columniter++;
   }

  return face2D;

}

#endif
