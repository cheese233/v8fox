// chromium中绑定JavaScript 层级中的顶层对象Window到v8中的入口
bool V8DOMWindowShell::installDOMWindow(v8::Handle<v8::Context> context, DOMWindow *window)
{
    // 创建一个JS window对象
    v8::Handle<v8::Function> windowConstructor =
        V8DOMWrapper::getConstructor(&V8DOMWindow::info, getHiddenObjectPrototype(context));
    v8::Local<v8::Object> jsWindow = SafeAllocation::newInstance(windowConstructor);
    // Bail out if allocation failed.
    if (jsWindow.IsEmpty())
        return false;
    // 把c++对象DOMWindow打包到JS window对象的内部数据域
    V8DOMWrapper::setDOMWrapper(jsWindow, &V8DOMWindow::info, window);
    // 把c++对象DOMWindow打包到JS window对象的原型的内部数据域
    V8DOMWrapper::setDOMWrapper(v8::Handle<v8::Object>::Cast(jsWindow->GetPrototype()), &V8DOMWindow::info, window);
    window->ref();
    V8DOMWrapper::setJSWrapperForDOMObject(window, v8::Persistent<v8::Object>::New(jsWindow));
    v8::Handle<v8::Object> v8RealGlobal = v8::Handle<v8::Object>::Cast(context->Global()->GetPrototype());
    // 把c++对象DOMWindow打包到JS 全局对象的内部数据域
    V8DOMWrapper::setDOMWrapper(v8RealGlobal, &V8DOMWindow::info, window);
    // 将JS window对象设置为JS全局对象的原型，v8RealGlobal即为jsWindow的shadow object
    v8RealGlobal->SetPrototype(jsWindow);
    return true;
}
// Sets contents of a DOM wrapper.
static void setDOMWrapper(v8::Handle<v8::Object> object, WrapperTypeInfo *type, void *cptr)
{
    ASSERT(object->InternalFieldCount() >= 2);
    object->SetPointerInInternalField(v8DOMWrapperObjectIndex, cptr);
    object->SetPointerInInternalField(v8DOMWrapperTypeIndex, type);
}