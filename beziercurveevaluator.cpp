#include "BezierCurveEvaluator.h"
#include <assert.h>
#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.h>
#include <FL/gl.h>
#include <GL/glu.h>
#include <cstdio>

#include "iostream"

void BezierCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength,
	const bool& bWrap) const
{
	ptvEvaluatedCurvePts.clear();
	int iCtrlPtCount = ptvCtrlPts.size();

	GLfloat bbm[16] = { -1, 3, -3, 1, 3, -6, 3, 0, -3, 3, 0, 0, 1, 0, 0, 0 };

//	ptvEvaluatedCurvePts.clear();
//	std::sort(ptvCtrlPts.begin(), ptvCtrlPts.end());

	if (iCtrlPtCount > 3)
	{
			GLfloat ctrlpoint[8] = { ptvCtrlPts[0].x, ptvCtrlPts[1].x, ptvCtrlPts[2].x, ptvCtrlPts[3].x, ptvCtrlPts[0].y, ptvCtrlPts[1].y,
				ptvCtrlPts[2].y, ptvCtrlPts[3].y};
			for (float j = 0.0; j < 1.0; j = j + 0.1)
			{
				GLfloat t[4] = { j*j*j, j*j, j, 1};
				GLfloat mat1[4] = { bbm[0] * t[0] + bbm[1] * t[1] + bbm[2] * t[2] + bbm[3] * t[3], bbm[4] * t[0] + bbm[5] * t[1] + bbm[6] * t[2] + bbm[7] * t[3],
					bbm[8] * t[0] + bbm[9] * t[1] + bbm[10] * t[2] + bbm[11] * t[3], bbm[12] * t[0] + bbm[13] * t[1] + bbm[14] * t[2] + bbm[15] * t[3] };
				GLfloat evapoint[2] = { mat1[0] * ctrlpoint[0] + mat1[1] * ctrlpoint[1] + mat1[2] * ctrlpoint[2] + mat1[3] * ctrlpoint[3],
					mat1[0] * ctrlpoint[4] + mat1[1] * ctrlpoint[5] + mat1[2] * ctrlpoint[6] + mat1[3] * ctrlpoint[7] };
				Point p = Point(evapoint[0], evapoint[1]);
				ptvEvaluatedCurvePts.push_back(p);
			}
	}


	float x = 0.0;
	float y1;

	if (bWrap) {
		// if wrapping is on, interpolate the y value at xmin and
		// xmax so that the slopes of the lines adjacent to the
		// wraparound are equal.

		if ((ptvCtrlPts[0].x + fAniLength) - ptvCtrlPts[iCtrlPtCount - 1].x > 0.0f) {
			y1 = (ptvCtrlPts[0].y * (fAniLength - ptvCtrlPts[iCtrlPtCount - 1].x) +
				ptvCtrlPts[iCtrlPtCount - 1].y * ptvCtrlPts[0].x) /
				(ptvCtrlPts[0].x + fAniLength - ptvCtrlPts[iCtrlPtCount - 1].x);
		}
		else
			y1 = ptvCtrlPts[0].y;
	}
	else {
		// if wrapping is off, make the first and last segments of
		// the curve horizontal.

		y1 = ptvCtrlPts[0].y;
	}

	ptvEvaluatedCurvePts.push_back(Point(x, y1));

	/// set the endpoint based on the wrap flag.
	float y2;
	x = fAniLength;
	if (bWrap)
		y2 = y1;
	else
		y2 = ptvCtrlPts[iCtrlPtCount - 1].y;

	ptvEvaluatedCurvePts.push_back(Point(x, y2));

	//test part

	std::cout << "ictrlcount = " << iCtrlPtCount << std::endl;
	for (int i = 0; i < iCtrlPtCount; i++)
	{
		std::cout << ptvCtrlPts[i].x << " " << ptvCtrlPts[i].y << std::endl;
	}
	std::cout << std::endl;
	for (int i = 0; i < ptvEvaluatedCurvePts.size(); i++)
	{
		std::cout << ptvEvaluatedCurvePts[i].x << " " << ptvEvaluatedCurvePts[i].y << std::endl;
	}
	std::cout << std::endl;
	//end of test part
}