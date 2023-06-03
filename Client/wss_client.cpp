/****************************************************************************
**
** Copyright (C) 2016 Kurt Pattyn <pattyn.kurt@gmail.com>.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtWebSockets module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include "wss_client.h"
#include <QtCore/QDebug>
#include <QtWebSockets/QWebSocket>
#include <QCoreApplication>

QT_USE_NAMESPACE

//! [constructor]
TWSS_Client::TWSS_Client(QObject *o, const QUrl &url, QObject *parent) :
    QObject(parent)
{
    Log("connecting to "+url.toDisplayString());
    messagecounter=0;

    connect(this,SIGNAL(MessageReceived(QString)),o,SLOT(on_WSS_MessageReceived(QString)));
    connect(this,SIGNAL(BinaryMessageReceived(QByteArray)),o,SLOT(on_WSS_BinaryMessageReceived(QByteArray)));

    connect(&m_webSocket, &QWebSocket::connected, this, &TWSS_Client::onConnected);
    connect(&m_webSocket, QOverload<const QList<QSslError>&>::of(&QWebSocket::sslErrors),
            this, &TWSS_Client::onSslErrors);

    QObject::connect(&m_webSocket, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(onError(QAbstractSocket::SocketError)));
    QObject::connect(&m_webSocket, SIGNAL(preSharedKeyAuthenticationRequired(QSslPreSharedKeyAuthenticator*)),
                                                this, SLOT(onpreSharedKeyAuthenticationRequired(QSslPreSharedKeyAuthenticator*)));

    Log("connecting server "+url.toDisplayString() );

    m_webSocket.open(url);

    Log("done starting connector");

}
//! [constructor]
//!

void TWSS_Client::onpreSharedKeyAuthenticationRequired(QSslPreSharedKeyAuthenticator *authenticator)
{
    Log("onpreSharedKeyAuthenticationRequired");
}

void TWSS_Client::onError(QAbstractSocket::SocketError error)
{
    Log("onError "+m_webSocket.errorString());
}

//! [onConnected]
void TWSS_Client::onConnected()
{
    Log("wss_client connected");

    connect(&m_webSocket, &QWebSocket::textMessageReceived,
            this, &TWSS_Client::onTextMessageReceived);
    connect(&m_webSocket, &QWebSocket::binaryMessageReceived, this, &TWSS_Client::onBinaryMessageReceived);

    //Inform Launcher about connect
    QJsonObject jsonObj;
    jsonObj["MessageType"] = "Internal";
    jsonObj["Msg"] = "connected";
    QJsonDocument jsondoc=QJsonDocument(jsonObj);
    emit BinaryMessageReceived(jsondoc.toJson());
}
//! [onConnected]

//! [onTextMessageReceived]
void TWSS_Client::onTextMessageReceived(QString message)
{
    Log("Message received: " + message);
    emit MessageReceived(message);
}
//! [onTextMessageReceived]

//! [processBinaryMessage]
void TWSS_Client::onBinaryMessageReceived(QByteArray message)
{
        Log("Binary Message received: "+message);
        emit BinaryMessageReceived(message);
}
//! [processBinaryMessage]

//! [onSslErrors]
void TWSS_Client::onSslErrors(const QList<QSslError> &errors)
{
    QList<QSslError>::ConstIterator e = errors.constBegin();
    for(e=errors.begin();e!=errors.end();e++)
    Log("SSLError "+e->errorString());
}
//! [onSslErrors]

void TWSS_Client::SendJsonDocument(QJsonDocument *doc)
{
    m_webSocket.sendBinaryMessage(doc->toJson());
}



