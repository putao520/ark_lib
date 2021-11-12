#pragma once
#include <string>
// 
#include <ixwebsocket/IXWebSocket.h>
// #include <ixwebsocket/IXUserAgent.h>
#include <memory.h>
#include <functional>
namespace gs_net {
	class WebsockerServer {
	public:
		WebsockerServer();
		WebsockerServer& Start(std::string& url);
		WebsockerServer& Close();

		WebsockerServer& SendText(std::string content);

		WebsockerServer& onMessage(std::function<void(ix::WebSocket&, std::string)> pf);
		WebsockerServer& onOpen(std::function<void(ix::WebSocket&, ix::WebSocketOpenInfo)> pf);
		WebsockerServer& onClose(std::function<void(ix::WebSocket&, ix::WebSocketCloseInfo)> pf);
		WebsockerServer& onError(std::function<void(ix::WebSocket&, ix::WebSocketErrorInfo)> pf);
	private:
		std::shared_ptr<ix::WebSocket> pWebSocket;
		std::function<void(ix::WebSocket&, std::string)> pfMessage;
		std::function<void(ix::WebSocket&, ix::WebSocketOpenInfo)> pfOpen;
		std::function<void(ix::WebSocket&, ix::WebSocketCloseInfo)> pfClose;
		std::function<void(ix::WebSocket&, ix::WebSocketErrorInfo)> pfError;
	};
};