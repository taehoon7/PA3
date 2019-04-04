
// File:        main.cpp
// Author:      cheeren
// Date:        2019-03-04
// Description: Partial test of PA3 functionality


#include "cs221util/HSLAPixel.h"
#include "cs221util/PNG.h"
#include "toqutree.h"

using namespace cs221util;
using namespace std;

int main()
{/*
	PNG origIm1;
	origIm1.readFromFile("images/ada.png");
	toqutree t1(origIm1, 9); //3
	PNG ppic1 = t1.render();
	ppic1.writeToFile("images/zada.png");

	PNG origIm2;
	origIm2.readFromFile("images/canadaPlace.png");
	toqutree t2(origIm2, 8); //3
	PNG ppic2 = t2.render();
	ppic2.writeToFile("images/zcanadaPlace.png");
	
	PNG origIm3;
	origIm3.readFromFile("images/color.png");
	toqutree t3(origIm3, 8); //3
	PNG ppic3 = t3.render();
	ppic3.writeToFile("images/zcolor.png");*/
	
	PNG origIm4;
	origIm4.readFromFile("images/geo.png");
	toqutree t4(origIm4, 5); //3
	t4.prune(0.05);
	PNG ppic4 = t4.render();
	ppic4.writeToFile("images/zgeo.png");
	/*
	PNG origIm5;
	origIm5.readFromFile("images/oneSmall.png");
	toqutree t5(origIm5, 3); //3
	PNG ppic5 = t5.render();
	ppic5.writeToFile("images/zoneSmall.png");

	PNG origIm6;
	origIm6.readFromFile("images/oneSquare.png");
	toqutree t6(origIm6, 4); //3
	PNG ppic6 = t6.render();
	ppic6.writeToFile("images/zoneSquare.png");
	
	PNG origIm7;
	origIm7.readFromFile("images/smB.png");
	toqutree t7(origIm7, 3); //3
	PNG ppic7 = t7.render();
	ppic7.writeToFile("images/zsmB.png");
	
	PNG origIm8;
	origIm8.readFromFile("images/spaceShips.png");
	toqutree t8(origIm8, 10); //3
	PNG ppic8 = t8.render();
	ppic8.writeToFile("images/zspaceShips.png");
	
	PNG origIm9;
	origIm9.readFromFile("images/stanleySquare.png");
	toqutree t9(origIm9, 9); //3
	PNG ppic9 = t9.render();
	ppic9.writeToFile("images/zstanleySquare.png");
	
	PNG origIm10;
	origIm10.readFromFile("images/twoSquares.png");
	toqutree t10(origIm10, 4); //3
	PNG ppic10 = t10.render();
	ppic10.writeToFile("images/ztwoSquares.png");
	
	PNG origIm11;
	origIm11.readFromFile("images/ubc-totem-poles.png");
	toqutree t11(origIm11, 9); //3
	PNG ppic11 = t11.render();
	ppic11.writeToFile("images/zubc-totem-poles.png");*/
	// read in an image
	/*PNG origIm1;
	origIm1.readFromFile("images/canadaPlace.png");
	
	// use it to build a toqutree
	toqutree t1(origIm1,3); //3
	
    // make some copies for pruning tests
	toqutree tCopy1(t1);
	toqutree tCopy2(t1);
	toqutree tCopy3(t1);
	toqutree tCopy4(t1);

	// prune the toqutree
	//where all subtree pixels are within Y of mean

	tCopy1.prune(.2); 
	tCopy2.prune(.1); 
	tCopy3.prune(.05); 
	tCopy4.prune(.025); 

	// render the toqutree
    PNG ppic1 = t1.render();
    PNG ppiccopy1 = tCopy1.render();
    PNG ppiccopy2 = tCopy2.render();
    PNG ppiccopy3 = tCopy3.render();
    PNG ppiccopy4 = tCopy4.render();

    // write the pngs to files.
	ppic1.writeToFile("images/output-CP.png");
	ppiccopy1.writeToFile("images/output-CP.2.png");
	ppiccopy2.writeToFile("images/output-CP.1.png");
	ppiccopy3.writeToFile("images/output-CP.05.png");
	ppiccopy4.writeToFile("images/output-CP.025.png");*/


  return 0;
}

