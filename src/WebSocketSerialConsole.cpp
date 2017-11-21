#include "WebSocketSerialConsole.hpp"

WSSerialConsole::WSSerialConsole(WebServer& svr, WSServer& ws)
: _ws(ws), _client(-1) {
  svr.on("/console", [this, &svr]() { _returnConsolePage(svr); });
  ws.setHandler([this](WSServer& ws, uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
    _socketEvent(ws, num, type, payload, length);
  });
}

void WSSerialConsole::run() {
  if (_client != -1)
    _serialEvent();
}

void WSSerialConsole::_returnConsolePage(WebServer& svr) {
  svr.send(200, "text/html", F("<!DOCTYPE html><meta charset='utf-8' /><style>p{white-space:pre;word-wrap:break-word;font-family:monospace;}</style><title>Serial Console</title><script language='javascript' type='text/javascript'>var b='ws://'+location.hostname+':81/',c,d,e;function g(){d=new WebSocket(b);d.onopen=function(){h('[connected]')};d.onclose=function(){h('[disconnected]')};d.onmessage=function(a){h('<span style=\"color: blue;\">[response] '+a.data+'</span>')};d.onerror=function(a){h('<span style=\"color: red;\">[error] </span> '+a.data)}}\nfunction k(a){if(13==a.keyCode){a=e.value;if('/disconnect'==a)d.close();else if('/clear'==a)for(;c.firstChild;)c.removeChild(c.firstChild);else''!=a&&(h('[sent] '+a),d.send(a));e.value='';e.focus()}}function h(a){var f=document.createElement('p');f.innerHTML=a;c.appendChild(f);window.scrollTo(0,document.body.scrollHeight)}\nwindow.addEventListener('load',function(){c=document.getElementById('c');e=document.getElementById('i');g();document.getElementById('i').addEventListener('keyup',k,!1);e.focus()},!1);</script><h2>Serial Console</h2><div id='c'></div><input type='text' id='i' style=\"width:100%;font-family:monospace;\">\n"));
}

void WSSerialConsole::_socketEvent(WSServer& ws, uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      _client = -1;
      break;
    case WStype_CONNECTED:
      if (_client != -1)
        ws.disconnect(_client);
      _client = num;
      ws.sendTXT(num, "Connected to serial console");
      // reset serial buffer on new connection to prevent garbage
      _serialBuffer[0] = '\0';
      _bufferSize = 0;
      break;
    case WStype_TEXT:
    case WStype_BIN:
      Serial.write(payload, length); // send incoming data to serial
      break;
  }
}


void WSSerialConsole::_serialEvent() {
  while (Serial.available() > 0) {
    uint32_t in = Serial.read();
    // there is no proper utf-8 support so replace all non-ascii
    // characters (<127) with underscores; this should have no
    // impact on normal operations and is only relevant for non-english
    // plain-text error messages
    if (in > 127) {
      in = '_';
    }
    _serialBuffer[_bufferSize] = in & 0xFF;
    _bufferSize++;
    // fill up the serial buffer until its max size (8192 bytes, see serialBufferSize)
    // or unitl the end of file marker (ctrl-z; \x1A) is reached
    if (_bufferSize > serialBufferSize - 1 || in == '\x1A') {
      _serialBuffer[_bufferSize] = '\0';
      bool allSend = false;
      uint8_t localBuffer[1464];
      int localNum = 0;
      int bufferNum = 0;
      while (!allSend) {
        localBuffer[localNum] = _serialBuffer[bufferNum];
        localNum++;
        // split the websocket packet in smaller (1300 + x) byte packets
        // this is a workaround for some issue that causes data corruption
        // if the payload is split by the wifi library into more than 2 tcp packets
        if (_serialBuffer[bufferNum] == '\x1A' || (_serialBuffer[bufferNum] == '\n' && localNum > 1300)) {
          localBuffer[localNum] = '\0';
          localNum = 0;
          _ws.sendTXT(_client, localBuffer);
        }
        if (_serialBuffer[bufferNum] == '\x1A') {
          allSend = true;
        }
        bufferNum++;
      }
      _serialBuffer[0] = '\0';
      _bufferSize = 0;
    }
  }
}
