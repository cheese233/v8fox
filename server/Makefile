TARGET_V8 = x64.release.sample

all: server

depot_tools:
	git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git

v8/README.md: 
	git submodule update --init --recursive

define export-depot_tools-PATH
export PATH=$(shell pwd)/depot_tools:$$PATH
endef

.ONESHELL:
v8/buildtools: v8/README.md depot_tools
	$(export-depot_tools-PATH)
	gclient config https://github.com/v8/v8.git
	$(INSTALL_DEPS)
	gclient sync

.ONESHELL:
v8/out.gn/$(TARGET_V8): v8/README.md
	$(export-depot_tools-PATH)
	cd v8
	tools/dev/v8gen.py $(TARGET_V8)
	gn args out.gn/$(TARGET_V8)
	ninja -C out.gn/$(TARGET_V8) v8_monolith

v8: v8/out.gn/$(TARGET_V8)


libuv/README.md: 
	git submodule update --init --recursive

.ONESHELL:
libuv/Makefile: libuv/README.md
	cd libuv
	./autogen.sh
	./configure

libuv/.libs/libuv.a: libuv/Makefile
	$(MAKE) --directory=libuv

libuv: libuv/.libs/libuv.a

CXXFLAGS += -Iv8 -Iv8/include -Iv8/out.gn/$(TARGET_V8)/gen -Ilibuv/include \
	-fno-rtti -lv8_monolith -lv8_libbase -lv8_libplatform -ldl -Lv8/out.gn/$(TARGET_V8)/obj/ libuv/.libs/libuv.a \
	-pthread -std=c++20 -DV8_COMPRESS_POINTERS -DV8_ENABLE_SANDBOX

server: v8 libuv main.cpp
	$(CXX) main.cpp -o server $(CXXFLAGS)

.PHONY:
clean:
	$(RM) *.o
	$(RM) server
	$(RM) v8fox_server.json


define INSTALL-WINDOWS
cp ./v8fox_server.windows.json ./v8fox_server.json
reg add HKEY_LOCAL_MACHINE\\SOFTWARE\\Mozilla\\NativeMessagingHosts\\v8fox_server /d $(shell pwd)\\v8fox_server.json
endef
INSTALL-LINUX = echo '$(subst __SERVER_PATH__,$(shell pwd)/server,$(shell cat ./v8fox_server.linux.json) )' > $(1)
INSTALL-DARWIN = echo '$(subst __SERVER_PATH__,$(shell pwd)/server,$(shell cat ./v8fox_server.darwin.json) )' > $(1)

ifeq ($(LANG),) #Windows don't have $LANG

.PHONY:
install: server
	$(INSTALL-WINDOWS)

else

OS := $(shell uname)
ifeq ($(OS),Darwin)

INSTALL_PATH := /Library/Application Support/Mozilla/NativeMessagingHosts/v8fox_server.json
.PHONY:
install: server
	$(call INSTALL-DARWIN,$(INSTALL_PATH))

else ifeq ($(OS),Windows_NT)

.PHONY:
install: server
	$(INSTALL-WINDOWS)

else #Linux or other

INSTALL_DEPS = cd v8 && ./build/install-build-deps.sh && cd ..

INSTALL_PATH := /usr/lib/mozilla/native-messaging-hosts/v8fox_server.json
.PHONY:
install: server
	$(call INSTALL-LINUX,$(INSTALL_PATH))

endif

endif