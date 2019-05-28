#ifndef SHAPE_H
#define SHAPE_H

#include <vector>
#include <utility>

typedef std::pair<double, double> Point;

class Shape
{
public:
  enum PolygonType
  {
    Convex,
    Concave
  };

  enum QueryPointPosition
  {
    Included,
    Excluded,
    Boundary
  };

  Shape(const std::vector<Point>& points, Shape::PolygonType type);

  bool isValid(int status);
  double GetArea();
  std::vector<Point> GetBoundingBox();
  QueryPointPosition QueryPoint(double x, double y);

private:
  std::vector<Point> m_points;
  std::vector<Point> m_bounding_box;
  double m_Xmax;
  double m_Xmin;
  double m_Ymax;
  double m_Ymin;
  double m_area;
  PolygonType m_type;

  void CalculateBoundingBox(std::vector<Point>& bounding_box);
  void CalculateArea(double& surfaceArea);
  void SortPoints(std::vector<Point>& points);
};

#endif