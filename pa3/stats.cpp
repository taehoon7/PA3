
#include "stats.h"

stats::stats(PNG & im){
	sumHueX.resize(im.height(), vector<double>(im.width(), 0));
	sumHueY.resize(im.height(), vector<double>(im.width(), 0));
	sumSat.resize(im.height(), vector<double>(im.width(), 0));
	sumLum.resize(im.height(), vector<double>(im.width(), 0));
	//printf("%f", im.getPixel(1, 1)->h);
	sumHueX[0][0] = cos(PI/180.0*im.getPixel(0,0)->h); //initialize position 0,0
	sumHueY[0][0] = sin(PI/180.0*im.getPixel(0,0)->h);
	sumSat[0][0] = im.getPixel(0,0)->s;
	sumLum[0][0] = im.getPixel(0,0)->l;
	
	hist.resize(im.height(), vector<vector<int>>(im.width(), vector<int>(36, 0)));
	for (int k = 0; k < 36; k++) { 
		if (im.getPixel(0,0)->h >= k * 10 && im.getPixel(0,0)->h < (k + 1) * 10) {
			hist[0][0][k]++;
			break;
		}
	}
	//might not be able to cast????
	for (unsigned int row = 0; row < im.height(); row++) {// for sumHueX and sumHueY
		for (unsigned int col = 0; col < im.width(); col++) {
			if (row >= 1 && col >= 1) { //usually sum equals left & up of position - corner left position + position hue
				sumHueX[row][col] = sumHueX[row - 1][col] + sumHueX[row][col - 1] - sumHueX[row - 1][col - 1] + cos(PI/180.0*im.getPixel(col,row)->h);
				sumHueY[row][col] = sumHueY[row - 1][col] + sumHueY[row][col - 1] - sumHueY[row - 1][col - 1] + sin(PI/180.0*im.getPixel(col,row)->h);
				sumSat[row][col] = sumSat[row - 1][col] + sumSat[row][col - 1] - sumSat[row - 1][col - 1] + im.getPixel(col,row)->s;
				sumLum[row][col] = sumLum[row - 1][col] + sumLum[row][col - 1] - sumLum[row - 1][col - 1] + im.getPixel(col,row)->l;

				for (int k = 0; k < 36; k++) { //for hist
					hist[row][col][k] = hist[row - 1][col][k] + hist[row][col - 1][k] - hist[row - 1][col - 1][k];
					if (im.getPixel(col,row)->h >= k * 10 && im.getPixel(col,row)->h < (k + 1) * 10) {
						hist[row][col][k]++;
					}
				}
			}
			else if (col == 0 && row >= 1) { //x value is 0
				sumHueX[row][col] = sumHueX[row - 1][col] + cos(PI/180.0*im.getPixel(col,row)->h);
				sumHueY[row][col] = sumHueY[row - 1][col] + sin(PI/180.0*im.getPixel(col,row)->h);
				sumSat[row][col] = sumSat[row - 1][col] + im.getPixel(col,row)->s;
				sumLum[row][col] = sumLum[row - 1][col] + im.getPixel(col,row)->l;

				for (int k = 0; k < 36; k++) { //for hist
					hist[row][col][k] = hist[row - 1][col][k];
					if (im.getPixel(col,row)->h >= k * 10 && im.getPixel(col,row)->h < (k + 1) * 10) {
						hist[row][col][k]++;
					}
				}
			}
			else if (row == 0 && col >= 1) { //y value is 0
				sumHueX[row][col] = sumHueX[row][col - 1] + cos(PI/180.0*im.getPixel(col,row)->h);
				sumHueY[row][col] = sumHueY[row][col - 1] + sin(PI/180.0*im.getPixel(col,row)->h);
				sumSat[row][col] = sumSat[row][col - 1] + im.getPixel(col,row)->s;
				sumLum[row][col] = sumLum[row][col - 1] + im.getPixel(col,row)->l;

				for (int k = 0; k < 36; k++) { //for hist
					hist[row][col][k] = hist[row][col - 1][k];
					if (im.getPixel(col,row)->h >= k * 10 && im.getPixel(col,row)->h < (k + 1) * 10) {
						hist[row][col][k]++;
					}
				}
			}

		}
	}
	

	
}

long stats::rectArea(pair<int,int> ul, pair<int,int> lr){
	return (lr.first - ul.first + 1)*(lr.second - ul.second + 1);
/* your code here */

}

HSLAPixel stats::getAvg(pair<int,int> ul, pair<int,int> lr){
	HSLAPixel pixel = HSLAPixel();
	double hueX = sumHueX[lr.second][lr.first];
	double hueY = sumHueY[lr.second][lr.first];
	pixel.s = sumSat[lr.second][lr.first];
	pixel.l = sumLum[lr.second][lr.first];
	double area = rectArea(ul, lr);
	if (ul.first != 0) {
		hueX -= sumHueX[lr.second][ul.first - 1];
		hueY -= sumHueY[lr.second][ul.first - 1];
		pixel.s -= sumSat[lr.second][ul.first - 1];
		pixel.l -= sumLum[lr.second][ul.first - 1];
	}
	if (ul.second != 0) {
		hueX -= sumHueX[ul.second - 1][lr.first];
		hueY -= sumHueY[ul.second - 1][lr.first];
		pixel.s -= sumSat[ul.second - 1][lr.first];
		pixel.l -= sumLum[ul.second - 1][lr.first];
	}
	if (ul.first != 0 && ul.second != 0) {
		hueX += sumHueX[ul.second - 1][ul.first - 1];
		hueY += sumHueY[ul.second - 1][ul.first - 1];
		pixel.s += sumSat[ul.second - 1][ul.first - 1];
		pixel.l += sumLum[ul.second - 1][ul.first - 1];
	}
	if (hueX == 0) {
		if (hueY > 0) {
			pixel.h = 90.0;
		}
		else {
			pixel.h = 270.0;
		}
	}
	else {
		pixel.h = atan2(hueY, hueX) / PI * 180.0;
	}
	pixel.s /= area;
	pixel.l /= area;
	pixel.a = 1.0;

	return pixel;
/* your code here */
}

vector<int> stats::buildHist(pair<int,int> ul, pair<int,int> lr){
	vector<int> histogram(36);
	for (int k = 0; k < 36; k++) {
		histogram[k] = hist[lr.second][lr.first][k];
		if (ul.first != 0) {
			histogram[k] -= hist[lr.second][ul.first - 1][k];
		}
		if (ul.second != 0) {
			histogram[k] -= hist[ul.second - 1][lr.first][k];
		}
		if (ul.first != 0 && ul.second != 0) {
			histogram[k] += hist[ul.second - 1][ul.first - 1][k];
		}
	}

	return histogram;
/* your code here */
}

// takes a distribution and returns entropy
// partially implemented so as to avoid rounding issues.
double stats::entropy(vector<int> & distn,int area){

    double entropy = 0.;

/* your code here */

    for (int row = 0; row < 36; row++) {
        if (distn[row] > 0 ) 
            entropy += ((double) distn[row]/(double) area) 
                                    * log2((double) distn[row]/(double) area);
    }

    return  -1.0 * entropy;

}

double stats::entropy(pair<int,int> ul, pair<int,int> lr){
	vector<int> histo = buildHist(ul, lr);
	return entropy(histo, (int)rectArea(ul, lr));
/* your code here */

}
