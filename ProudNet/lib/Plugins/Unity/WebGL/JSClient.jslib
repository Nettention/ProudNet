// UTF8로 저장하지 마세요. Unity Editor에서 WebGL Build 명령 실행 중에 "cp-949 decode" 어쩌고 내부 에러가 발생해버립니다. 유니티측에 에러 현상을 제보해주세요.

var WebGL = {
	// enableLog를 true로 변경하면 JS단의 개략적인 로그를 확인 가능. writeLog를 사용하여 사용자의 로그도 추가 가능.
	// setEnv는 최초 false, prototype 정의 후 true. function을 재정의 하지 않고 사용하기 위해 prototype에 정의한 것임. 사용자는 이값을 손대지 마세요. 퍼포먼스 저하로 이어집니다.
	// internalVersion은 프라우드넷 내부 버전정보. C#쪽에서 넘어오는 값으로 설정된다.
	$globalVariables: {enableLog: false, setEnv: false, internalVersion: 0},
	
	// NetClient들이 저장되는 공간
	$netClientInstances: [],
	
	// C#쪽의 콜백들이 저장되는 공간
	$callbacks: {},
	
	SetOnJoinServerComplete: function(callback) {
		callbacks.onJoinServerComplete = callback;
	},
	SetOnLeaveServer: function(callback) {
		callbacks.onLeaveServer = callback;
	},
	SetOnSynchronizeServerTime: function(callback) {
		callbacks.onSynchronizeServerTime = callback;
	},
	SetProcessMessage_UserMessage: function(callback) {
		callbacks.onUserMessage = callback;
	},
	SetProcessMessage_Rmi: function(callback) {
		callbacks.onRmi = callback;
	},
	SetOnHostIDIssued: function(callback) {
		callbacks.onHostIDIssued = callback;
	},
	
	$WebSocketWrapper__postset: 'WebSocketWrapper();',
	$WebSocketWrapper: function () {
	},

	$Guid__postset: 'Guid();',
	$Guid: function () {
	},

	$NetClient__postset: 'NetClient();',
	$NetClient: function () {
		this.proxies = new Array();
		this.stubs = new Array();
		this.connected = false;
		this.pingTimerID = 0;
		this.localHostID = 0;
		this.serverTcpLastPingMs = 0;
		// onLeaveServer
		this.errorType = 0;
		this.detailType = 0;
		this.reason = "";
	},

	$ProudC2S__postset: 'ProudC2S();',
	$ProudC2S: function () {
	},

	$ProudS2C__postset: 'ProudS2C();',
	$ProudS2C: function () {
	},

	$NetConnectionParam__postset: 'NetConnectionParam();',
	$NetConnectionParam: function () {
	},

	$NetC2SProxy__postset: 'NetC2SProxy();',
	$NetC2SProxy: function () {
		this.rmiIDList = new Array();
		this.rmiIDList.push(ProudC2S.Rmi_ReliablePing);
		this.rmiIDList.push(ProudC2S.Rmi_P2PGroup_MemberJoin_Ack);
		this.rmiIDList.push(ProudC2S.Rmi_ShutdownTcp);
	},

	$NetS2CStub__postset: 'NetS2CStub();',
	$NetS2CStub: function () {
		this.rmiIDList = new Array();
	},

	$ErrorInfo__postset: 'ErrorInfo();',
	$ErrorInfo: function () {
	},
	
	$RmiContext__postset: 'RmiContext();',
	$RmiContext: function() {
	},

	$ByteArrayBase__postset: 'ByteArrayBase();',
	$ByteArrayBase: function (buffer, offset, length) {
		if (offset === void 0) { offset = 0; }
		if (length === void 0) { length = 0; }
		this.bitsPending = 0;
		this.BUFFER_EXT_SIZE = 1024;
		this.array = null;
		this.EOF_byte = -1;
		this.EOF_code_point = -1;
		if (buffer == undefined) {
			buffer = new ArrayBuffer(this.BUFFER_EXT_SIZE);
			this.write_position = 0;
		}
		else if (buffer == null) {
			this.write_position = 0;
		}
		else {
			this.write_position = length > 0 ? length : buffer.byteLength;
		}
		if (buffer) {
			this.data = new DataView(buffer, offset, length > 0 ? length : buffer.byteLength);
		}
		this._position = 0;
		this.endian = ByteArrayBase.BIG_ENDIAN;
	},

	$SetEnvironment: function() {
		// 한 번만 실행하기 위해
		if(globalVariables.setEnv)
			return;
		else
			globalVariables.setEnv = true;

		// Enums
		// MessageType
		var MessageType = {"None": 0, "Rmi": 1, "UserMessage": 2, "ConnectServerTimedout": 4, "NotifyStartupEnvironment": 5,
			"RequestServerConnection": 6,
			"NotifyProtocolVersionMismatch": 11, "NotifyServerDeniedConnection": 12, "NotifyServerConnectSuccess": 13,
			"ReliableRelay1": 26, "UnreliableRelay1": 27,
			"ReliableRelay2": 30, "UnreliableRelay2": 31, "PolicyRequest": 54, "NotifyLicenseMismatch": 57};
		// HostID
		var HostID = {"None": 0, "Server": 1, "Last": 2};
		// MessageReliability
		var MessageReliability = {"Unreliable": 0, "Reliable": 1, "Last": 2};
		// MessagePriority
		var MessagePriority = {"Ring0": 0, "Ring1": 1, "High": 2, "Medium": 3, "Low": 4, "Ring99": 5, "Last": 6};
		// ErrorType
		var ErrorType = {"Ok": 0, "TCPConnectFailure": 3, "ConnectServerTimeout": 7, "ProtocolVersionMismatch": 8,
			"InvalidLicense": 9, "NotifyServerDeniedConnection": 10, "DisconnectFromRemote": 12, "DisconnectFromLocal": 13};
		// SocketErrorCode
		var SocketErrorCode = {"Ok": 0};
		// RuntimePlatform
		var RuntimePlatform = {"UWebGL": 17};
		// EncryptMode
		var EncryptMode = {"None":0, "Secure":1, "Fast":2};
		
		// WebSocketWrapper
		WebSocketWrapper.prototype.connect = function (url, protocols) {
			this.socket = new WebSocket(url, protocols);
		};
		WebSocketWrapper.prototype.sendToServer = function (hostIDArr, payload, rmiContext) {
			if(rmiContext === undefined)
			{
				rmiContext = new RmiContext();
				rmiContext.setValue(MessageReliability.Reliable, MessagePriority.Medium, EncryptMode.None);
			}
			var relayDestList = new Array();
			for (var i = 0; i < hostIDArr.length; i++) {
				if (hostIDArr[i] == HostID.Server) {
					this.socket.send(payload.getArray().slice(0, payload.length));
				}
				else {
					relayDestList.push(hostIDArr[i]);
				}
			}
			if (relayDestList.length > 0) {
				var sendMsg = new ByteArrayBase();
				if (rmiContext.reliability == MessageReliability.Reliable) {
					sendMsg.writeByte(MessageType.ReliableRelay1);
					sendMsg.writeByte(rmiContext.encryptMode);
					sendMsg.writeVLQ(relayDestList.length);
					for (var i = 0; i < relayDestList.length; i++) {
						sendMsg.writeInt(relayDestList[i]);
					}
				}
				else {
					sendMsg.writeByte(MessageType.UnreliableRelay1);
					sendMsg.writeByte(rmiContext.encryptMode);
					sendMsg.writeByte(rmiContext.priority);
					sendMsg.writeDouble(0);
					sendMsg.writeBoolean(false);
					sendMsg.writeVLQ(relayDestList.length);
					for (var i = 0; i < relayDestList.length; i++) {
						sendMsg.writeInt(relayDestList[i]);
					}
				}
				sendMsg.writeVLQ(payload.length);
				sendMsg.writeBytes(payload);
				
				this.socket.send(sendMsg.getArray().slice(0, payload.length));
			}
		};

		// Guid
		Guid.prototype.setValue = function (data1, data2, data3, data4) {
			this.data1 = data1;
			this.data2 = data2;
			this.data3 = data3;
			this.data4 = data4;
		};
		Guid.prototype.toByteArray = function () {
			var byteArr = new ByteArrayBase();
			byteArr.writeUnsignedInt(this.data1);
			byteArr.writeUnsignedShort(this.data2);
			byteArr.writeUnsignedShort(this.data3);
			for (var _i = 0, _a = this.data4; _i < _a.length; _i++) {
				var i = _a[_i];
				byteArr.writeUnsignedByte(i);
			}
			return byteArr;
		};

		// RmiContext
		RmiContext.prototype.setValue = function (reliability, priority, encryptMode) {
			this.reliability = reliability;
			this.priority = priority;
			this.encryptMode = encryptMode;
		};

		// NetClient
		NetClient.prototype.sendUserMessage = function (hostIDArr, payload, rmiContext) {
			this.socketWrapper.sendToServer(hostIDArr, payload, rmiContext);
		};
		NetClient.prototype.connect = function (param) {
		    this.connectionParam = param;
			var _this = this;
			this.socketWrapper = new WebSocketWrapper();
			this.socketWrapper.connect(param.serverUrl, param.protocols);
			this.netC2SProxy = new NetC2SProxy();
			this.netC2SProxy.setSocket(this.socketWrapper);
			this.netS2CStub = new NetS2CStub();
			this.netS2CStub.setClient(this);
			this.socketWrapper.socket.onopen = function (event) {
				_this.connected = true;
				if(globalVariables.enableLog)
					writeLog("websocket[" + _this.instanceID + "] onopen. event.type[" + event.type + "]", "NetClient.connect");
			};
			this.socketWrapper.socket.onmessage = function (message) {
				if(globalVariables.enableLog)
					writeLog("websocket[" + _this.instanceID + "] onmessage. message.data.length[" + message.data.length + "]", "NetClient.connect");
				
				var msg = new ByteArrayBase();
				msg.setArray(base64ToUint8Array(message.data));
				_this.processMessage_ProudNetLayer(msg);
			};
			this.socketWrapper.socket.onclose = function (closeEvent) {
				_this.connected = false;
				_this.stopPing();
				if(globalVariables.enableLog)
					writeLog("websocket[" + _this.instanceID + "] onclose. closeEvent.code[" + closeEvent.code + "], closeEvent.type[" + closeEvent.type + "]", "NetClient.connect");
				
				var disconnReason = "";
				if(closeEvent.code == 1000)
					disconnReason = "Normal Closure";
				else
					disconnReason = "Abnormal Closure";
				if(_this.disconnectByServer == true)
				{
					Runtime.dynCall('viiii', callbacks.onLeaveServer,
						[_this.instanceID, _this.errorType, _this.detailType, allocate(intArrayFromString(_this.reason + "[" + disconnReason + "]"), 'i8', ALLOC_STACK)]);
				}
			};
			this.socketWrapper.socket.onerror = function (event) {
				if(globalVariables.enableLog)
					writeLog("websocket[" + _this.instanceID + "] onerror. event.type[" + event.type + "]", "NetClient.connect");
			};
		};
		NetClient.prototype.disconnect = function (disconnectByServer) {
			this.disconnectByServer = disconnectByServer;
			this.netC2SProxy.ShutdownTcp();
		};
		NetClient.prototype.startPing = function () {
			var _this = this;
			this.pingTimerID = setInterval(function () {
				_this.netC2SProxy.ReliablePing(1, Date.now() / 1e3 | 0, 0, this.serverTcpLastPingMs);
			}, 3e3);
		};
		NetClient.prototype.stopPing = function () {
			if (this.pingTimerID != 0) {
				clearInterval(this.pingTimerID);
			}
		};
		NetClient.prototype.attachProxy = function (proxy) {
			this.proxies.push(proxy);
		};
		NetClient.prototype.detachProxy = function (proxy) {
			for (var i = 0; i < this.proxies.length; i++) {
				if (this.proxies[i] === proxy)
					this.proxies.splice(i, 1);
			}
		};
		NetClient.prototype.attachStub = function (stub) {
			this.stubs.push(stub);
		};
		NetClient.prototype.detachStub = function (stub) {
			for (var i = 0; i < this.stubs.length; i++) {
				if (this.stubs[i] === stub)
					this.stubs.splice(i, 1);
			}
		};
		NetClient.prototype.processMessage_ProudNetLayer = function (msg, remote) {
			if (remote === void 0) { remote = HostID.Server; }
			var msgType = msg.readByte();
			if(globalVariables.enableLog)
				writeLog("msgType[" + msgType + "]", "NetClient.processMessage_ProudNetLayer");
			switch (msgType) {
				case MessageType.ConnectServerTimedout:
					this.processMessage_ConnectServerTimedout(msg);
					break;
				case MessageType.NotifyStartupEnvironment:
					this.processMessage_NotifyStartupEnvironment(msg);
					break;
				case MessageType.NotifyServerConnectSuccess:
					this.processMessage_NotifyServerConnectSuccess(msg);
					break;
				case MessageType.NotifyProtocolVersionMismatch:
					this.processMessage_NotifyProtocolVersionMismatch(msg);
					break;
				case MessageType.NotifyLicenseMismatch:
					this.processMessage_NotifyLicenseMismatch(msg);
					break;
				case MessageType.NotifyServerDeniedConnection:
					this.processMessage_NotifyServerDeniedConnection(msg);
					break;
				case MessageType.ReliableRelay2:
					this.processMessage_ReliableRelay2(msg);
					break;
				case MessageType.UnreliableRelay2:
					this.processMessage_UnreliableRelay2(msg);
					break;
				case MessageType.Rmi:
					this.processMessage_Rmi(msg, remote);
					break;
				case MessageType.UserMessage:
					Runtime.dynCall('viii', callbacks.onUserMessage, [this.instanceID, remote, allocate(intArrayFromString(Uint8ArrayToBase64(msg.getArray().subarray(1))), 'i8', ALLOC_STACK)]);
					break;
				case MessageType.PolicyRequest:
					break;
			}
		};
		NetClient.prototype.processMessage_ConnectServerTimedout = function (msg) {
			Runtime.dynCall('viiii', callbacks.onJoinServerComplete,
				[this.instanceID,
				allocate(intArrayFromString(ErrorType.ConnectServerTimeout + "|" + 0), 'i8', ALLOC_STACK),
				allocate(intArrayFromString("Remote detected no receive."), 'i8', ALLOC_STACK),
				allocate(intArrayFromString(""), 'i8', ALLOC_STACK)]);
		};
		NetClient.prototype.processMessage_NotifyStartupEnvironment = function (msg) {
			var enableLog;
			try {
				enableLog = msg.readBoolean();
			}
			catch (e) {
				writeLog(e, "NetClient.processMessage_NotifyStartupEnvironment");
				
				this.errorType = ErrorType.ProtocolVersionMismatch;
				this.detailType = ErrorType.TCPConnectFailure;
				this.reason = "NSE-R1";
				
				this.client.disconnectByServer = true;
				this.client.socketWrapper.socket.close();
				
				return;
			}
			var fallbackMethod;
			var serverMessageMaxLength;
			var clientMessageMaxLength;
			var defaultTimeoutTime;
			var autoConnectionRecoveryTimeoutTimeMs;
			var directP2PStartCondition;
			var overSendSuspectingThresholdInBytes;
			var enableNagleAlgorithm;
			var encryptedMessageKeyLength;
			var fastEncryptedMessageKeyLength;
			var allowServerAsP2PGroupMember;
			var enableEncryptedMessaging;
			var enableP2PEncryptedMessaging;
			var upnpDetectNatDevice;
			var upnpTcpAddPortMapping;
			var enableLookaheadP2PSend;
			var enablePingTest;
			var ignoreFailedBindPort;
			var emergencyLogLineCount;
			try {
				fallbackMethod = msg.readByte();
				serverMessageMaxLength = msg.readInt();
				clientMessageMaxLength = msg.readInt();
				defaultTimeoutTime = msg.readInt();
				autoConnectionRecoveryTimeoutTimeMs = msg.readInt();
				directP2PStartCondition = msg.readByte();
				overSendSuspectingThresholdInBytes = msg.readInt();
				enableNagleAlgorithm = msg.readBoolean();
				encryptedMessageKeyLength = msg.readInt();
				fastEncryptedMessageKeyLength = msg.readInt();
				allowServerAsP2PGroupMember = msg.readBoolean();
				enableEncryptedMessaging = msg.readBoolean();
				enableP2PEncryptedMessaging = msg.readBoolean();
				upnpDetectNatDevice = msg.readBoolean();
				upnpTcpAddPortMapping = msg.readBoolean();
				enableLookaheadP2PSend = msg.readBoolean();
				enablePingTest = msg.readBoolean();
				ignoreFailedBindPort = msg.readBoolean();
				emergencyLogLineCount = msg.readInt();
			}
			catch (e) {
				writeLog(e, "NetClient.processMessage_NotifyStartupEnvironment");
				
				this.errorType = ErrorType.ProtocolVersionMismatch;
				this.detailType = ErrorType.TCPConnectFailure;
				this.reason = "NSE-R2";
				
				this.client.disconnectByServer = true;
				this.client.socketWrapper.socket.close();
				
				return;
			}
			try {
				this.serverTcpLastPingMs = msg.readInt();
			}
			catch (e) {
				writeLog(e, "NetClient.processMessage_NotifyStartupEnvironment");
				
				this.errorType = ErrorType.ProtocolVersionMismatch;
				this.detailType = ErrorType.TCPConnectFailure;
				this.reason = "";
				
				this.client.disconnectByServer = true;
				this.client.socketWrapper.socket.close();
				
				return;
			}
			var sendMsg = new ByteArrayBase();
			sendMsg.writeByte(MessageType.RequestServerConnection);
			sendMsg.writeInt(RuntimePlatform.UWebGL);
			sendMsg.writeInt(this.serverTcpLastPingMs);
			if (this.connectionParam.userData != null) {
				sendMsg.writeVLQ(this.connectionParam.userData.length);
				sendMsg.writeBytes(this.connectionParam.userData);
			}
			else {
				sendMsg.writeVLQ(0);
			}
			sendMsg.writeBytes(this.connectionParam.protocolVersion.toByteArray());
			sendMsg.writeInt(globalVariables.internalVersion);
			this.socketWrapper.sendToServer([HostID.Server], sendMsg);
		};
		NetClient.prototype.processMessage_NotifyServerConnectSuccess = function (msg) {
			this.startPing();
			this.localHostID = msg.readInt();
			var userData = new ByteArrayBase();
			userData.setArray(msg.readUint8Array(msg.readVLQ()));
			var localAddr = msg.readStringTillNull();
			var localPort = msg.readUnsignedShort();
			
			Runtime.dynCall('vii', callbacks.onHostIDIssued, [this.instanceID, this.localHostID]);
			
			Runtime.dynCall('viiii', callbacks.onJoinServerComplete,
				[this.instanceID, 
				allocate(intArrayFromString(ErrorType.Ok + "|" + 0), 'i8', ALLOC_STACK), 
				allocate(intArrayFromString(""), 'i8', ALLOC_STACK), 
				allocate(intArrayFromString(Uint8ArrayToBase64(userData.getArray())), 'i8', ALLOC_STACK)]);
		};
		NetClient.prototype.processMessage_NotifyProtocolVersionMismatch = function (msg) {
			Runtime.dynCall('viiii', callbacks.onJoinServerComplete,
				[this.instanceID,
				allocate(intArrayFromString(ErrorType.ProtocolVersionMismatch + "|" + 0), 'i8', ALLOC_STACK),
				allocate(intArrayFromString("NPVM1"), 'i8', ALLOC_STACK),
				allocate(intArrayFromString(""), 'i8', ALLOC_STACK)]);
		};
		NetClient.prototype.processMessage_NotifyLicenseMismatch = function (msg) {
			Runtime.dynCall('viiii', callbacks.onJoinServerComplete,
				[this.instanceID,
				allocate(intArrayFromString(ErrorType.InvalidLicense + "|" + 0), 'i8', ALLOC_STACK),
				allocate(intArrayFromString("NLM1"), 'i8', ALLOC_STACK),
				allocate(intArrayFromString(""), 'i8', ALLOC_STACK)]);
		};
		NetClient.prototype.processMessage_NotifyServerDeniedConnection = function (msg) {
			Runtime.dynCall('viiii', callbacks.onJoinServerComplete,
				[this.instanceID,
				allocate(intArrayFromString(ErrorType.NotifyServerDeniedConnection + "|" + 0), 'i8', ALLOC_STACK),
				allocate(intArrayFromString("NPDC"), 'i8', ALLOC_STACK),
				allocate(intArrayFromString(""), 'i8', ALLOC_STACK)]);
		};
		NetClient.prototype.processMessage_ReliableRelay2 = function (msg) {
			var senderHostID = msg.readInt();
			var frameNumber = msg.readInt();
			var relayedMsg = new ByteArrayBase();
			relayedMsg.setArray(msg.readUint8Array(msg.readVLQ()));
			this.processMessage_ProudNetLayer(relayedMsg);
		};
		NetClient.prototype.processMessage_UnreliableRelay2 = function (msg) {
			var senderHostID = msg.readInt();
			var relayedMsg = new ByteArrayBase();
			relayedMsg.setArray(msg.readUint8Array(msg.readVLQ()));
			this.processMessage_ProudNetLayer(relayedMsg);
		};
		NetClient.prototype.processMessage_Rmi = function (msg, remote) {
			if(this.netS2CStub.ProcessReceivedMessage(msg, remote) == false) {
				Runtime.dynCall('viii', callbacks.onRmi, [this.instanceID, remote, allocate(intArrayFromString(Uint8ArrayToBase64(msg.getArray().subarray(1))), 'i8', ALLOC_STACK)]);
			}
		};

		// ProudC2S
		ProudC2S.Rmi_ReliablePing = 64001;
		ProudC2S.Rmi_ShutdownTcp = 64006;

		// ProudS2C
		ProudS2C.Rmi_ReliablePong = 63008;
		ProudS2C.Rmi_EnableLog = 63009;
		ProudS2C.Rmi_DisableLog = 63010;
		ProudS2C.Rmi_ShutdownTcpAck = 63013;
		ProudS2C.Rmi_RequestAutoPrune = 63014;

		// NetC2SProxy
		NetC2SProxy.prototype.setSocket = function (socketWrapper) {
			this.socketWrapper = socketWrapper;
		};

		NetC2SProxy.prototype.GetRmiIDList = function () {
			return this.rmiIDList;
		};
		NetC2SProxy.prototype.GetRmiIDListCount = function () {
			return this.rmiIDList.length;
		};
		NetC2SProxy.prototype.RmiSend = function (remote, msg) {
			this.socketWrapper.sendToServer([remote], msg);
		};
		NetC2SProxy.prototype.ReliablePing = function (recentFrameRate, localTimeMs, messageID, lastReliablePingMs) {
			var msg = new ByteArrayBase();
			msg.writeByte(MessageType.Rmi);
			msg.writeShort(ProudC2S.Rmi_ReliablePing);
			msg.writeDouble(recentFrameRate);
			msg.writeInt(new Date().getSeconds());
			msg.writeInt(messageID);
			msg.writeInt(lastReliablePingMs);
			msg.writeInt(0);
			this.RmiSend(HostID.Server, msg);
		};
		NetC2SProxy.prototype.ShutdownTcp = function (comment) {
			var msg = new ByteArrayBase();
			msg.writeByte(MessageType.Rmi);
			msg.writeShort(ProudC2S.Rmi_ShutdownTcp);
			if (comment) {
				msg.writeVLQ(comment.length);
				msg.writeUint8Array(comment.getArray());
			}
			else {
				msg.writeVLQ(0);
			}
			msg.writeInt(0);
			this.RmiSend(HostID.Server, msg);
		};

		// NetS2CStub
		NetS2CStub.prototype.ProcessReceivedMessage = function (msg) {
			try {
				var msgType = msg.readUnsignedShort();
			} catch (e) {
				return false;
			}
			if(globalVariables.enableLog)
				writeLog("msgType[" + msgType + "]", "NetS2CStub.ProcessReceivedMessage");
			switch (msgType) {
				case 63001:
				case 63002:
				case 63003:
				case 63004:
				case 63005:
				case 63006:
				case 63007:
				case 63011:
				case 63012:
				case 63015:
				case 63016:
				case 63017:
				case 63018:
				case 63019:
				case 63020:
					break;
				case ProudS2C.Rmi_ReliablePong:
					this.client.serverTcpLastPingMs = msg.readInt();
					var messageID = msg.readInt();
					Runtime.dynCall('vi', callbacks.onSynchronizeServerTime, [this.client.instanceID]);
					break;
				case ProudS2C.Rmi_EnableLog:
					globalVariables.enableLog = true;
					break;
				case ProudS2C.Rmi_DisableLog:
					globalVariables.enableLog = false;
					break;
				case ProudS2C.Rmi_ShutdownTcpAck:
					if(this.client.disconnectByServer == false)
					{
						this.client.errorType = ErrorType.DisconnectFromLocal;
						this.client.detailType = ErrorType.TCPConnectFailure;
						this.client.reason = "User called Disconnect()";
					}
					
					this.client.socketWrapper.socket.close();
					break;
				case ProudS2C.Rmi_RequestAutoPrune:
					this.client.errorType = ErrorType.DisconnectFromRemote;
					this.client.detailType = ErrorType.TCPConnectFailure;
					this.client.reason = "VIA-AP";
					
					this.client.disconnect(true);

					break;
				default:
					return false;
			}
			return true;
		};
		NetS2CStub.prototype.GetRmiIDList = function () {
			return this.rmiIDList;
		};
		NetS2CStub.prototype.GetRmiIDListCount = function () {
			return this.rmiIDList.length;
		};
		NetS2CStub.prototype.setClient = function (client) {
			this.client = client;
		};

		// ErrorInfo
		ErrorInfo.prototype.setValue = function (errorType, detailType, comment) {
			if (errorType) {
				this.errorType = errorType;
			}
			else {
				this.errorType = ErrorType.Ok;
			}
			if (detailType) {
				this.detailType = detailType;
			}
			else {
				this.detailType = ErrorType.Ok;
			}
			this.socketError = SocketErrorCode.Ok;
			this.remote = HostID.None;
			if (comment) {
				this.comment = comment;
			}
			else {
				this.comment = "";
			}
			this.lastReceivedMessage = new ByteArrayBase();
		};

		// ByteArrayBase
		Object.defineProperty(ByteArrayBase.prototype, "buffer", {
			get: function () {
				return this.data.buffer;
			},
			set: function (value) {
				this.data = new DataView(value);
			},
			enumerable: true,
			configurable: true
		});
		Object.defineProperty(ByteArrayBase.prototype, "dataView", {
			get: function () {
				return this.data;
			},
			set: function (value) {
				this.data = value;
				this.write_position = value.byteLength;
			},
			enumerable: true,
			configurable: true
		});
		Object.defineProperty(ByteArrayBase.prototype, "bufferOffset", {
			get: function () {
				return this.data.byteOffset;
			},
			enumerable: true,
			configurable: true
		});
		Object.defineProperty(ByteArrayBase.prototype, "position", {
			get: function () {
				return this._position;
			},
			set: function (value) {
				if (this._position < value) {
					if (!this.validate(this._position - value)) {
						return;
					}
				}
				this._position = value;
				this.write_position = value > this.write_position ? value : this.write_position;
			},
			enumerable: true,
			configurable: true
		});
		Object.defineProperty(ByteArrayBase.prototype, "length", {
			get: function () {
				return this.write_position;
			},
			set: function (value) {
				this.validateBuffer(value);
			},
			enumerable: true,
			configurable: true
		});
		Object.defineProperty(ByteArrayBase.prototype, "bytesAvailable", {
			get: function () {
				return this.data.byteLength - this._position;
			},
			enumerable: true,
			configurable: true
		});
		ByteArrayBase.prototype.clear = function () {
			this._position = 0;
		};
		ByteArrayBase.prototype.getArray = function () {
			if (this.array == null) {
				this.array = new Uint8Array(this.data.buffer, this.data.byteOffset, this.data.byteLength);
			}
			return this.array;
		};
		ByteArrayBase.prototype.setArray = function (array) {
			this.array = array;
			this.setBuffer(array.buffer, array.byteOffset, array.byteLength);
		};
		ByteArrayBase.prototype.setBuffer = function (buffer, offset, length) {
			if (offset === void 0) { offset = 0; }
			if (length === void 0) { length = 0; }
			if (buffer) {
				this.data = new DataView(buffer, offset, length > 0 ? length : buffer.byteLength);
				this.write_position = length > 0 ? length : buffer.byteLength;
			}
			else {
				this.write_position = 0;
			}
			this._position = 0;
		};
		ByteArrayBase.prototype.readVLQ = function () {
			var fillee = 0;
			var leftShiftOffset = 0;
			while (true) {
				var byte = this.readByte();
				if ((byte & 0x80) == 0) {
					fillee |= byte & 0x3f << leftShiftOffset;
					return fillee;
				}
				else {
					fillee |= byte & 0x7f << leftShiftOffset;
					leftShiftOffset += 7;
				}
			}
		};
		ByteArrayBase.prototype.writeVLQ = function (n) {
			while (true) {
				var byte = n & 0x7F;
				n >>= 7;
				if (n != 0) {
					this.writeByte(byte | 0x80);
				}
				else {
					if (byte & 0x40) {
						this.writeByte(byte | 0x80);
						byte = 0;
					}
					this.writeByte(byte);
					break;
				}
			}
		};
		ByteArrayBase.prototype.readBoolean = function () {
			if (!this.validate(ByteArrayBase.SIZE_OF_BOOLEAN))
				return null;
			return this.data.getUint8(this.position++) != 0;
		};
		ByteArrayBase.prototype.readByte = function () {
			if (!this.validate(ByteArrayBase.SIZE_OF_INT8))
				return null;
			return this.data.getInt8(this.position++);
		};
		ByteArrayBase.prototype.readInt = function () {
			if (!this.validate(ByteArrayBase.SIZE_OF_INT32))
				return null;
			var value = this.data.getInt32(this.position, this.endian == ByteArrayBase.BIG_ENDIAN);
			this.position += ByteArrayBase.SIZE_OF_INT32;
			return value;
		};
		ByteArrayBase.prototype.readShort = function () {
			if (!this.validate(ByteArrayBase.SIZE_OF_INT16))
				return null;
			var value = this.data.getInt16(this.position, this.endian == ByteArrayBase.BIG_ENDIAN);
			this.position += ByteArrayBase.SIZE_OF_INT16;
			return value;
		};
		ByteArrayBase.prototype.readUnsignedByte = function () {
			if (!this.validate(ByteArrayBase.SIZE_OF_UINT8))
				return null;
			return this.data.getUint8(this.position++);
		};
		ByteArrayBase.prototype.readUnsignedInt = function () {
			if (!this.validate(ByteArrayBase.SIZE_OF_UINT32))
				return null;
			var value = this.data.getUint32(this.position, this.endian == ByteArrayBase.BIG_ENDIAN);
			this.position += ByteArrayBase.SIZE_OF_UINT32;
			return value;
		};
		ByteArrayBase.prototype.readUnsignedShort = function () {
			if (!this.validate(ByteArrayBase.SIZE_OF_UINT16))
				return null;
			var value = this.data.getUint16(this.position, this.endian == ByteArrayBase.BIG_ENDIAN);
			this.position += ByteArrayBase.SIZE_OF_UINT16;
			return value;
		};
		ByteArrayBase.prototype.readStringTillNull = function (keepEvenByte) {
			if (keepEvenByte === void 0) { keepEvenByte = true; }
			var str = "";
			var num = 0;
			while (this.bytesAvailable > 0) {
				var _byte = this.data.getUint8(this.position++);
				num++;
				if (_byte != 0) {
					str += String.fromCharCode(_byte);
				}
				else {
					if (keepEvenByte && num % 2 != 0) {
						this.position++;
					}
					break;
				}
			}
			return str;
		};
		ByteArrayBase.prototype.writeBoolean = function (value) {
			this.validateBuffer(ByteArrayBase.SIZE_OF_BOOLEAN);
			this.data.setUint8(this.position++, value ? 1 : 0);
		};
		ByteArrayBase.prototype.writeByte = function (value) {
			this.validateBuffer(ByteArrayBase.SIZE_OF_INT8);
			this.data.setInt8(this.position++, value);
		};
		ByteArrayBase.prototype.writeUnsignedByte = function (value) {
			this.validateBuffer(ByteArrayBase.SIZE_OF_UINT8);
			this.data.setUint8(this.position++, value);
		};
		ByteArrayBase.prototype.writeBytes = function (_bytes, offset, length) {
			if (offset === void 0) { offset = 0; }
			if (length === void 0) { length = 0; }
			this.validateBuffer(length);
			var tmp_data = new DataView(_bytes.buffer);
			for (var i = 0; i < _bytes.length; i++) {
				this.data.setUint8(this.position++, tmp_data.getUint8(i));
			}
		};
		ByteArrayBase.prototype.writeDouble = function (value) {
			this.validateBuffer(ByteArrayBase.SIZE_OF_FLOAT64);
			this.data.setFloat64(this.position, value, this.endian == ByteArrayBase.BIG_ENDIAN);
			this.position += ByteArrayBase.SIZE_OF_FLOAT64;
		};
		ByteArrayBase.prototype.writeInt = function (value) {
			this.validateBuffer(ByteArrayBase.SIZE_OF_INT32);
			this.data.setInt32(this.position, value, this.endian == ByteArrayBase.BIG_ENDIAN);
			this.position += ByteArrayBase.SIZE_OF_INT32;
		};
		ByteArrayBase.prototype.writeShort = function (value) {
			this.validateBuffer(ByteArrayBase.SIZE_OF_INT16);
			this.data.setInt16(this.position, value, this.endian == ByteArrayBase.BIG_ENDIAN);
			this.position += ByteArrayBase.SIZE_OF_INT16;
		};
		ByteArrayBase.prototype.writeUnsignedShort = function (value) {
			this.validateBuffer(ByteArrayBase.SIZE_OF_UINT16);
			this.data.setUint16(this.position, value, this.endian == ByteArrayBase.BIG_ENDIAN);
			this.position += ByteArrayBase.SIZE_OF_UINT16;
		};
		ByteArrayBase.prototype.writeUnsignedInt = function (value) {
			this.validateBuffer(ByteArrayBase.SIZE_OF_UINT32);
			this.data.setUint32(this.position, value, this.endian == ByteArrayBase.BIG_ENDIAN);
			this.position += ByteArrayBase.SIZE_OF_UINT32;
		};
		ByteArrayBase.prototype.toString = function () {
			return "[ByteArrayBase] length:" + this.length + ", bytesAvailable:" + this.bytesAvailable;
		};
		ByteArrayBase.prototype.writeUint8Array = function (_bytes) {
			this.validateBuffer(this.position + _bytes.length);
			for (var i = 0; i < _bytes.length; i++) {
				this.data.setUint8(this.position++, _bytes[i]);
			}
		};
		ByteArrayBase.prototype.readUint8Array = function (length, createNewBuffer) {
			if (createNewBuffer === void 0) { createNewBuffer = true; }
			if (!this.validate(length))
				return null;
			if (!createNewBuffer) {
				var result = new Uint8Array(this.buffer, this.bufferOffset + this.position, length);
				this.position += length;
			}
			else {
				result = new Uint8Array(new ArrayBuffer(length));
				for (var i = 0; i < length; i++) {
					result[i] = this.data.getUint8(this.position);
					this.position += ByteArrayBase.SIZE_OF_UINT8;
				}
			}
			return result;
		};
		ByteArrayBase.prototype.validate = function (len) {
			if (this.data.byteLength > 0 && this._position + len <= this.data.byteLength) {
				return true;
			}
			else {
				throw "Error #2030: End of file was encountered.";
			}
		};
		ByteArrayBase.prototype.validateBuffer = function (len) {
			this.write_position = len > this.write_position ? len : this.write_position;
			if (this.data.byteLength < len) {
				var tmp = new Uint8Array(new ArrayBuffer(len + this.BUFFER_EXT_SIZE));
				tmp.set(new Uint8Array(this.data.buffer));
				this.data.buffer = tmp.buffer;
			}
		};
		ByteArrayBase.BIG_ENDIAN = "bigEndian";
		ByteArrayBase.LITTLE_ENDIAN = "littleEndian";
		ByteArrayBase.SIZE_OF_BOOLEAN = 1;
		ByteArrayBase.SIZE_OF_INT8 = 1;
		ByteArrayBase.SIZE_OF_INT16 = 2;
		ByteArrayBase.SIZE_OF_INT32 = 4;
		ByteArrayBase.SIZE_OF_INT64 = 8;
		ByteArrayBase.SIZE_OF_UINT8 = 1;
		ByteArrayBase.SIZE_OF_UINT16 = 2;
		ByteArrayBase.SIZE_OF_UINT32 = 4;
		ByteArrayBase.SIZE_OF_UINT64 = 8;
		ByteArrayBase.SIZE_OF_FLOAT32 = 4;
		ByteArrayBase.SIZE_OF_FLOAT64 = 8;
	},
	
	$base64ToUint8Array: function(base64encodedStr) {
		var byteCharacters = atob(base64encodedStr);
		var byteNumbers = new Array(byteCharacters.length);
		for(var i = 0; i < byteCharacters.length; i++) {
			byteNumbers[i] = byteCharacters.charCodeAt(i);
		}
		
		return new Uint8Array(byteNumbers);
	},
	
	$Uint8ArrayToBase64: function(arr) {
		var CHUNK_SZ = 0x8000;
		var c = [];
		for(var i = 0; i < arr.length; i += CHUNK_SZ)
			c.push(String.fromCharCode.apply(null, arr.subarray(i, i + CHUNK_SZ)));
		
		return btoa(c.join(""));
	},

/*
	$deepObjCopy: function(obj) {
		var copy = new Object();
		if(typeof(obj) == 'object') {
			if(typeof(obj.length) != 'undefined')
				var copy = new Array();
			for(var idx in obj) {
				if(typeof(obj[idx]) == 'object')
					copy[idx] = deepObjCopy(obj[idx]);
				else if(typeof(obj[idx]) == 'string' || typeof(obj[idx]) == 'number')
					copy[idx] = obj[idx];
				else if(typeof(obj[idx]) == 'boolean')
					((obj[idx] == true) ? copy[idx] = true : obj[idx] = false);
			}
		}
		return copy;
	},
*/
	$writeLog: function(content, caller) {
		var date = new Date();
		if(caller == undefined || caller.length == 0)
			caller = "";
		else
			caller = "[JS." + caller + "]";

		console.log(
			lpad(date.getFullYear(), 4) + "-" + lpad((date.getMonth() + 1), 2) + "-" + lpad(date.getDate(), 2) +
            " " +
            lpad(date.getHours(), 2) + ":" + lpad(date.getMinutes(), 2) + ":" + lpad(date.getSeconds(), 2) + "." + lpad(date.getMilliseconds(), 3) +
            " " + caller + " - " +
            content);
	},
	
	$lpad: function(n, width) {
		n = n + '';
		return n.length >= width ? n : new Array(width - n.length + 1).join('0') + n;
	},

	// C#에서 직접적으로 호출하지 않는 JS function에 대해서는 Unity Release빌드시 strip해버린다.
	// 해당 상황을 방지하기 위해서 CreateJSClient에서 dependency가 걸려있다고 알려줘야 strip되지 않고 실행시 오류가 나지 않는다.
	CreateJSClient__deps: 
		['$netClientInstances',
		'$callbacks',
		'$base64ToUint8Array',
		'$Uint8ArrayToBase64',
//		'$deepObjCopy',
		'$writeLog',
		'$lpad',
		'$SetEnvironment',
		'$WebSocketWrapper',
		'$Guid',
		'$RmiContext',
		'$NetClient',
		'$ProudC2S',
		'$ProudS2C',
		'$NetConnectionParam',
		'$NetC2SProxy',
		'$NetS2CStub',
		'$ErrorInfo',
		'$ByteArrayBase'],
	
	// 이 아래는 C#에서 호출하는 JS function들
	CreateJSClient: function(url, protocolVersion, userData, internalVersion) {
		SetEnvironment();
		var param = new NetConnectionParam();
		param.serverUrl = Pointer_stringify(url);
		
		var guidArr = new ByteArrayBase();
		guidArr.setArray(base64ToUint8Array(Pointer_stringify(protocolVersion)));
		
		param.protocolVersion = new Guid();
		param.protocolVersion.setValue(guidArr.readUnsignedInt(), guidArr.readUnsignedShort(), guidArr.readUnsignedShort(), [guidArr.readUnsignedByte(), guidArr.readUnsignedByte(), guidArr.readUnsignedByte(), guidArr.readUnsignedByte(), guidArr.readUnsignedByte(), guidArr.readUnsignedByte(), guidArr.readUnsignedByte(), guidArr.readUnsignedByte()]);
		
		param.userData = new ByteArrayBase();
		param.userData.setArray(userData == "" ? new Uint8Array(new ArrayBuffer(0)) : base64ToUint8Array(Pointer_stringify(userData)));
		
		globalVariables.internalVersion = internalVersion;
		
		var instanceID = netClientInstances.push(new NetClient()) - 1;
		netClientInstances[instanceID].instanceID = instanceID;
		netClientInstances[instanceID].connect(param);
		
		return instanceID;
	},
	
	JSSendUserMessage: function(instanceID, remotes, remoteLength, reliability, priority, encryptMode, base64encoded) {
		var hostIDArr = new Array();
		for(var i = 0; i < remoteLength; i++)
			hostIDArr.push(HEAP32[(remotes>>2) + i]);
		
		var payload = new ByteArrayBase();
		payload.setArray(base64ToUint8Array(Pointer_stringify(base64encoded)));
		
		var rmiContext = new RmiContext();
		rmiContext.setValue(reliability, priority, encryptMode);

		netClientInstances[instanceID].sendUserMessage(hostIDArr, payload, rmiContext);

		return true;
	},
	
	JSDisconnect: function(instanceID) {
		netClientInstances[instanceID].disconnect(false);
	},
	
	JSGarbageClient: function(instanceID) {
		netClientInstances[instanceID] = null;
	},

	JSRmiSend: function(instanceID, remotes, remoteLength, reliability, priority, encryptMode, base64encoded) {
		var hostIDArr = new Array();
		for(var i = 0; i < remoteLength; i++)
			hostIDArr.push(HEAP32[(remotes>>2) + i]);
		
		var payload = new ByteArrayBase();
		payload.setArray(base64ToUint8Array(Pointer_stringify(base64encoded)));

		var rmiContext = new RmiContext();
		rmiContext.setValue(reliability, priority, encryptMode);
		
		netClientInstances[instanceID].socketWrapper.sendToServer(hostIDArr, payload, rmiContext);
	},
	JSGetServerConnection: function(instanceID) {
		switch(netClientInstances[instanceID].socketWrapper.socket.readyState)
		{
			case WebSocket.CONNECTING:
			return 1;	// Connecting
			case WebSocket.OPEN:
			return 2;	// Connected
			case WebSocket.CLOSING:
			return 3;	// Disconnecting
			case WebSocket.CLOSED:
			return 0;	// Disconnected
		}
	}
};


autoAddDeps(WebGL, '$globalVariables');
autoAddDeps(WebGL, '$netClientInstances');
mergeInto(LibraryManager.library, WebGL);