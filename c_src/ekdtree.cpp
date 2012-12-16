#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/basic.h>
#include <CGAL/Search_traits.h>
#include <CGAL/Search_traits_2.h>
#include <CGAL/Search_traits_3.h>
#include <CGAL/Search_traits_adapter.h>
#include <CGAL/Orthogonal_k_neighbor_search.h>
#include <boost/iterator/zip_iterator.hpp>
#include <utility>

#include "ekdtree.h"
#include "erl_nif_compat.h"

typedef long Key;

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_3 Point;
typedef CGAL::Search_traits_3<Kernel> Traits_base;

typedef boost::tuple<Point, Key> Point_key;

class Point_pmap 
{
public:
  typedef Point value_type;
  typedef const value_type& reference;
  typedef const Point_key& key_type;
  typedef boost::readable_property_map_tag category;
};

Point_pmap::reference get(Point_pmap, Point_pmap::key_type p) 
{
  return boost::get<0>(p);
}

typedef CGAL::Search_traits_adapter<Point_key, Point_pmap, Traits_base> Traits;
typedef CGAL::Orthogonal_k_neighbor_search<Traits> K_neighbor_search;
typedef K_neighbor_search::Tree Tree;
typedef K_neighbor_search::Distance Distance;

static ErlNifResourceType* KDTREE1;
static ErlNifResourceType* KDTREE2;
static ErlNifResourceType* KDTREE3;

typedef struct { Tree* tree; } thandle3;

extern "C"
{
  ERL_NIF_TERM ekdtree_new(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]);
  ERL_NIF_TERM ekdtree_search(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]);
  ERL_NIF_TERM ekdtree_size(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]);

  void ekdtree_dtor(ErlNifEnv* env, void* arg);

  int on_load(ErlNifEnv* env, void** priv_data, ERL_NIF_TERM load_info);

  static ErlNifFunc nif_funcs[] = 
    {
      {"new",           1, ekdtree_new},
      {"search",        3, ekdtree_search},
      {"size",          1, ekdtree_size},
    };
  
  ERL_NIF_INIT(ekdtree, nif_funcs, &on_load, NULL, NULL, NULL);
};

ERL_NIF_TERM ekdtree_new(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
  unsigned int list_size;
  int tuple_size;
  ERL_NIF_TERM head, tail;
  const ERL_NIF_TERM *tuple;
  long point_id;
  double point_x, point_y, point_z;

  ERL_NIF_TERM list = argv[0];

  if (!enif_get_list_length(env, list, &list_size)) 
    {
      return enif_make_badarg(env);    
    }

  std::vector<Point> points;
  std::vector<Key> indices;

  while (enif_get_list_cell(env, list, &head, &tail)) 
    {
      if (enif_get_tuple(env, head, &tuple_size, &tuple) &&
          tuple_size == 4 &&
          enif_get_long(env, tuple[0], &point_id) &&
          enif_get_double(env, tuple[1], &point_x) &&
          enif_get_double(env, tuple[2], &point_y) &&
          enif_get_double(env, tuple[3], &point_z)) 
        {
          points.push_back(Point(point_x, point_y, point_z));      
          indices.push_back(point_id);
          list = tail;
        } 
      else 
        {
          return enif_make_badarg(env);
        }
    }

  thandle3* handle = (thandle3*)enif_alloc_resource_compat(env, KDTREE3,
                                                           sizeof(thandle3));

  handle->tree = new Tree(boost::make_zip_iterator(boost::make_tuple(points.begin(), indices.begin())),
                          boost::make_zip_iterator(boost::make_tuple(points.end(), indices.end())));

  points.clear();
  indices.clear();

  Point query(0.0, 0.0, 0.0);
  K_neighbor_search search(*handle->tree, query, 1);

  ERL_NIF_TERM result = enif_make_resource(env, handle);
  enif_release_resource_compat(env, handle);

  return enif_make_tuple2(env, enif_make_atom(env, "ok"), result);
}

ERL_NIF_TERM ekdtree_search(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
  thandle3* handle;

  double point_x, point_y, point_z;
  long K;
  int tuple_size;
  const ERL_NIF_TERM *tuple;

  if (enif_get_resource(env, argv[0], KDTREE3, (void**)&handle) &&
      enif_get_tuple(env, argv[1], &tuple_size, &tuple) &&
      tuple_size == 3 &&
      enif_get_double(env, tuple[0], &point_x) &&
      enif_get_double(env, tuple[1], &point_y) &&
      enif_get_double(env, tuple[2], &point_z) &&
      enif_get_long(env, argv[2], &K))
    {
      Point query(point_x, point_y, point_z);

      K_neighbor_search search(*handle->tree, query, K);

      ERL_NIF_TERM list = enif_make_list(env, 0);
      
      Distance tr_dist;
  
      for(K_neighbor_search::iterator it = search.begin(); it != search.end(); it++) 
        {
          long point_id = boost::get<1>(it->first);
          double distance = it->second;

          ERL_NIF_TERM id = enif_make_long(env, point_id);
          ERL_NIF_TERM dist = enif_make_double(env, distance);

          ERL_NIF_TERM head = enif_make_tuple2(env, id, dist);

          list = enif_make_list_cell(env, head, list);
        }

      ERL_NIF_TERM result;
      enif_make_reverse_list(env, list, &result);

      return result;
    }
  else
    {
      return enif_make_badarg(env);
    }
}

ERL_NIF_TERM ekdtree_size(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
  thandle3* handle;
  
  if (enif_get_resource(env, argv[0], KDTREE3, (void**)&handle))
    {
      long result = handle->tree->size();
      return enif_make_long(env, result);
    }
  else
    {
      return enif_make_badarg(env);
    }
}

void ekdtree_dtor(ErlNifEnv* env, void* arg)
{
  thandle3* handle = (thandle3*)arg;

  delete handle->tree;
}

int on_load(ErlNifEnv* env, void** priv_data, ERL_NIF_TERM load_info)
{
  ErlNifResourceFlags flags = 
    (ErlNifResourceFlags)(ERL_NIF_RT_CREATE | ERL_NIF_RT_TAKEOVER);

  KDTREE1 = enif_open_resource_type_compat(env, "kdtree1", &ekdtree_dtor,
                                           flags, 0);
  KDTREE2 = enif_open_resource_type_compat(env, "kdtree2", &ekdtree_dtor,
                                           flags, 0);
  KDTREE3 = enif_open_resource_type_compat(env, "kdtree3", &ekdtree_dtor,
                                           flags, 0);
  return 0;
}
