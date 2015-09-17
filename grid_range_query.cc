// -*- mode: c++ -*-
// compile: [c++ | g++] -Wall -std=c++11 grid_range_query.cc

#include <iostream>
#include <algorithm>
#include <tuple>
#include <utility>
#include <vector>
#include <cmath>
#include <cassert>
#include <valhalla/midgard/aabb2.h>
#include <valhalla/midgard/pointll.h>
#include <valhalla/midgard/linesegment2.h>


using GraphId = uint32_t;
using Point = valhalla::midgard::PointLL;
using LineSegment = valhalla::midgard::LineSegment2<Point>;
using BoundingBox = valhalla::midgard::AABB2<Point>;




class GridRangeQuery
{
 public:
  GridRangeQuery(const BoundingBox& bbox, float cell_width, float cell_height) {
    Init(bbox, cell_width, cell_height);
  }


  // Divide the grid into num_cols by num_rows cells
  GridRangeQuery(const BoundingBox& bbox, uint32_t num_cols, uint32_t num_rows) {
    Init(bbox, bbox.Width() / num_cols, bbox.Height() / num_rows);
  }


  void Init(const BoundingBox& bbox, float cell_width, float cell_height) {
    bbox_ = bbox;
    cell_width_ = cell_width;
    cell_height_ = cell_height;
    num_rows_ = ceil(bbox_.Width() / cell_width);
    num_cols_ = ceil(bbox_.Height() / cell_height);
    items_.resize(num_cols_ * num_rows_);
  }


  // Get bbox of the grid
  const BoundingBox bbox() const {
    return bbox_;
  }

  std::pair<int, int> GridCoordinates(const Point &p) {
    float dx = p.x() - bbox_.minx();
    float dy = p.y() - bbox_.miny();
    return { int(dx / cell_width_), int(dy / cell_height_) };
  }

  BoundingBox CellBoundingBox(int i, int j) {
    return BoundingBox(
        bbox_.minx() + i * cell_width_,
        bbox_.miny() + j * cell_height_,
        bbox_.minx() + (i + 1) * cell_width_,
        bbox_.miny() + (j + 1) * cell_height_
      );
  }


  // Index a line segment into the grid
  void AddLineSegment(const GraphId edgeid, const LineSegment& segment) {
    // For now assume the segment is entirely inside the box
    assert(bbox_.Contains(segment.a()) && bbox_.Contains(segment.b()));

    Point start = segment.a();
    Point end = segment.b();
    Point current_point = start;

    while (Unlerp(start, end, current_point) < 1.0) {
      std::pair<int, int> current_cell = GridCoordinates(current_point);
      items_[current_cell.first + current_cell.second * num_rows_].push_back(edgeid);

    }
  }


  // Query all edges that intersects with the range
  std::vector<GraphId> Query(const BoundingBox& range) const {
    return {};
  }


  // Return t such that p = a + t * (b - a)
  float Unlerp(const Point &a, const Point &b, const Point &p) {
    if (std::abs(b.x() - a.x()) > std::abs(b.y() - a.y())) {
      return (p.x() - a.x()) / (b.x() - a.x());
    } else {
      return (p.y() - a.y()) / (b.y() - a.y());
    }
  }


  std::vector<Point>
  CellLineSegmentIntersections(int i, int j, const LineSegment &segment) {
    std::vector<Point> intersects;

    BoundingBox cell = CellBoundingBox(i, j);
    LineSegment e1({cell.minx(), cell.miny()}, {cell.maxx(), cell.miny()});
    LineSegment e2({cell.maxx(), cell.miny()}, {cell.maxx(), cell.maxy()});
    LineSegment e3({cell.maxx(), cell.maxy()}, {cell.minx(), cell.maxy()});
    LineSegment e4({cell.minx(), cell.maxy()}, {cell.minx(), cell.miny()});

    Point intersect;
    if (segment.Intersect(e1, intersect)) intersects.push_back(intersect);
    if (segment.Intersect(e2, intersect)) intersects.push_back(intersect);
    if (segment.Intersect(e3, intersect)) intersects.push_back(intersect);
    if (segment.Intersect(e4, intersect)) intersects.push_back(intersect);

    return intersects;
  }


 private:
  BoundingBox bbox_;
  float cell_width_;
  float cell_height_;
  size_t num_rows_;
  size_t num_cols_;
  std::vector<std::vector<GraphId> > items_;
};


void TestGridTools()
{
  BoundingBox bbox(0, 0, 100, 100);
  // Divide the grid into 100x100 cells
  GridRangeQuery grid(bbox, 100u, 100u);

  auto c = grid.GridCoordinates({12.5, 13.7});
  assert(c.first == 12 && c.second == 13);

  auto intersects = grid.CellLineSegmentIntersections(2, 3, LineSegment({2.5, 3.5}, {10, 3.5}));
  assert(intersects.size() == 1);
  assert(intersects[0].x() == 3 && intersects[0].y() == 3.5);
}

void TestGridRangeQuery()
{
  BoundingBox bbox(0, 0, 100, 100);
  // Divide the grid into 100x100 cells
  GridRangeQuery grid(bbox, 100u, 100u);


  // grid.AddLineSegment(0, LineSegment({0, 0}, {0.5, 0.5}));

  // auto edges = grid.Query(BoundingBox(0, 0, 0.5, 0.5));
  // assert(edges.size() == 1 && edges[0] == 0);

  // edges = grid.Query(BoundingBox(0.6, 0.6, 1, 1));
  // assert(edges.empty());

  // TODO more tests
}


int main(int argc, char *argv[])
{
  TestGridTools();
  TestGridRangeQuery();
  return 0;
}
