-module(ekdtree_tests).
-author('oleg.smirnov@gmail.com').

-include_lib("eunit/include/eunit.hrl").

-define(POINTS, [{0,  10.0, 10.0, 10.0},
                 {1,  10.0, 10.0,-10.0},
                 {2,  10.0,-10.0, 10.0},
                 {3,  10.0,-10.0,-10.0},
                 {4, -10.0, 10.0, 10.0},
                 {5, -10.0, 10.0,-10.0},
                 {6, -10.0,-10.0, 10.0},
                 {7, -10.0,-10.0,-10.0}]).

size_test_() ->
    {ok, Tree} = ekdtree:new(?POINTS),
    Size = ekdtree:size(Tree),
    [?_assertEqual(Size, length(?POINTS))].

search_test_() ->
    {ok, Tree} = ekdtree:new(?POINTS),
    [?_assertEqual(ekdtree:search(Tree, {10.0, 10.0, 10.0}, 1), [{0, 0.0}]),
     ?_assertEqual(ekdtree:search(Tree, {10.0, 10.0, 11.0}, 1), [{0, 1.0}]),
     ?_assertEqual(ekdtree:search(Tree, {0.0, 0.0, 10.0}, 4),
                   [{2, 200.0}, {0, 200.0}, {4, 200.0}, {6, 200.0}]),
     ?_assertEqual(ekdtree:search(Tree, {5.0, 5.0, 10.0}, 3),
                   [{0, 50.0}, {2, 250.0}, {4, 250.0}])].

     
     
