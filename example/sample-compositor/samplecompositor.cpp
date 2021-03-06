#include <QProcess>
#include <QtQml>
#include <QJSValue>

#include "samplecompositor.h"

SampleCompositor *SampleCompositor::s_instance = Q_NULLPTR;

QObject *createCompositorAppSingleton(QQmlEngine *, QJSEngine *)
{
    return SampleCompositor::instance();
}

SampleCompositor::SampleCompositor(QObject *parent) : QObject(parent),
    m_process1(Q_NULLPTR),
    m_process2(Q_NULLPTR)
{
    qmlRegisterSingletonType<SampleCompositor>("compositor", 1, 0, "SampleCompositor", createCompositorAppSingleton);

    m_quickView.setSource(QStringLiteral("qrc:/qmlfiles/main.qml"));
    m_quickView.show();
    // we only create one instance of this class anyhow, so no need
    // to implement a proper singleton
    s_instance = this;

    connect(&m_qnxCompositor, &QnxCompositor::windowCreated,
            this, &SampleCompositor::windowCreated);
    connect(&m_qnxCompositor, &QnxCompositor::windowDeleted,
            this, &SampleCompositor::windowDeleted);
}

void SampleCompositor::startApp(const QString &appName)
{
    qWarning() << Q_FUNC_INFO << "starting" << appName;
    if (m_process1 == Q_NULLPTR) {
        m_process1 = new QProcess(this);
        m_process1->start(appName);
    } else {
        m_process2 = new QProcess(this);
        m_process2->start(appName);
    }
}

void SampleCompositor::windowCreated(void *window)
{
    qDebug() << Q_FUNC_INFO;
    int size[2];
    size[0] = 984;
    size[1] = 440;
    const screen_window_t nativeWindow = reinterpret_cast<screen_window_t>(window);
    screen_set_window_property_iv(nativeWindow, SCREEN_PROPERTY_SIZE, size);

    int position[2];
    position[0] = 20;
    position[1] = 140;
    screen_set_window_property_iv(nativeWindow, SCREEN_PROPERTY_POSITION, position);
}

void SampleCompositor::windowDeleted(void *window)
{
    Q_UNUSED(window);
}

SampleCompositor *SampleCompositor::instance()
{
    return s_instance;
}
