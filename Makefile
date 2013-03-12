PROJECT=ekdtree
VERSION=`cat version`
RELEASE=`cat release`
REVISION=`git --no-pager log --max-count=1 --format=format:%H`

REBAR=rebar

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
spec:
	cat opensuse.spec.in | \
		sed "s/{{VERSION}}/$(VERSION)/" | \
		sed "s/{{RELEASE}}/$(RELEASE)/" | \
		sed "s/{{REVISION}}/$(REVISION)/" \
		> strikead-$(PROJECT).spec
