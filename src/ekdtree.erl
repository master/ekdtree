-module(ekdtree).
-author('oleg.smirnov@gmail.com').

-export([new/1, search/3, size/1]).
-on_load(init/0).

-type point() :: {integer(), float(), float(), float()}.

-define(NOT_LOADED, not_loaded(?LINE)).

-spec init() -> ok | {error, any()}.
init() ->
    PrivDir = case code:priv_dir(?MODULE) of
                  {error, _} ->
                      EbinDir = filename:dirname(code:which(?MODULE)),
                      AppPath = filename:dirname(EbinDir),
                      filename:join(AppPath, "priv");
                  Path -> 
                      Path
              end,
    erlang:load_nif(filename:join(PrivDir, "ekdtree"), 0).

not_loaded(Line) ->
    erlang:nif_error({not_loaded, [{module, ?MODULE}, {line, Line}]}).

-spec new([point()]) -> {ok, reference()}.
new(_Points) ->
    ?NOT_LOADED.

-spec search(reference(), {float(), float(), float()}, integer()) -> [point()].
search(_Ref, _Point, _K) ->
    ?NOT_LOADED.

-spec size(reference()) -> integer().
size(_Ref) ->
    ?NOT_LOADED.
