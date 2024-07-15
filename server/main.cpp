#include <cstdint>
#include <iostream>
#include <ostream>
#include <stdint.h>
#include <stdlib.h>
#include <string>

#include "uv.h"

#include "libplatform/libplatform.h"
#include "src/json/json-stringifier.h"
#include "v8-array-buffer.h"
#include "v8-context.h"
#include "v8-initialization.h"
#include "v8-isolate.h"
#include "v8-local-handle.h"
#include "v8-object.h"
#include "v8-primitive.h"
#include "v8-script.h"

void output_uint32(uint32_t num)
{
    uint8_t LengthArray[sizeof(num)];
    memcpy(&LengthArray[0], &num, sizeof(num));
    for (int i = 0; i < sizeof(num); i++)
        std::cout << LengthArray[i];
}

void insertGlobalWindow()
{
}

int main(int argc, char *argv[])
{
    // Initialize libuv.
    uv_loop_t *loop = new uv_loop_t();

    uv_loop_init(loop);
    // Initialize V8.
    v8::V8::InitializeICUDefaultLocation(argv[0]);
    v8::V8::InitializeExternalStartupData(argv[0]);
    std::unique_ptr<v8::Platform> platform = v8::platform::NewDefaultPlatform();
    v8::V8::InitializePlatform(platform.get());
    v8::V8::Initialize();

    // Create a new Isolate and make it the current one.
    v8::Isolate::CreateParams create_params;
    create_params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    v8::Isolate *isolate = v8::Isolate::New(create_params);

    {
        std::string output = "";
        output_uint32(output.length());
        std::cout << output << std::flush;
    }

    // Dispose the isolate and tear down V8.
    isolate->Dispose();
    v8::V8::Dispose();
    v8::V8::DisposePlatform();
    delete create_params.array_buffer_allocator;

    uv_run(loop, UV_RUN_DEFAULT);

    uv_loop_close(loop);

    free(loop);
    return 0;
}