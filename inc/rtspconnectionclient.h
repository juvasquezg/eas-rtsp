/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** rtspconnectionclient.h
** 
** Interface to an RTSP client connection
** 
** -------------------------------------------------------------------------*/

#pragma once

#include "environment.h"
#include "liveMedia.hh"

#include <nan.h>
#include <v8.h>

#define RTSP_CALLBACK(uri, resultCode, resultString) \
static void continueAfter ## uri(RTSPClient* rtspClient, int resultCode, char* resultString) { static_cast<RTSPConnection::RTSPClientConnection*>(rtspClient)->continueAfter ## uri(resultCode, resultString); } \
void continueAfter ## uri (int resultCode, char* resultString); \
/**/

#define TASK_CALLBACK(task) \
static void Task ## task(void* rtspClient) { static_cast<RTSPConnection::RTSPClientConnection*>(rtspClient)->Task ## task(); } \
void Task ## task (); \
/**/


#if LIVEMEDIA_LIBRARY_VERSION_INT > 1371168000 
	#define RTSPClientConstrutor(env, url, verbosity, appname, httpTunnelPort) RTSPClient(env, url, verbosity, appname, httpTunnelPort ,-1)
#else					
	#define RTSPClientConstrutor(env, url, verbosity, appname, httpTunnelPort) RTSPClient(env, url, verbosity, appname, httpTunnelPort)
#endif

/* ---------------------------------------------------------------------------
**  RTSP client connection interface
** -------------------------------------------------------------------------*/
class RTSPConnection 
{
	public:
		/* ---------------------------------------------------------------------------
		**  RTSP client callback interface
		** -------------------------------------------------------------------------*/
		class Callback
		{
			public:
				virtual bool    onNewSession(const char* id, const char* media, const char* codec, const char* sdp) { return true; }
				virtual bool    onData(const char* id, unsigned char* buffer, ssize_t size, struct timeval presentationTime, Nan::Callback* node_callback) = 0;
				virtual ssize_t onNewBuffer(unsigned char* , ssize_t ) { return 0; }
				virtual void    onError(const char* ) {}
				virtual void    onConnectionTimeout(RTSPConnection&) {}
				virtual void    onDataTimeout(RTSPConnection&)       {}
		};

	protected:
		/* ---------------------------------------------------------------------------
		**  RTSP client Sink
		** -------------------------------------------------------------------------*/
		class SessionSink: public MediaSink 
		{
            public:
            static SessionSink* createNew(UsageEnvironment& env, Callback* callback, Nan::Callback* node_callback) { return new SessionSink(env, callback, node_callback); }

			private:
            SessionSink(UsageEnvironment& env, Callback* callback, Nan::Callback* node_callback);
				virtual ~SessionSink();

				void allocate(ssize_t bufferSize);

				static void afterGettingFrame(void* clientData, unsigned frameSize,
							unsigned numTruncatedBytes,
							struct timeval presentationTime,
							unsigned durationInMicroseconds)
				{
					static_cast<SessionSink*>(clientData)->afterGettingFrame(frameSize, numTruncatedBytes, presentationTime, durationInMicroseconds);
				}
				
				void afterGettingFrame(unsigned frameSize, unsigned numTruncatedBytes, struct timeval presentationTime, unsigned durationInMicroseconds);

				virtual Boolean continuePlaying();

			private:
				u_int8_t*              m_buffer;
				size_t                 m_bufferSize;
				Callback*              m_callback;
                Nan::Callback*         m_node_callback;
				ssize_t                m_markerSize;
		};
	
		/* ---------------------------------------------------------------------------
		**  RTSP client 
		** -------------------------------------------------------------------------*/
		class RTSPClientConnection : public RTSPClient
		{
			public:
			RTSPClientConnection(RTSPConnection& connection, Environment& env, Callback* callback, const char* rtspURL, Nan::Callback* node_callback, int timeout, bool rtpovertcp, int verbosityLevel);
				virtual ~RTSPClientConnection(); 
			
			protected:
				void sendNextCommand(); 
						
				RTSP_CALLBACK(DESCRIBE,resultCode,resultString);
				RTSP_CALLBACK(SETUP,resultCode,resultString);
				RTSP_CALLBACK(PLAY,resultCode,resultString);
			
				TASK_CALLBACK(ConnectionTimeout);
				TASK_CALLBACK(DataArrivalTimeout);
				
			protected:
				RTSPConnection&          m_connection;
				int                      m_timeout;
				bool                     m_rtpovertcp;
				MediaSession*            m_session;                   
				MediaSubsession*         m_subSession;             
				MediaSubsessionIterator* m_subSessionIter;
				Callback*                m_callback;
                Nan::Callback*           m_node_callback;
				TaskToken 		 m_connectionTask;
				TaskToken 		 m_dataTask;
				unsigned int             m_nbPacket;
		};
		
	public:
		RTSPConnection(Environment& env, Callback* callback, const char* rtspURL, Nan::Callback* node_callbak, int timeout = 5, bool rtpovertcp = false, int verbosityLevel = 1);
		virtual ~RTSPConnection();

		void start();
	
	
	protected:
		Environment&             m_env;
		Callback*                m_callback; 	
		const char*              m_url;
        Nan::Callback*           m_node_callback;
		int                      m_timeout;
		bool                     m_rtpovertcp;
		int                      m_verbosity;
	
		RTSPClientConnection*    m_rtspClient;
};
