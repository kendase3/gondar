// Copyright 2017 Neverware
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "newest_image_url.h"

#include <QNetworkReply>
#include <QString>
#include <QUrl>
#include "log.h"

static QUrl getLatestUrl(QNetworkReply* reply) {
  const char base[] = "https://ddnynf025unax.cloudfront.net/";
  const auto path = QString::fromUtf8(reply->readAll());
  return QUrl(base + path.trimmed());
}
NewestImageUrl::NewestImageUrl() {
  url32Ready = false;
  url64Ready = false;
}

void NewestImageUrl::fetch() {
  QString baseUrl(
      "https://s3.amazonaws.com/neverware-cloudready-free-releases/");
  // for the free version, we have to find out what the url is
  connect(&networkManager, &QNetworkAccessManager::finished, this,
          &NewestImageUrl::handleReply);
  networkManager.get(QNetworkRequest(QUrl(baseUrl + "latest-stable-64bit")));
  networkManager.get(QNetworkRequest(QUrl(baseUrl + "latest-stable-32bit")));
}

void NewestImageUrl::handleReply(QNetworkReply* reply) {
  const auto error = reply->error();
  if (error != QNetworkReply::NoError) {
    LOG_ERROR << "Error retrieving CloudReady Home Edition URL";
    emit errorOccurred();
    return;
  }
  // we find out which request this is a response for
  QUrl url = getLatestUrl(reply);
  if (reply->url().toString().contains("32bit")) {
    thirtyTwoUrl = url;
    url32Ready = true;
  } else {
    sixtyFourUrl = url;
    url64Ready = true;
  }
  LOG_INFO << "using latest url: " << url.toString();
  reply->deleteLater();
}

void NewestImageUrl::set32Url(QUrl url_in) {
  thirtyTwoUrl = url_in;
}

void NewestImageUrl::set64Url(QUrl url_in) {
  sixtyFourUrl = url_in;
}

bool NewestImageUrl::isReady() {
  if (thirtyTwoUrl.isEmpty() || sixtyFourUrl.isEmpty()) {
    return false;
  } else {
    return true;
  }
}

QUrl NewestImageUrl::get32Url() {
  return thirtyTwoUrl;
}

QUrl NewestImageUrl::get64Url() {
  return sixtyFourUrl;
}