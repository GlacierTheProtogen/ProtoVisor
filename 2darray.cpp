#include <iostream>
#include <fstream>
#include <string>
#include <vector>


//take a face name from the faces folder and then convert it to a 2d array of bools
bool** FileToFace(std::string filename)
{
  std::string fullfile="faces/"+filename+".txt";
  std::ifstream newfile;
  newfile.open(fullfile);

  bool** face2D = 0;
  face2D = new bool*[32];


  std::string iter;
  int columniter = 0;
  while(getline(newfile, iter)){
      face2D[columniter] = new bool[64];
      for(int j = 0; j < 64; j++)
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

int main()
{

  std::string face_names[5] = { "baseface", "blink", "happy", "heart", "poker" };
  bool** face = FileToFace("blink");


  return 0;
}
