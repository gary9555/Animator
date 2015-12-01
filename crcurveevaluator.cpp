#include "CRCurveEvaluator.h"
#include <assert.h>
#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.h>
#include <FL/gl.h>
#include <GL/glu.h>
#include <cstdio>

#include "iostream"

void CRCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength,
	const bool& bWrap) const
{
	ptvEvaluatedCurvePts.clear();
	int iCtrlPtCount = ptvCtrlPts.size();

	GLfloat bbm[16] = { -0.5, 1, -0.5, 0, 1.5, -2.5, 0, 1, -1.5, 2, 0.5, 0, 0.5, -0.5, 0, 0 };

	//	ptvEvaluatedCurvePts.clear();
	//	std::sort(ptvCtrlPts.begin(), ptvCtrlPts.end());

	std::vector<Point>shadowptvCtrlPts;
	shadowptvCtrlPts.push_back(Point(0, ptvCtrlPts[0].y));
	for (int k = 0; k<iCtrlPtCount; k++)
	{
		shadowptvCtrlPts.push_back(ptvCtrlPts[k]);
	}
	shadowptvCtrlPts.push_back(Point(fAniLength, ptvCtrlPts[iCtrlPtCount - 1].y));

	if (shadowptvCtrlPts.size() > 3 && !bWrap)
	{
		for (int i = 0; i < shadowptvCtrlPts.size() - 3; i++)
		{
			GLfloat ctrlpoint[8] = { shadowptvCtrlPts[i].x, shadowptvCtrlPts[i + 1].x, shadowptvCtrlPts[i + 2].x, shadowptvCtrlPts[i + 3].x, shadowptvCtrlPts[i].y, shadowptvCtrlPts[i + 1].y,
				shadowptvCtrlPts[i + 2].y, shadowptvCtrlPts[i + 3].y };
			for (float j = 0.0; j < 1.01; j = j + 0.1)
			{
				GLfloat t[4] = { j*j*j, j*j, j, 1 };
				GLfloat mat1[4] = { bbm[0] * t[0] + bbm[1] * t[1] + bbm[2] * t[2] + bbm[3] * t[3], bbm[4] * t[0] + bbm[5] * t[1] + bbm[6] * t[2] + bbm[7] * t[3],
					bbm[8] * t[0] + bbm[9] * t[1] + bbm[10] * t[2] + bbm[11] * t[3], bbm[12] * t[0] + bbm[13] * t[1] + bbm[14] * t[2] + bbm[15] * t[3] };
				GLfloat evapoint[2] = { mat1[0] * ctrlpoint[0] + mat1[1] * ctrlpoint[1] + mat1[2] * ctrlpoint[2] + mat1[3] * ctrlpoint[3],
					mat1[0] * ctrlpoint[4] + mat1[1] * ctrlpoint[5] + mat1[2] * ctrlpoint[6] + mat1[3] * ctrlpoint[7] };
				Point p = Point(evapoint[0], evapoint[1]);
				ptvEvaluatedCurvePts.push_back(p);
			}
		}
	}



	if (shadowptvCtrlPts.size() > 3 && bWrap)
	{
		std::vector<Point> wrappoints = { ptvCtrlPts[iCtrlPtCount - 3], ptvCtrlPts[iCtrlPtCount - 2], ptvCtrlPts[iCtrlPtCount - 1],
			Point(ptvCtrlPts[0].x + fAniLength, ptvCtrlPts[0].y), Point(ptvCtrlPts[1].x + fAniLength, ptvCtrlPts[1].y), Point(ptvCtrlPts[2].x + fAniLength, ptvCtrlPts[2].y) };

		for (int i = 0; i < wrappoints.size() - 3; i++)
		{
			GLfloat ctrlpoint[8] = { wrappoints[i].x, wrappoints[i + 1].x, wrappoints[i + 2].x, wrappoints[i + 3].x, wrappoints[i].y, wrappoints[i + 1].y,
				wrappoints[i + 2].y, wrappoints[i + 3].y };
			for (float j = 0.0; j < 1.01; j = j + 0.1)
			{
				GLfloat t[4] = { j*j*j, j*j, j, 1 };
				GLfloat mat1[4] = { bbm[0] * t[0] + bbm[1] * t[1] + bbm[2] * t[2] + bbm[3] * t[3], bbm[4] * t[0] + bbm[5] * t[1] + bbm[6] * t[2] + bbm[7] * t[3],
					bbm[8] * t[0] + bbm[9] * t[1] + bbm[10] * t[2] + bbm[11] * t[3], bbm[12] * t[0] + bbm[13] * t[1] + bbm[14] * t[2] + bbm[15] * t[3] };
				GLfloat evapoint[2] = { mat1[0] * ctrlpoint[0] + mat1[1] * ctrlpoint[1] + mat1[2] * ctrlpoint[2] + mat1[3] * ctrlpoint[3],
					mat1[0] * ctrlpoint[4] + mat1[1] * ctrlpoint[5] + mat1[2] * ctrlpoint[6] + mat1[3] * ctrlpoint[7] };

				if (evapoint[0] > fAniLength)
				{
					evapoint[0] -= fAniLength;
					Point p = Point(evapoint[0], evapoint[1]);
					ptvEvaluatedCurvePts.push_back(p);
				}
			}
		}


		for (int i = 0; i < ptvCtrlPts.size() - 3; i++)
		{
			GLfloat ctrlpoint[8] = { ptvCtrlPts[i].x, ptvCtrlPts[i + 1].x, ptvCtrlPts[i + 2].x, ptvCtrlPts[i + 3].x, ptvCtrlPts[i].y, ptvCtrlPts[i + 1].y,
				ptvCtrlPts[i + 2].y, ptvCtrlPts[i + 3].y };
			for (float j = 0.0; j < 1.01; j = j + 0.1)
			{
				GLfloat t[4] = { j*j*j, j*j, j, 1 };
				GLfloat mat1[4] = { bbm[0] * t[0] + bbm[1] * t[1] + bbm[2] * t[2] + bbm[3] * t[3], bbm[4] * t[0] + bbm[5] * t[1] + bbm[6] * t[2] + bbm[7] * t[3],
					bbm[8] * t[0] + bbm[9] * t[1] + bbm[10] * t[2] + bbm[11] * t[3], bbm[12] * t[0] + bbm[13] * t[1] + bbm[14] * t[2] + bbm[15] * t[3] };
				GLfloat evapoint[2] = { mat1[0] * ctrlpoint[0] + mat1[1] * ctrlpoint[1] + mat1[2] * ctrlpoint[2] + mat1[3] * ctrlpoint[3],
					mat1[0] * ctrlpoint[4] + mat1[1] * ctrlpoint[5] + mat1[2] * ctrlpoint[6] + mat1[3] * ctrlpoint[7] };
				Point p = Point(evapoint[0], evapoint[1]);
				ptvEvaluatedCurvePts.push_back(p);
			}
		}

		for (int i = 0; i < wrappoints.size() - 3; i++)
		{
			GLfloat ctrlpoint[8] = { wrappoints[i].x, wrappoints[i + 1].x, wrappoints[i + 2].x, wrappoints[i + 3].x, wrappoints[i].y, wrappoints[i + 1].y,
				wrappoints[i + 2].y, wrappoints[i + 3].y };
			for (float j = 0.0; j < 1.01; j = j + 0.1)
			{
				GLfloat t[4] = { j*j*j, j*j, j, 1 };
				GLfloat mat1[4] = { bbm[0] * t[0] + bbm[1] * t[1] + bbm[2] * t[2] + bbm[3] * t[3], bbm[4] * t[0] + bbm[5] * t[1] + bbm[6] * t[2] + bbm[7] * t[3],
					bbm[8] * t[0] + bbm[9] * t[1] + bbm[10] * t[2] + bbm[11] * t[3], bbm[12] * t[0] + bbm[13] * t[1] + bbm[14] * t[2] + bbm[15] * t[3] };
				GLfloat evapoint[2] = { mat1[0] * ctrlpoint[0] + mat1[1] * ctrlpoint[1] + mat1[2] * ctrlpoint[2] + mat1[3] * ctrlpoint[3],
					mat1[0] * ctrlpoint[4] + mat1[1] * ctrlpoint[5] + mat1[2] * ctrlpoint[6] + mat1[3] * ctrlpoint[7] };

				if (evapoint[0] < fAniLength)
				{
					Point p = Point(evapoint[0], evapoint[1]);
					ptvEvaluatedCurvePts.push_back(p);
				}
			}
		}

	}


	float x = 0.0;
	float y1;

	if(!bWrap) {
		// if wrapping is off, make the first and last segments of
		// the curve horizontal.

		y1 = ptvCtrlPts[0].y;
		ptvEvaluatedCurvePts.push_back(Point(x, y1));
	}

	

	/// set the endpoint based on the wrap flag.
	float y2;
	x = fAniLength;
	if (!bWrap)
	{
		y2 = ptvCtrlPts[iCtrlPtCount - 1].y;
		ptvEvaluatedCurvePts.push_back(Point(x, y2));
	}

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