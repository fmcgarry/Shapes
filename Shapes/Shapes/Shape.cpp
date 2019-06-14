#include "Shape.h"
#include <algorithm>

/***
 *  PURPOSE:  Construct a new Shape object.
 *  WHERE:    points  =   A vector containing Point types.
 *            type    =   Convex or Concave type polygon.
 *
 *  NOTE:     A Concave type polygon REQUIRES the points to be given in a
 *            clock-wise or anticlock-wise order from the first vertex
 *            to the last.
 ***/
Shape::Shape(const std::vector<Point>& points, Shape::PolygonType type)
{
  m_points = points;
  m_bounding_box;
  m_type = type;
  int error{};

  if (isValid(error))
  {
    if (m_type == Convex)
    {
      SortPoints(m_points);
    }

    CalculateBoundingBox(m_bounding_box);
    CalculateArea(m_area);
  }
}

/***
 *  PURPOSE:  Calculate the area of the shape.
 *  WHERE:    shapeArea = The calculated area
 *
 *  RETURN:   Void.
 ***/
void Shape::CalculateArea(double& shapeArea)
{
  double area{};
  int j{};

  // Add the return to origin point.
  m_points.push_back(m_points[0]);

  // Shoelace formula
  for (size_t i = 1; i < m_points.size(); ++i)
  {
    area += (m_points[j].first * m_points[i].second) - (m_points[j].second * m_points[i].first);
    j = i; // j is the point before i.
  }

  shapeArea = abs(area) * .5;

  // Remove the return to origin point.
  m_points.pop_back();
}

/***
 *  PURPOSE:  Calculate the bounding box.
 *  WHERE:    bounding_box  =   A vector containing Point types.
 *
 *  RETURN:   Void.
 ***/
void Shape::CalculateBoundingBox(std::vector<Point>& bounding_box)
{
  m_Xmax = m_points[0].first;
  m_Xmin = m_points[0].first;
  m_Ymax = m_points[0].second;
  m_Ymin = m_points[0].second;

  // Find X max
  for (size_t i = 1; i < m_points.size(); ++i)
  {
    if (m_points[i].first > m_Xmax)
    {
      m_Xmax = m_points[i].first;
    }
  }

  // Find X min
  for (size_t i = 1; i < m_points.size(); ++i)
  {
    if (m_points[i].first < m_Xmin)
    {
      m_Xmin = m_points[i].first;
    }
  }

  // Find Y max
  for (size_t i = 1; i < m_points.size(); ++i)
  {
    if (m_points[i].second > m_Ymax)
    {
      m_Ymax = m_points[i].second;
    }
  }

  // Find Y min
  for (size_t i = 1; i < m_points.size(); ++i)
  {
    if (m_points[i].second < m_Ymin)
    {
      m_Ymin = m_points[i].second;
    }
  }

  // Create points in a clock-wise manner starting at top left.
  bounding_box =
  {
    {m_Xmin, m_Ymax},
    {m_Xmax, m_Ymax},
    {m_Xmax, m_Ymin},
    {m_Xmin, m_Ymin}
  };
}

/***
 *  PURPOSE:  Return the area of the shape.
 *
 *  RETURN:   A double value that represents the area of the shape.
 ***/
double Shape::GetArea()
{
  return m_area;
}

/***
 *  PURPOSE:  Return the bounding box of the shape.
 *  RETURN:   A vector of Point types that represent the four vertices of the bounding box.
 *
 *  NOTE:     Points will be returned in a clock-wise order starting
 *            at the top left position.
 ***/
std::vector<Point> Shape::GetBoundingBox()
{
  return m_bounding_box;
}

/***
 *  PURPOSE:  Query a point to see its location relative to the bounding box.
 *  WHERE:    x = The point's x coordinate.
 *            y = The point's y coordinate.
 *
 *  RETURN:   Boundary  = The point resides on the boundary of the bounding box.
 *            Included =  The point resides inside the bounding box.
 *            Excluded =  The point resides outside of the bounding box.
 ***/
Shape::QueryPointPosition Shape::QueryPoint(double x, double y)
{
  if (x <= m_Xmax && y <= m_Ymax && x >= m_Xmin && y >= m_Ymin)
  {
    if (x == m_Xmax || y == m_Ymax)
    {
      return Shape::Boundary;
    }

    return Shape::Included;
  }

  return Shape::Excluded;
}

/***
 *  PURPOSE:  Sort points in a clock-wise manner.
 *  WHERE:    points  =   A vector containing Point types.
 *
 *  RETURN:   Void.
 *  NOTE:     m_type MUST BE Convex to be sorted.
 ***/
void Shape::SortPoints(std::vector<Point>& points)
{
  // Sort points by x value, then y value.
  std::sort(points.begin(), points.end());

  // Get the min x value point and max x value point.
  Point* xMinPoint = &points[0];
  Point* xMaxPoint = &points[points.size() - 1];

  // Create line between two points
  auto m = (xMaxPoint->second - xMinPoint->second) / (xMaxPoint->first - xMinPoint->first);
  auto b = xMinPoint->second - (m * xMinPoint->first);

  // Create two new vectors.
  std::vector<Point> pointsAboveLine = { *xMinPoint };
  std::vector<Point> pointsBelowLine = { *xMaxPoint };

  // Check each point for its position above or below the line. Add to relevant vector.
  for (size_t i = 1; i < points.size() - 1; ++i)
  {
    auto y = m * points[i].first + b;

    if (points[i].second > y)
    {
      pointsAboveLine.push_back(points[i]);
    }
    else
    {
      pointsBelowLine.push_back(points[i]);
    }
  }

  // Sort the vectors by ascending x values.
  std::sort(pointsAboveLine.begin(), pointsAboveLine.end());
  std::sort(pointsBelowLine.begin(), pointsBelowLine.end());

  // Clear old point vector.
  points.clear();

  // Append two vectors together.
  points.insert(points.end(), pointsAboveLine.begin(), pointsAboveLine.end());
  points.insert(points.end(), pointsBelowLine.rbegin(), pointsBelowLine.rend());
}

/***
 *  PURPOSE:  Check to make sure the shape is a valid shape.
 *  WHERE:    error  = The return code. 0 if shape is valid.
 *
 *  RETURN:   True if shape is valid, false otherwise.
 ***/
bool Shape::isValid(int& error)
{
  // Make sure no points are duplicated. (Shoelace does not work)
  //   - Maybe split shape into 2 shapes, do shoelace, then add together?
  // Make sure there are more than 2 points. (Has to be at least a triangle)

  bool isValidShape = m_points.size() > 2;
  bool isPointDuplicated{ CheckForDuplicates() };

  if (!isValidShape)
  {
    error = 1;
    return false;
  }

  if (isPointDuplicated)
  {
    error = 2;
    return false;
  }

  error = 0;
  return true;
}

/***
 *  PURPOSE:  Check the point collection for duplicates.
 *
 *  RETURN:   True if there are duplicates, false otherwise.
 ***/
bool Shape::CheckForDuplicates()
{
  int duplicates{ 1 };

  for (size_t i = 0; i < m_points.size(); ++i)
  {
    duplicates = std::count(m_points.begin(), m_points.end(), m_points[i]);

    if (duplicates != 1)
    {
      return true;
    }
  }

  return false;
}