// Shapes.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Shape.h"

int main()
{
  double x{};
  double y{};

  std::vector<Point> points
  {
    {-4,2},
    {-2,-2},
    {2,1},
    {0,4},
    {-3,4},
  };

  Shape shape(points, Shape::PolygonType::Convex);

  int error{};

  if (shape.isValid(error))
  {
    std::cout << "Area: " << shape.GetArea() << std::endl;

    std::vector<Point> boundingBox = shape.GetBoundingBox();

    std::cout << "Bounding Box:" << std::endl;
    for (size_t i = 0; i < boundingBox.size(); ++i)
    {
      std::cout << "(" << boundingBox[i].first << "," << boundingBox[i].second << ")" << std::endl;
    }

    std::cout << std::endl << std::endl;

    std::cout << "Enter a point to query:" << std::endl;
    std::cout << "X = ";
    std::cin >> x;
    std::cout << "Y = ";
    std::cin >> y;

    auto result = shape.QueryPoint(x, y);

    std::string printResult{};

    switch (result)
    {
    case Shape::Included:
      printResult = "Included";
      break;
    case Shape::Excluded:
      printResult = "Exluded";
      break;
    case Shape::Boundary:
      printResult = "Boundary";
      break;
    default:
      break;
    }

    std::cout << "Point is " + printResult << std::endl;
  }
  else
  {
    (Shape::Error)error;
    std::string error_message{};

    switch (error)
    {
    case Shape::Error::NOT_VALID_SHAPE:
      error_message = "Shape is not valid.";
      break;

    case Shape::Error::DUPLICATE_POINT:
      error_message = "There is a duplicate point.";
      break;

    default:
      error_message = "Unknown Error.";
      break;
    }

    std::cout << error_message << std::endl;
  }
}