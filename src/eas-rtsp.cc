#include <rtspconnectionclient.h>

using namespace v8;

NAN_METHOD(getBuffer) {
	Nan:: HandleScope scope;
	
	Nan::Callback *node_callback = new Nan::Callback(info[0].As<Function>());

	class RTSPCallback : public RTSPConnection::Callback
	{
	public:
		virtual bool onData(const char* id, unsigned char* buffer, ssize_t size, struct timeval presentationTime,  Nan::Callback* node_callback) {
			// std::cout << id << " " << size << " ts:" << presentationTime.tv_sec << "." << presentationTime.tv_usec << std::endl;
			v8::Local<v8::Object> node_buffer = Nan::NewBuffer((char *)buffer, size).ToLocalChecked();
			v8::Local<v8::Value> argv[] = { node_buffer };
			node_callback->Call(1, argv);
			return true;
		}
	};
  
	Environment env;
	RTSPCallback cb;
	RTSPConnection rtspClient(env, &cb, "rtsp://admin:@192.168.1.38:554/live1.sdp", node_callback);
	env.mainloop();
}


NAN_MODULE_INIT(Initialize) {
    NAN_EXPORT(target, getBuffer);
}

NODE_MODULE(addon, Initialize);
