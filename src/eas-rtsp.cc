#include <rtspconnectionclient.h>
#include <signal.h>

using namespace Nan;
using namespace v8;

uint8_t marker[] = { 0, 0, 0, 1 };

char stop = 0;


void buffer_delete_callback(char* data, void* the_vector) {
	delete reinterpret_cast<vector<unsigned char> *> (the_vector);
}

void sig_handler(int signo) {
	if (signo == SIGINT) {
		printf("received SIGINT\n");
		stop = 1;
	}
	if (signo == SIGUSR1) {
		printf("received SIGUSR1\n");
		stop = 1;
	}
}

class NodeWorker : public AsyncWorker {
  public:
	NodeWorker(Callback *callback) : AsyncWorker(callback) {}

	~NodeWorker() {}
	
	void Execute () {
		class RTSPCallback : public RTSPConnection::Callback
		{
		  public:
			virtual bool onData(const char* id, unsigned char* buffer, ssize_t size, struct timeval presentationTime, vector<unsigned char>& m_node_buffer) {
				m_node_buffer.clear();
				m_node_buffer.insert(m_node_buffer.end(), marker, marker+sizeof(marker));
				m_node_buffer.insert(m_node_buffer.end(), buffer, buffer+size);

				std::cout << id << " " << size << " ts:" << presentationTime.tv_sec << "." << presentationTime.tv_usec << std::endl;
				return true;
			}
		};

		// Reserva espacio en memoria para el puntero al vector<uint8_t>
		m_node_buffer = new vector<uint8_t>();
			
		Environment env(stop);
		RTSPCallback cb;
		RTSPConnection rtspClient(env, &cb, "rtsp://admin:@192.168.1.38:554/live1.sdp", *m_node_buffer);
		signal(SIGINT, sig_handler);
		signal(SIGUSR1, sig_handler);

		env.mainloop();
	
	}

	// We have the results, and we're back in the event loop.
	void HandleOKCallback () {
		Nan:: HandleScope scope;

		v8::Local<v8::Object> node_buffer = 
			Nan::NewBuffer((char *)m_node_buffer->data(), 
						   m_node_buffer->size(), buffer_delete_callback,
						   m_node_buffer).ToLocalChecked();
		v8::Local<v8::Value> argv[] = { Nan::Null(), node_buffer };
		callback->Call(2, argv);
	}
  private:
	std::vector<uint8_t> * m_node_buffer;
	char m_stop = 1;
};

void hijo(void) {
	std::vector<uint8_t> * m_node_buffer;
	
	class RTSPCallback : public RTSPConnection::Callback
	{
	  public:
		virtual bool onData(const char* id, unsigned char* buffer, ssize_t size, struct timeval presentationTime, vector<unsigned char>& m_node_buffer) {
			m_node_buffer.clear();
			m_node_buffer.insert(m_node_buffer.end(), marker, marker+sizeof(marker));
			m_node_buffer.insert(m_node_buffer.end(), buffer, buffer+size);

			//std::cout << id << " " << size << " ts:" << presentationTime.tv_sec << "." << presentationTime.tv_usec << std::endl;
			return true;
		}
	};

	// Reserva espacio en memoria para el puntero al vector<uint8_t>
	m_node_buffer = new vector<uint8_t>();
			
	Environment env(stop);
	RTSPCallback cb;
	RTSPConnection rtspClient(env, &cb, "rtsp://admin:@192.168.1.38:554/live1.sdp", *m_node_buffer);
	signal(SIGINT, sig_handler);
	signal(SIGUSR1, sig_handler);

	env.mainloop();
}

NAN_METHOD(getBuffer) {
	// Nan::Callback *node_callback = new Nan::Callback(info[0].As<Function>());

	pid_t pid;
	int status;

	pid=fork();
	if (pid == -1) {
		Nan::ThrowError("Can't create child process...");
		return;
	}

	if (pid == 0) {
        printf("*********** I am in the child with PID = %d *****************\n", childpid); 
		hijo();
    }

	// Solo el padre, no es alcanzado por el hijo porque se queda en el event
	// loop de live555
	auto message = Nan::New<v8::String>("I'm a Node Hero!").ToLocalChecked();
	info.GetReturnValue().Set(message);
	
	// AsyncQueueWorker(new NodeWorker(node_callback));
}


NAN_MODULE_INIT(Initialize) {
    NAN_EXPORT(target, getBuffer);
}

NODE_MODULE(addon, Initialize);
