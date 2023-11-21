#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

struct MouseParams
{
    Mat frame;
    Mat input;
    vector<Point2f> in, out;

    MouseParams() : frame(), in(), out() {}
};
static void onMouse(int event, int x, int y, int, void *param)
{
    MouseParams *mp = (MouseParams *)param;
    Mat frame = mp->frame;
    Mat original = frame.clone();
    if (event == EVENT_LBUTTONDOWN)
    {
        mp->in.push_back(Point2f(x, y));
        if (mp->in.size() == 4)
        {
            Mat homo_mat = getPerspectiveTransform(mp->in, mp->out);
            warpPerspective(frame, frame, homo_mat, Size(800, 600));
            imshow("drain", frame);
        }
        else
        {
            cout << mp->in.size() << endl;
            for (size_t i = 0; i < mp->in.size(); i++)
            {
                circle(frame, mp->in[i], 3, Scalar(0, 0, 255), 5);
            }

            imshow("cleanDrain", frame);
        }
    }
    if (event == EVENT_RBUTTONDOWN)
    {
        mp->in.clear();
        imshow("cleanDrain", frame);
    }
}
int main()
{
    Mat cleanDrain = imread("assets/cleanRoadDrain.jpeg", 1);

    MouseParams mp;
    Mat frame = cleanDrain.clone();
    imshow("cleanDrain", frame);
    mp.out.push_back(Point2f(0, 0));
    mp.out.push_back(Point2f(800, 0));
    mp.out.push_back(Point2f(800, 600));
    mp.out.push_back(Point2f(0, 600));
    mp.frame = frame;
    setMouseCallback("cleanDrain", onMouse, (void *)&mp);

    waitKey(0);
    return 0;
}