# Shapes
Allows a user to define a custom 2D shape and query its area and bounding box. There are two types of shapes: Convex and Concave polygons.

## Prerequisites
If using type `Shape::Polygon::Concave`, the points **MUST BE ORDERED** in a clockwise or anti-clockwise order starting with the first vertex.

Duplicate points are not supported. The shape will close itself.

## Constructing A New Shape
A new shape object can be constructed by calling either constructors:
```.cpp
  std::vector<Point> points
  {
    {-4,2},
    {-2,-2},
    {2,1},
    {0,4},
    {-3,4},
  };
  
  // Constructs a new concave type polygon with a given vector of points
  Shape shape(points, Shape::PolygonType::Concave);

  // Constructs a new convex type polygon with a given vector of points.
  Shape shape(points, Shape::PolygonType::Convex);
```

## Available Functions
```.cpp
// Check to make sure the shape is valid.
bool isValid(int& error);

// Returns the area of the shape.
double GetArea();

// Returns the bounding box as a set of ordered points starting at top left of box
// and continuing clockwise to each corner.
std::vector<Point> GetBoundingBox();

// Checks to see if a point is Included, Excluded, or on the Boundary of the bounding box.
QueryPointPosition QueryPoint(double x, double y);
```
