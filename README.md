**ekdtree** implements Erlang interface to [CGAL](http://www.cgal.org/)
[k-d tree](http://en.wikipedia.org/wiki/K-d_tree) and 
[k-NN search](http://en.wikipedia.org/wiki/K-nearest_neighbor_algorithm)
algorithm.

## Usage

**ekdtree:new/1** constructs tree from points which are 4-tuples of integer Id, and float XYZ coordinates:

```erlang
Points = [{0, 10.0, 10.0, 10.0}, {1, 10.0, 10.0, -10.0}, {2, 10.0,-10.0, 10.0}],
{ok, Tree} = ekdtree:new(Points).
```

**ekdtree:size/1** returns a number of points:

```erlang
3 = ekdtree:size(Tree).
```

**ekdtree:search/3** returns an ordered list of **K** nearest neighbors along with
a transformed (squared) distance(s):

```erlang
Knn = ekdtree:search(Tree, {5.0, 5.0, 10.0}, 2), 
[{0, 50.0}, {2, 250.0}])] = Knn.
```
