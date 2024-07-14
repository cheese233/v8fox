// Copyright 2015 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <bit>
#include <cstdint>
#include <iostream>
#include <ostream>
#include <stdint.h>
#include <stdlib.h>
#include <string>

#include "libplatform/libplatform.h"
#include "src/json/json-stringifier.h"
#include "v8-array-buffer.h"
#include "v8-context.h"
#include "v8-initialization.h"
#include "v8-isolate.h"
#include "v8-local-handle.h"
#include "v8-primitive.h"
#include "v8-script.h"

std::string JSmain(int argc, char *argv[])
{
    std::string output = "";
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
        v8::Isolate::Scope isolate_scope(isolate);

        // Create a stack-allocated handle scope.
        v8::HandleScope handle_scope(isolate);

        // Create a new context.
        v8::Local<v8::Context> context = v8::Context::New(isolate);

        // Enter the context for compiling and running the hello world script.
        v8::Context::Scope context_scope(context);

        {
            // Create a string containing the JavaScript source code.
            v8::Local<v8::String> source =
                v8::String::NewFromUtf8Literal(isolate, "'Hello' + ', World' + String.fromCharCode(33)");

            // Compile the source code.
            v8::Local<v8::Script> script = v8::Script::Compile(context, source).ToLocalChecked();

            // Run the script to get the result.
            v8::Local<v8::Value> result = script->Run(context).ToLocalChecked();

            // Convert the result to an UTF8 string and print it.
            v8::String::Utf8Value utf8(isolate, result);
            output += *utf8;
        }
        // {

        //     v8::internal::JsonStringify(isolate, , , );
        // }
    }

    // Dispose the isolate and tear down V8.
    isolate->Dispose();
    v8::V8::Dispose();
    v8::V8::DisposePlatform();
    delete create_params.array_buffer_allocator;
    return output;
}
int main(int argc, char *argv[])
{
    std::string output = '"' + JSmain(argc, argv) + '"';
    int32_t outputLength = output.length();
    uint8_t LengthArray[sizeof(outputLength)];
    memcpy(&LengthArray[0], &outputLength, sizeof(outputLength));
    for (int i = 0; i < sizeof(outputLength); i++)
    {
        std::cout << LengthArray[i];
    }
    std::cout << output << std::flush;
    return 0;
}