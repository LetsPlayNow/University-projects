package com.alex.handdetection;
import org.opencv.core.Point;
import java.util.ArrayList;

public class Hand
{
    Hand()
    {
        fingers = new ArrayList<Point>();
        center  = new Point();
        contour = new ArrayList<Point>();
    };
    ArrayList<Point> fingers;
    Point center;
    ArrayList<Point> contour;
};


