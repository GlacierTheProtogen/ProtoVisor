#ifndef FACE_H
#define FACE_H

#include <string>
#include <fstream>
#include <iostream>


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

//take a face name from the faces folder and convert it to a 2d array of bools
bool** TwoFilesToFace(std::string filename_left, std::string filename_right)
{
  std::string filename_left_string="faces/"+filename_left+".txt";
  std::string filename_right_string="faces/"+filename_right+".txt";
  std::ifstream newfile_left;
  std::ifstream newfile_right;
  newfile_left.open(filename_left_string);
  newfile_right.open(filename_right_string);

  int width = 64;
  int face_size = 128;

  bool** face2D = 0;
  face2D = new bool*[32];

  std::string iter;
  int columniter = 0;
  while(getline(newfile_left, iter)){
    face2D[columniter] = new bool[face_size];
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

  columniter = 0;

  while(getline(newfile_right, iter)){
    for(int j = 0; j < width; j++)
    {
      if(iter[j] == 'X')
      {
        face2D[columniter][j + width] = true;
      }
      else
      {
        face2D[columniter][j + width] = false;
      }
   }

    columniter++;
  }

  return face2D;

}


//take a face name from the faces/dinosaur-game folder and convert it to a 2d array of bools with a predetermined size. 
bool** FileToSprite(std::string filename, int height, int width)
{
  std::string fullfile="faces/dinosaur-game/"+filename+".txt";
  std::ifstream newfile;
  newfile.open(fullfile);

  bool** face2D = 0;
  face2D = new bool*[height];

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
