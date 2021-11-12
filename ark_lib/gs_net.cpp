#include <ixwebsocket/IXNetSystem.h>
#include "gs_net.h"

namespace gs_net {
	bool IsInitNetSystem = false;
	WebsockerServer::WebsockerServer(): pWebSocket(nullptr)
		, pfOpen(nullptr)
		, pfClose(nullptr)
		, pfMessage(nullptr){
		if (!IsInitNetSystem) {
			ix::initNetSystem();
		}
	}

	WebsockerServer& WebsockerServer::Start(std::string& url) {
		pWebSocket = std::make_shared<ix::WebSocket>();
		pWebSocket->setUrl(url);
		pWebSocket->setOnMessageCallback([this](const ix::WebSocketMessagePtr& msg) {
			switch (msg->type) {
			case ix::WebSocketMessageType::Message:
				if (this->pfMessage)
					this->pfMessage(*pWebSocket, msg->str);
				break;
			case ix::WebSocketMessageType::Open:
				if (this->pfOpen)
					this->pfOpen(*pWebSocket, msg->openInfo);
				break;
			case ix::WebSocketMessageType::Close:
				if (this->pfClose)
					this->pfClose(*pWebSocket, msg->closeInfo);
				break;
			case ix::WebSocketMessageType::Error:
				if (this->pfError)
					this->pfError(*pWebSocket, msg->errorInfo);
				break;
			default:
				break;
			}
		 });
		return *this;
	}

	WebsockerServer& WebsockerServer::Close() {
		if (pWebSocket) {
			pWebSocket->close();
		}
		return *this;
	}

	WebsockerServer& WebsockerServer::SendText(std::string content) {
		if (pWebSocket) {
			pWebSocket->sendText(content);
		}
		return *this;
	}



	WebsockerServer& WebsockerServer::onMessage(std::function<void(ix::WebSocket&, std::string)> pf) {
		this->pfMessage = pf;
		return *this;
	}
	WebsockerServer& WebsockerServer::onOpen(std::function<void(ix::WebSocket&, ix::WebSocketOpenInfo)> pf) {
		this->pfOpen = pf;
		return *this;
	}
	WebsockerServer& WebsockerServer::onClose(std::function<void(ix::WebSocket&, ix::WebSocketCloseInfo)> pf) {
		this->pfClose = pf;
		return *this;
	}
	WebsockerServer& WebsockerServer::onError(std::function<void(ix::WebSocket&, ix::WebSocketErrorInfo)> pf) {
		this->pfError = pf;
		return *this;
	}
}