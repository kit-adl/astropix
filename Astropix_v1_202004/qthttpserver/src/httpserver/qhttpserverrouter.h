/****************************************************************************
**
** Copyright (C) 2019 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtHttpServer module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QHTTPSERVERROUTER_H
#define QHTTPSERVERROUTER_H

#include <qthttpserverglobal.h>
#include <qhttpserverrouterviewtraits.h>

#include <QtCore/qscopedpointer.h>
#include <QtCore/qmetatype.h>
#include <QtCore/qregularexpression.h>

#include <functional>
#include <initializer_list>

QT_BEGIN_NAMESPACE

namespace QtPrivate {
    template<int> struct QHttpServerRouterPlaceholder {};
}

QT_END_NAMESPACE

namespace std {

template<int N>
struct is_placeholder<QT_PREPEND_NAMESPACE(QtPrivate::QHttpServerRouterPlaceholder<N>)> :
    integral_constant<int , N + 1>
{};

}

QT_BEGIN_NAMESPACE

class QTcpSocket;
class QHttpServerRequest;
class QHttpServerRouterRule;

class QHttpServerRouterPrivate;
class Q_HTTPSERVER_EXPORT QHttpServerRouter
{
    Q_DECLARE_PRIVATE(QHttpServerRouter)

public:
    QHttpServerRouter();
    ~QHttpServerRouter();

    template<typename Type>
    bool addConverter(const QLatin1String &regexp) {
        static_assert(QMetaTypeId2<Type>::Defined,
                      "Type is not registered with Qt's meta-object system: "
                      "please apply Q_DECLARE_METATYPE() to it");

        if (!QMetaType::registerConverter<QString, Type>())
            return false;

        addConverter(qMetaTypeId<Type>(), regexp);
        return true;
    }

    void addConverter(const int type, const QLatin1String &regexp);
    void removeConverter(const int);
    void clearConverters();
    const QMap<int, QLatin1String> &converters() const;

    static const QMap<int, QLatin1String> &defaultConverters();

    template<typename ViewHandler, typename ViewTraits = QHttpServerRouterViewTraits<ViewHandler>>
    bool addRule(QHttpServerRouterRule *rule)
    {
        return addRuleHelper<ViewTraits>(
                rule,
                typename QtPrivate::Indexes<ViewTraits::ArgumentCount>::Value{});
    }

    template<typename ViewHandler, typename ViewTraits = QHttpServerRouterViewTraits<ViewHandler>>
    typename ViewTraits::BindableType bindCaptured(ViewHandler &&handler,
                      QRegularExpressionMatch &match) const
    {
        return bindCapturedImpl<ViewHandler, ViewTraits>(
                std::forward<ViewHandler>(handler),
                match,
                typename QtPrivate::Indexes<ViewTraits::ArgumentCapturableCount>::Value{},
                typename QtPrivate::Indexes<ViewTraits::ArgumentPlaceholdersCount>::Value{});
    }

    bool handleRequest(const QHttpServerRequest &request,
                       QTcpSocket *socket) const;

private:
    template<typename ViewTraits, int ... Idx>
    bool addRuleHelper(QHttpServerRouterRule *rule,
                       QtPrivate::IndexesList<Idx...>)
    {
        const std::initializer_list<int> types = {ViewTraits::template Arg<Idx>::metaTypeId()...};
        return addRuleImpl(rule, types);
    }

    bool addRuleImpl(QHttpServerRouterRule *rule,
                     const std::initializer_list<int> &metaTypes);

    template<typename ViewHandler, typename ViewTraits, int ... Cx, int ... Px>
    typename ViewTraits::BindableType bindCapturedImpl(ViewHandler &&handler,
                          QRegularExpressionMatch &match,
                          QtPrivate::IndexesList<Cx...>,
                          QtPrivate::IndexesList<Px...>) const
    {

        return std::bind(
                std::forward<ViewHandler>(handler),
                QVariant(match.captured(Cx + 1)).value<typename ViewTraits::template Arg<Cx>::Type>()...,
                QtPrivate::QHttpServerRouterPlaceholder<Px>{}...);
    }


    QScopedPointer<QHttpServerRouterPrivate> d_ptr;
};

QT_END_NAMESPACE

#endif // QHTTPSERVERROUTER_H
