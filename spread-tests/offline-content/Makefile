required_snaps=hello-world snapd-hacker-toolbelt ubuntu-core core

.PHONY: all
all: $(addsuffix .snap,$(required_snaps)) $(addprefix packaging.,ubuntu)

.PHONY: clean
clean:
	rm -f *.snap

latest_revision_of_snap=$(lastword $(sort $(wildcard /var/lib/snapd/snaps/$(1)_*.snap)))

.SECONDEXPANSION:

# XXX: core.snap cannot be installed when you have ubuntu-core, get it directly from cdimage maybe

%.snap: $$(call latest_revision_of_snap,%)
	sudo ln $< $@
	sudo chown $(shell id -u).$(shell id -g) $@

packaging.ubuntu.git:
	git clone --mirror https://git.launchpad.net/snap-confine packaging.ubuntu.git

xenial-amd64.tar.gz: /var/lib/sbuild/xenial-amd64.tar.gz
	sudo ln $< $@
	sudo chown $(shell id -u).$(shell id -g) $@

yakkety-amd64.tar.gz: /var/lib/sbuild/yakkety-amd64.tar.gz
	sudo ln $< $@
	sudo chown $(shell id -u).$(shell id -g) $@

# NOTE: This assumes a working sbuild system
/var/lib/sbuild/xenial-amd64.tar.gz:
	sudo http_proxy=$(APT_PROXY) sbuild-createchroot \
		--include=eatmydata \
		--make-sbuild-tarball=/var/lib/sbuild/xenial-amd64.tar.gz \
		--components=main,universe \
		xenial $$(mktemp -d) http://archive.ubuntu.com/ubuntu

/var/lib/sbuild/yakkety-amd64.tar.gz:
	sudo http_proxy=$(APT_PROXY) sbuild-createchroot \
		--include=eatmydata \
		--make-sbuild-tarball=/var/lib/sbuild/yakkety-amd64.tar.gz \
		--components=main,universe \
		yakkety $$(mktemp -d) http://archive.ubuntu.com/ubuntu
