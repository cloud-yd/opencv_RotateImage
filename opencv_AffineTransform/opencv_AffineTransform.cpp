#include <iostream>
#include <opencv2/opencv.hpp>

#if 1
using namespace std;
using namespace cv;

Mat rotateImage1(Mat img, int degree)

{

	degree = -degree;

	double angle = degree * CV_PI / 180.; // 弧度  

	double a = sin(angle), b = cos(angle);

	int width = img.cols;

	int height = img.rows;

	int width_rotate = int(height * fabs(a) + width * fabs(b));

	int height_rotate = int(width * fabs(a) + height * fabs(b));

	//旋转数组map

	// [ m0  m1  m2 ] ===>  [ A11  A12   b1 ]

	// [ m3  m4  m5 ] ===>  [ A21  A22   b2 ]

	float map[6];

	Mat map_matrix = Mat(2, 3, CV_32F, map);

	// 旋转中心

	CvPoint2D32f center = cvPoint2D32f(width / 2, height / 2);

	CvMat map_matrix2 = map_matrix;

	cv2DRotationMatrix(center, degree, 1.0, &map_matrix2);

	map[2] += (width_rotate - width) / 2;

	map[5] += (height_rotate - height) / 2;

	Mat img_rotate;

	//对图像做仿射变换

	//CV_WARP_FILL_OUTLIERS - 填充所有输出图像的象素。

	//如果部分象素落在输入图像的边界外，那么它们的值设定为 fillval.

	//CV_WARP_INVERSE_MAP - 指定 map_matrix 是输出图像到输入图像的反变换，

	warpAffine(img, img_rotate, map_matrix, Size(width_rotate, height_rotate), INTER_LINEAR, BORDER_CONSTANT, Scalar());

	return img_rotate;

}

int main(int argc, char *argv[])

{
	static bool quit = false;
	static int degree = 0;

	std::string image = "E:\\opencv_material\\tian.jpg";

	Mat m_SrcImg;

	m_SrcImg = imread(image);

	namedWindow("原图像", CV_WINDOW_FULLSCREEN);

	imshow("原图像", m_SrcImg);

	while(true)
	{
		char Key = waitKey(0);

		switch (Key)
		{
		case 'n':
			degree += 90;
			break;
		case 'm':
			degree += 30;
			break;
		case 'b':
			degree -= 90;
			break;
		case 'q':
			quit = true;
			break;
		}
		Mat m_ResImg = rotateImage1(m_SrcImg, degree);

		namedWindow("旋转后图像", 1);

		imshow("旋转后图像", m_ResImg);

		if (quit)
			break;
	}
	destroyAllWindows();
	return 0;

}

#else
using namespace std;

int main(int argc, char** argv)
{
	string image = "E:\\opencv_material\\tian.jpg";
	cv::Mat srcImage = cv::imread(image);
	if (srcImage.empty())
	{
		cout << "can not load " << image << endl;
		return -1;
	}

	cv::Point2f srcTri[] = {
		cv::Point2f(0, 0),
		cv::Point2f(srcImage.cols-1, 0),
		cv::Point2f(0, srcImage.rows-1)
	};

	cv::Point2f dstTri[]= {
		cv::Point2f(srcImage.cols*0.f, srcImage.rows*0.33f),
		cv::Point2f(srcImage.cols*0.85f, srcImage.rows*0.25f),
		cv::Point2f(srcImage.cols*0.15f, srcImage.rows*0.7f)
	};

	//COMPUTE AFFINE MATRIX  计算仿射矩阵

	cv::Mat warp_mat = cv::getAffineTransform(srcTri, dstTri);
	cv::Mat dstImage, dstImage2;
	cv::warpAffine(
		srcImage,
		dstImage,
		warp_mat,
		srcImage.size(),
		cv::INTER_LINEAR,
		cv::BORDER_CONSTANT,
		cv::Scalar()
	);
	for (int i = 0; i < 3; ++i)
	{
		cv::circle(dstImage, dstTri[i], 5, cv::Scalar(255, 0, 255), -1, CV_AA);
	}

	//cv::imshow("Affine Transfoem Test", dstImage);
	cv::namedWindow("原图", cv::WINDOW_FULLSCREEN);
	cv::imshow("原图", srcImage);
	//cv::waitKey();

	for (int frame = 0; ; ++frame)
	{
		//COMPUTE ROTATION MATRIX  计算旋转矩阵

		char Key = cv::waitKey(0);
		static double angle;
		static bool quit = false;
		switch (Key)
		{
		case 'n':
			angle = frame * 90 % 360;
			break;
		case 'q':
			quit = true;
			break;
		}
		
		cv::Point2f center(srcImage.cols*0.5f, srcImage.rows*0.5f);
		//double scale = (cos((angle - 60) * CV_PI / 180) + 1.05)*0.8;
		double scale = 1.0;
		cv::Mat rot_mat = cv::getRotationMatrix2D(center, angle, scale);

		cv::namedWindow("Rotated Image", cv::WINDOW_FULLSCREEN);

		if (angle == 180 || angle == 0)
		{

			cv::warpAffine(
				srcImage,
				dstImage,
				rot_mat,
				srcImage.size(),
				cv::INTER_LINEAR,
				cv::BORDER_CONSTANT,
				cv::Scalar()
			);
		}
		else if (angle == 90 || angle == 270)
		{
			cv::warpAffine(
				srcImage,
				dstImage,
				rot_mat,
				srcImage.size(),
				cv::INTER_LINEAR, 
				cv::BORDER_CONSTANT,
				cv::Scalar()
			);
			
		}
		cv::imshow("Rotated Image", dstImage);
		if (quit)
			break;
	}
	cv::destroyAllWindows();
	return 0;
}

#endif