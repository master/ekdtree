REBAR=`PATH=".:$$PATH" which rebar`

.PHONY: all test clean build

all: compile

compile:
	@$(REBAR) compile
test:	compile
	@$(REBAR) skip_deps=true eunit
clean:
	@$(REBAR) clean
dialyze: deps compile
	dialyzer --src -r src -I ebin \
	-pa deps/yaws \
	-Wunmatched_returns \
	-Werror_handling \
	-Wno_undefined_callbacks \
	-Wrace_conditions
