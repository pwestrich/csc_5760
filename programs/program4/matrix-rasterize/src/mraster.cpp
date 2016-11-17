#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include "IOMatrix.h"


void gen_png(char* filename, IOMatrix<int> world);


using namespace std;


#define BPP 24
#include <FreeImage.h>
void gen_png(char* filename, IOMatrix<int> world)
{
    FreeImage_Initialise();
  
    int rows = world.num_rows();
    int cols = world.num_cols();

  FIBITMAP* bitmap = FreeImage_Allocate(
  	cols*cols, 
  	rows*rows, BPP);
  FIBITMAP* gifmap;

  RGBQUAD color;
  
  if (!bitmap)
    exit(1);

    
  int k=0,l=0;
  for (int i=0; i<cols*cols; i++) {
      k = (i / cols);
    for (int j=0; j<rows*rows; j++) {
      l = (j / rows);
      color.rgbRed = 0;
      color.rgbGreen = 0;
      color.rgbBlue = (world.get_value(l,k) == 0) ? 0.0 : (255.0);

      FreeImage_SetPixelColor(bitmap,i,j,&color);

    }
  }

  // convert to 8-bit, preserving color
  gifmap = FreeImage_ColorQuantize(bitmap, FIQ_WUQUANT);

  FIBITMAP* done = FreeImage_Rotate(gifmap, 180.0);
  FreeImage_FlipHorizontal(done);

  FreeImage_Save(FIF_GIF, done, filename, 0);
  FreeImage_DeInitialise();

} // ------------------------------------------------------------------------



int main(int argc, char* argv[])
{

	string filename;
	int rows=0,cols=0;

	if(argc > 1)
	{
		filename = string(argv[1]);
		if(argc > 2)
		{
			rows = atoi(argv[2]);
			cerr << "rows: "<<rows << endl;
			if(argc > 3)
			{
				cols = atoi(argv[3]);
				cerr << "cols: "<<cols << endl;
			}
		}
	}


	IOMatrix<int> M;
	M.load_from_file(rows,cols,filename);


	string outfile(filename+".gif");
	char *ofile = new char[outfile.length() + 1]; 
	std::strcpy(ofile, outfile.c_str());


	gen_png(ofile, M);

	delete[] ofile;

	return 0;
}

