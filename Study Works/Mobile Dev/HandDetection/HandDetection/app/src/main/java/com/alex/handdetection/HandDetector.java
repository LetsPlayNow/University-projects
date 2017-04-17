package com.alex.handdetection;

import org.opencv.core.*;
import org.opencv.imgcodecs.Imgcodecs;
import org.opencv.imgproc.*;
import java.util.List;
import java.util.ArrayList;
import org.opencv.utils.Converters;


public class HandDetector
{
    class Params
    {
        int area;
        int r;
        int step;
        double cosThreshold;
        double equalThreshold;
    };

    public HandDetector(){
        param = new Params();
    };

    void detect(Mat mask, List<Hand> hands)
    {
        hands.clear();
        ArrayList<MatOfPoint> contours = new ArrayList<MatOfPoint>();
        Mat hierarchy = new Mat();
        int mysize;

        Imgproc.findContours(mask.clone(), contours, hierarchy, Imgproc.RETR_EXTERNAL, Imgproc.CHAIN_APPROX_NONE, new Point(0, 0));
        if(!contours.isEmpty())
        {
            for(int i=0; i<contours.size(); i++)
            {
                if(Imgproc.contourArea(contours.get(i))>param.area)
                {
                    Hand tmp = new Hand();
                    Moments m=Imgproc.moments(contours.get(i));
                    tmp.center.x=m.m10/m.m00;
                    tmp.center.y=m.m01/m.m00;

                    mysize = (int) contours.get(i).toList().size();
                    for(int j = 0; j < mysize; j+= param.step ) // or height?
                    {
                        double cos0 = angle (contours.get(i).toList(), j, param.r);

                        if ((cos0 > 0.5)&&(j+param.step<mysize)) // or height?
                        {
                            double cos1 = angle (contours.get(i).toList(), j - param.step, param.r);
                            double cos2 = angle (contours.get(i).toList(), j + param.step, param.r);
                            double maxCos = Math.max(Math.max(cos0, cos1), cos2);
                            boolean equal = isEqual (maxCos , cos0);
                            int z = rotation (contours.get(i).toList(), j, param.r);
                            if (equal == true && z<0)
                            {
                                tmp.fingers.add(contours.get(i).toList().get(j));
                            }
                        }
                    }
                    tmp.contour = new ArrayList<Point>(contours.get(i).toList());
                    hands.add(tmp);
                }
            }
        }
    }

    void setParams(Params p)
    {
        param.area = p.area;
        param.cosThreshold = p.cosThreshold;
        param.equalThreshold = p.equalThreshold;
        param.r = p.r;
        param.step = p.step;
    }

    private Params param;
    int rotation(List<Point> contour, int pt, int r)
    {
        int size = contour.size();
        Point p0=(pt>0)?contour.get(pt%size):contour.get(size-1+pt);
        Point p1=contour.get((pt+r)%size);
        Point p2=(pt>r)?contour.get(pt-r):contour.get(size-1-r);

        double ux=p0.x-p1.x;
        double uy=p0.y-p1.y;
        double vx=p0.x-p2.x;
        double vy=p0.y-p2.y;
        return (int)(ux*vy - vx*uy);
    }
    double angle(List<Point> contour, int pt, int r)
    {
        int size = contour.size();
        Point p0=(pt>0)?contour.get(pt%size):contour.get(size-1+pt);
        Point p1=contour.get((pt+r)%size);
        Point p2=(pt>r)?contour.get(pt-r):contour.get(size-1-r);

        double ux=p0.x-p1.x;
        double uy=p0.y-p1.y;
        double vx=p0.x-p2.x;
        double vy=p0.y-p2.y;
        return (ux*vx + uy * vy) / Math.sqrt((ux * ux + uy * uy) *(vx*vx + vy*vy));
    }

    boolean isEqual(double a, double b)
    {
        return Math.abs(a - b) <= param.equalThreshold;
    }


    void drawHands(Mat image, List<Hand> hands)
    {
        int size = hands.size();
        ArrayList<ArrayList<Point>> c = new ArrayList<ArrayList<Point>>();
        for(int i = 0; i<size; i++)
        {
            c.clear();
            c.add(hands.get(i).contour);
            Imgproc.circle(image, hands.get(i).center, 20, new Scalar(0, 255, 0), 2);
            int fingersSize = hands.get(i).fingers.size();
            for(int j = 0; j < fingersSize; j++)
            {
                Imgproc.circle(image, hands.get(i).fingers.get(j), 10, new Scalar(0, 0, 255), 2);
                Imgproc.line(image, hands.get(i).center, hands.get(i).fingers.get(j), new Scalar(0, 0, 255), 4);
            }
        }
    }

    public Mat getImageWithHands(Mat in)
    {
        Params p = new Params();
        p.area=3000;
        p.cosThreshold=0.3;
        p.equalThreshold=1e-7;
        p.r=40;
        p.step=15;

        setParams(p);

        ArrayList<Hand> hands = new ArrayList();
        Mat depthMap = new Mat();
        Mat bgrImage = new Mat();
        in.clone().convertTo(depthMap, CvType.CV_8UC1);
        in.clone().convertTo(bgrImage, CvType.CV_32FC1);

        // Mat bgrImage = new Mat(); it's in argument of this method

        //Mat tmp = new Mat();
        Mat tmp = new Mat();
        Imgproc.cvtColor(depthMap.clone(), tmp, Imgproc.COLOR_GRAY2BGR); // CV_GRAY_TO_BGR
        Mat depthMapCopy = new Mat();
        Imgproc.threshold(depthMap, depthMapCopy, 60, 255, Imgproc.THRESH_BINARY);

        detect(depthMapCopy, hands);

        if(!hands.isEmpty())
        {
            //drawHands(tmp, hands);
            drawHands(tmp, hands);
        }

        return tmp;
    }
};
