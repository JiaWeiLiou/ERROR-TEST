/******************************************************************************
FileName     [ basic.h ]
PackageName  [ AGS ]
Synopsis     [ Automatic Grain Sizing ]
Author       [ Jia-Wei (Jimmy) Liou ]
Copyright    [ Copyleft(c) 2018-present LaDF, CE-Hydrolic, NTU, Taiwan ]
******************************************************************************/

#ifndef BASIC_H
#define BASIC_H

#include "output.h"
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// Minimum watershed segment label
#define LABEL_MINIMUM 0
// Label for the watersheds pixels (local minima)
#define LABEL_RIDGE -1
// Label for pixels that are not local minima
#define LABEL_NOLOCALMINIMUM -2
// Label for pixels that are or have been in the OrderedQueques
#define LABEL_PROCESSING -3
// Label for unprocessed pixels
#define LABEL_UNPROCESSED -4

//Remove Ambient Light for Area
//gray : gray image (8UC1)
//blur : blur image (8UC1)
//divide : remove ambient light image (8UC1)
void DivideArea(InputArray _gray, InputArray _blur, OutputArray _divide);

//Otsu Threshold
//gray : gray image (8UC1)
//binary : binary image (8UC1(BW))
void OtsuThreshold(InputArray _gray, OutputArray _binary);

//Calculate Image Gradient
//gray : gray image(8UC1)
//gradient : gradient image(8UC1)
void Gradient(InputArray _gray, OutputArray _gradient);

//Remove Ambient Light for Image Gradient
//gradient : gradient image(8UC1)
//blur : blur gradient image (8UC1)
//divide : remove ambient light gradient image (8UC1)
void DivideLine(InputArray _gradient, InputArray _blur, OutputArray _divide);

//Hysteresis Cut Binary Image to Line by Area
//binary : binary image (8UC1(BW))
//area : area binary image (8UC1(BW))
//line : line binary image (8UC1(BW))
void HysteresisCut(InputArray _binary, InputArray _area, OutputArray _line);

//Reverse Binary Image
//binary : binary image (8UC1(BW))
//rbinary : reversed binary image (8UC1(BW))
void ReverseBinary(InputArray _binary, OutputArray _rbinary);

//Combine Area and Line Extraction
//area : area binary image (8UC1(BW))
//line : line binary image (8UC1(BW))
//binary : combine binary image (8UC1(BW))
void Combine(InputArray _area, InputArray _line, OutputArray _binary);

//Clear Black Noise
//binary : binary image (8UC1(BW))
//clear : clear noise image (8UC1(BW))
void ClearNoise(InputArray _binary, OutputArray _clear);

//Image Reconstruct
//marker : marker image (32FC1)
//mask : mask image (32FC1)
void Reconstruct(InputArray _marker, InputArray _mask, OutputArray _hdistance);

//H Minima Transform
//distance : distance image (32FC1)
//hdistance : distance image with h minima transform (32FC1)
//h : height
void HMinimaTransform(InputArray _distance, OutputArray _hdistance, float h);

//Detect Regional Minima
//distance : distance image (32FC1)
//seed : seed image (8UC1(BW))
void DetectRegionalMinima(InputArray _distance, OutputArray _seed);

//Extend Regional Minima
//distance : distance image (32FC1)
//seed : seed image (8UC1(BW))
//h : height
void ExtendRegionalMinima(InputArray _distance, OutputArray _seed, float h);

//Distance Cut
//distance : distance image (32FC1)
//seed : seed image (8UC1(BW))
void DistanceCut(InputArray _distance, OutputArray _seed);

//Add Unseed Labels
//binary : binary image (8UC1(BW))
//seed : seed image (8UC1(BW))
//fseed : finial Seed
void AddSeed(InputArray _binary, InputArray _seed, OutputArray _fseed);

//Impose Minima
//distance : distance image (32FC1)
//seed : seed image (8UC1(BW))
//idistance : impose minima distance image (32FC1)
void ImposeMinima(InputArray _distance, InputArray _seed, OutputArray _idistance);

class PixelElement
{
public:
	PixelElement(float i, int j, int k) {
		value = i;
		x = j;
		y = k;
	}
	float value;  ///< pixel intensity
	int x;      ///< x coordinate of pixel
	int y;      ///< y coordinate of pixel
};

class mycomparison
{
	bool reverse;
public:
	/** \brief constructor
	* \param revparam true if we want to sort from large to small*/
	mycomparison(const bool& revparam = true)
	{
		reverse = revparam;
	}

	/** \brief comparison operator: compares the values of two pixel elements
	* \param lhs first pixel element
	* \param rhs second pixel element*/
	bool operator() (PixelElement& lhs, PixelElement& rhs) const
	{
		if (reverse) return (lhs.value > rhs.value);
		else return (lhs.value < rhs.value);
	}
};

//Detect Minima
//distance : distance image (32FC1)
//labelImg : labels image(8UC3)
//sortedQueue : sorted queue
void DetectMinima(InputArray _distance, OutputArray _labelImg, priority_queue<PixelElement, vector<PixelElement>, mycomparison> &sortedQueue);

bool CheckForAlreadyLabeledNeighbours(int x, int y, Mat &label, Point2i &outLabeledNeighbour, int &outLabel);

bool CheckIfPixelIsWatershed(int x, int y, Mat &label, Point2i &inLabeledNeighbour, int &inLabelOfNeighbour);

// Watershed Transform
//binary : binary image (8UC1(BW))
//distance : distance image (32FC1)
//object : object image (8UC1(BW))
void WatershedTransform(InputArray _binary, InputArray _distance, OutputArray _object);

//Delete Edge object
//binary : binary image (8UC1(BW))
//object : object image (8UC1(BW))
void DeleteEdge(InputArray _binary, OutputArray _object);

#endif // BASIC_H